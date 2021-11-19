#pragma once

#include <stdint.h>

/* LOG_GPRS_RLC_UL_ABNRML_RLS_COUNTS_C */
struct gprs_rlc_ul_abnrml_rls_counts {
	uint16_t access_reject_cnt;
	uint16_t arc_retry_cnt;
	uint16_t arc_wo_retry_cnt;
	uint16_t arc_sys_info_cnt;
} __attribute__ ((packed));

/* LOG_GPRS_RLC_UL_EVENT_COUNTS_C */
struct gprs_rlc_ul_event_counts {
	uint16_t llc_event_cnt;
	uint16_t mac_event_cnt;
	uint16_t pl1_event_cnt;
	uint16_t tmr_event_cnt;
} __attribute__ ((packed));

/* LOG_GPRS_RLC_UL_STATS_C */
enum gprs_rlc_ul_state {
	US_NULL,
	US_AWAIT_ASS,
	US_ACK_XFER,
	US_GTTP_XFER, // optional
	US_UNACK_XFER,
	US_TEST_MODE,
	US_AWAIT_RECOVERY,
};
extern struct value_string gprs_rlc_ul_state_vals[];

enum gprs_rlc_ul_substate {
	RALS_NULL,
	RALS_AWAIT_TX_COMPLETE,
	RALS_CURRENT_PDU_TX_COMPLETED,
	RALS_COMPLETE,
};
extern struct value_string gprs_rlc_ul_substate_vals[];

enum gprs_rlc_dl_state {
	DS_NULL,
	DS_RX_ACK,
	DS_RX_UNACK,
	DS_RX_ACK_PENDING_RELEASE,
	DS_RX_UNACK_PENDING_RELEASE,
	DS_TEST_MODE,
};
extern struct value_string gprs_rlc_dl_state_vals[];

/* LOG_GPRS_RLC_UL_STATS_C */
struct gprs_rlc_ul_stats {
	uint8_t	rlc_ul_state;
	uint8_t rlc_ul_substate;
	uint32_t ack_tx_pdu_cnt;
	uint32_t ack_tx_oct_cnt;
	uint32_t unack_tx_pdu_cnt;
	uint32_t unack_tx_oct_cnt;
	uint16_t coding_sch_changes;
	uint8_t ul_seq_va;
	uint8_t ul_seq_vs;
	uint8_t ul_seq_nseg;
	uint8_t ul_seq_vsf;
	uint8_t ul_seq_pack_vsf;
	uint8_t ul_seq_nack_vsf;
	int8_t ul_n3102;
	int8_t ul_n3104;
	uint32_t ul_gen_p_cnt_a;
	uint32_t ul_gen_p_cnt_b;
	uint32_t ul_gen_p_cnt_c;
} __attribute__ ((packed));

/* LOG_GPRS_RLC_DL_STATS_C */
struct gprs_rlc_dl_stats {
	uint8_t	rlc_dl_state;
	uint32_t ack_rx_pdu_cnt;
	uint32_t ack_rx_oct_cnt;
	uint32_t unack_rx_pdu_cnt;
	uint32_t unack_pdu_filled_cnt;
	uint32_t unack_rx_oct_cnt;
	uint32_t unack_oct_filled_cnt;
	uint32_t ack_nack_req_cnt;
	uint8_t dl_seq_vq;
	uint8_t dl_seq_vr;
	uint8_t dl_seq_ssn;
	uint32_t dl_gen_p_cnt_a;
	uint32_t dl_gen_p_cnt_b;
	uint32_t dl_gen_p_cnt_c;
} __attribute__ ((packed));

/* LOG_GPRS_RLC_UL_ACKNACK_PARAMS_VER2_C */
struct gprs_rlc_ul_acknack_params_v2 {
	uint8_t ul_tfi;
	uint8_t final_ack_ind;
	uint8_t start_seq_nr;
	uint8_t coding_scheme;
	uint8_t countdown_val;
	uint8_t va;
	uint8_t vs;
	uint8_t stall_ind;
	uint32_t rrb_high32;
	uint32_t rrb_low32;
} __attribute__ ((packed));

/* LOG_GPRS_RLC_DL_ACKNACK_PARAMS_VER2_C */
struct gprs_rlc_dl_acknack_params_v2 {
	uint8_t dl_tfi;
	uint8_t final_ack_ind;
	uint8_t start_seq_nr;
	uint8_t vq;
	uint32_t rrb_high32;
	uint32_t rrb_low32;
	uint8_t coding_scheme;
} __attribute__ ((packed));

/* LOG_GPRS_RLC_UL_RELEASE_IND_C + LOG_GPRS_RLC_DL_RELEASE_IND_C */
struct gprs_rlc_release_ind {
	uint8_t tfi;
	uint8_t cause;
} __attribute__ ((packed));

enum gprs_rlc_hdr_type {
	GPRS_HEADER_TYPE,
	EGPRS_HEADER_TYPE_1,
	EGPRS_HEADER_TYPE_2,
	EGPRS_HEADER_TYPE_3
};
extern struct value_string gprs_rlc_hdr_type_vals[];


/* LOG_EGPRS_RLC_DL_HEADER_C */
struct gprs_rlc_dl_header {
	uint8_t type;
	uint8_t ul_hdr[5];
	uint8_t crc_status;
	uint8_t ts;
	uint32_t fn;
	uint8_t dl_payload[6];
} __attribute__ ((packed));

/* LOG_EGPRS_RLC_UL_HEADER_C */
struct gprs_rlc_ul_header {
	uint8_t type;
	uint8_t ul_hdr[6];
} __attribute__ ((packed));

/* LOG_EGPRS_RLC_EPUAN_C */
struct diag_egprs_rlc_epuan {
	uint8_t tfi;
	uint8_t msc;
	uint8_t final_ack_ind:1,
		begin_of_window:1,
		end_of_window:1,
		resegment_ir_off:1,
		preemptive_tx:1,
		starting_color_code:1,
		_pad:2;
	uint16_t ssn;
	uint8_t crrb_num_bits;
	uint8_t crrb[16];
	uint8_t urrb_num_bits;
	uint8_t urrb[20];
} __attribute__ ((packed));

/* LOG_EGPRS_RLC_EPDAN_C */
struct diag_egprs_rlc_epdan {
	uint8_t tfi;
	uint8_t final_ack_ind:1,
		begin_of_window:1,
		end_of_window:1,
		esp:2,
		starting_color_code:1,
		gmsk_valid:1,
		psk_valid:1;
	uint16_t ssn;
	uint8_t crrb_num_bits;
	uint8_t crrb[16];
	uint8_t urrb_num_bits;
	uint8_t urrb[20];
	uint8_t gmsk_bep;
	uint8_t psk_bep;
	uint8_t c_value;
} __attribute__ ((packed));
