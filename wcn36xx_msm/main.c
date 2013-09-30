/*
 * Copyright (c) 2013 Eugene Krasnikov <k.eugene.e@gmail.com>
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/completion.h>
#include <linux/firmware.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/wcnss_wlan.h>
#include <linux/workqueue.h>
#include <mach/msm_smd.h>
#include "../wcn36xx.h"

#define MAC_ADDR_0 "wlan/macaddr0"

struct wcn36xx_msm {
	struct wcn36xx_platform_ctrl_ops ctrl_ops;
	struct platform_device *core;
	void *drv_priv;
	void (*rsp_cb)(void *drv_priv, void *buf, size_t len);
	/* SMD related */
	struct workqueue_struct	*wq;
	struct work_struct	smd_work;
	struct completion	smd_compl;
	smd_channel_t		*smd_ch;
} wmsm;

static int wcn36xx_msm_smsm_change_state(u32 clear_mask, u32 set_mask)
{
	 return smsm_change_state(SMSM_APPS_STATE, clear_mask, set_mask);
}

static int wcn36xx_msm_get_hw_mac(u8 *addr)
{
	const struct firmware *addr_file = NULL;
	int status;
	u8 tmp[18];
	static const u8 qcom_oui[3] = {0x00, 0x0A, 0xF5};
	static const char *files = {MAC_ADDR_0};

	status = request_firmware(&addr_file, files, &wmsm.core->dev);

	if (status) {
		/* Assign a random mac with Qualcomm oui */
		dev_err(&wmsm.core->dev, "Failed to read macaddress file %s, using a random address instead",
			     files);
		memcpy(addr, qcom_oui, 3);
		get_random_bytes(addr + 3, 3);
	} else {
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, addr_file->data, sizeof(tmp) - 1);
		sscanf(tmp, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
		       &addr[0],
		       &addr[1],
		       &addr[2],
		       &addr[3],
		       &addr[4],
		       &addr[5]);

		release_firmware(addr_file);
	}

	return 0;
}

static int wcn36xx_msm_smd_send_and_wait(char *buf, size_t len)
{
	int avail;
	int ret = 0;

	avail = smd_write_avail(wmsm.smd_ch);

	if (avail >= len) {
		avail = smd_write(wmsm.smd_ch, buf, len);
		if (avail != len) {
			dev_err(&wmsm.core->dev,
				"Cannot write to SMD channel\n");
			ret = -EAGAIN;
			goto out;
		}
	} else {
		dev_err(&wmsm.core->dev,
			"SMD channel can accept only %d bytes\n", avail);
		ret = -ENOMEM;
		goto out;
	}

out:
	return ret;
}

static void wcn36xx_msm_smd_notify(void *data, unsigned event)
{
	struct wcn36xx_msm *wmsm_priv = (struct wcn36xx_msm *)data;

	switch (event) {
	case SMD_EVENT_OPEN:
		complete(&wmsm_priv->smd_compl);
		break;
	case SMD_EVENT_DATA:
		queue_work(wmsm_priv->wq, &wmsm_priv->smd_work);
		break;
	case SMD_EVENT_CLOSE:
		break;
	case SMD_EVENT_STATUS:
		break;
	case SMD_EVENT_REOPEN_READY:
		break;
	default:
		dev_err(&wmsm_priv->core->dev,
			"SMD_EVENT (%d) not supported\n", event);
		break;
	}
}

static void wcn36xx_msm_smd_work(struct work_struct *work)
{
	int avail;
	int msg_len;
	void *msg;
	int ret;
	struct wcn36xx_msm *wmsm_priv =
		container_of(work, struct wcn36xx_msm, smd_work);

	while (1) {
		msg_len = smd_cur_packet_size(wmsm_priv->smd_ch);
		if (0 == msg_len) {
			return;
		}
		avail = smd_read_avail(wmsm_priv->smd_ch);
		if (avail < msg_len) {
			return;
		}
		msg = kmalloc(msg_len, GFP_KERNEL);
		if (NULL == msg) {
			return;
		}

		ret = smd_read(wmsm_priv->smd_ch, msg, msg_len);
		if (ret != msg_len) {
			return;
		}
		wmsm_priv->rsp_cb(wmsm_priv->drv_priv, msg, msg_len);
		kfree(msg);
	}
}

int wcn36xx_msm_smd_open(void *drv_priv, void *rsp_cb)
{
	int ret, left;
	wmsm.drv_priv = drv_priv;
	wmsm.rsp_cb = rsp_cb;
	INIT_WORK(&wmsm.smd_work, wcn36xx_msm_smd_work);
	init_completion(&wmsm.smd_compl);

	wmsm.wq = create_workqueue("wcn36xx_msm_smd_wq");
	if (!wmsm.wq) {
		dev_err(&wmsm.core->dev, "failed to allocate wq");
		ret = -ENOMEM;
		return ret;
	}

	ret = smd_named_open_on_edge("WLAN_CTRL", SMD_APPS_WCNSS,
		&wmsm.smd_ch, &wmsm, wcn36xx_msm_smd_notify);
	if (ret) {
		dev_err(&wmsm.core->dev,
			"smd_named_open_on_edge failed: %d\n", ret);
		return ret;
	}

	left = wait_for_completion_interruptible_timeout(&wmsm.smd_compl,
		msecs_to_jiffies(HAL_MSG_TIMEOUT));
	if (left <= 0) {
		dev_err(&wmsm.core->dev,
			"timeout waiting for smd open: %d\n", ret);
		return left;
	}

	/* Not to receive INT until the whole buf from SMD is read */
	smd_disable_read_intr(wmsm.smd_ch);

	return 0;
}

void wcn36xx_msm_smd_close(void)
{
	smd_close(wmsm.smd_ch);
	flush_workqueue(wmsm.wq);
	destroy_workqueue(wmsm.wq);
}

static int __init wcn36xx_msm_init(void)
{
	int ret;
	struct resource *wcnss_memory;
	struct resource *tx_irq;
	struct resource *rx_irq;
	struct resource res[3];
	wmsm.core = platform_device_alloc("wcn36xx", -1);

	memset(res, 0x00, sizeof(res));
	wmsm.ctrl_ops.open = wcn36xx_msm_smd_open;
	wmsm.ctrl_ops.close = wcn36xx_msm_smd_close;
	wmsm.ctrl_ops.tx = wcn36xx_msm_smd_send_and_wait;
	wmsm.ctrl_ops.get_hw_mac = wcn36xx_msm_get_hw_mac;
	wmsm.ctrl_ops.smsm_change_state = wcn36xx_msm_smsm_change_state;
	wcnss_memory =
		platform_get_resource_byname(wcnss_get_platform_device(),
					      IORESOURCE_MEM,
					      "wcnss_mmio");
	if (wcnss_memory == NULL) {
		dev_err(&wmsm.core->dev,
			"Failed to get wcnss wlan memory map.\n");
		ret = -ENOMEM;
		return ret;
	}
	memcpy(&res[0], wcnss_memory, sizeof(*wcnss_memory));

	tx_irq = platform_get_resource_byname(wcnss_get_platform_device(),
					      IORESOURCE_IRQ,
					      "wcnss_wlantx_irq");
	if (tx_irq == NULL) {
		dev_err(&wmsm.core->dev, "Failed to get wcnss tx_irq");
		ret = -ENOMEM;
		return ret;
	}
	memcpy(&res[1], tx_irq, sizeof(*tx_irq));

	rx_irq = platform_get_resource_byname(wcnss_get_platform_device(),
					      IORESOURCE_IRQ,
					      "wcnss_wlanrx_irq");
	if (rx_irq == NULL) {
		dev_err(&wmsm.core->dev, "Failed to get wcnss rx_irq");
		ret = -ENOMEM;
		return ret;
	}
	memcpy(&res[2], rx_irq, sizeof(*rx_irq));

	platform_device_add_resources(wmsm.core, res, ARRAY_SIZE(res));

	ret = platform_device_add_data(wmsm.core, &wmsm.ctrl_ops,
				       sizeof(wmsm.ctrl_ops));
	if (ret) {
		dev_err(&wmsm.core->dev, "Can't add platform data\n");
		ret = -ENOMEM;
		return ret;
	}

	platform_device_add(wmsm.core);
	return 0;
}
static void __exit wcn36xx_msm_exit(void)
{
	platform_device_del(wmsm.core);
	platform_device_put(wmsm.core);
}
module_init(wcn36xx_msm_init);
module_exit(wcn36xx_msm_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eugene Krasnikov k.eugene.e@gmail.com");
MODULE_FIRMWARE(MAC_ADDR_0);
