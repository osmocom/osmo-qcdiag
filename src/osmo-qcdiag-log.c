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
#include <signal.h>
#include <getopt.h>

#include <osmocom/core/msgb.h>
#include <osmocom/core/serial.h>
#include <osmocom/core/gsmtap_util.h>
#include <osmocom/core/gsmtap.h>

#include "diag_io.h"
#include "diag_log.h"
#include "diag_msg.h"
#include "diag_cmd.h"
#include "diag_dpl.h"
#include "protocol/protocol.h"
#include "protocol/diagcmd.h"
#include "protocol/diag_log_gsm.h"

struct diag_instance di;
static char *serial_path = 0;
static uint32_t cfg_flags = 0;

static void do_configure(struct diag_instance *di)
{
	static uint8_t timestamp[] = { DIAG_TS_F };
	static const uint8_t enable_evt_report[] = {
		DIAG_EVENT_REPORT_F, 0x01
	};
	static const uint8_t disable_evt_report[] = {
		DIAG_EVENT_REPORT_F, 0x00
	};
	static const uint8_t disable_log[] = {
		DIAG_LOG_CONFIG_F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};

//	diag_transceive_buf_ign(di, timestamp, sizeof(timestamp));
	/* response: 3d 67 a8 81 d4 46 6d d9 00 */

	/* enable|disable the event report */
#if 0
	diag_transceive_buf_ign(di, enable_evt_report, sizeof(enable_evt_report));
#else
	diag_transceive_buf_ign(di, disable_log, sizeof(disable_log));
	diag_msg_config_set_all_rt_mask(di, 0);
//	diag_transceive_buf_ign(di, disable_evt_report, sizeof(disable_evt_report));
	/* response: 40 00 00 */
#endif

//	diag_msg_config_set_rt_mask(di, MSG_SSID_LINUX_DATA, 0xffffffff);
//	/* response: 5d 5d 04 41 00 41 00 01 00 ff ff ff ff */
//	diag_msg_config_set_rt_mask(di, MSG_SSID_DS_ATCOP, 0xffffffff);
//	/* response: 5d 5d 04 94 13 94 13 01 00 ff ff ff ff */
//	diag_msg_config_set_rt_mask(di, MSG_SSID_DS, 0xffffffff);
//	/* response: 5d 5d 04 88 13 88 13 01 00 ff ff ff ff */
//	//diag_msg_config_set_rt_mask(di, MSG_SSID_DS_IPA, 0xffffffff);
//	/* response: 5d 5d 04 a6 13 a6 13 01 00 ff ff ff ff */
//	diag_msg_config_set_rt_mask(di, MSG_SSID_DS_GPRS, 0xffffffff);
//	/* response: 5d 5d 04 91 13 91 13 01 00 ff ff ff ff */
//	diag_msg_config_set_rt_mask(di, MSG_SSID_DS_GSM, 0xffffffff);
//	diag_msg_config_set_rt_mask(di, MSG_SSID_DS_UMTS, 0xffffffff);
//	diag_msg_config_set_rt_mask(di, MSG_SSID_DS_SOCKETS, 0xffffffff);
//	diag_msg_config_set_rt_mask(di, MSG_SSID_DS_SIO, 0xffffffff);
//	diag_msg_config_set_rt_mask(di, MSG_SSID_DS_APPS, 0xffffffff);
//	diag_msg_config_set_rt_mask(di, MSG_SSID_DIAG, 0xffffffff);

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

	diag_tranceive_msgb(di, msg);

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

	diag_tranceive_msgb(di, msg);
#else
	diag_log_enable_all_supported(di);
#endif
}

static void enable_pcap(struct diag_instance *di, uint8_t if_num)
{
	struct dpl_iid iid = { .flags = 0, .if_name = if_num,
			       .protocol = DIAG_DPL_IID_PROT_NET_IP,
			       .link_instance = 64 };
#if 1
	iid.flags &= ~DIAG_DPL_IID_FLAG_DIR_TX;
	diag_dpl_set_if_log(di, if_num, iid, DIAG_DPL_IID_PROT_LINK_ANY);
#else
	iid.flags |= DIAG_DPL_IID_FLAG_DIR_TX;
	diag_dpl_set_if_log(di, if_num, iid, DIAG_DPL_IID_PROT_LINK_ANY);
#endif
}


static void signal_handler(int s)
{
	struct termios tio;
	switch (s) {
	case SIGINT:
	case SIGTERM:
			diag_log_disable_all_supported(&di);
			/* graceful exit by immediate read timeout */
			tcgetattr(di.fd, &tio);
			tio.c_cflag &= CRTSCTS;
			tio.c_cc[VMIN] = 0;
			tio.c_cc[VTIME] = 1;
			tcsetattr(di.fd, TCSANOW, &tio);
//			exit(0);
		break;
	default:
		break;
	}
}

static void print_help()
{
	printf(	"  Some useful help...\n"
		"  -h --help		This help text\n"
		"  -s --serial-path	path to modem serial port, i.e /dev/ttyUSB0\n"
		"   Any combination of the following output flags:\n"
		"  -G --gsmtap		GSMTAP messages sent to localhost\n"
		"  -Q --qcomdbg		plain QC DIAG GSMTAP messages\n"
		"  -H --hexdump		console output of rx/tx messages\n"
		);
}

static void handle_options(int argc, char **argv)
{
	while (1) {
		int option_index = 0, c;
		static struct option long_options[] = {
			{ "help", 0, 0, 'h' },
			{ "gsmtap", 0, 0, 'G' },
			{ "qcomdbg", 0, 0, 'Q' },
			{ "hexdump", 0, 0, 'H' },
			{ "serial-path", 1, 0, 's' },
			{ 0, 0, 0, 0 }
		};

		c = getopt_long(argc, argv, "hGQHs:", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'h':
			print_help();
			exit(0);
		case 'G':
			cfg_flags |= DIAG_INST_F_GSMTAP_DECODED;
			break;
		case 'Q':
			cfg_flags |= DIAG_INST_F_GSMTAP_DIAG;
			break;
		case 'H':
			cfg_flags |= DIAG_INST_F_HEXDUMP;
			break;
		case 's':
			serial_path = optarg;
			break;
		}
	}
}


int main(int argc, char **argv)
{
	struct termios tio;
	int i;
	int rc;

	handle_options(argc, argv);

	if (!serial_path) {
		printf("Invoke with %s -s PATH_TO_SERIAL\n", argv[0]);
		return EXIT_FAILURE;
	}
	if (!cfg_flags) {
		printf("no output options (-G/-Q/-H) specified?! exiting...\n");
		print_help();
		return EXIT_FAILURE;
	}

	memset(&di, 0, sizeof(di));
	di.fd = osmo_serial_init(serial_path, 921600);
	if (di.fd < 0) {
		printf("can't open serial port: errno %d\n", di.fd);
		return EXIT_FAILURE;
	}

	/* blocking reads */
	rc = tcgetattr(di.fd, &tio);
	tio.c_cflag &= CRTSCTS;
	tio.c_cc[VMIN] = 1;
	tio.c_cc[VTIME] = 0;
	rc = tcsetattr(di.fd, TCSANOW, &tio);


	di.gsmtap = gsmtap_source_init("localhost", GSMTAP_UDP_PORT, 0);
	if (di.gsmtap == 0) {
		printf("error initializing gsmtap source!\n");
		return EXIT_FAILURE;
	}

	di.flags = cfg_flags;
	rc = gsmtap_source_add_sink(di.gsmtap);
	if (rc < 0) {
		printf("error initializing gsmtap sink!\n");
		return EXIT_FAILURE;
	}

	printf("\n===> CONFIGURING\n");

	do_configure(&di);
//	diag_dpl_get_sup_if(&di);
//	for (i = 1; i < 0x56; i++) {
//		diag_dpl_get_if_desc(&di, i);
//		enable_pcap(&di, i);
//	}

	/* Handle keyboard interrupts */
	signal(SIGINT, &signal_handler);
	signal(SIGTERM, &signal_handler);
	signal(SIGABRT, &signal_handler);
	signal(SIGUSR1, &signal_handler);
	signal(SIGUSR2, &signal_handler);


	printf("\n===> ENTERING MAIN\n");

	while (1) {
		i++;
		struct msgb *rx = diag_read_msg(&di);
		if (rx)
			diag_process_msg(&di, rx);
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
