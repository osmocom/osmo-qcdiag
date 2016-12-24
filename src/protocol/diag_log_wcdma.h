#pragma once

#include <stdint.h>

#define UMTS(x)	(0x4000 + x)

enum diag_log_code_umts {
	LOG_WCDMA_RRC_STATES_C				= 0x125,
	LOG_WCDMA_RRC_PROTOCOL_ERRORS_C			= 0x126,
	LOG_WCDMA_CELL_ID_C				= 0x127,
	LOG_WCDMA_SIGNALING_MSG_C			= 0x12f,
};

enum diag_umts_rrc_chtype {
	DIAG_UMTS_RRC_CHT_UL_CCCH	= 0,
	DIAG_UMTS_RRC_CHT_DL_DCCH	= 1,
	DIAG_UMTS_RRC_CHT_DL_CCCH	= 2,
	DIAG_UMTS_RRC_CHT_DL_BCCH	= 3,
	DIAG_UMTS_RRC_CHT_DL_BCCH_BCH	= 4,
	DIAG_UMTS_RRC_CHT_DL_BCCH_FACH	= 5,
	DIAG_UMTS_RRC_CHT_DL_PCCH	= 6,
};

/* LOG_WCDMA_SIGNALING_MSG_C */
struct diag_umts_rrc_msg {
	uint8_t		chan_type;
	uint8_t		rb_id;
	uint16_t	length;
	uint8_t		msg[0];
} __attribute__((packed));
