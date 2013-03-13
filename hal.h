/*
 * Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _HAL_H_
#define _HAL_H_

#include "hw.h"

/*---------------------------------------------------------------------------
  API VERSIONING INFORMATION

  The RIVA API is versioned as MAJOR.MINOR.VERSION.REVISION
  The MAJOR is incremented for major product/architecture changes
      (and then MINOR/VERSION/REVISION are zeroed)
  The MINOR is incremented for minor product/architecture changes
      (and then VERSION/REVISION are zeroed)
  The VERSION is incremented if a significant API change occurs
      (and then REVISION is zeroed)
  The REVISION is incremented if an insignificant API change occurs
      or if a new API is added
  All values are in the range 0..255 (ie they are 8-bit values)
 ---------------------------------------------------------------------------*/
#define WCN36XX_HAL_VER_MAJOR 1
#define WCN36XX_HAL_VER_MINOR 4
#define WCN36XX_HAL_VER_VERSION 1
#define WCN36XX_HAL_VER_REVISION 2

/* This is to force compiler to use the maximum of an int ( 4 bytes ) */
#define WCN36XX_HAL_MAX_ENUM_SIZE    0x7FFFFFFF
#define WCN36XX_HAL_MSG_TYPE_MAX_ENUM_SIZE    0x7FFF

/* Max no. of transmit categories */
#define STACFG_MAX_TC    8

/* The maximum value of access category */
#define WCN36XX_HAL_MAX_AC  4

typedef u8 tHalIpv4Addr[4];

#define HAL_MAC_ADDR_LEN        6
#define HAL_IPV4_ADDR_LEN       4

#define WALN_HAL_STA_INVALID_IDX 0xFF
#define WCN36XX_HAL_BSS_INVALID_IDX 0xFF

/* Default Beacon template size */
#define BEACON_TEMPLATE_SIZE 0x180

/* Param Change Bitmap sent to HAL */
#define PARAM_BCN_INTERVAL_CHANGED                      (1 << 0)
#define PARAM_SHORT_PREAMBLE_CHANGED                 (1 << 1)
#define PARAM_SHORT_SLOT_TIME_CHANGED                 (1 << 2)
#define PARAM_llACOEXIST_CHANGED                            (1 << 3)
#define PARAM_llBCOEXIST_CHANGED                            (1 << 4)
#define PARAM_llGCOEXIST_CHANGED                            (1 << 5)
#define PARAM_HT20MHZCOEXIST_CHANGED                  (1<<6)
#define PARAM_NON_GF_DEVICES_PRESENT_CHANGED (1<<7)
#define PARAM_RIFS_MODE_CHANGED                            (1<<8)
#define PARAM_LSIG_TXOP_FULL_SUPPORT_CHANGED   (1<<9)
#define PARAM_OBSS_MODE_CHANGED                               (1<<10)
#define PARAM_BEACON_UPDATE_MASK \
	(PARAM_BCN_INTERVAL_CHANGED |					\
	 PARAM_SHORT_PREAMBLE_CHANGED |					\
	 PARAM_SHORT_SLOT_TIME_CHANGED |				\
	 PARAM_llACOEXIST_CHANGED |					\
	 PARAM_llBCOEXIST_CHANGED |					\
	 PARAM_llGCOEXIST_CHANGED |					\
	 PARAM_HT20MHZCOEXIST_CHANGED |					\
	 PARAM_NON_GF_DEVICES_PRESENT_CHANGED |				\
	 PARAM_RIFS_MODE_CHANGED |					\
	 PARAM_LSIG_TXOP_FULL_SUPPORT_CHANGED |				\
	 PARAM_OBSS_MODE_CHANGED)

/* dump command response Buffer size */
#define DUMPCMD_RSP_BUFFER 100

/* version string max length (including NULL) */
#define WCN36XX_HAL_VERSION_LENGTH  64

/* message types for messages exchanged between WDI and HAL */
enum hal_host_msg_type {
	/* Init/De-Init */
	WCN36XX_HAL_START_REQ = 0,
	WCN36XX_HAL_START_RSP = 1,
	WCN36XX_HAL_STOP_REQ = 2,
	WCN36XX_HAL_STOP_RSP = 3,

	/* Scan */
	WCN36XX_HAL_INIT_SCAN_REQ = 4,
	WCN36XX_HAL_INIT_SCAN_RSP = 5,
	WCN36XX_HAL_START_SCAN_REQ = 6,
	WCN36XX_HAL_START_SCAN_RSP = 7,
	WCN36XX_HAL_END_SCAN_REQ = 8,
	WCN36XX_HAL_END_SCAN_RSP = 9,
	WCN36XX_HAL_FINISH_SCAN_REQ = 10,
	WCN36XX_HAL_FINISH_SCAN_RSP = 11,

	/* HW STA configuration/deconfiguration */
	WCN36XX_HAL_CONFIG_STA_REQ = 12,
	WCN36XX_HAL_CONFIG_STA_RSP = 13,
	WCN36XX_HAL_DELETE_STA_REQ = 14,
	WCN36XX_HAL_DELETE_STA_RSP = 15,
	WCN36XX_HAL_CONFIG_BSS_REQ = 16,
	WCN36XX_HAL_CONFIG_BSS_RSP = 17,
	WCN36XX_HAL_DELETE_BSS_REQ = 18,
	WCN36XX_HAL_DELETE_BSS_RSP = 19,

	/* Infra STA asscoiation */
	WCN36XX_HAL_JOIN_REQ = 20,
	WCN36XX_HAL_JOIN_RSP = 21,
	WCN36XX_HAL_POST_ASSOC_REQ = 22,
	WCN36XX_HAL_POST_ASSOC_RSP = 23,

	/* Security */
	WCN36XX_HAL_SET_BSSKEY_REQ = 24,
	WCN36XX_HAL_SET_BSSKEY_RSP = 25,
	WCN36XX_HAL_SET_STAKEY_REQ = 26,
	WCN36XX_HAL_SET_STAKEY_RSP = 27,
	WCN36XX_HAL_RMV_BSSKEY_REQ = 28,
	WCN36XX_HAL_RMV_BSSKEY_RSP = 29,
	WCN36XX_HAL_RMV_STAKEY_REQ = 30,
	WCN36XX_HAL_RMV_STAKEY_RSP = 31,

	/* Qos Related */
	WCN36XX_HAL_ADD_TS_REQ = 32,
	WCN36XX_HAL_ADD_TS_RSP = 33,
	WCN36XX_HAL_DEL_TS_REQ = 34,
	WCN36XX_HAL_DEL_TS_RSP = 35,
	WCN36XX_HAL_UPD_EDCA_PARAMS_REQ = 36,
	WCN36XX_HAL_UPD_EDCA_PARAMS_RSP = 37,
	WCN36XX_HAL_ADD_BA_REQ = 38,
	WCN36XX_HAL_ADD_BA_RSP = 39,
	WCN36XX_HAL_DEL_BA_REQ = 40,
	WCN36XX_HAL_DEL_BA_RSP = 41,

	WCN36XX_HAL_CH_SWITCH_REQ = 42,
	WCN36XX_HAL_CH_SWITCH_RSP = 43,
	WCN36XX_HAL_SET_LINK_ST_REQ = 44,
	WCN36XX_HAL_SET_LINK_ST_RSP = 45,
	WCN36XX_HAL_GET_STATS_REQ = 46,
	WCN36XX_HAL_GET_STATS_RSP = 47,
	WCN36XX_HAL_UPDATE_CFG_REQ = 48,
	WCN36XX_HAL_UPDATE_CFG_RSP = 49,

	WCN36XX_HAL_MISSED_BEACON_IND = 50,
	WCN36XX_HAL_UNKNOWN_ADDR2_FRAME_RX_IND = 51,
	WCN36XX_HAL_MIC_FAILURE_IND = 52,
	WCN36XX_HAL_FATAL_ERROR_IND = 53,
	WCN36XX_HAL_SET_KEYDONE_MSG = 54,

	/* NV Interface */
	WCN36XX_HAL_DOWNLOAD_NV_REQ = 55,
	WCN36XX_HAL_DOWNLOAD_NV_RSP = 56,

	WCN36XX_HAL_ADD_BA_SESSION_REQ = 57,
	WCN36XX_HAL_ADD_BA_SESSION_RSP = 58,
	WCN36XX_HAL_TRIGGER_BA_REQ = 59,
	WCN36XX_HAL_TRIGGER_BA_RSP = 60,
	WCN36XX_HAL_UPDATE_BEACON_REQ = 61,
	WCN36XX_HAL_UPDATE_BEACON_RSP = 62,
	WCN36XX_HAL_SEND_BEACON_REQ = 63,
	WCN36XX_HAL_SEND_BEACON_RSP = 64,

	WCN36XX_HAL_SET_BCASTKEY_REQ = 65,
	WCN36XX_HAL_SET_BCASTKEY_RSP = 66,
	WCN36XX_HAL_DELETE_STA_CONTEXT_IND = 67,
	WCN36XX_HAL_UPDATE_PROBE_RSP_TEMPLATE_REQ = 68,
	WCN36XX_HAL_UPDATE_PROBE_RSP_TEMPLATE_RSP = 69,

	/* PTT interface support */
	WCN36XX_HAL_PROCESS_PTT_REQ = 70,
	WCN36XX_HAL_PROCESS_PTT_RSP = 71,

	/* BTAMP related events */
	WCN36XX_HAL_SIGNAL_BTAMP_EVENT_REQ = 72,
	WCN36XX_HAL_SIGNAL_BTAMP_EVENT_RSP = 73,
	WCN36XX_HAL_TL_HAL_FLUSH_AC_REQ = 74,
	WCN36XX_HAL_TL_HAL_FLUSH_AC_RSP = 75,

	WCN36XX_HAL_ENTER_IMPS_REQ = 76,
	WCN36XX_HAL_EXIT_IMPS_REQ = 77,
	WCN36XX_HAL_ENTER_BMPS_REQ = 78,
	WCN36XX_HAL_EXIT_BMPS_REQ = 79,
	WCN36XX_HAL_ENTER_UAPSD_REQ = 80,
	WCN36XX_HAL_EXIT_UAPSD_REQ = 81,
	WCN36XX_HAL_UPDATE_UAPSD_PARAM_REQ = 82,
	WCN36XX_HAL_CONFIGURE_RXP_FILTER_REQ = 83,
	WCN36XX_HAL_ADD_BCN_FILTER_REQ = 84,
	WCN36XX_HAL_REM_BCN_FILTER_REQ = 85,
	WCN36XX_HAL_ADD_WOWL_BCAST_PTRN = 86,
	WCN36XX_HAL_DEL_WOWL_BCAST_PTRN = 87,
	WCN36XX_HAL_ENTER_WOWL_REQ = 88,
	WCN36XX_HAL_EXIT_WOWL_REQ = 89,
	WCN36XX_HAL_HOST_OFFLOAD_REQ = 90,
	WCN36XX_HAL_SET_RSSI_THRESH_REQ = 91,
	WCN36XX_HAL_GET_RSSI_REQ = 92,
	WCN36XX_HAL_SET_UAPSD_AC_PARAMS_REQ = 93,
	WCN36XX_HAL_CONFIGURE_APPS_CPU_WAKEUP_STATE_REQ = 94,

	WCN36XX_HAL_ENTER_IMPS_RSP = 95,
	WCN36XX_HAL_EXIT_IMPS_RSP = 96,
	WCN36XX_HAL_ENTER_BMPS_RSP = 97,
	WCN36XX_HAL_EXIT_BMPS_RSP = 98,
	WCN36XX_HAL_ENTER_UAPSD_RSP = 99,
	WCN36XX_HAL_EXIT_UAPSD_RSP = 100,
	WCN36XX_HAL_SET_UAPSD_AC_PARAMS_RSP = 101,
	WCN36XX_HAL_UPDATE_UAPSD_PARAM_RSP = 102,
	WCN36XX_HAL_CONFIGURE_RXP_FILTER_RSP = 103,
	WCN36XX_HAL_ADD_BCN_FILTER_RSP = 104,
	WCN36XX_HAL_REM_BCN_FILTER_RSP = 105,
	WCN36XX_HAL_SET_RSSI_THRESH_RSP = 106,
	WCN36XX_HAL_HOST_OFFLOAD_RSP = 107,
	WCN36XX_HAL_ADD_WOWL_BCAST_PTRN_RSP = 108,
	WCN36XX_HAL_DEL_WOWL_BCAST_PTRN_RSP = 109,
	WCN36XX_HAL_ENTER_WOWL_RSP = 110,
	WCN36XX_HAL_EXIT_WOWL_RSP = 111,
	WCN36XX_HAL_RSSI_NOTIFICATION_IND = 112,
	WCN36XX_HAL_GET_RSSI_RSP = 113,
	WCN36XX_HAL_CONFIGURE_APPS_CPU_WAKEUP_STATE_RSP = 114,

	/* 11k related events */
	WCN36XX_HAL_SET_MAX_TX_POWER_REQ = 115,
	WCN36XX_HAL_SET_MAX_TX_POWER_RSP = 116,

	/* 11R related msgs */
	WCN36XX_HAL_AGGR_ADD_TS_REQ = 117,
	WCN36XX_HAL_AGGR_ADD_TS_RSP = 118,

	/* P2P  WLAN_FEATURE_P2P */
	WCN36XX_HAL_SET_P2P_GONOA_REQ = 119,
	WCN36XX_HAL_SET_P2P_GONOA_RSP = 120,

	/* WLAN Dump commands */
	WCN36XX_HAL_DUMP_COMMAND_REQ = 121,
	WCN36XX_HAL_DUMP_COMMAND_RSP = 122,

	/* OEM_DATA FEATURE SUPPORT */
	WCN36XX_HAL_START_OEM_DATA_REQ = 123,
	WCN36XX_HAL_START_OEM_DATA_RSP = 124,

	/* ADD SELF STA REQ and RSP */
	WCN36XX_HAL_ADD_STA_SELF_REQ = 125,
	WCN36XX_HAL_ADD_STA_SELF_RSP = 126,

	/* DEL SELF STA SUPPORT */
	WCN36XX_HAL_DEL_STA_SELF_REQ = 127,
	WCN36XX_HAL_DEL_STA_SELF_RSP = 128,

	/* Coex Indication */
	WCN36XX_HAL_COEX_IND = 129,

	/* Tx Complete Indication */
	WCN36XX_HAL_OTA_TX_COMPL_IND = 130,

	/* Host Suspend/resume messages */
	WCN36XX_HAL_HOST_SUSPEND_IND = 131,
	WCN36XX_HAL_HOST_RESUME_REQ = 132,
	WCN36XX_HAL_HOST_RESUME_RSP = 133,

	WCN36XX_HAL_SET_TX_POWER_REQ = 134,
	WCN36XX_HAL_SET_TX_POWER_RSP = 135,
	WCN36XX_HAL_GET_TX_POWER_REQ = 136,
	WCN36XX_HAL_GET_TX_POWER_RSP = 137,

	WCN36XX_HAL_P2P_NOA_ATTR_IND = 138,

	WCN36XX_HAL_ENABLE_RADAR_DETECT_REQ = 139,
	WCN36XX_HAL_ENABLE_RADAR_DETECT_RSP = 140,
	WCN36XX_HAL_GET_TPC_REPORT_REQ = 141,
	WCN36XX_HAL_GET_TPC_REPORT_RSP = 142,
	WCN36XX_HAL_RADAR_DETECT_IND = 143,
	WCN36XX_HAL_RADAR_DETECT_INTR_IND = 144,
	WCN36XX_HAL_KEEP_ALIVE_REQ = 145,
	WCN36XX_HAL_KEEP_ALIVE_RSP = 146,

	/* PNO messages */
	WCN36XX_HAL_SET_PREF_NETWORK_REQ = 147,
	WCN36XX_HAL_SET_PREF_NETWORK_RSP = 148,
	WCN36XX_HAL_SET_RSSI_FILTER_REQ = 149,
	WCN36XX_HAL_SET_RSSI_FILTER_RSP = 150,
	WCN36XX_HAL_UPDATE_SCAN_PARAM_REQ = 151,
	WCN36XX_HAL_UPDATE_SCAN_PARAM_RSP = 152,
	WCN36XX_HAL_PREF_NETW_FOUND_IND = 153,

	WCN36XX_HAL_SET_TX_PER_TRACKING_REQ = 154,
	WCN36XX_HAL_SET_TX_PER_TRACKING_RSP = 155,
	WCN36XX_HAL_TX_PER_HIT_IND = 156,

	WCN36XX_HAL_8023_MULTICAST_LIST_REQ = 157,
	WCN36XX_HAL_8023_MULTICAST_LIST_RSP = 158,

	WCN36XX_HAL_SET_PACKET_FILTER_REQ = 159,
	WCN36XX_HAL_SET_PACKET_FILTER_RSP = 160,
	WCN36XX_HAL_PACKET_FILTER_MATCH_COUNT_REQ = 161,
	WCN36XX_HAL_PACKET_FILTER_MATCH_COUNT_RSP = 162,
	WCN36XX_HAL_CLEAR_PACKET_FILTER_REQ = 163,
	WCN36XX_HAL_CLEAR_PACKET_FILTER_RSP = 164,

	/*
	 * This is temp fix. Should be removed once Host and Riva code is
	 * in sync.
	 */
	WCN36XX_HAL_INIT_SCAN_CON_REQ = 165,

	WCN36XX_HAL_SET_POWER_PARAMS_REQ = 166,
	WCN36XX_HAL_SET_POWER_PARAMS_RSP = 167,

	WCN36XX_HAL_TSM_STATS_REQ = 168,
	WCN36XX_HAL_TSM_STATS_RSP = 169,

	/* wake reason indication (WOW) */
	WCN36XX_HAL_WAKE_REASON_IND = 170,

	/* GTK offload support */
	WCN36XX_HAL_GTK_OFFLOAD_REQ = 171,
	WCN36XX_HAL_GTK_OFFLOAD_RSP = 172,
	WCN36XX_HAL_GTK_OFFLOAD_GETINFO_REQ = 173,
	WCN36XX_HAL_GTK_OFFLOAD_GETINFO_RSP = 174,

	WCN36XX_HAL_FEATURE_CAPS_EXCHANGE_REQ = 175,
	WCN36XX_HAL_FEATURE_CAPS_EXCHANGE_RSP = 176,
	WCN36XX_HAL_EXCLUDE_UNENCRYPTED_IND = 177,

	WCN36XX_HAL_SET_THERMAL_MITIGATION_REQ = 178,
	WCN36XX_HAL_SET_THERMAL_MITIGATION_RSP = 179,

	WCN36XX_HAL_UPDATE_VHT_OP_MODE_REQ = 182,
	WCN36XX_HAL_UPDATE_VHT_OP_MODE_RSP = 183,

	WCN36XX_HAL_P2P_NOA_START_IND = 184,

	WCN36XX_HAL_GET_ROAM_RSSI_REQ = 185,
	WCN36XX_HAL_GET_ROAM_RSSI_RSP = 186,

	WCN36XX_HAL_CLASS_B_STATS_IND = 187,
	WCN36XX_HAL_DEL_BA_IND = 188,
	WCN36XX_HAL_DHCP_START_IND = 189,
	WCN36XX_HAL_DHCP_STOP_IND = 190,

	WCN36XX_HAL_MSG_MAX = WCN36XX_HAL_MSG_TYPE_MAX_ENUM_SIZE
};

/* Enumeration for Version */
enum hal_host_msg_version {
	WCN36XX_HAL_MSG_VERSION0 = 0,
	WCN36XX_HAL_MSG_VERSION1 = 1,
	WCN36XX_HAL_MSG_WCNSS_CTRL_VERSION = 0x7FFF,	/*define as 2 bytes data */
	WCN36XX_HAL_MSG_VERSION_MAX_FIELD = WCN36XX_HAL_MSG_WCNSS_CTRL_VERSION
};

enum driver_type {
	eDRIVER_TYPE_PRODUCTION = 0,
	eDRIVER_TYPE_MFG = 1,
	eDRIVER_TYPE_DVT = 2,
	eDRIVER_TYPE_MAX = WCN36XX_HAL_MAX_ENUM_SIZE
};

enum hal_stop_type {
	HAL_STOP_TYPE_SYS_RESET,
	HAL_STOP_TYPE_SYS_DEEP_SLEEP,
	HAL_STOP_TYPE_RF_KILL,
	HAL_STOP_TYPE_MAX = WCN36XX_HAL_MAX_ENUM_SIZE
};

enum hal_sys_mode {
	eHAL_SYS_MODE_NORMAL,
	eHAL_SYS_MODE_LEARN,
	eHAL_SYS_MODE_SCAN,
	eHAL_SYS_MODE_PROMISC,
	eHAL_SYS_MODE_SUSPEND_LINK,
	eHAL_SYS_MODE_ROAM_SCAN,
	eHAL_SYS_MODE_ROAM_SUSPEND_LINK,
	eHAL_SYS_MODE_MAX = WCN36XX_HAL_MAX_ENUM_SIZE
};

enum phy_chan_bond_state {
	/* 20MHz IF bandwidth centered on IF carrier */
	PHY_SINGLE_CHANNEL_CENTERED = 0,

	/* 40MHz IF bandwidth with lower 20MHz supporting the primary channel */
	PHY_DOUBLE_CHANNEL_LOW_PRIMARY = 1,

	/* 40MHz IF bandwidth centered on IF carrier */
	PHY_DOUBLE_CHANNEL_CENTERED = 2,

	/* 40MHz IF bandwidth with higher 20MHz supporting the primary channel */
	PHY_DOUBLE_CHANNEL_HIGH_PRIMARY = 3,

	/* 20/40MHZ offset LOW 40/80MHZ offset CENTERED */
	PHY_QUADRUPLE_CHANNEL_20MHZ_LOW_40MHZ_CENTERED = 4,

	/* 20/40MHZ offset CENTERED 40/80MHZ offset CENTERED */
	PHY_QUADRUPLE_CHANNEL_20MHZ_CENTERED_40MHZ_CENTERED = 5,

	/* 20/40MHZ offset HIGH 40/80MHZ offset CENTERED */
	PHY_QUADRUPLE_CHANNEL_20MHZ_HIGH_40MHZ_CENTERED = 6,

	/* 20/40MHZ offset LOW 40/80MHZ offset LOW */
	PHY_QUADRUPLE_CHANNEL_20MHZ_LOW_40MHZ_LOW = 7,

	/* 20/40MHZ offset HIGH 40/80MHZ offset LOW */
	PHY_QUADRUPLE_CHANNEL_20MHZ_HIGH_40MHZ_LOW = 8,

	/* 20/40MHZ offset LOW 40/80MHZ offset HIGH */
	PHY_QUADRUPLE_CHANNEL_20MHZ_LOW_40MHZ_HIGH = 9,

	/* 20/40MHZ offset-HIGH 40/80MHZ offset HIGH */
	PHY_QUADRUPLE_CHANNEL_20MHZ_HIGH_40MHZ_HIGH = 10,

	PHY_CHANNEL_BONDING_STATE_MAX = WCN36XX_HAL_MAX_ENUM_SIZE
};

/* Spatial Multiplexing(SM) Power Save mode */
enum sir_ht_mimo_state {
	/* Static SM Power Save mode */
	eSIR_HT_MIMO_PS_STATIC = 0,

	/* Dynamic SM Power Save mode */
	eSIR_HT_MIMO_PS_DYNAMIC = 1,

	/* reserved */
	eSIR_HT_MIMO_PS_NA = 2,

	/* SM Power Save disabled */
	eSIR_HT_MIMO_PS_NO_LIMIT = 3,

	eSIR_HT_MIMO_PS_MAX = WCN36XX_HAL_MAX_ENUM_SIZE
};

/* each station added has a rate mode which specifies the sta attributes */
enum sta_rate_mode {
	eSTA_TAURUS = 0,
	eSTA_TITAN,
	eSTA_POLARIS,
	eSTA_11b,
	eSTA_11bg,
	eSTA_11a,
	eSTA_11n,
	eSTA_11ac,
	eSTA_INVALID_RATE_MODE = WCN36XX_HAL_MAX_ENUM_SIZE
};

/* 1,2,5.5,11 */
#define SIR_NUM_11B_RATES           4

/* 6,9,12,18,24,36,48,54 */
#define SIR_NUM_11A_RATES           8

/* 72,96,108 */
#define SIR_NUM_POLARIS_RATES       3

#define SIR_MAC_MAX_SUPPORTED_MCS_SET    16

enum sir_bss_type {
	eSIR_INFRASTRUCTURE_MODE,

	/* Added for softAP support */
	eSIR_INFRA_AP_MODE,

	eSIR_IBSS_MODE,

	/* Added for BT-AMP support */
	eSIR_BTAMP_STA_MODE,

	/* Added for BT-AMP support */
	eSIR_BTAMP_AP_MODE,

	eSIR_AUTO_MODE,

	eSIR_DONOT_USE_BSS_TYPE = WCN36XX_HAL_MAX_ENUM_SIZE
};

enum sir_nw_type {
	eSIR_11A_NW_TYPE,
	eSIR_11B_NW_TYPE,
	eSIR_11G_NW_TYPE,
	eSIR_11N_NW_TYPE,
	eSIR_DONOT_USE_NW_TYPE = WCN36XX_HAL_MAX_ENUM_SIZE
};

#define SIR_MAC_RATESET_EID_MAX            12

enum sir_ht_operating_mode {
	/* No Protection */
	eSIR_HT_OP_MODE_PURE,

	/* Overlap Legacy device present, protection is optional */
	eSIR_HT_OP_MODE_OVERLAP_LEGACY,

	/* No legacy device, but 20 MHz HT present */
	eSIR_HT_OP_MODE_NO_LEGACY_20MHZ_HT,

	/* Protection is required */
	eSIR_HT_OP_MODE_MIXED,

	eSIR_HT_OP_MODE_MAX = WCN36XX_HAL_MAX_ENUM_SIZE
};

/* Encryption type enum used with peer */
enum ani_ed_type {
	eSIR_ED_NONE,
	eSIR_ED_WEP40,
	eSIR_ED_WEP104,
	eSIR_ED_TKIP,
	eSIR_ED_CCMP,
	eSIR_ED_WPI,
	eSIR_ED_AES_128_CMAC,
	eSIR_ED_NOT_IMPLEMENTED = WCN36XX_HAL_MAX_ENUM_SIZE
};

#define WLAN_MAX_KEY_RSC_LEN                16
#define WLAN_WAPI_KEY_RSC_LEN               16

/* MAX key length when ULA is used */
#define SIR_MAC_MAX_KEY_LENGTH              32
#define SIR_MAC_MAX_NUM_OF_DEFAULT_KEYS     4

/*
 * Enum to specify whether key is used for TX only, RX only or both.
 */
enum ani_key_direction {
	eSIR_TX_ONLY,
	eSIR_RX_ONLY,
	eSIR_TX_RX,
	eSIR_TX_DEFAULT,
	eSIR_DONOT_USE_KEY_DIRECTION = WCN36XX_HAL_MAX_ENUM_SIZE
};

enum ani_wep_type {
	eSIR_WEP_STATIC,
	eSIR_WEP_DYNAMIC,
	eSIR_WEP_MAX = WCN36XX_HAL_MAX_ENUM_SIZE
};

enum sir_link_state {

	eSIR_LINK_IDLE_STATE = 0,
	eSIR_LINK_PREASSOC_STATE = 1,
	eSIR_LINK_POSTASSOC_STATE = 2,
	eSIR_LINK_AP_STATE = 3,
	eSIR_LINK_IBSS_STATE = 4,

	/* BT-AMP Case */
	eSIR_LINK_BTAMP_PREASSOC_STATE = 5,
	eSIR_LINK_BTAMP_POSTASSOC_STATE = 6,
	eSIR_LINK_BTAMP_AP_STATE = 7,
	eSIR_LINK_BTAMP_STA_STATE = 8,

	/* Reserved for HAL Internal Use */
	eSIR_LINK_LEARN_STATE = 9,
	eSIR_LINK_SCAN_STATE = 10,
	eSIR_LINK_FINISH_SCAN_STATE = 11,
	eSIR_LINK_INIT_CAL_STATE = 12,
	eSIR_LINK_FINISH_CAL_STATE = 13,
	eSIR_LINK_LISTEN_STATE = 14,

	eSIR_LINK_MAX = WCN36XX_HAL_MAX_ENUM_SIZE
};

enum hal_stats_mask {
	HAL_SUMMARY_STATS_INFO = 0x00000001,
	HAL_GLOBAL_CLASS_A_STATS_INFO = 0x00000002,
	HAL_GLOBAL_CLASS_B_STATS_INFO = 0x00000004,
	HAL_GLOBAL_CLASS_C_STATS_INFO = 0x00000008,
	HAL_GLOBAL_CLASS_D_STATS_INFO = 0x00000010,
	HAL_PER_STA_STATS_INFO = 0x00000020
};

/* BT-AMP events type */
enum bt_amp_event_type {
	BTAMP_EVENT_CONNECTION_START,
	BTAMP_EVENT_CONNECTION_STOP,
	BTAMP_EVENT_CONNECTION_TERMINATED,

	/* This and beyond are invalid values */
	BTAMP_EVENT_TYPE_MAX = WCN36XX_HAL_MAX_ENUM_SIZE,
};

/* PE Statistics */
enum pe_stats_mask {
	PE_SUMMARY_STATS_INFO = 0x00000001,
	PE_GLOBAL_CLASS_A_STATS_INFO = 0x00000002,
	PE_GLOBAL_CLASS_B_STATS_INFO = 0x00000004,
	PE_GLOBAL_CLASS_C_STATS_INFO = 0x00000008,
	PE_GLOBAL_CLASS_D_STATS_INFO = 0x00000010,
	PE_PER_STA_STATS_INFO = 0x00000020,

	/* This and beyond are invalid values */
	PE_STATS_TYPE_MAX = WCN36XX_HAL_MAX_ENUM_SIZE
};

/* Message definitons - All the messages below need to be packed */

/* Definition for HAL API Version. */
struct wcnss_wlan_version {
	u8 revision;
	u8 version;
	u8 minor;
	u8 major;
} __packed;

/* Definition for Encryption Keys */
struct sir_keys {
	u8 keyId;

	/* 0 for multicast */
	u8 unicast;

	enum ani_key_direction keyDirection;

	/* Usage is unknown */
	u8 keyRsc[WLAN_MAX_KEY_RSC_LEN];

	/* =1 for authenticator,=0 for supplicant */
	u8 paeRole;

	u16 keyLength;
	u8 key[SIR_MAC_MAX_KEY_LENGTH];
};

/* SetStaKeyParams Moving here since it is shared by configbss/setstakey msgs */
struct set_sta_key_params {
	/* STA Index */
	u16 staIdx;

	/* Encryption Type used with peer */
	enum ani_ed_type encType;

	/* STATIC/DYNAMIC - valid only for WEP */
	enum ani_wep_type wepType;

	/* Default WEP key, valid only for static WEP, must between 0 and 3. */
	u8 defWEPIdx;

	/* valid only for non-static WEP encyrptions */
	struct sir_keys key[SIR_MAC_MAX_NUM_OF_DEFAULT_KEYS];

	/*
	 * Control for Replay Count, 1= Single TID based replay count on Tx
	 * 0 = Per TID based replay count on TX
	 */
	u8 singleTidRc;

};

/* 4-byte control message header used by HAL*/
struct hal_msg_header {
	enum hal_host_msg_type msgType:16;
	enum hal_host_msg_version msgVersion:16;
	u32 msgLen;
};

/* Config format required by HAL for each CFG item*/
struct hal_cfg {
	/* Cfg Id. The Id required by HAL is exported by HAL
	 * in shared header file between UMAC and HAL.*/
	u16 uCfgId;

	/* Length of the Cfg. This parameter is used to go to next cfg 
	 * in the TLV format.*/
	u16 uCfgLen;

	/* Padding bytes for unaligned address's */
	u16 uCfgPadBytes;

	/* Reserve bytes for making cfgVal to align address */
	u16 uCfgReserve;

	/* Following the uCfgLen field there should be a 'uCfgLen' bytes
	 * containing the uCfgValue ; u8 uCfgValue[uCfgLen] */
};

struct hal_mac_start_parameters {
	/* Drive Type - Production or FTM etc */
	enum driver_type driverType;

	/*Length of the config buffer */
	u32 uConfigBufferLen;

	/* Following this there is a TLV formatted buffer of length 
	 * "uConfigBufferLen" bytes containing all config values. 
	 * The TLV is expected to be formatted like this:
	 * 0           15            31           31+CFG_LEN-1        length-1
	 * |   CFG_ID   |   CFG_LEN   |   CFG_BODY    |  CFG_ID  |......|
	 */
} __packed;

struct hal_mac_start_req_msg {
	/* Note: The length specified in tHalMacStartReqMsg messages should be
	 * header.msgLen = sizeof(tHalMacStartReqMsg) + uConfigBufferLen */
	struct hal_msg_header header;
	struct hal_mac_start_parameters startReqParams;
};

struct hal_mac_start_rsp_params {
	/*success or failure */
	u16 status;

	/*Max number of STA supported by the device */
	u8 ucMaxStations;

	/*Max number of BSS supported by the device */
	u8 ucMaxBssids;

	/*API Version */
	struct wcnss_wlan_version wcnssWlanVersion;

	/*CRM build information */
	u8 wcnssCrmVersionString[WCN36XX_HAL_VERSION_LENGTH];

	/*hardware/chipset/misc version information */
	u8 wcnssWlanVersionString[WCN36XX_HAL_VERSION_LENGTH];

};

struct hal_mac_start_rsp_msg {
	struct hal_msg_header header;
	struct hal_mac_start_rsp_params startRspParams;
};

struct hal_mac_stop_req_params {
	/*The reason for which the device is being stopped */
	enum hal_stop_type reason;

};

struct hal_mac_stop_req_msg {
	struct hal_msg_header header;
	struct hal_mac_stop_req_params stopReqParams;
};

struct hal_mac_stop_rsp_msg {
	struct hal_msg_header header;

	/*success or failure */
	u32 status;
};

struct hal_update_cfg_req_msg {
	/* Note: The length specified in tHalUpdateCfgReqMsg messages should be
	 * header.msgLen = sizeof(tHalUpdateCfgReqMsg) + uConfigBufferLen */
	struct hal_msg_header header;

	/* Length of the config buffer. Allows UMAC to update multiple CFGs */
	u32 uConfigBufferLen;

	/* Following this there is a TLV formatted buffer of length 
	 * "uConfigBufferLen" bytes containing all config values. 
	 * The TLV is expected to be formatted like this:
	 * 0           15            31           31+CFG_LEN-1        length-1
	 * |   CFG_ID   |   CFG_LEN   |   CFG_BODY    |  CFG_ID  |......|
	 */

};

struct hal_update_cfg_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

/* Frame control field format (2 bytes) */
typedef struct sSirMacFrameCtl {

#ifndef ANI_LITTLE_BIT_ENDIAN

	u8 subType:4;
	u8 type:2;
	u8 protVer:2;

	u8 order:1;
	u8 wep:1;
	u8 moreData:1;
	u8 powerMgmt:1;
	u8 retry:1;
	u8 moreFrag:1;
	u8 fromDS:1;
	u8 toDS:1;

#else

	u8 protVer:2;
	u8 type:2;
	u8 subType:4;

	u8 toDS:1;
	u8 fromDS:1;
	u8 moreFrag:1;
	u8 retry:1;
	u8 powerMgmt:1;
	u8 moreData:1;
	u8 wep:1;
	u8 order:1;

#endif

} tSirMacFrameCtl, *tpSirMacFrameCtl;

/* Sequence control field */
typedef struct sSirMacSeqCtl {
	u8 fragNum:4;
	u8 seqNumLo:4;
	u8 seqNumHi:8;
} tSirMacSeqCtl, *tpSirMacSeqCtl;

/* Management header format */
typedef struct sSirMacMgmtHdr {
	tSirMacFrameCtl fc;
	u8 durationLo;
	u8 durationHi;
	u8 da[6];
	u8 sa[6];
	u8 bssId[6];
	tSirMacSeqCtl seqControl;
} tSirMacMgmtHdr, *tpSirMacMgmtHdr;

/* Scan Entry to hold active BSS idx's */
struct sir_scan_entry {
	u8 bssIdx[HAL_NUM_BSSID];
	u8 activeBSScnt;
};

struct hal_init_scan_req_msg {
	struct hal_msg_header header;

	/* LEARN - AP Role
	   SCAN - STA Role */
	enum hal_sys_mode scanMode;

	/* BSSID of the BSS */
	u8 bssid[ETH_ALEN];

	/* Whether BSS needs to be notified */
	u8 notifyBss;

	/* Kind of frame to be used for notifying the BSS (Data Null, QoS
	 * Null, or CTS to Self). Must always be a valid frame type. */
	u8 frameType;

	/* UMAC has the option of passing the MAC frame to be used for
	 * notifying the BSS. If non-zero, HAL will use the MAC frame
	 * buffer pointed to by macMgmtHdr. If zero, HAL will generate the
	 * appropriate MAC frame based on frameType. */
	u8 frameLength;

	/* Following the framelength there is a MAC frame buffer if
	 * frameLength is non-zero. */
	tSirMacMgmtHdr macMgmtHdr;

	/* Entry to hold number of active BSS idx's */
	struct sir_scan_entry scanEntry;
};

struct hal_init_scan_con_req_msg {
	struct hal_msg_header header;

	/* LEARN - AP Role
	   SCAN - STA Role */
	enum hal_sys_mode scanMode;

	/* BSSID of the BSS */
	u8 bssid[ETH_ALEN];

	/* Whether BSS needs to be notified */
	u8 notifyBss;

	/* Kind of frame to be used for notifying the BSS (Data Null, QoS
	 * Null, or CTS to Self). Must always be a valid frame type. */
	u8 frameType;

	/* UMAC has the option of passing the MAC frame to be used for
	 * notifying the BSS. If non-zero, HAL will use the MAC frame
	 * buffer pointed to by macMgmtHdr. If zero, HAL will generate the
	 * appropriate MAC frame based on frameType. */
	u8 frameLength;

	/* Following the framelength there is a MAC frame buffer if
	 * frameLength is non-zero. */
	tSirMacMgmtHdr macMgmtHdr;

	/* Entry to hold number of active BSS idx's */
	struct sir_scan_entry scanEntry;

	/* Single NoA usage in Scanning */
	u8 useNoA;

	/* Indicates the scan duration (in ms) */
	u16 scanDuration;

};

struct hal_init_scan_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct hal_start_scan_req_msg {
	struct hal_msg_header header;

	/* Indicates the channel to scan */
	u8 scanChannel;
};

struct hal_start_rsm_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

	u32 startTSF[2];
	u8 txMgmtPower;

};

struct hal_end_scan_req_msg {
	struct hal_msg_header header;

	/* Indicates the channel to stop scanning. Not used really. But
	 * retained for symmetry with "start Scan" message. It can also
	 * help in error check if needed. */
	u8 scanChannel;
};

struct hal_end_scan_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
};

struct hal_finish_scan_req_msg {
	struct hal_msg_header header;

	/* Identifies the operational state of the AP/STA
	 * LEARN - AP Role SCAN - STA Role */
	enum hal_sys_mode scanMode;

	/* Operating channel to tune to. */
	u8 currentOperChannel;

	/* Channel Bonding state If 20/40 MHz is operational, this will
	 * indicate the 40 MHz extension channel in combination with the
	 * control channel */
	enum phy_chan_bond_state cbState;

	/* BSSID of the BSS */
	u8 bssid[ETH_ALEN];

	/* Whether BSS needs to be notified */
	u8 notifyBss;

	/* Kind of frame to be used for notifying the BSS (Data Null, QoS
	 * Null, or CTS to Self). Must always be a valid frame type. */
	u8 frameType;

	/* UMAC has the option of passing the MAC frame to be used for
	 * notifying the BSS. If non-zero, HAL will use the MAC frame
	 * buffer pointed to by macMgmtHdr. If zero, HAL will generate the
	 * appropriate MAC frame based on frameType. */
	u8 frameLength;

	/* Following the framelength there is a MAC frame buffer if
	 * frameLength is non-zero. */
	tSirMacMgmtHdr macMgmtHdr;

	/* Entry to hold number of active BSS idx's */
	struct sir_scan_entry scanEntry;

};

struct hal_finish_scan_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct sir_supported_rates {
	/*
	 * For Self STA Entry: this represents Self Mode.
	 * For Peer Stations, this represents the mode of the peer.
	 * On Station:
	 *
	 * --this mode is updated when PE adds the Self Entry.
	 *
	 * -- OR when PE sends 'ADD_BSS' message and station context in BSS
	 *    is used to indicate the mode of the AP.
	 *
	 * ON AP:
	 *
	 * -- this mode is updated when PE sends 'ADD_BSS' and Sta entry
	 *     for that BSS is used to indicate the self mode of the AP.
	 *
	 * -- OR when a station is associated, PE sends 'ADD_STA' message
	 *    with this mode updated.
	 */

	enum sta_rate_mode opRateMode;

	/* 11b, 11a and aniLegacyRates are IE rates which gives rate in
	 * unit of 500Kbps */
	u16 llbRates[SIR_NUM_11B_RATES];
	u16 llaRates[SIR_NUM_11A_RATES];
	u16 aniLegacyRates[SIR_NUM_POLARIS_RATES];
	u16 reserved;

	/* Taurus only supports 26 Titan Rates(no ESF/concat Rates will be
	 * supported) First 26 bits are reserved for those Titan rates and
	 * the last 4 bits(bit28-31) for Taurus, 2(bit26-27) bits are
	 * reserved. */
	/* Titan and Taurus Rates */
	u32 aniEnhancedRateBitmap;

	/*
	 * 0-76 bits used, remaining reserved
	 * bits 0-15 and 32 should be set.
	 */
	u8 supportedMCSSet[SIR_MAC_MAX_SUPPORTED_MCS_SET];

	/*
	 * RX Highest Supported Data Rate defines the highest data
	 * rate that the STA is able to receive, in unites of 1Mbps.
	 * This value is derived from "Supported MCS Set field" inside
	 * the HT capability element.
	 */
	u16 rxHighestDataRate;

};

struct config_sta_params {
	/* BSSID of STA */
	u8 bssId[ETH_ALEN];

	/* ASSOC ID, as assigned by UMAC */
	u16 assocId;

	/* STA entry Type: 0 - Self, 1 - Other/Peer, 2 - BSSID, 3 - BCAST */
	u8 staType;

	/* Short Preamble Supported. */
	u8 shortPreambleSupported;

	/* MAC Address of STA */
	u8 staMac[ETH_ALEN];

	/* Listen interval of the STA */
	u16 listenInterval;

	/* Support for 11e/WMM */
	u8 wmmEnabled;

	/* 11n HT capable STA */
	u8 htCapable;

	/* TX Width Set: 0 - 20 MHz only, 1 - 20/40 MHz */
	u8 txChannelWidthSet;

	/* RIFS mode 0 - NA, 1 - Allowed */
	u8 rifsMode;

	/* L-SIG TXOP Protection mechanism 
	   0 - No Support, 1 - Supported
	   SG - there is global field */
	u8 lsigTxopProtection;

	/* Max Ampdu Size supported by STA. TPE programming.
	   0 : 8k , 1 : 16k, 2 : 32k, 3 : 64k */
	u8 maxAmpduSize;

	/* Max Ampdu density. Used by RA.  3 : 0~7 : 2^(11nAMPDUdensity -4) */
	u8 maxAmpduDensity;

	/* Max AMSDU size 1 : 3839 bytes, 0 : 7935 bytes */
	u8 maxAmsduSize;

	/* Short GI support for 40Mhz packets */
	u8 fShortGI40Mhz;

	/* Short GI support for 20Mhz packets */
	u8 fShortGI20Mhz;

	/* Robust Management Frame (RMF) enabled/disabled */
	u8 rmfEnabled;

	/* The unicast encryption type in the association */
	u32 encryptType;

	/* HAL should update the existing STA entry, if this flag is set. UMAC
	   will set this flag in case of RE-ASSOC, where we want to reuse the old
	   STA ID. 0 = Add, 1 = Update */
	u8 action;

	/* U-APSD Flags: 1b per AC.  Encoded as follows:
	   b7 b6 b5 b4 b3 b2 b1 b0 =
	   X  X  X  X  BE BK VI VO */
	u8 uAPSD;

	/* Max SP Length */
	u8 maxSPLen;

	/* 11n Green Field preamble support
	   0 - Not supported, 1 - Supported */
	u8 greenFieldCapable;

	/* MIMO Power Save mode */
	enum sir_ht_mimo_state mimoPS;

	/* Delayed BA Support */
	u8 delayedBASupport;

	/* Max AMPDU duration in 32us */
	u8 us32MaxAmpduDuration;

	/* HT STA should set it to 1 if it is enabled in BSS. HT STA should
	 * set it to 0 if AP does not support it. This indication is sent
	 * to HAL and HAL uses this flag to pickup up appropriate 40Mhz
	 * rates. */
	u8 fDsssCckMode40Mhz;

	/* Valid STA Idx when action=Update. Set to 0xFF when invalid!
	 * Retained for backward compalibity with existing HAL code */
	u8 staIdx;

	/* BSSID of BSS to which station is associated. Set to 0xFF when
	 * invalid. Retained for backward compalibity with existing HAL
	 * code */
	u8 bssIdx;

	u8 p2pCapableSta;

	/* Reserved to align next field on a dword boundary */
	u8 reserved;

	/* These rates are the intersection of peer and self capabilities. */
	struct sir_supported_rates supportedRates;

};

struct sir_supported_rates_v1 {
	/*
	 * For Self STA Entry: this represents Self Mode.
	 * For Peer Stations, this represents the mode of the peer.
	 *
	 * On Station:
	 *
	 * --this mode is updated when PE adds the Self Entry.
	 *
	 * -- OR when PE sends 'ADD_BSS' message and station context in BSS
	 *    is used to indicate the mode of the AP.
	 *
	 * ON AP:
	 *
	 * -- this mode is updated when PE sends 'ADD_BSS' and Sta entry
	 *    for that BSS is used to indicate the self mode of the AP.
	 *
	 * -- OR when a station is associated, PE sends 'ADD_STA' message
         *     with this mode updated.
	 */

	enum sta_rate_mode opRateMode;

	/* 11b, 11a and aniLegacyRates are IE rates which gives rate in
	 * unit of 500Kbps */
	u16 llbRates[SIR_NUM_11B_RATES];
	u16 llaRates[SIR_NUM_11A_RATES];
	u16 aniLegacyRates[SIR_NUM_POLARIS_RATES];
	u16 reserved;

	/* Taurus only supports 26 Titan Rates(no ESF/concat Rates will be
	 * supported) First 26 bits are reserved for those Titan rates and
	 * the last 4 bits(bit28-31) for Taurus, 2(bit26-27) bits are
	 * reserved. */

	/* Titan and Taurus Rates */
	u32 aniEnhancedRateBitmap;

	/*
	 * 0-76 bits used, remaining reserved
	 * bits 0-15 and 32 should be set.
	 */
	u8 supportedMCSSet[SIR_MAC_MAX_SUPPORTED_MCS_SET];

	/*
	 * RX Highest Supported Data Rate defines the highest data
	 * rate that the STA is able to receive, in unites of 1Mbps.
	 * This value is derived from "Supported MCS Set field" inside
	 * the HT capability element.
	 */
	u16 rxHighestDataRate;

	/* Indicates the Maximum MCS that can be received for each number
	 * of spacial streams */
	u16 vhtRxMCSMap;

	/* Indicate the highest VHT data rate that the STA is able to receive */
	u16 vhtRxHighestDataRate;

	/* Indicates the Maximum MCS that can be transmitted  for each number
	 * of spacial streams */
	u16 vhtTxMCSMap;

	/* Indicate the highest VHT data rate that the STA is able to
	 * transmit */
	u16 vhtTxHighestDataRate;

};

struct config_sta_params_v1 {
	/* BSSID of STA */
	u8 bssId[ETH_ALEN];

	/* ASSOC ID, as assigned by UMAC */
	u16 assocId;

	/* STA entry Type: 0 - Self, 1 - Other/Peer, 2 - BSSID, 3 - BCAST */
	u8 staType;

	/* Short Preamble Supported. */
	u8 shortPreambleSupported;

	/* MAC Address of STA */
	u8 staMac[ETH_ALEN];

	/* Listen interval of the STA */
	u16 listenInterval;

	/* Support for 11e/WMM */
	u8 wmmEnabled;

	/* 11n HT capable STA */
	u8 htCapable;

	/* TX Width Set: 0 - 20 MHz only, 1 - 20/40 MHz */
	u8 txChannelWidthSet;

	/* RIFS mode 0 - NA, 1 - Allowed */
	u8 rifsMode;

	/* L-SIG TXOP Protection mechanism
	   0 - No Support, 1 - Supported
	   SG - there is global field */
	u8 lsigTxopProtection;

	/* Max Ampdu Size supported by STA. TPE programming.
	   0 : 8k , 1 : 16k, 2 : 32k, 3 : 64k */
	u8 maxAmpduSize;

	/* Max Ampdu density. Used by RA.  3 : 0~7 : 2^(11nAMPDUdensity -4) */
	u8 maxAmpduDensity;

	/* Max AMSDU size 1 : 3839 bytes, 0 : 7935 bytes */
	u8 maxAmsduSize;

	/* Short GI support for 40Mhz packets */
	u8 fShortGI40Mhz;

	/* Short GI support for 20Mhz packets */
	u8 fShortGI20Mhz;

	/* Robust Management Frame (RMF) enabled/disabled */
	u8 rmfEnabled;

	/* The unicast encryption type in the association */
	u32 encryptType;

	/* HAL should update the existing STA entry, if this flag is set.
	 * UMAC will set this flag in case of RE-ASSOC, where we want to
	 * reuse the old STA ID. 0 = Add, 1 = Update */
	u8 action;

	/* U-APSD Flags: 1b per AC.  Encoded as follows:
	   b7 b6 b5 b4 b3 b2 b1 b0 =
	   X  X  X  X  BE BK VI VO */
	u8 uAPSD;

	/* Max SP Length */
	u8 maxSPLen;

	/* 11n Green Field preamble support
	   0 - Not supported, 1 - Supported */
	u8 greenFieldCapable;

	/* MIMO Power Save mode */
	enum sir_ht_mimo_state mimoPS;

	/* Delayed BA Support */
	u8 delayedBASupport;

	/* Max AMPDU duration in 32us */
	u8 us32MaxAmpduDuration;

	/* HT STA should set it to 1 if it is enabled in BSS. HT STA should
	 * set it to 0 if AP does not support it. This indication is sent
	 * to HAL and HAL uses this flag to pickup up appropriate 40Mhz
	 * rates. */
	u8 fDsssCckMode40Mhz;

	/* Valid STA Idx when action=Update. Set to 0xFF when invalid!
	 * Retained for backward compalibity with existing HAL code */
	u8 staIdx;

	/* BSSID of BSS to which station is associated. Set to 0xFF when
	 * invalid. Retained for backward compalibity with existing HAL
	 * code */
	u8 bssIdx;

	u8 p2pCapableSta;

	/* Reserved to align next field on a dword boundary */
	u8 htLdpcEnabled:1;
	u8 vhtLdpcEnabled:1;
	u8 vhtTxBFEnabled:1;
	u8 reserved:5;

	/* These rates are the intersection of peer and self capabilities. */
	struct sir_supported_rates_v1 supportedRates;

	u8 vhtCapable;
	u8 vhtTxChannelWidthSet;

};

struct config_sta_req_msg {
	struct hal_msg_header header;
	union {
		struct config_sta_params configStaParams;
		struct config_sta_params_v1 configStaParams_V1;
	} uStaParams;
};

struct config_sta_rsp_params {
	/* success or failure */
	u32 status;

	/* Station index; valid only when 'status' field value SUCCESS */
	u8 staIdx;

	/* BSSID Index of BSS to which the station is associated */
	u8 bssIdx;

	/* DPU Index for PTK */
	u8 dpuIndex;

	/* DPU Index for GTK */
	u8 bcastDpuIndex;

	/* DPU Index for IGTK  */
	u8 bcastMgmtDpuIdx;

	/* PTK DPU signature */
	u8 ucUcastSig;

	/* GTK DPU isignature */
	u8 ucBcastSig;

	/* IGTK DPU signature */
	u8 ucMgmtSig;

	u8 p2pCapableSta;

};

struct config_sta_rsp_msg {
	struct hal_msg_header header;
	struct config_sta_rsp_params configStaRspParams;
};

/* Delete STA Request message */
struct delete_sta_req_msg {
	struct hal_msg_header header;

	/* Index of STA to delete */
	u8 staIdx;

};

/* Delete STA Response message */
struct delete_sta_rsp_msg {
	struct hal_msg_header header;

	/*success or failure */
	u32 status;

	/* Index of STA deleted */
	u8 staId;
};

/* 12 Bytes long because this structure can be used to represent rate and
 * extended rate set IEs. The parser assume this to be at least 12 */
struct sir_rate_set {
	u8 numRates;
	u8 rate[SIR_MAC_RATESET_EID_MAX];
};

/* access category record */
struct sir_aci_aifsn {
#ifndef ANI_LITTLE_BIT_ENDIAN
	u8 rsvd:1;
	u8 aci:2;
	u8 acm:1;
	u8 aifsn:4;
#else
	u8 aifsn:4;
	u8 acm:1;
	u8 aci:2;
	u8 rsvd:1;
#endif
};

/* contention window size */
typedef struct sSirMacCW {
#ifndef ANI_LITTLE_BIT_ENDIAN
	u8 max:4;
	u8 min:4;
#else
	u8 min:4;
	u8 max:4;
#endif
} tSirMacCW;

struct sir_edca_param_record {
	struct sir_aci_aifsn aci;
	tSirMacCW cw;
	u16 txoplimit;
};

struct sir_mac_ssid {
	u8 length;
	u8 ssId[32];
};

/* Concurrency role. These are generic IDs that identify the various roles
 *  in the software system. */
enum hal_con_mode {
	HAL_STA_MODE = 0,

	/* to support softAp mode . This is misleading. It means AP MODE only. */
	HAL_STA_SAP_MODE = 1,

	HAL_P2P_CLIENT_MODE,
	HAL_P2P_GO_MODE,
	HAL_MONITOR_MODE,
};

/* This is a bit pattern to be set for each mode
 * bit 0 - sta mode
 * bit 1 - ap mode
 * bit 2 - p2p client mode
 * bit 3 - p2p go mode */
enum hal_concurrency_mode {
	HAL_STA = 1,
	HAL_SAP = 2,

	/* to support sta, softAp  mode . This means STA+AP mode */
	HAL_STA_SAP = 3,

	HAL_P2P_CLIENT = 4,
	HAL_P2P_GO = 8,
	HAL_MAX_CONCURRENCY_PERSONA = 4
};

struct config_bss_params {
	/* BSSID */
	u8 bssId[ETH_ALEN];

	/* Self Mac Address */
	u8 selfMacAddr[ETH_ALEN];

	/* BSS type */
	enum sir_bss_type bssType;

	/* Operational Mode: AP =0, STA = 1 */
	u8 operMode;

	/* Network Type */
	enum sir_nw_type nwType;

	/* Used to classify PURE_11G/11G_MIXED to program MTU */
	u8 shortSlotTimeSupported;

	/* Co-exist with 11a STA */
	u8 llaCoexist;

	/* Co-exist with 11b STA */
	u8 llbCoexist;

	/* Co-exist with 11g STA */
	u8 llgCoexist;

	/* Coexistence with 11n STA */
	u8 ht20Coexist;

	/* Non GF coexist flag */
	u8 llnNonGFCoexist;

	/* TXOP protection support */
	u8 fLsigTXOPProtectionFullSupport;

	/* RIFS mode */
	u8 fRIFSMode;

	/* Beacon Interval in TU */
	u16 beaconInterval;

	/* DTIM period */
	u8 dtimPeriod;

	/* TX Width Set: 0 - 20 MHz only, 1 - 20/40 MHz */
	u8 txChannelWidthSet;

	/* Operating channel */
	u8 currentOperChannel;

	/* Extension channel for channel bonding */
	u8 currentExtChannel;

	/* Reserved to align next field on a dword boundary */
	u8 reserved;

	/* SSID of the BSS */
	struct sir_mac_ssid ssId;

	/* HAL should update the existing BSS entry, if this flag is set.
	 * UMAC will set this flag in case of reassoc, where we want to
	 * resue the the old BSSID and still return success 0 = Add, 1 =
	 * Update */
	u8 action;

	/* MAC Rate Set */
	struct sir_rate_set rateSet;

	/* Enable/Disable HT capabilities of the BSS */
	u8 htCapable;

	/* Enable/Disable OBSS protection */
	u8 obssProtEnabled;

	/* RMF enabled/disabled */
	u8 rmfEnabled;

	/* HT Operating Mode operating mode of the 802.11n STA */
	enum sir_ht_operating_mode htOperMode;

	/* Dual CTS Protection: 0 - Unused, 1 - Used */
	u8 dualCTSProtection;

	/* Probe Response Max retries */
	u8 ucMaxProbeRespRetryLimit;

	/* To Enable Hidden ssid */
	u8 bHiddenSSIDEn;

	/* To Enable Disable FW Proxy Probe Resp */
	u8 bProxyProbeRespEn;

	/* Boolean to indicate if EDCA params are valid. UMAC might not
	 * have valid EDCA params or might not desire to apply EDCA params
	 * during config BSS. 0 implies Not Valid ; Non-Zero implies
	 * valid */
	u8 edcaParamsValid;

	/* EDCA Parameters for Best Effort Access Category */
	struct sir_edca_param_record acbe;

	/* EDCA Parameters forBackground Access Category */
	struct sir_edca_param_record acbk;

	/* EDCA Parameters for Video Access Category */
	struct sir_edca_param_record acvi;

	/* EDCA Parameters for Voice Access Category */
	struct sir_edca_param_record acvo;

#ifdef WLAN_FEATURE_VOWIFI_11R
	/* Ext Bss Config Msg if set */
	u8 extSetStaKeyParamValid;

	/* SetStaKeyParams for ext bss msg */
	tSetStaKeyParams extSetStaKeyParam;
#endif

	/* Persona for the BSS can be STA,AP,GO,CLIENT value same as enum hal_con_mode */
	u8 halPersona;

	u8 bSpectrumMgtEnable;

	/* HAL fills in the tx power used for mgmt frames in txMgmtPower */
	s8 txMgmtPower;

	/* maxTxPower has max power to be used after applying the power
	 * constraint if any */
	s8 maxTxPower;

	/*Context of the station being added in HW
	   Add a STA entry for "itself" -
	   On AP  - Add the AP itself in an "STA context"
	   On STA - Add the AP to which this STA is joining in an "STA context" */
	struct config_sta_params staContext;
};

struct config_bss_params_v1 {
	/* BSSID */
	u8 bssId[ETH_ALEN];

	/* Self Mac Address */
	u8 selfMacAddr[ETH_ALEN];

	/* BSS type */
	enum sir_bss_type bssType;

	/* Operational Mode: AP =0, STA = 1 */
	u8 operMode;

	/* Network Type */
	enum sir_nw_type nwType;

	/* Used to classify PURE_11G/11G_MIXED to program MTU */
	u8 shortSlotTimeSupported;

	/* Co-exist with 11a STA */
	u8 llaCoexist;

	/* Co-exist with 11b STA */
	u8 llbCoexist;

	/* Co-exist with 11g STA */
	u8 llgCoexist;

	/* Coexistence with 11n STA */
	u8 ht20Coexist;

	/* Non GF coexist flag */
	u8 llnNonGFCoexist;

	/* TXOP protection support */
	u8 fLsigTXOPProtectionFullSupport;

	/* RIFS mode */
	u8 fRIFSMode;

	/* Beacon Interval in TU */
	u16 beaconInterval;

	/* DTIM period */
	u8 dtimPeriod;

	/* TX Width Set: 0 - 20 MHz only, 1 - 20/40 MHz */
	u8 txChannelWidthSet;

	/* Operating channel */
	u8 currentOperChannel;

	/* Extension channel for channel bonding */
	u8 currentExtChannel;

	/* Reserved to align next field on a dword boundary */
	u8 reserved;

	/* SSID of the BSS */
	struct sir_mac_ssid ssId;

	/* HAL should update the existing BSS entry, if this flag is set.
	 * UMAC will set this flag in case of reassoc, where we want to
	 * resue the the old BSSID and still return success 0 = Add, 1 =
	 * Update */
	u8 action;

	/* MAC Rate Set */
	struct sir_rate_set rateSet;

	/* Enable/Disable HT capabilities of the BSS */
	u8 htCapable;

	/* Enable/Disable OBSS protection */
	u8 obssProtEnabled;

	/* RMF enabled/disabled */
	u8 rmfEnabled;

	/* HT Operating Mode operating mode of the 802.11n STA */
	enum sir_ht_operating_mode htOperMode;

	/* Dual CTS Protection: 0 - Unused, 1 - Used */
	u8 dualCTSProtection;

	/* Probe Response Max retries */
	u8 ucMaxProbeRespRetryLimit;

	/* To Enable Hidden ssid */
	u8 bHiddenSSIDEn;

	/* To Enable Disable FW Proxy Probe Resp */
	u8 bProxyProbeRespEn;

	/* Boolean to indicate if EDCA params are valid. UMAC might not
	 * have valid EDCA params or might not desire to apply EDCA params
	 * during config BSS. 0 implies Not Valid ; Non-Zero implies
	 * valid */
	u8 edcaParamsValid;

	/* EDCA Parameters for Best Effort Access Category */
	struct sir_edca_param_record acbe;

	/* EDCA Parameters forBackground Access Category */
	struct sir_edca_param_record acbk;

	/* EDCA Parameters for Video Access Category */
	struct sir_edca_param_record acvi;

	/* EDCA Parameters for Voice Access Category */
	struct sir_edca_param_record acvo;

#ifdef WLAN_FEATURE_VOWIFI_11R
	/* Ext Bss Config Msg if set */
	u8 extSetStaKeyParamValid;

	/* SetStaKeyParams for ext bss msg */
	tSetStaKeyParams extSetStaKeyParam;
#endif

	/* Persona for the BSS can be STA,AP,GO,CLIENT value same as enum hal_con_mode */
	u8 halPersona;

	u8 bSpectrumMgtEnable;

	/* HAL fills in the tx power used for mgmt frames in txMgmtPower */
	s8 txMgmtPower;

	/* maxTxPower has max power to be used after applying the power
	 * constraint if any */
	s8 maxTxPower;

	/*Context of the station being added in HW
	   Add a STA entry for "itself" -
	   On AP  - Add the AP itself in an "STA context"
	   On STA - Add the AP to which this STA is joining in an "STA context" */
	struct config_sta_params_v1 staContext;

	u8 vhtCapable;
	u8 vhtTxChannelWidthSet;
};

struct config_bss_req_msg {
	struct hal_msg_header header;
	union {
		struct config_bss_params configBssParams;
		struct config_bss_params_v1 configBssParams_V1;
	} uBssParams;
};

struct config_bss_rsp_params {
	/* Success or Failure */
	u32 status;

	/* BSS index allocated by HAL */
	u8 bssIdx;

	/* DPU descriptor index for PTK */
	u8 dpuDescIndx;

	/* PTK DPU signature */
	u8 ucastDpuSignature;

	/* DPU descriptor index for GTK */
	u8 bcastDpuDescIndx;

	/* GTK DPU signature */
	u8 bcastDpuSignature;

	/* DPU descriptor for IGTK */
	u8 mgmtDpuDescIndx;

	/* IGTK DPU signature */
	u8 mgmtDpuSignature;

	/* Station Index for BSS entry */
	u8 bssStaIdx;

	/* Self station index for this BSS */
	u8 bssSelfStaIdx;

	/* Bcast station for buffering bcast frames in AP role */
	u8 bssBcastStaIdx;

	/* MAC Address of STA(PEER/SELF) in staContext of configBSSReq */
	u8 staMac[ETH_ALEN];

	/* HAL fills in the tx power used for mgmt frames in this field. */
	s8 txMgmtPower;

};

struct config_bss_rsp_msg {
	struct hal_msg_header header;
	struct config_bss_rsp_params configBssRspParams;
};

struct delete_bss_req_msg {
	struct hal_msg_header header;

	/* BSS index to be deleted */
	u8 bssIdx;

};

struct delete_bss_rsp_msg {
	struct hal_msg_header header;

	/* Success or Failure */
	u32 status;

	/* BSS index that has been deleted */
	u8 bssIdx;

};

struct hal_join_req_msg {
	struct hal_msg_header header;

	/* Indicates the BSSID to which STA is going to associate */
	u8 bssId[ETH_ALEN];

	/* Indicates the channel to switch to. */
	u8 ucChannel;

	/* Self STA MAC */
	u8 selfStaMacAddr[ETH_ALEN];

	/* Local power constraint */
	u8 ucLocalPowerConstraint;

	/* Secondary channel offset */
	enum phy_chan_bond_state secondaryChannelOffset;

	/* link State */
	enum sir_link_state linkState;

	/* Max TX power */
	s8 maxTxPower;
};

struct hal_join_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

	/* HAL fills in the tx power used for mgmt frames in this field */
	u8 txMgmtPower;
};

struct post_assoc_req_msg {
	struct hal_msg_header header;

	struct config_sta_params configStaParams;
	struct config_bss_params configBssParams;
};

struct post_assoc_rsp_msg {
	struct hal_msg_header header;
	struct config_sta_rsp_params configStaRspParams;
	struct config_bss_rsp_params configBssRspParams;
};

/* This is used by PE to create a set of WEP keys for a given BSS. */
struct set_bss_key_req_msg {
	struct hal_msg_header header;

	/* BSS Index of the BSS */
	u8 bssIdx;

	/* Encryption Type used with peer */
	enum ani_ed_type encType;

	/* Number of keys */
	u8 numKeys;

	/* Array of keys. */
	struct sir_keys key[SIR_MAC_MAX_NUM_OF_DEFAULT_KEYS];

	/* Control for Replay Count, 1= Single TID based replay count on Tx
	 * 0 = Per TID based replay count on TX */
	u8 singleTidRc;
};

/* tagged version of set bss key */
struct set_bss_key_req_msg_tagged {
	struct set_bss_key_req_msg Msg;
	u32 Tag;
};

struct set_bss_key_rsp_msg {
	struct hal_msg_header header;

	/*success or failure */
	u32 status;

};

/*
 * This is used by PE to configure the key information on a given station.
 * When the secType is WEP40 or WEP104, the defWEPIdx is used to locate
 * a preconfigured key from a BSS the station assoicated with; otherwise
 * a new key descriptor is created based on the key field.
 */
struct set_sta_key_req_msg {
	struct hal_msg_header header;
	struct set_sta_key_params setStaKeyParams;
};

struct set_sta_key_rsp_msg {
	struct hal_msg_header header;

	/*success or failure */
	u32 status;
};

struct remove_bss_key_req_msg {
	struct hal_msg_header header;

	/* BSS Index of the BSS */
	u8 bssIdx;

	/* Encryption Type used with peer */
	enum ani_ed_type encType;

	/* Key Id */
	u8 keyId;

	/* STATIC/DYNAMIC. Used in Nullifying in Key Descriptors for
	 * Static/Dynamic keys */
	enum ani_wep_type wepType;

};

struct remove_bss_key_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

/*
 * This is used by PE to Remove the key information on a given station.
 */
struct remove_sta_key_req_msg {
	struct hal_msg_header header;

	/* STA Index */
	u16 staIdx;

	/* Encryption Type used with peer */
	enum ani_ed_type encType;

	/* Key Id */
	u8 keyId;

	/* Whether to invalidate the Broadcast key or Unicast key. In case
	 * of WEP, the same key is used for both broadcast and unicast. */
	u8 unicast;

};

struct remove_sta_key_rsp_msg {
	struct hal_msg_header header;

	/*success or failure */
	u32 status;

};

#ifdef FEATURE_OEM_DATA_SUPPORT

#ifndef OEM_DATA_REQ_SIZE
#define OEM_DATA_REQ_SIZE 134
#endif

#ifndef OEM_DATA_RSP_SIZE
#define OEM_DATA_RSP_SIZE 1968
#endif

struct start_oem_data_req_msg {
	struct hal_msg_header header;

	u32 status;
	tSirMacAddr selfMacAddr;
	u8 oemDataReq[OEM_DATA_REQ_SIZE];

};

struct start_oem_data_rsp_msg {
	struct hal_msg_header header;

	u8 oemDataRsp[OEM_DATA_RSP_SIZE];
};

#endif

struct switch_channel_req_msg {
	struct hal_msg_header header;

	/* Channel number */
	u8 channelNumber;

	/* Local power constraint */
	u8 localPowerConstraint;

	/* Secondary channel offset */
	enum phy_chan_bond_state secondaryChannelOffset;

	/* HAL fills in the tx power used for mgmt frames in this field. */
	u8 txMgmtPower;

	/* Max TX power */
	u8 maxTxPower;

	/* Self STA MAC */
	u8 selfStaMacAddr[ETH_ALEN];

	/* VO WIFI comment: BSSID needed to identify session. As the
	 * request has power constraints, this should be applied only to
	 * that session Since MTU timing and EDCA are sessionized, this
	 * struct needs to be sessionized and bssid needs to be out of the
	 * VOWifi feature flag V IMP: Keep bssId field at the end of this
	 * msg. It is used to mantain backward compatbility by way of
	 * ignoring if using new host/old FW or old host/new FW since it is
	 * at the end of this struct
	 */
	u8 bssId[ETH_ALEN];
};

struct switch_channel_rsp_msg {
	struct hal_msg_header header;

	/* Status */
	u32 status;

	/* Channel number - same as in request */
	u8 channelNumber;

	/* HAL fills in the tx power used for mgmt frames in this field */
	u8 txMgmtPower;

	/* BSSID needed to identify session - same as in request */
	u8 bssId[ETH_ALEN];

};

struct update_edca_params_req_msg {
	struct hal_msg_header header;

	/*BSS Index */
	u16 bssIdx;

	/* Best Effort */
	struct sir_edca_param_record acbe;

	/* Background */
	struct sir_edca_param_record acbk;

	/* Video */
	struct sir_edca_param_record acvi;

	/* Voice */
	struct sir_edca_param_record acvo;
};

struct update_edca_params_rsp_msg {
	struct hal_msg_header header;

	/*success or failure */
	u32 status;
};

struct dpu_stats_params
{
	/* Index of STA to which the statistics */
	u16 staIdx;

	/* Encryption mode */
	u8 encMode;

	/* status */
	u32 status;

	/* Statistics */
	u32 sendBlocks;
	u32 recvBlocks;
	u32 replays;
	u8 micErrorCnt;
	u32 protExclCnt;
	u16 formatErrCnt;
	u16 unDecryptableCnt;
	u32 decryptErrCnt;
	u32 decryptOkCnt;
};

struct hal_stats_req_msg {
	struct hal_msg_header header;

	/* Valid STA Idx for per STA stats request */
	u32 staId;

	/* Categories of stats requested as specified in eHalStatsMask */
	u32 statsMask;
};

struct ani_summary_stats_info {
	/* Total number of packets(per AC) that were successfully
	 * transmitted with retries */
	u32 retry_cnt[4];

	/* The number of MSDU packets and MMPDU frames per AC that the
	 * 802.11 station successfully transmitted after more than one
	 * retransmission attempt */
	u32 multiple_retry_cnt[4];

	/* Total number of packets(per AC) that were successfully
	 * transmitted (with and without retries, including multi-cast,
	 * broadcast) */
     	u32 tx_frm_cnt[4];


	/* Total number of packets that were successfully received (after
	 * appropriate filter rules including multi-cast, broadcast) */
	u32 rx_frm_cnt;	

	/* Total number of duplicate frames received successfully */
	u32 frm_dup_cnt;

	/* Total number packets(per AC) failed to transmit */
	u32 fail_cnt[4];

	/* Total number of RTS/CTS sequence failures for transmission of a
	 * packet */
	u32 rts_fail_cnt;

	/* Total number packets failed transmit because of no ACK from the
	 * remote entity */
	u32 ack_fail_cnt;

	/* Total number of RTS/CTS sequence success for transmission of a
	 * packet */
	u32 rts_succ_cnt;

	/* The sum of the receive error count and dropped-receive-buffer
	 * error count. HAL will provide this as a sum of (FCS error) +
	 * (Fail get BD/PDU in HW) */
	u32 rx_discard_cnt;

	/*
	 * The receive error count. HAL will provide the RxP FCS error
	 * global counter. */
	u32 rx_error_cnt;

	/* The sum of the transmit-directed byte count, transmit-multicast
	 * byte count and transmit-broadcast byte count. HAL will sum TPE
	 * UC/MC/BCAST global counters to provide this. */
	u32 tx_byte_cnt;
};

/* defines tx_rate_flags */
enum tx_rate_info {
	/* Legacy rates */
	eHAL_TX_RATE_LEGACY = 0x1,

	/* HT20 rates */
	eHAL_TX_RATE_HT20 = 0x2,

	/* HT40 rates */
	eHAL_TX_RATE_HT40 = 0x4,

	/* Rate with Short guard interval */
	eHAL_TX_RATE_SGI = 0x8,

	/* Rate with Long guard interval */
	eHAL_TX_RATE_LGI = 0x10
};

struct ani_global_class_a_stats_info {
	/* The number of MPDU frames received by the 802.11 station for
	 * MSDU packets or MMPDU frames */
	u32 rx_frag_cnt;

	/* The number of MPDU frames received by the 802.11 station for
	 * MSDU packets or MMPDU frames when a promiscuous packet filter
	 * was enabled */
	u32 promiscuous_rx_frag_cnt;

	/* The receiver input sensitivity referenced to a FER of 8% at an
	 * MPDU length of 1024 bytes at the antenna connector. Each element
	 * of the array shall correspond to a supported rate and the order
	 * shall be the same as the supporteRates parameter. */
	u32 rx_input_sensitivity;

	/* The maximum transmit power in dBm upto one decimal. for eg: if
	 * it is 10.5dBm, the value would be 105 */
	u32 max_pwr;

	/* Number of times the receiver failed to synchronize with the
	 * incoming signal after detecting the sync in the preamble of the
	 * transmitted PLCP protocol data unit. */
	u32 sync_fail_cnt;

	/* Legacy transmit rate, in units of 500 kbit/sec, for the most
	 * recently transmitted frame */
	u32 tx_rate;

	/* mcs index for HT20 and HT40 rates */
	u32 mcs_index;

	/* to differentiate between HT20 and HT40 rates; short and long
	 * guard interval */
	u32 tx_rate_flags;
};

struct ani_global_security_stats {
	/* The number of unencrypted received MPDU frames that the MAC
	 * layer discarded when the IEEE 802.11 dot11ExcludeUnencrypted
	 * management information base (MIB) object is enabled */
	u32 rx_wep_unencrypted_frm_cnt;

	/* The number of received MSDU packets that that the 802.11 station
	 * discarded because of MIC failures */
	u32 rx_mic_fail_cnt;

	/* The number of encrypted MPDU frames that the 802.11 station
	 * failed to decrypt because of a TKIP ICV error */
	u32 tkip_icv_err;

	/* The number of received MPDU frames that the 802.11 discarded
	 * because of an invalid AES-CCMP format */
	u32 aes_ccmp_format_err;

	/* The number of received MPDU frames that the 802.11 station
	 * discarded because of the AES-CCMP replay protection procedure */
	u32 aes_ccmp_replay_cnt;

	/* The number of received MPDU frames that the 802.11 station
	 * discarded because of errors detected by the AES-CCMP decryption
	 * algorithm */
	u32 aes_ccmp_decrpt_err;

	/* The number of encrypted MPDU frames received for which a WEP
	 * decryption key was not available on the 802.11 station */
	u32 wep_undecryptable_cnt;

	/* The number of encrypted MPDU frames that the 802.11 station
	 * failed to decrypt because of a WEP ICV error */
	u32 wep_icv_err;

	/* The number of received encrypted packets that the 802.11 station
	 * successfully decrypted */
	u32 rx_decrypt_succ_cnt;

	/* The number of encrypted packets that the 802.11 station failed
	 * to decrypt */
	u32 rx_decrypt_fail_cnt;
};

struct ani_global_class_b_stats_info {
	struct ani_global_security_stats ucStats;
	struct ani_global_security_stats mcbcStats;
};

struct ani_global_class_c_stats_info {
	/* This counter shall be incremented for a received A-MSDU frame
	 * with the stations MAC address in the address 1 field or an
	 * A-MSDU frame with a group address in the address 1 field */
	u32 rx_amsdu_cnt;

	/* This counter shall be incremented when the MAC receives an AMPDU
	 * from the PHY */
	u32 rx_ampdu_cnt;

	/* This counter shall be incremented when a Frame is transmitted
	 * only on the primary channel */
	u32 tx_20_frm_cnt;

	/* This counter shall be incremented when a Frame is received only
	 * on the primary channel */
	u32 rx_20_frm_cnt;

	/* This counter shall be incremented by the number of MPDUs
	 * received in the A-MPDU when an A-MPDU is received */
	u32 rx_mpdu_in_ampdu_cnt;

	/* This counter shall be incremented when an MPDU delimiter has a
	 * CRC error when this is the first CRC error in the received AMPDU
	 * or when the previous delimiter has been decoded correctly */
	u32 ampdu_delimiter_crc_err;
};

struct ani_per_sta_stats_info {
	/* The number of MPDU frames that the 802.11 station transmitted
	 * and acknowledged through a received 802.11 ACK frame */
	u32 tx_frag_cnt[4];

	/* This counter shall be incremented when an A-MPDU is transmitted */
	u32 tx_ampdu_cnt;

	/* This counter shall increment by the number of MPDUs in the AMPDU
	 * when an A-MPDU is transmitted */
	u32 tx_mpdu_in_ampdu_cnt;
};

struct hal_stats_rsp_msg {
	struct hal_msg_header header;

	/* Success or Failure */
	u32 status;

	/* STA Idx */
	u32 staId;

	/* Categories of STATS being returned as per eHalStatsMask */
	u32 statsMask;

	/* message type is same as the request type */
	u16 msgType;

	/* length of the entire request, includes the pStatsBuf length too */
	u16 msgLen;

};

struct set_link_state_req_msg {
	struct hal_msg_header header;

	u8 bssid[ETH_ALEN];
	enum sir_link_state state;
	u8 selfMacAddr[ETH_ALEN];

};

struct set_link_state_rsp_msg {
	struct hal_msg_header header;

	/*success or failure */
	u32 status;

};

/* TSPEC Params */
struct sir_ts_info_tfc {
#ifndef ANI_LITTLE_BIT_ENDIAN
	u16 ackPolicy:2;
	u16 userPrio:3;
	u16 psb:1;
	u16 aggregation:1;
	u16 accessPolicy:2;
	u16 direction:2;
	u16 tsid:4;
	u16 trafficType:1;
#else
	u16 trafficType:1;
	u16 tsid:4;
	u16 direction:2;
	u16 accessPolicy:2;
	u16 aggregation:1;
	u16 psb:1;
	u16 userPrio:3;
	u16 ackPolicy:2;
#endif
};

/* Flag to schedule the traffic type */
struct sir_ts_info_sch {
#ifndef ANI_LITTLE_BIT_ENDIAN
	u8 rsvd:7;
	u8 schedule:1;
#else
	u8 schedule:1;
	u8 rsvd:7;
#endif
};

/* Traffic and scheduling info */
struct sir_ts_info {
	struct sir_ts_info_tfc traffic;
	struct sir_ts_info_sch schedule;
};

/* Information elements */
struct sir_tspec_ie {
	u8 type;
	u8 length;
	struct sir_ts_info tsinfo;
	u16 nomMsduSz;
	u16 maxMsduSz;
	u32 minSvcInterval;
	u32 maxSvcInterval;
	u32 inactInterval;
	u32 suspendInterval;
	u32 svcStartTime;
	u32 minDataRate;
	u32 meanDataRate;
	u32 peakDataRate;
	u32 maxBurstSz;
	u32 delayBound;
	u32 minPhyRate;
	u16 surplusBw;
	u16 mediumTime;
};

struct add_ts_req_msg {
	struct hal_msg_header header;

	/* Station Index */
	u16 staIdx;

	/* TSPEC handler uniquely identifying a TSPEC for a STA in a BSS */
	u16 tspecIdx;

	/* To program TPE with required parameters */
	struct sir_tspec_ie tspec;

	/* U-APSD Flags: 1b per AC.  Encoded as follows:
	   b7 b6 b5 b4 b3 b2 b1 b0 =
	   X  X  X  X  BE BK VI VO */
	u8 uAPSD;

	/* These parameters are for all the access categories */

	/* Service Interval */
	u32 srvInterval[WCN36XX_HAL_MAX_AC];

	/* Suspend Interval */
	u32 susInterval[WCN36XX_HAL_MAX_AC];

	/* Delay Interval */
	u32 delayInterval[WCN36XX_HAL_MAX_AC];
};

struct add_rs_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct del_ts_req_msg {
	struct hal_msg_header header;

	/* Station Index */
	u16 staIdx;

	/* TSPEC identifier uniquely identifying a TSPEC for a STA in a BSS */
	u16 tspecIdx;

	/* To lookup station id using the mac address */
	u8 bssId[ETH_ALEN];
};

struct del_ts_rsp_msg {
	struct hal_msg_header header;

	/*success or failure */
	u32 status;

};

/* End of TSpec Parameters */

/* Start of BLOCK ACK related Parameters */

struct add_ba_session_req_msg {
	struct hal_msg_header header;

	/* Station Index */
	u16 staIdx;

	/* Peer MAC Address */
	u8 peerMacAddr[ETH_ALEN];

	/* ADDBA Action Frame dialog token
	   HAL will not interpret this object */
	u8 baDialogToken;

	/* TID for which the BA is being setup
	   This identifies the TC or TS of interest */
	u8 baTID;

	/* 0 - Delayed BA (Not supported)
	   1 - Immediate BA */
	u8 baPolicy;

	/* Indicates the number of buffers for this TID (baTID)
	   NOTE - This is the requested buffer size. When this
	   is processed by HAL and subsequently by HDD, it is
	   possible that HDD may change this buffer size. Any
	   change in the buffer size should be noted by PE and
	   advertized appropriately in the ADDBA response */
	u16 baBufferSize;

	/* BA timeout in TU's 0 means no timeout will occur */
	u16 baTimeout;

	/* b0..b3 - Fragment Number - Always set to 0
	   b4..b15 - Starting Sequence Number of first MSDU
	   for which this BA is setup */
	u16 baSSN;

	/* ADDBA direction
	   1 - Originator
	   0 - Recipient */
	u8 baDirection;
};

struct add_ba_session_rsp_msg {
	struct hal_msg_header header;

	/*success or failure */
	u32 status;

	/* Dialog token */
	u8 baDialogToken;

	/* TID for which the BA session has been setup */
	u8 baTID;

	/* BA Buffer Size allocated for the current BA session */
	u8 baBufferSize;

	u8 baSessionID;

	/* Reordering Window buffer */
	u8 winSize;

	/*Station Index to id the sta */
	u8 STAID;

	/* Starting Sequence Number */
	u16 SSN;
};

struct add_ba_req_msg {
	struct hal_msg_header header;

	/* Session Id */
	u8 baSessionID;

	/* Reorder Window Size */
	u8 winSize;

#ifdef FEATURE_ON_CHIP_REORDERING
	u8 isReorderingDoneOnChip;
#endif
};

struct add_ba_rsp_msg {
	struct hal_msg_header header;

	/*success or failure */
	u32 status;

	/* Dialog token */
	u8 baDialogToken;
};

struct add_ba_info {
	u16 fBaEnable:1;
	u16 startingSeqNum:12;
	u16 reserved:3;
};

struct trigger_ba_rsp_candidate {
	u8 staAddr[ETH_ALEN];
	struct add_ba_info baInfo[STACFG_MAX_TC];
};

struct trigget_ba_req_candidate {
	u8 staIdx;
	u8 tidBitmap;
};

struct trigger_ba_req_msg {
	struct hal_msg_header header;

	/* Session Id */
	u8 baSessionID;

	/* baCandidateCnt is followed by trigger BA 
	 * Candidate List(tTriggerBaCandidate)
	 */
	u16 baCandidateCnt;

};

struct trigger_ba_rsp_msg {
	struct hal_msg_header header;

	/* TO SUPPORT BT-AMP */
	u8 bssId[ETH_ALEN];

	/* success or failure */
	u32 status;

	/* baCandidateCnt is followed by trigger BA 
	 * Rsp Candidate List(tTriggerRspBaCandidate)
	 */
	u16 baCandidateCnt;
};

struct del_ba_req_msg {
	struct hal_msg_header header;

	/* Station Index */
	u16 staIdx;

	/* TID for which the BA session is being deleted */
	u8 baTID;

	/* DELBA direction
	   1 - Originator
	   0 - Recipient */
	u8 baDirection;

};

struct del_ba_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct tsm_stats_req_msg {
	struct hal_msg_header header;

	/* Traffic Id */
	u8 tsmTID;

	u8 bssId[ETH_ALEN];
};

struct tsm_stats_rsp_msg {
	struct hal_msg_header header;

	/*success or failure */
	u32 status;

	/* Uplink Packet Queue delay */
	u16 UplinkPktQueueDly;

	/* Uplink Packet Queue delay histogram */
	u16 UplinkPktQueueDlyHist[4];

	/* Uplink Packet Transmit delay */
	u32 UplinkPktTxDly;

	/* Uplink Packet loss */
	u16 UplinkPktLoss;

	/* Uplink Packet count */
	u16 UplinkPktCount;

	/* Roaming count */
	u8 RoamingCount;

	/* Roaming Delay */
	u16 RoamingDly;

};

struct set_key_done_msg {
	struct hal_msg_header header;

	/*bssid of the keys */
	u8 bssidx;
	u8 encType;
};

struct hal_nv_img_download_req_msg {
	/* Note: The length specified in tHalNvImgDownloadReqMsg messages
	 * should be header.msgLen = sizeof(tHalNvImgDownloadReqMsg) +
	 * nvImgBufferSize */
	struct hal_msg_header header;

	/* Fragment sequence number of the NV Image. Note that NV Image
	 * might not fit into one message due to size limitation of the SMD
	 * channel FIFO. UMAC can hence choose to chop the NV blob into
	 * multiple fragments starting with seqeunce number 0, 1, 2 etc.
	 * The last fragment MUST be indicated by marking the
	 * isLastFragment field to 1. Note that all the NV blobs would be
	 * concatenated together by HAL without any padding bytes in
	 * between.*/
	u16 fragNumber;

	/* Is this the last fragment? When set to 1 it indicates that no
	 * more fragments will be sent by UMAC and HAL can concatenate all
	 * the NV blobs rcvd & proceed with the parsing. HAL would generate
	 * a WCN36XX_HAL_DOWNLOAD_NV_RSP to the WCN36XX_HAL_DOWNLOAD_NV_REQ after
	 * it receives each fragment */
	u16 isLastFragment;

	/* NV Image size (number of bytes) */
	u32 nvImgBufferSize;

	/* Following the 'nvImageBufferSize', there should be
	 * nvImageBufferSize bytes of NV Image i.e.
	 * u8[nvImageBufferSize] */

};

struct hal_nv_img_download_rsp_msg {
	struct hal_msg_header header;

	/* Success or Failure. HAL would generate a
	 * WCN36XX_HAL_DOWNLOAD_NV_RSP after each fragment */
	u32 status;
};

struct hal_nv_store_ind {
	/* Note: The length specified in tHalNvStoreInd messages should be
	 * header.msgLen = sizeof(tHalNvStoreInd) + nvBlobSize */
	struct hal_msg_header header;

	/* NV Item */
	u32 tableID;

	/* Size of NV Blob */
	u32 nvBlobSize;

	/* Following the 'nvBlobSize', there should be nvBlobSize bytes of
	 * NV blob i.e. u8[nvBlobSize] */
};

/* End of Block Ack Related Parameters */

#define SIR_CIPHER_SEQ_CTR_SIZE 6

/* Definition for MIC failure indication MAC reports this each time a MIC
 * failure occures on Rx TKIP packet
 */
struct mic_failure_ind_msg {
	struct hal_msg_header header;

	u8 bssId[ETH_ALEN];

	/* address used to compute MIC */
	u8 srcMacAddr[ETH_ALEN];

	/* transmitter address */
	u8 taMacAddr[ETH_ALEN];

	u8 dstMacAddr[ETH_ALEN];

	u8 multicast;

	/* first byte of IV */
	u8 IV1;

	/* second byte of IV */
	u8 keyId;

	/* sequence number */
	u8 TSC[SIR_CIPHER_SEQ_CTR_SIZE];

	/* receive address */
	u8 rxMacAddr[ETH_ALEN];
};

struct update_vht_op_mode_req_msg {
	struct hal_msg_header header;

	u16 opMode;
	u16 staId;

};

struct update_vht_op_mode_params_rsp_msg {
	struct hal_msg_header header;

	u32 status;
};

struct update_beacon_req_msg {
	struct hal_msg_header header;

	u8 bssIdx;

	/* shortPreamble mode. HAL should update all the STA rates when it
	 * receives this message */
	u8 fShortPreamble;

	/* short Slot time. */
	u8 fShortSlotTime;

	/* Beacon Interval */
	u16 beaconInterval;

	/* Protection related */
	u8 llaCoexist;
	u8 llbCoexist;
	u8 llgCoexist;
	u8 ht20MhzCoexist;
	u8 llnNonGFCoexist;
	u8 fLsigTXOPProtectionFullSupport;
	u8 fRIFSMode;

	u16 paramChangeBitmap;
};

struct update_beacon_rsp_msg {
	struct hal_msg_header header;
	u32 status;
};

struct send_beacon_req_msg {
	struct hal_msg_header header;

	/* length of the template. */
	u32 beaconLength;

	/* Beacon data. */
	u8 beacon[BEACON_TEMPLATE_SIZE];

	u8 bssId[ETH_ALEN];

	/* TIM IE offset from the beginning of the template. */
	u32 timIeOffset;

	/* P2P IE offset from the begining of the template */
	u16 p2pIeOffset;
};

struct send_beacon_rsp_msg {
	struct hal_msg_header header;
	u32 status;
};

struct enable_radar_req_msg {
	struct hal_msg_header header;

	u8 BSSID[ETH_ALEN];
	u8 channel;
};

struct enable_radar_rsp_msg {
	struct hal_msg_header header;

	/* Link Parameters */
	u8 BSSID[ETH_ALEN];

	/* success or failure */
	u32 status;
};

struct radar_detect_intr_ind_msg {
	struct hal_msg_header header;

	u8 radarDetChannel;

};

struct radar_detect_ind_msg {
	struct hal_msg_header header;

	/* channel number in which the RADAR detected */
	u8 channelNumber;

	/* RADAR pulse width in usecond */
	u16 radarPulseWidth;

	/* Number of RADAR pulses */
	u16 numRadarPulse;
};

struct sir_get_tpc_report_req_msg {
	struct hal_msg_header header;

	u8 sta[ETH_ALEN];
	u8 dialogToken;
	u8 txpower;
};

struct sir_get_tpc_report_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
};

struct send_probe_resp_req_msg {
	struct hal_msg_header header;

	u8 pProbeRespTemplate[BEACON_TEMPLATE_SIZE];
	u32 probeRespTemplateLen;
	u32 ucProxyProbeReqValidIEBmap[8];
	u8 bssId[ETH_ALEN];
};

struct send_probe_resp_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
};

struct send_unknown_frame_rx_ind_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
};

struct delete_sta_context_ind_msg {
	struct hal_msg_header header;

	u16 assocId;
	u16 staId;

	/* TO SUPPORT BT-AMP */
	u8 bssId[ETH_ALEN];

	/* HAL copies bssid from the sta table. */
	u8 addr2[ETH_ALEN];

	/* To unify the keepalive / unknown A2 / tim-based disa */
	u16 reasonCode;	
};

struct indicate_del_sta {
	struct hal_msg_header header;
	u8 assocId;
	u8 staIdx;
	u8 bssIdx;
	u8 uReasonCode;
	u32 uStatus;
};

struct bt_amp_event_msg {
	struct hal_msg_header header;

	enum bt_amp_event_type btAmpEventType;

};

struct bt_amp_event_rsp {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
};

struct tl_hal_flush_ac_req_msg {
	struct hal_msg_header header;

	/* Station Index. originates from HAL */
	u8 ucSTAId;

	/* TID for which the transmit queue is being flushed */
	u8 ucTid;

};

struct tl_hal_flush_ac_rsp_msg {
	struct hal_msg_header header;

	/* Station Index. originates from HAL */
	u8 ucSTAId;

	/* TID for which the transmit queue is being flushed */
	u8 ucTid;

	/* success or failure */
	u32 status;
};

struct hal_enter_imps_req_msg {
	struct hal_msg_header header;
};

struct hal_exit_imps_req {
	struct hal_msg_header header;
};

struct hal_enter_bmps_req_msg {
	struct hal_msg_header header;

	u8 bssIdx;

	/* TBTT value derived from the last beacon */
#ifndef BUILD_QWPTTSTATIC
	u64 tbtt;
#endif
	u8 dtimCount;

	/* DTIM period given to HAL during association may not be valid, if
	 * association is based on ProbeRsp instead of beacon. */
	u8 dtimPeriod;

	/* For CCX and 11R Roaming */
	u32 rssiFilterPeriod;

	u32 numBeaconPerRssiAverage;
	u8 bRssiFilterEnable;
};

struct hal_exit_bmps_req_msg {
	struct hal_msg_header header;

	u8 sendDataNull;
	u8 bssIdx;
};

struct hal_missed_beacon_ind_msg {
	struct hal_msg_header header;

	u8 bssIdx;
};

/* Beacon Filtering data structures */

/* The above structure would be followed by multiple of below mentioned structure */
struct beacon_filter_ie {
	u8 elementId;
	u8 checkIePresence;
	u8 offset;
	u8 value;
	u8 bitMask;
	u8 ref;
};

struct hal_add_bcn_filter_req_msg {
	struct hal_msg_header header;

	u16 capabilityInfo;
	u16 capabilityMask;
	u16 beaconInterval;
	u16 ieNum;
	u8 bssIdx;
	u8 reserved;
};

struct hal_rem_bcn_filter_req {
	struct hal_msg_header header;

	u8 ucIeCount;
	u8 ucRemIeId[1];
};

#define HAL_IPV4_ARP_REPLY_OFFLOAD                  0
#define HAL_IPV6_NEIGHBOR_DISCOVERY_OFFLOAD         1
#define HAL_IPV6_NS_OFFLOAD                         2
#define HAL_IPV6_ADDR_LEN                           16
#define HAL_MAC_ADDR_LEN                            6
#define HAL_OFFLOAD_DISABLE                         0
#define HAL_OFFLOAD_ENABLE                          1
#define HAL_OFFLOAD_BCAST_FILTER_ENABLE             0x2
#define HAL_OFFLOAD_ARP_AND_BCAST_FILTER_ENABLE     (HAL_OFFLOAD_ENABLE|HAL_OFFLOAD_BCAST_FILTER_ENABLE)

struct hal_ns_offload_params {
	u8 srcIPv6Addr[HAL_IPV6_ADDR_LEN];
	u8 selfIPv6Addr[HAL_IPV6_ADDR_LEN];

	/* Only support 2 possible Network Advertisement IPv6 address */
	u8 targetIPv6Addr1[HAL_IPV6_ADDR_LEN];
	u8 targetIPv6Addr2[HAL_IPV6_ADDR_LEN];

	u8 selfMacAddr[HAL_MAC_ADDR_LEN];
	u8 srcIPv6AddrValid:1;
	u8 targetIPv6Addr1Valid:1;
	u8 targetIPv6Addr2Valid:1;
	u8 reserved1:5;

	/* make it DWORD aligned */
	u8 reserved2;

	/* slot index for this offload */
	u32 slotIndex;
	u8 bssIdx;
};

struct hal_host_offload_req {
	u8 offloadType;
	u8 enableOrDisable;
	union {
		u8 hostIpv4Addr[4];
		u8 hostIpv6Addr[HAL_IPV6_ADDR_LEN];
	} params;
};

struct hal_host_offload_req_msg {
	struct hal_msg_header header;
	struct hal_host_offload_req hostOffloadParams;
	struct hal_ns_offload_params nsOffloadParams;
};

/* Packet Types. */
#define HAL_KEEP_ALIVE_NULL_PKT              1
#define HAL_KEEP_ALIVE_UNSOLICIT_ARP_RSP     2

/* Enable or disable keep alive */
#define HAL_KEEP_ALIVE_DISABLE   0
#define HAL_KEEP_ALIVE_ENABLE    1

/* Keep Alive request. */
struct hal_keep_alive_req_msg {
	struct hal_msg_header header;

	u8 packetType;
	u32 timePeriod;
	tHalIpv4Addr hostIpv4Addr;
	tHalIpv4Addr destIpv4Addr;
	u8 destMacAddr[ETH_ALEN];
	u8 bssIdx;
};

struct hal_rssi_threshold_req_msg {
	struct hal_msg_header header;

	s8 ucRssiThreshold1:8;
	s8 ucRssiThreshold2:8;
	s8 ucRssiThreshold3:8;
	u8 bRssiThres1PosNotify:1;
	u8 bRssiThres1NegNotify:1;
	u8 bRssiThres2PosNotify:1;
	u8 bRssiThres2NegNotify:1;
	u8 bRssiThres3PosNotify:1;
	u8 bRssiThres3NegNotify:1;
	u8 bReserved10:2;

};

struct hal_enter_uapsd_req_msg {
	struct hal_msg_header header;

	u8 bkDeliveryEnabled:1;
	u8 beDeliveryEnabled:1;
	u8 viDeliveryEnabled:1;
	u8 voDeliveryEnabled:1;
	u8 bkTriggerEnabled:1;
	u8 beTriggerEnabled:1;
	u8 viTriggerEnabled:1;
	u8 voTriggerEnabled:1;
	u8 bssIdx;

};

struct hal_exit_uapsd_req_msg {
	struct hal_msg_header header;
	u8 bssIdx;
};

#define HAL_WOWL_BCAST_PATTERN_MAX_SIZE 128
#define HAL_WOWL_BCAST_MAX_NUM_PATTERNS 16

struct hal_wowl_add_bcast_ptrn_req_msg {
	struct hal_msg_header header;

	/* Pattern ID */
	u8 ucPatternId;

	/* Pattern byte offset from beginning of the 802.11 packet to start
	 * of the wake-up pattern */
	u8 ucPatternByteOffset;

	/* Non-Zero Pattern size */
	u8 ucPatternSize;

	/* Pattern */
	u8 ucPattern[HAL_WOWL_BCAST_PATTERN_MAX_SIZE];

	/* Non-zero pattern mask size */
	u8 ucPatternMaskSize;

	/* Pattern mask */
	u8 ucPatternMask[HAL_WOWL_BCAST_PATTERN_MAX_SIZE];

	/* Extra pattern */
	u8 ucPatternExt[HAL_WOWL_BCAST_PATTERN_MAX_SIZE];

	/* Extra pattern mask */
	u8 ucPatternMaskExt[HAL_WOWL_BCAST_PATTERN_MAX_SIZE];

	u8 bssIdx;
};

struct hal_wow_del_bcast_ptrn_req_msg {
	struct hal_msg_header header;

	/* Pattern ID of the wakeup pattern to be deleted */
	u8 ucPatternId;
	u8 bssIdx;

};

struct hal_wowl_enter_req_msg {
	struct hal_msg_header header;

	/* Enables/disables magic packet filtering */
	u8 ucMagicPktEnable;

	/* Magic pattern */
	u8 magicPtrn[ETH_ALEN];

	/* Enables/disables packet pattern filtering in firmware. Enabling
	 * this flag enables broadcast pattern matching in Firmware. If
	 * unicast pattern matching is also desired,
	 * ucUcastPatternFilteringEnable flag must be set tot true as well
	 */
	u8 ucPatternFilteringEnable;

	/* Enables/disables unicast packet pattern filtering. This flag
	 * specifies whether we want to do pattern match on unicast packets
	 * as well and not just broadcast packets. This flag has no effect
	 * if the ucPatternFilteringEnable (main controlling flag) is set
	 * to false
	 */
	u8 ucUcastPatternFilteringEnable;

	/* This configuration is valid only when magicPktEnable=1. It
	 * requests hardware to wake up when it receives the Channel Switch
	 * Action Frame.
	 */
	u8 ucWowChnlSwitchRcv;

	/* This configuration is valid only when magicPktEnable=1. It
	 * requests hardware to wake up when it receives the
	 * Deauthentication Frame.
	 */
	u8 ucWowDeauthRcv;

	/* This configuration is valid only when magicPktEnable=1. It
	 * requests hardware to wake up when it receives the Disassociation
	 * Frame.
	 */
	u8 ucWowDisassocRcv;

	/* This configuration is valid only when magicPktEnable=1. It
	 * requests hardware to wake up when it has missed consecutive
	 * beacons. This is a hardware register configuration (NOT a
	 * firmware configuration).
	 */
	u8 ucWowMaxMissedBeacons;

	/* This configuration is valid only when magicPktEnable=1. This is
	 * a timeout value in units of microsec. It requests hardware to
	 * unconditionally wake up after it has stayed in WoWLAN mode for
	 * some time. Set 0 to disable this feature.
	 */
	u8 ucWowMaxSleepUsec;

	/* This configuration directs the WoW packet filtering to look for
	 * EAP-ID requests embedded in EAPOL frames and use this as a wake
	 * source.
	 */
	u8 ucWoWEAPIDRequestEnable;

	/* This configuration directs the WoW packet filtering to look for
	 * EAPOL-4WAY requests and use this as a wake source.
	 */
	u8 ucWoWEAPOL4WayEnable;

	/* This configuration allows a host wakeup on an network scan
	 * offload match.
	 */
	u8 ucWowNetScanOffloadMatch;

	/* This configuration allows a host wakeup on any GTK rekeying
	 * error.
	 */
	u8 ucWowGTKRekeyError;

	/* This configuration allows a host wakeup on BSS connection loss.
	 */
	u8 ucWoWBSSConnLoss;

	u8 bssIdx;

};

struct hal_wowl_exit_req_msg {
	struct hal_msg_header header;

	u8 bssIdx;
};

struct hal_get_rssi_req_msg {
	struct hal_msg_header header;
};

struct hal_get_roam_rssi_req_msg {
	struct hal_msg_header header;

	/* Valid STA Idx for per STA stats request */
	u32 staId;

};

struct hal_set_uapsd_ac_params_req_msg {
	struct hal_msg_header header;

	/* STA index */
	u8 staidx;

	/* Access Category */
	u8 ac;

	/* User Priority */
	u8 up;

	/* Service Interval */
	u32 srvInterval;

	/* Suspend Interval */
	u32 susInterval;

	/* Delay Interval */
	u32 delayInterval;
};

struct hal_configure_rxp_filter_req_msg {
	struct hal_msg_header header;

	u8 setMcstBcstFilterSetting;
	u8 setMcstBcstFilter;
};

struct hal_enter_imps_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
};

struct hal_exit_imps_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
};

struct hal_enter_bmps_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
	u8 bssIdx;
};

struct hal_exit_bmps_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
	u8 bssIdx;

};

struct hal_enter_uapsd_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
	u8 bssIdx;

};

struct hal_exit_uapsd_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
	u8 bssIdx;

};

struct hal_rssi_notification_ind_msg {
	struct hal_msg_header header;

	u32 bRssiThres1PosCross:1;
	u32 bRssiThres1NegCross:1;
	u32 bRssiThres2PosCross:1;
	u32 bRssiThres2NegCross:1;
	u32 bRssiThres3PosCross:1;
	u32 bRssiThres3NegCross:1;
	u32 avgRssi:8;
	u32 bReserved:18;

};

struct hal_get_rssio_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
	s8 rssi;

};

struct hal_get_roam_rssi_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

	u8 staId;
	s8 rssi;

};

struct hal_wowl_enter_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
	u8 bssIdx;

};

struct hal_wowl_exit_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
	u8 bssIdx;

};

struct hal_add_bcn_filter_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct hal_rem_bcn_filter_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct hal_add_wowl_bcast_ptrn_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
	u8 bssIdx;

};

struct hal_del_wowl_bcast_ptrn_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
	u8 bssIdx;

};

struct hal_host_offload_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct hal_keep_alive_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct hal_set_rssi_thresh_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct hal_set_uapsd_ac_params_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct hal_configure_rxp_filter_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct set_max_tx_pwr_req {
	struct hal_msg_header header;

	/* BSSID is needed to identify which session issued this request.
	 * As the request has power constraints, this should be applied
	 * only to that session */
	u8 bssId[ETH_ALEN];

	u8 selfStaMacAddr[ETH_ALEN];

	/* In request, power == MaxTx power to be used. */
	u8 power;
};

struct set_max_tx_pwr_rsp_msg {
	struct hal_msg_header header;

	/* power == tx power used for management frames */
	u8 power;

	/* success or failure */
	u32 status;
};

struct set_tx_pwr_req_msg {
	struct hal_msg_header header;

	/* TX Power in milli watts */
	u32 txPower;
	u8 bssIdx;
};

struct set_tx_pwr_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
};

struct get_tx_pwr_req_msg {
	struct hal_msg_header header;

	u8 staId;
};

struct get_tx_pwr_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

	/* TX Power in milli watts */
	u32 txPower;
};

struct set_p2p_gonoa_req_msg {
	struct hal_msg_header header;

	u8 opp_ps;
	u32 ctWindow;
	u8 count;
	u32 duration;
	u32 interval;
	u32 single_noa_duration;
	u8 psSelection;
};

struct set_p2p_gonoa_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct add_sta_self_req {
	struct hal_msg_header header;

	u8 selfMacAddr[ETH_ALEN];
	u32 status;
};

struct add_sta_self_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

	/*Self STA Index */
	u8 selfStaIdx;

	/* DPU Index (IGTK, PTK, GTK all same) */
	u8 dpuIdx;

	/* DPU Signature */
	u8 dpuSignature;
};

struct del_sta_self_req_msg {
	struct hal_msg_header header;

	u8 selfMacAddr[ETH_ALEN];

};

struct del_sta_self_rsp_msg {
	struct hal_msg_header header;

	/*success or failure */
	u32 status;

	u8 selfMacAddr[ETH_ALEN];

};

struct aggr_add_ts_req {
	struct hal_msg_header header;

	/* Station Index */
	u16 staIdx;

	/* TSPEC handler uniquely identifying a TSPEC for a STA in a BSS.
	 * This will carry the bitmap with the bit positions representing
	 * different AC.s */
	u16 tspecIdx;

	/* Tspec info per AC To program TPE with required parameters */
	struct sir_tspec_ie tspec[WCN36XX_HAL_MAX_AC];

	/* U-APSD Flags: 1b per AC.  Encoded as follows:
	   b7 b6 b5 b4 b3 b2 b1 b0 =
	   X  X  X  X  BE BK VI VO */
	u8 uAPSD;

	/* These parameters are for all the access categories */

	/* Service Interval */
	u32 srvInterval[WCN36XX_HAL_MAX_AC];

	/* Suspend Interval */
	u32 susInterval[WCN36XX_HAL_MAX_AC];

	/* Delay Interval */
	u32 delayInterval[WCN36XX_HAL_MAX_AC];
};

struct aggr_add_ts_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status0;

	/* FIXME PRIMA for future use for 11R */
	u32 status1;
};

struct hal_configure_apps_cpu_wakeup_state_req_msg {
	struct hal_msg_header header;

	u8 isAppsCpuAwake;

};

struct hal_configure_apps_cpu_wakeup_state_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct hal_dump_cmd_req_msg {
	struct hal_msg_header header;

	u32 argument1;
	u32 argument2;
	u32 argument3;
	u32 argument4;
	u32 argument5;

};

struct hal_dump_cmd_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

	/* Length of the responce message */
	u32 rspLength;

	/* FIXME: Currently considering the the responce will be less than
	 * 100bytes */
	u8 rspBuffer[DUMPCMD_RSP_BUFFER];

};

#define WLAN_COEX_IND_DATA_SIZE (4)
#define WLAN_COEX_IND_TYPE_DISABLE_HB_MONITOR (0)
#define WLAN_COEX_IND_TYPE_ENABLE_HB_MONITOR (1)

struct coex_ind_msg {
	struct hal_msg_header header;

	/* Coex Indication Type */
	u32 coexIndType;

	/* Coex Indication Data */
	u32 coexIndData[WLAN_COEX_IND_DATA_SIZE];
};

struct tx_compl_ind_msg {
	struct hal_msg_header header;

	/* Tx Complete Indication Success or Failure */
	u32 status;

};

struct hal_wlan_host_suspend_ind_msg {
	struct hal_msg_header header;

	u32 configuredMcstBcstFilterSetting;
	u32 activeSessionCount;
};

struct hal_wlan_exclude_unencrpted_ind_msg {
	struct hal_msg_header header;

	u8 bDot11ExcludeUnencrypted;
	u8 bssId[ETH_ALEN];
};

struct noa_attr_ind_msg {
	struct hal_msg_header header;

	u8 index;
	u8 oppPsFlag;
	u16 ctWin;

	u16 uNoa1IntervalCnt;
	u16 bssIdx;
	u32 uNoa1Duration;
	u32 uNoa1Interval;
	u32 uNoa1StartTime;

	u16 uNoa2IntervalCnt;
	u16 rsvd2;
	u32 uNoa2Duration;
	u32 uNoa2Interval;
	u32 uNoa2StartTime;

	u32 status;
};

struct noa_start_ind_msg {
	struct hal_msg_header header;

	u32 status;
	u32 bssIdx;

};

struct hal_wlan_host_resume_req_msg {
	struct hal_msg_header header;

	u8 configuredMcstBcstFilterSetting;

};

struct hal_host_resume_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct hal_del_ba_ind_msg {
	struct hal_msg_header header;

	u16 staIdx;

	/* Peer MAC Address, whose BA session has timed out */
	u8 peerMacAddr[ETH_ALEN];

	/* TID for which a BA session timeout is being triggered */
	u8 baTID;

	/* DELBA direction
	 * 1 - Originator
	 * 0 - Recipient
	 */
	u8 baDirection;

	u32 reasonCode;

	/* TO SUPPORT BT-AMP */
	u8 bssId[ETH_ALEN];

};

/* PNO Messages */

/* Max number of channels that a network can be found on */
#define WCN36XX_HAL_PNO_MAX_NETW_CHANNELS  26

/* Max number of channels that a network can be found on */
#define WCN36XX_HAL_PNO_MAX_NETW_CHANNELS_EX  60

/* Maximum numbers of networks supported by PNO */
#define WCN36XX_HAL_PNO_MAX_SUPP_NETWORKS  16

/* The number of scan time intervals that can be programmed into PNO */
#define WCN36XX_HAL_PNO_MAX_SCAN_TIMERS    10

/* Maximum size of the probe template */
#define WCN36XX_HAL_PNO_MAX_PROBE_SIZE     450

/* Type of PNO enabling:
 *
 * Immediate - scanning will start immediately and PNO procedure will be
 * repeated based on timer
 *
 * Suspend - scanning will start at suspend
 *
 * Resume - scanning will start on system resume
 */
enum pno_mode {
	ePNO_MODE_IMMEDIATE,
	ePNO_MODE_ON_SUSPEND,
	ePNO_MODE_ON_RESUME,
	ePNO_MODE_MAX = WCN36XX_HAL_MAX_ENUM_SIZE
};

/* Authentication type */
enum auth_type {
	eAUTH_TYPE_ANY = 0,
	eAUTH_TYPE_OPEN_SYSTEM = 1,

	/* Upper layer authentication types */
	eAUTH_TYPE_WPA = 2,
	eAUTH_TYPE_WPA_PSK = 3,

	eAUTH_TYPE_RSN = 4,
	eAUTH_TYPE_RSN_PSK = 5,
	eAUTH_TYPE_FT_RSN = 6,
	eAUTH_TYPE_FT_RSN_PSK = 7,
	eAUTH_TYPE_WAPI_WAI_CERTIFICATE = 8,
	eAUTH_TYPE_WAPI_WAI_PSK = 9,

	eAUTH_TYPE_MAX = WCN36XX_HAL_MAX_ENUM_SIZE
};

/* Encryption type */
enum ed_type {
	eED_ANY = 0,
	eED_NONE = 1,
	eED_WEP = 2,
	eED_TKIP = 3,
	eED_CCMP = 4,
	eED_WPI = 5,

	eED_TYPE_MAX = WCN36XX_HAL_MAX_ENUM_SIZE
};

/* SSID broadcast  type */
enum ssid_bcast_type {
	eBCAST_UNKNOWN = 0,
	eBCAST_NORMAL = 1,
	eBCAST_HIDDEN = 2,

	eBCAST_TYPE_MAX = WCN36XX_HAL_MAX_ENUM_SIZE
};

/* The network description for which PNO will have to look for */
struct network_type {
	/* SSID of the BSS */
	struct sir_mac_ssid ssId;

	/* Authentication type for the network */
	enum auth_type authentication;

	/* Encryption type for the network */
	enum ed_type encryption;

	/* Indicate the channel on which the Network can be found 0 - if
	 * all channels */
	u8 ucChannelCount;
	u8 aChannels[WCN36XX_HAL_PNO_MAX_NETW_CHANNELS];

	/* Indicates the RSSI threshold for the network to be considered */
	u8 rssiThreshold;
};

struct scan_timer {
	/* How much it should wait */
	u32 uTimerValue;

	/* How many times it should repeat that wait value 0 - keep using
	 * this timer until PNO is disabled */
	u32 uTimerRepeat;

	/* e.g: 2 3 4 0 - it will wait 2s between consecutive scans for 3
	 * times - after that it will wait 4s between consecutive scans
	 * until disabled */
};

/* The network parameters to be sent to the PNO algorithm */
struct scan_timers_type {
	/* set to 0 if you wish for PNO to use its default telescopic timer */
	u8 ucScanTimersCount;

	/* A set value represents the amount of time that PNO will wait
	 * between two consecutive scan procedures If the desired is for a
	 * uniform timer that fires always at the exact same interval - one
	 * single value is to be set If there is a desire for a more
	 * complex - telescopic like timer multiple values can be set -
	 * once PNO reaches the end of the array it will continue scanning
	 * at intervals presented by the last value */
	struct scan_timer aTimerValues[WCN36XX_HAL_PNO_MAX_SCAN_TIMERS];

};

/* Preferred network list request */
struct set_pref_netw_list_req {
	struct hal_msg_header header;

	/* Enable PNO */
	u32 enable;

	/* Immediate,  On Suspend,   On Resume */
	enum pno_mode modePNO;

	/* Number of networks sent for PNO */
	u32 ucNetworksCount;

	/* The networks that PNO needs to look for */
	struct network_type aNetworks[WCN36XX_HAL_PNO_MAX_SUPP_NETWORKS];

	/* The scan timers required for PNO */
	struct scan_timers_type scanTimers;

	/* Probe template for 2.4GHz band */
	u16 us24GProbeSize;
	u8 a24GProbeTemplate[WCN36XX_HAL_PNO_MAX_PROBE_SIZE];

	/* Probe template for 5GHz band */
	u16 us5GProbeSize;
	u8 a5GProbeTemplate[WCN36XX_HAL_PNO_MAX_PROBE_SIZE];
};

/* The network description for which PNO will have to look for */
struct network_type_new {
	/* SSID of the BSS */
	struct sir_mac_ssid ssId;

	/* Authentication type for the network */
	enum auth_type authentication;

	/* Encryption type for the network */
	enum ed_type encryption;

	/* SSID broadcast type, normal, hidden or unknown */
	enum ssid_bcast_type bcastNetworkType;

	/* Indicate the channel on which the Network can be found 0 - if
	 * all channels */
	u8 ucChannelCount;
	u8 aChannels[WCN36XX_HAL_PNO_MAX_NETW_CHANNELS];

	/* Indicates the RSSI threshold for the network to be considered */
	u8 rssiThreshold;
};

/* Preferred network list request new */
struct set_pref_netw_list_req_new {
	struct hal_msg_header header;

	/* Enable PNO */
	u32 enable;

	/* Immediate,  On Suspend,   On Resume */
	enum pno_mode modePNO;

	/* Number of networks sent for PNO */
	u32 ucNetworksCount;

	/* The networks that PNO needs to look for */
	struct network_type_new aNetworks[WCN36XX_HAL_PNO_MAX_SUPP_NETWORKS];

	/* The scan timers required for PNO */
	struct scan_timers_type scanTimers;

	/* Probe template for 2.4GHz band */
	u16 us24GProbeSize;
	u8 a24GProbeTemplate[WCN36XX_HAL_PNO_MAX_PROBE_SIZE];

	/* Probe template for 5GHz band */
	u16 us5GProbeSize;
	u8 a5GProbeTemplate[WCN36XX_HAL_PNO_MAX_PROBE_SIZE];

};

/* Preferred network list response */
struct set_pref_netw_list_resp {
	struct hal_msg_header header;

	/* status of the request - just to indicate that PNO has
	 * acknowledged the request and will start scanning */
	u32 status;
};

/* Preferred network found indication */
struct pref_netw_found_ind {

	struct hal_msg_header header;

	/* Network that was found with the highest RSSI */
	struct sir_mac_ssid ssId;

	/* Indicates the RSSI */
	u8 rssi;
};

/* RSSI Filter request */
struct set_rssi_filter_req {
	struct hal_msg_header header;

	/* RSSI Threshold */
	u8 ucRssiThreshold;

};

/* Set RSSI filter resp */
struct set_rssi_filter_resp {
	struct hal_msg_header header;

	/* status of the request */
	u32 status;
};

/* Update scan params - sent from host to PNO to be used during PNO
 * scanningx */
struct update_scan_params_req {

	struct hal_msg_header header;

	/* Host setting for 11d */
	u8 b11dEnabled;

	/* Lets PNO know that host has determined the regulatory domain */
	u8 b11dResolved;

	/* Channels on which PNO is allowed to scan */
	u8 ucChannelCount;
	u8 aChannels[WCN36XX_HAL_PNO_MAX_NETW_CHANNELS];

	/* Minimum channel time */
	u16 usActiveMinChTime;

	/* Maximum channel time */
	u16 usActiveMaxChTime;

	/* Minimum channel time */
	u16 usPassiveMinChTime;

	/* Maximum channel time */
	u16 usPassiveMaxChTime;

	/* Cb State */
	enum phy_chan_bond_state cbState;
};

/* Update scan params - sent from host to PNO to be used during PNO
 * scanningx */
struct update_scan_params_req_ex {

	struct hal_msg_header header;

	/* Host setting for 11d */
	u8 b11dEnabled;

	/* Lets PNO know that host has determined the regulatory domain */
	u8 b11dResolved;

	/* Channels on which PNO is allowed to scan */
	u8 ucChannelCount;
	u8 aChannels[WCN36XX_HAL_PNO_MAX_NETW_CHANNELS_EX];

	/* Minimum channel time */
	u16 usActiveMinChTime;

	/* Maximum channel time */
	u16 usActiveMaxChTime;

	/* Minimum channel time */
	u16 usPassiveMinChTime;

	/* Maximum channel time */
	u16 usPassiveMaxChTime;

	/* Cb State */
	enum phy_chan_bond_state cbState;

};

/* Update scan params - sent from host to PNO to be used during PNO
 * scanningx */
struct update_scan_params_resp {

	struct hal_msg_header header;

	/* status of the request */
	u32 status;

};

struct hal_set_tx_per_tracking_req_msg {
	struct hal_msg_header header;

	/* 0: disable, 1:enable */
	u8 ucTxPerTrackingEnable;

	/* Check period, unit is sec. */
	u8 ucTxPerTrackingPeriod;

	/* (Fail TX packet)/(Total TX packet) ratio, the unit is 10%. */
	u8 ucTxPerTrackingRatio;

	/* A watermark of check number, once the tx packet exceed this
	 * number, we do the check, default is 5 */
	u32 uTxPerTrackingWatermark;

};

struct hal_set_tx_per_tracking_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

};

struct tx_per_hit_ind_msg {
	struct hal_msg_header header;
};

/* Packet Filtering Definitions Begin */
#define    HAL_PROTOCOL_DATA_LEN                  8
#define    HAL_MAX_NUM_MULTICAST_ADDRESS        240
#define    HAL_MAX_NUM_FILTERS                   20
#define    HAL_MAX_CMP_PER_FILTER                10

enum hal_receive_packet_filter_type {
	HAL_RCV_FILTER_TYPE_INVALID,
	HAL_RCV_FILTER_TYPE_FILTER_PKT,
	HAL_RCV_FILTER_TYPE_BUFFER_PKT,
	HAL_RCV_FILTER_TYPE_MAX_ENUM_SIZE
};

enum hal_rcv_pkt_flt_protocol_type {
	HAL_FILTER_PROTO_TYPE_INVALID,
	HAL_FILTER_PROTO_TYPE_MAC,
	HAL_FILTER_PROTO_TYPE_ARP,
	HAL_FILTER_PROTO_TYPE_IPV4,
	HAL_FILTER_PROTO_TYPE_IPV6,
	HAL_FILTER_PROTO_TYPE_UDP,
	HAL_FILTER_PROTO_TYPE_MAX
};

enum hal_rcv_pkt_flt_cmp_flag_type {
	HAL_FILTER_CMP_TYPE_INVALID,
	HAL_FILTER_CMP_TYPE_EQUAL,
	HAL_FILTER_CMP_TYPE_MASK_EQUAL,
	HAL_FILTER_CMP_TYPE_NOT_EQUAL,
	HAL_FILTER_CMP_TYPE_MAX
};

struct hal_rcv_pkt_filter_params {
	u8 protocolLayer;
	u8 cmpFlag;

	/* Length of the data to compare */
	u16 dataLength;

	/* from start of the respective frame header */
	u8 dataOffset;

	/* Reserved field */
	u8 reserved;

	/* Data to compare */
	u8 compareData[HAL_PROTOCOL_DATA_LEN];

	/* Mask to be applied on the received packet data before compare */
	u8 dataMask[HAL_PROTOCOL_DATA_LEN];
};

struct hal_sessionized_rcv_pkt_filter_cfg_type {
	u8 filterId;
	u8 filterType;
	u8 numParams;
	u32 coleasceTime;
	u8 bssIdx;
	struct hal_rcv_pkt_filter_params paramsData[1];
};

struct hal_set_rcv_pkt_filter_req_msg {
	struct hal_msg_header header;

	u8 filterId;
	u8 filterType;
	u8 numParams;
	u32 coalesceTime;
	struct hal_rcv_pkt_filter_params paramsData[1];

};

struct hal_rcv_flt_mc_addr_list_type {
	/* from start of the respective frame header */
	u8 dataOffset;

	u32 cMulticastAddr;
	u8 multicastAddr[ETH_ALEN][HAL_MAX_NUM_MULTICAST_ADDRESS];
	u8 bssIdx;
};

struct hal_set_pkt_filter_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;

	u8 bssIdx;
};

struct hal_rcv_flt_pkt_match_cnt_req_msg {
	struct hal_msg_header header;

	u8 bssIdx;

};

struct hal_rcv_flt_pkt_match_cnt {
	u8 filterId;
	u32 matchCnt;
};

struct hal_rcv_flt_pkt_match_cnt_rsp_msg {
	struct hal_msg_header header;

	/* Success or Failure */
	u32 status;

	u32 matchCnt;
	struct hal_rcv_flt_pkt_match_cnt filterMatchCnt[HAL_MAX_NUM_FILTERS];
	u8 bssIdx;
};

struct hal_rcv_flt_pkt_clear_param {
	/* only valid for response message */
	u32 status;
	u8 filterId;
	u8 bssIdx;
};

struct hal_rcv_flt_pkt_clear_req_msg {
	struct hal_msg_header header;
	struct hal_rcv_flt_pkt_clear_param filterClearParam;
};

struct hal_rcv_flt_pkt_clear_rsp_msg {
	struct hal_msg_header header;
	struct hal_rcv_flt_pkt_clear_param filterClearParam;
};

struct hal_rcv_flt_pkt_set_mc_list_req_msg {
	struct hal_msg_header header;
	struct hal_rcv_flt_mc_addr_list_type mcAddrList;
};

struct hal_rcv_flt_pkt_set_mc_list_rsp_msg {
	struct hal_msg_header header;
	u32 status;
	u8 bssIdx;
};

/* Packet Filtering Definitions End */

struct set_power_params_req_msg {
	struct hal_msg_header header;

	/*  Ignore DTIM */
	u32 uIgnoreDTIM;

	/* DTIM Period */
	u32 uDTIMPeriod;

	/* Listen Interval */
	u32 uListenInterval;

	/* Broadcast Multicast Filter  */
	u32 uBcastMcastFilter;

	/* Beacon Early Termination */
	u32 uEnableBET;

	/* Beacon Early Termination Interval */
	u32 uBETInterval;
};

struct set_power_params_resp {

	struct hal_msg_header header;

	/* status of the request */
	u32 status;

};

/* Capability bitmap exchange definitions and macros starts */

enum place_holder_in_cap_bitmap {
	MCC = 0,
	P2P = 1,
	DOT11AC = 2,
	SLM_SESSIONIZATION = 3,
	DOT11AC_OPMODE = 4,
	SAP32STA = 5,
	TDLS = 6,
	P2P_GO_NOA_DECOUPLE_INIT_SCAN = 7,
	WLANACTIVE_OFFLOAD = 8,
	BEACON_OFFLOAD = 9,
	SCAN_OFFLOAD = 10,
	ROAM_OFFLOAD = 11,
	BCN_MISS_OFFLOAD = 12,
	STA_POWERSAVE = 13,
	STA_ADVANCED_PWRSAVE = 14,
	AP_UAPSD = 15,
	AP_DFS = 16,
	BLOCKACK = 17,
	PHY_ERR = 18,
	BCN_FILTER = 19,
	RTT = 20,
	RATECTRL = 21,
	WOW = 22,
	MAX_FEATURE_SUPPORTED = 128,
};

struct wlan_feat_caps_msg {

	struct hal_msg_header header;

	u32 featCaps[4];

};

#define IS_MCC_SUPPORTED_BY_HOST (!!(halMsg_GetHostWlanFeatCaps(MCC)))
#define IS_SLM_SESSIONIZATION_SUPPORTED_BY_HOST (!!(halMsg_GetHostWlanFeatCaps(SLM_SESSIONIZATION)))
#define IS_FEATURE_SUPPORTED_BY_HOST(featEnumValue) (!!halMsg_GetHostWlanFeatCaps(featEnumValue))
#define IS_WLANACTIVE_OFFLOAD_SUPPORTED_BY_HOST (!!(halMsg_GetHostWlanFeatCaps(WLANACTIVE_OFFLOAD)))

u8 halMsg_GetHostWlanFeatCaps(u8 feat_enum_value);

#define setFeatCaps(a,b)   {  u32 arr_index, bit_index; \
                              if ((b)<=127) { \
                                arr_index = (b)/32; \
                                bit_index = (b)%32; \
                                (a)->featCaps[arr_index] |= (1<<bit_index); \
                              } \
                           }
#define getFeatCaps(a,b,c) {  u32 arr_index, bit_index; \
                              if ((b)<=127) { \
                                arr_index = (b)/32; \
                                bit_index = (b)%32; \
                                (c) = ((a)->featCaps[arr_index] & (1<<bit_index))?1:0; \
                              } \
                           }
#define clearFeatCaps(a,b) {  u32 arr_index, bit_index; \
                              if ((b)<=127) { \
                                arr_index = (b)/32; \
                                bit_index = (b)%32; \
                                (a)->featCaps[arr_index] &= ~(1<<bit_index); \
                              } \
                           }

/* status codes to help debug rekey failures */
enum gtk_rekey_status {
	WCN36XX_HAL_GTK_REKEY_STATUS_SUCCESS = 0,

	/* rekey detected, but not handled */
	WCN36XX_HAL_GTK_REKEY_STATUS_NOT_HANDLED = 1,

	/* MIC check error on M1 */
	WCN36XX_HAL_GTK_REKEY_STATUS_MIC_ERROR = 2,

	/* decryption error on M1  */
	WCN36XX_HAL_GTK_REKEY_STATUS_DECRYPT_ERROR = 3,

	/* M1 replay detected */
	WCN36XX_HAL_GTK_REKEY_STATUS_REPLAY_ERROR = 4,

	/* missing GTK key descriptor in M1 */
	WCN36XX_HAL_GTK_REKEY_STATUS_MISSING_KDE = 5,

	/* missing iGTK key descriptor in M1 */
	WCN36XX_HAL_GTK_REKEY_STATUS_MISSING_IGTK_KDE = 6,

	/* key installation error */
	WCN36XX_HAL_GTK_REKEY_STATUS_INSTALL_ERROR = 7,

	/* iGTK key installation error */
	WCN36XX_HAL_GTK_REKEY_STATUS_IGTK_INSTALL_ERROR = 8,

	/* GTK rekey M2 response TX error */
	WCN36XX_HAL_GTK_REKEY_STATUS_RESP_TX_ERROR = 9,

	/* non-specific general error */
	WCN36XX_HAL_GTK_REKEY_STATUS_GEN_ERROR = 255
};

/* wake reason types */
enum wake_reason_type {
	WCN36XX_HAL_WAKE_REASON_NONE = 0,

	/* magic packet match */
	WCN36XX_HAL_WAKE_REASON_MAGIC_PACKET = 1,

	/* host defined pattern match */
	WCN36XX_HAL_WAKE_REASON_PATTERN_MATCH = 2,

	/* EAP-ID frame detected */
	WCN36XX_HAL_WAKE_REASON_EAPID_PACKET = 3,

	/* start of EAPOL 4-way handshake detected */
	WCN36XX_HAL_WAKE_REASON_EAPOL4WAY_PACKET = 4,

	/* network scan offload match */
	WCN36XX_HAL_WAKE_REASON_NETSCAN_OFFL_MATCH = 5,

	/* GTK rekey status wakeup (see status) */
	WCN36XX_HAL_WAKE_REASON_GTK_REKEY_STATUS = 6,

	/* BSS connection lost */
	WCN36XX_HAL_WAKE_REASON_BSS_CONN_LOST = 7,
};

/*
  Wake Packet which is saved at tWakeReasonParams.DataStart
  This data is sent for any wake reasons that involve a packet-based wakeup :

  WCN36XX_HAL_WAKE_REASON_TYPE_MAGIC_PACKET
  WCN36XX_HAL_WAKE_REASON_TYPE_PATTERN_MATCH
  WCN36XX_HAL_WAKE_REASON_TYPE_EAPID_PACKET
  WCN36XX_HAL_WAKE_REASON_TYPE_EAPOL4WAY_PACKET
  WCN36XX_HAL_WAKE_REASON_TYPE_GTK_REKEY_STATUS

  The information is provided to the host for auditing and debug purposes

*/

/* Wake reason indication */
struct hal_wake_reason_ind {
	struct hal_msg_header header;

	/* see tWakeReasonType */
	u32 ulReason;

	/* argument specific to the reason type */
	u32 ulReasonArg;

	/* length of optional data stored in this message, in case HAL
	 * truncates the data (i.e. data packets) this length will be less
	 * than the actual length */
	u32 ulStoredDataLen;

	/* actual length of data */
	u32 ulActualDataLen;

	/* variable length start of data (length == storedDataLen) see
	 * specific wake type */
	u8 aDataStart[1];

	u32 uBssIdx:8;
	u32 bReserved:24;
};

#define HAL_GTK_KEK_BYTES 16
#define HAL_GTK_KCK_BYTES 16

#define WCN36XX_HAL_GTK_OFFLOAD_FLAGS_DISABLE (1 << 0)

#define GTK_SET_BSS_KEY_TAG  0x1234AA55

struct hal_gtk_offload_req_msg {
	struct hal_msg_header header;

	/* optional flags */
	u32 ulFlags;

	/* Key confirmation key */
	u8 aKCK[HAL_GTK_KCK_BYTES];

	/* key encryption key */
	u8 aKEK[HAL_GTK_KEK_BYTES];

	/* replay counter */
	u64 ullKeyReplayCounter;

	u8 bssIdx;
};

struct hal_gtk_offload_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 ulStatus;

	u8 bssIdx;
};

struct hal_gtk_offload_get_info_req_msg {
	struct hal_msg_header header;
	u8 bssIdx;
};

struct hal_gtk_offload_get_info_rsp_msg {
	struct hal_msg_header header;

	/* success or failure */
	u32 ulStatus;

	/* last rekey status when the rekey was offloaded */
	u32 ulLastRekeyStatus;

	/* current replay counter value */
	u64 ullKeyReplayCounter;

	/* total rekey attempts */
	u32 ulTotalRekeyCount;

	/* successful GTK rekeys */
	u32 ulGTKRekeyCount;

	/* successful iGTK rekeys */
	u32 ulIGTKRekeyCount;

	u8 bssIdx;

};

struct dhcp_info {
	/* Indicates the device mode which indicates about the DHCP activity */
	u8 device_mode;

	u8 macAddr[ETH_ALEN];
};

struct dhcp_ind_status {
	struct hal_msg_header header;

	/* success or failure */
	u32 status;
};

/*
 *   Thermal Mitigation mode of operation.
 *
 *  HAL_THERMAL_MITIGATION_MODE_0 - Based on AMPDU disabling aggregation
 *
 *  HAL_THERMAL_MITIGATION_MODE_1 - Based on AMPDU disabling aggregation and
 *  reducing transmit power
 *
 *  HAL_THERMAL_MITIGATION_MODE_2 - Not supported */
enum hal_thermal_mitigation_mode_type {
	HAL_THERMAL_MITIGATION_MODE_INVALID = -1,
	HAL_THERMAL_MITIGATION_MODE_0,
	HAL_THERMAL_MITIGATION_MODE_1,
	HAL_THERMAL_MITIGATION_MODE_2,
	HAL_THERMAL_MITIGATION_MODE_MAX = WCN36XX_HAL_MAX_ENUM_SIZE,
};


/*
 *   Thermal Mitigation level.
 *   Note the levels are incremental i.e HAL_THERMAL_MITIGATION_LEVEL_2 =
 * HAL_THERMAL_MITIGATION_LEVEL_0 + HAL_THERMAL_MITIGATION_LEVEL_1
 *
 * HAL_THERMAL_MITIGATION_LEVEL_0 - lowest level of thermal mitigation. This
 * level indicates normal mode of operation
 *
 * HAL_THERMAL_MITIGATION_LEVEL_1 - 1st level of thermal mitigation
 *
 * HAL_THERMAL_MITIGATION_LEVEL_2 - 2nd level of thermal mitigation
 *
 * HAL_THERMAL_MITIGATION_LEVEL_3 - 3rd level of thermal mitigation
 *
 * HAL_THERMAL_MITIGATION_LEVEL_4 - 4th level of thermal mitigation
 */
enum hal_thermal_mitigation_level_type {
	HAL_THERMAL_MITIGATION_LEVEL_INVALID = -1,
	HAL_THERMAL_MITIGATION_LEVEL_0,
	HAL_THERMAL_MITIGATION_LEVEL_1,
	HAL_THERMAL_MITIGATION_LEVEL_2,
	HAL_THERMAL_MITIGATION_LEVEL_3,
	HAL_THERMAL_MITIGATION_LEVEL_4,
	HAL_THERMAL_MITIGATION_LEVEL_MAX = WCN36XX_HAL_MAX_ENUM_SIZE,
};


/* WCN36XX_HAL_SET_THERMAL_MITIGATION_REQ */
struct set_thermal_mitigation_req_msg {
	struct hal_msg_header header;

	/* Thermal Mitigation Operation Mode */
	enum hal_thermal_mitigation_mode_type thermalMitMode;

	/* Thermal Mitigation Level */
	enum hal_thermal_mitigation_level_type thermalMitLevel;

};

struct set_thermal_mitigation_resp {

	struct hal_msg_header header;

	/* status of the request */
	u32 status;

};

/* Per STA Class B Statistics. Class B statistics are STA TX/RX stats
 * provided to FW from Host via periodic messages */
struct stats_class_b_ind {
	struct hal_msg_header header;

	/* Duration over which this stats was collected */
	u32 duration;

	/* Per STA Stats */

	/* TX stats */
	u32 txBytesPushed;
	u32 txPacketsPushed;

	/* RX stats */
	u32 rxBytesRcvd;
	u32 rxPacketsRcvd;
	u32 rxTimeTotal;

};

#endif /* _HAL_H_ */
