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

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <osmocom/core/msgb.h>

#include "diag_cmd.h"
#include "protocol/protocol.h"
#include "protocol/diagcmd.h"

/***********************************************************************
 * EVENT Configuration / Protocol
 ***********************************************************************/

struct diag_event_mask_set_req {
	uint8_t cmd_code;
	uint8_t pad[3];
	uint8_t mask[0];
} __attribute__((packed));

struct diag_event_cfg_req {
	uint8_t cmd_code;
	uint8_t enable;
} __attribute__((packed));

static struct msgb *gen_event_set_mask(uint32_t last_item)
{
	struct msgb *msg = msgb_alloc(DIAG_MAX_REQ_SIZE, "Diag Event Cfg");
	struct diag_event_mask_set_req *demsr;

	msg->l2h = msgb_put(msg, sizeof(*demsr));
	demsr = (struct diag_event_mask_set_req *) msg->l2h;
	demsr->cmd_code = DIAG_EVENT_MASK_SET_F;
	msg->l3h = msgb_put(msg, bytes_rqd_for_bit(last_item));

	return msg;
}

int diag_event_set_bit(struct msgb *msg, uint32_t bit_in)
{
	struct diag_event_mask_set_req *demsr;
	demsr = (struct diag_event_mask_set_req *) msg->l2h;
	uint8_t *mask = demsr->mask;
	unsigned int byte = bit_in / 8;
	unsigned int bit = bit_in % 8;

	if (mask + byte > msg->tail) {
		fprintf(stderr, "bit %u is outside event mask!\n", bit_in);
		return -1;
	}

	mask[byte] |= (1 << bit);

	return 0;
}

struct diag_event_id {
	uint16_t id:12,
		 _pad:1,
		 payload_len:2,		/* 0x2000, 0x4000 */
		 ts_trunc:1;		/* 0x8000 */
} __attribute__ ((packed));

struct diag_event_report_event {
	struct diag_event_id event_id;
	uint64_t timestamp;
	uint8_t payload[0];
} __attribute__ ((packed));

struct diag_event_report_event_trunc {
	struct diag_event_id event_id;
	uint16_t timestamp_trunc;
	uint8_t payload[0];
} __attribute__ ((packed));

struct diag_event_report {
	uint8_t cmd_code;
	uint16_t len;
	struct diag_event_report_event events[0];
} __attribute__ ((packed));

static void diag_rx_event_report_f(struct diag_instance *di, struct msgb *msg)
{
	struct diag_event_report *erep = (struct diag_event_report *) msgb_l2(msg);
	uint16_t erep_len = osmo_load16le(&erep->len);
	uint8_t *cur;

	//printf("EVENT (erep_len=%u): %s\n", erep_len, msgb_hexdump(msg));

	for (cur = (uint8_t *)erep->events; cur < (uint8_t *)erep->events + erep_len;) {
		uint16_t _eid = osmo_load16le(cur);
		struct diag_event_id *event_id = (struct diag_event_id *)&_eid;

		if (event_id->ts_trunc) {
			struct diag_event_report_event_trunc *ret;
			ret = (struct diag_event_report_event_trunc *) cur;
			cur += sizeof(*ret);
			printf("EVENT-TRUNC(0x%04x|FIXME)", event_id->id);
		} else {
			struct diag_event_report_event *re;
			re = (struct diag_event_report_event *) cur;
			cur += sizeof(*re);
			uint64_t ts = osmo_load64le(&re->timestamp);

			printf("EVENT(0x%04x|%u|%u)", event_id->id,
				diag_ts_to_epoch(ts), diag_ts_to_fn(ts));
		}
		switch (event_id->payload_len) {
		case 0:
			printf("\n");
			break;
		case 1:
			printf(": 0x%02x\n", cur[0]);
			cur += 1;
			break;
		case 2:
			printf(": 0x%02x, 0x%02x\n", cur[0], cur[1]);
			cur += 2;
			break;
		case 3:
			printf(": %s\n", osmo_hexdump(cur+1, *cur));
			cur += *cur + 1;
			break;
		}
	}

#if 0
	for (i = 0; (i+1)*sizeof(struct diag_event_report_event) <= erep_len; i++) {
		struct diag_event_report_event *evt = &erep->events[i];
		uint16_t event_id = osmo_load16le(&evt->event_id);
	}
#endif
}

static const struct diag_cmd_dispatch_tbl cmd_tbl[] = {
	{ DIAG_EVENT_REPORT_F, diag_rx_event_report_f },
};
#if 0
static __attribute__((constructor)) void on_dso_load_event(void)
{
	diag_cmd_reg_dispatch(cmd_tbl, ARRAY_SIZE(cmd_tbl));
}
#endif
