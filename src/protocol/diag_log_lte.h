#pragma once

#include <stdint.h>

#define LTE(x) (0xb000 + x)

enum diag_log_code_lte {
	LOG_LTE_RRC_OTA_MSG_LOG_C = 0xc0,
	LOG_LTE_RRC_SERV_CELL_INFO_LOG_C = 0xc2,

	LOG_LTE_NAS_ESM_OTA_IN_SEC_MSG_LOG_C  = 0xE0,
	LOG_LTE_NAS_ESM_OTA_OUT_SEC_MSG_LOG_C  = 0xE1,

	LOG_LTE_NAS_ESM_OTA_IN_MSG_LOG_C = 0xe2,
	LOG_LTE_NAS_ESM_OTA_OUT_MSG_LOG_C = 0xe3,

	LOG_LTE_NAS_EMM_OTA_IN_SEC_MSG_LOG_C = 0xEA,
	LOG_LTE_NAS_EMM_OTA_OUT_SEC_MSG_LOG_C = 0xEB,

	LOG_LTE_NAS_EMM_OTA_IN_MSG_LOG_C = 0xec,
	LOG_LTE_NAS_EMM_OTA_OUT_MSG_LOG_C = 0xed,

};

enum diag_lte_rrc_chtype {
	DIAG_UMTS_RRC_CHT_UL_CCCH = 0,
	DIAG_UMTS_RRC_CHT_UL_DCCH = 1,
	DIAG_UMTS_RRC_CHT_DL_CCCH = 2,
	DIAG_UMTS_RRC_CHT_DL_DCCH = 3,
	DIAG_UMTS_RRC_CHT_DL_BCCH_BCH = 4,
	DIAG_UMTS_RRC_CHT_DL_BCCH_FACH = 5,
	DIAG_UMTS_RRC_CHT_DL_PCCH = 6,
	DIAG_UMTS_RRC_CHT_EXTENSION_SIB = 9,
	DIAG_UMTS_RRC_CHT_SIB_CONTAINER = 10,
};

struct diag_lte_rrc_ota_base_msg {
	uint8_t ext_ver;
	uint8_t rrc_release;
	uint8_t rrc_version;
	uint8_t bearer;
	uint16_t cellid;
} __attribute__((packed));

struct diag_lte_rrc_ota_base_pre8_msg {
	struct diag_lte_rrc_ota_base_msg hdr;
	uint16_t freq;
	uint16_t sfn;
	uint8_t channel_type;
	uint16_t len;

} __attribute__((packed));

struct diag_lte_rrc_ota_base_8plus_msg {
	struct diag_lte_rrc_ota_base_msg hdr;
	uint32_t freq;
	uint16_t sfn;
	uint8_t channel_type;
	uint8_t sib_mask_pad[4];
	uint16_t len;
} __attribute__((packed));


struct diag_lte_rrc_nas_msg {
	uint8_t ext_ver;
	uint8_t rrc_release;
	uint8_t rrc_version_minor;
	uint8_t rrc_version_major;
} __attribute__((packed));


