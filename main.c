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

	wcn36xx_dbg(WCN36XX_DBG_MAC, "mac start");

	/* SMD initialization */
	ret = wcn36xx_smd_open(wcn);
	if (ret) {
		wcn36xx_error("Failed to open smd channel: %d", ret);
		goto out_err;
	}

	/* Not to receive INT until the whole buf from SMD is read */
	smd_disable_read_intr(wcn->smd_ch);

	/* Allocate memory pools for Mgmt BD headers and Data BD headers */
	ret = wcn36xx_dxe_allocate_mem_pools(wcn);
	if (ret) {
		wcn36xx_error("Failed to alloc DXE mempool: %d", ret);
		goto out_smd_close;
	}

	wcn36xx_dxe_alloc_ctl_blks(wcn);
	if (ret) {
		wcn36xx_error("Failed to alloc DXE ctl blocks: %d", ret);
		goto out_free_dxe_pool;
	}

	INIT_WORK(&wcn->rx_ready_work, wcn36xx_rx_ready_work);

	/* Maximum SMD message size is 4k */
	wcn->smd_buf = kmalloc(4096, GFP_KERNEL);
	if (!wcn->smd_buf) {
		wcn36xx_error("Failed to allocate smd buf");
		ret = -ENOMEM;
		goto out_free_dxe_ctl;
	}

	/* TODO pass configuration to FW */
	ret = wcn36xx_smd_load_nv(wcn);
	if (ret) {
		wcn36xx_error("Failed to push NV to chip");
		goto out_free_smd_buf;
	}

	ret = wcn36xx_smd_start(wcn);
	if (ret) {
		wcn36xx_error("Failed to start chip");
		goto out_free_smd_buf;
	}
	/* DMA channel initialization */
	ret = wcn36xx_dxe_init(wcn);
	if (ret) {
		wcn36xx_error("DXE init failed");
		goto out_smd_stop;
	}
	return 0;

out_smd_stop:
	wcn36xx_smd_stop(wcn);
out_free_smd_buf:
	kfree(wcn->smd_buf);
out_free_dxe_pool:
	wcn36xx_dxe_free_mem_pools(wcn);
out_free_dxe_ctl:
	wcn36xx_dxe_free_ctl_blks(wcn);
out_smd_close:
	wcn36xx_smd_close(wcn);
out_err:
	return ret;
}
static void wcn36xx_stop(struct ieee80211_hw *hw)
{
	struct wcn36xx *wcn = hw->priv;

	wcn36xx_dbg(WCN36XX_DBG_MAC, "mac stop");

	wcn36xx_smd_stop(wcn);
	wcn36xx_dxe_deinit(wcn);
	wcn36xx_smd_close(wcn);

	wcn36xx_dxe_free_mem_pools(wcn);
	wcn36xx_dxe_free_ctl_blks(wcn);

	kfree(wcn->smd_buf);
}

static int wcn36xx_change_interface(struct ieee80211_hw *hw,
				      struct ieee80211_vif *vif,
				      enum nl80211_iftype new_type, bool p2p)
{
	wcn36xx_dbg(WCN36XX_DBG_MAC, "mac change interface vif %p new_type %d p2p %d",
		    vif, new_type, p2p);

	return 0;
}

static int wcn36xx_config(struct ieee80211_hw *hw, u32 changed)
{
	struct wcn36xx *wcn = hw->priv;

	wcn36xx_dbg(WCN36XX_DBG_MAC, "mac config changed 0x%08x", changed);

	if (changed & IEEE80211_CONF_CHANGE_CHANNEL) {
		wcn->ch = hw->conf.chandef.chan->hw_value;
		wcn->current_channel = hw->conf.chandef.chan;
		wcn->band = hw->conf.chandef.chan->band;
		wcn36xx_dbg(WCN36XX_DBG_MAC, "wcn36xx_config channel switch=%d", wcn->ch);
		wcn36xx_smd_switch_channel(wcn, wcn->ch);
	}

	return 0;
}

#define WCN36XX_SUPPORTED_FILTERS (0)

static void wcn36xx_configure_filter(struct ieee80211_hw *hw,
				       unsigned int changed,
				       unsigned int *total, u64 multicast)
{
	wcn36xx_dbg(WCN36XX_DBG_MAC, "mac configure filter");

	changed &= WCN36XX_SUPPORTED_FILTERS;
	*total &= WCN36XX_SUPPORTED_FILTERS;
}

static void wcn36xx_tx(struct ieee80211_hw *hw,
		       struct ieee80211_tx_control *control,
		       struct sk_buff *skb)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct ieee80211_mgmt *mgmt;
	bool high, bcast, tx_compl;
	u32 header_len = 0;
	struct wcn36xx *wcn = hw->priv;

	tx_compl = info->flags & IEEE80211_TX_CTL_REQ_TX_STATUS;
	mgmt = (struct ieee80211_mgmt *)skb->data;

	high = !(ieee80211_is_data(mgmt->frame_control) ||
		 ieee80211_is_data_qos(mgmt->frame_control));

	bcast = is_broadcast_ether_addr(mgmt->da) ||
		is_multicast_ether_addr(mgmt->da);

	/*
	 * In joining state trick hardware that probe is sent as unicast even
	 * if address is broadcast.
	 */
	if (wcn->is_joining && ieee80211_is_probe_req(mgmt->frame_control))
		bcast = false;
	wcn36xx_dbg(WCN36XX_DBG_TX,
		    "tx skb %p len %d fc %04x sn %d %s %s",
		    skb, skb->len, __le16_to_cpu(mgmt->frame_control),
		    IEEE80211_SEQ_TO_SN(__le16_to_cpu(mgmt->seq_ctrl)),
		    high ? "high" : "low", bcast ? "bcast" : "ucast");

	wcn36xx_dbg_dump(WCN36XX_DBG_TX_DUMP, "", skb->data, skb->len);

	header_len = ieee80211_is_data_qos(mgmt->frame_control) ?
		sizeof(struct ieee80211_qos_hdr) :
		sizeof(struct ieee80211_hdr_3addr);
	wcn36xx_dxe_tx(hw->priv, skb, bcast, high, header_len, tx_compl);
}

static int wcn36xx_set_key(struct ieee80211_hw *hw, enum set_key_cmd cmd,
			   struct ieee80211_vif *vif,
			   struct ieee80211_sta *sta,
			   struct ieee80211_key_conf *key_conf)
{
	struct wcn36xx *wcn = hw->priv;
	int ret = 0;
	enum ani_ed_type enc_type;
	wcn36xx_dbg(WCN36XX_DBG_MAC, "mac80211 set key");
	wcn36xx_dbg(WCN36XX_DBG_MAC, "Key: cmd=0x%x algo:0x%x, "
		    "id:%d, len:%d flags 0x%x",
		    cmd, key_conf->cipher, key_conf->keyidx,
		    key_conf->keylen, key_conf->flags);
	wcn36xx_dbg_dump(WCN36XX_DBG_MAC, "KEY: ",
			 key_conf->key,
			 key_conf->keylen);

	switch (key_conf->cipher) {
	case WLAN_CIPHER_SUITE_CCMP:
		enc_type = WCN36XX_HAL_ED_CCMP;
		break;
	case WLAN_CIPHER_SUITE_TKIP:
		enc_type = WCN36XX_HAL_ED_TKIP;
		break;
	default:
		wcn36xx_error("Unsupported key type 0x%x",
			      key_conf->cipher);
		ret = -EOPNOTSUPP;
		goto out;
		break;
	}

	switch (cmd) {
	case SET_KEY:
		if (WCN36XX_STA_KEY == wcn->en_state) {
			wcn36xx_smd_set_stakey(wcn,
				enc_type,
				key_conf->keyidx,
				key_conf->keylen,
				key_conf->key);
			wcn->en_state = WCN36XX_BSS_KEY;
		} else {
			wcn36xx_smd_set_bsskey(wcn,
				enc_type,
				key_conf->keyidx,
				key_conf->keylen,
				key_conf->key);
		}
		break;
	case DISABLE_KEY:
		if (WCN36XX_BSS_KEY == wcn->en_state) {
			wcn36xx_smd_remove_bsskey(wcn,
				enc_type,
				key_conf->keyidx);
			wcn->en_state = WCN36XX_STA_KEY;
		} else {
			/* do not remove key if disassociated */
			if (wcn->aid)
				wcn36xx_smd_remove_stakey(wcn,
							  enc_type,
							  key_conf->keyidx);
		}
		break;
	default:
		wcn36xx_error("Unsupported key cmd 0x%x", cmd);
		ret = -EOPNOTSUPP;
		goto out;
		break;
	}

out:
	return ret;
}

static void wcn36xx_sw_scan_start(struct ieee80211_hw *hw)
{
	struct wcn36xx *wcn = hw->priv;

	wcn36xx_smd_init_scan(wcn);
	wcn36xx_smd_start_scan(wcn, wcn->ch);
}

static void wcn36xx_sw_scan_complete(struct ieee80211_hw *hw)
{
	struct wcn36xx *wcn = hw->priv;

	wcn36xx_smd_end_scan(wcn, wcn->ch);
	wcn36xx_smd_finish_scan(wcn);
}

static void wcn36xx_bss_info_changed(struct ieee80211_hw *hw,
				       struct ieee80211_vif *vif,
				       struct ieee80211_bss_conf *bss_conf,
				       u32 changed)
{
	struct wcn36xx *wcn = hw->priv;
	struct sk_buff *skb = NULL;
	u16 tim_off, tim_len;
	enum wcn36xx_hal_link_state link_state;
	wcn->current_vif = (struct wcn36xx_vif *)vif->drv_priv;

	wcn36xx_dbg(WCN36XX_DBG_MAC, "mac bss info changed vif %p changed 0x%08x",
		    vif, changed);

	if (changed & BSS_CHANGED_BEACON_INT) {
		wcn36xx_dbg(WCN36XX_DBG_MAC,
			    "mac bss changed beacon_int %d",
			    bss_conf->beacon_int);

		wcn->beacon_interval = bss_conf->beacon_int;
	}

	if (changed & BSS_CHANGED_BSSID) {
		wcn36xx_dbg(WCN36XX_DBG_MAC, "mac bss changed_bssid %pM",
			    bss_conf->bssid);

		if (vif->type == NL80211_IFTYPE_STATION &&
		    !is_zero_ether_addr(bss_conf->bssid)) {
			wcn->is_joining = true;
			wcn36xx_smd_join(wcn, bss_conf->bssid,
					 vif->addr, wcn->ch);
			wcn36xx_smd_config_bss(wcn, NL80211_IFTYPE_STATION,
					       bss_conf->bssid, false,
					       wcn->beacon_interval);
		}
	}

	if (changed & BSS_CHANGED_SSID) {
		wcn36xx_dbg(WCN36XX_DBG_MAC,
			    "mac bss changed ssid");
		wcn36xx_dbg_dump(WCN36XX_DBG_MAC, "ssid ",
				 bss_conf->ssid, bss_conf->ssid_len);

		wcn->ssid.length = bss_conf->ssid_len;
		memcpy(&wcn->ssid.ssid, bss_conf->ssid, bss_conf->ssid_len);
	}

	if (changed & BSS_CHANGED_ASSOC) {
		wcn->is_joining = false;
		if (bss_conf->assoc) {
			wcn36xx_dbg(WCN36XX_DBG_MAC,
				    "mac assoc bss %pM vif %pM AID=%d",
				     bss_conf->bssid,
				     vif->addr,
				     bss_conf->aid);

			wcn->aid = bss_conf->aid;
			wcn->en_state = WCN36XX_STA_KEY;

			wcn36xx_smd_set_link_st(wcn, bss_conf->bssid,
						vif->addr,
						WCN36XX_HAL_LINK_POSTASSOC_STATE);
			wcn36xx_smd_config_bss(wcn, NL80211_IFTYPE_STATION,
					       bss_conf->bssid,
					       true, wcn->beacon_interval);
			wcn36xx_smd_config_sta(wcn, bss_conf->bssid, vif->addr);

		} else {
			wcn36xx_dbg(WCN36XX_DBG_MAC,
				    "disassociated bss %pM vif %pM AID=%d",
				    bss_conf->bssid,
				    vif->addr,
				    bss_conf->aid);
			wcn->aid = 0;
			wcn->en_state = WCN36XX_STA_KEY;
			wcn36xx_smd_delete_sta(wcn);
			wcn36xx_smd_delete_bss(wcn);
			wcn36xx_smd_set_link_st(wcn,
						bss_conf->bssid,
						vif->addr,
						WCN36XX_HAL_LINK_IDLE_STATE);
		}
	}
	if (changed & BSS_CHANGED_AP_PROBE_RESP) {
		wcn36xx_dbg(WCN36XX_DBG_MAC, "mac bss changed ap probe resp");
		skb = ieee80211_proberesp_get(hw, vif);
		if (skb) {
			wcn36xx_smd_update_proberesp_tmpl(wcn, skb);
			dev_kfree_skb(skb);
		} else
			wcn36xx_error("failed to alloc probereq skb");
	}

	if (changed & BSS_CHANGED_BEACON_ENABLED) {
		wcn36xx_dbg(WCN36XX_DBG_MAC,
			    "mac bss changed beacon enabled %d",
			    bss_conf->enable_beacon);

		if (bss_conf->enable_beacon) {
			wcn->beacon_enable = true;
			wcn36xx_smd_config_bss(wcn, wcn->iftype,
					       wcn->addresses[0].addr, false,
					       wcn->beacon_interval);
			skb = ieee80211_beacon_get_tim(hw, vif, &tim_off,
						       &tim_len);
			if (skb) {
				wcn36xx_smd_send_beacon(wcn, skb, tim_off, 0);
				dev_kfree_skb(skb);
			} else
				wcn36xx_error("failed to alloc beacon skb");

			if (vif->type == NL80211_IFTYPE_ADHOC ||
			    vif->type == NL80211_IFTYPE_MESH_POINT)
				link_state = WCN36XX_HAL_LINK_IBSS_STATE;
			else
				link_state = WCN36XX_HAL_LINK_AP_STATE;

			wcn36xx_smd_set_link_st(wcn, vif->addr, vif->addr,
						link_state);
		} else {
			/* FIXME: disable beaconing */
		}
	}
}

/* this is required when using IEEE80211_HW_HAS_RATE_CONTROL */
static int wcn36xx_set_rts_threshold(struct ieee80211_hw *hw, u32 value)
{
	return 0;
}

static void wcn36xx_remove_interface(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif)
{
	struct wcn36xx *wcn = hw->priv;
	wcn36xx_dbg(WCN36XX_DBG_MAC, "mac remove interface vif %p", vif);
	wcn36xx_smd_delete_sta_self(wcn, wcn->addresses[0].addr);
}

static int wcn36xx_add_interface(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif)
{
	struct wcn36xx *wcn = hw->priv;

	wcn36xx_dbg(WCN36XX_DBG_MAC, "mac add interface vif %p type %d",
		    vif, vif->type);

	wcn->current_vif = (struct wcn36xx_vif *)vif->drv_priv;
	switch (vif->type) {
	case NL80211_IFTYPE_STATION:
		wcn36xx_smd_add_sta_self(wcn, vif->addr, 0);
		break;
	case NL80211_IFTYPE_AP:
		wcn36xx_smd_add_sta_self(wcn, vif->addr, 0);
		break;
	case NL80211_IFTYPE_ADHOC:
	case NL80211_IFTYPE_MESH_POINT:
		wcn36xx_smd_add_sta_self(wcn, vif->addr, 0);
		break;
	default:
		wcn36xx_warn("Unsupported interface type requested: %d",
			     vif->type);
		return -EOPNOTSUPP;
	}

	wcn->iftype = vif->type;

	return 0;
}

static int wcn36xx_sta_add(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			   struct ieee80211_sta *sta)
{
	struct wcn36xx *wcn = hw->priv;

	wcn36xx_dbg(WCN36XX_DBG_MAC, "mac sta add vif %p sta %pM",
		    vif, sta->addr);

	if (vif->type == NL80211_IFTYPE_ADHOC ||
	    vif->type == NL80211_IFTYPE_AP ||
	    vif->type == NL80211_IFTYPE_MESH_POINT)
		wcn36xx_smd_config_sta(wcn, wcn->addresses[0].addr,
				       sta->addr);

	return 0;
}

static int wcn36xx_sta_remove(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif,
			      struct ieee80211_sta *sta)
{
	struct wcn36xx *wcn = hw->priv;

	wcn36xx_dbg(WCN36XX_DBG_MAC, "mac sta remove vif %p sta %pM",
		    vif, sta->addr);

	if (vif->type == NL80211_IFTYPE_ADHOC ||
	    vif->type == NL80211_IFTYPE_MESH_POINT)
		wcn36xx_smd_delete_sta(wcn);

	return 0;
}

static const struct ieee80211_ops wcn36xx_ops = {
	.start			= wcn36xx_start,
	.stop			= wcn36xx_stop,
	.add_interface		= wcn36xx_add_interface,
	.remove_interface	= wcn36xx_remove_interface,
	.change_interface	= wcn36xx_change_interface,
	.config			= wcn36xx_config,
	.configure_filter	= wcn36xx_configure_filter,
	.tx			= wcn36xx_tx,
	.set_key		= wcn36xx_set_key,
	.sw_scan_start		= wcn36xx_sw_scan_start,
	.sw_scan_complete	= wcn36xx_sw_scan_complete,
	.bss_info_changed	= wcn36xx_bss_info_changed,
	.set_rts_threshold	= wcn36xx_set_rts_threshold,
	.sta_add		= wcn36xx_sta_add,
	.sta_remove		= wcn36xx_sta_remove,
};

static struct ieee80211_hw *wcn36xx_alloc_hw(void)
{
	struct ieee80211_hw *hw;

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

/* The wcn firmware expects channel values to matching
 * their mnemonic values. So use these for .hw_value. */
static struct ieee80211_channel wcn_2ghz_channels[] = {
	CHAN2G(2412, 1), /* Channel 1 */
	CHAN2G(2417, 2), /* Channel 2 */
	CHAN2G(2422, 3), /* Channel 3 */
	CHAN2G(2427, 4), /* Channel 4 */
	CHAN2G(2432, 5), /* Channel 5 */
	CHAN2G(2437, 6), /* Channel 6 */
	CHAN2G(2442, 7), /* Channel 7 */
	CHAN2G(2447, 8), /* Channel 8 */
	CHAN2G(2452, 9), /* Channel 9 */
	CHAN2G(2457, 10), /* Channel 10 */
	CHAN2G(2462, 11), /* Channel 11 */
	CHAN2G(2467, 12), /* Channel 12 */
	CHAN2G(2472, 13), /* Channel 13 */
	CHAN2G(2484, 14)  /* Channel 14 */

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
	.channels	= wcn_2ghz_channels,
	.n_channels	= ARRAY_SIZE(wcn_2ghz_channels),
	.bitrates	= wcn_legacy_rates,
	.n_bitrates	= ARRAY_SIZE(wcn_legacy_rates),
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
	.channels	= wcn_5ghz_channels,
	.n_channels	= ARRAY_SIZE(wcn_5ghz_channels),
	.bitrates	= wcn_5ghz_rates,
	.n_bitrates	= ARRAY_SIZE(wcn_5ghz_rates),
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

static const struct ieee80211_iface_limit if_limits[] = {
	{ .max = 2, .types = BIT(NL80211_IFTYPE_STATION) },
	{ .max = 1, .types = BIT(NL80211_IFTYPE_AP) },
};

static const struct ieee80211_iface_combination if_comb = {
	.limits = if_limits,
	.n_limits = ARRAY_SIZE(if_limits),
	.max_interfaces = 2,
	.num_different_channels = 1,
};

static int wcn36xx_init_ieee80211(struct wcn36xx *wcn)
{
	int ret = 0;

	static const u32 cipher_suites[] = {
		WLAN_CIPHER_SUITE_TKIP,
		WLAN_CIPHER_SUITE_CCMP,
	};

	wcn->hw->flags = IEEE80211_HW_SIGNAL_DBM |
		IEEE80211_HW_HAS_RATE_CONTROL |
		IEEE80211_HW_REPORTS_TX_ACK_STATUS;

	wcn->hw->wiphy->interface_modes = BIT(NL80211_IFTYPE_STATION) |
		BIT(NL80211_IFTYPE_AP) |
		BIT(NL80211_IFTYPE_ADHOC) |
		BIT(NL80211_IFTYPE_MESH_POINT);

	wcn->hw->wiphy->iface_combinations = &if_comb;
	wcn->hw->wiphy->n_iface_combinations = 1;

	wcn->hw->wiphy->bands[IEEE80211_BAND_2GHZ] = &wcn_band_2ghz;
	wcn->hw->wiphy->bands[IEEE80211_BAND_5GHZ] = &wcn_band_5ghz;

	wcn->hw->wiphy->max_scan_ssids = 1;

	wcn->hw->wiphy->cipher_suites = cipher_suites;
	wcn->hw->wiphy->n_cipher_suites = ARRAY_SIZE(cipher_suites);

	wcn->hw->wiphy->flags |= WIPHY_FLAG_AP_PROBE_RESP_OFFLOAD;
	/* TODO make a conf file where to read this information from */
	wcn->hw->max_listen_interval = 200;

	wcn->hw->queues = 4;

	SET_IEEE80211_DEV(wcn->hw, wcn->dev);

	wcn->hw->sta_data_size = sizeof(struct wcn_sta);
	wcn->hw->vif_data_size = sizeof(struct wcn36xx_vif);


	return ret;
}

static int wcn36xx_read_mac_addresses(struct wcn36xx *wcn)
{
	const struct firmware *addr_file = NULL;
	int status;
	u8 tmp[18];
	static const u8 qcom_oui[3] = {0x00, 0x0A, 0xF5};
	static const char *files[1] = {MAC_ADDR_0};
	int i;

	for (i = 0; i < ARRAY_SIZE(wcn->addresses); i++) {
		if (i > ARRAY_SIZE(files) - 1)
			status = -1;
		else
			status = request_firmware(&addr_file, files[i],
						  wcn->dev);

		if (status) {
			if (i == 0) {
				/* Assign a random mac with Qualcomm oui */
				wcn36xx_warn("Failed to read macaddress file %s, using a random address instead",
					     files[i]);
				memcpy(wcn->addresses[i].addr, qcom_oui, 3);
				get_random_bytes(wcn->addresses[i].addr + 3, 3);
			} else {
				wcn36xx_warn("Failed to read macaddress file, using a random address instead");
				/* Assign locally administered mac addresses to
				 * all but the first mac */
				memcpy(wcn->addresses[i].addr,
				       wcn->addresses[0].addr, ETH_ALEN);
				wcn->addresses[i].addr[0] |= BIT(1);
				get_random_bytes(wcn->addresses[i].addr + 3, 3);
			}

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

	wcn->wq = create_workqueue("wcn36xx_wq");
	if (!wcn->wq) {
		wcn36xx_error("failed to allocate wq");
		ret = -ENOMEM;
		goto out_err;
	}

	wcn36xx_init_ieee80211(wcn);

	/* Configuring supported rates */
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

	wcn->aid = 0;
	wcn->current_vif = NULL;
	wcn->is_joining = false;
	wcn->hw->wiphy->n_addresses = ARRAY_SIZE(wcn->addresses);
	wcn->hw->wiphy->addresses = wcn->addresses;

	wcnss_memory = wcnss_wlan_get_memory_map(wcn->dev);
	if (wcnss_memory == NULL) {
		wcn36xx_error("failed to get wcnss wlan memory map");
		ret = -ENOMEM;
		goto out_wq;
	}

	wcn->tx_irq = wcnss_wlan_get_dxe_tx_irq(wcn->dev);
	wcn->rx_irq = wcnss_wlan_get_dxe_rx_irq(wcn->dev);

	wcn->mmio = ioremap(wcnss_memory->start, resource_size(wcnss_memory));
	if (NULL == wcn->mmio) {
		wcn36xx_error("failed to map io memory");
		ret = -ENOMEM;
		goto out_wq;
	}

	private_hw = hw;
	wcn->beacon_enable = false;

	ret = request_firmware(&wcn->nv, WLAN_NV_FILE, wcn->dev);
	if (ret) {
		wcn36xx_error("Failed to load nv file %s: %d", WLAN_NV_FILE,
			      ret);
		goto out_unmap;
	}

	wcn36xx_read_mac_addresses(wcn);
	SET_IEEE80211_PERM_ADDR(wcn->hw, wcn->addresses[0].addr);

	ret = ieee80211_register_hw(wcn->hw);
	if (ret)
		goto out_free_nv;

	return 0;

out_free_nv:
	release_firmware(wcn->nv);
out_unmap:
	iounmap(wcn->mmio);
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
	destroy_workqueue(wcn->wq);
	iounmap(wcn->mmio);
	release_firmware(wcn->nv);
	ieee80211_free_hw(hw);
}
module_exit(wcn36xx_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eugene Krasnikov k.eugene.e@gmail.com");
MODULE_FIRMWARE(WLAN_NV_FILE);
MODULE_FIRMWARE(MAC_ADDR_0);
