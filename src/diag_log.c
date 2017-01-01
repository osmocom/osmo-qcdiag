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

#include "diag_log.h"
#include "protocol/protocol.h"
#include "protocol/diagcmd.h"

/***********************************************************************
 * LOG Configuration / Protocol
 ***********************************************************************/

enum log_config_op {
	LOG_CONFIG_DISABLE_OP			= 0,
	LOG_CONFIG_RETRIEVE_ID_RANGES_OP	= 1,
	LOG_CONFIG_RETRIEVE_VALID_MASK_OP	= 2,
	LOG_CONFIG_SET_MASK_OP			= 3,
	LOG_CONFIG_GET_LOGMASK_OP		= 4,
};

struct diag_log_config_req_hdr {
	uint8_t		msg_type;
	uint8_t		pad[3];
	uint32_t	operation;
} __attribute((packed));

struct diag_log_config_set_mask {
	struct diag_log_config_req_hdr hdr;
	uint32_t	equip_id;
	uint32_t	last_item;
	uint8_t		data[0];
} __attribute((packed));

struct msgb *gen_log_config_set_mask(uint32_t equip_id, uint32_t last_item)
{
	struct msgb *msg = msgb_alloc(DIAG_MAX_REQ_SIZE, "Diag Tx");
	struct diag_log_config_set_mask *dlcsm;

	msg->l2h = msgb_put(msg, sizeof(*dlcsm));
	dlcsm = (struct diag_log_config_set_mask *) msg->l2h;
	dlcsm->hdr.msg_type = DIAG_LOG_CONFIG_F;
	dlcsm->hdr.operation = LOG_CONFIG_SET_MASK_OP;
	dlcsm->equip_id = equip_id;
	dlcsm->last_item = last_item;
	msg->l3h = msgb_put(msg, dlcsm->last_item/8);

	return msg;
}

int log_config_set_mask_bit(struct msgb *msg, uint32_t bit_in)
{
	struct diag_log_config_set_mask *dlcsm;
	dlcsm = (struct diag_log_config_set_mask *) msg->l2h;
	uint8_t *mask = msg->l3h;
	unsigned int byte = bit_in / 8;
	unsigned int bit = bit_in % 8;

	if (byte > dlcsm->last_item/8)
		return -1;

	mask[byte] |= (1 << bit);

	return 0;
}


/***********************************************************************
 * LOG Dispatch
 ***********************************************************************/

/* not particularly memory efficient, but welll, only 500kB on 64bit */
static diag_log_handler *log_handlers[0xffff];

/* called by individual modules to register their own decoders */
void diag_log_reg_dispatch(const struct diag_log_dispatch_tbl *tbl, unsigned int size)
{
	unsigned int i;
	for (i = 0; i < size; i++) {
		printf("Registering dispatch for 0x%04x\n", tbl[i].code);
		log_handlers[tbl[i].code] = tbl[i].handler;
	}
}

void diag_log_enable_all_supported_family(struct diag_instance *di, uint8_t family)
{
	struct msgb *msg, *rx;
	unsigned int i, size;
	unsigned int family_base = (family & 0xf) << 12;
	unsigned int max = 0;

	for (i = family_base; i < family_base + 0x1000; i++) {
		if (log_handlers[i]) {
			if (max < i)
				max = i;
		}
	}

	if (!max)
		return;

	size = max - family_base;
	printf("family %u: allocating log mask of size %u\n", family, size);
	msg = gen_log_config_set_mask(family, size);
	for (i = family_base; i < family_base + 0x1000; i++) {
		if (log_handlers[i])
			log_config_set_mask_bit(msg, i-family_base);
	}

	rx = diag_transceive_msg(di, msg);
	/* FIXME */
	msgb_free(rx);
}

void diag_log_enable_all_supported(struct diag_instance *di)
{
	unsigned int i;

	for (i = 0; i < 0xF; i++) {
		diag_log_enable_all_supported_family(di, i);
	}
}

void diag_log_handle(struct diag_instance *di, struct msgb *msg)
{
	struct diag_log_hdr *dlh;
	struct log_hdr *lh;

	dlh = (struct diag_log_hdr *) msg->data;
	/* FIXME: verify length */
	msg->l3h = msgb_pull(msg, sizeof(*dlh));

	lh = (struct log_hdr *) msg->l3h;
	/* FIXME: verify length */
	msgb_pull(msg, sizeof(*lh));

	printf("LOG(0x%04x|%u|%u): ", lh->code,
		diag_ts_to_epoch(lh->ts), diag_ts_to_fn(lh->ts));

	if (log_handlers[lh->code])
		log_handlers[lh->code](lh, msg);
	else
		printf("%s\n", osmo_hexdump(lh->data, lh->len));
}
