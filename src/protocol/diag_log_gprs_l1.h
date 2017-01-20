#pragma once

#include <stdint.h>

struct gprs_air_if_summary {
	uint32_t	fn;
	uint8_t		band_ind;
	uint8_t		dl_ts;
	uint8_t		ul_ts;
	uint16_t	rx_power;
	uint8_t		tx_power_ts[4];
	uint8_t		ta;
	uint32_t	rlc_dl_rx;
	uint32_t	rlc_ul_tx;
	uint32_t	rlc_ul_retx;
	uint32_t	llpdu_tx_sum;
	uint32_t	llpdu_retx;
	uint32_t	llpdu_rx_sum;
	uint32_t	llpdu_rx_err;
	uint32_t	user_oct_dl;
	uint32_t	user_oct_ul;
} __attribute__ ((packed));

struct gprs_rx_msg_metr_a_v2 {
	uint32_t	fn;
	uint8_t		tn;
	uint8_t		chan;
	uint8_t		cs;
	uint16_t	rx_qual;
	uint8_t 	_res:2,
			crc_passed:1,
			egprs_msg1_crc_passed:1,
			egprs_msg2_crc_passed:1,
			usf_match:1,
			dl_tfi_match:1;
	uint16_t 	msg_len;
	uint8_t		usf;
	/* EGPRS specifics below */
	uint8_t		ir_status;
	uint8_t		mod_detected;
	uint16_t	blk_mean_bep_tslot;
	uint16_t	blk_cv_bep_tslot;
	uint16_t	filtered_cv_bep_tslot;
	uint16_t	reliability;
} __attribute__ ((packed));

struct gprs_xfer_sum {
	uint8_t		band_ind;
	uint8_t		current_alloc;
	uint8_t		ul_ptcch_ts;
	uint8_t		dl_ptcch_ts;
	uint8_t		ta;
	uint8_t		usf_granularity;
	uint8_t		ul_bitmap_tn;
	uint8_t		dl_bitmap_tn;
} __attribute__ ((packed));

struct gprs_power_control {
	uint8_t		tx_channel;
	uint8_t		tx_tn;
	uint16_t	tx_pwr;
	uint8_t		cs;
	uint8_t		gamma_tn;
	uint16_t	gamma_band;
	uint8_t		alpha;
	uint8_t		derived_c;
	uint8_t		pmax;
} __attribute__ ((packed));

enum gprs_tx_ul_chan_t {
	DIAG_TX_UL_CH_T_PRACH,
	DIAG_TX_UL_CH_T_UL_PTCCH,
	DIAG_TX_UL_CH_T_UL_PACCH_PDTCH,
};
const struct value_string gprs_tx_ul_chans[4];

enum gprs_coding_scheme {
	DIAG_GPRS_CS_CS1,
	DIAG_GPRS_CS_CS2,
	DIAG_GPRS_CS_CS3,
	DIAG_GPRS_CS_CS4,
	DIAG_GPRS_CS_ACCESS,
	DIAG_GPRS_CS_EXT_ACCESS,
	DIAG_GPRS_CS_MCS1,
	DIAG_GPRS_CS_MCS2,
	DIAG_GPRS_CS_MCS3,
	DIAG_GPRS_CS_MCS4,
	DIAG_GPRS_CS_MCS5,
	DIAG_GPRS_CS_MCS6,
	DIAG_GPRS_CS_MCS7,
	DIAG_GPRS_CS_MCS8,
	DIAG_GPRS_CS_MCS9,
};
const struct value_string gprs_coding_schemes[16];

struct gprs_tx_sched_res {
	uint32_t	fn;
	uint8_t		num_msg_tx;
	uint16_t 	arfcn[4];
	struct {
		uint8_t		tn;
		uint8_t		chan_type;	/* gprs_tx_ul_chan_t */
		uint8_t		dyn_alloc;
		int16_t		power_idx;
		uint8_t		ta;
		uint8_t		cs;
	} msg_info[4];
} __attribute__ ((packed));
