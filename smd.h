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

#ifndef _SMD_H_
#define _SMD_H_

#include <mach/msm_smd.h>
#include "wcn36xx.h"
#include "hal.h"

//Max shared size is 4k but we take less.
#define WCN36XX_NV_FRAGMENT_SIZE 			3072

#define SMD_MSG_TIMEOUT 200
#define WCN36XX_SMSM_WLAN_TX_ENABLE	 		0x00000400
#define WCN36XX_SMSM_WLAN_TX_RINGS_EMPTY		0x00000200

enum wcn36xx_fw_msg_type {
	/* CFG */
	WCN36XX_FW_MSG_TYPE_UPDATE_CFG_REQ		= 48,
	WCN36XX_FW_MSG_TYPE_UPDATE_CFG_RSP		= 49,

	WCN36XX_FW_MSG_TYPE_ADD_BCN_FILTER_REQ		= 84,
	WCN36XX_FW_MSG_TYPE_ADD_BCN_FILTER_RSP		= 104,

	WCN36XX_FW_MSG_TYPE_EX_CAPABILITIES_REQ		= 175,
	WCN36XX_FW_MSG_TYPE_EX_CAPABILITIES_RSP		= 176

};


enum wcn36xx_fw_msg_result {
	WCN36XX_FW_MSG_RESULT_SUCCESS			= 0,
	WCN36XX_FW_MSG_RESULT_SUCCESS_SYNC		= 1,

	WCN36XX_FW_MSG_RESULT_MEM_FAIL			= 5,
};

enum wcn36xx_fw_msg_ver {
	WCN36XX_FW_MSG_VER0				= 0
};

/******************************/
/* SMD requests and responses */
/******************************/
struct wcn36xx_fw_msg_status_rsp {
	u32 	status;
} __packed;

#define wcn36xx_fw_msg_ex_caps_rsp 		wcn36xx_fw_msg_ex_caps_req

/* WCN36XX_FW_MSG_TYPE_UPDATE_CFG_REQ */
struct wcn36xx_fw_msg_update_cfg_req {
	u32 	conf_len;
	 // config buffer must start in TLV format just here
} __packed;

/* WCN36XX_FW_MSG_TYPE_EX_CAPABILITIES_REQ */
struct wcn36xx_fw_msg_ex_capabilities_req {
	u32	caps[4];
} __packed;
#define wcn36xx_fw_msg_ex_capabilities_rsp wcn36xx_fw_msg_ex_capabilities_req

/* WCN36XX_FW_MSG_TYPE_ADD_BCN_FILTER_REQ */
struct wcn36xx_fw_msg_add_bcn_filter_req {
	u8	enable_11d;
	u8	resolved_11d;
	u8	ch_count;
	u8	ch[26];
	u16	active_min_ch_time;
	u16	active_max_ch_time;
	u16	passive_min_ch_time;
	u16	passive_max_ch_time;
	u32	phy_ch_state;
} __packed;

struct wcn36xx_fw_msg_header {
	enum wcn36xx_fw_msg_type	msg_type:16;
	enum wcn36xx_fw_msg_ver		msg_ver:16;
	u32				msg_len;
};
struct wcn36xx;

int wcn36xx_smd_open(struct wcn36xx *wcn);
void wcn36xx_smd_close(struct wcn36xx *wcn);

int wcn36xx_smd_load_nv(struct wcn36xx *wcn);
int wcn36xx_smd_start(struct wcn36xx *wcn);
int wcn36xx_smd_init_scan(struct wcn36xx *wcn);
int wcn36xx_smd_start_scan(struct wcn36xx *wcn, u8 ch);
int wcn36xx_smd_end_scan(struct wcn36xx *wcn, u8 ch);
int wcn36xx_smd_finish_scan(struct wcn36xx *wcn);
int wcn36xx_smd_update_scan_params(struct wcn36xx *wcn);
int wcn36xx_smd_add_sta_self(struct wcn36xx *wcn, struct mac_address addr, u32 status);
int wcn36xx_smd_join(struct wcn36xx *wcn, u8 *bssid, u8 *vif, u8 ch);
int wcn36xx_smd_set_link_st(struct wcn36xx *wcn, u8 *bssid, u8 *sta_mac, enum wcn36xx_hal_link_state state);
int wcn36xx_smd_config_bss(struct wcn36xx *wcn, bool sta_mode, u8 *bssid, u8 update);
int wcn36xx_smd_config_sta(struct wcn36xx *wcn, u8 *bssid, u16 ass_id, u8 *sta_mac);
int wcn36xx_smd_send_beacon(struct wcn36xx *wcn, struct sk_buff *skb_beacon, u16 tim_off, u16 p2p_off);

// WCN36XX configuration parameters
struct wcn36xx_fw_cfg {
	u16		id;
	u16		len;
	u16		pad_bytes;
	u16		reserved;
	u8		*val;
};
#endif	/* _SMD_H_ */
