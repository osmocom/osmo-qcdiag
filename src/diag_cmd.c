/*
 * (C) 2013-2016 by Harald Welte <laforge@gnumonks.org>
 *
 * originally based on earlier code from Dieter Spaar and Holger
 * Freyther, though by now almost entirely rewritten.  Nevertheless,
 * thanks to Dieter and Holger!
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

#include <osmocom/core/gsmtap.h>
#include <osmocom/core/gsmtap_util.h>

#include "protocol/protocol.h"
#include "protocol/diagcmd.h"
#include "diag_cmd.h"
#include "diag_log.h"
#include "diag_msg.h"


/***********************************************************************
 * CMD Dispatch
 ***********************************************************************/

static diag_cmd_handler *cmd_handlers[0xff];

/* called by individual modules to register their own decoders */
void diag_cmd_reg_dispatch(const struct diag_cmd_dispatch_tbl *tbl, unsigned int size)
{
	unsigned int i;
	for (i = 0; i < size; i++) {
		printf("Registering CMD dispatch for 0x%02x\n", tbl[i].code);
		cmd_handlers[tbl[i].code] = tbl[i].handler;
	}
}

int diag_process_msg(struct diag_instance *di, struct msgb *msg)
{
	uint8_t cmd = msg->l2h[0];

	if (di->gsmtap) {
		gsmtap_send_ex(di->gsmtap, GSMTAP_TYPE_QC_DIAG, 0, 0, 0,
				0, 0, 0, 0, msgb_l2(msg), msgb_l2len(msg));
	}

	switch (cmd) {
	case DIAG_LOG_F:
		diag_log_handle(di, msg);
		msgb_free(msg);
		return 1;
	case DIAG_BAD_CMD_F:
		fprintf(stderr, "BAD COMMAND: %s\n", msgb_hexdump(msg));
		return 0;
	case DIAG_BAD_PARM_F:
		fprintf(stderr, "BAD PARAMETER: %s\n", msgb_hexdump(msg));
		return 0;
	case DIAG_BAD_LEN_F:
		fprintf(stderr, "BAD LENGTH: %s\n", msgb_hexdump(msg));
		return 0;
	default:
		if (cmd_handlers[cmd]) {
			(cmd_handlers[cmd])(di, msg);
			msgb_free(msg);
			return 1;
		} else {
			printf("Got %d bytes data of unknown payload type 0x%02x: %s\n",
				msgb_length(msg), msg->l2h[0],
				osmo_hexdump(msgb_data(msg), msgb_length(msg)));
			return 0;
		}
		break;
	}
}
