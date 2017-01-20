#pragma once

#include "diag_io.h"
#include "protocol/protocol.h"

typedef void diag_log_handler(struct log_hdr *lh, struct msgb *msg);

struct diag_log_dispatch_tbl {
	uint16_t code;
	diag_log_handler *handler;
};

/* called by input whenever a LOG message is registered */
void diag_log_handle(struct diag_instance *di, struct msgb *msg);

/* called by individual modules to register their own decoders */
void diag_log_reg_dispatch(const struct diag_log_dispatch_tbl *tbl, unsigned int size);

void diag_log_enable_all_supported_family(struct diag_instance *di, uint8_t family);
void diag_log_enable_all_supported(struct diag_instance *di);

/* functions for log configuration */
struct msgb *gen_log_config_set_mask(uint32_t equip_id, uint32_t last_item);
int log_config_set_mask_bit(struct msgb *msg, uint32_t bit_in);

void diag_log_hdl_default(struct log_hdr *lh, struct msgb *msg);
