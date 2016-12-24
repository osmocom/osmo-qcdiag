#include <osmocom/core/utils.h>
#include <osmocom/core/msgb.h>

#include "protocol/diagcmd.h"
#include "protocol/diag_log_gsm.h"
#include "protocol/protocol.h"

const struct value_string diag_gsm_rr_st_vals[] = {
	{ DIAG_RR_ST_INACTIVE,		"INACTIVE" },
	{ DIAG_RR_ST_GOING_ACTIVE,	"GOING_ACTIVE" },
	{ DIAG_RR_ST_GOING_INACTIVE,	"GOING_INACTIVE" },
	{ DIAG_RR_ST_CELL_SELECTION,	"CELL_SELECTION" },
	{ DIAG_RR_ST_PLMN_LIST_CONSTRUCTION, "PLMN_LIST_CONSTR" },
	{ DIAG_RR_ST_IDLE,		"IDLE" },
	{ DIAG_RR_ST_CELL_RESELECTION,	"CELL_RESELECTION" },
	{ DIAG_RR_ST_CONNECTION_PENDING, "CONNECTION_PENDING" },
	{ DIAG_RR_ST_CHOOSE_CELL,	"CHOOSE_CELL" },
	{ DIAG_RR_ST_DATA_TRANSFER,	"DATA_TRANSFER" },
	{ DIAG_RR_ST_NO_CHANNELS,	"NO_CHANNELS" },
	{ DIAG_RR_ST_CONNECTION_RELEASE, "CONNECTION_RELEASE" },
	{ DIAG_RR_ST_EARLY_CAMPED_WAIT_FOR_SI, "EARLY_CAMPED_WAIT_SI" },
	{ DIAG_RR_ST_W2G_INTERRAT_HO_PROGRESS, "IRAT_W2G_HO_PROGRESS" },
	{ DIAG_RR_ST_W2G_INTERRAT_RESELECTION_PROGRESS, "IRAT_W2G_RESEL_PROGRESS" },
	{ DIAG_RR_ST_W2G_INTERRAT_CC_ORDER_PROGRESS, "IRAT_W2G_CC_ORDER_PROGRESS" },
	{ DIAG_RR_ST_G2W_INTERRAT_RESELECTION_PROGRESS, "IRAT_G2W_RESEL_PROGRESS" },
	{ DIAG_RR_ST_WAIT_FOR_EARLY_PSCAN,	"WAIT_FOR_EARLY_PSCAN" },
	{ DIAG_RR_ST_GRR,		"GRR" },
	{ DIAG_RR_ST_G2W_INTERRAT_HO_PROGRESS, "IRAT_G2W_HO_PROGRESS" },
	{ DIAG_RR_ST_BACKGROUND_PLMN_SEARCH, "BGROUND_PLMN_SEARCH" },
	{ DIAG_RR_ST_W2G_SERVICE_REDIR_IN_PROGRESS, "W2G_SERVICE_REDIR_PROGRESS" },
	{ DIAG_RR_ST_RESET,		"RESET" },
	{ DIAG_RR_ST_W2G_BACKGROUND_PLN_SEARCH, "W2G_BGROUND_PLMN_SEARCH" },
	{ 0, NULL }
};

const struct value_string diag_gprs_grr_st_vals[] = {
	{ DIAG_GRR_ST_NULL,		"NULL" },
	{ DIAG_GRR_ST_ACQUIRE,		"ACQUIRE" },
	{ DIAG_GRR_ST_CAMPED,		"CAMPED" },
	{ DIAG_GRR_ST_CONN_PEND,	"CONN_PEND" },
	{ DIAG_GRR_ST_CELL_RESEL,	"CELL_RESEL" },
	{ DIAG_GRR_ST_CELL_RESEL_G2W,	"CELL_RESEL_G2W" },
	{ DIAG_GRR_ST_BG_PLMN_SEARCH,	"BG_PLMNN_SEARCH" },
	{ DIAG_GRR_ST_PCCO,		"PCCO" },
	{ 0, NULL }
};

const struct value_string diag_gprs_llme_st_vals[] = {
	{ DIAG_GPRS_LLMES_TLLI_UNASS,	"TLLI_UNASSIGNED" },
	{ DIAG_GPRS_LLMES_TLLI_ASS,	"TLLI_ASSIGNED" },
	{ DIAG_GPRS_LLMES_TEST_MODE,	"TEST_MODE" },
	{ 0, NULL }
};



/* GSM_GPRS_LOG_PACKET_REQ_F */
struct diag_gsm_log_packet_req {
	uint16_t code;
	uint8_t zero_stats;
	uint8_t addl_info;
} __attribute__((packed));

struct msgb *diag_gsm_make_log_pack_req(uint16_t log_code, uint8_t zero_stats, uint8_t addl_info)
{
	struct msgb *msg = msgb_alloc_headroom(1024, 128, "GSM Log Packet Req");
	struct diag_gsm_log_packet_req *glpr;

	glpr = (struct diag_gsm_log_packet_req *) msgb_put(msg, sizeof(*glpr));
	glpr->code = log_code;
	glpr->zero_stats = zero_stats;
	glpr->addl_info = addl_info;
	diag_push_subsys_hdr(msg, DIAG_SUBSYS_GSM, GSM_GPRS_LOG_PACKET_REQ_F);

	return msg;
}
