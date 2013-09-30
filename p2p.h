/*
 * Copyright (c) 2013 Eugene Krasnikov <k.eugene.e@gmail.com>
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
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
#ifndef _WCN36XX_P2P_H_
#define _WCN36XX_P2P_H_

struct wcn36xx;

int wcn36xx_p2p_init(struct wcn36xx *wcn);
int wcn36xx_p2p_deinit(struct wcn36xx *wcn);
int wcn36xx_remain_on_channel(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif,
			      struct ieee80211_channel *chan,
			      int duration,
			      enum ieee80211_roc_type type);
int wcn36xx_cancel_remain_on_channel(struct ieee80211_hw *hw);

#endif	/* _WCN36XX_P2P_H_ */

