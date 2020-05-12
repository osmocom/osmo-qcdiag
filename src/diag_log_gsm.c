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

#include <osmocom/core/gsmtap.h>
#include <osmocom/core/gsmtap_util.h>
#include <osmocom/core/utils.h>

#include "diag_log.h"
#include "protocol/diag_log_gsm.h"

struct dirty_fcch {
	uint16_t arfcn_band;
	/* don't care about anyhting else */
};

static void handle_rr_sig_msg(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	uint32_t ctype = 0;
	uint16_t arfcn = di->gsm_arfcn;
	uint8_t *data;
	unsigned int len;
	struct diag_gsm_rr_msg *rm = (struct diag_gsm_rr_msg *) msgb_data(msg);

	di->rat_type = DIAG_INST_RAT_2G;
//	printf("RR: %s %02x %u: %s\n",
//		get_value_string(diag_gsm_l2_chantype_vals, rm->chan_type & 0x7f),
//		rm->msg_type, rm->length, osmo_hexdump(msgb_data(msg), rm->length));

	switch(rm->chan_type & 0x7f){
		case DIAG_GSM_L2_CHAN_TYPE_DCCH: ctype =GSMTAP_CHANNEL_SDCCH; break;
		case DIAG_GSM_L2_CHAN_TYPE_BCCH: ctype =GSMTAP_CHANNEL_BCCH; break;
		case DIAG_GSM_L2_CHAN_TYPE_RACH: ctype =GSMTAP_CHANNEL_RACH; break;
		case DIAG_GSM_L2_CHAN_TYPE_CCCH: ctype =GSMTAP_CHANNEL_CCCH; break;
		case DIAG_GSM_L2_CHAN_TYPE_SACCH: ctype =GSMTAP_CHANNEL_SDCCH | GSMTAP_CHANNEL_ACCH; break;
		case DIAG_GSM_L2_CHAN_TYPE_SDCCH: ctype =GSMTAP_CHANNEL_SDCCH; break;
		case DIAG_GSM_L2_CHAN_TYPE_FACCH_F: ctype =GSMTAP_CHANNEL_TCH_F | GSMTAP_CHANNEL_ACCH; break;
		case DIAG_GSM_L2_CHAN_TYPE_FACCH_H: ctype =GSMTAP_CHANNEL_TCH_H | GSMTAP_CHANNEL_ACCH; break;
		default:
			printf("Unhandled RR: %s  %u: %s\n",
				get_value_string(diag_gsm_l2_chantype_vals, rm->chan_type & 0x7f),
				rm->length,
				osmo_hexdump(msgb_data(msg), rm->length));
			return;
	}

	arfcn = rm->chan_type & 0x80 ? arfcn : arfcn | GSMTAP_ARFCN_F_UPLINK;

	data = rm->data;
	len = rm->length;

	/* skip l2 pseudo len */
	if (ctype ==  GSMTAP_CHANNEL_BCCH || ctype ==  GSMTAP_CHANNEL_CCCH){
			data++;
			len--;
		}

	/* abis due to no lapdm header */
	if (di->gsmtap && di->flags & DIAG_INST_F_GSMTAP_DECODED) {
		gsmtap_send_ex(di->gsmtap, GSMTAP_TYPE_ABIS, arfcn, 0, ctype, 0, 0, 0, 0, data, len);
	}
}

static void handle_rr_state_msg(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct diag_gsm_rr_state *rrs = (struct diag_gsm_rr_state *) msgb_data(msg);
	printf("RR-STATE { state=%s, substate=%u, status=%u, mode=%u }\n",
		get_value_string(diag_gsm_rr_st_vals, rrs->state)
		, rrs->substate, rrs->status, rrs->mode);

}

static void handle_mdsp_cmd(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
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

static void handle_l2_state(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
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

static void handle_l2_transm_status(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct diag_gsm_l2_transm_status *lts = (struct diag_gsm_l2_transm_status *) msgb_data(msg);

	printf("L2-TRANSM-STATUS { sapi=%u, chan_type=%s, vs=%u, va=%u, vr=%u, retrans_ctr=%u, seq_err=%u, frame_type=%u, msg_entries=%u, seg_entries=%u }\n",
		lts->sapi,
		get_value_string(diag_gsm_l2_chantype_vals, lts->channel_type),
		lts->vs, lts->va, lts->vr, lts->retrans_ctr,
		lts->seq_err, lts->frame_type, lts->msg_entries, lts->seg_entries);
}

static void handle_arfcn(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct dirty_fcch *rm = (struct dirty_fcch *) msgb_data(msg);
	di->gsm_arfcn = rm->arfcn_band & 0x0FFF;
}

static const struct diag_log_dispatch_tbl log_tbl[] = {
	{ GSM(LOG_GSM_RR_SIGNALING_MESSAGE_C), handle_rr_sig_msg },
	{ GSM(LOG_GSM_RR_CELL_INFO_C), handle_arfcn },
	{ GSM(LOG_GSM_FCCH_DECODE_C), handle_arfcn },
	{ GSM(LOG_GSM_SCH_DECODE_C), handle_arfcn },
#if 0
	{ GSM(LOG_GSM_RR_STATE_C), handle_rr_state_msg },
	{ GSM(LOG_GSM_MDSP_CMD_C), handle_mdsp_cmd },
	{ GSM(LOG_GSM_L2_STATE_C), handle_l2_state },
	{ GSM(LOG_GSM_L2_TRANSMISSION_STATUS_C), handle_l2_transm_status},
#endif
};

static __attribute__((constructor)) void on_dso_load_gsm(void)
{
	diag_log_reg_dispatch(log_tbl, ARRAY_SIZE(log_tbl));
}
