/*
 * Copyright (c) 2013 Eugene Krasnikov <k.eugene.e@gmail.com>
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

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/wcnss_wlan.h>

static struct platform_device *core;

static int __init wcn36xx_msm_init(void)
{
	int ret;
	struct resource *wcnss_memory;
	struct resource *tx_irq;
	struct resource *rx_irq;
	struct resource res[3];
	core = platform_device_alloc("wcn36xx", -1);

	memset(res, 0x00, sizeof(res));

	wcnss_memory =
		platform_get_resource_byname(wcnss_get_platform_device(),
					      IORESOURCE_MEM,
					      "wcnss_mmio");
	if (wcnss_memory == NULL) {
		dev_err(&core->dev, "Failed to get wcnss wlan memory map.\n");
		ret = -ENOMEM;
		return ret;
	}
	memcpy(&res[0], wcnss_memory, sizeof(*wcnss_memory));

	tx_irq = platform_get_resource_byname(wcnss_get_platform_device(),
					      IORESOURCE_IRQ,
					      "wcnss_wlantx_irq");
	if (tx_irq == NULL) {
		dev_err(&core->dev, "Failed to get wcnss tx_irq");
		ret = -ENOMEM;
		return ret;
	}
	memcpy(&res[1], tx_irq, sizeof(*tx_irq));

	rx_irq = platform_get_resource_byname(wcnss_get_platform_device(),
					      IORESOURCE_IRQ,
					      "wcnss_wlanrx_irq");
	if (rx_irq == NULL) {
		dev_err(&core->dev, "Failed to get wcnss rx_irq");
		ret = -ENOMEM;
		return ret;
	}
	memcpy(&res[2], rx_irq, sizeof(*rx_irq));

	platform_device_add_resources(core, res, ARRAY_SIZE(res));
	platform_device_add(core);
	return 0;
}
static void __exit wcn36xx_msm_exit(void)
{
	platform_device_del(core);
	platform_device_put(core);
}
module_init(wcn36xx_msm_init);
module_exit(wcn36xx_msm_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eugene Krasnikov k.eugene.e@gmail.com");
