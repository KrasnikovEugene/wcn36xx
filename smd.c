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

#include "smd.h"

int wcn36xx_smd_send_and_wait(struct wcn36xx *wcn, size_t len)
{
	int avail;

	init_completion(&wcn->smd_compl);
	avail = smd_write_avail(wcn->smd_ch);

	wcn36xx_dbg_dump("SMD >>> ", wcn->smd_buf, len);
	if (avail >= len) {
		avail = smd_write(wcn->smd_ch, wcn->smd_buf, len);
		if (avail != len) {
			wcn36xx_error("Cannot write to SMD channel");
			return -EAGAIN;
		}
	} else {
		wcn36xx_error("SMD channel can accept only %d bytes", avail);
		return -ENOMEM;
	}

	if (wait_for_completion_timeout(&wcn->smd_compl,
		msecs_to_jiffies(SMD_MSG_TIMEOUT)) <= 0) {
		wcn36xx_error("Timeout while waiting SMD response");
		return -ETIME;
	}
	return 0;
}

#define INIT_HAL_MSG(msg_body, type) \
	memset(&msg_body, 0, sizeof(msg_body)); \
	msg_body.header.msgType = type; \
	msg_body.header.msgVersion = WCN36XX_HAL_MSG_VERSION0; \
	msg_body.header.len = sizeof(msg_body);

#define PREPARE_HAL_BUF(send_buf, msg_body) \
	memset(send_buf, 0, msg_body.header.len); \
	memcpy(send_buf, &msg_body, sizeof(msg_body));

int wcn36xx_smd_rsp_status_check(void *buf, size_t len)
{
	struct wcn36xx_fw_msg_status_rsp * rsp;
	if (len < sizeof(struct wcn36xx_fw_msg_header) +
		sizeof(struct wcn36xx_fw_msg_status_rsp))
		return -EIO;
	rsp = (struct wcn36xx_fw_msg_status_rsp *)
		(buf + sizeof(struct wcn36xx_fw_msg_header));

	if (WCN36XX_FW_MSG_RESULT_SUCCESS != rsp->status) {
		return -EIO;
	}
	return 0;
}

#define INIT_MSG(msg_header, msg_body, type) \
	msg_header.msg_type = type; \
	msg_header.msg_ver = WCN36XX_FW_MSG_VER0; \
	msg_header.msg_len = sizeof(msg_header)+sizeof(*msg_body); \
	memset(msg_body, 0, sizeof(*msg_body));

#define INIT_MSG_S(msg_header, type) \
	msg_header.msg_type = type; \
	msg_header.msg_ver = WCN36XX_FW_MSG_VER0; \
	msg_header.msg_len = sizeof(msg_header);

#define PREPARE_BUF(send_buf, msg_header, msg_body) \
	memset(send_buf, 0, msg_header.msg_len); \
	memcpy(send_buf, &msg_header, sizeof(msg_header)); \
	memcpy((void*)(send_buf + sizeof(msg_header)), msg_body, sizeof(*msg_body));		\

#define PREPARE_BUF_S(send_buf, msg_header) \
	memset(send_buf, 0, msg_header.msg_len); \
	memcpy(send_buf, &msg_header, sizeof(msg_header)); \

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
	INIT_HAL_MSG(msg_body, WCN36XX_HAL_DOWNLOAD_NV_REQ)

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

			// Do not forget update general message len
			msg_body.header.len = sizeof(msg_body) + fw_bytes_left;

		}

		// Add load NV request message header
		memcpy((void*)(wcn->smd_buf), &msg_body,
			sizeof(msg_body));

		// Add NV body itself
		// Rework me
		memcpy((void*)(wcn->smd_buf + sizeof(msg_body)),
			(void*)((void*)(&nv_d->table) + fm_offset), msg_body.nv_img_buffer_size);

		ret = wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
		if(ret)	return ret;

		msg_body.frag_number++;
		fm_offset += WCN36XX_NV_FRAGMENT_SIZE;

	} while(msg_body.last_fragment != 1);

	return ret;
}

int wcn36xx_smd_start(struct wcn36xx *wcn)
{
	struct wcn36xx_hal_mac_start_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_START_REQ)

	msg_body.params.type = DRIVER_TYPE_PRODUCTION;
	msg_body.params.len = 0;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body)

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}
static int wcn36xx_smd_start_rsp(void *buf, size_t len)
{
	struct wcn36xx_hal_mac_start_rsp_msg * rsp;

	if (len < sizeof(*rsp))
		return -EIO;

	rsp = (struct wcn36xx_hal_mac_start_rsp_msg *)buf;

	if (WCN36XX_FW_MSG_RESULT_SUCCESS != rsp->start_rsp_params.status)
		return -EIO;
	wcn36xx_info("WLAN ver=%s, CRM ver=%s",
		rsp->start_rsp_params.wlan_version, rsp->start_rsp_params.crm_version);
	return 0;
}

int wcn36xx_smd_init_scan(struct wcn36xx *wcn)
{
	struct wcn36xx_hal_init_scan_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_INIT_SCAN_REQ)

	msg_body.mode = HAL_SYS_MODE_SCAN;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body)

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_start_scan(struct wcn36xx *wcn, u8 ch)
{
	struct wcn36xx_hal_start_scan_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_START_SCAN_REQ)

	msg_body.scan_channel = ch;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body)

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}
int wcn36xx_smd_end_scan(struct wcn36xx *wcn, u8 ch)
{
	struct wcn36xx_hal_end_scan_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_END_SCAN_REQ)

	msg_body.scan_channel = ch;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body)

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}
int wcn36xx_smd_finish_scan(struct wcn36xx *wcn)
{
	struct wcn36xx_hal_finish_scan_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_FINISH_SCAN_REQ)

	msg_body.mode = HAL_SYS_MODE_SCAN;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body)

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}
int wcn36xx_smd_update_scan_params(struct wcn36xx *wcn){
	struct wcn36xx_hal_update_scan_params_req msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_UPDATE_SCAN_PARAM_REQ)

	// TODO read this from config
	msg_body.dot11d_enabled	= 0;
	msg_body.dot11d_resolved = 0;
	msg_body.channel_count = 26;
	msg_body.active_min_ch_time = 60;
	msg_body.active_max_ch_time = 120;
	msg_body.passive_min_ch_time = 60;
	msg_body.passive_max_ch_time = 110;
	msg_body.state = 0;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body)

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}
static int wcn36xx_smd_update_scan_params_rsp(void *buf, size_t len)
{
	struct  wcn36xx_hal_update_scan_params_resp * rsp;

	rsp = (struct wcn36xx_hal_update_scan_params_resp *)buf;

	wcn36xx_info("Scan params stattus=0x%x",rsp->status);
	return 0;
}

int wcn36xx_smd_add_sta_self(struct wcn36xx *wcn, struct mac_address addr, u32 status)
{
	struct wcn36xx_hal_add_sta_self_req msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_ADD_STA_SELF_REQ)

	memcpy(&msg_body.self_addr, &addr, ETH_ALEN);
	msg_body.status = status;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body)

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_join(struct wcn36xx *wcn, u8 *bssid, u8 *vif, u8 ch)
{
	struct wcn36xx_hal_join_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_JOIN_REQ)


	memcpy(&msg_body.bssid, bssid, ETH_ALEN);
	memcpy(&msg_body.self_sta_mac_addr, vif, ETH_ALEN);
	msg_body.channel = ch;
	msg_body.link_state = WCN36XX_HAL_LINK_PREASSOC_STATE;

	msg_body.max_tx_power = 0xbf;
	PREPARE_HAL_BUF(wcn->smd_buf, msg_body)

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}
int wcn36xx_smd_set_link_st(struct wcn36xx *wcn, u8 *bssid, u8 *sta_mac, enum wcn36xx_hal_link_state state)
{
	struct wcn36xx_hal_set_link_state_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_SET_LINK_ST_REQ)

	memcpy(&msg_body.bssid, bssid, ETH_ALEN);
	memcpy(&msg_body.self_mac_addr, sta_mac, ETH_ALEN);
	msg_body.state = state;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body)

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}

int wcn36xx_smd_config_sta(struct wcn36xx *wcn, u8 *bssid, u16 ass_id, u8 *sta_mac)
{
	struct wcn36xx_hal_config_sta_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_CONFIG_STA_REQ)

	memcpy(&msg_body.u.sta_params.bssid, bssid, ETH_ALEN);
	memcpy(&msg_body.u.sta_params.mac, sta_mac, ETH_ALEN);
	msg_body.u.sta_params.aid = 1;
	msg_body.u.sta_params.type = 0;
	msg_body.u.sta_params.listen_interval = 0x8;
	msg_body.u.sta_params.ht_capable = 1;

	msg_body.u.sta_params.max_ampdu_size = 3;
	msg_body.u.sta_params.max_ampdu_density = 5;
	msg_body.u.sta_params.sgi_40mhz = 1;
	msg_body.u.sta_params.sgi_20Mhz = 1;

	memcpy(&msg_body.u.sta_params.supported_rates, &wcn->supported_rates,
		sizeof(wcn->supported_rates));
	msg_body.u.sta_params.sta_index = 1;

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body)

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}
static int wcn36xx_smd_join_rsp(void *buf, size_t len)
{
	struct wcn36xx_hal_join_rsp_msg * rsp;

	if(wcn36xx_smd_rsp_status_check(buf, len))
		return -EIO;

	rsp = (struct wcn36xx_hal_join_rsp_msg *)buf;

	wcn36xx_info("Join stattus=%d, Power ver=%d",
		rsp->status, rsp->tx_mgmt_power);
	return 0;
}

int wcn36xx_smd_config_bss(struct wcn36xx *wcn, bool sta_mode, u8 *bssid, u8 update)
{
	struct wcn36xx_hal_config_bss_req_msg msg_body;

	INIT_HAL_MSG(msg_body, WCN36XX_HAL_CONFIG_BSS_REQ)

	if(sta_mode) {
		memcpy(&msg_body.u.bss_params.bssid, bssid, ETH_ALEN);
		memcpy(&msg_body.u.bss_params.self_mac_addr, &wcn->addresses[0], ETH_ALEN);
		msg_body.u.bss_params.bss_type = WCN36XX_HAL_INFRASTRUCTURE_MODE;
		//TODO define enum for oper_mode
		msg_body.u.bss_params.oper_mode = 1;  //0 - AP,  1-STA
		msg_body.u.bss_params.llg_coexist = 1;
		msg_body.u.bss_params.beacon_interval = 0x64;

		msg_body.u.bss_params.oper_channel = wcn->ch;
		memcpy(&msg_body.u.bss_params.sta.bssid, bssid, ETH_ALEN);
		msg_body.u.bss_params.sta.type = 1;
		msg_body.u.bss_params.sta.listen_interval = 0x64;
		msg_body.u.bss_params.sta.wmm_enabled = 1;

		msg_body.u.bss_params.sta.max_ampdu_size = 3;
		msg_body.u.bss_params.sta.max_ampdu_density = 5;
		msg_body.u.bss_params.sta.dsss_cck_mode_40mhz = 1;
	} else {
		memcpy(&msg_body.u.bss_params.bssid, &wcn->addresses[0], ETH_ALEN);
		memcpy(&msg_body.u.bss_params.self_mac_addr, &wcn->addresses[0], ETH_ALEN);

		//TODO do all this configurabel
		msg_body.u.bss_params.bss_type = WCN36XX_HAL_INFRA_AP_MODE;
		msg_body.u.bss_params.oper_mode = 0; //0 - AP,  1-STA

		msg_body.u.bss_params.short_slot_time_supported = 1;
		msg_body.u.bss_params.beacon_interval = 0x64;
		msg_body.u.bss_params.dtim_period = 2;
		msg_body.u.bss_params.oper_channel = 1;
		msg_body.u.bss_params.ssid.length = 1;
		msg_body.u.bss_params.ssid.ssid[0] = 'K';
		msg_body.u.bss_params.obss_prot_enabled = 1;
		msg_body.u.bss_params.wcn36xx_hal_persona = 1;
		msg_body.u.bss_params.max_tx_power = 0x10;

		memcpy(&msg_body.u.bss_params.sta.bssid, &wcn->addresses[0], ETH_ALEN);
		msg_body.u.bss_params.sta.short_preamble_supported = 1;
		memcpy(&msg_body.u.bss_params.sta.mac, &wcn->addresses[0], ETH_ALEN);
		msg_body.u.bss_params.sta.listen_interval = 8;
	}
	msg_body.u.bss_params.nw_type = WCN36XX_HAL_11G_NW_TYPE;
	msg_body.u.bss_params.sta.ht_capable = 1;
	msg_body.u.bss_params.sta.sgi_40mhz = 1;
	msg_body.u.bss_params.sta.sgi_20Mhz = 1;
	if (update == 1) {
		msg_body.u.bss_params.short_slot_time_supported = 1;
		msg_body.u.bss_params.lln_non_gf_coexist = 1;
		msg_body.u.bss_params.dtim_period = 0;
		msg_body.u.bss_params.sta.aid = 1;
		msg_body.u.bss_params.sta.bssid_index = 0;
		msg_body.u.bss_params.action = 1;
		msg_body.u.bss_params.tx_mgmt_power = 6;
		msg_body.u.bss_params.max_tx_power = 0x10;
	} else {
		msg_body.u.bss_params.max_tx_power = 0x14;
		msg_body.u.bss_params.dtim_period = 1;
		msg_body.u.bss_params.sta.bssid_index = 0xff;
	}
	msg_body.u.bss_params.sta.sta_index = 0xff;

	memcpy(&msg_body.u.bss_params.sta.supported_rates, &wcn->supported_rates, sizeof(wcn->supported_rates));

	PREPARE_HAL_BUF(wcn->smd_buf, msg_body)

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
}
int wcn36xx_smd_send_beacon(struct wcn36xx *wcn, struct sk_buff *skb_beacon, u16 tim_off, u16 p2p_off){
	struct wcn36xx_hal_send_beacon_req_msg msg_body;
	INIT_HAL_MSG(msg_body, WCN36XX_HAL_SEND_BEACON_REQ)

	// TODO need to find out why this is needed?
	msg_body.beacon_length = skb_beacon->len + 6;

	// TODO make this as a const
	if (BEACON_TEMPLATE_SIZE > msg_body.beacon_length) {
		memcpy(&msg_body.beacon, &skb_beacon->len, sizeof(u32));
		memcpy(&(msg_body.beacon[4]), skb_beacon->data, skb_beacon->len);
	} else {
		wcn36xx_error("Beacon is to big: beacon size=%d", msg_body.beacon_length);
		return -ENOMEM;
	}
	memcpy(&msg_body.bssid, &wcn->addresses[0], ETH_ALEN);

	// TODO need to find out why this is needed?
	msg_body.tim_ie_offset = tim_off+4;
	msg_body.p2p_ie_offset = p2p_off;
	PREPARE_HAL_BUF(wcn->smd_buf, msg_body)

	return wcn36xx_smd_send_and_wait(wcn, msg_body.header.len);
};

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
		wcn36xx_error("SMD_EVENT not supported");
		break;
	}
}
static void wcn36xx_smd_rsp_process (void *buf, size_t len)
{
	struct wcn36xx_fw_msg_header * msg_header = buf;

	wcn36xx_dbg_dump("SMD <<< ", buf, len);
	switch (msg_header->msg_type) {
	case WCN36XX_HAL_START_RSP:
		wcn36xx_smd_start_rsp(buf, len);
		break;
	case WCN36XX_HAL_ADD_STA_SELF_RSP:
	case WCN36XX_HAL_INIT_SCAN_RSP:
	case WCN36XX_HAL_START_SCAN_RSP:
	case WCN36XX_HAL_END_SCAN_RSP:
	case WCN36XX_HAL_FINISH_SCAN_RSP:
	case WCN36XX_HAL_DOWNLOAD_NV_RSP:
	case WCN36XX_HAL_CONFIG_BSS_RSP:
	case WCN36XX_HAL_CONFIG_STA_RSP:
	case WCN36XX_HAL_SEND_BEACON_REQ:

		if(wcn36xx_smd_rsp_status_check(buf, len)) {
			wcn36xx_error("response failed");
		}
		break;
	case WCN36XX_HAL_JOIN_RSP:
		wcn36xx_smd_join_rsp(buf, len);
		break;
	case WCN36XX_HAL_UPDATE_SCAN_PARAM_RSP:
		wcn36xx_smd_update_scan_params_rsp(buf, len);
		break;
	default:
		wcn36xx_error("SMD_EVENT not supported");
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
		wcn36xx_smd_rsp_process(msg, msg_len);
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
