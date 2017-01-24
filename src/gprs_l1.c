/*
 * (C) 2013-2017 by Harald Welte <laforge@gnumonks.org>
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

#include "protocol/diag_log_gprs_l1.h"

const struct value_string gprs_tx_ul_chans[4] = {
	{ DIAG_TX_UL_CH_T_PRACH,	"PRACK" },
	{ DIAG_TX_UL_CH_T_UL_PTCCH,	"UL-PTCCH" },
	{ DIAG_TX_UL_CH_T_UL_PACCH_PDTCH, "UL-PACCH-PDTCH" },
	{ 0, NULL }
};

const struct value_string gprs_coding_schemes[16] = {
	{ DIAG_GPRS_CS_CS1,	"CS1" },
	{ DIAG_GPRS_CS_CS2,	"CS2" },
	{ DIAG_GPRS_CS_CS3,	"CS3" },
	{ DIAG_GPRS_CS_CS4,	"CS4" },
	{ DIAG_GPRS_CS_ACCESS,	"ACCESS" },
	{ DIAG_GPRS_CS_EXT_ACCESS, "ACCESS-EXT" },
	{ DIAG_GPRS_CS_MCS1,	"MCS1" },
	{ DIAG_GPRS_CS_MCS2,	"MCS2" },
	{ DIAG_GPRS_CS_MCS3,	"MCS3" },
	{ DIAG_GPRS_CS_MCS4,	"MCS4" },
	{ DIAG_GPRS_CS_MCS5,	"MCS5" },
	{ DIAG_GPRS_CS_MCS6,	"MCS6" },
	{ DIAG_GPRS_CS_MCS7,	"MCS7" },
	{ DIAG_GPRS_CS_MCS8,	"MCS8" },
	{ DIAG_GPRS_CS_MCS9,	"MCS9" },
	{ 0, NULL }
};
