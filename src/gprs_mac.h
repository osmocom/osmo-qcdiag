#include <stdint.h>

enum gprs_mac_mode {
	NULL_MODE,
	IDLE_MODE,
	TRANSFER_MODE,
};

enum gprs_mac_null_substates {
	NULL_INACTIVE,
	NULL_GPRS_51_ACTIVE,
	NULL_SUSPENDED,
	NULL_SUSPEND_RESEL,
	NULL_UPLINK_TBF_ESTAB,
	NULL_UPLINK_SB_ESTAB,
	NULL_DOWNLINK_SB_ESTAB,
	NULL_UPLINK_TBF_ACCESS,
	NULL_DL_TBF_EST,
	NULL_DTM_CONNECT_PENDING,
	NULL_DTM_GTTP_UL_CON,
	NULL_RESET,
};

enum gprs_mac_idle_substates {
	IDLE_PASSIVE,
	IDLE_UL_PACKET_ACCESS,
	IDLE_UL_ACCESS_QUEUED,
	IDLE_UL_ACCESS_REJECT,
	IDLE_UL_1PHASE_ASS,
	IDLE_UL_2PHASE_ASS,
	IDLE_UL_1PHASE_CONT_RES,
	IDLE_DL_ASS,
};

enum gprs_mac_transfer_substates {
	TRANSFER_INACTIVE,
	TRANSFER_DL,
	TRANSFER_DL_REASSIGN,
	TRANSFER_DL_WITH_DYNAMIC_ALLOC,
	TRANSFER_DL_REASSIGN_DYNAMIC_ALLOC,
	TRANSFER_UL_DYNAMIC,
	TRANSFER_UL_DYNAMIC_REALLOC,
	TRANSFER_UL_DYNAMIC_DL_ASSIGN,
	TRANSFER_UL_DYNAMIC_REALLOC_DL_ASSIGN,
	TRANSFER_CONCURRENT_DYNAMIC_TRANSFER,
	TRANSFER_CONCURRENT_DYNAMIC_DL_REASSIGN,
	TRANSFER_CONCURRENT_DYNAMIC_REALLOC,
	TRANSFER_CONCURRENT_DL_REASSIGN_DYNAMIC_REALLOC,
};

/* LOG_GPRS_MAC_STATE_C */
struct gprs_mac_state_change {
	uint8_t mac_mode;
	uint8_t mac_null_fsm_state;
	uint8_t mac_idle_fsm_state;
	uint8_t mac_transfer_fsm_state;
} __attribute__((packed));

/* LOG_GPRS_MAC_SIGNALLING_MESSAGE_C */
struct gprs_mac_signalling_msg {
	uint8_t chan_type;
	uint8_t msg_type;
	uint8_t msg_len;
	uint8_t msg[0];
} __attribute__((packed));

/* LOG_GPRS_MAC_DL_TBF_ESTABLISH_C */
struct gprs_mac_dl_tbf_est {
	uint8_t dl_tfi;
	uint8_t rlc_mode;
	uint8_t dl_ts_bmap;
	uint8_t is_egprs_tbf;
	uint16_t egprs_win_size;
	uint8_t egprs_link_qual_mode;
	uint8_t bep_period2;
} __attribute__((packed));

/* LOG_GPRS_MAC_UL_TBF_ESTABLISH_C */
struct gprs_mcc_ul_tbf_est {
	uint8_t tbf_req_cause;
	uint8_t acc_granted;
	uint8_t radio_prio;
	uint8_t peak_tput;
	uint8_t ul_tfi;
	uint8_t rlc_mode;
	uint8_t ul_ts_bmap;

	uint8_t is_egprs_tbf;
	uint16_t egprs_win_size;
	uint8_t resegment;
	uint8_t bep_period2;
} __attribute__ ((packed));

/* LOG_EGPRS_MAC_DL_ACKNACK_C */
struct gprs_mac_dl_acknack {
	uint8_t is_egprs;
	uint8_t dl_acknack[0];
} __attribute__ ((packed));

/* LOG_EGPRS_MAC_UL_ACKNACK_C */
struct gprs_mac_ul_acknack {
	uint8_t ul_acknack[0];
} __attribute__ ((packed));

/* identical for LOG_GPRS_MAC_UL_TBF_RELEASE_C and
 * LOG_GPRS_MAC_DL_TBF_RELEASE_C */
struct gprs_mac_tbf_release {
	uint8_t tfi;
	uint8_t fail_cause;
} __attribute__ ((packed));


/* EVENT_GPRS_MAC_MSG_RECEIVED / EVENT_GPRS_MAC_MSG_SENT */
enum gprs_mac_chan_type {
	PRACH_11BIT_CHANNEL	= 0x01,
	PRACH_8BIT_CHANNEL	= 0x02,
	PACCH_RRBP_CHANNEL	= 0x03,
	UL_PACCH_CHANNEL	= 0x04,
	PCCCH_CHANNEL		= 0x81,
	PBCCH_CHANNEL		= 0x82,
	DL_PACCH_CHANNEL	= 0x83,
};

enum gprs_mac_msg_type {
	PACKET_CELL_CHANGE_FAILURE		= 0x00,
	PACKET_CONTROL_ACKNOWLEDGE		= 0x01,
	PACKET_DOWNLINK_ACK_NACK		= 0x02,
	PACKET_UPLK_DUMMY_CNTRL_BLK		= 0x03,
	PACKET_MEASUREMENT_REPORT		= 0x04,
	PACKET_RESOURCE_REQUEST			= 0x05,
	PACKET_MOBILE_TBF_STATUS		= 0x06,
	PACKET_PSI_STATUS			= 0x07,
	EGPRS_PACKET_DOWNLINK_ACK_NACK		= 0x08,
	PACKET_PAUSE				= 0x09,
	PACKET_ENHANCED_MEASUREMENT_REPORT	= 0x0a,
	ADDITIONAL_MS_RAC			= 0x0b,
	PACKET_CELL_CHANGE_NOTIFICATION		= 0x0c,
	PACKET_SI_STATUS			= 0x0d,
	PACKET_CS_REQUEST			= 0x0e,
	PACKET_CHANNEL_REQUEST			= 0x20,
};

struct evt_gprs_mac_msg {
	uint8_t msg_type;
	uint8_t prot_disc;
	uint8_t chan_type;
} __attribute__ ((packed));
