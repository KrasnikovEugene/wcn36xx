/*
 * Contact: Eugene Krasnikov <k.eugene.e@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
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
	ieee80211_rx_ni(wcn->hw, skb2);

	return 0;
}
