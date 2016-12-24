
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "protocol.h"
#include "diag_io.h"
#include "diagchar_hdlc.h"

/* transmit a msgb containing a DIAG message over the given fd */
int diag_transmit_msgb(struct diag_instance *di, struct msgb *msg)
{
	int out_len, rc;
	uint8_t packet[DIAG_MAX_HDLC_BUF_SIZE];
	struct diag_send_desc_type send;
	struct diag_hdlc_dest_type enc = { NULL, NULL, 0 };

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
	struct msgb *msg = msgb_alloc(DIAG_MAX_REQ_SIZE, "DIAG Tx");

	memcpy(msg->tail, data, data_len);
	msgb_put(msg, data_len);

	return diag_transmit_msgb(di, msg);
}

int diag_read(struct diag_instance *di)
{
	uint8_t buf[DIAG_MAX_HDLC_BUF_SIZE];
	struct diag_hdlc_decode_type hdlc_decode;
	struct msgb *msg;
	int rc;

	/* read raw data into buffer */
	rc = read(di->fd, buf, sizeof(buf));
	if (rc <= 0 ) {
		fprintf(stderr, "Short read!\n");
		return -EIO;
	}

	if (!di->rx.msg) {
		di->rx.msg = msgb_alloc(DIAG_MAX_REQ_SIZE, "DIAG Rx");
		di->rx.msg->l2h = di->rx.msg->tail;
	}
	msg = di->rx.msg;

	hdlc_decode.dest_ptr = msg->tail;
	hdlc_decode.dest_size = msgb_tailroom(msg);
	hdlc_decode.src_ptr = buf;
	hdlc_decode.src_size = rc;
	hdlc_decode.src_idx = 0;
	hdlc_decode.dest_idx = 0;

	rc = diag_hdlc_decode(&hdlc_decode);

	if (msgb_length(msg) + hdlc_decode.dest_idx > DIAG_MAX_REQ_SIZE) {
		fprintf(stderr, "Dropping packet. pkt_size: %d, max: %d\n",
			msgb_length(msg) + hdlc_decode.dest_idx,
			DIAG_MAX_REQ_SIZE);
		return -EIO;
	}

	msgb_put(msg, hdlc_decode.dest_idx);

	if (rc == HDLC_COMPLETE) {
		di->rx.msg = NULL;
		rc = crc_check(msgb_data(msg), msgb_length(msg));
		if (rc) {
			fprintf(stderr, "Bad CRC, dropping packet\n");
			msgb_free(msg);
			return -EINVAL;
		}
		msgb_get(msg, HDLC_FOOTER_LEN);

		if (msgb_length(msg) < 1) {
			fprintf(stderr, "Message too short, len: %u\n", msgb_length(msg));
			msgb_free(msg);
			return -EINVAL;
		}

		if (di->rx.rcvmsg)
			(di->rx.rcvmsg)(di, msg);
		else
			msgb_free(msg);
	}

	return 0;
};
