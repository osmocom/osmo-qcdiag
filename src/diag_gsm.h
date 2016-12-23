#pragma once

#include <stdint.h>

enum diag_gsm_cmd {
	GSM_DIAG_VERNO_F		= 0,
	GSM_STATUS_F			= 1,
	GSM_TMC_STATE_RETRIEVAL_F	= 2,
	GSM_DIAG_CALL_ORIG_F		= 3,
	GSM_DIAG_CALL_END_F		= 4,
	GSM_GPRS_LOG_PACKET_REQ_F	= 5,
	DIAG_GSM_MAX_F			= 6,
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

struct msgb *diag_gsm_make_log_pack_req(uint16_t log_code, uint8_t zero_stats, uint8_t addl_info);
