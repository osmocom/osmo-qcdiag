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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <osmocom/core/msgb.h>
#include <osmocom/core/bit16gen.h>
#include <osmocom/core/bit32gen.h>

#include "protocol/protocol.h"
#include "diag_msg.h"
#include "protocol/diagcmd.h"

struct diag_set_rt_mask_req {
	uint8_t cmd_code;
	uint8_t sub_cmd;
	uint16_t ssid_start;
	uint16_t ssid_end;
	uint16_t _pad;
	uint32_t runtime_mask[1];
};

#define MSG_EXT_SUBCMD_SET_RT_MASK	4

struct msgb *gen_msg_config_set_rt_mask(uint16_t ssid, uint32_t runtime_mask)
{
	struct msgb *msg = msgb_alloc(DIAG_MAX_REQ_SIZE, "Diag Msg Config");
	struct diag_set_rt_mask_req *dsrmr;

	msg->l2h = msgb_put(msg, sizeof(*dsrmr));
	dsrmr = (struct diag_set_rt_mask_req *) msg->l2h;
	dsrmr->cmd_code = DIAG_EXT_MSG_CONFIG_F;
	dsrmr->sub_cmd = MSG_EXT_SUBCMD_SET_RT_MASK;
	osmo_store16le(ssid, &dsrmr->ssid_start);
	osmo_store16le(ssid, &dsrmr->ssid_end);
	osmo_store32le(runtime_mask, &dsrmr->runtime_mask[0]);

	return msg;
}

int diag_msg_config_set_rt_mask(struct diag_instance *di, uint16_t ssid, uint32_t runtime_mask)
{
	struct msgb *msg = gen_msg_config_set_rt_mask(ssid, runtime_mask);
	diag_transmit_msgb(di, msg);
	diag_read(di);

	return 0;
}

/* handler for EXT MSG */
int diag_rx_ext_msg_f(struct diag_instance *di, struct msgb *msgb)
{
	const uint8_t *data = msgb_data(msgb);
	const size_t len = msgb_length(msgb);
	const struct ext_log_msg *msg;
	const char *file = NULL, *fmt;
	unsigned int num_args;

	if (len < sizeof(struct ext_log_msg)) {
		printf("too short ext_log_msg.\n");
		return -1;
	}

	msg = (struct ext_log_msg *) data;
	num_args = msg->num_args;
	fmt = (const char *) msg->params + num_args*sizeof(msg->params[0]);
	file = fmt + strlen(fmt) + 1;

	printf("MSG(%u|%s:%u): ", diag_ts_to_epoch(msg->timestamp), file, msg->line_nr);
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


