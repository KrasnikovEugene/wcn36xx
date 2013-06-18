/*
 * Contact: Eugene Krasnikov <k.eugene.e@gmail.com>
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

#ifndef _WCN36XX_H_
#define _WCN36XX_H_

#include <linux/completion.h>
#include <linux/printk.h>
#include <linux/firmware.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <mach/msm_smd.h>
#include <net/mac80211.h>

#include "smd.h"
#include "dxe.h"
#include "hal.h"

#define DRIVER_PREFIX "wcn36xx: "
#define WLAN_NV_FILE               "wlan/prima/WCNSS_qcom_wlan_nv.bin"
#define MAC_ADDR_0 "wlan/macaddr0"

extern unsigned int debug_mask;

enum wcn36xx_debug_mask {
	WCN36XX_DBG_DXE		= 0x00000001,
	WCN36XX_DBG_DXE_DUMP	= 0x00000002,
	WCN36XX_DBG_SMD		= 0x00000004,
	WCN36XX_DBG_SMD_DUMP	= 0x00000008,
	WCN36XX_DBG_RX		= 0x00000010,
	WCN36XX_DBG_RX_DUMP	= 0x00000020,
	WCN36XX_DBG_TX		= 0x00000040,
	WCN36XX_DBG_TX_DUMP	= 0x00000080,
	WCN36XX_DBG_HAL		= 0x00000100,
	WCN36XX_DBG_HAL_DUMP	= 0x00000200,
	WCN36XX_DBG_MAC		= 0x00000400,
	WCN36XX_DBG_BEACON	= 0x00000800,
	WCN36XX_DBG_BEACON_DUMP	= 0x00001000,
	WCN36XX_DBG_ANY		= 0xffffffff,
};

#define wcn36xx_error(fmt, arg...) do {			\
	pr_err(DRIVER_PREFIX "ERROR " fmt "\n", ##arg);	\
	__WARN();					\
} while (0)

#define wcn36xx_warn(fmt, arg...)				\
	pr_warn(DRIVER_PREFIX "WARNING " fmt "\n", ##arg)

#define wcn36xx_info(fmt, arg...)		\
	pr_info(DRIVER_PREFIX fmt "\n", ##arg)

#define wcn36xx_dbg(mask, fmt, arg...) do {			\
	if (debug_mask & mask)					\
		pr_debug(DRIVER_PREFIX fmt "\n", ##arg);	\
} while (0)

#define wcn36xx_dbg_dump(mask, prefix_str, buf, len) do {	\
	if (debug_mask & mask)					\
		print_hex_dump(KERN_DEBUG, prefix_str,		\
			       DUMP_PREFIX_ADDRESS, 32, 1,	\
			       buf, len, false);		\
} while (0)

enum wcn36xx_encryption_state {
	WCN36XX_STA_KEY,
	WCN36XX_BSS_KEY
};

static inline void buff_to_be(u32 *buf, size_t len)
{
	int i;
	for (i = 0; i < len; i++)
		buf[i] = cpu_to_be32(buf[i]);
}
struct nv_data {
	int	is_valid;
	void	*table;
};
struct wcn36xx_vif {
	u8 sta_index;
	u8 dpu_desc_index;
	u8 ucast_dpu_signature;
	/* Returned from WCN36XX_HAL_ADD_STA_SELF_RSP */
	u8 self_sta_index;
	u8 self_dpu_desc_index;
};
struct wcn_sta {
	u8 sta_id;
};
struct wcn36xx_dxe_ch;
struct wcn36xx {
	struct ieee80211_hw	*hw;
	struct workqueue_struct	*wq;
	struct device		*dev;
	const struct firmware	*nv;
	struct mac_address	addresses[2];
	int			ch;
	struct ieee80211_channel *current_channel;
	enum ieee80211_band	band;
	struct wcn36xx_hal_mac_ssid ssid;
	enum nl80211_iftype	iftype;
	u16			aid;
	struct wcn36xx_vif	*current_vif;
	u16			beacon_interval;

	u8			fw_revision;
	u8			fw_version;
	u8			fw_minor;
	u8			fw_major;
	enum wcn36xx_encryption_state	en_state;

	/* extra byte for the NULL termination */
	u8			crm_version[WCN36XX_HAL_VERSION_LENGTH + 1];
	u8			wlan_version[WCN36XX_HAL_VERSION_LENGTH + 1];

	bool			beacon_enable;
	/* IRQs */
	int			tx_irq;
	int			rx_irq;
	void __iomem		*mmio;

	/* Rates */
	struct wcn36xx_hal_supported_rates supported_rates;

	/* SMD related */
	smd_channel_t		*smd_ch;
	u8			*smd_buf;
	struct work_struct	smd_work;
	struct work_struct	start_work;
	struct work_struct	rx_ready_work;
	struct completion	smd_compl;

	bool			is_joining;

	/* DXE channels */
	struct wcn36xx_dxe_ch	dxe_tx_l_ch;	/* TX low */
	struct wcn36xx_dxe_ch	dxe_tx_h_ch;	/* TX high */
	struct wcn36xx_dxe_ch	dxe_rx_l_ch;	/* RX low */
	struct wcn36xx_dxe_ch	dxe_rx_h_ch;	/* RX high */

	/* For synchronization of DXE resources from BH, IRQ and WQ contexts */
	spinlock_t	dxe_lock;

	/* Memory pools */
	struct wcn36xx_dxe_mem_pool mgmt_mem_pool;
	struct wcn36xx_dxe_mem_pool data_mem_pool;

	struct sk_buff		*tx_ack_skb;
};

#endif	/* _WCN36XX_H_ */
