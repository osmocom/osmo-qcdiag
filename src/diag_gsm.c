/* Utlity code for GSM related logging */
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

const struct value_string diag_gsm_l2_sapi0_st_vals[] = {
	{ DIAG_SAPI0_ST_NULL,		"NULL" },
	{ DIAG_SAPI0_ST_CON_PEND,	"CONN_PEND" },
	{ DIAG_SAPI0_ST_IDLE,		"IDLE" },
	{ DIAG_SAPI0_ST_EST_PEND,	"EST_PEND" },
	{ DIAG_SAPI0_ST_REL_PEND,	"REL_PEND" },
	{ DIAG_SAPI0_ST_LINK_EST,	"LINK_ESTABLISHED" },
	{ DIAG_SAPI0_ST_TMR_RECOV,	"TIMER_RECOVERY" },
	{ DIAG_SAPI0_ST_LINK_SUSP,	"LINK_SUSPENDED" },
	{ DIAG_SAPI0_ST_UA_PEND,	"UA_PENDING" },
	{ 0, NULL }
};

const struct value_string diag_gsm_l2_sapi3_st_vals[] = {
	{ DIAG_SAPI3_ST_NULL,		"NULL" },
	{ DIAG_SAPI3_ST_CON_PEND,	"CONN_PEND" },
	{ DIAG_SAPI3_ST_IDLE,		"IDLE" },
	{ DIAG_SAPI3_ST_EST_PEND,	"EST_PEND" },
	{ DIAG_SAPI3_ST_REL_PEND,	"REL_PEND" },
	{ DIAG_SAPI3_ST_LINK_EST,	"LINK_ESTABLISHED" },
	{ DIAG_SAPI3_ST_TMR_RECOV,	"TIMER_RECOVERY" },
	{ DIAG_SAPI3_ST_UA_PEND,	"UA_PENDING" },
	{ 0, NULL }
};

const struct value_string diag_gsm_l2_event_vals[] = {
	{ DIAG_L2_EV_NO_EVENT,		"NONE" },
	{ DIAG_L2_EV_CONNECT_RECEIVED,	"CON-RX" },
	{ DIAG_L2_EV_ESTABLISH_REQUEST,	"EST.req" },
	{ DIAG_L2_EV_RELEASE_REQUEST,	"REL.req" },
	{ DIAG_L2_EV_SUSPEND_REQUEST,	"SUSP.req" },
	{ DIAG_L2_EV_RESUME_REQUEST,	"RESUME.req" },
	{ DIAG_L2_EV_RECONNECT_REQUEST,	"RECONN.req" },
	{ DIAG_L2_EV_DATA_REQUEST,	"DATA.req" },
	{ DIAG_L2_EV_MDL_RELEASE_REQUEST, "MDL-REL.req" },
	{ DIAG_L2_EV_UA_RECEIVED,	"UA-RX" },
	{ DIAG_L2_EV_DM_RECEIVED,	"DM-RX" },
	{ DIAG_L2_EV_DISC_RECEIVED,	"DISC-RX" },
	{ DIAG_L2_EV_SABM_RECEIVED,	"SABM-RX" },
	{ DIAG_L2_EV_I_RECEIVED,	"I-RX" },
	{ DIAG_L2_EV_UI_RECEIVED,	"UI-RX" },
	{ DIAG_L2_EV_RR_RECEIVED,	"RR-RX" },
	{ DIAG_L2_EV_REJ_RECEIVED,	"REJ-RX" },
	{ DIAG_L2_EV_T200_TIMEOUT,	"T200-EXP" },
	{ DIAG_L2_EV_CONTENTION_FAILED,	"CONTENT-FAIL" },
	{ DIAG_L2_EV_ABORT_ESTABLISHMENT, "ABORT-EST" },
	{ DIAG_L2_EV_LINK_ESTABLISHED,	"LINK-ESTABLISHED" },
	{ DIAG_L2_EV_RELEASE_CONFIRMED,	"REL.conf" },
	{ DIAG_L2_EV_CLEAR_RECOVERY_CONDITION, "CLEAR-RECOV-COND" },
	{ DIAG_L2_EV_OPTIONAL_SEND,	"OPT-SEND" },
	{ DIAG_L2_EV_RESET_L2,		"RESET-L2" },
	{ DIAG_L2_EV_UA_SENT,		"UA-SENT" },
	{ DIAG_L2_EV_FORCED_SUSPEND_REQ,"FORCED-SUSP.req" },
	{ 0, NULL }
};

const struct value_string diag_gsm_l2_chantype_vals[] = {
	{ DIAG_L2_CHT_DCCH,	"DCCH" },
	{ DIAG_L2_CHT_BCCH,	"BCCH" },
	{ DIAG_L2_CHT_L2_RACH,	"RACH" },
	{ DIAG_L2_CHT_CCCH,	"CCCH" },
	{ DIAG_L2_CHT_SACCH,	"SACCH" },
	{ DIAG_L2_CHT_SDCCH,	"SDCCH" },
	{ DIAG_L2_CHT_FACCH_F,	"FACCH/F" },
	{ DIAG_L2_CHT_FACCH_H,	"FACCH/H" },
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
