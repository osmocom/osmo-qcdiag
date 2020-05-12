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
#include "protocol/diag_log_gprs_rlc.h"
#include "protocol/diag_log_gprs_mac.h"
#include "protocol/diag_log_gprs_l1.h"


static void handle_grr_state_msg(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct diag_gprs_grr_state *rrs = (struct diag_gprs_grr_state *) msgb_data(msg);

	printf("GRR-STATE { active=%u, state=%s }\n", rrs->active_state,
		get_value_string(diag_gprs_grr_st_vals, rrs->grr_state));
}

static void handle_rlc_ul_abnrml_rls(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rlc_ul_abnrml_rls_counts *arc;
	arc = (struct gprs_rlc_ul_abnrml_rls_counts *) msgb_data(msg);

	printf("RLC-UL-ABNRML-RLS-COUNTS { access_reject_cnt=%u, arc_retry_cnt=%u, arc_wo_retry_cnt=%u, arc_sys_info_cnt=%u }",
		arc->access_reject_cnt, arc->arc_retry_cnt, arc->arc_wo_retry_cnt, arc->arc_sys_info_cnt);
}

static void handle_mac_sign_msg(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	uint32_t ctype = 0;
	uint16_t arfcn = di->gsm_arfcn;
	uint8_t *data;
	unsigned int len;
	struct gprs_mac_signalling_msg *msm;
	msm = (struct gprs_mac_signalling_msg *) msgb_data(msg);
	di->rat_type = DIAG_INST_RAT_2G;
//	printf("MAC-SIGN-MSG { chan_type=%s, msg_type=%s, msg=%s }\n",
//		get_value_string(gprs_mac_chan_type_vals, msm->chan_type),
//		get_value_string(gprs_mac_msg_type_vals, msm->msg_type),
//			osmo_hexdump(msm->msg, msm->msg_len));

	if(msm->msg_type == PACKET_CHANNEL_REQUEST)
		return;
	if(msm->chan_type == 255)
		return;

	switch(msm->chan_type & 0x7f){
		case PACCH_RRBP_CHANNEL:
		case UL_PACCH_CHANNEL:
		case DL_PACCH_CHANNEL:
			ctype = GSMTAP_CHANNEL_PACCH;
			break;
		default:
			printf("Unhandled MAC-SIGN-MSG: %s  %u: %s\n",
				get_value_string(gprs_mac_chan_type_vals, msm->chan_type & 0x7f),
				msm->msg_len,
				osmo_hexdump(msgb_data(msg), msm->msg_len));
			return;
	}

	arfcn = msm->chan_type & 0x80 ? arfcn: arfcn | GSMTAP_ARFCN_F_UPLINK;

	data = msm->msg;
	len = msm->msg_len;

	/* prepend flag */
	data--;
	*data = 1 << 6; /* PAYLOAD_TYPE_CTRL_NO_OPT_OCTET */
	len++;


	/* abis due to no lapdm header */
	if (di->gsmtap && di->flags & DIAG_INST_F_GSMTAP_DECODED) {
		gsmtap_send_ex(di->gsmtap, GSMTAP_TYPE_UM, arfcn, 0, ctype, 0, 0, 0, 0, data, len);
	}
}

static void handle_llc_me_info(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct diag_gprs_llme_info *gli;
	gli = (struct diag_gprs_llme_info *) msgb_data(msg);
	printf("LLC-ME-INFO { state=%s, xid_version=%u, tlli_new=0x%08x, tlli_old=0x%08x, gea=%u, key=%s }\n",
		get_value_string(diag_gprs_llme_st_vals, gli->state), gli->xid_version,
		gli->tlli_new, gli->tlli_old, gli->enc_alg,
		osmo_hexdump_nospc(gli->enc_key, sizeof(gli->enc_key)));
}

static void handle_llc_xid_info(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct diag_gprs_llc_xid_info *glxi;

	glxi = (struct diag_gprs_llc_xid_info *) msgb_data(msg);
	printf("LLC-XID-INFO { sapi=%u, pending_xid_bitmap=0x%04x, iov_i=%u, cur/pref "
		"t200=%u/%u, n200=%u/%u, n201_u=%u/%u, n201_i=%u/%u, md=%u/%u, mu=%u/%u, kd=%u/%u, ku=%u/%u }\n",
		glxi->gllc_sapi, glxi->pending_xid_bitmap, glxi->cur_iov_i,
		glxi->t200.current, glxi->t200.preferred,
		glxi->n200.current, glxi->n200.preferred,
		glxi->n201_u.current, glxi->n201_u.preferred,
		glxi->n201_i.current, glxi->n201_i.preferred,
		glxi->md.current, glxi->md.preferred,
		glxi->mu.current, glxi->mu.preferred,
		glxi->kd.current, glxi->kd.preferred,
		glxi->ku.current, glxi->ku.preferred
	      );
}


static void handle_llc_pdu_stats(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
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

static void handle_mac_state(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
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

static void handle_mac_dl_tbf_est(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_mac_dl_tbf_est *dte;
	dte = (struct gprs_mac_dl_tbf_est *) msgb_data(msg);

	printf("MAC-DL-TBF-EST { tfi=%u, rlc_mode=%u, dl_ts_bmap=0x%x, is_egprs=%u, egprs_win_size=%u, egprs_link_qual_mode=%u, bep_period2=%u }\n",
		dte->dl_tfi, dte->rlc_mode, dte->dl_ts_bmap, dte->is_egprs_tbf, dte->egprs_win_size, dte->egprs_link_qual_mode, dte->bep_period2);
}

static void handle_mac_ul_tbf_est(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_mac_ul_tbf_est *ute;
	ute = (struct gprs_mac_ul_tbf_est *) msgb_data(msg);

	printf("MAC-UL-TBF-EST { tbf_req_cause=%u, acc_granted=%u, radio_prio=%u, peak_tput=%u, ul_tfi=%u, rlc_mode=%u, ul_ts_bmap=0x%x, is_egprs=%u, egprs_win_size=%u, resegment=%u, bep_period2=%u }\n",
		ute->tbf_req_cause, ute->acc_granted, ute->radio_prio, ute->peak_tput, ute->ul_tfi, ute->rlc_mode, ute->ul_ts_bmap, ute->is_egprs_tbf, ute->egprs_win_size, ute->resegment, ute->bep_period2);
}

static void handle_mac_dl_tbf_rel(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_mac_tbf_release *tr;
	tr = (struct gprs_mac_tbf_release *) msgb_data(msg);

	printf("MAC-DL-TBF-REL { tfi=%u, fail_cause=%u }\n", tr->tfi, tr->fail_cause);
}

static void handle_mac_ul_tbf_rel(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_mac_tbf_release *tr;
	tr = (struct gprs_mac_tbf_release *) msgb_data(msg);

	printf("MAC-DL-TBF-REL { tfi=%u, fail_cause=%u }\n", tr->tfi, tr->fail_cause);
}

static void handle_mac_dl_acknack(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_mac_dl_acknack *da;
	da = (struct gprs_mac_dl_acknack *) msgb_data(msg);

	printf("MAC-DL-ACK-NACK { is_egprs=%d, acknack=%s }\n", da->is_egprs,
		osmo_hexdump(msgb_data(msg)+1, msgb_length(msg)-1));
}

static void handle_mac_ul_acknack(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	//struct gprs_mac_ul_acknack *ua;
	//ua = (struct gprs_mac_ul_acknack *) msgb_data(msg);

	printf("MAC-DL-ACK-NACK { acknack=%s }\n",
		osmo_hexdump(msgb_data(msg), msgb_length(msg)));
}

static void handle_rlc_ul_evt_cnt(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rlc_ul_event_counts *uec;
	uec = (struct gprs_rlc_ul_event_counts *) msgb_data(msg);

	printf("RLC-UL-EVT-CNT { llc_event_cnt=%u, mac_event_cnt=%u, pl1_event_cnt=%u, tmr_event_cnt=%u }\n",
		uec->llc_event_cnt, uec->mac_event_cnt, uec->pl1_event_cnt, uec->tmr_event_cnt);
}

static void handle_rlc_ul_stats(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rlc_ul_stats *uls;
	uls = (struct gprs_rlc_ul_stats *) msgb_data(msg);

	printf("RLC-UL-STATS { state=%s(%s), FIXME... }\n",
		get_value_string(gprs_rlc_ul_state_vals, uls->rlc_ul_state),
		get_value_string(gprs_rlc_ul_substate_vals, uls->rlc_ul_substate));
}

static void handle_rlc_dl_stats(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rlc_dl_stats *dls;
	dls = (struct gprs_rlc_dl_stats *) msgb_data(msg);

	printf("RLC-DL-STATS { state=%s, FIXME... }\n",
		get_value_string(gprs_rlc_dl_state_vals, dls->rlc_dl_state));
}

static void handle_rlc_ul_header(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rlc_ul_header *ulh;
	ulh = (struct gprs_rlc_ul_header *) msgb_data(msg);

	printf("RLC-UL-HEADER { type=%s, hdr=%s }\n",
		get_value_string(gprs_rlc_hdr_type_vals, ulh->type),
		osmo_hexdump(ulh->ul_hdr, sizeof(ulh->ul_hdr)));
}

static void handle_rlc_rel(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rlc_release_ind *rli;
	rli = (struct gprs_rlc_release_ind *) msgb_data(msg);
	char ud = 'D';

	if (lh->code == LOG_GPRS_RLC_UL_RELEASE_IND_C)
		ud ='U';

	printf("RLC-%cL-RELEASE { tfi=%u, cause=%u }\n", ud, rli->tfi, rli->cause);
}

static void handle_gmm_ota_msg(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{	uint32_t ctype = 0;
	uint16_t arfcn = di->gsm_arfcn;
	uint8_t *data;
	unsigned int len;
	struct gprs_sm_gmm_ota_msg *msm;
	msm = (struct gprs_sm_gmm_ota_msg *) msgb_data(msg);
	di->rat_type = DIAG_INST_RAT_2G;

	arfcn = msm->chan_type ? arfcn: arfcn | GSMTAP_ARFCN_F_UPLINK;

	data = msm->msg;
	len = msm->msg_len;

	if (di->gsmtap && di->flags & DIAG_INST_F_GSMTAP_DECODED) {
		gsmtap_send_ex(di->gsmtap, GSMTAP_TYPE_ABIS, arfcn, 0, ctype, 0, 0, 0, 0, data, len);
	}
}

static void handle_ul_acknack_v2(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rlc_ul_acknack_params_v2 *ula;
	ula = (struct gprs_rlc_ul_acknack_params_v2 *) msgb_data(msg);

	printf("RLC-UL-ACKNACK-V2 { tfi=%u, final_ack_ind=%u, start_seq_nr=%u, cs=%s, countdown_val=%u, va=%u, vs=%u, stall_ind=%u, rrb=%08x%08x }\n",
		ula->ul_tfi, ula->final_ack_ind, ula->start_seq_nr, get_value_string(gprs_coding_schemes, ula->coding_scheme),
		ula->countdown_val, ula->va, ula->vs, ula->stall_ind, ula->rrb_high32, ula->rrb_low32);
}

static void handle_dl_acknack_v2(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rlc_dl_acknack_params_v2 *dla;
	dla = (struct gprs_rlc_dl_acknack_params_v2 *) msgb_data(msg);

	printf("RLC-DL-ACKNACK-V2 { tfi=%u, final_ack_ind=%u, start_seq_nr=%u, vq=%u, cs=%s, rrb=%08x%08x }\n",
		dla->dl_tfi, dla->final_ack_ind, dla->start_seq_nr, dla->vq,
		get_value_string(gprs_coding_schemes, dla->coding_scheme), dla->rrb_high32, dla->rrb_low32);
}

static void handle_tx_sched_res(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_tx_sched_res *tsr;
	tsr = (struct gprs_tx_sched_res *) msgb_data(msg);
	int i;

	printf("GPRS-TX-SCHED-RES { fn=%u, num_msg_tx=%u, arfcm = [ %u, %u, %u, %u ], msg_info = [ ",
		tsr->fn, tsr->num_msg_tx, tsr->arfcn[0], tsr->arfcn[1], tsr->arfcn[2], tsr->arfcn[3]);
	for (i = 0; i < tsr->num_msg_tx; i++) {
		printf("{ tn=%u, chan_type=%s, dyn_alloc=%u, power_idx=%d, ta=%u, cs=%u }",
			tsr->msg_info[i].tn,
			get_value_string(gprs_tx_ul_chans, tsr->msg_info[i].chan_type),
			tsr->msg_info[i].dyn_alloc, tsr->msg_info[i].power_idx,
			tsr->msg_info[i].ta, tsr->msg_info[i].cs);

		if (i < tsr->num_msg_tx-1)
			printf(", ");
	}
	printf(" ] }\n");
}

static void handle_gprs_power_control(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_power_control *gpc;
	gpc = (struct gprs_power_control *) msgb_data(msg);

	printf("GPRS-POWER-CONTROL { tx_channel=%u, tx_tn=%u, tx_pwr=%u, cs=%s, gamma_tn=%u, gamma_band=%u, "
		"alpha=%u, derived_c=%u, pmax=%u }\n", gpc->tx_channel, gpc->tx_tn, gpc->tx_pwr,
		get_value_string(gprs_coding_schemes, gpc->cs), gpc->gamma_tn, gpc->gamma_band,
		gpc->alpha, gpc->derived_c, gpc->pmax);
}

static void handle_gprs_xfer_sum(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_xfer_sum *gxs;
	gxs = (struct gprs_xfer_sum *) msgb_data(msg);

	printf("GPRS-XFER-SUMMARY { band=%u, cur_alloc=%u, ul_ptcch_ts=%u, dl_ptcch_ts=%u, ta=%u, usf_granularity=%u, "
		"ul_tn_bitmap=0x%02x, dl_tn_bitmap=0x%02x }\n", gxs->band_ind, gxs->current_alloc, gxs->ul_ptcch_ts,
		gxs->dl_ptcch_ts, gxs->ta, gxs->usf_granularity, gxs->ul_bitmap_tn, gxs->dl_bitmap_tn);
}

static void handle_gprs_aif_sum(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_air_if_summary *gaifs;
	gaifs = (struct gprs_air_if_summary *) msgb_data(msg);

	printf("GPRS-AIR-IF-SUM { fn=%d, band=%u, dl_ts=%u, ul_ts=%u, rx_power=%d. ... }\n",
		gaifs->fn, gaifs->band_ind, gaifs->dl_ts, gaifs->ul_ts, gaifs->rx_power);
}

static void handle_gprs_rx_msg_metrics_a_v2(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct gprs_rx_msg_metr_a_v2 *metr;
	metr = (struct gprs_rx_msg_metr_a_v2 *) msgb_data(msg);

	printf("GPRS-RX-METR-A-V2 { fn=%d, tn=%d, chan=%u, cs=%s, rx_qual=%u crc_passed = [ %u, %u, %u ], usf_match=%u, dl_tfi_match=%u, msg_len=%u, usf=%u, ... }\n",
		metr->fn, metr->tn, metr->chan, get_value_string(gprs_coding_schemes, metr->cs),
		metr->rx_qual,
		metr->crc_passed, metr->egprs_msg1_crc_passed, metr->egprs_msg2_crc_passed,
		metr->usf_match, metr->dl_tfi_match,
		metr->msg_len, metr->usf);
}

static inline uint32_t round_next_octet(uint32_t num_bits)
{
	uint32_t num_bytes = num_bits / 8;
	if (num_bits % 8)
		num_bytes++;
	return num_bytes;
}

static void handle_egprs_rlc_epdan(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct diag_egprs_rlc_epdan *epd = (struct diag_egprs_rlc_epdan *) msgb_data(msg);

	printf("EGPRS-RLC-EPDAN { tfi=%u, final_ack=%u, begin_of_win=%u, end_of_win=%u, esp=%u, starting_color_code=%u, gmsk=%u, psk=%u, ssn=%u, crrb_num_bits=%u, crrb=%s, ",
		epd->tfi, epd->final_ack_ind, epd->begin_of_window, epd->end_of_window,
		epd->esp, epd->starting_color_code, epd->gmsk_valid, epd->psk_valid,
		epd->ssn, epd->crrb_num_bits,
		osmo_hexdump_nospc(epd->crrb, round_next_octet(epd->crrb_num_bits)));
	printf("urrb_num_bits=%u, urrb=%s, gmsk_bep=%u, psk_bep=%u, c_value=%u }\n",
		epd->urrb_num_bits,
		osmo_hexdump_nospc(epd->urrb, round_next_octet(epd->urrb_num_bits)),
		epd->gmsk_bep, epd->psk_bep, epd->c_value);
}

static const struct diag_log_dispatch_tbl log_tbl[] = {
#if 0
	/* LLC */
	{ GSM(LOG_GPRS_LLC_ME_INFO_C), handle_llc_me_info },		/* requested? */
	{ GSM(LOG_GPRS_LLC_PDU_STATS_C), handle_llc_pdu_stats },	/* requested? */
	{ GSM(LOG_GPRS_LLC_XID_INFO_C), handle_llc_xid_info },		/* requested? */
	/* RR */
	{ GSM(LOG_GPRS_GRR_STATE_C), handle_grr_state_msg },
	/* RLC */
	{ GSM(LOG_GPRS_RLC_UL_ABNRML_RLS_COUNTS_C), handle_rlc_ul_abnrml_rls },
	{ GSM(LOG_GPRS_RLC_UL_EVENT_COUNTS_C), handle_rlc_ul_evt_cnt },
	{ GSM(LOG_GPRS_RLC_UL_RELEASE_IND_C), handle_rlc_rel },
	{ GSM(LOG_GPRS_RLC_DL_RELEASE_IND_C), handle_rlc_rel },
	{ GSM(LOG_GPRS_RLC_UL_STATS_C), handle_rlc_ul_stats },
	{ GSM(LOG_GPRS_RLC_DL_STATS_C), handle_rlc_dl_stats },
	{ GSM(LOG_GPRS_RLC_UL_ACKNACK_PARAMS_VER2_C), handle_ul_acknack_v2 },
	{ GSM(LOG_GPRS_RLC_DL_ACKNACK_PARAMS_VER2_C), handle_dl_acknack_v2 },
	{ GSM(LOG_EGPRS_RLC_UL_HEADER_C), handle_rlc_ul_header },
	{ GSM(LOG_EGPRS_RLC_EPDAN_C), handle_egprs_rlc_epdan },
	{ 0x5206, diag_log_hdl_default },
	/* MAC */
	{ GSM(LOG_GPRS_MAC_STATE_C), handle_mac_state },
	#endif
	{ GSM(LOG_GPRS_MAC_SIGNALLING_MESSAGE_C), handle_mac_sign_msg },
	{ GSM(LOG_GPRS_SM_GMM_OTA_MESSAGE_C), handle_gmm_ota_msg },
	#if 0
	{ GSM(LOG_GPRS_MAC_DL_TBF_ESTABLISH_C), handle_mac_dl_tbf_est },
	{ GSM(LOG_GPRS_MAC_UL_TBF_ESTABLISH_C), handle_mac_ul_tbf_est },
	{ GSM(LOG_EGPRS_MAC_DL_ACKNACK_C), handle_mac_dl_acknack },
	{ GSM(LOG_EGPRS_MAC_UL_ACKNACK_C), handle_mac_ul_acknack },
	{ GSM(LOG_GPRS_MAC_DL_TBF_RELEASE_C), handle_mac_dl_tbf_rel },
	{ GSM(LOG_GPRS_MAC_UL_TBF_RELEASE_C), handle_mac_ul_tbf_rel },


	/* Layer 1 */
	{ 0x5230, diag_log_hdl_default },
	{ GSM(LOG_GPRS_AIR_INTERFACE_SUMMARY_C), handle_gprs_aif_sum },
	{ 0x5232, diag_log_hdl_default },
	{ GSM(LOG_GPRS_POWER_CONTROL_C), handle_gprs_power_control },
	{ GSM(LOG_GPRS_TRANSFER_SUMMARY_C), handle_gprs_xfer_sum },
	{ GSM(LOG_GPRS_TX_SCHEDULED_RESULTS_C), handle_tx_sched_res },

	{ GSM(LOG_GPRS_RECEIVE_MSG_METRICS_A_VER2_C), handle_gprs_rx_msg_metrics_a_v2 },
	{ 0x524e, diag_log_hdl_default },
	{ 0x524f, diag_log_hdl_default },
	{ 0x5250, diag_log_hdl_default },

	{ 0x51f4, diag_log_hdl_default },
	{ 0x51f5, diag_log_hdl_default },
	{ 0x51f6, diag_log_hdl_default },
	{ 0x51f7, diag_log_hdl_default },

	//{ 0x508d, diag_log_hdl_default }, hardware cmd
	{ 0x508f, diag_log_hdl_default },

	{ 0x5209, diag_log_hdl_default },
	#endif
};

static __attribute__((constructor)) void on_dso_load_gprs(void)
{
	diag_log_reg_dispatch(log_tbl, ARRAY_SIZE(log_tbl));
}
