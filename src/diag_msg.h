#pragma once

#include <osmocom/core/msgb.h>
#include "diag_io.h"
#include "protocol/diag_msg_codes.h"

struct msgb *gen_msg_config_set_rt_mask(uint16_t ssid, uint32_t runtime_mask);
int diag_msg_config_set_rt_mask(struct diag_instance *di, uint16_t ssid, uint32_t runtime_mask);
