#include <osmocom/core/msgb.h>
#include "protocol.h"
#include "diagcmd.h"

int diag_push_subsys_hdr(struct msgb *msg, uint8_t subsys, uint8_t code)
{
	struct diagpkt_subsys_hdr *ssh;
	ssh = (struct diagpkt_subsys_hdr *) msgb_push(msg, sizeof(*ssh));
	ssh->command = DIAG_SUBSYS_CMD_F;
	ssh->subsys_id = subsys;
	ssh->subsys_cmd_code = code;

	return 0;
}
