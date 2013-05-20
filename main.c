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

#include <linux/module.h>
#include <linux/qcomwlan_pwrif.h>
#include <linux/platform_device.h>
#include <linux/wcnss_wlan.h>
#include <linux/etherdevice.h>
#include "wcn36xx.h"
#include "dxe.h"
#include "hal.h"

unsigned int debug_mask;
module_param(debug_mask, uint, 0644);
MODULE_PARM_DESC(debug_mask, "Debugging mask");

/*
 * provide hw to module exit function
 *
 * FIXME: implement this properly, maybe with platform device?
 */
static struct ieee80211_hw *private_hw;

static int wcn36xx_start(struct ieee80211_hw *hw)
{
	struct wcn36xx *wcn = hw->priv;
	int ret;

	ENTER();

	// SMD initialization
	ret = wcn36xx_smd_open(wcn);
	if (ret) {
		wcn36xx_error("failed to open smd channel: %d", ret);
		return ret;
	}

	// Not to receive INT untill the whole buf from SMD is read
	smd_disable_read_intr(wcn->smd_ch);

	// Allocate memory pools for Mgmt BD headers and Data BD headers
	wcn36xx_dxe_allocate_mem_pools(wcn);
	wcn36xx_dxe_alloc_ctl_blks(wcn);

	INIT_WORK(&wcn->rx_ready_work, wcn36xx_rx_ready_work);

	ret = request_firmware(&wcn->nv, WLAN_NV_FILE, wcn->dev);
	if (ret) {
		//TODO error handling
		wcn36xx_error("request FM %d", ret);
	}
	// maximu SMD message size is 4k
	wcn->smd_buf = kmalloc(4096, GFP_KERNEL);

	//TODO pass configuration to FW
	wcn36xx_smd_load_nv(wcn);
	wcn36xx_smd_start(wcn);

	// DMA chanel initialization
	wcn36xx_dxe_init(wcn);

	return 0;
}
static void wcn36xx_stop(struct ieee80211_hw *hw)
{
	struct wcn36xx *wcn = hw->priv;

	ENTER();

	wcn36xx_dxe_deinit(wcn);
	wcn36xx_smd_close(wcn);

	kfree(wcn->smd_buf);
}

static void wcn36xx_remove_interface(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif)
{
	ENTER();
}

static int wcn36xx_change_interface(struct ieee80211_hw *hw,
				      struct ieee80211_vif *vif,
				      enum nl80211_iftype new_type, bool p2p)
{
	ENTER();
	return 0;
}

static int wcn36xx_config(struct ieee80211_hw *hw, u32 changed)
{
	struct wcn36xx *wcn = hw->priv;
	ENTER();
	if (changed & IEEE80211_CONF_CHANGE_CHANNEL) {
		wcn->ch = ieee80211_frequency_to_channel(hw->conf.chandef.chan->center_freq);
		wcn36xx_info("wcn36xx_config channel switch=%d", wcn->ch);
		if (wcn->is_scanning) {
			if (wcn->prev_channel) {
				wcn36xx_smd_end_scan(wcn, wcn->prev_channel);
			}
			wcn36xx_smd_start_scan(wcn, wcn->ch);
			wcn->prev_channel = wcn->ch;
		}
	}

	return 0;
}

static u64 wcn36xx_prepare_multicast(struct ieee80211_hw *hw,
				       struct netdev_hw_addr_list *mc_list)
{
	ENTER();
	return 0;
}
#define WCN36XX_SUPPORTED_FILTERS (FIF_PROMISC_IN_BSS | \
				  FIF_ALLMULTI | \
				  FIF_FCSFAIL | \
				  FIF_BCN_PRBRESP_PROMISC | \
				  FIF_CONTROL | \
				  FIF_OTHER_BSS)
static void wcn36xx_configure_filter(struct ieee80211_hw *hw,
				       unsigned int changed,
				       unsigned int *total, u64 multicast)
{
	ENTER();
	changed &= WCN36XX_SUPPORTED_FILTERS;
	*total &= WCN36XX_SUPPORTED_FILTERS;
}

static void wcn36xx_tx(struct ieee80211_hw *hw,  struct ieee80211_tx_control *control, struct sk_buff *skb)
{
	struct ieee80211_mgmt *mgmt;
	ENTER();
	mgmt = (struct ieee80211_mgmt *)skb->data;
	wcn36xx_dbg(WCN36XX_DBG_TX, "wcn36xx_tx: = %x", mgmt->frame_control);
	if (ieee80211_is_data(mgmt->frame_control) || (ieee80211_is_data_qos(mgmt->frame_control))) {
		wcn36xx_dxe_tx(hw->priv, skb, is_broadcast_ether_addr(mgmt->da) || is_multicast_ether_addr(mgmt->da), false);
	} else {
		wcn36xx_dxe_tx(hw->priv, skb, is_broadcast_ether_addr(mgmt->da) || is_multicast_ether_addr(mgmt->da), true);
	}
}

static int wcn36xx_set_key(struct ieee80211_hw *hw, enum set_key_cmd cmd,
			     struct ieee80211_vif *vif,
			     struct ieee80211_sta *sta,
			     struct ieee80211_key_conf *key_conf)
{
	ENTER();
	return 0;
}

static void wcn36xx_sw_scan_start(struct ieee80211_hw *hw)
{
	struct wcn36xx *wcn = hw->priv;
	ENTER();
	wcn36xx_smd_init_scan(wcn);
	wcn->is_scanning = 1;
	wcn36xx_smd_start_scan(wcn, wcn->ch);
	wcn->prev_channel = wcn->ch;
}

static void wcn36xx_sw_scan_complete(struct ieee80211_hw *hw)
{
	struct wcn36xx *wcn = hw->priv;
	ENTER();
	if (wcn->prev_channel) {
		wcn36xx_smd_end_scan(wcn, wcn->prev_channel);
		wcn->prev_channel = 0;
	}
	wcn36xx_smd_finish_scan(wcn);
	wcn->is_scanning = 0;
}

static void wcn36xx_bss_info_changed(struct ieee80211_hw *hw,
				       struct ieee80211_vif *vif,
				       struct ieee80211_bss_conf *bss_conf,
				       u32 changed)
{
	struct wcn36xx *wcn = hw->priv;
	struct sk_buff *skb = NULL;
	u16 tim_off, tim_len;

	ENTER();

	if(changed & BSS_CHANGED_BSSID) {
		wcn36xx_info("wcn36xx_bss_info_changed BSS_CHANGED_BSSID=%pM", bss_conf->bssid);
		if(!is_zero_ether_addr(bss_conf->bssid)) {
			wcn36xx_smd_join(wcn, (u8*)bss_conf->bssid, vif->addr, wcn->ch);
			wcn36xx_smd_config_bss(wcn, true, (u8*)bss_conf->bssid, 0);
		}
	} else if (changed & BSS_CHANGED_BEACON_ENABLED){
		if(!wcn->beacon_enable) {
			wcn->beacon_enable = true;
			skb = ieee80211_beacon_get_tim(hw, vif, &tim_off, &tim_len);
			wcn36xx_smd_config_bss(wcn, false, NULL, 0);
			wcn36xx_smd_send_beacon(wcn, skb, tim_off, 0);
		}
	} else {
		wcn36xx_info("wcn36xx_bss_info_changed No handling for change=%x", changed);
	}
}
static int wcn36xx_set_frag_threshold(struct ieee80211_hw *hw, u32 value)
{
	ENTER();
	return 0;
}
static int wcn36xx_set_rts_threshold(struct ieee80211_hw *hw, u32 value)
{
	ENTER();
	return 0;
}

static int wcn36xx_get_survey(struct ieee80211_hw *hw, int idx,
				struct survey_info *survey)
{
	ENTER();
	return 0;
}

static int wcn36xx_ampdu_action(struct ieee80211_hw *hw,
				  struct ieee80211_vif *vif,
				  enum ieee80211_ampdu_mlme_action action,
				  struct ieee80211_sta *sta, u16 tid, u16 *ssn,
				  u8 buf_size)
{
	ENTER();
	return 0;
}
static bool wcn36xx_tx_frames_pending(struct ieee80211_hw *hw)
{
	ENTER();
	return true;
}
static int wcn36xx_set_bitrate_mask(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif,
				   const struct cfg80211_bitrate_mask *mask)
{
	ENTER();
	return 0;
}

static void wcn36xx_channel_switch(struct ieee80211_hw *hw,
				   struct ieee80211_channel_switch *ch_switch)
{
	ENTER();
}

static int wcn36xx_suspend(struct ieee80211_hw *hw,
			    struct cfg80211_wowlan *wow)
{
	ENTER();
	return 0;
}
static int wcn36xx_resume(struct ieee80211_hw *hw)
{
	ENTER();
	return 0;
}
static int wcn36xx_add_interface(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif)
{
	struct wcn36xx *wcn = hw->priv;
	ENTER();

	if(vif) {
		switch (vif->type) {
		case NL80211_IFTYPE_STATION:
			wcn36xx_info("Add station interface");
			wcn36xx_smd_add_sta_self(wcn, wcn->addresses[0], 0);
			break;
		case NL80211_IFTYPE_AP:
			wcn36xx_info("Add AP interface");
			wcn36xx_smd_add_sta_self(wcn, wcn->addresses[0], 0);
			break;
		default:
			wcn36xx_info("Add interface=%d", vif->type);
			break;
		}
	}

	return 0;
}

static int wcn36xx_sta_add(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
		       struct ieee80211_sta *sta)
{
	struct wcn36xx *wcn = hw->priv;
	ENTER();
	wcn36xx_smd_set_link_st(wcn, sta->addr, vif->addr, WCN36XX_HAL_LINK_POSTASSOC_STATE);
	wcn36xx_smd_config_sta(wcn, sta->addr, sta->aid, vif->addr);
	wcn36xx_smd_config_bss(wcn, true, sta->addr, 1);
	return 0;
}
static int wcn36xx_sta_remove(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			  struct ieee80211_sta *sta)
{
	struct wcn36xx *wcn = hw->priv;
	ENTER();
	wcn36xx_smd_delete_sta(wcn);
	wcn36xx_smd_delete_bss(wcn);
	wcn36xx_smd_set_link_st(wcn, sta->addr, vif->addr, WCN36XX_HAL_LINK_IDLE_STATE);
	return 0;
}

static const struct ieee80211_ops wcn36xx_ops = {
	.start 			= wcn36xx_start,
	.stop	 		= wcn36xx_stop,
	.add_interface		= wcn36xx_add_interface,
	.remove_interface 	= wcn36xx_remove_interface,
	.change_interface 	= wcn36xx_change_interface,
#ifdef CONFIG_PM
	.suspend 		= wcn36xx_suspend,
	.resume			= wcn36xx_resume,
#endif
	.config 		= wcn36xx_config,
	.prepare_multicast 	= wcn36xx_prepare_multicast,
	.configure_filter 	= wcn36xx_configure_filter,
	.tx 			= wcn36xx_tx,
	.set_key 		= wcn36xx_set_key,
	.sw_scan_start          = wcn36xx_sw_scan_start,
	.sw_scan_complete       = wcn36xx_sw_scan_complete,
	.bss_info_changed 	= wcn36xx_bss_info_changed,
	.set_frag_threshold 	= wcn36xx_set_frag_threshold,
	.set_rts_threshold 	= wcn36xx_set_rts_threshold,
	.sta_add 		= wcn36xx_sta_add,
	.sta_remove	 	= wcn36xx_sta_remove,
	.get_survey 		= wcn36xx_get_survey,
	.ampdu_action 		= wcn36xx_ampdu_action,
	.tx_frames_pending 	= wcn36xx_tx_frames_pending,
	.set_bitrate_mask 	= wcn36xx_set_bitrate_mask,
	.channel_switch 	= wcn36xx_channel_switch
};

static struct ieee80211_hw *wcn36xx_alloc_hw(void)
{
	struct ieee80211_hw *hw;
	ENTER();
	hw = ieee80211_alloc_hw(sizeof(struct wcn36xx), &wcn36xx_ops);
	return hw;
}
#define CHAN2G(_freq, _idx) { \
	.band = IEEE80211_BAND_2GHZ, \
	.center_freq = (_freq), \
	.hw_value = (_idx), \
	.max_power = 25, \
}

#define CHAN5G(_freq, _idx) { \
	.band = IEEE80211_BAND_5GHZ, \
	.center_freq = (_freq), \
	.hw_value = (_idx), \
	.max_power = 25, \
}

static struct ieee80211_channel wcn_2ghz_channels[] = {
	CHAN2G(2412, 0), /* Channel 1 */
	CHAN2G(2417, 1), /* Channel 2 */
	CHAN2G(2422, 2), /* Channel 3 */
	CHAN2G(2427, 3), /* Channel 4 */
	CHAN2G(2432, 4), /* Channel 5 */
	CHAN2G(2437, 5), /* Channel 6 */
	CHAN2G(2442, 6), /* Channel 7 */
	CHAN2G(2447, 7), /* Channel 8 */
	CHAN2G(2452, 8), /* Channel 9 */
	CHAN2G(2457, 9), /* Channel 10 */
	CHAN2G(2462, 10), /* Channel 11 */
	CHAN2G(2467, 11), /* Channel 12 */
	CHAN2G(2472, 12), /* Channel 13 */
	CHAN2G(2484, 13)  /* Channel 14 */

};

static struct ieee80211_channel wcn_5ghz_channels[] = {
	CHAN5G(5180, 36),
	CHAN5G(5200, 40),
	CHAN5G(5220, 44),
	CHAN5G(5240, 48),
	CHAN5G(5260, 52),
	CHAN5G(5280, 56),
	CHAN5G(5300, 60),
	CHAN5G(5320, 64),
	CHAN5G(5500, 100),
	CHAN5G(5520, 104),
	CHAN5G(5540, 108),
	CHAN5G(5560, 112),
	CHAN5G(5580, 116),
	CHAN5G(5600, 120),
	CHAN5G(5620, 124),
	CHAN5G(5640, 128),
	CHAN5G(5660, 132),
	CHAN5G(5700, 140),
	CHAN5G(5745, 149),
	CHAN5G(5765, 153),
	CHAN5G(5785, 157),
	CHAN5G(5805, 161),
	CHAN5G(5825, 165)
};

#define RATE(_bitrate, _hw_rate, _flags) { \
	.bitrate        = (_bitrate),                   \
	.flags          = (_flags),                     \
	.hw_value       = (_hw_rate),                   \
	.hw_value_short = (_hw_rate)  \
}
static struct ieee80211_rate wcn_legacy_rates[] = {
	RATE(10, BIT(0), 0),
	RATE(20, BIT(1), IEEE80211_RATE_SHORT_PREAMBLE),
	RATE(55, BIT(2), IEEE80211_RATE_SHORT_PREAMBLE),
	RATE(110, BIT(3), IEEE80211_RATE_SHORT_PREAMBLE),
	RATE(60, BIT(4), 0),
	RATE(90, BIT(5), 0),
	RATE(120, BIT(6), 0),
	RATE(180, BIT(7), 0),
	RATE(240, BIT(8), 0),
	RATE(360, BIT(9), 0),
	RATE(480, BIT(10), 0),
	RATE(540, BIT(11), 0)
};

static struct ieee80211_rate wcn_5ghz_rates[] = {
	RATE(60, BIT(4), 0),
	RATE(90, BIT(5), 0),
	RATE(120, BIT(6), 0),
	RATE(180, BIT(7), 0),
	RATE(240, BIT(8), 0),
	RATE(360, BIT(9), 0),
	RATE(480, BIT(10), 0),
	RATE(540, BIT(11), 0)
};

static struct ieee80211_supported_band wcn_band_2ghz = {
	.channels 	= wcn_2ghz_channels,
	.n_channels 	= ARRAY_SIZE(wcn_2ghz_channels),
	.bitrates 	= wcn_legacy_rates,
	.n_bitrates 	= ARRAY_SIZE(wcn_legacy_rates),
	.ht_cap		= {
		.cap = IEEE80211_HT_CAP_GRN_FLD | IEEE80211_HT_CAP_SGI_20 |
			(1 << IEEE80211_HT_CAP_RX_STBC_SHIFT),
		.ht_supported = true,
		.ampdu_factor = IEEE80211_HT_MAX_AMPDU_8K,
		.ampdu_density = IEEE80211_HT_MPDU_DENSITY_8,
		.mcs = {
			.rx_mask = { 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
			.rx_highest = cpu_to_le16(72),
			.tx_params = IEEE80211_HT_MCS_TX_DEFINED,
		}
	}
};

static struct ieee80211_supported_band wcn_band_5ghz = {
	.channels 	= wcn_5ghz_channels,
	.n_channels 	= ARRAY_SIZE(wcn_5ghz_channels),
	.bitrates 	= wcn_5ghz_rates,
	.n_bitrates 	= ARRAY_SIZE(wcn_5ghz_rates),
	.ht_cap		= {
		.cap = IEEE80211_HT_CAP_GRN_FLD
			| IEEE80211_HT_CAP_SGI_20
			| IEEE80211_HT_CAP_DSSSCCK40
			| IEEE80211_HT_CAP_LSIG_TXOP_PROT
                        | IEEE80211_HT_CAP_SGI_40
                        | IEEE80211_HT_CAP_SUP_WIDTH_20_40,
		.ht_supported = true,
		.ampdu_factor = IEEE80211_HT_MAX_AMPDU_64K,
		.ampdu_density = IEEE80211_HT_MPDU_DENSITY_16,
		.mcs = {
			.rx_mask = { 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
			.rx_highest = cpu_to_le16(72),
			.tx_params = IEEE80211_HT_MCS_TX_DEFINED,
		}
	}
};

static int wcn36xx_init_ieee80211(struct wcn36xx * wcn_priv)
{
	int ret = 0;

	wcn_priv->hw->flags = IEEE80211_HW_SIGNAL_DBM |
		IEEE80211_HW_SUPPORTS_PS |
		IEEE80211_HW_SUPPORTS_DYNAMIC_PS |
		IEEE80211_HW_AP_LINK_PS |
		/* Need ?*/
		IEEE80211_HW_REPORTS_TX_ACK_STATUS |
		IEEE80211_HW_HAS_RATE_CONTROL |
		IEEE80211_HW_REPORTS_TX_ACK_STATUS;

	wcn_priv->hw->wiphy->interface_modes = BIT(NL80211_IFTYPE_STATION)|
		BIT(NL80211_IFTYPE_AP);

	wcn_priv->hw->wiphy->bands[IEEE80211_BAND_2GHZ] = &wcn_band_2ghz;
	wcn_priv->hw->wiphy->bands[IEEE80211_BAND_5GHZ] = &wcn_band_5ghz;

	wcn_priv->hw->wiphy->max_scan_ssids = 1;

	// TODO make a conf file where to read this information from
	wcn_priv->hw->max_listen_interval = 200;

	wcn_priv->hw->queues = 4;

	SET_IEEE80211_DEV(wcn_priv->hw, wcn_priv->dev);

	wcn_priv->hw->sta_data_size = sizeof(struct wcn_sta);
	wcn_priv->hw->vif_data_size = sizeof(struct wcn_vif);


	return ret;
}

static int wcn36xx_read_mac_addresses(struct wcn36xx *wcn)
{
	const struct firmware *addr_file = NULL;
	int status;
	u8 tmp[18];
	u8 qcom_oui[3] = {0x00, 0xA0, 0xC6};
	char *files[1] = {MAC_ADDR_0};
	int i;

	for (i = 0; i < ARRAY_SIZE(files); i++) {
		status = request_firmware(&addr_file, files[i], wcn->dev);

		if (status) {
			wcn36xx_error("Failed to read macaddress file %s",
				      files[i]);
			/* Assign a random mac address with Qualcomm oui */
			memcpy(wcn->addresses[i].addr, qcom_oui, 3);
			get_random_bytes(wcn->addresses[i].addr + 3, 3);

		} else {
			memset(tmp, 0, sizeof(tmp));
			memcpy(tmp, addr_file->data, sizeof(tmp) - 1);
			sscanf(tmp, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
			       &wcn->addresses[i].addr[0],
			       &wcn->addresses[i].addr[1],
			       &wcn->addresses[i].addr[2],
			       &wcn->addresses[i].addr[3],
			       &wcn->addresses[i].addr[4],
			       &wcn->addresses[i].addr[5]);

			release_firmware(addr_file);
		}
		wcn36xx_info("mac%d: %pM", i, wcn->addresses[i].addr);
	}

	return 0;
}

static int __init wcn36xx_init(void)
{
	struct ieee80211_hw *hw;
	struct wcn36xx *wcn;
	struct resource *wcnss_memory;
	int ret;

	hw = wcn36xx_alloc_hw();
	if (!hw) {
		wcn36xx_error("failed to alloc hw");
		ret = -ENOMEM;
		goto out_err;
	}

	wcn = hw->priv;
	wcn->hw = hw;

	wcn->dev = wcnss_wlan_get_device();
	if (wcn->dev == NULL) {
		wcn36xx_error("failed to get wcnss wlan device");
		ret = -ENOENT;
		goto out_err;
	}

	wcn->wq = create_freezable_workqueue("wcn36xx_wq");
	if (!wcn->wq) {
		wcn36xx_error("failed to allocate wq");
		ret = -ENOMEM;
		goto out_err;
	}

	wcn->ctl_wq = create_freezable_workqueue("wcn36xx_ctl_wq");
	if (!wcn->ctl_wq) {
		wcn36xx_error("failed to allocate ctl wq");
		ret = -ENOMEM;
		goto out_wq;
	}

	wcn36xx_init_ieee80211(wcn);

	// Configuring supported rates
	wcn->supported_rates.op_rate_mode = STA_11n;
	wcn->supported_rates.llb_rates[0] = 0x82;
	wcn->supported_rates.llb_rates[1] = 0x84;
	wcn->supported_rates.llb_rates[2] = 0x8b;
	wcn->supported_rates.llb_rates[3] = 0x96;

	wcn->supported_rates.lla_rates[0] = 0x0C;
	wcn->supported_rates.lla_rates[1] = 0x12;
	wcn->supported_rates.lla_rates[2] = 0x18;
	wcn->supported_rates.lla_rates[3] = 0x24;
	wcn->supported_rates.lla_rates[4] = 0x30;
	wcn->supported_rates.lla_rates[5] = 0x48;
	wcn->supported_rates.lla_rates[6] = 0x60;
	wcn->supported_rates.lla_rates[7] = 0x6C;

	wcn->supported_rates.supported_mcs_set[0] = 0xFF;

	wcn->hw->wiphy->n_addresses = ARRAY_SIZE(wcn->addresses);
	wcn->hw->wiphy->addresses = wcn->addresses;

	wcnss_memory = wcnss_wlan_get_memory_map(wcn->dev);
	if (wcnss_memory == NULL) {
		wcn36xx_error("failed to get wcnss wlan memory map");
		ret = -ENOMEM;
		goto out_wq_ctl;
	}

	wcn->tx_irq = wcnss_wlan_get_dxe_tx_irq(wcn->dev);
	wcn->rx_irq = wcnss_wlan_get_dxe_rx_irq(wcn->dev);

	wcn->mmio = ioremap(wcnss_memory->start, resource_size(wcnss_memory));
	if (NULL == wcn->mmio) {
		wcn36xx_error("failed to map io memory");
		ret = -ENOMEM;
		goto out_wq_ctl;
	}

	private_hw = hw;
	wcn->beacon_enable = false;

	wcn36xx_read_mac_addresses(wcn);
	SET_IEEE80211_PERM_ADDR(wcn->hw, (u8*)(wcn->addresses[0].addr));

	ret = ieee80211_register_hw(wcn->hw);
	if (ret)
		goto out_unmap;

	return 0;

out_unmap:
	iounmap(wcn->mmio);
out_wq_ctl:
	destroy_workqueue(wcn->ctl_wq);
out_wq:
	destroy_workqueue(wcn->wq);
out_err:
	return ret;
}

module_init(wcn36xx_init);

static void __exit wcn36xx_exit(void)
{
	struct ieee80211_hw *hw = private_hw;
	struct wcn36xx *wcn = hw->priv;

	ieee80211_unregister_hw(hw);
	destroy_workqueue(wcn->ctl_wq);
	destroy_workqueue(wcn->wq);
	iounmap(wcn->mmio);
	ieee80211_free_hw(hw);
}
module_exit(wcn36xx_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eugene Krasnikov k.eugene.e@gmail.com");
MODULE_FIRMWARE(MAC_ADDR_0);
