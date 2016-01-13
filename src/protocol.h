#ifndef protocol_h
#define protocol_h

#include <stdint.h>

/*
 * 79 00 03 00 06 00 9C 76 B6 07 00 00 D4 00 00 00    y......v¶...Ô...
 * 02 00 00 00 39 00 00 00 63 F9 FF FF 00 00 00 00    ....9...cùÿÿ....
 * 41 43 51 20 53 74 61 72 74 65 64 2C 20 41 52 46    ACQ.Started,.ARF
 * 43 4E 3D 25 64 2C 20 52 78 50 57 52 78 31 36 3D    CN=%d,.RxPWRx16=
 * 25 64 00 6C 31 5F 61 63 71 2E 63 00                %d.l1_acq.c.
 */

struct ext_log_msg {
	uint8_t		type;
	uint16_t	unknown1;	/* 0x00 0x03 type??? */
	uint16_t	unknown2;	/* 0x00 0x06 type??? */
	uint64_t	timestamp;	/* More 32 bit but dm-commands.h */
	uint8_t		unknown3[3];	/* 0x00 0x00 0x00 */
	uint32_t	unknown4;	/* 0x02 0x00 0x00 0x00 */
	int32_t		params[3];	/* three params */
	uint8_t		data[0];	/* two NULL terminated strings from here */
} __attribute__((packed));


/* message header */
struct diag_msg_hdr {
	uint8_t		cmd_code;
	uint8_t		timestamp_type;
	uint8_t		num_args;
	uint8_t		drop_count;
	uint64_t	ts;
} __attribute__((packed));

/* message descriptor */
struct diag_msg_desc {
	uint16_t	line;
	uint16_t	subsys_id;
	uint16_t	subsys_mask;
} __attribute__((packed));

/* message header for DIAG_EXT_MSG_F */
struct diag_msg_ext {
	struct diag_msg_hdr	hdr;
	struct diag_msg_desc	desc;
	uint32_t		args[0];	/* see hdr.num_args */
	/* followed by null-terminated strings */
} __attribute__((packed));


/* message header for DIAG_LOG_F */
struct diag_log_hdr {
	uint8_t		cmd_code;
	uint8_t		more;
	uint16_t	len;
	uint8_t		data[0];
} __attribute__((packed));

struct log_hdr {
	uint16_t	len;
	uint16_t	code;
	uint64_t	ts;
	uint8_t		data[0];
} __attribute__((packed));


/* extended DIAG packet, */
struct diagpkt_subsys_hdr {
	uint8_t		command;	/* DIAG_SUBSYS_CMD_F */
	uint8_t		subsys_id;
	uint8_t		subsys_cmd_code;
} __attribute__((packed));

int diag_push_subsys_hdr(struct msgb *msg, uint8_t subsys, uint8_t code);

#endif
