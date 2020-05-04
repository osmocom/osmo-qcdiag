#pragma once

#include <stdint.h>
#include <osmocom/core/msgb.h>
#include <osmocom/core/gsmtap_util.h>

#define DIAG_INST_F_HEXDUMP	0x00000001
#define DIAG_INST_F_GSMTAP_DIAG	0x00000002
#define DIAG_INST_F_GSMTAP_DECODED	0x00000004

struct diag_instance {
	int fd;
	struct {
		struct msgb *msg;
	} rx;
	struct {
	} tx;
	struct gsmtap_inst *gsmtap;
	uint32_t flags;
	uint32_t gsm_arfcn;
	uint32_t umts_arfcn_ul;
	uint32_t umts_arfcn_dl;
	uint32_t lte_arfcn_ul;
	uint32_t lte_arfcn_dl;
};

struct msgb *msgb_alloc_diag(void);
int diag_transmit_msgb(struct diag_instance *di, struct msgb *msg);
int diag_transmit_buf(struct diag_instance *di, const uint8_t *data, size_t data_len);
struct msgb *diag_read_msg(struct diag_instance *di);
int diag_process_msg(struct diag_instance *di, struct msgb *msg);
struct msgb *diag_transceive_msg(struct diag_instance *di, struct msgb *tx);
struct msgb *diag_subsys_transceive_msg(struct diag_instance *di, struct msgb *tx,
					uint8_t subsys, uint16_t code);
void diag_transceive_msg_ign(struct diag_instance *di, struct msgb *tx);
struct msgb *diag_transceive_buf(struct diag_instance *di, const uint8_t *data, size_t data_len);
void diag_transceive_buf_ign(struct diag_instance *di, const uint8_t *data, size_t data_len);

/* FIXME: this should be in libosmocore */
static inline unsigned int bytes_rqd_for_bit(unsigned int bit)
{
	if (bit % 8)
		return bit/8 + 1;
	else
		return bit/8;
}
