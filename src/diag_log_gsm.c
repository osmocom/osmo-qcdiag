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

static void handle_mdsp_cmd(struct log_hdr *lh, struct msgb *msg)
{
	struct diag_mdsp_log_cmds *dmlcs = (struct diag_mdsp_log_cmds *) msgb_data(msg);
	int i;

	printf("MDSP-COMMANDS { num_cmds=%u, cmds=[ ", dmlcs->num_cmds);

	for (i = 0; i < dmlcs->num_cmds; i++) {
		struct diag_mdsp_log_cmd *cmd = &dmlcs->cmds[i];
		printf("{ fn=%u, cnt=%u, seq=%u, cmd=%u, params=[ %u, %u, %u, %u, %u ] }",
			cmd->fn, cmd->cnt, cmd->seq, cmd->cmd,
			cmd->params[0], cmd->params[1], cmd->params[2], cmd->params[3],
			cmd->params[4]);
		if (i+1 != dmlcs->num_cmds)
			printf(", ");

	}
	printf(" ] }\n");
}

static void handle_l2_state(struct log_hdr *lh, struct msgb *msg)
{
	struct diag_gsm_l2_state *l2s = (struct diag_gsm_l2_state *) msgb_data(msg);

	printf("L2-STATE { sapi=%u, ", l2s->sapi);
	switch (l2s->sapi) {
	case 0:
		printf("l2_state=%s, ",
			get_value_string(diag_gsm_l2_sapi0_st_vals, l2s->l2_state));
		break;
	case 3:
		printf("l2_state=%s, ",
			get_value_string(diag_gsm_l2_sapi3_st_vals, l2s->l2_state));
		break;
	default:
		break;
	}
	printf("l2_event=%s }\n",
		get_value_string(diag_gsm_l2_event_vals, l2s->l2_event));
}

static void handle_l2_transm_status(struct log_hdr *lh, struct msgb *msg)
{
	struct diag_gsm_l2_transm_status *lts = (struct diag_gsm_l2_transm_status *) msgb_data(msg);

	printf("L2-TRANSM-STATUS { sapi=%u, chan_type=%u, vs=%u, va=%u, vr=%u, retrans_ctr=%u, seq_err=%u, frame_type=%u, msg_entries=%u, seg_entries=%u }\n",
		lts->sapi, lts->channel_type, lts->vs, lts->va, lts->vr, lts->retrans_ctr,
		lts->seq_err, lts->frame_type, lts->msg_entries, lts->seg_entries);
}

static const struct diag_log_dispatch_tbl log_tbl[] = {
	{ GSM(LOG_GSM_RR_SIGNALING_MESSAGE_C), handle_rr_sig_msg },
	{ GSM(LOG_GSM_RR_STATE_C), handle_rr_state_msg },
	{ GSM(LOG_GSM_MDSP_CMD_C), handle_mdsp_cmd },
	{ GSM(LOG_GSM_L2_STATE_C), handle_l2_state },
	{ GSM(LOG_GSM_L2_TRANSMISSION_STATUS_C), handle_l2_transm_status},
};

static __attribute__((constructor)) void on_dso_load_gsm(void)
{
	diag_log_reg_dispatch(log_tbl, ARRAY_SIZE(log_tbl));
}
