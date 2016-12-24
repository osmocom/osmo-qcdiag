
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <osmocom/core/msgb.h>

#include "protocol.h"
#include "diag_msg.h"
#include "diagcmd.h"


/* handler for EXT MSG */
int diag_rx_ext_msg_f(struct diag_instance *di, struct msgb *msgb)
{
	const uint8_t *data = msgb_data(msgb);
	const size_t len = msgb_length(msgb);
	const struct ext_log_msg *msg;
	const char *file = NULL, *fmt;
	unsigned int num_args;

	if (len < sizeof(struct ext_log_msg)) {
		printf("too short ext_log_msg.\n");
		return -1;
	}

	msg = (struct ext_log_msg *) data;
	num_args = msg->num_args;
	fmt = (const char *) msg->params + num_args*sizeof(msg->params[0]);
	file = fmt + strlen(fmt) + 1;

	printf("MSG(%u|%s:%u): ", diag_ts_to_epoch(msg->timestamp), file, msg->line_nr);
	switch (num_args) {
	case 0:
		fputs(fmt, stdout);
		break;
	case 1:
		printf(fmt, msg->params[0]);
		break;
	case 2:
		printf(fmt, msg->params[0], msg->params[1]);
		break;
	case 3:
		printf(fmt, msg->params[0], msg->params[1], msg->params[2]);
		break;
	case 4:
		printf(fmt, msg->params[0], msg->params[1], msg->params[2], msg->params[3]);
		break;
	}
	fputc('\n', stdout);
	return 0;
}


