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

/* Max shared size is 4k but we take less.*/
#define WCN36XX_NV_FRAGMENT_SIZE			3072

#define SMD_MSG_TIMEOUT 200
#define WCN36XX_SMSM_WLAN_TX_ENABLE			0x00000400
#define WCN36XX_SMSM_WLAN_TX_RINGS_EMPTY		0x00000200
/* The PNO version info be contained in the rsp msg */
#define WCN36XX_FW_MSG_PNO_VERSION_MASK			0x8000

enum wcn36xx_fw_msg_result {
	WCN36XX_FW_MSG_RESULT_SUCCESS			= 0,
	WCN36XX_FW_MSG_RESULT_SUCCESS_SYNC		= 1,

	WCN36XX_FW_MSG_RESULT_MEM_FAIL			= 5,
};

/******************************/
/* SMD requests and responses */
/******************************/
struct wcn36xx_fw_msg_status_rsp {
	u32	status;
} __packed;

struct wcn36xx;

int wcn36xx_smd_open(struct wcn36xx *wcn);
void wcn36xx_smd_close(struct wcn36xx *wcn);

int wcn36xx_smd_load_nv(struct wcn36xx *wcn);
int wcn36xx_smd_start(struct wcn36xx *wcn);
int wcn36xx_smd_stop(struct wcn36xx *wcn);
int wcn36xx_smd_init_scan(struct wcn36xx *wcn);
int wcn36xx_smd_start_scan(struct wcn36xx *wcn, int ch);
int wcn36xx_smd_end_scan(struct wcn36xx *wcn, int ch);
int wcn36xx_smd_finish_scan(struct wcn36xx *wcn);
int wcn36xx_smd_update_scan_params(struct wcn36xx *wcn);
int wcn36xx_smd_add_sta_self(struct wcn36xx *wcn, u8 *addr, u32 status);
int wcn36xx_smd_delete_sta_self(struct wcn36xx *wcn, u8 *addr);
int wcn36xx_smd_delete_sta(struct wcn36xx *wcn);
int wcn36xx_smd_join(struct wcn36xx *wcn, const u8 *bssid, u8 *vif, u8 ch);
int wcn36xx_smd_set_link_st(struct wcn36xx *wcn, const u8 *bssid,
			    const u8 *sta_mac,
			    enum wcn36xx_hal_link_state state);
int wcn36xx_smd_config_bss(struct wcn36xx *wcn, enum nl80211_iftype type,
			   const u8 *bssid, bool update, u16 beacon_interval);
int wcn36xx_smd_delete_bss(struct wcn36xx *wcn);
int wcn36xx_smd_config_sta(struct wcn36xx *wcn, const u8 *bssid,
			   const u8 *sta_mac);
int wcn36xx_smd_send_beacon(struct wcn36xx *wcn, struct sk_buff *skb_beacon,
			    u16 tim_off, u16 p2p_off);
int wcn36xx_smd_switch_channel(struct wcn36xx *wcn, int ch);
int wcn36xx_smd_update_proberesp_tmpl(struct wcn36xx *wcn, struct sk_buff *skb);
int wcn36xx_smd_set_stakey(struct wcn36xx *wcn,
			   enum ani_ed_type enc_type,
			   u8 keyidx,
			   u8 keylen,
			   u8 *key);
int wcn36xx_smd_set_bsskey(struct wcn36xx *wcn,
			   enum ani_ed_type enc_type,
			   u8 keyidx,
			   u8 keylen,
			   u8 *key);
int wcn36xx_smd_remove_stakey(struct wcn36xx *wcn,
			      enum ani_ed_type enc_type,
			      u8 keyidx);
int wcn36xx_smd_remove_bsskey(struct wcn36xx *wcn,
			      enum ani_ed_type enc_type,
			      u8 keyidx);

/* WCN36XX configuration parameters */
struct wcn36xx_fw_cfg {
	u16		id;
	u16		len;
	u16		pad_bytes;
	u16		reserved;
	u8		*val;
};
#endif	/* _SMD_H_ */
