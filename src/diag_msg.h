#pragma once

#include <osmocom/core/msgb.h>
#include "diag_io.h"


int diag_rx_ext_msg_f(struct diag_instance *di, struct msgb *msg);
