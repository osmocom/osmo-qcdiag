#include <stdint.h>
#include <osmocom/core/msgb.h>

#include "diagcmd.h"
#include "framing.h"

enum log_config_op {
	LOG_CONFIG_DISABLE_OP			= 0,
	LOG_CONFIG_RETRIEVE_ID_RANGES_OP	= 1,
	LOG_CONFIG_RETRIEVE_VALID_MASK_OP	= 2,
	LOG_CONFIG_SET_MASK_OP			= 3,
	LOG_CONFIG_GET_LOGMASK_OP		= 4,
};

struct diag_log_config_req_hdr {
	uint8_t		msg_type;
	uint8_t		pad[3];
	uint32_t	operation;
} __attribute((packed));

struct diag_log_config_set_mask {
	struct diag_log_config_req_hdr hdr;
	uint32_t	equip_id;
	uint32_t	last_item;
	uint8_t		data[0];
} __attribute((packed));

struct msgb *gen_log_config_set_mask(uint32_t last_item)
{
	struct msgb *msg = msgb_alloc(MAX_PACKET, "Diag Tx");
	struct diag_log_config_set_mask *dlcsm;
	uint8_t *mask;

	msg->l2h = msgb_put(msg, sizeof(*dlcsm));
	dlcsm = (struct diag_log_config_set_mask *) msg->l2h;
	dlcsm->hdr.msg_type = DIAG_LOG_CONFIG_F;
	dlcsm->hdr.operation = LOG_CONFIG_SET_MASK_OP;
	dlcsm->equip_id = 5;
	dlcsm->last_item = last_item;
	msg->l3h = msgb_put(msg, dlcsm->last_item/8);

	return msg;
}

int log_config_set_mask_bit(struct msgb *msg, uint32_t bit_in)
{
	struct diag_log_config_set_mask *dlcsm = msg->l2h;
	uint8_t *mask = msg->l3h;
	unsigned int byte = bit_in / 8;
	unsigned int bit = bit_in % 8;

	if (byte > dlcsm->last_item/8)
		return -1;

	mask[byte] |= (1 << bit);

	return 0;
}
