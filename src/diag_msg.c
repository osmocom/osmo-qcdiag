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
#include "diag_cmd.h"
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
	dsrmr = (struct diag_set_rt_mask_req *) msgb_l2(msg);
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
	struct msgb *rx;
	struct diag_set_rt_mask_req *res;
	int rc = 0;

	rx = diag_transceive_msg(di, msg);
	res = (struct diag_set_rt_mask_req *) (msgb_l2(msg)+1);
	if ((rx->l2h[0] != DIAG_EXT_MSG_CONFIG_F) || res->cmd_code != MSG_EXT_SUBCMD_SET_RT_MASK ||
	    res->sub_cmd != MSG_EXT_SUBCMD_SET_RT_MASK ||
	    osmo_load16le(&res->ssid_start) != ssid ||
	    osmo_load16le(&res->ssid_end) != ssid ||
	    osmo_load32le(&res->runtime_mask) != runtime_mask) {
		fprintf(stderr, "Error setting RT mask\n");
		rc = -1;
	}
	msgb_free(rx);

	return rc;
}

/* handler for EXT MSG */
static void diag_rx_ext_msg_f(struct diag_instance *di, struct msgb *msgb)
{
	uint8_t *data = msgb_data(msgb);
	const size_t len = msgb_length(msgb);
	struct ext_log_msg *msg;
	char *file = NULL, *fmt;
	unsigned int num_args;

	if (len < sizeof(struct ext_log_msg)) {
		printf("too short ext_log_msg.\n");
		return;
	}

	msg = (struct ext_log_msg *) data;
	num_args = msg->hdr.num_args;
	fmt = (char *) msg->params + num_args*sizeof(msg->params[0]);
	file = fmt + strlen(fmt) + 1;

	printf("MSG(%u|%u|%s:%u): ", osmo_load16le(&msg->subsys_id),
		diag_ts_to_epoch(osmo_load64le(&msg->hdr.timestamp)),
		file, osmo_load16le(&msg->line_nr));

	/* replace all '%s' with '%p', as %s obviously doesn't work */
	for (char *cur = fmt; cur && (cur < fmt + strlen(fmt)); cur = strstr(fmt, "%s")) {
		cur[1] = 'p';
	}

	switch (num_args) {
	case 0:
		fputs(fmt, stdout);
		break;
	case 1:
		printf(fmt, osmo_load32le(&msg->params[0]));
		break;
	case 2:
		printf(fmt, osmo_load32le(&msg->params[0]),
			    osmo_load32le(&msg->params[1]));
		break;
	case 3:
		printf(fmt, osmo_load32le(&msg->params[0]),
			    osmo_load32le(&msg->params[1]),
			    osmo_load32le(&msg->params[2]));
		break;
	case 4:
		printf(fmt, osmo_load32le(&msg->params[0]),
			    osmo_load32le(&msg->params[1]),
			    osmo_load32le(&msg->params[2]),
			    osmo_load32le(&msg->params[3]));
		break;
	}
	fputc('\n', stdout);
}

static void diag_rx_ext_msg_terse_f(struct diag_instance *di, struct msgb *msgb)
{
	const uint8_t *data = msgb_data(msgb);
	const size_t len = msgb_length(msgb);
	const struct qsr_ext_msg_terse *msg;
	unsigned int num_args;

	if (len < sizeof(struct qsr_ext_msg_terse)) {
		printf("too short ext_log_msg.\n");
		return;
	}

	msg = (struct qsr_ext_msg_terse *) data;
	num_args = msg->hdr.num_args;

	printf("MSG_QS(%u|%u|%08x:%u): ", osmo_load16le(&msg->subsys_id),
		diag_ts_to_epoch(osmo_load64le(&msg->hdr.timestamp)),
		osmo_load32le(&msg->hash), osmo_load16le(&msg->line_nr));
	switch (num_args) {
	case 0:
		fputs("", stdout);
		break;
	case 1:
		printf("%08x", osmo_load32le(&msg->params[0]));
		break;
	case 2:
		printf("%08x %08x", osmo_load32le(&msg->params[0]),
			    osmo_load32le(&msg->params[1]));
		break;
	case 3:
		printf("%08x %08x %08x", osmo_load32le(&msg->params[0]),
			    osmo_load32le(&msg->params[1]),
			    osmo_load32le(&msg->params[2]));
		break;
	case 4:
		printf("%08x %08x %08x %08x", osmo_load32le(&msg->params[0]),
			    osmo_load32le(&msg->params[1]),
			    osmo_load32le(&msg->params[2]),
			    osmo_load32le(&msg->params[3]));
		break;
	}
	fputc('\n', stdout);
}


struct diag_cmd_dispatch_tbl cmd_tbl[] = {
	{ DIAG_EXT_MSG_F, diag_rx_ext_msg_f },
	{ DIAG_QSR_EXT_MSG_TERSE_F, diag_rx_ext_msg_terse_f },
};

static __attribute__((constructor)) void on_dso_load_msg(void)
{
	diag_cmd_reg_dispatch(cmd_tbl, ARRAY_SIZE(cmd_tbl));
}
