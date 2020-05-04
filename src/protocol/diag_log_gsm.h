#pragma once

#include <stdint.h>

#define GSM(x)	(0x5000 + x)

enum diag_log_code_gsm {
	LOG_GSM_POWER_SCAN_C				= 0x64,
	LOG_GSM_FCCH_DECODE_C				= 0x65,
	LOG_GSM_SCH_DECODE_C				= 0x66,
	LOG_GSM_NCELL_ACQ_C				= 0x75,
	LOG_GSM_AFC_ADJUST_C				= 0x7c,
	LOG_GSM_MON_BURST_C				= 0x82,
	LOG_GSM_BCCH_BURST_METRICS_C			= 0x85,
	LOG_GSM_MDSP_CMD_C				= 0x8c,
	LOG_GSM_GL1_HW_CMD_C				= 0x8d,
	LOG_GSM_RR_SIGNALING_MESSAGE_C			= 0x12f,

	/* Layer2 (LAPDm) */
	LOG_GSM_L2_STATE_C				= 200,
	LOG_GSM_L2_TRANSMISSION_STATUS_C		= 201,
	LOG_GSM_L2_OUTSTANDING_FRAME_C			= 202,


	LOG_GSM_RR_STATE_C				= 300,
	//= 303,
	LOG_GSM_RR_CONTROL_CHANNEL_PARAMS_C		= 306,
	LOG_GSM_RR_CELL_INFO_C					= 308,

	LOG_GPRS_INDICATORS_C				= 500,
	LOG_GPRS_GENERAL_PARAMS_C			= 501,
	LOG_GPRS_CELL_OPTIONS_C				= 502,
	LOG_GPRS_POWER_CONTROL_PARAMS_C			= 503,
	LOG_GPRS_MOBILE_ALLOCATION_C			= 504,
	LOG_GPRS_PBCCH_DESCRIPTION_C			= 505,
	LOG_GPRS_GRR_STATE_C				= 506,
	LOG_GPRS_RR_CELL_RESELECTION_PARAMS_C		= 507,
	LOG_GPRS_RR_CELL_RESELECTION_MEASMNTS_C		= 508,
	LOG_GPRS_RR_PSI1_C				= 509,
	LOG_GPRS_RR_PSI2_C				= 510,
	LOG_GPRS_RR_PSI3_C				= 511,
	LOG_GPRS_RLC_UL_ABNRML_RLS_COUNTS_C		= 512,
	LOG_GPRS_RLC_UL_EVENT_COUNTS_C			= 513,
	LOG_GPRS_RLC_UL_STATS_C				= 514,
	LOG_EGPRS_RLC_UL_HEADER_C			= 517,
	LOG_EGPRS_RLC_DL_HEADER_C			= 518,
	LOG_EGPRS_RLC_UL_STATS_C			= 519,
	LOG_EGPRS_RLC_DL_STATS_C			= 520,
	LOG_EGPRS_RLC_EPUAN_C				= 521,
	LOG_GPRS_RLC_DL_STATS_C				= 522,
	LOG_GPRS_RLC_UL_ACKNACK_PARAMS_C		= 523,
	LOG_GPRS_RLC_DL_ACKNACK_PARAMS_C		= 524,
	LOG_GPRS_RLC_UL_RELEASE_IND_C			= 525,
	LOG_GPRS_RLC_DL_RELEASE_IND_C			= 526,
	LOG_GPRS_RLC_UL_ACKNACK_PARAMS_VER2_C		= 527,
	LOG_GPRS_RLC_DL_ACKNACK_PARAMS_VER2_C		= 528,
	LOG_EGPRS_RLC_EPDAN_C				= 529,
	LOG_GPRS_LLC_ME_INFO_C				= 530,
	LOG_GPRS_LLC_SAPI_STATES_C			= 531,
	LOG_GPRS_LLC_XID_INFO_C				= 532,
	LOG_GPRS_LLC_PDU_STATS_C			= 533,
	LOG_GPRS_LLC_PERIODIC_STATS_C			= 534,
	LOG_GPRS_LLC_READY_TIMER_STATUS_C		= 535,
	LOG_GPRS_LLC_SEND_UI_FRAME_C			= 536,
	LOG_GPRS_LLC_RECV_UI_FRAME_C			= 537,
	LOG_GPRS_LLC_READY_TIMER_STATUS_VER2_C		= 538,
	LOG_GPRS_SNDCP_ACTIVE_PDP_CONTEXTS_C		= 540,
	LOG_GPRS_SNDCP_PDP_CONTEXT_INFO_C		= 541,
	LOG_GPRS_SNDCP_STATES_C				= 542,
	LOG_GPRS_SNDCP_PDU_STATS_C			= 543,
	LOG_GPRS_SNDCP_UL_TCP_HDR_C			= 544,
	LOG_GPRS_SNDCP_DL_TCP_HDR_C			= 545,
	LOG_GPRS_MAC_SIGNALLING_MESSAGE_C		= 550,
	LOG_GPRS_MAC_STATE_C				= 551,
	LOG_GPRS_MAC_UL_TBF_ESTABLISH_C			= 552,
	LOG_GPRS_MAC_UL_TBF_RELEASE_C			= 553,
	LOG_GPRS_MAC_DL_TBF_ESTABLISH_C			= 554,
	LOG_GPRS_MAC_DL_TBF_RELEASE_C			= 555,
	LOG_EGPRS_MAC_UL_ACKNACK_C			= 556,
	LOG_EGPRS_MAC_DL_ACKNACK_C			= 557,
	LOG_EGPRS_SRB_HARD_DECISIONS_C			= 558,
	LOG_GPRS_SM_GMM_OTA_MESSAGE_C			= 560,
	LOG_GPRS_AIR_INTERFACE_SUMMARY_C		= 561,
	LOG_GPRS_AGC_C					= 562,
	LOG_GPRS_TIMING_ADVANCE_C			= 563,
	LOG_GPRS_POWER_CONTROL_C			= 564,
	LOG_GPRS_TRANSFER_SUMMARY_C			= 565,
	LOG_GPRS_TX_SCHEDULED_RESULTS_C			= 567,
	LOG_GPRS_HW_SCHEDULE_C				= 570,
	LOG_UPLINK_TBF_DATA_BLOCK_COUNT_C		= 575,
	LOG_GPRS_RECEIVE_BURST_METRICS_A_C		= 581,
	LOG_GPRS_RECEIVE_BURST_METRICS_B_C		= 582,
	LOG_GPRS_RECEIVE_BURST_METRICS_C_C		= 583,
	LOG_GPRS_RECEIVE_BURST_METRICS_D_C		= 584,
	LOG_GPRS_RECEIVE_MSG_METRICS_A_C		= 585,
	LOG_GPRS_RECEIVE_MSG_METRICS_B_C		= 586,
	LOG_GPRS_RECEIVE_MSG_METRICS_C_C		= 587,
	LOG_GPRS_RECEIVE_MSG_METRICS_D_C		= 588,
	LOG_GPRS_RECEIVE_MSG_METRICS_A_VER2_C		= 589,
	LOG_GPRS_RECEIVE_MSG_METRICS_B_VER2_C		= 590,
	LOG_GPRS_RECEIVE_MSG_METRICS_C_VER2_C		= 591,
	LOG_GPRS_RECEIVE_MSG_METRICS_D_VER2_C		= 592,
	LOG_GPRS_DLPTCCH_BURST_METRICS_C		= 593,
	LOG_GPRS_DLPTCCH_MSG_METRICS_C			= 594,
};

enum diag_gsm_cmd {
	GSM_DIAG_VERNO_F		= 0,
	GSM_STATUS_F			= 1,
	GSM_TMC_STATE_RETRIEVAL_F	= 2,
	GSM_DIAG_CALL_ORIG_F		= 3,
	GSM_DIAG_CALL_END_F		= 4,
	GSM_GPRS_LOG_PACKET_REQ_F	= 5,
	DIAG_GSM_MAX_F			= 6,
};

/* channel_type | 0x80 == downlink */
enum diag_gsm_rr_chan_type {
	DIAG_GSM_L2_CHAN_TYPE_DCCH	= 0,
	DIAG_GSM_L2_CHAN_TYPE_BCCH	= 1,
	DIAG_GSM_L2_CHAN_TYPE_RACH	= 2,
	DIAG_GSM_L2_CHAN_TYPE_CCCH	= 3,
	DIAG_GSM_L2_CHAN_TYPE_SACCH	= 4,
	DIAG_GSM_L2_CHAN_TYPE_SDCCH	= 5,
	DIAG_GSM_L2_CHAN_TYPE_FACCH_F	= 6,
	DIAG_GSM_L2_CHAN_TYPE_FACCH_H	= 7,
};

/* LOG_GSM_RR_SIGNALING_MESSAGE_C */
struct diag_gsm_rr_msg {
	uint8_t chan_type;
	uint8_t msg_type;
	uint8_t length;
	uint8_t data[0];
} __attribute__((packed));

enum diag_gsm_rr_st {
	DIAG_RR_ST_INACTIVE		= 0,
	DIAG_RR_ST_GOING_ACTIVE		= 1,
	DIAG_RR_ST_GOING_INACTIVE	= 2,
	DIAG_RR_ST_CELL_SELECTION	= 3,
	DIAG_RR_ST_PLMN_LIST_CONSTRUCTION = 4,
	DIAG_RR_ST_IDLE			= 5,
	DIAG_RR_ST_CELL_RESELECTION	= 6,
	DIAG_RR_ST_CONNECTION_PENDING	= 7,
	DIAG_RR_ST_CHOOSE_CELL		= 8,
	DIAG_RR_ST_DATA_TRANSFER	= 9,
	DIAG_RR_ST_NO_CHANNELS		= 10,
	DIAG_RR_ST_CONNECTION_RELEASE	= 11,
	DIAG_RR_ST_EARLY_CAMPED_WAIT_FOR_SI= 12,
	DIAG_RR_ST_W2G_INTERRAT_HO_PROGRESS= 13,
	DIAG_RR_ST_W2G_INTERRAT_RESELECTION_PROGRESS = 14,
	DIAG_RR_ST_W2G_INTERRAT_CC_ORDER_PROGRESS = 15,
	DIAG_RR_ST_G2W_INTERRAT_RESELECTION_PROGRESS = 16,
	DIAG_RR_ST_WAIT_FOR_EARLY_PSCAN	= 17,
	DIAG_RR_ST_GRR			= 18,
	DIAG_RR_ST_G2W_INTERRAT_HO_PROGRESS = 20,
	DIAG_RR_ST_BACKGROUND_PLMN_SEARCH = 21,
	DIAG_RR_ST_W2G_SERVICE_REDIR_IN_PROGRESS = 22,
	DIAG_RR_ST_RESET		= 23,
	DIAG_RR_ST_W2G_BACKGROUND_PLN_SEARCH = 24,
};
extern const struct value_string diag_gsm_rr_st_vals[25];

/* LOG_GSM_RR_STATE_C */
struct diag_gsm_rr_state {
	uint8_t state;
	uint8_t substate;
	uint8_t status;
	uint8_t mode;
} __attribute__((packed));

enum diag_gprs_grr_st {
	DIAG_GRR_ST_NULL	= 0,
	DIAG_GRR_ST_ACQUIRE	= 1,
	DIAG_GRR_ST_CAMPED	= 2,
	DIAG_GRR_ST_CONN_PEND	= 3,
	DIAG_GRR_ST_CELL_RESEL	= 4,
	DIAG_GRR_ST_CELL_RESEL_G2W	= 6,
	DIAG_GRR_ST_BG_PLMN_SEARCH	= 7,
	DIAG_GRR_ST_PCCO	= 8,
};
extern const struct value_string diag_gprs_grr_st_vals[9];

/* LOG_GPRS_GRR_STATE_C */
struct diag_gprs_grr_state {
	uint8_t	active_state;
	uint8_t grr_state;
} __attribute__((packed));


enum diag_gprs_llme_state {
	DIAG_GPRS_LLMES_TLLI_UNASS	= 0,
	DIAG_GPRS_LLMES_TLLI_ASS	= 1,
	DIAG_GPRS_LLMES_TEST_MODE	= 2,
};
const struct value_string diag_gprs_llme_st_vals[4];

/* LOG_GPRS_LLC_ME_INFO_C */
struct diag_gprs_llme_info {
	uint8_t state;
	uint8_t xid_version;
	uint32_t tlli_new;
	uint32_t tlli_old;
	uint8_t enc_alg;
	uint8_t enc_key[8];
	uint32_t input_offset_iov_ui;
} __attribute__((packed));

/* LOG_GPRS_LLC_PDU_STATS_C */
struct diag_gprs_llc_stats {
	uint8_t	sapi;
	uint32_t ack_l3pdu_tx;
	uint32_t ack_octet_tx;
	uint32_t ack_l3pdu_retrans;
	uint32_t ack_octet_retrans;
	uint32_t ack_l3pdu_rx;
	uint32_t ack_octet_rx;
	uint32_t unack_l3pdu_tx;
	uint32_t unack_octet_tx;
	uint32_t unack_l3pdu_rx;
	uint32_t unack_octet_rx;
	uint32_t llpdu_tx;
	uint32_t llpdu_rx;
	uint32_t llpdu_fcs_err;
	uint32_t llpdu_frm_rej;
	uint32_t llpdu_tlli_err;
	uint32_t llpdu_addr_err;
	uint32_t llpdu_short_err;
} __attribute__((packed));

struct diag_xid_tuple32 {
	uint32_t current;
	uint32_t preferred;
} __attribute__ ((packed));

struct diag_xid_tuple16 {
	uint16_t current;
	uint16_t preferred;
} __attribute__ ((packed));

struct diag_xid_tuple8 {
	uint8_t current;
	uint8_t preferred;
} __attribute__ ((packed));

/* LOG_GPRS_LLC_XID_INFO_C */
struct diag_gprs_llc_xid_info {
	uint8_t gllc_sapi;
	uint16_t pending_xid_bitmap;
	uint32_t cur_iov_i;
	struct diag_xid_tuple32 t200;
	struct diag_xid_tuple8 n200;
	struct diag_xid_tuple16 n201_u;
	struct diag_xid_tuple16 n201_i;
	struct diag_xid_tuple16 md;
	struct diag_xid_tuple16 mu;
	struct diag_xid_tuple8 kd;
	struct diag_xid_tuple8 ku;
} __attribute__ ((packed));

struct diag_mdsp_log_cmd {
	uint32_t fn;
	uint16_t cnt;
	uint16_t seq;
	uint16_t cmd;
	uint16_t params[5];
} __attribute__ ((packed));

struct diag_mdsp_log_cmds {
	uint32_t num_cmds;
	struct diag_mdsp_log_cmd cmds[16];
} __attribute__ ((packed));


enum diag_gsm_sapi0_state {
	DIAG_SAPI0_ST_NULL,
	DIAG_SAPI0_ST_CON_PEND,
	DIAG_SAPI0_ST_IDLE,
	DIAG_SAPI0_ST_EST_PEND,
	DIAG_SAPI0_ST_REL_PEND,
	DIAG_SAPI0_ST_LINK_EST,
	DIAG_SAPI0_ST_TMR_RECOV,
	DIAG_SAPI0_ST_LINK_SUSP,
	DIAG_SAPI0_ST_UA_PEND,
};
const struct value_string diag_gsm_l2_sapi0_st_vals[10];

enum diag_gsm_sapi3_state {
	DIAG_SAPI3_ST_NULL,
	DIAG_SAPI3_ST_CON_PEND,
	DIAG_SAPI3_ST_IDLE,
	DIAG_SAPI3_ST_EST_PEND,
	DIAG_SAPI3_ST_REL_PEND,
	DIAG_SAPI3_ST_LINK_EST,
	DIAG_SAPI3_ST_TMR_RECOV,
	DIAG_SAPI3_ST_UA_PEND,
};
const struct value_string diag_gsm_l2_sapi3_st_vals[9];

enum diag_gsm_l2_event {
	DIAG_L2_EV_NO_EVENT,
	DIAG_L2_EV_CONNECT_RECEIVED,
	DIAG_L2_EV_ESTABLISH_REQUEST,
	DIAG_L2_EV_RELEASE_REQUEST,
	DIAG_L2_EV_SUSPEND_REQUEST,
	DIAG_L2_EV_RESUME_REQUEST,
	DIAG_L2_EV_RECONNECT_REQUEST,
	DIAG_L2_EV_DATA_REQUEST,
	DIAG_L2_EV_MDL_RELEASE_REQUEST,
	DIAG_L2_EV_UA_RECEIVED,
	DIAG_L2_EV_DM_RECEIVED,
	DIAG_L2_EV_DISC_RECEIVED,
	DIAG_L2_EV_SABM_RECEIVED,
	DIAG_L2_EV_I_RECEIVED,
	DIAG_L2_EV_UI_RECEIVED,
	DIAG_L2_EV_RR_RECEIVED,
	DIAG_L2_EV_REJ_RECEIVED,
	DIAG_L2_EV_T200_TIMEOUT,
	DIAG_L2_EV_CONTENTION_FAILED,
	DIAG_L2_EV_ABORT_ESTABLISHMENT,
	DIAG_L2_EV_LINK_ESTABLISHED,
	DIAG_L2_EV_RELEASE_CONFIRMED,
	DIAG_L2_EV_CLEAR_RECOVERY_CONDITION,
	DIAG_L2_EV_OPTIONAL_SEND,
	DIAG_L2_EV_RESET_L2,
	DIAG_L2_EV_UA_SENT,
	DIAG_L2_EV_FORCED_SUSPEND_REQ,
};
const struct value_string diag_gsm_l2_event_vals[28];

struct diag_gsm_l2_state {
	uint8_t sapi;
	uint8_t l2_state;
	uint8_t l2_event;
} __attribute__ ((packed));

enum diag_gsm_l2_chantype {
	DIAG_L2_CHT_DCCH	= 0,
	DIAG_L2_CHT_BCCH	= 1,
	DIAG_L2_CHT_L2_RACH	= 2,
	DIAG_L2_CHT_CCCH	= 3,
	DIAG_L2_CHT_SACCH	= 4,
	DIAG_L2_CHT_SDCCH	= 5,
	DIAG_L2_CHT_FACCH_F	= 6,
	DIAG_L2_CHT_FACCH_H	= 7,
};
const struct value_string diag_gsm_l2_chantype_vals[9];

struct diag_gsm_l2_transm_status {
	uint8_t sapi;
	uint8_t channel_type;
	uint8_t vs;
	uint8_t va;
	uint8_t vr;
	uint8_t retrans_ctr;
	uint8_t seq_err;
	uint8_t frame_type;
	uint8_t msg_entries;
	uint8_t seg_entries;
} __attribute__ ((packed));

struct msgb *diag_gsm_make_log_pack_req(uint16_t log_code, uint8_t zero_stats, uint8_t addl_info);
