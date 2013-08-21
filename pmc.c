/*
 * Copyright (c) 2013 Eugene Krasnikov <k.eugene.e@gmail.com>
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

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include "wcn36xx.h"

int wcn36xx_pmc_init(struct wcn36xx *wcn)
{
	wcn->pw_state = WCN36XX_FULL_POWER;
	return 0;
}

int wcn36xx_pmc_enter_bmps_state(struct wcn36xx *wcn, u64 tsf)
{
	/* TODO: Make sure the TX chain clean */
	wcn36xx_smd_enter_bmps(wcn, tsf);
	wcn->pw_state = WCN36XX_BMPS;
	return 0;
}

int wcn36xx_pmc_exit_bmps_state(struct wcn36xx *wcn)
{
	wcn36xx_smd_exit_bmps(wcn);
	wcn->pw_state = WCN36XX_FULL_POWER;
	return 0;
}

int wcn36xx_enable_keep_alive_null_packet(struct wcn36xx *wcn)
{
	wcn36xx_dbg(WCN36XX_DBG_PMC, "%s", __func__);
	return wcn36xx_smd_keep_alive_req(wcn, WCN36XX_HAL_KEEP_ALIVE_NULL_PKT);
}
