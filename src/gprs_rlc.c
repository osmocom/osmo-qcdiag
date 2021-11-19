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

#include "protocol/diag_log_gprs_rlc.h"

struct value_string gprs_rlc_ul_state_vals[] = {
	{ US_NULL,		"US_NULL" },
	{ US_AWAIT_ASS,		"US_AWAIT_ASS" },
	{ US_ACK_XFER,		"US_AWAIT_XFER" },
	{ US_GTTP_XFER,		"US_GTTP_XFER" },
	{ US_UNACK_XFER,	"US_UNACK_XFER" },
	{ US_TEST_MODE,		"US_TEST_MODE" },
	{ US_AWAIT_RECOVERY,	"US_AWAIT_RECOVERY" },
	{ 0, NULL }
};

struct value_string gprs_rlc_ul_substate_vals[] = {
	{ RALS_NULL,		"RALS_NULL" },
	{ RALS_AWAIT_TX_COMPLETE, "RALS_AWAIT_TX_COMPLETE" },
	{ RALS_CURRENT_PDU_TX_COMPLETED, "RALS_CURRENT_PDU_TX_COMPLETED" },
	{ RALS_COMPLETE,	"RALS_COMPLETE" },
	{ 0, NULL }
};

struct value_string gprs_rlc_dl_state_vals[] = {
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
