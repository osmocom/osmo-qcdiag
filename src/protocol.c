#include <osmocom/core/msgb.h>
#include "protocol.h"
#include "diagcmd.h"

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
