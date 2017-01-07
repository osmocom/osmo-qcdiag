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
#include "protocol/diag_log_gprs_rlc.h"
#include "protocol/diag_log_gprs_mac.h"


static void handle_grr_state_msg(struct log_hdr *lh, struct msgb *msg)
{
	struct diag_gprs_grr_state *rrs = (struct diag_gprs_grr_state *) msgb_data(msg);

	printf("GRR-STATE { active=%u, state=%s }\n", rrs->active_state,
		get_value_string(diag_gprs_grr_st_vals, rrs->grr_state));
}

static void handle_rlc_ul_abnrml_rls(struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rlc_ul_abnrml_rls_counts *arc;
	arc = (struct gprs_rlc_ul_abnrml_rls_counts *) msgb_data(msg);

	printf("RLC-UL-ABNRML-RLS-COUNTS { access_reject_cnt=%u, arc_retry_cnt=%u, arc_wo_retry_cnt=%u, arc_sys_info_cnt=%u }",
		arc->access_reject_cnt, arc->arc_retry_cnt, arc->arc_wo_retry_cnt, arc->arc_sys_info_cnt);
}

static void handle_mac_sign_msg(struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_mac_signalling_msg *msm;
	msm = (struct gprs_mac_signalling_msg *) msgb_data(msg);
	printf("MAC-SIGN-MSG { chan_type=%s, msg_type=%s, msg=%s }\n",
		get_value_string(gprs_mac_chan_type_vals, msm->chan_type),
		get_value_string(gprs_mac_msg_type_vals, msm->msg_type),
			osmo_hexdump(msm->msg, msm->msg_len));
}

static void handle_llc_me_info(struct log_hdr *lh, struct msgb *msg)
{
	struct diag_gprs_llme_info *gli;
	gli = (struct diag_gprs_llme_info *) msgb_data(msg);
	printf("LLC-ME-INFO { state=%s, xid_version=%u, tlli_new=0x%08x, tlli_old=0x%08x, gea=%u, key=%s }\n",
		get_value_string(diag_gprs_llme_st_vals, gli->state), gli->xid_version,
		gli->tlli_new, gli->tlli_old, gli->enc_alg,
		osmo_hexdump_nospc(gli->enc_key, sizeof(gli->enc_key)));
}

static void handle_llc_pdu_stats(struct log_hdr *lh, struct msgb *msg)
{
	struct diag_gprs_llc_stats *gls;
	gls = (struct diag_gprs_llc_stats *) msgb_data(msg);
	printf("LLC-PDU-STATS-ACK { sap=%u, l3pdu_tx=%u, octet_tx=%u, octet_retrans=%u, l3pdu_rx=%u, octet_rx=%u }\n",
		gls->sapi, gls->ack_l3pdu_tx, gls->ack_octet_tx, gls->ack_l3pdu_retrans, gls->ack_l3pdu_rx, gls->ack_octet_rx);
	printf("LLC-PDU-STATS-UNACK { sapi=%u, l3pdu_tx=%u, octet_tx=%u, l3pdu_rx=%u, octet_rx=%u }\n",
		gls->sapi, gls->unack_l3pdu_tx, gls->unack_octet_tx, gls->unack_l3pdu_rx, gls->unack_octet_rx);
	printf("LLC-PDU-STATS-LLPDU { tx=%u, rx=%u, fcs_err=%u, frm_rej=%u, tlli_err=%u, addr_err=%u, short_err=%u }\n",
		gls->llpdu_tx, gls->llpdu_rx, gls->llpdu_fcs_err, gls->llpdu_frm_rej, gls->llpdu_tlli_err, gls->llpdu_addr_err, gls->llpdu_short_err);
}

static void handle_mac_state(struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_mac_state_change *msc;
	msc = (struct gprs_mac_state_change *) msgb_data(msg);
	const char *name = "";

	switch (msc->mac_mode) {
	case NULL_MODE:
		name = get_value_string(gprs_mac_null_substate_vals, msc->mac_null_fsm_state);
		break;
	case IDLE_MODE:
		name = get_value_string(gprs_mac_idle_substate_vals, msc->mac_idle_fsm_state);
		break;
	case TRANSFER_MODE:
		name = get_value_string(gprs_mac_transfer_substate_vals, msc->mac_transfer_fsm_state);
		break;
	}

	printf("MAC-STATE { mode=%s, state=%s }\n",
		get_value_string(gprs_mac_mode_vals, msc->mac_mode), name);
}

static void handle_mac_dl_tbf_est(struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_mac_dl_tbf_est *dte;
	dte = (struct gprs_mac_dl_tbf_est *) msgb_data(msg);

	printf("MAC-DL-TBF-EST { tfi=%u, rlc_mode=%u, dl_ts_bmap=0x%x, is_egprs=%u, egprs_win_size=%u, egprs_link_qual_mode=%u, bep_period2=%u }\n",
		dte->dl_tfi, dte->rlc_mode, dte->dl_ts_bmap, dte->is_egprs_tbf, dte->egprs_win_size, dte->egprs_link_qual_mode, dte->bep_period2);
}

static void handle_mac_ul_tbf_est(struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_mac_ul_tbf_est *ute;
	ute = (struct gprs_mac_ul_tbf_est *) msgb_data(msg);

	printf("MAC-UL-TBF-EST { tbf_req_cause=%u, acc_granted=%u, radio_prio=%u, peak_tput=%u, ul_tfi=%u, rlc_mode=%u, ul_ts_bmap=0x%x, is_egprs=%u, egprs_win_size=%u, resegment=%u, bep_period2=%u }\n",
		ute->tbf_req_cause, ute->acc_granted, ute->radio_prio, ute->peak_tput, ute->ul_tfi, ute->rlc_mode, ute->ul_ts_bmap, ute->is_egprs_tbf, ute->egprs_win_size, ute->resegment, ute->bep_period2);
}

static void handle_mac_dl_tbf_rel(struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_mac_tbf_release *tr;
	tr = (struct gprs_mac_tbf_release *) msgb_data(msg);

	printf("MAC-DL-TBF-REL { tfi=%u, fail_cause=%u }\n", tr->tfi, tr->fail_cause);
}

static void handle_mac_ul_tbf_rel(struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_mac_tbf_release *tr;
	tr = (struct gprs_mac_tbf_release *) msgb_data(msg);

	printf("MAC-DL-TBF-REL { tfi=%u, fail_cause=%u }\n", tr->tfi, tr->fail_cause);
}

static void handle_rlc_ul_evt_cnt(struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rlc_ul_event_counts *uec;
	uec = (struct gprs_rlc_ul_event_counts *) msgb_data(msg);

	printf("RLC-UL-EVT-CNT { llc_event_cnt=%u, mac_event_cnt=%u, pl1_event_cnt=%u, tmr_event_cnt=%u }\n",
		uec->llc_event_cnt, uec->mac_event_cnt, uec->pl1_event_cnt, uec->tmr_event_cnt);
}

static void handle_rlc_ul_stats(struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rlc_ul_stats *uls;
	uls = (struct gprs_rlc_ul_stats *) msgb_data(msg);

	printf("RLC-UL-STATS { state=%s(%s), FIXME... }\n",
		get_value_string(gprs_rlc_ul_state_vals, uls->rlc_ul_state),
		get_value_string(gprs_rlc_ul_substate_vals, uls->rlc_ul_substate));
}

static void handle_rlc_dl_stats(struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rlc_dl_stats *dls;
	dls = (struct gprs_rlc_dl_stats *) msgb_data(msg);

	printf("RLC-DL-STATS { state=%s, FIXME... }\n",
		get_value_string(gprs_rlc_dl_state_vals, dls->rlc_dl_state));
}

static void handle_rlc_rel(struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rlc_release_ind *rli;
	rli = (struct gprs_rlc_release_ind *) msgb_data(msg);
	char ud = 'D';

	if (lh->code == LOG_GPRS_RLC_UL_RELEASE_IND_C)
		ud ='U';

	printf("RLC-%cL-RELEASE { tfi=%u, cause=%u }\n", ud, rli->tfi, rli->cause);
}

static void handle_gmm_ota_msg(struct log_hdr *lh, struct msgb *msg)
{
	printf("GMM-OTA-MESSAGE { FIXME }\n");
}

static const struct diag_log_dispatch_tbl log_tbl[] = {
	{ GSM(LOG_GPRS_LLC_ME_INFO_C), handle_llc_me_info },
	{ GSM(LOG_GPRS_LLC_PDU_STATS_C), handle_llc_pdu_stats },
	{ GSM(LOG_GPRS_GRR_STATE_C), handle_grr_state_msg },
	{ GSM(LOG_GPRS_RLC_UL_ABNRML_RLS_COUNTS_C), handle_rlc_ul_abnrml_rls },
	{ GSM(LOG_GPRS_RLC_UL_EVENT_COUNTS_C), handle_rlc_ul_evt_cnt },
	{ GSM(LOG_GPRS_RLC_UL_RELEASE_IND_C), handle_rlc_rel },
	{ GSM(LOG_GPRS_RLC_DL_RELEASE_IND_C), handle_rlc_rel },
	{ GSM(LOG_GPRS_MAC_SIGNALLING_MESSAGE_C), handle_mac_sign_msg },
	{ GSM(LOG_GPRS_MAC_STATE_C), handle_mac_state },
	{ GSM(LOG_GPRS_MAC_DL_TBF_ESTABLISH_C), handle_mac_dl_tbf_est },
	{ GSM(LOG_GPRS_MAC_UL_TBF_ESTABLISH_C), handle_mac_ul_tbf_est },
	{ GSM(LOG_GPRS_MAC_DL_TBF_RELEASE_C), handle_mac_dl_tbf_rel },
	{ GSM(LOG_GPRS_MAC_UL_TBF_RELEASE_C), handle_mac_ul_tbf_rel },
	{ GSM(LOG_GPRS_RLC_UL_STATS_C), handle_rlc_ul_stats },
	{ GSM(LOG_GPRS_RLC_DL_STATS_C), handle_rlc_dl_stats },
	{ GSM(LOG_GPRS_SM_GMM_OTA_MESSAGE_C), handle_gmm_ota_msg },
};

static __attribute__((constructor)) void on_dso_load_gprs(void)
{
	diag_log_reg_dispatch(log_tbl, ARRAY_SIZE(log_tbl));
}
