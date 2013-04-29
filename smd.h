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

#define SMD_MSG_SCAN_MODE 				2

enum wcn36xx_fw_msg_type {
	WCN36XX_FW_MSG_TYPE_START_REQ			= 0,
	WCN36XX_FW_MSG_TYPE_START_RSP			= 1,

	/* Scan */
	WCN36XX_FW_MSG_TYPE_INIT_SCAN_REQ		= 4,
	WCN36XX_FW_MSG_TYPE_INIT_SCAN_RSP		= 5,
	WCN36XX_FW_MSG_TYPE_START_SCAN_REQ		= 6,
	WCN36XX_FW_MSG_TYPE_START_SCAN_RSP		= 7,
	WCN36XX_FW_MSG_TYPE_END_SCAN_REQ		= 8,
	WCN36XX_FW_MSG_TYPE_END_SCAN_RSP		= 9,
	WCN36XX_FW_MSG_TYPE_DEINIT_SCAN_REQ		= 10,
	WCN36XX_FW_MSG_TYPE_DEINIT_SCAN_RSP		= 11,

	WCN36XX_FW_MSG_TYPE_CONFIG_BSS_REQ              = 16,
	WCN36XX_FW_MSG_TYPE_CONFIG_BSS_RSP              = 17,

	WCN36XX_FW_MSG_TYPE_JOIN_REQ			= 20,
	WCN36XX_FW_MSG_TYPE_JOIN_RSP			= 21,

	WCN36XX_FW_MSG_TYPE_SET_LINK_ST_REQ		= 44,
	WCN36XX_FW_MSG_TYPE_SET_LINK_ST_RSP		= 45,

	WCN36XX_FW_MSG_TYPE_UPDATE_SCAN_PARAM_REQ	= 151,
	WCN36XX_FW_MSG_TYPE_UPDATE_SCAN_PARAM_RSP	= 152,

	/* CFG */
	WCN36XX_FW_MSG_TYPE_UPDATE_CFG_REQ		= 48,
	WCN36XX_FW_MSG_TYPE_UPDATE_CFG_RSP		= 49,

	WCN36XX_FW_MSG_TYPE_LOAD_NV_REQ			= 55,
	WCN36XX_FW_MSG_TYPE_LOAD_NV_RSP			= 56,

	WCN36XX_FW_MSG_TYPE_SEND_BEACON_REQ             = 63,
	WCN36XX_FW_MSG_TYPE_SEND_BEACON_RSP             = 64,

	WCN36XX_FW_MSG_TYPE_ENTER_IMPS_REQ		= 76,
	WCN36XX_FW_MSG_TYPE_ENTER_IMPS_RSP		= 95,

	WCN36XX_FW_MSG_TYPE_EXIT_IMPS_REQ		= 77,
	WCN36XX_FW_MSG_TYPE_EXIT_IMPS_RSP		= 96,

	WCN36XX_FW_MSG_TYPE_ADD_BCN_FILTER_REQ		= 84,
	WCN36XX_FW_MSG_TYPE_ADD_BCN_FILTER_RSP		= 104,

	WCN36XX_FW_MSG_TYPE_ADD_STA_REQ			= 125,
	WCN36XX_FW_MSG_TYPE_ADD_STA_RSP			= 126,

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

enum wcn36xx_fw_msg_driver_type {
	WCN36XX_FW_MSG_DRIVER_TYPE_PROD 		= 0
};

enum wcn36xx_fw_msg_bss_type {
	WCN36XX_FW_MSG_BSS_TYPE_STA                     = 0,
	WCN36XX_FW_MSG_BSS_TYPE_AP                      = 1
};
enum wcn36xx_fw_msg_bss_oper_mode {
	WCN36XX_FW_MSG_BSS_MODE_AP                      = 0,
	WCN36XX_FW_MSG_BSS_MODE_STA                     = 1
};
enum wcn36xx_fw_msg_network_type {
	WCN36XX_FW_MSG_NET_TYPE_11A                     = 0,
	WCN36XX_FW_MSG_NET_TYPE_11B                     = 1,
	WCN36XX_FW_MSG_NET_TYPE_11G                     = 2,
	WCN36XX_FW_MSG_NET_TYPE_11N                     = 3
};
enum wcn36xx_fw_msg_ht_op_mode {
	WCN36XX_FW_MSG_HT_OP_MODE_PURE                  = 0,
	WCN36XX_FW_MSG_HT_OP_MODE_OVERLAP_LEG           = 1,
	WCN36XX_FW_MSG_HT_OP_MODE_NO_LEG                = 2,
	WCN36XX_FW_MSG_HT_OP_MODE_MIXED                 = 3
};
enum wcn36xx_fw_msg_ht_mimo_ps {
	WCN36XX_FW_MSG_HT_MIMO_PS_STATIC                = 0,
	WCN36XX_FW_MSG_HT_MIMO_PS_DYNAMIC               = 1
};
enum wcn36xx_fw_msg_sta_rate_mode {
	WCN36XX_FW_MSG_STA_RATE_MODE_TAURUS             = 0,
	WCN36XX_FW_MSG_STA_RATE_MODE_TITAN              = 1,
	WCN36XX_FW_MSG_STA_RATE_MODE_POLARIS            = 2,
	WCN36XX_FW_MSG_STA_RATE_MODE_11B                = 3,
	WCN36XX_FW_MSG_STA_RATE_MODE_11BG               = 4,
	WCN36XX_FW_MSG_STA_RATE_MODE_11A                = 5,
	WCN36XX_FW_MSG_STA_RATE_MODE_11N                = 6
};

/******************************/
/* SMD requests and responses */
/******************************/
struct wcn36xx_fw_msg_status_rsp {
	u32 	status;
} __packed;

#define wcn36xx_fw_msg_nv_load_rsp 		wcn36xx_fw_msg_status_rsp
#define wcn36xx_fw_msg_enter_imps_rsp 		wcn36xx_fw_msg_status_rsp
#define wcn36xx_fw_msg_exit_imps_rsp 		wcn36xx_fw_msg_status_rsp
#define wcn36xx_fw_msg_init_scan_rsp 		wcn36xx_fw_msg_status_rsp
#define wcn36xx_fw_msg_end_scan_rsp 		wcn36xx_fw_msg_status_rsp
#define wcn36xx_fw_msg_scan_params_rsp 		wcn36xx_fw_msg_status_rsp
#define wcn36xx_fw_msg_ex_caps_rsp 		wcn36xx_fw_msg_ex_caps_req

/* WCN36XX_FW_MSG_TYPE_LOAD_NV_REQ */
struct wcn36xx_fw_msg_nv_load_header {
	u16 	frag_num;
	u16 	is_last;
	u32 	msg_len;
} __packed;

/* WCN36XX_FW_MSG_TYPE_START_REQ */
struct wcn36xx_fw_msg_start_req {
	enum wcn36xx_fw_msg_driver_type 	driver_type;
	u32 	conf_len;
	 // config buffer must start in TLV format just here
} __packed;
struct wcn36xx_fw_msg_api_ver {
	u8	rev;
	u8	ver;
	u8	minor;
	u8	major;
} __packed;
struct wcn36xx_fw_msg_start_rsp {
	u16 				status;
	u8 				max_sta;
	u8 				max_bss;
	struct wcn36xx_fw_msg_api_ver 	api_ver;
	u8				crm_ver[64];
	u8				wlan_ver[64];
} __packed;

/* WCN36XX_FW_MSG_TYPE_UPDATE_CFG_REQ */
struct wcn36xx_fw_msg_update_cfg_req {
	u32 	conf_len;
	 // config buffer must start in TLV format just here
} __packed;

/* WCN36XX_FW_MSG_TYPE_INIT_SCAN_REQ */
struct wcn36xx_fw_msg_init_scan_req {
	u32 				scan_mode;
	u8 				bssid[ETH_ALEN];
	u8				notify_bss;
	u8				frame_type;
	u8				frame_len;
	struct ieee80211_hdr_3addr 	hdr;
	u8				bssidx[2];
	u8				active_bss;
} __packed;

/* WCN36XX_FW_MSG_TYPE_START_SCAN_REQ */
struct wcn36xx_fw_msg_start_scan_req {
	u8	ch;
} __packed;
struct wcn36xx_fw_msg_start_scan_rsp {
	u32 	status;
	u32	start_TSF[2];
	u8	mgmt_power;
} __packed;

/* WCN36XX_FW_MSG_TYPE_END_SCAN_REQ */
struct wcn36xx_fw_msg_end_scan_req {
	u8	ch;
} __packed;

/* WCN36XX_FW_MSG_TYPE_DEINIT_SCAN_REQ */
struct wcn36xx_fw_msg_deinit_scan_req {
	u32 				scan_mode;
	u8				cur_ch;
	u32				bon_state;
	u8 				bssid[ETH_ALEN];
	u8				notify_bss;
	u8				frame_type;
	u8				frame_len;
	struct ieee80211_hdr_3addr 	hdr;
	u8				bssidx[2];
	u8				active_bss;
} __packed;

/* WCN36XX_FW_MSG_TYPE_UPDATE_SCAN_PARAM_REQ */
struct wcn36xx_fw_msg_update_scan_params_req {
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

/* WCN36XX_FW_MSG_TYPE_EX_CAPABILITIES_REQ */
struct wcn36xx_fw_msg_ex_capabilities_req {
	u32	caps[4];
} __packed;
#define wcn36xx_fw_msg_ex_capabilities_rsp wcn36xx_fw_msg_ex_capabilities_req

/* WCN36XX_FW_MSG_TYPE_ADD_STA_REQ */
struct wcn36xx_fw_msg_add_sta_req {
	u8	mac[ETH_ALEN];
	u32 	status;
} __packed;
struct wcn36xx_fw_msg_add_sta_rsp {
	u32 	status;
	u8	sta_id;
	u8	dpu_id;
	u8	dpu_sign;
} __packed;

//TODO
/* WCN36XX_FW_MSG_TYPE_CONFIG_STA_REQ */
struct wcn36xx_fw_msg_supported_rates {
	enum wcn36xx_fw_msg_sta_rate_mode       sta_rate_mode;
	u16                                     rates_11b[4];
	u16                                     rates_11a[8];
	u16                                     ani_leg_rate[3];
	u16                                     reserved;
	u32                                     enhan_rate_bitmap;
	u8                                      supported_mcs_set[16];
	u16                                     rx_highes_rate;
} __packed;

struct wcn36xx_fw_msg_config_sta {
	u8                                      bssid[ETH_ALEN];
	u16                                     ass_id;
	u8                                      sta_type;
	u8                                      short_pream_sup;
	u8                                      sta_mac[ETH_ALEN];
	u16                                     listen_int;
	u8                                      wmm_en;
	u8                                      ht_cap;
	u8                                      tx_ch_width;
	u8                                      rifs_mode;
	u8                                      lsig_txop_prot;
	u8                                      max_ampdu_size;
	u8                                      max_ampdu_dens;
	u8                                      max_amsdu_size;
	u8                                      short_gi40mhz;
	u8                                      short_gi20mhz;
	struct wcn36xx_fw_msg_supported_rates   supported_rates;
	u8                                      rmf_en;         // robust management frame
	u32                                     encrypt_type;
	u8                                      action;
	u8                                      uapsd;
	u8                                      max_sp_len;
	u8                                      gf_cap;
	enum wcn36xx_fw_msg_ht_mimo_ps          mimo_ps;
	u8                                      delayed_ba;
	u8                                      max_ampdu_dur;
	u8                                      dsss_cck_mode_40mhz;
	u8                                      sta_id;
	u8                                      bss_id;
	u8                                      p2p_cap;
	// TODO add this parameter for 3680.
	//u8                                      reserved;
} __packed;

/* WCN36XX_FW_MSG_TYPE_CONFIG_BSS_REQ */
struct wcn36xx_fw_msg_ssid {
	u8 len;
	u8 ssid[IEEE80211_MAX_SSID_LEN];
} __packed;

struct wcn36xx_fw_msg_rate_set {
	u8 num;
	u8 rate[12];
} __packed;

struct wcn36xx_fw_msg_edca_param {
	u8      ac;             //access category
	u8      cw;             // contention window size
	u16     tx_op_limit;
} __packed;

struct wcn36xx_fw_msg_config_bss_req {
	u8                                      bssid[ETH_ALEN];
	u8                                      self_mac[ETH_ALEN];
	enum wcn36xx_fw_msg_bss_type            bss_type;
	u8                                      oper_mode;
	enum wcn36xx_fw_msg_network_type        net_type;
	u8                                      short_slot_time;
	u8                                      coex_11a;
	u8                                      coex_11b;
	u8                                      coex_11g;
	u8                                      coex_ht20;
	u8                                      coex_11n_non_gf;
	u8                                      lsig_txop_prot;
	u8                                      rifs_mode;
	u16                                     beacon_interval;
	u8                                      dtim_period;
	u8                                      tx_ch_width;
	u8                                      cur_op_ch;
	u8                                      cur_ext_ch;
	u8                                      reserved;
	struct wcn36xx_fw_msg_config_sta        sta_context;
	struct wcn36xx_fw_msg_ssid              ssid;
	u8                                      action;
	struct wcn36xx_fw_msg_rate_set          rate_set;
	u8                                      ht_caps;
	u8                                      obss_prot;
	u8                                      rmf;
	enum wcn36xx_fw_msg_ht_op_mode          ht_op_mode;
	u8                                      dual_cts_prot;
	u8                                      prb_resp_max_ret;
	u8                                      hidden_ssid_en;
	u8                                      proxy_prb_rsp_en;
	u8                                      edca_params_valid;
	struct wcn36xx_fw_msg_edca_param        acbe;
	struct wcn36xx_fw_msg_edca_param        acbk;
	struct wcn36xx_fw_msg_edca_param        acvi;
	struct wcn36xx_fw_msg_edca_param        acvo;
	// TODO So far support only open network
	u8                                      sta_key_params[241];
	u8                                      hal_pers;
	u8                                      spec_mgmt_en;
	u8                                      tx_mgmt_power;
	u8                                      max_tx_power;
} __packed;

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

/* WCN36XX_FW_MSG_TYPE_SEND_BEACON_REQ */
struct wcn36xx_fw_msg_send_bcn_req {
	u32             beacon_len;
	//TODO What is the purpose of having two len?
	u32             beacon_len2;
	u8              beacon[0x17C];
	u8              mac[ETH_ALEN];
	u32             tim_ie_offset;
	u16             p2p_ie_offset;
} __packed;


/* WCN36XX_FW_MSG_TYPE_JOIN_REQ */
struct wcn36xx_fw_msg_join_req {
	u8	bssid[ETH_ALEN];
	u8	ch;
	u8	sta_mac[ETH_ALEN];
	u8	power;
	u32	chan_off;
	u32	link_state;
	u8	max_power;
} __packed;

struct wcn36xx_fw_msg_join_rsp {
	u32	status;
	u8	power;
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
int wcn36xx_smd_deinit_scan(struct wcn36xx *wcn);
int wcn36xx_smd_update_scan_params(struct wcn36xx *wcn);
int wcn36xx_smd_add_sta(struct wcn36xx *wcn, struct mac_address addr, u32 status);
int wcn36xx_smd_enter_imps(struct wcn36xx *wcn);
int wcn36xx_smd_exit_imps(struct wcn36xx *wcn);
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
