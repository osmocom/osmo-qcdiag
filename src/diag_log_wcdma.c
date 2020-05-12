#include <stdio.h>

#include <osmocom/core/gsmtap.h>
#include <osmocom/core/gsmtap_util.h>
#include <osmocom/core/utils.h>

#include "diag_log.h"
#include "protocol/diag_log_gsm.h"
#include "protocol/diag_log_wcdma.h"

const struct value_string rrc_states[] = {
	{ DIAG_UMTS_RRC_STATE_DISCONNECTED,	"DISCONNETED" },
	{ DIAG_UMTS_RRC_STATE_CONNETING,	"CONNECTING" },
	{ DIAG_UMTS_RRC_STATE_CELL_FACH,	"CELL_FACH" },
	{ DIAG_UMTS_RRC_STATE_CELL_DCH,		"CELL_DCH" },
	{ DIAG_UMTS_RRC_STATE_CELL_PCH,		"CELL_PCH" },
	{ DIAG_UMTS_RRC_STATE_URA_PCH,		"URA_PCH" },
	{ 0, NULL }
};

const struct value_string rrc_procedures[] = {
	{ DIAG_UMTS_RRC_PROC_CSP,	"Cell Selection" },
	{ DIAG_UMTS_RRC_PROC_SIB,	"SIB Processing" },
	{ DIAG_UMTS_RRC_PROC_PG2,	"Paging Type 2" },
	{ DIAG_UMTS_RRC_PROC_MCMR,	"Measurement Control Reporting" },
	{ DIAG_UMTS_RRC_PROC_RCE,	"RRC Connection Establishment" },
	{ DIAG_UMTS_RRC_PROC_RCR,	"RRC Connection Release" },
	{ DIAG_UMTS_RRC_PROC_UECI,	"UE Capability Information" },
	{ DIAG_UMTS_RRC_PROC_UECE,	"UE Capability Enquiry" },
	{ DIAG_UMTS_RRC_PROC_IDT,	"Initial Direct Transfer" },
	{ DIAG_UMTS_RRC_PROC_UDT,	"Uplink Direct Transfer" },
	{ DIAG_UMTS_RRC_PROC_DDT,	"Downlink Direct Transfer" },
	{ DIAG_UMTS_RRC_PROC_SCR,	"Signaling Connection Release" },
	{ DIAG_UMTS_RRC_PROC_SCRR,	"Signalling connection Release Request" },
	{ DIAG_UMTS_RRC_PROC_CC,	"Counter Check" },
	{ DIAG_UMTS_RRC_PROC_RBE,	"Radio Bearer Establishment" },
	{ DIAG_UMTS_RRC_PROC_RBRC,	"Radio Bearer Re-configuration" },
	{ DIAG_UMTS_RRC_PROC_RBR,	"Radio Bearer Release" },
	{ DIAG_UMTS_RRC_PROC_TCR,	"Transport Channel Re-configuration" },
	{ DIAG_UMTS_RRC_PROC_PCR,	"Physical Channel Re-configuration" },
	{ DIAG_UMTS_RRC_PROC_TFC,	"Transport Format Combination Control" },
	{ DIAG_UMTS_RRC_PROC_CU,	"Cell Update" },
	{ DIAG_UMTS_RRC_PROC_UU,	"URA Update" },
	{ DIAG_UMTS_RRC_PROC_UMI,	"UTRAN Mobility Information" },
	{ DIAG_UMTS_RRC_PROC_ASU,	"Active Set Update" },
	{ DIAG_UMTS_RRC_PROC_ISHFU,	"Inter-System Handover from UTRAN" },
	{ DIAG_UMTS_RRC_PROC_USHTU,	"Inter-System Handover to UTRAN" },
	{ DIAG_UMTS_RRC_PROC_ISCFU,	"Inter-System Cell Reselection from UTRAN" },
	{ DIAG_UMTS_RRC_PROC_ISCTU,	"Inter-System Cell Reselection to UTRAN" },
	{ DIAG_UMTS_RRC_PROC_PG1,	"Paging Type 1" },
	{ DIAG_UMTS_RRC_PROC_SMC,	"Security Mode Command" },
	{ 0, NULL }
};

const struct value_string rrc_fail_causes[] = {
	{ DIAG_UMTS_RRC_FAIL_CFG_UNSUP,		"Configuration Unsupported" },
	{ DIAG_UMTS_RRC_FAIL_PHY_CHAN_FAIL,	"Physical Channel Failure" },
	{ DIAG_UMTS_RRC_FAIL_INCOMP_SIM_RECONF,	"Incompatible Simultaneous Reconfig" },
	{ DIAG_UMTS_RRC_FAIL_PROT_ERR,		"Protocol Error" },
	{ DIAG_UMTS_RRC_FAIL_COMP_RT_ERR,	"Compressed Mode Runtime Error" },
	{ DIAG_UTMS_RRC_FAIL_CELL_RESEL,	"Cell Reselection" },
	{ DIAG_UMTS_RRC_FAIL_INVAL_CFG,		"Invalid Configuration" },
	{ DIAG_UMTS_RRC_FAIL_CFG_INCOMPLETE,	"Configuration Incomplete" },
	{ DIAG_UMTS_RRC_FAIL_UNSUP_MEAS,	"Unsupported Measurement" },
	{ 0, NULL }
};

const struct value_string rrc_prot_errs[] = {
	{ DIAG_UMTS_RRC_PERR_ASN1_ENC_ERR,	"ASN.1 violation / encoding error" },
	{ DIAG_UMTS_RRC_PERR_BAD_MSG_TYPE,	"Not existing or not implemented message type" },
	{ DIAG_UMTS_RRC_PERR_IMCOMP_RX_STATE,	"Message incompatible with Rx state" },
	{ DIAG_UMTS_RRC_PERR_BAD_IE,		"IE Value not understood" },
	{ DIAG_UMTS_RRC_PERR_COND_IE_ERR,	"Conditional IE error" },
	{ DIAG_UMTS_RRC_PERR_BAD_MSG_EXT,	"Message extension not undersetood" },
	{ 0, NULL }
};

const struct value_string rrc_chan_types[] = {
	{ DIAG_UMTS_RRC_CHT_UL_CCCH,	"CCCH(Uplink)" },
	{ DIAG_UMTS_RRC_CHT_UL_DCCH,	"DCCH(Uplink)" },
	{ DIAG_UMTS_RRC_CHT_DL_CCCH,	"CCCH(Downlink)" },
	{ DIAG_UMTS_RRC_CHT_DL_DCCH,	"DCCH(Downlink)" },
	{ DIAG_UMTS_RRC_CHT_DL_BCCH_BCH,"BCCH/BCH" },
	{ DIAG_UMTS_RRC_CHT_DL_BCCH_FACH, "BCCH/FACH" },
	{ DIAG_UMTS_RRC_CHT_DL_PCCH,	"PCCH" },
	{ DIAG_UMTS_RRC_CHT_EXTENSION_SIB,	 "unknown" },
	{ DIAG_UMTS_RRC_CHT_SIB_CONTAINER,	"unknown" },
	{ 0, NULL }
};


struct wcdma_cell_id {
	uint32_t ul_arfcn;
	uint32_t dl_arfcn;
	uint32_t cell_id;
};

static void handle_rrc_sig_msg(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	uint32_t ctype = 0;
	uint16_t arfcn = 0;
	struct diag_umts_rrc_msg *rrm = (struct diag_umts_rrc_msg *) msgb_data(msg);

	di->rat_type = DIAG_INST_RAT_3G;

//	printf("RRC: %s %u %u: %s\n",
//		get_value_string(rrc_chan_types, rrm->chan_type),
//		rrm->rb_id, rrm->length,
//		osmo_hexdump(msgb_data(msg), rrm->length));

	switch(rrm->chan_type){
		case 254:
		case 255:
		case DIAG_UMTS_RRC_CHT_EXTENSION_SIB:
		case DIAG_UMTS_RRC_CHT_SIB_CONTAINER:
			return;
		case DIAG_UMTS_RRC_CHT_UL_CCCH: ctype =GSMTAP_RRC_SUB_UL_CCCH_Message; break;
		case DIAG_UMTS_RRC_CHT_UL_DCCH: ctype =GSMTAP_RRC_SUB_UL_DCCH_Message; break;
		case DIAG_UMTS_RRC_CHT_DL_CCCH: ctype =GSMTAP_RRC_SUB_DL_CCCH_Message; break;
		case DIAG_UMTS_RRC_CHT_DL_DCCH: ctype =GSMTAP_RRC_SUB_DL_DCCH_Message; break;
		case DIAG_UMTS_RRC_CHT_DL_BCCH_BCH: ctype =GSMTAP_RRC_SUB_BCCH_BCH_Message; break;
		case DIAG_UMTS_RRC_CHT_DL_BCCH_FACH: ctype =GSMTAP_RRC_SUB_BCCH_FACH_Message; break;
		case DIAG_UMTS_RRC_CHT_DL_PCCH: ctype =GSMTAP_RRC_SUB_PCCH_Message; break;
		default:
			printf("Unhandled RRC: %s %u %u: %s\n",
				get_value_string(rrc_chan_types, rrm->chan_type),
				rrm->rb_id, rrm->length,
				osmo_hexdump(msgb_data(msg), rrm->length));
			return;
	}

	arfcn = rrm->chan_type < DIAG_UMTS_RRC_CHT_DL_CCCH ? di->umts_arfcn_ul | GSMTAP_ARFCN_F_UPLINK: di->umts_arfcn_dl;

	if (di->gsmtap && di->flags & DIAG_INST_F_GSMTAP_DECODED) {
		gsmtap_send_ex(di->gsmtap, GSMTAP_TYPE_UMTS_RRC, arfcn, 0, ctype, 0, 0, 0, 0, rrm->msg,rrm->length);
	}
}

static void handle_cell_id(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct wcdma_cell_id *rrm = (struct wcdma_cell_id *) msgb_data(msg);

	di->umts_arfcn_dl =rrm->dl_arfcn;
	di->umts_arfcn_ul =rrm->ul_arfcn;
}


static const struct diag_log_dispatch_tbl log_tbl[] = {
	{ WCDMA(LOG_WCDMA_SIGNALING_MSG_C), handle_rrc_sig_msg },
	{ WCDMA(LOG_WCDMA_CELL_ID_C), handle_cell_id },
};

static __attribute__((constructor)) void on_dso_load_umts(void)
{
	diag_log_reg_dispatch(log_tbl, ARRAY_SIZE(log_tbl));
}
