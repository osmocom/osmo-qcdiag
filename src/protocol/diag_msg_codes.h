#pragma once

/* Taken from Qualcomm source marked as
 *
 * Copyright (c) 2002-2014 by Qualcomm Technologies, Inc.
 * All Rights Reserved.
 *
 * However, given that it is just a simple list of #defines, it is
 * fair to assume that it is actually not copyrihghtable.
 */

#define MSG_SSID_GEN_FIRST  0
  #define MSG_SSID_DFLT     0
  #define MSG_SSID_LEGACY   0
  #define MSG_SSID_AUDFMT   1
  #define MSG_SSID_AVS      2
  #define MSG_SSID_BOOT     3
  #define MSG_SSID_BT       4
  #define MSG_SSID_CM       5
  #define MSG_SSID_CMX      6
  #define MSG_SSID_DIAG     7
  #define MSG_SSID_DSM      8
  #define MSG_SSID_FS       9
  #define MSG_SSID_HS      10
  #define MSG_SSID_MDSP    11
  #define MSG_SSID_QDSP    12
  #define MSG_SSID_REX     13
  #define MSG_SSID_RF      14
  #define MSG_SSID_SD      15
  #define MSG_SSID_SIO     16
  #define MSG_SSID_VS      17
  #define MSG_SSID_WMS     18
  #define MSG_SSID_GPS     19
  #define MSG_SSID_MMOC    20
  #define MSG_SSID_RUIM    21
  #define MSG_SSID_TMC     22
  #define MSG_SSID_FTM     23
  #define MSG_SSID_MMGPS   24
  #define MSG_SSID_SLEEP   25
  #define MSG_SSID_SAM     26
  #define MSG_SSID_SRM     27
  #define MSG_SSID_SFAT    28
  #define MSG_SSID_JOYST   29
  #define MSG_SSID_MFLO    30
  #define MSG_SSID_DTV     31
  #define MSG_SSID_TCXOMGR 32
  #define MSG_SSID_EFS     33
  #define MSG_SSID_IRDA    34
  #define MSG_SSID_FM_RADIO 35
  #define MSG_SSID_AAM     36
  #define MSG_SSID_BM      37
  #define MSG_SSID_PE      38
  #define MSG_SSID_QIPCALL 39
  #define MSG_SSID_FLUTE   40
  #define MSG_SSID_CAMERA  41
  #define MSG_SSID_HSUSB   42
  #define MSG_SSID_FC      43
  #define MSG_SSID_USBHOST 44
  #define MSG_SSID_PROFILER 45
  #define MSG_SSID_MGP     46
  #define MSG_SSID_MGPME   47
  #define MSG_SSID_GPSOS   48
  #define MSG_SSID_MGPPE   49
  #define MSG_SSID_GPSSM   50
  #define MSG_SSID_IMS     51
  #define MSG_SSID_MBP_RF  52
  #define MSG_SSID_SNS     53
  #define MSG_SSID_WM      54
  #define MSG_SSID_LK      55
  #define MSG_SSID_PWRDB   56
  #define MSG_SSID_DCVS    57
  #define MSG_SSID_ANDROID_ADB    58
  #define MSG_SSID_VIDEO_ENCODER  59
  #define MSG_SSID_VENC_OMX       60
  #define MSG_SSID_GAN            61 /* Generic Access Network */
  #define MSG_SSID_KINETO_GAN     62
  #define MSG_SSID_ANDROID_QCRIL  63
  #define MSG_SSID_A2             64
  #define MSG_SSID_LINUX_DATA   65
  #define MSG_SSID_ECALL        66
  #define MSG_SSID_CHORD        67
  #define MSG_SSID_QCNE         68
  #define MSG_SSID_APPS_CAD_GENERAL 69
  #define MSG_SSID_OMADM     70 /* OMA device management */
  #define MSG_SSID_SIWA      71 /* Secure Instant Wireless Access */
  #define MSG_SSID_APR_MODEM 72 /* Audio Packet Router Modem */
  #define MSG_SSID_APR_APPS  73 /* Audio Packet Router Apps*/
  #define MSG_SSID_APR_ADSP  74 /* Audio Packet Router Adsp*/
  #define MSG_SSID_SRD_GENERAL 75
  #define MSG_SSID_ACDB_GENERAL 76
  #define MSG_SSID_DALTF  77  /* DAL Test Frame Work */
  #define MSG_SSID_CFM    78 /* Centralized Flow Control Manager */
  #define MSG_SSID_PMIC    79 /* PMIC SSID */
  #define MSG_SSID_GPS_SDP    80
  #define MSG_SSID_TLE    81
  #define MSG_SSID_TLE_XTM    82
  #define MSG_SSID_TLE_TLM    83
  #define MSG_SSID_TLE_TLM_MM 84
  #define MSG_SSID_WWAN_LOC   85
  #define MSG_SSID_GNSS_LOCMW 86
  #define MSG_SSID_QSET       87
  #define MSG_SSID_QBI        88
  #define MSG_SSID_ADC        89
  #define MSG_SSID_MMODE_QMI  90
  #define MSG_SSID_MCFG       91
  #define MSG_SSID_SSM        92
  #define MSG_SSID_MPOWER     93
  #define MSG_SSID_RMTS       94
  #define MSG_SSID_ADIE       95
  #define MSG_SSID_VT_VCEL    96
  #define MSG_SSID_FLASH_SCRUB 97
  #define MSG_SSID_STRIDE     98
  #define MSG_SSID_POLICYMAN  99
  #define MSG_SSID_TMS        100
  #define MSG_SSID_LWIP       101


  #define MSG_SSID_GEN_LAST   101


/* Messages arising from ONCRPC AMSS modules */
#define MSG_SSID_ONCRPC             500
#define MSG_SSID_ONCRPC_MISC_MODEM  501
#define MSG_SSID_ONCRPC_MISC_APPS   502
#define MSG_SSID_ONCRPC_CM_MODEM    503
#define MSG_SSID_ONCRPC_CM_APPS     504
#define MSG_SSID_ONCRPC_DB          505
#define MSG_SSID_ONCRPC_SND         506
#define MSG_SSID_ONCRPC_LAST        506

/* Default master category for 1X. */
#define MSG_SSID_1X             1000
  #define MSG_SSID_1X_ACP       1001
  #define MSG_SSID_1X_DCP       1002
  #define MSG_SSID_1X_DEC       1003
  #define MSG_SSID_1X_ENC       1004
  #define MSG_SSID_1X_GPSSRCH   1005
  #define MSG_SSID_1X_MUX       1006
  #define MSG_SSID_1X_SRCH      1007
#define MSG_SSID_1X_LAST        1007


/* Default master category for HDR. */
#define MSG_SSID_HDR_PROT      2000
  #define MSG_SSID_HDR_DATA    2001
  #define MSG_SSID_HDR_SRCH    2002
  #define MSG_SSID_HDR_DRIVERS 2003
  #define MSG_SSID_HDR_IS890   2004
  #define MSG_SSID_HDR_DEBUG   2005
  #define MSG_SSID_HDR_HIT     2006
  #define MSG_SSID_HDR_PCP     2007
  #define MSG_SSID_HDR_HEAPMEM 2008
#define MSG_SSID_HDR_LAST      2008


/* Default master category for UMTS. */
#define MSG_SSID_UMTS           3000
  #define MSG_SSID_WCDMA_L1     3001
  #define MSG_SSID_WCDMA_L2     3002
  #define MSG_SSID_WCDMA_MAC    3003
  #define MSG_SSID_WCDMA_RLC    3004
  #define MSG_SSID_WCDMA_RRC    3005
  #define MSG_SSID_NAS_CNM      3006
  #define MSG_SSID_NAS_MM       3007
  #define MSG_SSID_NAS_MN       3008
  #define MSG_SSID_NAS_RABM     3009
  #define MSG_SSID_NAS_REG      3010
  #define MSG_SSID_NAS_SM       3011
  #define MSG_SSID_NAS_TC       3012
  #define MSG_SSID_NAS_CB       3013
  #define MSG_SSID_WCDMA_LEVEL  3014
#define MSG_SSID_UMTS_LAST      3014


/* Default master category for GSM. */
#define MSG_SSID_GSM                4000
  #define MSG_SSID_GSM_L1           4001
  #define MSG_SSID_GSM_L2           4002
  #define MSG_SSID_GSM_RR           4003
  #define MSG_SSID_GSM_GPRS_GCOMMON 4004
  #define MSG_SSID_GSM_GPRS_GLLC    4005
  #define MSG_SSID_GSM_GPRS_GMAC    4006
  #define MSG_SSID_GSM_GPRS_GPL1    4007
  #define MSG_SSID_GSM_GPRS_GRLC    4008
  #define MSG_SSID_GSM_GPRS_GRR     4009
  #define MSG_SSID_GSM_GPRS_GSNDCP  4010
#define MSG_SSID_GSM_LAST           4010



#define MSG_SSID_WLAN           4500
  #define MSG_SSID_WLAN_ADP     4501
  #define MSG_SSID_WLAN_CP      4502
  #define MSG_SSID_WLAN_FTM     4503
  #define MSG_SSID_WLAN_OEM     4504
  #define MSG_SSID_WLAN_SEC     4505
  #define MSG_SSID_WLAN_TRP     4506
  #define MSG_SSID_WLAN_RESERVED_1  4507
  #define MSG_SSID_WLAN_RESERVED_2  4508
  #define MSG_SSID_WLAN_RESERVED_3  4509
  #define MSG_SSID_WLAN_RESERVED_4  4510
  #define MSG_SSID_WLAN_RESERVED_5  4511
  #define MSG_SSID_WLAN_RESERVED_6  4512
  #define MSG_SSID_WLAN_RESERVED_7  4513
  #define MSG_SSID_WLAN_RESERVED_8  4514
  #define MSG_SSID_WLAN_RESERVED_9  4515
  #define MSG_SSID_WLAN_RESERVED_10 4516
  #define MSG_SSID_WLAN_TL      4517
  #define MSG_SSID_WLAN_BAL     4518
  #define MSG_SSID_WLAN_SAL     4519
  #define MSG_SSID_WLAN_SSC     4520
  #define MSG_SSID_WLAN_HDD     4521
  #define MSG_SSID_WLAN_SME     4522
  #define MSG_SSID_WLAN_PE      4523
  #define MSG_SSID_WLAN_HAL     4524
  #define MSG_SSID_WLAN_SYS     4525
  #define MSG_SSID_WLAN_VOSS    4526
#define MSG_SSID_WLAN_LAST      4526


#define MSG_SSID_ATS            4600
  #define MSG_SSID_MSGR         4601
  #define MSG_SSID_APPMGR       4602
  #define MSG_SSID_QTF          4603
  #define MSG_SSID_FWS          4604
  #define MSG_SSID_SRCH4        4605
  #define MSG_SSID_CMAPI        4606
  #define MSG_SSID_MMAL         4607
  #define MSG_SSID_MCS_RESERVED_5 4608
  #define MSG_SSID_MCS_RESERVED_6 4609
  #define MSG_SSID_MCS_RESERVED_7 4610
  #define MSG_SSID_MCS_RESERVED_8 4611
  #define MSG_SSID_IRATMAN      4612
  #define MSG_SSID_CXM          4613
  #define MSG_SSID_VSTMR        4614
#define MSG_SSID_MCS_LAST     4614




/* Default master category for data services. */
#define MSG_SSID_DS             5000
  #define MSG_SSID_DS_RLP     5001
  #define MSG_SSID_DS_PPP     5002
  #define MSG_SSID_DS_TCPIP   5003
  #define MSG_SSID_DS_IS707   5004
  #define MSG_SSID_DS_3GMGR   5005
  #define MSG_SSID_DS_PS      5006
  #define MSG_SSID_DS_MIP     5007
  #define MSG_SSID_DS_UMTS    5008
  #define MSG_SSID_DS_GPRS    5009
  #define MSG_SSID_DS_GSM     5010
  #define MSG_SSID_DS_SOCKETS 5011
  #define MSG_SSID_DS_ATCOP   5012
  #define MSG_SSID_DS_SIO     5013
  #define MSG_SSID_DS_BCMCS   5014
  #define MSG_SSID_DS_MLRLP   5015
  #define MSG_SSID_DS_RTP     5016
  #define MSG_SSID_DS_SIPSTACK 5017
  #define MSG_SSID_DS_ROHC     5018
  #define MSG_SSID_DS_DOQOS    5019
  #define MSG_SSID_DS_IPC      5020
  #define MSG_SSID_DS_SHIM     5021
  #define MSG_SSID_DS_ACLPOLICY 5022
  #define MSG_SSID_DS_APPS     5023
  #define MSG_SSID_DS_MUX     5024
  #define MSG_SSID_DS_3GPP    5025
  #define MSG_SSID_DS_LTE     5026
  #define MSG_SSID_DS_WCDMA   5027
  #define MSG_SSID_DS_ACLPOLICY_APPS 5028 /* ACL POLICY */
  #define MSG_SSID_DS_HDR      5029
  #define MSG_SSID_DS_IPA      5030
  #define MSG_SSID_DS_EPC      5031

#define MSG_SSID_DS_LAST       5031


/* Default master category for Security. */
#define MSG_SSID_SEC                5500
#define MSG_SSID_SEC_CRYPTO         5501  /* Cryptography */
#define MSG_SSID_SEC_SSL            5502  /* Secure Sockets Layer */
#define MSG_SSID_SEC_IPSEC          5503  /* Internet Protocol Security */
#define MSG_SSID_SEC_SFS            5504  /* Secure File System */
#define MSG_SSID_SEC_TEST           5505  /* Security Test Subsystem */
#define MSG_SSID_SEC_CNTAGENT       5506  /* Content Agent Interface */
#define MSG_SSID_SEC_RIGHTSMGR      5507  /* Rights Manager Interface */
#define MSG_SSID_SEC_ROAP           5508  /* Rights Object Aquisition Protocol */
#define MSG_SSID_SEC_MEDIAMGR       5509  /* Media Manager Interface */
#define MSG_SSID_SEC_IDSTORE        5510  /* ID Store Interface */
#define MSG_SSID_SEC_IXFILE         5511  /* File interface */
#define MSG_SSID_SEC_IXSQL          5512  /* SQL interface */
#define MSG_SSID_SEC_IXCOMMON       5513  /* Common Interface */
#define MSG_SSID_SEC_BCASTCNTAGENT  5514  /* Broadcast Content Agent Interface */
#define MSG_SSID_SEC_PLAYREADY      5515  /* Broadcast Content Agent Interface */
#define MSG_SSID_SEC_WIDEVINE       5516  /* Broadcast Content Agent Interface */
#define MSG_SSID_SEC_LAST           5516


/* Default master category for applications. */
#define MSG_SSID_APPS                    6000
  #define MSG_SSID_APPS_APPMGR           6001
  #define MSG_SSID_APPS_UI               6002
  #define MSG_SSID_APPS_QTV              6003
  #define MSG_SSID_APPS_QVP              6004
  #define MSG_SSID_APPS_QVP_STATISTICS   6005
  #define MSG_SSID_APPS_QVP_VENCODER     6006
  #define MSG_SSID_APPS_QVP_MODEM        6007
  #define MSG_SSID_APPS_QVP_UI           6008
  #define MSG_SSID_APPS_QVP_STACK        6009
  #define MSG_SSID_APPS_QVP_VDECODER     6010
  #define MSG_SSID_APPS_ACM              6011
  #define MSG_SSID_APPS_HEAP_PROFILE     6012
  #define MSG_SSID_APPS_QTV_GENERAL      6013
  #define MSG_SSID_APPS_QTV_DEBUG        6014
  #define MSG_SSID_APPS_QTV_STATISTICS   6015
  #define MSG_SSID_APPS_QTV_UI_TASK      6016
  #define MSG_SSID_APPS_QTV_MP4_PLAYER   6017
  #define MSG_SSID_APPS_QTV_AUDIO_TASK   6018
  #define MSG_SSID_APPS_QTV_VIDEO_TASK   6019
  #define MSG_SSID_APPS_QTV_STREAMING    6020
  #define MSG_SSID_APPS_QTV_MPEG4_TASK   6021
  #define MSG_SSID_APPS_QTV_FILE_OPS     6022
  #define MSG_SSID_APPS_QTV_RTP          6023
  #define MSG_SSID_APPS_QTV_RTCP         6024
  #define MSG_SSID_APPS_QTV_RTSP         6025
  #define MSG_SSID_APPS_QTV_SDP_PARSE    6026
  #define MSG_SSID_APPS_QTV_ATOM_PARSE   6027
  #define MSG_SSID_APPS_QTV_TEXT_TASK    6028
  #define MSG_SSID_APPS_QTV_DEC_DSP_IF   6029
  #define MSG_SSID_APPS_QTV_STREAM_RECORDING 6030
  #define MSG_SSID_APPS_QTV_CONFIGURATION    6031
  #define MSG_SSID_APPS_QCAMERA              6032
  #define MSG_SSID_APPS_QCAMCORDER           6033
  #define MSG_SSID_APPS_BREW                 6034
  #define MSG_SSID_APPS_QDJ                  6035
  #define MSG_SSID_APPS_QDTX                 6036
  #define MSG_SSID_APPS_QTV_BCAST_FLO        6037
  #define MSG_SSID_APPS_MDP_GENERAL          6038
  #define MSG_SSID_APPS_PBM                  6039
  #define MSG_SSID_APPS_GRAPHICS_GENERAL     6040
  #define MSG_SSID_APPS_GRAPHICS_EGL         6041
  #define MSG_SSID_APPS_GRAPHICS_OPENGL      6042
  #define MSG_SSID_APPS_GRAPHICS_DIRECT3D    6043
  #define MSG_SSID_APPS_GRAPHICS_SVG         6044
  #define MSG_SSID_APPS_GRAPHICS_OPENVG      6045
  #define MSG_SSID_APPS_GRAPHICS_2D          6046
  #define MSG_SSID_APPS_GRAPHICS_QXPROFILER  6047
  #define MSG_SSID_APPS_GRAPHICS_DSP         6048
  #define MSG_SSID_APPS_GRAPHICS_GRP         6049
  #define MSG_SSID_APPS_GRAPHICS_MDP         6050
  #define MSG_SSID_APPS_CAD                  6051
  #define MSG_SSID_APPS_IMS_DPL              6052
  #define MSG_SSID_APPS_IMS_FW               6053
  #define MSG_SSID_APPS_IMS_SIP              6054
  #define MSG_SSID_APPS_IMS_REGMGR           6055
  #define MSG_SSID_APPS_IMS_RTP              6056
  #define MSG_SSID_APPS_IMS_SDP              6057
  #define MSG_SSID_APPS_IMS_VS               6058
  #define MSG_SSID_APPS_IMS_XDM              6059
  #define MSG_SSID_APPS_IMS_HOM              6060
  #define MSG_SSID_APPS_IMS_IM_ENABLER       6061
  #define MSG_SSID_APPS_IMS_IMS_CORE         6062
  #define MSG_SSID_APPS_IMS_FWAPI            6063
  #define MSG_SSID_APPS_IMS_SERVICES         6064
  #define MSG_SSID_APPS_IMS_POLICYMGR        6065
  #define MSG_SSID_APPS_IMS_PRESENCE         6066
  #define MSG_SSID_APPS_IMS_QIPCALL          6067
  #define MSG_SSID_APPS_IMS_SIGCOMP          6068
  #define MSG_SSID_APPS_IMS_PSVT             6069
  #define MSG_SSID_APPS_IMS_UNKNOWN          6070
  #define MSG_SSID_APPS_IMS_SETTINGS         6071
  #define MSG_SSID_OMX_COMMON                6072
  #define MSG_SSID_APPS_IMS_RCS_CD           6073
  #define MSG_SSID_APPS_IMS_RCS_IM           6074
  #define MSG_SSID_APPS_IMS_RCS_FT           6075
  #define MSG_SSID_APPS_IMS_RCS_IS           6076
  #define MSG_SSID_APPS_IMS_RCS_AUTO_CONFIG  6077
  #define MSG_SSID_APPS_IMS_RCS_COMMON       6078
  #define MSG_SSID_APPS_IMS_UT               6079
  #define MSG_SSID_APPS_IMS_XML              6080

#define MSG_SSID_APPS_LAST                   6080


/* Default master category for aDSP Tasks. */
#define MSG_SSID_ADSPTASKS                     6500
  #define MSG_SSID_ADSPTASKS_KERNEL            6501
  #define MSG_SSID_ADSPTASKS_AFETASK           6502
  #define MSG_SSID_ADSPTASKS_VOICEPROCTASK     6503
  #define MSG_SSID_ADSPTASKS_VOCDECTASK        6504
  #define MSG_SSID_ADSPTASKS_VOCENCTASK        6505
  #define MSG_SSID_ADSPTASKS_VIDEOTASK         6506
  #define MSG_SSID_ADSPTASKS_VFETASK           6507
  #define MSG_SSID_ADSPTASKS_VIDEOENCTASK      6508
  #define MSG_SSID_ADSPTASKS_JPEGTASK          6509
  #define MSG_SSID_ADSPTASKS_AUDPPTASK         6510
  #define MSG_SSID_ADSPTASKS_AUDPLAY0TASK      6511
  #define MSG_SSID_ADSPTASKS_AUDPLAY1TASK      6512
  #define MSG_SSID_ADSPTASKS_AUDPLAY2TASK      6513
  #define MSG_SSID_ADSPTASKS_AUDPLAY3TASK      6514
  #define MSG_SSID_ADSPTASKS_AUDPLAY4TASK      6515
  #define MSG_SSID_ADSPTASKS_LPMTASK           6516
  #define MSG_SSID_ADSPTASKS_DIAGTASK          6517
  #define MSG_SSID_ADSPTASKS_AUDRECTASK        6518
  #define MSG_SSID_ADSPTASKS_AUDPREPROCTASK    6519
  #define MSG_SSID_ADSPTASKS_MODMATHTASK       6520
  #define MSG_SSID_ADSPTASKS_GRAPHICSTASK      6521

#define MSG_SSID_ADSPTASKS_LAST                6521


/* Messages arising from Linux on L4, or its drivers or applications. */
#define MSG_SSID_L4LINUX_KERNEL          7000
#define MSG_SSID_L4LINUX_KEYPAD          7001
#define MSG_SSID_L4LINUX_APPS            7002
#define MSG_SSID_L4LINUX_QDDAEMON        7003
#define MSG_SSID_L4LINUX_LAST            MSG_SSID_L4LINUX_QDDAEMON

/* Messages arising from Iguana on L4, or its servers and drivers. */
#define MSG_SSID_L4IGUANA_IGUANASERVER   7100   /* Iguana Server itself */
#define MSG_SSID_L4IGUANA_EFS2           7101   /* platform/apps stuff */
#define MSG_SSID_L4IGUANA_QDMS           7102
#define MSG_SSID_L4IGUANA_REX            7103
#define MSG_SSID_L4IGUANA_SMMS           7104
#define MSG_SSID_L4IGUANA_FRAMEBUFFER    7105   /* platform/iguana stuff */
#define MSG_SSID_L4IGUANA_KEYPAD         7106
#define MSG_SSID_L4IGUANA_NAMING         7107
#define MSG_SSID_L4IGUANA_SDIO           7108
#define MSG_SSID_L4IGUANA_SERIAL         7109
#define MSG_SSID_L4IGUANA_TIMER          7110
#define MSG_SSID_L4IGUANA_TRAMP          7111
#define MSG_SSID_L4IGUANA_LAST           MSG_SSID_L4IGUANA_TRAMP

/* Messages arising from L4-specific AMSS modules */
#define MSG_SSID_L4AMSS_QDIAG            7200
#define MSG_SSID_L4AMSS_APS              7201
#define MSG_SSID_L4AMSS_LAST             MSG_SSID_L4AMSS_APS


/* Default master category for HIT. */
#define MSG_SSID_HIT         8000
#define MSG_SSID_HIT_LAST    8000


/* Default master category for Q6 */
#define MSG_SSID_QDSP6         8500
#define MSG_SSID_ADSP_AUD_SVC            8501  /* Audio Service */
#define MSG_SSID_ADSP_AUD_ENCDEC         8502  /* audio encoders/decoders */
#define MSG_SSID_ADSP_AUD_VOC            8503  /* voice encoders/decoders */
#define MSG_SSID_ADSP_AUD_VS             8504  /* voice services */
#define MSG_SSID_ADSP_AUD_MIDI           8505  /* MIDI-based file formats */
#define MSG_SSID_ADSP_AUD_POSTPROC       8506  /* e.g. Graph EQ, Spec Analyzer */
#define MSG_SSID_ADSP_AUD_PREPROC        8507  /* e.g. AGC-R */
#define MSG_SSID_ADSP_AUD_AFE            8508  /* audio front end */
#define MSG_SSID_ADSP_AUD_MSESSION       8509  /* media session */
#define MSG_SSID_ADSP_AUD_DSESSION       8510  /* device session */
#define MSG_SSID_ADSP_AUD_DCM            8511  /* device configuration */
#define MSG_SSID_ADSP_VID_ENC            8512  /* Video Encoder */
#define MSG_SSID_ADSP_VID_ENCRPC         8513  /* Video Encoder DAL driver */
#define MSG_SSID_ADSP_VID_DEC            8514  /* Video Decoder */
#define MSG_SSID_ADSP_VID_DECRPC         8515  /* Video Decoder DAL driver */
#define MSG_SSID_ADSP_VID_COMMONSW       8516  /* Video Common Software Units */
#define MSG_SSID_ADSP_VID_HWDRIVER       8517  /* Video Hardware */
#define MSG_SSID_ADSP_JPG_ENC            8518  /* JPEG Encoder */
#define MSG_SSID_ADSP_JPG_DEC            8519  /* JPEG Decoder */
#define MSG_SSID_ADSP_OMM                8520  /* openmm */
#define MSG_SSID_ADSP_PWRDEM             8521  /* Power or DEM messages */
#define MSG_SSID_ADSP_RESMGR             8522  /* Resource Manager */
#define MSG_SSID_ADSP_CORE               8523  /* General core (startup, heap stats, etc.) */
#define MSG_SSID_ADSP_RDA                8524
#define MSG_SSID_QDSP6_LAST              8524

/* Default master category for UMB. */
#define MSG_SSID_UMB         9000
#define MSG_SSID_UMB_APP     9001    /* UMB Application component */
#define MSG_SSID_UMB_DS      9002      /* UMB Data Services component */
#define MSG_SSID_UMB_CP      9003      /* UMB Call Processing component */
#define MSG_SSID_UMB_RLL     9004      /* UMB Radio Link Layer component */
#define MSG_SSID_UMB_MAC     9005    /* UMB MAC component */
#define MSG_SSID_UMB_SRCH    9006   /* UMB SRCH component */
#define MSG_SSID_UMB_FW      9007     /* UMB Firmware component */
#define MSG_SSID_UMB_PLT     9008     /* UMB PLT component */
#define MSG_SSID_UMB_LAST    9008

/* Default master category for LTE. */
#define MSG_SSID_LTE         9500
#define MSG_SSID_LTE_RRC       9501
#define MSG_SSID_LTE_MACUL     9502
#define MSG_SSID_LTE_MACDL     9503
#define MSG_SSID_LTE_MACCTRL   9504
#define MSG_SSID_LTE_RLCUL     9505
#define MSG_SSID_LTE_RLCDL     9506
#define MSG_SSID_LTE_PDCPUL    9507
#define MSG_SSID_LTE_PDCPDL    9508
#define MSG_SSID_LTE_ML1       9509
#define MSG_SSID_LTE_LAST      9509

/* Default master category for QCHAT */
#define MSG_SSID_QCHAT                                          10200
#define MSG_SSID_QCHAT_CAPP                                     10201
#define MSG_SSID_QCHAT_CENG                                     10202
#define MSG_SSID_QCHAT_CREG                                     10203
#define MSG_SSID_QCHAT_CMED                                     10204
#define MSG_SSID_QCHAT_CAUTH                                    10205
#define MSG_SSID_QCHAT_QBAL                                     10206
#define MSG_SSID_QCHAT_OSAL                                     10207
#define MSG_SSID_QCHAT_OEMCUST                                  10208
#define MSG_SSID_QCHAT_MULTI_PROC                               10209
#define MSG_SSID_QCHAT_UPK                                      10210
#define MSG_SSID_QCHAT_LAST                                     10210

/* Default master category for TDSCDMA */
#define MSG_SSID_TDSCDMA_L1                                     10251
#define MSG_SSID_TDSCDMA_L2                                     10252
#define MSG_SSID_TDSCDMA_MAC                                    10253
#define MSG_SSID_TDSCDMA_RLC                                    10254
#define MSG_SSID_TDSCDMA_RRC                                    10255
#define MSG_SSID_TDSCDMA_LAST                                   10255

/* Messages from the CTA framework */
#define MSG_SSID_CTA                                            10300
#define MSG_SSID_CTA_LAST                                       10300

/* QCNEA related SSIDs */
#define MSG_SSID_QCNEA                                          10350
#define MSG_SSID_QCNEA_CAC                                      10351
#define MSG_SSID_QCNEA_CORE                                     10352
#define MSG_SSID_QCNEA_CORE_CAS                                 10353
#define MSG_SSID_QCNEA_CORE_CDE                                 10354
#define MSG_SSID_QCNEA_CORE_COM                                 10355
#define MSG_SSID_QCNEA_CORE_LEE                                 10356
#define MSG_SSID_QCNEA_CORE_QMI                                 10357
#define MSG_SSID_QCNEA_CORE_SRM                                 10358
#define MSG_SSID_QCNEA_GENERIC                                  10359
#define MSG_SSID_QCNEA_NETLINK                                  10360
#define MSG_SSID_QCNEA_NIMS                                     10361
#define MSG_SSID_QCNEA_NSRM                                     10362
#define MSG_SSID_QCNEA_NSRM_CORE                                10363
#define MSG_SSID_QCNEA_NSRM_GATESM                              10364
#define MSG_SSID_QCNEA_NSRM_TRG                                 10365
#define MSG_SSID_QCNEA_PLCY                                     10366
#define MSG_SSID_QCNEA_PLCY_ANDSF                               10367
#define MSG_SSID_QCNEA_TEST                                     10368
#define MSG_SSID_QCNEA_WQE                                      10369
#define MSG_SSID_QCNEA_WQE_BQE                                  10370
#define MSG_SSID_QCNEA_WQE_CQE                                  10371
#define MSG_SSID_QCNEA_WQE_ICD                                  10372
#define MSG_SSID_QCNEA_WQE_IFSEL                                10373
#define MSG_SSID_QCNEA_WQE_IFSELRSM                             10374
#define MSG_SSID_QCNEA_ATP                                      10375
#define MSG_SSID_QCNEA_ATP_PLCY                                 10376
#define MSG_SSID_QCNEA_ATP_RPRT                                 10377

#define MSG_SSID_QCNEA_LAST                                     10377

/* These SSIDs are reserved for OEM (customer) use only. These IDs will
   never be used by Qualcomm. */
#define MSG_FIRST_TARGET_OEM_SSID (0xC000)
#define MSG_LAST_TARGET_OEM_SSID (0xCFFF)
