#include <stdio.h>

#include "diag_log.h"
#include "protocol/diag_log_umts.h"

static void handle_nas_msg(struct log_hdr *lh, struct msgb *msg)
{
	struct diag_umts_nas_ota_msg *nas = (struct diag_umts_nas_ota_msg *) msgb_data(msg);

	printf("NAS: %cL %u: %s\n", nas->direction ? 'U':'D', nas->msg_length,
		osmo_hexdump(msgb_data(msg), nas->msg_length));
}

static const struct diag_log_dispatch_tbl log_tbl[] = {
	{ UMTS(LOG_UMTS_NAS_OTA_MESSAGE_LOG_PACKET_C), handle_nas_msg },
};

static __attribute__((constructor)) void on_dso_load_umts(void)
{
	diag_log_reg_dispatch(log_tbl, ARRAY_SIZE(log_tbl));
}
