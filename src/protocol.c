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

#include <osmocom/core/msgb.h>
#include "protocol/protocol.h"
#include "protocol/diagcmd.h"

int diag_push_subsys_hdr(struct msgb *msg, uint8_t subsys, uint8_t code)
{
	struct diagpkt_subsys_hdr *ssh;
	ssh = (struct diagpkt_subsys_hdr *) msgb_push(msg, sizeof(*ssh));
	ssh->command = DIAG_SUBSYS_CMD_F;
	ssh->subsys_id = subsys;
	ssh->subsys_cmd_code = code;

	return 0;
}

#include <sys/time.h>
#include <osmocom/gsm/gsm_utils.h>

uint32_t diag_ts_to_fn(uint64_t ts)
{
	return (ts/204800)%GSM_MAX_FN;
}

uint32_t diag_ts_to_epoch(uint64_t qd_time)
{
	double qd_ts;

	qd_ts = osmo_load64le(&qd_time);
	qd_ts *= 1.25*256.0/1000.0;

	/* Sanity check on timestamp (year > 2011) */
	if (qd_ts < 1000000000) {
		/* Use current time */
		int rv = -1;
		struct timeval tv;

		rv = gettimeofday(&tv, NULL);
		if (0 == rv)
			return tv.tv_sec;
	} else {
		/* Adjust timestamp from GPS to UNIX */
		qd_ts += 315964800.0;
	}

	return qd_ts;
}
