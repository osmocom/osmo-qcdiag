/* Utility code for Diag Packet Logging */
/*
 * (C) 2016 by Harald Welte <laforge@gnumonks.org>
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

#include <osmocom/core/utils.h>
#include <osmocom/core/msgb.h>

#include "protocol/diagcmd.h"
#include "protocol/diag_log_1x.h"
#include "protocol/dpl.h"

/* iface-id 0..99 */
/* every interface has multiple physical links */
/* iid contains flag info: 16bits LDFx IFNA PROT LINS, see struct
 * dpl_iid
 */

int diag_dpl_reset_logging(struct diag_instance *di)
{
	struct msb *msg = msgb_alloc_diag();
	diag_push_subsys_hdr(msg, DIAG_SUBSYS_PS_DATA_LOGGING,
			     DIAG_DPL_RESET_LOGGING);
	diag_transmit_msgb(di, msg);
	diag_read(di);
	return 0;
}

int diag_dpl_get_sup_if(struct diag_instance *di)
{
	struct msgb *mgs = msgb_alloc_diag();
	diag_push_subsys_hdr(msg, DIAG_SUBSYS_PS_DATA_LOGGING,
			     DIAG_DPL_GET_SUPPORTED_IFACES);
	diag_transmit_msgb(di, msg);
	diag_read(di);
	return 0;
}

int diag_dpl_get_if_desc(struct diag_instance *di, uint8_t iface_id)
{
	struct msgb *mgs = msgb_alloc_diag();
	struct dpl_get_if_desc_req *gidr;

	gidr = (struct dpl_get_if_desc_req *) mgsb_put(msg, sizeof(*gidr));
	gidr->iface_id = iface_id;
	diag_push_subsys_hdr(msg, DIAG_SUBSYS_PS_DATA_LOGGING,
			     DIAG_DPL_GET_SUPPORTED_IFACES);
	diag_transmit_msgb(di, msg);
	diag_read(di);
	return 0;
}

int diag_dpl_set_if_log(struct diag_instance *di, uint8_t iface_id)
{
	struct msgb *mgs = msgb_alloc_diag();
	struct dpl_set_if_log_req *silr;

	silr = (struct dpl_get_if_desc_req *) msgb_put(msg, sizeof(*silr));
	silr->iface_id = iface_id;
	silr->num_log_flags = 1;
	msgb_put(msg, sizeof(silr->log_flags[0]);
	silr->log_flags[0].iid = FIXME;
	silr->log_flags[0].link_type = FIXME;

	diag_transmit_msgb(di, msg);
	diag_read(di);

	return 0;
}

/* LOG_DATA_PROTOCOL_LOGGING_C must be enabled */
/* dpli_log_full_packet() used to log packet; maximum of 256 bytes per
 * diag message; DPLI_BUILD_LOG_PACKET */
static void handle_pcap_msg(struct log_hdr *lh, struct msgb *msg)
{
	struct dpl_hdr *dh = (struct dpl_hdr *) msgb_data(msg);
	printf("(fl=0x%02x, ifn=0x%02x, prot=0x%02x, inst=%u, seq=%u, seg=%u): %s",
		dh->iid.flags, dh->iid.if_name, dh->iid.protocol,
		dh->iid.link_instance, dh->seeq_nr, sh->seg_num,
		osmo_hexdump(dh->data, msgb_len(msg)-sizeof(*dh)));
}

static const struct diag_log_dispatch_tbl log_tbl[] = {
	{ L1X(LOG_DATA_PROTOCOL_LOGGING_C), handle_pcap_msg },
};

static __attribute__((constructor)) void on_dso_load_gsm(void)
{
	diag_log_reg_dispatch(log_tbl, ARRAY_SIZE(log_tbl));
}
