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

#include <linux/etherdevice.h>
#include "smd.h"

#define IS_BIT_SET(x, y) ((x) & (y) ? 1 : 0)
static void wcn36xx_smd_set_sta_ht_params(struct ieee80211_sta *sta,
		struct wcn36xx_hal_config_sta_params *sta_params)
{
	if (sta->ht_cap.ht_supported) {
		sta_params->ht_capable = sta->ht_cap.ht_supported;
		sta_params->tx_channel_width_set = IS_BIT_SET(sta->ht_cap.cap,
			IEEE80211_HT_CAP_SUP_WIDTH_20_40);
		sta_params->lsig_txop_protection = IS_BIT_SET(sta->ht_cap.cap,
			IEEE80211_HT_CAP_LSIG_TXOP_PROT);
		// TODO double check it this is correct with size
		sta_params->max_ampdu_size = sta->ht_cap.ampdu_factor;
		sta_params->max_ampdu_density = sta->ht_cap.ampdu_density;
		sta_params->max_amsdu_size = IS_BIT_SET(sta->ht_cap.cap,
			IEEE80211_HT_CAP_MAX_AMSDU);
		sta_params->sgi_20Mhz = IS_BIT_SET(sta->ht_cap.cap,
			IEEE80211_HT_CAP_SGI_20);
		sta_params->sgi_40mhz = IS_BIT_SET(sta->ht_cap.cap,
			IEEE80211_HT_CAP_SGI_40);
		sta_params->green_field_capable = IS_BIT_SET(sta->ht_cap.cap,
			IEEE80211_HT_CAP_GRN_FLD);
		sta_params->delayed_ba_support = IS_BIT_SET(sta->ht_cap.cap,
			IEEE80211_HT_CAP_DELAY_BA);
		sta_params->dsss_cck_mode_40mhz = IS_BIT_SET(sta->ht_cap.cap,
			IEEE80211_HT_CAP_DSSSCCK40);
	}
}

static void wcn36xx_smd_set_sta_params(struct ieee80211_sta *sta,
		struct wcn36xx_hal_config_sta_params *sta_params)
{
	if (sta) {
		memcpy(&sta_params->bssid, sta->addr, ETH_ALEN);
		sta_params->wmm_enabled = sta->wme;
		sta_params->max_sp_len = sta->max_sp;
		wcn36xx_smd_set_sta_ht_params(sta, sta_params);
	}
}

static int wcn36xx_smd_send_and_wait(struct wcn36xx *wcn, size_t len)
{
	int avail;
	int ret = 0;

	init_completion(&wcn->smd_compl);
	avail = smd_write_avail(wcn->smd_ch);

	wcn36xx_dbg_dump(WCN36XX_DBG_SMD_DUMP, "SMD >>> ", wcn->smd_buf, len);
	if (avail >= len) {
		avail = smd_write(wcn->smd_ch, wcn->smd_buf, len);
		if (avail != len) {
			wcn36xx_error("Cannot write to SMD channel");
			ret = -EAGAIN;
			goto out;
		}
	} else {
		wcn36xx_error("SMD channel can accept only %d bytes", avail);
		ret = -ENOMEM;
		goto out;
	}

	if (wait_for_completion_timeout(&wcn->smd_compl,
		msecs_to_jiffies(SMD_MSG_TIMEOUT)) <= 0) {
		wcn36xx_error("Timeout while waiting SMD response");
		ret = -ETIME;
		goto out;
	}
out:
	mutex_unlock(&wcn->smd_mutex);
	return ret;
}

#define INIT_HAL_MSG(msg_body, type) \
	do {								\
		memset(&msg_body, 0, sizeof(msg_body));			\
		msg_body.header.msg_type = type;			\
		msg_body.header.msg_version = WCN36XX_HAL_MSG_VERSION0; \
		msg_body.header.len = sizeof(msg_body);			\
	} while (0)							\

#define PREPARE_HAL_BUF(send_buf, msg_body) \
	do {							\
		struct wcn36xx *__wcn =				\
			container_of(&send_buf,			\
				     struct wcn36xx, smd_buf);	\
		mutex_lock(&__wcn->smd_mutex);                  \
		memset(send_buf, 0, msg_body.header.len);	\
		memcpy(send_buf, &msg_body, sizeof(msg_body));	\
	} while (0)						\

static int wcn36xx_smd_rsp_status_check(void *buf, size_t len)
{
	struct wcn36xx_fw_msg_status_rsp *rsp;
	if (len < sizeof(struct wcn36xx_hal_msg_header) +
		sizeof(struct wcn36xx_fw_msg_status_rsp))
		return -EIO;
	rsp = (struct wcn36xx_fw_msg_status_rsp *)
		(buf + sizeof(struct wcn36xx_hal_msg_header));

	if (WCN36XX_FW_MSG_RESULT_SUCCESS != rsp->status)
		return -EIO;
	return 0;
}

int wcn36xx_smd_load_nv(struct wcn36xx *wcn)
{
	struct nv_data *nv_d;
	struct wcn36xx_hal_nv_img_download_req_msg msg_body;
	int fw_bytes_left;
	int ret = 0, fw_size, i = 0;
	u16 fm_offset = 0;
	i = 0;

	nv_d = (struct nv_data *)wcn->nv->data;
	fw_size = wcn->nv->size;
	INIT_HAL_MSG(msg_body, WCN36XX_HAL_DOWNLOAD_NV_REQ);

	msg_body.header.len += WCN36XX_NV_FRAGMENT_SIZE;

	msg_body.frag_number = 0;
	do {
		fw_bytes_left = wcn->nv->size - fm_offset - 4;
		if (fw_bytes_left > WCN36XX_NV_FRAGMENT_SIZE) {
			msg_body.last_fragment = 0;
			msg_body.nv_img_buffer_size = WCN36XX_NV_FRAGMENT_SIZE;
		} else {
			msg_body.last_fragment = 1;
			msg_body.nv_img_buffer_size = fw_bytes_left;

			/* Do not forget update general message len */
			msg_body.header.len = sizeof(msg_body) + fw_bytes_left;

		}
		/* smd_buf must be protected with  mutex */
		mutex_lock(&wcn->smd_mutex);

		/* Add load NV request message header */
		memcpy(wcn->smd_buf, &msg_body,	sizeof(msg_body));

		/* Add NV body itself */
		/* Rework me */
		memcpy(wcn->smd_buf + sizeof(msg_body),
		       (void *)(&nv_d->table) + fm_offset,
		       msg_body.nv_img_buffer_size);

		ret = wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
		if (ret)
			return ret;

		msg_body.frag_number++;
		fm_offset += WCN36XX_NV_FRAGMENT_SIZE;

	} while (msg_body.last_fragment != 1);

	return ret;
}

int wcn36xx_smd_start(struct wcn36xx *wcn)
{
	struct wcn36xx_hal_mac_start_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_START_REQ);

	msg_body.params.type = DRIVER_TYPE_PRODUCTION;
	msg_body.params.len = 0;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL, "hal start type %d",
		    msg_body.params.type);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

static int wcn36xx_smd_start_rsp(struct wcn36xx *wcn, void *buf, size_t len)
{
	struct wcn36xx_hal_mac_start_rsp_msg *rsp;

	if (len < sizeof(*rsp))
		return -EIO;

	rsp = (struct wcn36xx_hal_mac_start_rsp_msg *)buf;

	if (WCN36XX_FW_MSG_RESULT_SUCCESS != rsp->start_rsp_params.status)
		return -EIO;

	memcpy(wcn->crm_version, rsp->start_rsp_params.crm_version,
	       WCN36XX_HAL_VERSION_LENGTH);
	memcpy(wcn->wlan_version, rsp->start_rsp_params.wlan_version,
	       WCN36XX_HAL_VERSION_LENGTH);

	/* null terminate the strings, just in case */
	wcn->crm_version[WCN36XX_HAL_VERSION_LENGTH] = '\0';
	wcn->wlan_version[WCN36XX_HAL_VERSION_LENGTH] = '\0';

	wcn->fw_revision = rsp->start_rsp_params.version.revision;
	wcn->fw_version = rsp->start_rsp_params.version.version;
	wcn->fw_minor = rsp->start_rsp_params.version.minor;
	wcn->fw_major = rsp->start_rsp_params.version.major;

	wcn36xx_info("firmware WLAN version '%s' and CRM version '%s'",
		     wcn->wlan_version, wcn->crm_version);

	wcn36xx_info("firmware API %u.%u.%u.%u, %u stations, %u bssids",
		     wcn->fw_major, wcn->fw_minor,
		     wcn->fw_version, wcn->fw_revision,
		     rsp->start_rsp_params.stations,
		     rsp->start_rsp_params.bssids);

	return 0;
}

int wcn36xx_smd_stop(struct wcn36xx *wcn)
{
	struct wcn36xx_hal_mac_stop_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_STOP_REQ);

	msg_body.stop_req_params.reason = HAL_STOP_TYPE_RF_KILL;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_init_scan(struct wcn36xx *wcn)
{
	struct wcn36xx_hal_init_scan_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_INIT_SCAN_REQ);

	msg_body.mode = HAL_SYS_MODE_SCAN;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL, "hal init scan mode %d", msg_body.mode);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_start_scan(struct wcn36xx *wcn, int ch)
{
	struct wcn36xx_hal_start_scan_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_START_SCAN_REQ);

	msg_body.scan_channel = (u8)ch;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL, "hal start scan channel %d",
		    msg_body.scan_channel);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}
int wcn36xx_smd_end_scan(struct wcn36xx *wcn, int ch)
{
	struct wcn36xx_hal_end_scan_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_END_SCAN_REQ);

	msg_body.scan_channel = (u8)ch;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL, "hal end scan channel %d",
		    msg_body.scan_channel);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}
int wcn36xx_smd_finish_scan(struct wcn36xx *wcn)
{
	struct wcn36xx_hal_finish_scan_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_FINISH_SCAN_REQ);

	msg_body.mode = HAL_SYS_MODE_SCAN;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL, "hal finish scan mode %d",
		    msg_body.mode);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_switch_channel(struct wcn36xx *wcn, int ch)
{
	struct wcn36xx_hal_switch_channel_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_CH_SWITCH_REQ);

	msg_body.channel_number = (u8)ch;
	msg_body.tx_mgmt_power = 0xbf;
	msg_body.max_tx_power = 0xbf;
	memcpy(msg_body.self_sta_mac_addr, wcn->addresses[0].addr, ETH_ALEN);

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

static void wcn36xx_smd_switch_channel_rsp(void *buf, size_t len)
{
	struct wcn36xx_hal_switch_channel_rsp_msg *rsp;
	rsp = (struct wcn36xx_hal_switch_channel_rsp_msg *)buf;
	wcn36xx_dbg(WCN36XX_DBG_HAL, "channel switched to: %d, status: %d", rsp->channel_number,
		     rsp->status);
}

int wcn36xx_smd_update_scan_params(struct wcn36xx *wcn)
{
	struct wcn36xx_hal_update_scan_params_req msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_UPDATE_SCAN_PARAM_REQ);

	/* TODO read this from config */
	msg_body.dot11d_enabled	= 0;
	msg_body.dot11d_resolved = 0;
	msg_body.channel_count = 26;
	msg_body.active_min_ch_time = 60;
	msg_body.active_max_ch_time = 120;
	msg_body.passive_min_ch_time = 60;
	msg_body.passive_max_ch_time = 110;
	msg_body.state = 0;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal update scan params channel_count %d",
		    msg_body.channel_count);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}
static int wcn36xx_smd_update_scan_params_rsp(void *buf, size_t len)
{
	struct wcn36xx_hal_update_scan_params_resp *rsp;

	rsp = (struct wcn36xx_hal_update_scan_params_resp *)buf;

	/* Remove the PNO version bit */
	rsp->status &= (~(WCN36XX_FW_MSG_PNO_VERSION_MASK));

	if (WCN36XX_FW_MSG_RESULT_SUCCESS != rsp->status) {
		wcn36xx_warn("error response from update scan");
		return -EIO;
	}
	return 0;
}

int wcn36xx_smd_add_sta_self(struct wcn36xx *wcn, u8 *addr, u32 status)
{
	struct wcn36xx_hal_add_sta_self_req msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_ADD_STA_SELF_REQ);

	memcpy(&msg_body.self_addr, addr, ETH_ALEN);
	msg_body.status = status;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal add sta self self_addr %pM status %d",
		    msg_body.self_addr, msg_body.status);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

static int wcn36xx_smd_add_sta_self_rsp(struct wcn36xx *wcn,
					void *buf,
					size_t len)
{
	struct wcn36xx_hal_add_sta_self_rsp_msg *rsp;

	if (len < sizeof(*rsp))
		return -EINVAL;

	rsp = (struct wcn36xx_hal_add_sta_self_rsp_msg *)buf;

	if (rsp->status != WCN36XX_FW_MSG_RESULT_SUCCESS) {
		wcn36xx_warn("hal add sta self failure: %d",
			     rsp->status);
		return -EIO;
	}

	wcn36xx_dbg(WCN36XX_DBG_HAL, "hal add sta self status %d "
		    "self_sta_index %d dpu_index %d",
		    rsp->status, rsp->self_sta_index, rsp->dpu_index);
	wcn->current_vif->self_sta_index = rsp->self_sta_index;
	wcn->current_vif->self_dpu_desc_index = rsp->dpu_index;

	return 0;
}

int wcn36xx_smd_delete_sta_self(struct wcn36xx *wcn, u8 *addr)
{
	struct wcn36xx_hal_del_sta_self_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_DEL_STA_SELF_REQ);

	memcpy(&msg_body.self_addr, addr, ETH_ALEN);

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_delete_sta(struct wcn36xx *wcn)
{
	struct wcn36xx_hal_delete_sta_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_DELETE_STA_REQ);

	msg_body.sta_index = 1;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal delete sta sta_index %d",
		    msg_body.sta_index);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);

}
int wcn36xx_smd_join(struct wcn36xx *wcn, const u8 *bssid, u8 *vif, u8 ch)
{
	struct wcn36xx_hal_join_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_JOIN_REQ);


	memcpy(&msg_body.bssid, bssid, ETH_ALEN);
	memcpy(&msg_body.self_sta_mac_addr, vif, ETH_ALEN);
	msg_body.channel = ch;
	if (conf_is_ht40_minus(&wcn->hw->conf))
		msg_body.secondary_channel_offset =
			PHY_DOUBLE_CHANNEL_HIGH_PRIMARY;
	else if (conf_is_ht40_plus(&wcn->hw->conf))
		msg_body.secondary_channel_offset =
			PHY_DOUBLE_CHANNEL_LOW_PRIMARY;
	else
		msg_body.secondary_channel_offset =
			PHY_SINGLE_CHANNEL_CENTERED;
	msg_body.link_state = WCN36XX_HAL_LINK_PREASSOC_STATE;

	msg_body.max_tx_power = 0xbf;
	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal join req bssid %pM self_sta_mac_addr %pM channel %d link_state %d",
		    msg_body.bssid, msg_body.self_sta_mac_addr,
		    msg_body.channel, msg_body.link_state);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_set_link_st(struct wcn36xx *wcn, const u8 *bssid,
			    const u8 *sta_mac,
			    enum wcn36xx_hal_link_state state)
{
	struct wcn36xx_hal_set_link_state_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_SET_LINK_ST_REQ);

	memcpy(&msg_body.bssid, bssid, ETH_ALEN);
	memcpy(&msg_body.self_mac_addr, sta_mac, ETH_ALEN);
	msg_body.state = state;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal set link state bssid %pM self_mac_addr %pM state %d",
		    msg_body.bssid, msg_body.self_mac_addr, msg_body.state);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

static void wcn36xx_smd_convert_sta_to_v1(struct wcn36xx *wcn,
					  const struct wcn36xx_hal_config_sta_params *orig,
					  struct wcn36xx_hal_config_sta_params_v1 *v1)
{
	/* convert orig to v1 format */
	memcpy(&v1->bssid, orig->bssid, ETH_ALEN);
	memcpy(&v1->mac, orig->mac, ETH_ALEN);
	v1->aid = orig->aid;
	v1->type = orig->type;
	v1->listen_interval = orig->listen_interval;
	v1->ht_capable = orig->ht_capable;

	v1->max_ampdu_size = orig->max_ampdu_size;
	v1->max_ampdu_density = orig->max_ampdu_density;
	v1->sgi_40mhz = orig->sgi_40mhz;
	v1->sgi_20Mhz = orig->sgi_20Mhz;

	memcpy(&v1->supported_rates, &orig->supported_rates,
	       sizeof(orig->supported_rates));
	v1->sta_index = orig->sta_index;
}

static int wcn36xx_smd_config_sta_v1(struct wcn36xx *wcn,
				     const struct wcn36xx_hal_config_sta_req_msg *orig)
{
	struct wcn36xx_hal_config_sta_req_msg_v1 msg_body;
	struct wcn36xx_hal_config_sta_params_v1 *sta = &msg_body.sta_params;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_CONFIG_STA_REQ);

	wcn36xx_smd_convert_sta_to_v1(wcn, &orig->sta_params,
				      &msg_body.sta_params);

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal config sta v1 action %d sta_index %d bssid_index %d bssid %pM type %d mac %pM aid %d",
		    sta->action, sta->sta_index, sta->bssid_index,
		    sta->bssid, sta->type, sta->mac, sta->aid);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_config_sta(struct wcn36xx *wcn, struct ieee80211_sta *sta,
			   const u8 *sta_mac)
{
	struct wcn36xx_hal_config_sta_req_msg msg;
	struct wcn36xx_hal_config_sta_params *sta_params;

	INIT_HAL_MSG(msg, WCN36XX_HAL_CONFIG_STA_REQ);

	sta_params = &msg.sta_params;
	sta_params->aid = wcn->aid;
	if (wcn->iftype == NL80211_IFTYPE_ADHOC ||
	    wcn->iftype == NL80211_IFTYPE_AP ||
	    wcn->iftype == NL80211_IFTYPE_MESH_POINT)
		sta_params->type = 1;
	else
		sta_params->type = 0;

	sta_params->short_preamble_supported = 0;

	memcpy(&sta_params->mac, sta_mac, ETH_ALEN);

	sta_params->listen_interval = 0x8;
	sta_params->rifs_mode = 0;
	wcn36xx_smd_set_sta_params(sta, sta_params);
	memcpy(&sta_params->supported_rates, &wcn->supported_rates,
		sizeof(wcn->supported_rates));

	sta_params->rmf = 0;
	sta_params->encrypt_type = 0;
	sta_params->action = 0;
	sta_params->uapsd = 0;
	sta_params->mimo_ps = WCN36XX_HAL_HT_MIMO_PS_STATIC;
	if (wcn->iftype == NL80211_IFTYPE_ADHOC ||
	    wcn->iftype == NL80211_IFTYPE_AP ||
	    wcn->iftype == NL80211_IFTYPE_MESH_POINT)
		sta_params->sta_index = 0xFF;
	else
		sta_params->sta_index = 1;

	sta_params->bssid_index = 0;
	sta_params->p2p = 0;

	if (!(wcn->fw_major == 1 &&
	      wcn->fw_minor == 2 &&
	      wcn->fw_version == 2 &&
	      wcn->fw_revision == 24))
		return wcn36xx_smd_config_sta_v1(wcn, &msg);

	PREPARE_HAL_BUF(wcn->smd_buf, msg);

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal config sta action %d sta_index %d bssid_index %d bssid %pM type %d mac %pM aid %d",
		    sta_params->action, sta_params->sta_index,
		    sta_params->bssid_index, sta_params->bssid,
		    sta_params->type, sta_params->mac,
		    sta_params->aid);

	return wcn36xx_smd_send_and_wait(wcn, msg.header.len);
}

static int wcn36xx_smd_config_sta_rsp(struct wcn36xx *wcn, void *buf,
				      size_t len)
{
	struct wcn36xx_hal_config_sta_rsp_msg *rsp;
	struct config_sta_rsp_params *params;

	if (len < sizeof(*rsp))
		return -EINVAL;

	rsp = (struct wcn36xx_hal_config_sta_rsp_msg *)buf;
	params = &rsp->params;

	if (params->status != WCN36XX_FW_MSG_RESULT_SUCCESS) {
		wcn36xx_warn("hal config sta response failure: %d",
			     params->status);
		return -EIO;
	}

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal config sta rsp status %d sta_index %d bssid_index %d p2p %d",
		    params->status, params->sta_index, params->bssid_index,
		    params->p2p);

	return 0;
}

static int wcn36xx_smd_join_rsp(void *buf, size_t len)
{
	struct wcn36xx_hal_join_rsp_msg *rsp;

	if (wcn36xx_smd_rsp_status_check(buf, len))
		return -EIO;

	rsp = (struct wcn36xx_hal_join_rsp_msg *)buf;

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal rsp join status %d tx_mgmt_power %d",
		    rsp->status, rsp->tx_mgmt_power);

	return 0;
}

static int wcn36xx_smd_config_bss_v1(struct wcn36xx *wcn,
				     const struct wcn36xx_hal_config_bss_req_msg *orig)
{
	struct wcn36xx_hal_config_bss_req_msg_v1 msg_body;
	struct wcn36xx_hal_config_bss_params_v1 *bss = &msg_body.bss_params;
	struct wcn36xx_hal_config_sta_params_v1 *sta = &bss->sta;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_CONFIG_BSS_REQ);

	/* convert orig to v1 */
	memcpy(&msg_body.bss_params.bssid,
	       &orig->bss_params.bssid, ETH_ALEN);
	memcpy(&msg_body.bss_params.self_mac_addr,
	       &orig->bss_params.self_mac_addr, ETH_ALEN);

	msg_body.bss_params.bss_type = orig->bss_params.bss_type;
	msg_body.bss_params.oper_mode = orig->bss_params.oper_mode;
	msg_body.bss_params.nw_type = orig->bss_params.nw_type;

	msg_body.bss_params.short_slot_time_supported =
		orig->bss_params.short_slot_time_supported;
	msg_body.bss_params.lla_coexist = orig->bss_params.lla_coexist;
	msg_body.bss_params.llb_coexist = orig->bss_params.llb_coexist;
	msg_body.bss_params.llg_coexist = orig->bss_params.llg_coexist;
	msg_body.bss_params.ht20_coexist = orig->bss_params.ht20_coexist;
	msg_body.bss_params.lln_non_gf_coexist = orig->bss_params.lln_non_gf_coexist;

	msg_body.bss_params.lsig_tx_op_protection_full_support = orig->bss_params.lsig_tx_op_protection_full_support;
	msg_body.bss_params.rifs_mode = orig->bss_params.rifs_mode;
	msg_body.bss_params.beacon_interval = orig->bss_params.beacon_interval;
	msg_body.bss_params.dtim_period = orig->bss_params.dtim_period;
	msg_body.bss_params.tx_channel_width_set = orig->bss_params.tx_channel_width_set;
	msg_body.bss_params.oper_channel = orig->bss_params.oper_channel;
	msg_body.bss_params.ext_channel = orig->bss_params.ext_channel;

	msg_body.bss_params.reserved = orig->bss_params.reserved;

	memcpy(&msg_body.bss_params.ssid,
	       &orig->bss_params.ssid,
	       sizeof(orig->bss_params.ssid));

	msg_body.bss_params.action = orig->bss_params.action;
	msg_body.bss_params.rateset = orig->bss_params.rateset;
	msg_body.bss_params.ht = orig->bss_params.ht;
	msg_body.bss_params.obss_prot_enabled = orig->bss_params.obss_prot_enabled;
	msg_body.bss_params.rmf = orig->bss_params.rmf;
	msg_body.bss_params.ht_oper_mode = orig->bss_params.ht_oper_mode;
	msg_body.bss_params.dual_cts_protection = orig->bss_params.dual_cts_protection;

	msg_body.bss_params.max_probe_resp_retry_limit = orig->bss_params.max_probe_resp_retry_limit;
	msg_body.bss_params.hidden_ssid = orig->bss_params.hidden_ssid;
	msg_body.bss_params.proxy_probe_resp = orig->bss_params.proxy_probe_resp;
	msg_body.bss_params.edca_params_valid = orig->bss_params.edca_params_valid;

	memcpy(&msg_body.bss_params.acbe,
	       &orig->bss_params.acbe,
	       sizeof(orig->bss_params.acbe));
	memcpy(&msg_body.bss_params.acbk,
	       &orig->bss_params.acbk,
	       sizeof(orig->bss_params.acbk));
	memcpy(&msg_body.bss_params.acvi,
	       &orig->bss_params.acvi,
	       sizeof(orig->bss_params.acvi));
	memcpy(&msg_body.bss_params.acvo,
	       &orig->bss_params.acvo,
	       sizeof(orig->bss_params.acvo));

	msg_body.bss_params.ext_set_sta_key_param_valid =
		orig->bss_params.ext_set_sta_key_param_valid;

	memcpy(&msg_body.bss_params.ext_set_sta_key_param,
	       &orig->bss_params.ext_set_sta_key_param,
	       sizeof(orig->bss_params.acvo));

	msg_body.bss_params.wcn36xx_hal_persona = orig->bss_params.wcn36xx_hal_persona;
	msg_body.bss_params.spectrum_mgt_enable = orig->bss_params.spectrum_mgt_enable;
	msg_body.bss_params.tx_mgmt_power = orig->bss_params.tx_mgmt_power;
	msg_body.bss_params.max_tx_power = orig->bss_params.max_tx_power;

	wcn36xx_smd_convert_sta_to_v1(wcn, &orig->bss_params.sta,
				      &msg_body.bss_params.sta);

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal config bss v1 bssid %pM self_mac_addr %pM bss_type %d oper_mode %d nw_type %d",
		    bss->bssid, bss->self_mac_addr, bss->bss_type,
		    bss->oper_mode, bss->nw_type);

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "- sta bssid %pM action %d sta_index %d bssid_index %d aid %d type %d mac %pM",
		    sta->bssid, sta->action, sta->sta_index,
		    sta->bssid_index, sta->aid, sta->type, sta->mac);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_config_bss(struct wcn36xx *wcn, struct ieee80211_sta *sta,
			   enum nl80211_iftype type, const u8 *bssid,
			   bool update, u16 beacon_interval)
{
	struct wcn36xx_hal_config_bss_req_msg msg;
	struct wcn36xx_hal_config_bss_params *bss;
	struct wcn36xx_hal_config_sta_params *sta_params;

	INIT_HAL_MSG(msg, WCN36XX_HAL_CONFIG_BSS_REQ);

	bss = &msg.bss_params;
	sta_params = &bss->sta;

	WARN_ON(is_zero_ether_addr(bssid));

	memcpy(&bss->bssid, bssid, ETH_ALEN);

	memcpy(&bss->self_mac_addr, &wcn->addresses[0], ETH_ALEN);

	if (type == NL80211_IFTYPE_STATION) {
		bss->bss_type = WCN36XX_HAL_INFRASTRUCTURE_MODE;

		/* STA */
		bss->oper_mode = 1;

	} else if (type == NL80211_IFTYPE_AP) {
		bss->bss_type = WCN36XX_HAL_INFRA_AP_MODE;

		/* AP */
		bss->oper_mode = 0;
	} else if (type == NL80211_IFTYPE_ADHOC ||
		   type == NL80211_IFTYPE_MESH_POINT) {
		bss->bss_type = WCN36XX_HAL_IBSS_MODE;

		/* STA */
		bss->oper_mode = 1;
	} else {
		wcn36xx_warn("Unknown type for bss config: %d", type);
	}

	bss->nw_type = WCN36XX_HAL_11N_NW_TYPE;
	bss->short_slot_time_supported = 0;
	bss->lla_coexist = 0;
	bss->llb_coexist = 0;
	bss->llg_coexist = 0;
	bss->ht20_coexist = 0;
	bss->lln_non_gf_coexist = 0;
	bss->lsig_tx_op_protection_full_support = 0;
	bss->rifs_mode = 0;
	bss->beacon_interval = beacon_interval;
	bss->dtim_period = wcn->dtim_period;
	bss->tx_channel_width_set = 0;
	bss->oper_channel = wcn->ch;
	if (conf_is_ht40_minus(&wcn->hw->conf))
		bss->ext_channel = WCN36XX_HAL_HT_SECONDARY_CHAN_OFF_DOWN;
	else if (conf_is_ht40_plus(&wcn->hw->conf))
		bss->ext_channel = WCN36XX_HAL_HT_SECONDARY_CHAN_OFF_UP;
	else
		bss->ext_channel = WCN36XX_HAL_HT_SECONDARY_CHAN_OFF_NONE;
	bss->reserved = 0;

	sta_params->aid = wcn->aid;
	sta_params->type = 0;
	sta_params->short_preamble_supported = 0;
	memcpy(&sta_params->mac, &wcn->addresses[0], ETH_ALEN);

	sta_params->listen_interval = 8;
	sta_params->rifs_mode = 0;

	wcn36xx_smd_set_sta_params(sta, sta_params);
	memcpy(&sta_params->supported_rates, &wcn->supported_rates,
	       sizeof(wcn->supported_rates));

	sta_params->encrypt_type = wcn->encrypt_type;
	sta_params->action = 0;
	sta_params->uapsd = 0;
	sta_params->mimo_ps = 0;
	sta_params->sta_index = 0xff;
	sta_params->bssid_index = 0;
	sta_params->p2p = 0;

	/* wcn->ssid is only valid in AP and IBSS mode */
	bss->ssid.length = wcn->ssid.length;
	memcpy(bss->ssid.ssid, wcn->ssid.ssid, wcn->ssid.length);

	bss->action = 0;

	/* FIXME: set rateset */

	bss->ht = 0;
	bss->obss_prot_enabled = 0;
	bss->rmf = 0;
	bss->ht_oper_mode = 0;
	bss->dual_cts_protection = 0;
	bss->max_probe_resp_retry_limit = 0;
	bss->hidden_ssid = 0;
	bss->proxy_probe_resp = 0;
	bss->edca_params_valid = 0;

	/* FIXME: set acbe, acbk, acvi and acvo */

	bss->ext_set_sta_key_param_valid = 0;

	/* FIXME: set ext_set_sta_key_param */

	bss->wcn36xx_hal_persona = 1;
	bss->spectrum_mgt_enable = 0;
	bss->tx_mgmt_power = 0;
	bss->max_tx_power = 0x10;

	if (update) {
		sta_params->bssid_index = 0;
		bss->action = 1;
	} else {
		sta_params->bssid_index = 0xff;
		bss->action = 0;
	}
	if (!(wcn->fw_major == 1 &&
		wcn->fw_minor == 2 &&
		wcn->fw_version == 2 &&
		wcn->fw_revision == 24))
		return wcn36xx_smd_config_bss_v1(wcn, &msg);

	PREPARE_HAL_BUF(wcn->smd_buf, msg);

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal config bss bssid %pM self_mac_addr %pM bss_type %d oper_mode %d nw_type %d",
		    bss->bssid, bss->self_mac_addr, bss->bss_type,
		    bss->oper_mode, bss->nw_type);

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "- sta bssid %pM action %d sta_index %d bssid_index %d aid %d type %d mac %pM",
		    sta_params->bssid, sta_params->action,
		    sta_params->sta_index, sta_params->bssid_index,
		    sta_params->aid, sta_params->type,
		    sta_params->mac);

	return wcn36xx_smd_send_and_wait(wcn, msg.header.len);
}

static int wcn36xx_smd_config_bss_rsp(struct wcn36xx *wcn, void *buf, size_t len)
{
	struct wcn36xx_hal_config_bss_rsp_msg *rsp;
	struct wcn36xx_hal_config_bss_rsp_params *params;

	if (len < sizeof(*rsp))
		return -EINVAL;

	rsp = (struct wcn36xx_hal_config_bss_rsp_msg *)buf;
	params = &rsp->bss_rsp_params;

	if (params->status != WCN36XX_FW_MSG_RESULT_SUCCESS) {
		wcn36xx_warn("hal config bss response failure: %d",
			     params->status);
		return -EIO;
	}

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal config bss rsp status %d bss_idx %d dpu_desc_index %d"
		    " sta_idx %d self_idx %d bcast_idx %d mac %pM"
		    " power %d ucast_dpu_signature %d",
		    params->status, params->bss_index, params->dpu_desc_index,
		    params->bss_sta_index, params->bss_self_sta_index,
		    params->bss_bcast_sta_idx, params->mac,
		    params->tx_mgmt_power, params->ucast_dpu_signature);

	wcn->current_vif->sta_index =  params->bss_sta_index;
	wcn->current_vif->dpu_desc_index = params->dpu_desc_index;
	wcn->current_vif->ucast_dpu_signature = params->ucast_dpu_signature;
	return 0;
}

int wcn36xx_smd_delete_bss(struct wcn36xx *wcn)
{
	struct wcn36xx_hal_delete_bss_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_DELETE_BSS_REQ);

	msg_body.bss_index = 0;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL, "hal delete bss %d", msg_body.bss_index);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}
int wcn36xx_smd_send_beacon(struct wcn36xx *wcn, struct sk_buff *skb_beacon,
			    u16 tim_off, u16 p2p_off)
{
	struct wcn36xx_hal_send_beacon_req_msg msg_body;
	INIT_HAL_MSG(msg_body, WCN36XX_HAL_SEND_BEACON_REQ);

	/* TODO need to find out why this is needed? */
	msg_body.beacon_length = skb_beacon->len + 6;

	/* TODO make this as a const */
	if (BEACON_TEMPLATE_SIZE > msg_body.beacon_length) {
		memcpy(&msg_body.beacon, &skb_beacon->len, sizeof(u32));
		memcpy(&(msg_body.beacon[4]), skb_beacon->data,
		       skb_beacon->len);
	} else {
		wcn36xx_error("Beacon is to big: beacon size=%d",
			      msg_body.beacon_length);
		return -ENOMEM;
	}
	memcpy(&msg_body.bssid, &wcn->addresses[0], ETH_ALEN);

	/* TODO need to find out why this is needed? */
	msg_body.tim_ie_offset = tim_off+4;
	msg_body.p2p_ie_offset = p2p_off;
	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal send beacon beacon_length %d",
		    msg_body.beacon_length);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
};

int wcn36xx_smd_update_proberesp_tmpl(struct wcn36xx *wcn, struct sk_buff *skb)
{
	struct wcn36xx_hal_send_probe_resp_req_msg msg;

	INIT_HAL_MSG(msg, WCN36XX_HAL_UPDATE_PROBE_RSP_TEMPLATE_REQ);

	/* // TODO need to find out why this is needed? */
	/* msg_body.beacon_length = skb_beacon->len + 6; */

	if (skb->len > BEACON_TEMPLATE_SIZE) {
		wcn36xx_warn("probe response template is too big: %d",
			     skb->len);
		return -E2BIG;
	}

	msg.probe_resp_template_len = skb->len;
	memcpy(&msg.probe_resp_template, skb->data, skb->len);

	memcpy(&msg.bssid, &wcn->addresses[0], ETH_ALEN);

	PREPARE_HAL_BUF(wcn->smd_buf, msg);

	wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "hal update probe rsp len %d bssid %pM",
		    msg.probe_resp_template_len, msg.bssid);

	return wcn36xx_smd_send_and_wait(wcn, msg.header.len);
};

int wcn36xx_smd_set_stakey(struct wcn36xx *wcn,
			   enum ani_ed_type enc_type,
			   u8 keyidx,
			   u8 keylen,
			   u8 *key)
{
	struct wcn36xx_hal_set_sta_key_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_SET_STAKEY_REQ);

	msg_body.set_sta_key_params.sta_index = wcn->current_vif->sta_index;
	msg_body.set_sta_key_params.enc_type = enc_type;

	msg_body.set_sta_key_params.key[0].id = keyidx;
	msg_body.set_sta_key_params.key[0].unicast = 1;
	msg_body.set_sta_key_params.key[0].direction = WCN36XX_HAL_TX_RX;
	msg_body.set_sta_key_params.key[0].pae_role = 0;
	msg_body.set_sta_key_params.key[0].length = keylen;
	memcpy(msg_body.set_sta_key_params.key[0].key, key, keylen);
	msg_body.set_sta_key_params.single_tid_rc = 1;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_set_bsskey(struct wcn36xx *wcn,
			   enum ani_ed_type enc_type,
			   u8 keyidx,
			   u8 keylen,
			   u8 *key)
{
	struct wcn36xx_hal_set_bss_key_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_SET_BSSKEY_REQ);
	msg_body.bss_idx = 0;
	msg_body.enc_type = enc_type;
	msg_body.num_keys = 1;
	msg_body.keys[0].id = keyidx;
	msg_body.keys[0].unicast = 0;
	msg_body.keys[0].direction = WCN36XX_HAL_RX_ONLY;
	msg_body.keys[0].pae_role = 0;
	msg_body.keys[0].length = keylen;
	memcpy(msg_body.keys[0].key, key, keylen);

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_remove_stakey(struct wcn36xx *wcn,
			      enum ani_ed_type enc_type,
			      u8 keyidx)
{
	struct wcn36xx_hal_remove_sta_key_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_RMV_STAKEY_REQ);
	msg_body.sta_idx = wcn->current_vif->sta_index;
	msg_body.enc_type = enc_type;
	msg_body.key_id = keyidx;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}
int wcn36xx_smd_remove_bsskey(struct wcn36xx *wcn,
			      enum ani_ed_type enc_type,
			      u8 keyidx)
{
	struct wcn36xx_hal_remove_bss_key_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_RMV_BSSKEY_REQ);
	msg_body.bss_idx = 0;
	msg_body.enc_type = enc_type;
	msg_body.key_id = keyidx;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_enter_bmps(struct wcn36xx *wcn, u64 tbtt)
{
	struct wcn36xx_hal_enter_bmps_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_ENTER_BMPS_REQ);

	msg_body.bss_index = 0;
	msg_body.tbtt = tbtt;
	msg_body.dtim_period = wcn->dtim_period;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_exit_bmps(struct wcn36xx *wcn)
{
	struct wcn36xx_hal_enter_bmps_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_EXIT_BMPS_REQ);

	msg_body.bss_index = 0;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

/* Notice: This function should be called after associated, or else it
 * will be invalid
 */
int wcn36xx_smd_keep_alive_req(struct wcn36xx *wcn, int packet_type)
{
	struct wcn36xx_hal_keep_alive_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_KEEP_ALIVE_REQ);

	if (packet_type == WCN36XX_HAL_KEEP_ALIVE_NULL_PKT) {
		msg_body.bss_index = 0;
		msg_body.packet_type = WCN36XX_HAL_KEEP_ALIVE_NULL_PKT;
		msg_body.time_period = WCN36XX_KEEP_ALIVE_TIME_PERIOD;
	} else if (packet_type == WCN36XX_HAL_KEEP_ALIVE_UNSOLICIT_ARP_RSP) {
		/* TODO: it also support ARP response type */
	} else {
		wcn36xx_warn("unknow keep alive packet type %d", packet_type);
		return -1;
	}

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_dump_cmd_req(struct wcn36xx *wcn, u32 arg1, u32 arg2,
			     u32 arg3, u32 arg4, u32 arg5)
{
	struct wcn36xx_hal_dump_cmd_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_DUMP_COMMAND_REQ);

	msg_body.arg1 = arg1;
	msg_body.arg2 = arg2;
	msg_body.arg3 = arg3;
	msg_body.arg4 = arg4;
	msg_body.arg5 = arg5;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

static inline void set_feat_caps(u32 *bitmap,
				 enum place_holder_in_cap_bitmap cap)
{
	int arr_idx, bit_idx;
	if (cap < 0 || cap > 127) {
		wcn36xx_warn("error cap idx %d", cap);
	} else {
		arr_idx = cap / 32;
		bit_idx = cap % 32;
		bitmap[arr_idx] |= (1 << bit_idx);
	}
}

static inline int get_feat_caps(u32 *bitmap,
				enum place_holder_in_cap_bitmap cap)
{
	int arr_idx, bit_idx;
	int ret = 0;

	if (cap < 0 || cap > 127) {
		wcn36xx_warn("error cap idx %d", cap);
		return -1;
	} else {
		arr_idx = cap / 32;
		bit_idx = cap % 32;
		ret = (bitmap[arr_idx] & (1 << bit_idx)) ? 1 : 0;
		return ret;
	}
}

static inline void clear_feat_caps(u32 *bitmap,
				enum place_holder_in_cap_bitmap cap)
{
	int arr_idx, bit_idx;

	if (cap < 0 || cap > 127) {
		wcn36xx_warn("error cap idx %d", cap);
	} else {
		arr_idx = cap / 32;
		bit_idx = cap % 32;
		bitmap[arr_idx] &= ~(1 << bit_idx);
	}
}

int wcn36xx_smd_feature_caps_exchange(struct wcn36xx *wcn)
{
	struct wcn36xx_hal_feat_caps_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_FEATURE_CAPS_EXCHANGE_REQ);

	set_feat_caps(msg_body.feat_caps, STA_POWERSAVE);

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body);

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

/* FW sends its capability bitmap as a response */
int wcn36xx_smd_feature_caps_exchange_rsp(void *buf, size_t len)
{
	/* TODO: print the caps of rsp for comapre */
	if (wcn36xx_smd_rsp_status_check(buf, len)) {
		wcn36xx_warn("error response for caps exchange");
	}
	return 0;
}

static void wcn36xx_smd_notify(void *data, unsigned event)
{
	struct wcn36xx *wcn = (struct wcn36xx *)data;

	switch (event) {
	case SMD_EVENT_OPEN:
		complete(&wcn->smd_compl);
		break;
	case SMD_EVENT_DATA:
		queue_work(wcn->wq, &wcn->smd_work);
		break;
	case SMD_EVENT_CLOSE:
		break;
	case SMD_EVENT_STATUS:
		break;
	case SMD_EVENT_REOPEN_READY:
		break;
	default:
		wcn36xx_error("SMD_EVENT (%d) not supported", event);
		break;
	}
}

static int wcn36xx_smd_tx_compl_ind(struct wcn36xx *wcn, void *buf, size_t len)
{
	struct wcn36xx_hal_tx_compl_ind_msg *rsp = buf;

	if (len != sizeof(*rsp)) {
		wcn36xx_warn("Bad TX complete indication");
		return -EIO;
	}

	wcn36xx_dxe_tx_ack_ind(wcn, rsp->status);

	return 0;
}

static int wcn36xx_smd_missed_beacon_ind(struct wcn36xx *wcn,
					 void *buf,
					 size_t len)
{
	struct ieee80211_vif *vif = container_of((void *)wcn->current_vif,
						 struct ieee80211_vif,
						 drv_priv);
	mutex_lock(&wcn->pm_mutex);
	/*
	 * In suspended state mac80211 is still sleeping and that means we
	 * cannot notify it about connection lost. Wait until resume and
	 * then notify mac80211 about it.
	 */
	if (wcn->is_suspended) {
		wcn36xx_dbg(WCN36XX_DBG_HAL,
		    "postpone connection lost notification");
		wcn->is_con_lost_pending = true;
	} else {
		wcn36xx_dbg(WCN36XX_DBG_HAL, "beacon missed");
		ieee80211_connection_loss(vif);
	}
	mutex_unlock(&wcn->pm_mutex);
	return 0;
}

static void wcn36xx_smd_rsp_process(struct wcn36xx *wcn, void *buf, size_t len)
{
	struct wcn36xx_hal_msg_header *msg_header = buf;

	wcn36xx_dbg_dump(WCN36XX_DBG_SMD_DUMP, "SMD <<< ", buf, len);
	switch (msg_header->msg_type) {
	case WCN36XX_HAL_START_RSP:
		wcn36xx_smd_start_rsp(wcn, buf, len);
		break;
	case WCN36XX_HAL_CONFIG_STA_RSP:
		wcn36xx_smd_config_sta_rsp(wcn, buf, len);
		break;
	case WCN36XX_HAL_CONFIG_BSS_RSP:
		wcn36xx_smd_config_bss_rsp(wcn, buf, len);
		break;
	case WCN36XX_HAL_STOP_RSP:
	case WCN36XX_HAL_ADD_STA_SELF_RSP:
		wcn36xx_smd_add_sta_self_rsp(wcn, buf, len);
		break;
	case WCN36XX_HAL_DEL_STA_SELF_RSP:
	case WCN36XX_HAL_DELETE_STA_RSP:
	case WCN36XX_HAL_INIT_SCAN_RSP:
	case WCN36XX_HAL_START_SCAN_RSP:
	case WCN36XX_HAL_END_SCAN_RSP:
	case WCN36XX_HAL_FINISH_SCAN_RSP:
	case WCN36XX_HAL_DOWNLOAD_NV_RSP:
	case WCN36XX_HAL_DELETE_BSS_RSP:
	case WCN36XX_HAL_SEND_BEACON_RSP:
	case WCN36XX_HAL_SET_LINK_ST_RSP:
	case WCN36XX_HAL_UPDATE_PROBE_RSP_TEMPLATE_RSP:
	case WCN36XX_HAL_SET_BSSKEY_RSP:
	case WCN36XX_HAL_SET_STAKEY_RSP:
	case WCN36XX_HAL_RMV_STAKEY_RSP:
	case WCN36XX_HAL_RMV_BSSKEY_RSP:
	case WCN36XX_HAL_ENTER_BMPS_RSP:
	case WCN36XX_HAL_EXIT_BMPS_RSP:
	case WCN36XX_HAL_KEEP_ALIVE_RSP:
	case WCN36XX_HAL_DUMP_COMMAND_RSP:
		if (wcn36xx_smd_rsp_status_check(buf, len)) {
			wcn36xx_warn("error response from hal request %d",
				     msg_header->msg_type);
		}
		break;
	case WCN36XX_HAL_JOIN_RSP:
		wcn36xx_smd_join_rsp(buf, len);
		break;
	case WCN36XX_HAL_UPDATE_SCAN_PARAM_RSP:
		wcn36xx_smd_update_scan_params_rsp(buf, len);
		break;
	case WCN36XX_HAL_CH_SWITCH_RSP:
		wcn36xx_smd_switch_channel_rsp(buf, len);
		break;
	case WCN36XX_HAL_OTA_TX_COMPL_IND:
		wcn36xx_smd_tx_compl_ind(wcn, buf, len);
		break;
	case WCN36XX_HAL_MISSED_BEACON_IND:
		wcn36xx_smd_missed_beacon_ind(wcn, buf, len);
		break;
	case WCN36XX_HAL_FEATURE_CAPS_EXCHANGE_RSP:
		wcn36xx_smd_feature_caps_exchange_rsp(buf, len);
		break;
	default:
		wcn36xx_error("SMD_EVENT (%d) not supported", msg_header->msg_type);
	}
}

static void wcn36xx_smd_work(struct work_struct *work)
{
	int msg_len;
	int avail;
	void *msg;
	int ret;
	struct wcn36xx *wcn =
		container_of(work, struct wcn36xx, smd_work);

	if (!wcn)
		return;

	while (1) {
		msg_len = smd_cur_packet_size(wcn->smd_ch);
		if (0 == msg_len) {
			complete(&wcn->smd_compl);
			return;
		}

		avail = smd_read_avail(wcn->smd_ch);
		if (avail < msg_len) {
			complete(&wcn->smd_compl);
			return;
		}
		msg = kmalloc(msg_len, GFP_KERNEL);
		if (NULL == msg) {
			complete(&wcn->smd_compl);
			return;
		}
		ret = smd_read(wcn->smd_ch, msg, msg_len);
		if (ret != msg_len) {
			complete(&wcn->smd_compl);
			return;
		}
		wcn36xx_smd_rsp_process(wcn, msg, msg_len);
		kfree(msg);
	}
}

int wcn36xx_smd_open(struct wcn36xx *wcn)
{
	int ret, left;

	INIT_WORK(&wcn->smd_work, wcn36xx_smd_work);
	init_completion(&wcn->smd_compl);

	ret = smd_named_open_on_edge("WLAN_CTRL", SMD_APPS_WCNSS,
				     &wcn->smd_ch, wcn, wcn36xx_smd_notify);
	if (ret) {
		wcn36xx_error("smd_named_open_on_edge failed: %d", ret);
		return ret;
	}

	left = wait_for_completion_interruptible_timeout(&wcn->smd_compl,
							 msecs_to_jiffies(SMD_MSG_TIMEOUT));
	if (left <= 0) {
		wcn36xx_error("timeout waiting for smd open: %d", ret);
		return left;
	}

	return 0;
}

void wcn36xx_smd_close(struct wcn36xx *wcn)
{
	smd_close(wcn->smd_ch);
}
