#include <stdio.h>

#include "diag_log.h"
#include "protocol/diag_log_wcdma.h"

static void handle_rrc_sig_msg(struct log_hdr *lh, struct msgb *msg)
{
	struct diag_umts_rrc_msg *rrm = (struct diag_umts_rrc_msg *) msgb_data(msg);

	printf("RRC: %u %u %u: %s\n", rrm->chan_type, rrm->rb_id, rrm->length,
		osmo_hexdump(msgb_data(msg), rrm->length));
}

static const struct diag_log_dispatch_tbl log_tbl[] = {
	{ UMTS(LOG_WCDMA_SIGNALING_MSG_C), handle_rrc_sig_msg },
};

static __attribute__((constructor)) void on_dso_load_umts(void)
{
	diag_log_reg_dispatch(log_tbl, ARRAY_SIZE(log_tbl));
}
