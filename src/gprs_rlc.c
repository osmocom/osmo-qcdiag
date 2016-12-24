#include <osmocom/core/utils.h>

#include "protocol/diag_log_gprs_rlc.h"

struct value_string gprs_rlc_ul_state_vals[8] = {
	{ US_NULL,		"US_NULL" },
	{ US_AWAIT_ASS,		"US_AWAIT_ASS" },
	{ US_ACK_XFER,		"US_AWAIT_XFER" },
	{ US_GTTP_XFER,		"US_GTTP_XFER" },
	{ US_UNACK_XFER,	"US_UNACK_XFER" },
	{ US_TEST_MODE,		"US_TEST_MODE" },
	{ US_AWAIT_RECOVERY,	"US_AWAIT_RECOVERY" },
	{ 0, NULL }
};

struct value_string gprs_rlc_ul_substate_vals[5] = {
	{ RALS_NULL,		"RALS_NULL" },
	{ RALS_AWAIT_TX_COMPLETE, "RALS_AWAIT_TX_COMPLETE" },
	{ RALS_CURRENT_PDU_TX_COMPLETED, "RALS_CURRENT_PDU_TX_COMPLETED" },
	{ RALS_COMPLETE,	"RALS_COMPLETE" },
	{ 0, NULL }
};

struct value_string gprs_rlc_dl_state_vals[7] = {
	{ DS_NULL,		"DS_NULL" },
	{ DS_RX_ACK,		"DS_RX_ACK" },
	{ DS_RX_UNACK,		"DS_RX_UNACK" },
	{ DS_RX_ACK_PENDING_RELEASE, "DS_RX_ACK_PENDING_RELEASE" },
	{ DS_RX_UNACK_PENDING_RELEASE, "DS_RX_UNACK_PENDING_RELEASE" },
	{ DS_TEST_MODE,		"DS_TEST_MODE" },
	{ 0, NULL }
};

struct value_string gprs_rlc_hdr_type_vals[] = {
	{ GPRS_HEADER_TYPE,	"GPRS" },
	{ EGPRS_HEADER_TYPE_1,	"EGPRS T1" },
	{ EGPRS_HEADER_TYPE_2,	"EGPRS T2" },
	{ EGPRS_HEADER_TYPE_3,	"EGPRS T3" },
	{ 0, NULL }
};
