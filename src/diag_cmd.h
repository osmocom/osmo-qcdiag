#pragma once

#include <osmocom/core/msgb.h>
#include "diag_io.h"

typedef void diag_cmd_handler(struct diag_instance *di, struct msgb *msg);

struct diag_cmd_dispatch_tbl {
	uint8_t code;
	diag_cmd_handler *handler;
};

void diag_cmd_reg_dispatch(const struct diag_cmd_dispatch_tbl *tbl, unsigned int size);
int diag_process_msg(struct diag_instance *di, struct msgb *msg);
