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

#include "txrx.h"
#include <linux/ieee80211.h>
int  wcn36xx_rx_skb(struct wcn36xx *wcn, struct sk_buff *skb)
{
	struct sk_buff *skb2 ;
	struct ieee80211_rx_status status;
	struct wcn36xx_rx_bd * bd;

	skb2 = skb_clone(skb, GFP_ATOMIC);
	bd = (struct wcn36xx_rx_bd *)skb2->data;
	buff_to_be((u32*)bd, sizeof(*bd)/sizeof(u32));

	skb_pull(skb2, bd->pdu.mpdu_header_off);

	skb_trim(skb2, bd->pdu.mpdu_len);
	status.mactime = 10;
	status.band = IEEE80211_BAND_2GHZ;
	status.freq = ieee80211_channel_to_frequency(bd->rx_ch, status.band);
	status.signal = 1;
	status.antenna = 1;
	status.rate_idx = 1;
	status.flag = 0;
	status.rx_flags = 0;
	memcpy(skb2->cb, &status, sizeof(struct ieee80211_rx_status));
	wcn36xx_dbg(WCN36XX_DBG_RX, "RX");
	wcn36xx_dbg_dump(WCN36XX_DBG_RX_DUMP, "SKB <<< ",
			 (char*)skb2->data, skb2->len);
	ieee80211_rx_ni(wcn->hw, skb2);

	return 0;
}
void wcn36xx_prepare_tx_bd(void * pBd, u32 len)
{
	struct wcn36xx_tx_bd * bd = (struct wcn36xx_tx_bd *)pBd;
	// Must be clean every time because we can have some leftovers from the previous packet
	memset(pBd, 0, (sizeof(struct wcn36xx_tx_bd)));
	bd->pdu.mpdu_header_len = WCN36XX_802_11_HEADER_LEN;
	bd->pdu.mpdu_header_off = sizeof(struct wcn36xx_tx_bd);
	bd->pdu.mpdu_data_off = bd->pdu.mpdu_header_len +
		bd->pdu.mpdu_header_off;
	bd->pdu.mpdu_len = len;
}
void wcn36xx_fill_tx_bd(void * pBd, u8 broadcast)
{
	struct wcn36xx_tx_bd * bd = (struct wcn36xx_tx_bd *)pBd;
	bd->dpu_rf = WCN36XX_BMU_WQ_TX;
	bd->pdu.tid   = WCN36XX_TID;
	bd->pdu.reserved3 = 0xd;

	if ( broadcast ) {
		// broadcast
		bd->ub = 1;
		bd->queue_id = WCN36XX_TX_B_WQ_ID;

		// default rate for broadcast
		bd->bd_rate = 0;

		// No ack needed not unicast
		bd->ack_policy = 1;
	} else {
		bd->queue_id = WCN36XX_TX_U_WQ_ID;
		// default rate for unicast
		bd->bd_rate = 2;
		bd->ack_policy = 0;
	}

	bd->sta_index = 1;

	// no encription
	bd->dpu_ne = 1;

	buff_to_be((u32*)bd, sizeof(*bd)/sizeof(u32));
	bd->tx_bd_sign = 0xbdbdbdbd;
}
