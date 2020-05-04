#include <stdio.h>

#include <osmocom/core/gsmtap.h>
#include <osmocom/core/gsmtap_util.h>
#include <osmocom/core/utils.h>

#include "diag_log.h"
#include "protocol/diag_log_lte.h"

struct sub_chan_map {
    uint8_t is_ul;
    uint8_t qc_id;
    uint8_t gsmtap_lte_rrc_msg;
};


static struct sub_chan_map rrc_subtype_map_lt9_13_22[] = {
    {0, 1, GSMTAP_LTE_RRC_SUB_BCCH_BCH_Message   },
    {0, 2, GSMTAP_LTE_RRC_SUB_BCCH_DL_SCH_Message},
    {0, 3, GSMTAP_LTE_RRC_SUB_MCCH_Message   },
    {0, 4, GSMTAP_LTE_RRC_SUB_PCCH_Message   },
    {0, 5, GSMTAP_LTE_RRC_SUB_DL_CCCH_Message     },
    {0, 6, GSMTAP_LTE_RRC_SUB_DL_DCCH_Message     },
    {1, 7, GSMTAP_LTE_RRC_SUB_UL_CCCH_Message     },
    {1, 8, GSMTAP_LTE_RRC_SUB_UL_DCCH_Message     },
};



static struct sub_chan_map rrc_subtype_map_9to12[] = {
	{0,8, GSMTAP_LTE_RRC_SUB_BCCH_BCH_Message      },
	{0,9, GSMTAP_LTE_RRC_SUB_BCCH_DL_SCH_Message   },
	{0,10, GSMTAP_LTE_RRC_SUB_MCCH_Message	      },
	{0,11, GSMTAP_LTE_RRC_SUB_PCCH_Message	      },
	{0,12, GSMTAP_LTE_RRC_SUB_DL_CCCH_Message      },
	{0,13, GSMTAP_LTE_RRC_SUB_DL_DCCH_Message      },
	{1,14, GSMTAP_LTE_RRC_SUB_UL_CCCH_Message      },
	{1,15, GSMTAP_LTE_RRC_SUB_UL_DCCH_Message      },
    };

static struct sub_chan_map rrc_subtype_map_14_15_16[] = {
	{0,1, GSMTAP_LTE_RRC_SUB_BCCH_BCH_Message       },
	{0,2, GSMTAP_LTE_RRC_SUB_BCCH_DL_SCH_Message    },
	{0,4, GSMTAP_LTE_RRC_SUB_MCCH_Message	       },
	{0,5, GSMTAP_LTE_RRC_SUB_PCCH_Message	       },
	{0,6, GSMTAP_LTE_RRC_SUB_DL_CCCH_Message       },
	{0,7, GSMTAP_LTE_RRC_SUB_DL_DCCH_Message       },
	{1,8, GSMTAP_LTE_RRC_SUB_UL_CCCH_Message       },
	{1,9, GSMTAP_LTE_RRC_SUB_UL_DCCH_Message       },
    };


static struct sub_chan_map rrc_subtype_map_19_26[] = {
	{0,1, GSMTAP_LTE_RRC_SUB_BCCH_BCH_Message	},
	{0,3, GSMTAP_LTE_RRC_SUB_BCCH_DL_SCH_Message	},
	{0,6, GSMTAP_LTE_RRC_SUB_MCCH_Message		},
	{0,7, GSMTAP_LTE_RRC_SUB_PCCH_Message		},
	{0,8, GSMTAP_LTE_RRC_SUB_DL_CCCH_Message	},
	{0,9, GSMTAP_LTE_RRC_SUB_DL_DCCH_Message	},
	{1,10,GSMTAP_LTE_RRC_SUB_UL_CCCH_Message	},
	{1,11,GSMTAP_LTE_RRC_SUB_UL_DCCH_Message	},
	{0,45, GSMTAP_LTE_RRC_SUB_BCCH_BCH_Message_NB	},
	{0,46, GSMTAP_LTE_RRC_SUB_BCCH_BCH_Message_NB	},
	{0,47, GSMTAP_LTE_RRC_SUB_PCCH_Message_NB	},
	{0,48, GSMTAP_LTE_RRC_SUB_DL_CCCH_Message_NB	},
	{0,49, GSMTAP_LTE_RRC_SUB_DL_DCCH_Message_NB	},
	{1,50, GSMTAP_LTE_RRC_SUB_UL_CCCH_Message_NB	},
	{1,52, GSMTAP_LTE_RRC_SUB_UL_DCCH_Message_NB	},
    };

static struct sub_chan_map rrc_subtype_map_20[] = {
	{0,1, GSMTAP_LTE_RRC_SUB_BCCH_BCH_Message ,	   },
	{0,2, GSMTAP_LTE_RRC_SUB_BCCH_DL_SCH_Message,	   },
	{0,4, GSMTAP_LTE_RRC_SUB_MCCH_Message,		   },
	{0,5, GSMTAP_LTE_RRC_SUB_PCCH_Message,		   },
	{0,6, GSMTAP_LTE_RRC_SUB_DL_CCCH_Message,	   },
	{0,7, GSMTAP_LTE_RRC_SUB_DL_DCCH_Message,	   },
	{1,8, GSMTAP_LTE_RRC_SUB_UL_CCCH_Message,	   },
	{1,9, GSMTAP_LTE_RRC_SUB_UL_DCCH_Message	   },
	{0,54, GSMTAP_LTE_RRC_SUB_BCCH_BCH_Message_NB,	   },
	{0,55, GSMTAP_LTE_RRC_SUB_BCCH_BCH_Message_NB,	   },
	{0,56, GSMTAP_LTE_RRC_SUB_PCCH_Message_NB,	   },
	{0,57, GSMTAP_LTE_RRC_SUB_DL_CCCH_Message_NB,	   },
	{0,58, GSMTAP_LTE_RRC_SUB_DL_DCCH_Message_NB,	   },
	{1,59, GSMTAP_LTE_RRC_SUB_UL_CCCH_Message_NB,	   },
	{1,61, GSMTAP_LTE_RRC_SUB_UL_DCCH_Message_NB	   },
    };

struct chan_map_result {
    uint8_t is_ul;
    uint8_t chan;
};

struct lte_cell_info_dirty_012 {
    uint16_t band;
    uint16_t dl_arfcn;
    uint16_t ul_arfcn;
    /* don't care about anything else */
};

struct lte_cell_info_dirty_2plus {
    uint16_t band;
    uint32_t dl_arfcn;
    uint32_t ul_arfcn;
    /* don't care about anything else */
};

#define MAPC(xx)	\
for(i=0; i < ARRAY_SIZE(xx); i++) {\
if(inout->chan == xx[i].qc_id){ \
	inout->chan = xx[i].gsmtap_lte_rrc_msg; \
	inout->is_ul = xx[i].is_ul; \
	return 0; \
    } \
    }


static int find_map_in_ver(int version, struct chan_map_result* inout) {
    unsigned int i = 0;
    if(version < 9 || version == 13 || version == 22)
	 MAPC(rrc_subtype_map_lt9_13_22)
    else if(version >= 9 && version <= 12)
	 MAPC(rrc_subtype_map_9to12)
    else if(version >= 14 && version <= 16)
	 MAPC(rrc_subtype_map_14_15_16)
    else if(version == 19 || version == 26)
	 MAPC(rrc_subtype_map_19_26)
    else if(version  == 20)
	 MAPC(rrc_subtype_map_20)
    else
	 return -1;

    return 0;

}

static void handle_lte_rrc_ota_msg(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	uint16_t arfcn = 0;
	uint8_t* data;
	struct chan_map_result mc;
	struct diag_lte_rrc_ota_base_8plus_msg *r8p;
	struct diag_lte_rrc_ota_base_msg *rrm = (struct diag_lte_rrc_ota_base_msg *) msgb_data(msg);

	if (rrm->ext_ver < 8 || rrm->ext_ver > 24) {
		printf("Unhandled LTE OTA rel: %u\n", rrm->ext_ver);
		return;
	}
	r8p = (struct diag_lte_rrc_ota_base_8plus_msg *)msgb_data(msg);
	data = msgb_data(msg) + sizeof(struct diag_lte_rrc_ota_base_8plus_msg);

	if (msgb_length(msg) - sizeof(struct diag_lte_rrc_ota_base_8plus_msg) != r8p->len) {
		printf("Unhandled LTE OTA rel len: %u %u\n", rrm->ext_ver, r8p->len);
		return;
	}

	switch (r8p->channel_type) {
	case 254:
	case 255:
		return;
	}

	mc.chan = r8p->channel_type;
	if (find_map_in_ver(r8p->hdr.ext_ver, &mc) < 0)
	    printf("Unhandled LTE OTA rel len: %u %u\n", rrm->ext_ver, r8p->len);

	arfcn = mc.is_ul ? di->lte_arfcn_ul : di->lte_arfcn_dl;
	arfcn = mc.is_ul ? arfcn | GSMTAP_ARFCN_F_UPLINK: arfcn;

//	printf("lte m: %s\n", osmo_hexdump(data, 1));


	if (di->gsmtap && di->flags & DIAG_INST_F_GSMTAP_DECODED) {
		gsmtap_send_ex(di->gsmtap, GSMTAP_TYPE_LTE_RRC, arfcn, 0, mc.chan, 0, 0, 0, 0, data ,r8p->len);
	}
}

static void handle_lte_rrc_cell_info(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{

	uint8_t* data;

	struct diag_lte_rrc_ota_base_8plus_msg *r8p;
	struct diag_lte_rrc_ota_base_msg *rrm = (struct diag_lte_rrc_ota_base_msg *) msgb_data(msg);

	uint8_t ver = *(uint8_t *) msgb_data(msg);
	if(ver < 3) {
		struct lte_cell_info_dirty_012* i = (struct lte_cell_info_dirty_012 *) msgb_data(msg);
		di->lte_arfcn_dl = i->dl_arfcn;
		di->lte_arfcn_ul = i->ul_arfcn;
	} else {
		struct lte_cell_info_dirty_2plus* i = (struct lte_cell_info_dirty_2plus *) msgb_data(msg);
		di->lte_arfcn_dl = i->dl_arfcn;
		di->lte_arfcn_ul = i->ul_arfcn;
	}

}

static void handle_lte_nas_msg(struct diag_instance *di, struct log_hdr *lh, struct msgb *msg)
{
	uint16_t arfcn = 0;
	uint8_t* data;
	uint32_t len;
	bool sec_hdr = false;
	bool is_ul = false;

	data = msgb_data(msg) + sizeof(struct diag_lte_rrc_nas_msg);
	len = msgb_length(msg) - sizeof(struct diag_lte_rrc_nas_msg);

	switch(lh->code) {
		case LTE(LOG_LTE_NAS_ESM_OTA_IN_SEC_MSG_LOG_C):
		case LTE(LOG_LTE_NAS_ESM_OTA_OUT_SEC_MSG_LOG_C):
		case LTE(LOG_LTE_NAS_EMM_OTA_IN_SEC_MSG_LOG_C):
		case LTE(LOG_LTE_NAS_EMM_OTA_OUT_SEC_MSG_LOG_C):
			sec_hdr = true;
			break;
	}

	switch(lh->code) {
		case LTE(LOG_LTE_NAS_ESM_OTA_IN_MSG_LOG_C):
		case LTE(LOG_LTE_NAS_EMM_OTA_IN_MSG_LOG_C):
		case LTE(LOG_LTE_NAS_ESM_OTA_IN_SEC_MSG_LOG_C):
		case LTE(LOG_LTE_NAS_EMM_OTA_IN_SEC_MSG_LOG_C):
			is_ul = true;
			break;
		}

	arfcn = is_ul ? di->lte_arfcn_ul : di->lte_arfcn_dl;
	if (di->gsmtap && di->flags & DIAG_INST_F_GSMTAP_DECODED) {
		gsmtap_send_ex(di->gsmtap, GSMTAP_TYPE_LTE_NAS, is_ul ? arfcn | GSMTAP_ARFCN_F_UPLINK : arfcn, 0, sec_hdr ? GSMTAP_LTE_NAS_SEC_HEADER : GSMTAP_LTE_NAS_PLAIN, 0, 0, 0, 0, data , len);
	}
}

static const struct diag_log_dispatch_tbl log_tbl[] = {
	{ LTE(LOG_LTE_RRC_OTA_MSG_LOG_C), handle_lte_rrc_ota_msg },
	{ LTE(LOG_LTE_NAS_ESM_OTA_IN_MSG_LOG_C), handle_lte_nas_msg },
	{ LTE(LOG_LTE_NAS_ESM_OTA_OUT_MSG_LOG_C), handle_lte_nas_msg },
	{ LTE(LOG_LTE_NAS_EMM_OTA_IN_MSG_LOG_C), handle_lte_nas_msg },
	{ LTE(LOG_LTE_NAS_EMM_OTA_OUT_MSG_LOG_C), handle_lte_nas_msg },
//	{ LTE(LOG_LTE_NAS_ESM_OTA_IN_SEC_MSG_LOG_C), handle_lte_nas_msg },
//	{ LTE(LOG_LTE_NAS_ESM_OTA_OUT_SEC_MSG_LOG_C), handle_lte_nas_msg },
//	{ LTE(LOG_LTE_NAS_EMM_OTA_IN_SEC_MSG_LOG_C), handle_lte_nas_msg },
//	{ LTE(LOG_LTE_NAS_EMM_OTA_OUT_SEC_MSG_LOG_C), handle_lte_nas_msg },
	{ LTE(LOG_LTE_RRC_SERV_CELL_INFO_LOG_C), handle_lte_rrc_cell_info },
};

static __attribute__((constructor)) void on_dso_load_lte(void)
{
	diag_log_reg_dispatch(log_tbl, ARRAY_SIZE(log_tbl));
}
