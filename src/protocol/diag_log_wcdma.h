#pragma once

#include <stdint.h>

#define WCDMA(x)	(0x4000 + x)

enum diag_log_code_wcdma {
	LOG_WCDMA_RRC_STATES_C				= 0x125,
	LOG_WCDMA_RRC_PROTOCOL_ERRORS_C			= 0x126,
	LOG_WCDMA_CELL_ID_C				= 0x127,
	LOG_WCDMA_UL_CHANNEL_MAPPING_C			= 0x128,
	LOG_WCDMA_DL_CHANNEL_MAPPING_C			= 0x129,
	LOG_WCDMA_FP_INFO_C				= 0x12a,
	LOG_WCDMA_SIB_C					= 0x12b,
	LOG_WCDMA_SIGNALING_MSG_C			= 0x12f,
};

enum diag_umts_rrc_chtype {
	DIAG_UMTS_RRC_CHT_UL_CCCH	= 0,
	DIAG_UMTS_RRC_CHT_UL_DCCH	= 1,
	DIAG_UMTS_RRC_CHT_DL_CCCH	= 2,
	DIAG_UMTS_RRC_CHT_DL_DCCH	= 3,
	DIAG_UMTS_RRC_CHT_DL_BCCH_BCH	= 4,
	DIAG_UMTS_RRC_CHT_DL_BCCH_FACH	= 5,
	DIAG_UMTS_RRC_CHT_DL_PCCH	= 6,
	DIAG_UMTS_RRC_CHT_EXTENSION_SIB = 9,
	DIAG_UMTS_RRC_CHT_SIB_CONTAINER = 10,
};

enum diag_umts_rrc_state {
	DIAG_UMTS_RRC_STATE_DISCONNECTED	= 0,
	DIAG_UMTS_RRC_STATE_CONNETING		= 1,
	DIAG_UMTS_RRC_STATE_CELL_FACH		= 2,
	DIAG_UMTS_RRC_STATE_CELL_DCH		= 3,
	DIAG_UMTS_RRC_STATE_CELL_PCH		= 4,
	DIAG_UMTS_RRC_STATE_URA_PCH		= 5,
	DIAG_UMTS_RRC_STATE_WILDCARD		= 6,
};

/* LOG_WCDMA_RRC_STATES_C */
struct diag_umts_rrc_state_msg {
	uint8_t 	rrc_state;
} __attribute__((packed));

enum diag_umts_rrc_procedure {
	DIAG_UMTS_RRC_PROC_CSP		= 0,
	DIAG_UMTS_RRC_PROC_SIB		= 1,
	DIAG_UMTS_RRC_PROC_PG2		= 2,
	DIAG_UMTS_RRC_PROC_MCMR		= 3,
	DIAG_UMTS_RRC_PROC_RCE		= 4,
	DIAG_UMTS_RRC_PROC_RCR		= 5,
	DIAG_UMTS_RRC_PROC_UECI		= 6,
	DIAG_UMTS_RRC_PROC_UECE		= 7,
	DIAG_UMTS_RRC_PROC_IDT		= 8,
	DIAG_UMTS_RRC_PROC_UDT		= 9,
	DIAG_UMTS_RRC_PROC_DDT		= 10,
	DIAG_UMTS_RRC_PROC_SCR		= 11,
	DIAG_UMTS_RRC_PROC_SCRR		= 12,
	DIAG_UMTS_RRC_PROC_CC		= 13,
	DIAG_UMTS_RRC_PROC_RBE		= 14,
	DIAG_UMTS_RRC_PROC_RBRC		= 15,
	DIAG_UMTS_RRC_PROC_RBR		= 16,
	DIAG_UMTS_RRC_PROC_TCR		= 17,
	DIAG_UMTS_RRC_PROC_PCR		= 18,
	DIAG_UMTS_RRC_PROC_TFC		= 19,
	DIAG_UMTS_RRC_PROC_CU		= 20,
	DIAG_UMTS_RRC_PROC_UU		= 21,
	DIAG_UMTS_RRC_PROC_UMI		= 22,
	DIAG_UMTS_RRC_PROC_ASU		= 23,
	DIAG_UMTS_RRC_PROC_ISHFU	= 24,
	DIAG_UMTS_RRC_PROC_USHTU	= 25,
	DIAG_UMTS_RRC_PROC_ISCFU	= 26,
	DIAG_UMTS_RRC_PROC_ISCTU	= 27,
	DIAG_UMTS_RRC_PROC_PG1		= 28,
	DIAG_UMTS_RRC_PROC_SMC		= 29,
};

enum diag_umts_rrc_fail_cause {
	DIAG_UMTS_RRC_FAIL_CFG_UNSUP		= 0,
	DIAG_UMTS_RRC_FAIL_PHY_CHAN_FAIL	= 1,
	DIAG_UMTS_RRC_FAIL_INCOMP_SIM_RECONF	= 2,
	DIAG_UMTS_RRC_FAIL_PROT_ERR		= 3,
	DIAG_UMTS_RRC_FAIL_COMP_RT_ERR		= 4,
	DIAG_UTMS_RRC_FAIL_CELL_RESEL		= 5,
	DIAG_UMTS_RRC_FAIL_INVAL_CFG		= 6,
	DIAG_UMTS_RRC_FAIL_CFG_INCOMPLETE	= 7,
	DIAG_UMTS_RRC_FAIL_UNSUP_MEAS		= 8,
};

enum diag_umts_rrc_prot_err {
	DIAG_UMTS_RRC_PERR_ASN1_ENC_ERR		= 0,
	DIAG_UMTS_RRC_PERR_BAD_MSG_TYPE		= 1,
	DIAG_UMTS_RRC_PERR_IMCOMP_RX_STATE	= 2,
	DIAG_UMTS_RRC_PERR_BAD_IE		= 3,
	DIAG_UMTS_RRC_PERR_COND_IE_ERR		= 4,
	DIAG_UMTS_RRC_PERR_BAD_MSG_EXT		= 5,
};

/* LOG_WCDMA_RRC_PROTOCOL_ERRORS_C */
struct diag_umts_rrc_prot_err_msg {
	uint8_t		rrc_state;
	uint8_t		rrc_procedure;
	uint8_t		failure_cause;
	uint8_t		prot_err_cause;
} __attribute__((packed));

/* LOG_WCDMA_SIGNALING_MSG_C */
struct diag_umts_rrc_msg {
	uint8_t		chan_type;
	uint8_t		rb_id;
	uint16_t	length;
	uint8_t		msg[0];
} __attribute__((packed));
