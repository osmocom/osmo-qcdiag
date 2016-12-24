/*
 * (C) 2013-2016 by Harald Welte <laforge@gnumonks.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>

#include "diag_log.h"
#include "protocol/diag_log_gsm.h"


static void handle_rr_sig_msg(struct log_hdr *lh, struct msgb *msg)
{
	struct diag_gsm_rr_msg *rm = (struct diag_gsm_rr_msg *) msgb_data(msg);

	printf("RR: %02x %02x %u: %s\n", rm->chan_type, rm->msg_type,
		rm->length, osmo_hexdump(msgb_data(msg), rm->length));
}

static void handle_rr_state_msg(struct log_hdr *lh, struct msgb *msg)
{
	struct diag_gsm_rr_state *rrs = (struct diag_gsm_rr_state *) msgb_data(msg);
	printf("RR-STATE { state=%s, substate=%u, status=%u, mode=%u }\n",
		get_value_string(diag_gsm_rr_st_vals, rrs->state)
		, rrs->substate, rrs->status, rrs->mode);

}

static const struct diag_log_dispatch_tbl log_tbl[] = {
	{ GSM(LOG_GSM_RR_SIGNALING_MESSAGE_C), handle_rr_sig_msg },
	{ GSM(LOG_GSM_RR_STATE_C), handle_rr_state_msg },
};

static __attribute__((constructor)) void on_dso_load_gsm(void)
{
	diag_log_reg_dispatch(log_tbl, ARRAY_SIZE(log_tbl));
}
