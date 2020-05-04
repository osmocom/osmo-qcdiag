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


#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <osmocom/core/gsmtap.h>
#include <osmocom/core/gsmtap_util.h>

#include "protocol/protocol.h"
#include "diag_io.h"
#include "diag_cmd.h"
#include "diagchar_hdlc.h"

#include <termios.h>
#include <stdio.h>
#include <errno.h>

struct msgb *msgb_alloc_diag(void)
{
	return msgb_alloc_headroom(DIAG_MAX_REQ_SIZE+16, 16, "DIAG Tx");
}

/* transmit a msgb containing a DIAG message over the given fd */
int diag_transmit_msgb(struct diag_instance *di, struct msgb *msg)
{
	int out_len, rc;
	uint8_t packet[DIAG_MAX_HDLC_BUF_SIZE];
	struct diag_send_desc_type send;
	struct diag_hdlc_dest_type enc = { NULL, NULL, 0 };

	msg->l2h = msgb_data(msg);

	if (di->flags & DIAG_INST_F_GSMTAP_DIAG && di->gsmtap) {
		gsmtap_send_ex(di->gsmtap, GSMTAP_TYPE_QC_DIAG, 0, 0, 0,
				0, 0, 0, 0, msgb_l2(msg), msgb_l2len(msg));
	}

	if (di->flags & DIAG_INST_F_HEXDUMP)
		printf("Tx: %s\n", msgb_hexdump(msg));

	send.state = DIAG_STATE_START;
	send.pkt = msgb_data(msg);
	send.last = msgb_data(msg) + msgb_length(msg) - 1;
	send.terminate = 1;

	enc.dest = packet;
	enc.dest_last = packet + sizeof(packet) - 1;

	diag_hdlc_encode(&send, &enc);

	out_len = (enc.dest - (void *)packet);

	rc = write(di->fd, packet, out_len);
	if (rc != out_len) {
		fprintf(stderr, "Short write on packet.\n");
		return -1;
	}

	msgb_free(msg);

	return 0;
}

/* transmit a message from a buffer (nto msgb) as DIAG over the given fd */
int diag_transmit_buf(struct diag_instance *di, const uint8_t *data, size_t data_len)
{
	struct msgb *msg = msgb_alloc_diag();

	memcpy(msg->tail, data, data_len);
	msgb_put(msg, data_len);

	return diag_transmit_msgb(di, msg);
}

struct msgb *diag_read_msg(struct diag_instance *di)
{
	static uint8_t buf[DIAG_MAX_HDLC_BUF_SIZE];
	struct diag_hdlc_decode_type hdlc_decode;
	struct msgb *msg;
	int rc = 0;
	static int buf_items_left = 0;

//	printf("--\nRx dump pre: %s\n", osmo_hexdump(buf, buf_items_left));
	/* read raw data into buffer */

	if(!buf_items_left) {
		rc = read(di->fd, buf+buf_items_left, sizeof(buf)-buf_items_left);
		if (rc <= 0) {
			fprintf(stderr, "Short read! %d -> exiting...\n", rc);
			exit(1);
		}
	}

	unsigned int read_total = rc + buf_items_left;
	unsigned int read_current = rc;

//	printf("1 Rx dump total: %s\n", osmo_hexdump(buf, read_total));
	fflush(stdout);

	if (!di->rx.msg) {
		di->rx.msg = msgb_alloc(DIAG_MAX_REQ_SIZE, "DIAG Rx");
		di->rx.msg->l2h = di->rx.msg->tail;
	}
	msg = di->rx.msg;

	memset(&hdlc_decode, 0, sizeof(hdlc_decode));
	hdlc_decode.dest_ptr = msg->tail;
	hdlc_decode.dest_size = msgb_tailroom(msg);
	hdlc_decode.src_ptr = buf;
	hdlc_decode.src_size = read_total;
	hdlc_decode.src_idx = 0;
	hdlc_decode.dest_idx = 0;


	rc = diag_hdlc_decode(&hdlc_decode);


	int consumed = hdlc_decode.dest_idx;
	int current_remaining_in_buf = read_total-consumed;
//	printf("rx %d->%d, rem %d %u \n", read_total, consumed, current_remaining_in_buf, hdlc_decode.src_idx-hdlc_decode.dest_idx);

	if(hdlc_decode.dest_idx < read_total) {
		memmove(buf, buf+consumed, current_remaining_in_buf);
		buf_items_left = current_remaining_in_buf;
	} else
		buf_items_left = 0;

//	printf("2 Rx dump total: %s\n", osmo_hexdump(buf, buf_items_left));


	if (msgb_length(msg) + hdlc_decode.dest_idx > DIAG_MAX_REQ_SIZE) {
		fprintf(stderr, "Dropping packet. pkt_size: %d, max: %d\n",
			msgb_length(msg) + hdlc_decode.dest_idx,
			DIAG_MAX_REQ_SIZE);
		return NULL;
	}

	msgb_put(msg, hdlc_decode.dest_idx);

	if (rc == HDLC_COMPLETE) {
		di->rx.msg = NULL;

		if (msgb_length(msg) < 3) {
			msgb_free(msg);
			return NULL;
		}

		rc = crc_check(msgb_data(msg), msgb_length(msg));
		if (rc) {
			fprintf(stderr, "Bad CRC, dropping packet\n");
			printf("Rx broken crc: %s\n", msgb_hexdump(msg));
			msgb_free(msg);
			return NULL;
		}
		msgb_get(msg, HDLC_FOOTER_LEN);

		if (msgb_length(msg) < 1) {
			fprintf(stderr, "Message too short, len: %u\n", msgb_length(msg));
			msgb_free(msg);
			return NULL;
		}

		if (di->flags & DIAG_INST_F_HEXDUMP)
			printf("Rx: %s\n", msgb_hexdump(msg));

		if (di->flags & DIAG_INST_F_GSMTAP_DIAG && di->gsmtap) {
			gsmtap_send_ex(di->gsmtap, GSMTAP_TYPE_QC_DIAG,
					GSMTAP_ARFCN_F_UPLINK, 0, 0, 0,
					0, 0, 0, msgb_l2(msg),
					msgb_l2len(msg));
		}

		return msg;
	}

	return NULL;
};

/* transmit a message, wait for response, return response */
struct msgb *diag_transceive_msg(struct diag_instance *di, struct msgb *tx)
{
	struct msgb *rx;
	int rc;

	/* transmit the tx message */
	diag_transmit_msgb(di, tx);

	/* blocking loop and process incoming messages until there is
	 * one for which we don't have a parser registered, let's assume
	 * that this is our response */
	while (1) {
		rx = diag_read_msg(di);
		if (rx) {
			rc = diag_process_msg(di, rx);
			if (rc == 0)
				return rx;
		}
	}
	return NULL;
}

struct msgb *diag_subsys_transceive_msg(struct diag_instance *di, struct msgb *tx,
					uint8_t subsys, uint16_t code)
{
	struct msgb *rx;
	struct diagpkt_subsys_hdr *dsh;

	diag_push_subsys_hdr(tx, subsys, code);
	rx = diag_transceive_msg(di, tx);
	if (!rx)
		return NULL;
	dsh = (struct diagpkt_subsys_hdr *) rx->l2h;
	if (msgb_l2len(rx) < sizeof(*dsh) ||
	    dsh->subsys_id != subsys ||
	    osmo_load16le(&dsh->subsys_cmd_code) != code) {
		msgb_free(rx);
		return NULL;
	}
	rx->l3h = rx->l2h + sizeof(*dsh);
	return rx;
}

/* transmit a message, wait for response, then ignore response */
void diag_transceive_msg_ign(struct diag_instance *di, struct msgb *tx)
{
	struct msgb *rx;

	rx = diag_transceive_msg(di, tx);
	msgb_free(rx);
}

/* transmit a message from a buffer, wait for response, return it */
struct msgb *diag_transceive_buf(struct diag_instance *di, const uint8_t *data, size_t data_len)
{
	struct msgb *msg = msgb_alloc_diag();

	memcpy(msg->tail, data, data_len);
	msgb_put(msg, data_len);

	return diag_transceive_msg(di, msg);
}

/* transmit a message from a buffer, wait for response, ignore it */
void diag_transceive_buf_ign(struct diag_instance *di, const uint8_t *data, size_t data_len)
{
	struct msgb *rx = diag_transceive_buf(di, data, data_len);
	msgb_free(rx);
}
