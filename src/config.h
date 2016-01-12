#pragma once

#include <stdint.h>

struct msgb;

struct msgb *gen_log_config_set_mask(uint32_t last_item);
int log_config_set_mask_bit(struct msgb *msg, uint32_t bit_in);
