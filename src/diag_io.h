#pragma once

#include <stdint.h>
#include <osmocom/core/msgb.h>

struct diag_instance {
	int fd;
	struct {
		struct msgb *msg;
		void (*rcvmsg)(struct diag_instance *di, struct msgb *msg);
	} rx;
	struct {
	} tx;
};

int diag_transmit_msgb(struct diag_instance *di, struct msgb *msg);
int diag_transmit_buf(struct diag_instance *di, const uint8_t *data, size_t data_len);
int diag_read(struct diag_instance *di);
