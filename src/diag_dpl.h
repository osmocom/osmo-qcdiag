#pragma once

#include "diag_io.h"
#include "protocol/dpl.h"

int diag_dpl_reset_logging(struct diag_instance *di);
int diag_dpl_reset_logging(struct diag_instance *di);
int diag_dpl_get_if_desc(struct diag_instance *di, uint8_t iface_id);

int diag_dpl_set_if_log(struct diag_instance *di, uint8_t iface_id,
			struct dpl_iid iid, uint32_t link_type);
