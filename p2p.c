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
#include "wcn36xx.h"

static void wcn36xx_roc_complete_work(struct work_struct *work)
{
	struct delayed_work *dwork;
	struct wcn36xx *wcn;

	wcn36xx_dbg(WCN36XX_DBG_MAC, "%s\n", __func__);

	dwork = container_of(work, struct delayed_work, work);
	wcn = container_of(dwork, struct wcn36xx, roc_complete_work);

	ieee80211_remain_on_channel_expired(wcn->hw);
}


int wcn36xx_remain_on_channel(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif,
			      struct ieee80211_channel *chan,
			      int duration,
			      enum ieee80211_roc_type type)
{
	struct wcn36xx *wcn = hw->priv;
	int ret;

	wcn36xx_dbg(WCN36XX_DBG_MAC, "%s\n", __func__);

	ret = wcn36xx_smd_switch_channel(wcn, vif, chan->hw_value);
	wcn36xx_smd_set_link_st(wcn, vif->addr, vif->addr,
				WCN36XX_HAL_LINK_LISTEN_STATE);
	ieee80211_ready_on_channel(hw);
	ieee80211_queue_delayed_work(wcn->hw, &wcn->roc_complete_work,
				     msecs_to_jiffies(duration));

	return ret;
}

int wcn36xx_cancel_remain_on_channel(struct ieee80211_hw *hw)
{
	struct wcn36xx *wcn = hw->priv;

	wcn36xx_dbg(WCN36XX_DBG_MAC, "%s\n", __func__);
	cancel_delayed_work_sync(&wcn->roc_complete_work);
	return 0;
}

int wcn36xx_p2p_init(struct wcn36xx *wcn)
{

	INIT_DELAYED_WORK(&wcn->roc_complete_work, wcn36xx_roc_complete_work);
	return 0;
}

int wcn36xx_p2p_deinit(struct wcn36xx *wcn)
{
	cancel_delayed_work(&wcn->roc_complete_work);
	return 0;
}
