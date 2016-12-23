
#include <stdlib.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <osmocom/core/msgb.h>

#include "framing.h"
#include "protocol.h"
#include "serial.h"
#include "config.h"
#include "diagcmd.h"
#include "diag_gsm.h"
#include "log_codes_gsm.h"
#include "log_codes_wcdma.h"
#include "log_codes_qmi.h"
#include "diag_wcdma.h"
#include "gprs_rlc.h"
#include "gprs_mac.h"
#include "qmi_decode.h"

static int transmit_msgb(int fd, struct msgb *msg)
{
	int out_len, rc;
	uint8_t packet[MAX_PACKET * 2];

	out_len = frame_pack(msgb_data(msg), msgb_length(msg),
			     packet, sizeof(packet));
	if (out_len < 0) {
		printf("Failed to pack packet\n");
		return -1;
	}

	rc = write(fd, packet, out_len);
	if (rc != out_len) {
		printf("Short write on packet.\n");
		return -1;
	}

	msgb_free(msg);

	return 0;
}

static int transmit_packet(int fd, const uint8_t *data, size_t data_len)
{
	int out_len, rc;
	uint8_t packet[MAX_PACKET * 2];	

	out_len = frame_pack(data, data_len, packet, sizeof(packet));
	if (out_len < 0) {
		printf("Failed to pack packet\n");
		return -1;
	}

	rc = write(fd, packet, out_len);
	if (rc != out_len) {
		printf("Short write on packet.\n");
		return -1;
	}

	return 0;
}

static int dump_log(const uint8_t *data, const size_t len)
{
	const struct ext_log_msg *msg;
	const char *file = NULL, *fmt;
	unsigned int num_args;

	if (len < sizeof(struct ext_log_msg)) {
		printf("too short log message.\n");
		return -1;
	}

	msg = (struct ext_log_msg *) data;
	num_args = msg->num_args;
	fmt = (const char *) msg->params + num_args*sizeof(msg->params[0]);
	file = fmt + strlen(fmt) + 1;

	printf("%"PRIu64" %-20s(%u): ", msg->timestamp, file, msg->line_nr);
	switch (num_args) {
	case 0:
		fputs(fmt, stdout);
		break;
	case 1:
		printf(fmt, msg->params[0]);
		break;
	case 2:
		printf(fmt, msg->params[0], msg->params[1]);
		break;
	case 3:
		printf(fmt, msg->params[0], msg->params[1], msg->params[2]);
		break;
	case 4:
		printf(fmt, msg->params[0], msg->params[1], msg->params[2], msg->params[3]);
		break;
	}
	fputc('\n', stdout);
	return 0;
}

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

static void handle_grr_state_msg(struct log_hdr *lh, struct msgb *msg)
{
	struct diag_gprs_grr_state *rrs = (struct diag_gprs_grr_state *) msgb_data(msg);

	printf("GRR-STATE { active=%u, state=%s }\n", rrs->active_state,
		get_value_string(diag_gprs_grr_st_vals, rrs->grr_state));
}

static void handle_rrc_sig_msg(struct log_hdr *lh, struct msgb *msg)
{
	struct diag_umts_rrc_msg *rrm = (struct diag_umts_rrc_msg *) msgb_data(msg);

	printf("RRC: %u %u %u: %s\n", rrm->chan_type, rrm->rb_id, rrm->length,
		osmo_hexdump(msgb_data(msg), rrm->length));
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



struct log_dispatch_tbl {
	uint16_t code;
	void (*handler)(struct log_hdr *lh, struct msgb *msg);
};

#define GSM(x)	(0x5000 + x)
#define UMTS(x)	(0x4000 + x)

static const struct log_dispatch_tbl log_tbl[] = {
	{ GSM(LOG_GSM_RR_SIGNALING_MESSAGE_C), handle_rr_sig_msg },
	{ GSM(LOG_GSM_RR_STATE_C), handle_rr_state_msg },
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
	{ UMTS(LOG_WCDMA_SIGNALING_MSG_C), handle_rrc_sig_msg },
};

static void diag_log_handle(struct msgb *msg)
{
	struct diag_log_hdr *dlh;
	struct log_hdr *lh;
	int i;

	dlh = (struct diag_log_hdr *) msg->data;
	/* FIXME: verify length */
	msg->l3h = msgb_pull(msg, sizeof(*dlh));

	lh = (struct log_hdr *) msg->l3h;
	/* FIXME: verify length */
	msgb_pull(msg, sizeof(*lh));

	for (i = 0; i < ARRAY_SIZE(log_tbl); i++) {
		if (log_tbl[i].code == lh->code) {
			log_tbl[i].handler(lh, msg);
			return;
		}
	}

	printf("LOG(0x%04x, %"PRIu64"u, %u): %s\n", lh->code, lh->ts, lh->len,
		osmo_hexdump(lh->data, lh->len));

	uint8_t subsys = lh->code >> 12;

	if (subsys == 0x01 &&
	    ((lh->code & 0xfff) >= LOG_QMI_RESERVED_CODES_BASE_C) &&
	    ((lh->code & 0xfff) <= LOG_QMI_LAST_C))
		dump_qmi_msg(lh->data, lh->len);
}

/*********/

static int do_read(int fd)
{
	uint8_t buf[MAX_PACKET*2];
	struct msgb *msg = msgb_alloc(MAX_PACKET, "DIAG Rx");
	int rc;

	rc = read(fd, buf, sizeof(buf));
	if (rc <= 0 ) {
		printf("Short read!\n");
		exit(EXIT_FAILURE);
	}

	rc = frame_unpack(buf, rc, msgb_data(msg));
	if (rc <= 0) {
		msgb_free(msg);
		return rc;
	}
	msg->l2h = msgb_put(msg, rc);

	switch (msg->l2h[0]) {
	case DIAG_LOG_F:
		diag_log_handle(msg);
		break;
	case DIAG_EXT_MSG_F:
		dump_log(msgb_data(msg), msgb_length(msg));
		break;
	default:
		printf("Got %d data of payload\n", rc); 
		printf("%s\n", osmo_hexdump(msgb_data(msg), msgb_length(msg)));
		break;
	};

	msgb_free(msg);

	return rc;
}

static void do_configure(int fd)
{
	static uint8_t timestamp[] = { DIAG_TS_F };
	static const uint8_t enable_evt_report[] = {
		DIAG_EVENT_REPORT_F, 0x01
	};
	static const uint8_t disable_evt_report[] = {
		DIAG_EVENT_REPORT_F, 0x00
	};
	static const uint8_t extended_report_cfg[] = {
		/* command code */
		DIAG_EXT_MSG_CONFIG_F,
		/* sub command */
		0x04,
		/* subsystem ID range start */
		0x00, 0x00,
		/* subsystem ID range end */
		0x00, 0x00,
		/* pad */
		0x00, 0x00,
		/* array of runtime masks */
		0x02, 0x00, 0x00, 0x00,
	};

	/* TODO: introduce a wait for response kind of method */
	transmit_packet(fd, timestamp, sizeof(timestamp));
	do_read(fd);

	/* enable|disable the event report */
#if 0
	transmit_packet(fd, enable_evt_report, sizeof(enable_evt_report));
	do_read(fd);
#else
	transmit_packet(fd, disable_evt_report, sizeof(disable_evt_report));
	do_read(fd);
#endif

	transmit_packet(fd, extended_report_cfg, sizeof(extended_report_cfg));
	do_read(fd);

	printf("GSM\n");
	struct msgb *msg = gen_log_config_set_mask(5, 1064);
#if 0
	for (int i = 0; i < 1064; i++)
		log_config_set_mask_bit(msg, i);
#endif

	log_config_set_mask_bit(msg, LOG_GSM_RR_CONTROL_CHANNEL_PARAMS_C);
	log_config_set_mask_bit(msg, LOG_GSM_RR_SIGNALING_MESSAGE_C);
	log_config_set_mask_bit(msg, LOG_GSM_RR_STATE_C);

	log_config_set_mask_bit(msg, LOG_GPRS_GRR_STATE_C);

	log_config_set_mask_bit(msg, LOG_GPRS_RLC_UL_ABNRML_RLS_COUNTS_C);
	log_config_set_mask_bit(msg, LOG_GPRS_RLC_UL_EVENT_COUNTS_C);
	log_config_set_mask_bit(msg, LOG_GPRS_RLC_UL_STATS_C);
	log_config_set_mask_bit(msg, LOG_GPRS_RLC_DL_STATS_C);
	log_config_set_mask_bit(msg, LOG_GPRS_RLC_UL_ACKNACK_PARAMS_VER2_C);
	log_config_set_mask_bit(msg, LOG_GPRS_RLC_DL_ACKNACK_PARAMS_VER2_C);
	log_config_set_mask_bit(msg, LOG_GPRS_RLC_UL_RELEASE_IND_C);
	log_config_set_mask_bit(msg, LOG_GPRS_RLC_DL_RELEASE_IND_C);
	log_config_set_mask_bit(msg, LOG_EGPRS_RLC_DL_HEADER_C);
	log_config_set_mask_bit(msg, LOG_EGPRS_RLC_UL_HEADER_C);

	log_config_set_mask_bit(msg, LOG_GPRS_LLC_ME_INFO_C);
	log_config_set_mask_bit(msg, LOG_GPRS_LLC_PDU_STATS_C);
	log_config_set_mask_bit(msg, LOG_GPRS_LLC_PERIODIC_STATS_C);

	log_config_set_mask_bit(msg, LOG_GPRS_SNDCP_UL_TCP_HDR_C);
	log_config_set_mask_bit(msg, LOG_GPRS_SNDCP_DL_TCP_HDR_C);
	log_config_set_mask_bit(msg, 546);
	log_config_set_mask_bit(msg, 547);
	log_config_set_mask_bit(msg, LOG_GPRS_MAC_STATE_C);
	log_config_set_mask_bit(msg, LOG_GPRS_MAC_SIGNALLING_MESSAGE_C);
	log_config_set_mask_bit(msg, LOG_GPRS_MAC_UL_TBF_ESTABLISH_C);
	log_config_set_mask_bit(msg, LOG_GPRS_MAC_DL_TBF_ESTABLISH_C);
	log_config_set_mask_bit(msg, LOG_EGPRS_MAC_UL_ACKNACK_C);
	log_config_set_mask_bit(msg, LOG_EGPRS_MAC_DL_ACKNACK_C);
	log_config_set_mask_bit(msg, LOG_GPRS_MAC_UL_TBF_RELEASE_C);
	log_config_set_mask_bit(msg, LOG_GPRS_MAC_DL_TBF_RELEASE_C);

	transmit_msgb(fd, msg);
	do_read(fd);

	printf("WCDMA\n");
	msg = gen_log_config_set_mask(4, 1064);
#if 0
	for (int i = 0; i < 1064; i++)
		log_config_set_mask_bit(msg, i);
#endif
	log_config_set_mask_bit(msg, 0x125);
	log_config_set_mask_bit(msg, 0x126);
	log_config_set_mask_bit(msg, 0x127);
	log_config_set_mask_bit(msg, 0x128);
	log_config_set_mask_bit(msg, 0x129);
	log_config_set_mask_bit(msg, LOG_WCDMA_SIGNALING_MSG_C);

	transmit_msgb(fd, msg);
	do_read(fd);


	printf("Core\n");
	msg = gen_log_config_set_mask(1, 1064);
	log_config_set_mask_bit(msg, LOG_QMI_CALL_FLOW_C);
	log_config_set_mask_bit(msg, LOG_QMI_SUPPORTED_INTERFACES_C);
	for (int i = 0; i < 16*2; i++)
		log_config_set_mask_bit(msg, LOG_QMI_RESERVED_CODES_BASE_C+i);
	for (int i = LOG_QMI_RESERVED_CODES_BASE_C; i < LOG_QMI_LAST_C; i++)
		log_config_set_mask_bit(msg, i);

	transmit_msgb(fd, msg);
	do_read(fd);
}

int main(int argc, char **argv)
{
	int flags, i;

	int fd, rc;
	if (argc < 2) {
		printf("Invoke with %s PATH_TO_SERIAL\n",
			argv[0]);
		return EXIT_FAILURE;
	}

	/* Use nonblock as the device might block otherwise */
	fd = open(argv[1], O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK);
	if (fd < 0) {
		printf("Opening the serial failed: %d/%s\n",
			errno, strerror(errno));
		return EXIT_FAILURE;
	}

	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0) {
		printf("Failed to get the flags.\n");
		return EXIT_FAILURE;
	}

	flags &= ~O_NONBLOCK;
	rc = fcntl(fd, F_SETFL, flags);
	if (rc != 0) {
		printf("Failed to set the flags.\n");
		return EXIT_FAILURE;
	}

	rc = serial_configure(fd);
	if (rc != 0)
		return EXIT_FAILURE;

	do_configure(fd);

	while (1) {
		i++;
		do_read(fd);
#if 0
		/* some packets need to be explicitly requested and
		 * don't appear automatically */
		if (i % 10 == 0) {
			struct msgb *msg = diag_gsm_make_log_pack_req(LOG_GPRS_LLC_PDU_STATS_C , 0, 0);
			printf("Requesting LLC stats...(%s)\n", osmo_hexdump(msgb_data(msg), msgb_length(msg)));
			transmit_msgb(fd, msg);
		}
#endif

	}
}
