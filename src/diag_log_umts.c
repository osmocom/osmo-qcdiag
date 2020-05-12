#include <stdio.h>

#include <osmocom/core/gsmtap.h>
#include <osmocom/core/gsmtap_util.h>
#include <osmocom/core/utils.h>

#include "diag_log.h"
#include "protocol/diag_log_umts.h"

static void handle_nas_msg(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	uint16_t arfcn = 0;
	struct diag_umts_nas_ota_msg *nas = (struct diag_umts_nas_ota_msg *) msgb_data(msg);

	/* track rat, don't duplicate rrc message contents here for != 2g */
	if(di->rat_type != DIAG_INST_RAT_2G)
		return;

//	printf("NAS: %cL %u: %s\n", nas->direction ? 'U':'D', nas->msg_length,
//		osmo_hexdump(msgb_data(msg), nas->msg_length));

	arfcn = nas->direction ? di->umts_arfcn_ul : di->umts_arfcn_dl;
	arfcn = nas->direction ? arfcn | GSMTAP_ARFCN_F_UPLINK: arfcn;

	if (di->gsmtap && di->flags & DIAG_INST_F_GSMTAP_DECODED) {
		gsmtap_send_ex(di->gsmtap, GSMTAP_TYPE_ABIS, arfcn, 0, 0, 0, 0, 0, 0, nas->data, nas->msg_length);
	}
}

static const struct diag_log_dispatch_tbl log_tbl[] = {
	{ UMTS(LOG_UMTS_NAS_OTA_MESSAGE_LOG_PACKET_C), handle_nas_msg },
};

static __attribute__((constructor)) void on_dso_load_umts(void)
{
	diag_log_reg_dispatch(log_tbl, ARRAY_SIZE(log_tbl));
}
