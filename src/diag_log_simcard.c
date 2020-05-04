/* Utility code for DIAG UIM (Simcard) Logging */
/*
 * (C) 2016 by Harald Welte <laforge@gnumonks.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <errno.h>
#include <string.h>
#include <osmocom/core/utils.h>
#include <osmocom/core/msgb.h>

#include "diag_log.h"
#include "diag_io.h"
#include "protocol/diagcmd.h"
#include "protocol/diag_log_1x.h"

static void handle_uim_data(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	struct diag_log_uim_msg *uim = (struct diag_log_uim_msg *) msgb_data(msg);
	printf("UIM_DATA { %s }\n", msgb_hexdump(msg));
}

static void handle_uim_ds_data(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	printf("UIM_DS_DATA {}\n");
}


static const struct diag_log_dispatch_tbl log_tbl[] = {
	{ L1X(LOG_UIM_DATA_C), handle_uim_data },
	{ L1X(LOG_UIM_DS_DATA_C), handle_uim_ds_data },
};
#if 0
static __attribute__((constructor)) void on_dso_load_gsm(void)
{
	diag_log_reg_dispatch(log_tbl, ARRAY_SIZE(log_tbl));
}
#endif
