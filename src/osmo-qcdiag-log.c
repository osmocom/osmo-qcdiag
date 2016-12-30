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

#include <stdlib.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <osmocom/core/msgb.h>
#include <osmocom/core/serial.h>
#include <osmocom/core/gsmtap_util.h>
#include <osmocom/core/gsmtap.h>

#include "diag_io.h"
#include "diag_log.h"
#include "diag_msg.h"
#include "protocol/protocol.h"
#include "protocol/diagcmd.h"

/*********/

static void diag_process_msg(struct diag_instance *di, struct msgb *msg)
{
	switch (msg->l2h[0]) {
	case DIAG_LOG_F:
		diag_log_handle(di, msg);
		break;
	case DIAG_EXT_MSG_F:
		diag_rx_ext_msg_f(di, msg);
		break;
	default:
		printf("Got %d bytes data of unknown payload type 0x%02x: %s\n",
			msgb_length(msg), msg->l2h[0],
			osmo_hexdump(msgb_data(msg), msgb_length(msg)));
		break;
	}
	msgb_free(msg);
}

static void do_configure(struct diag_instance *di)
{
	static uint8_t timestamp[] = { DIAG_TS_F };
	static const uint8_t enable_evt_report[] = {
		DIAG_EVENT_REPORT_F, 0x01
	};
	static const uint8_t disable_evt_report[] = {
		DIAG_EVENT_REPORT_F, 0x00
	};

	/* TODO: introduce a wait for response kind of method */
	diag_transmit_buf(di, timestamp, sizeof(timestamp));
	diag_read(di);

	/* enable|disable the event report */
#if 0
	diag_transmit_buf(di, enable_evt_report, sizeof(enable_evt_report));
	diag_read(di);
#else
	diag_transmit_buf(di, disable_evt_report, sizeof(disable_evt_report));
	diag_read(di);
#endif
	diag_msg_config_set_rt_mask(di, MSG_SSID_LINUX_DATA, 0xffffffff);
	diag_msg_config_set_rt_mask(di, 5012, 0xffffffff);
	diag_msg_config_set_rt_mask(di, 5000, 0xffffffff);
	diag_msg_config_set_rt_mask(di, 5030, 0xffffffff);
	diag_msg_config_set_rt_mask(di, 5009, 0xffffffff);

#if 0
	printf("GSM\n");
	struct msgb *msg = gen_log_config_set_mask(5, 1064);
#if 0
	for (int i = 0; i < 1064; i++)
		log_config_set_mask_bit(msg, i);
#endif

	log_config_set_mask_bit(msg, LOG_GSM_RR_CONTROL_CHANNEL_PARAMS_C);

	log_config_set_mask_bit(msg, LOG_GPRS_RLC_UL_ACKNACK_PARAMS_VER2_C);
	log_config_set_mask_bit(msg, LOG_GPRS_RLC_DL_ACKNACK_PARAMS_VER2_C);
	log_config_set_mask_bit(msg, LOG_EGPRS_RLC_DL_HEADER_C);
	log_config_set_mask_bit(msg, LOG_EGPRS_RLC_UL_HEADER_C);

	log_config_set_mask_bit(msg, LOG_GPRS_LLC_PERIODIC_STATS_C);

	log_config_set_mask_bit(msg, LOG_GPRS_SNDCP_UL_TCP_HDR_C);
	log_config_set_mask_bit(msg, LOG_GPRS_SNDCP_DL_TCP_HDR_C);
	log_config_set_mask_bit(msg, 546);
	log_config_set_mask_bit(msg, 547);
	log_config_set_mask_bit(msg, LOG_EGPRS_MAC_UL_ACKNACK_C);
	log_config_set_mask_bit(msg, LOG_EGPRS_MAC_DL_ACKNACK_C);

	diag_transmit_msgb(di, msg);
	diag_read(di);

	printf("WCDMA\n");
	msg = gen_log_config_set_mask(4, 1064);
#if 0
	for (int i = 0; i < 1064; i++)
		log_config_set_mask_bit(msg, i);
#endif
	log_config_set_mask_bit(msg, 0x125);
	log_config_set_mask_bit(msg, 0x126);
	log_config_set_mask_bit(msg, 0x127);
	log_config_set_mask_bit(msg, 0x128);
	log_config_set_mask_bit(msg, 0x129);

	diag_transmit_msgb(di, msg);
	diag_read(di);
#else
	diag_log_enable_all_supported(di);
#endif
}

int main(int argc, char **argv)
{
	struct diag_instance di;
	int i;
	int rc;

	if (argc < 2) {
		printf("Invoke with %s PATH_TO_SERIAL\n",
			argv[0]);
		return EXIT_FAILURE;
	}

	memset(&di, 0, sizeof(di));
	di.rx.rcvmsg = &diag_process_msg;
	di.fd = osmo_serial_init(argv[1], 921600);
	if (di.fd < 0)
		return EXIT_FAILURE;

	do_configure(&di);

	di.gsmtap = gsmtap_source_init("localhost", GSMTAP_UDP_PORT, 0);
	gsmtap_source_add_sink(di.gsmtap);

	while (1) {
		i++;
		rc = diag_read(&di);
		if (rc == -EIO)
			break;
#if 0
		/* some packets need to be explicitly requested and
		 * don't appear automatically */
		if (i % 10 == 0) {
			struct msgb *msg = diag_gsm_make_log_pack_req(LOG_GPRS_LLC_PDU_STATS_C , 0, 0);
			printf("Requesting LLC stats...(%s)\n", osmo_hexdump(msgb_data(msg), msgb_length(msg)));
			diag_transmit_msgb(&di, msg);
		}
#endif

	}

	return EXIT_SUCCESS;
}
