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

/* FIXME: remove these */
#define PACKED_PRE
#define PACKED_POST
#define __ani_attr_pre_packed
#define __ani_attr_packed

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
#define WLAN_HAL_VER_MAJOR 1
#define WLAN_HAL_VER_MINOR 4
#define WLAN_HAL_VER_VERSION 1
#define WLAN_HAL_VER_REVISION 2

/*---------------------------------------------------------------------------
  Commom Type definitons
 ---------------------------------------------------------------------------*/

//This is to force compiler to use the maximum of an int ( 4 bytes )
#define WLAN_HAL_MAX_ENUM_SIZE    0x7FFFFFFF
#define WLAN_HAL_MSG_TYPE_MAX_ENUM_SIZE    0x7FFF

//Max no. of transmit categories
#define STACFG_MAX_TC    8

//The maximum value of access category
#define WLAN_HAL_MAX_AC  4

typedef u8 tSirMacAddr[6];
typedef u8 tHalIpv4Addr[4];

#define HAL_MAC_ADDR_LEN        6
#define HAL_IPV4_ADDR_LEN       4

#define WALN_HAL_STA_INVALID_IDX 0xFF
#define WLAN_HAL_BSS_INVALID_IDX 0xFF

//Default Beacon template size
#define BEACON_TEMPLATE_SIZE 0x180 

//Param Change Bitmap sent to HAL 
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
#define PARAM_BEACON_UPDATE_MASK                (PARAM_BCN_INTERVAL_CHANGED|PARAM_SHORT_PREAMBLE_CHANGED|PARAM_SHORT_SLOT_TIME_CHANGED|PARAM_llACOEXIST_CHANGED |PARAM_llBCOEXIST_CHANGED|\
    PARAM_llGCOEXIST_CHANGED|PARAM_HT20MHZCOEXIST_CHANGED|PARAM_NON_GF_DEVICES_PRESENT_CHANGED|PARAM_RIFS_MODE_CHANGED|PARAM_LSIG_TXOP_FULL_SUPPORT_CHANGED| PARAM_OBSS_MODE_CHANGED)

/*Dump command response Buffer size*/
#define DUMPCMD_RSP_BUFFER 100

/*Version string max length (including NUL) */
#define WLAN_HAL_VERSION_LENGTH  64

/* Message types for messages exchanged between WDI and HAL */
typedef enum 
{
   //Init/De-Init
   WLAN_HAL_START_REQ = 0,
   WLAN_HAL_START_RSP = 1,
   WLAN_HAL_STOP_REQ  = 2,
   WLAN_HAL_STOP_RSP  = 3,

   //Scan
   WLAN_HAL_INIT_SCAN_REQ    = 4,
   WLAN_HAL_INIT_SCAN_RSP    = 5,
   WLAN_HAL_START_SCAN_REQ   = 6,
   WLAN_HAL_START_SCAN_RSP   = 7 ,
   WLAN_HAL_END_SCAN_REQ     = 8,
   WLAN_HAL_END_SCAN_RSP     = 9,
   WLAN_HAL_FINISH_SCAN_REQ  = 10,
   WLAN_HAL_FINISH_SCAN_RSP  = 11,

   // HW STA configuration/deconfiguration
   WLAN_HAL_CONFIG_STA_REQ   = 12,
   WLAN_HAL_CONFIG_STA_RSP   = 13,
   WLAN_HAL_DELETE_STA_REQ   = 14,
   WLAN_HAL_DELETE_STA_RSP   = 15,
   WLAN_HAL_CONFIG_BSS_REQ   = 16,
   WLAN_HAL_CONFIG_BSS_RSP   = 17,
   WLAN_HAL_DELETE_BSS_REQ   = 18,
   WLAN_HAL_DELETE_BSS_RSP   = 19,

   //Infra STA asscoiation
   WLAN_HAL_JOIN_REQ         = 20,
   WLAN_HAL_JOIN_RSP         = 21,
   WLAN_HAL_POST_ASSOC_REQ   = 22,
   WLAN_HAL_POST_ASSOC_RSP   = 23,

   //Security
   WLAN_HAL_SET_BSSKEY_REQ   = 24,
   WLAN_HAL_SET_BSSKEY_RSP   = 25,
   WLAN_HAL_SET_STAKEY_REQ   = 26,
   WLAN_HAL_SET_STAKEY_RSP   = 27,
   WLAN_HAL_RMV_BSSKEY_REQ   = 28,
   WLAN_HAL_RMV_BSSKEY_RSP   = 29,
   WLAN_HAL_RMV_STAKEY_REQ   = 30,
   WLAN_HAL_RMV_STAKEY_RSP   = 31,

   //Qos Related
   WLAN_HAL_ADD_TS_REQ          = 32,
   WLAN_HAL_ADD_TS_RSP          = 33,
   WLAN_HAL_DEL_TS_REQ          = 34,
   WLAN_HAL_DEL_TS_RSP          = 35,
   WLAN_HAL_UPD_EDCA_PARAMS_REQ = 36,
   WLAN_HAL_UPD_EDCA_PARAMS_RSP = 37,
   WLAN_HAL_ADD_BA_REQ          = 38,
   WLAN_HAL_ADD_BA_RSP          = 39,
   WLAN_HAL_DEL_BA_REQ          = 40,
   WLAN_HAL_DEL_BA_RSP          = 41,

   WLAN_HAL_CH_SWITCH_REQ       = 42,
   WLAN_HAL_CH_SWITCH_RSP       = 43,
   WLAN_HAL_SET_LINK_ST_REQ     = 44,
   WLAN_HAL_SET_LINK_ST_RSP     = 45,
   WLAN_HAL_GET_STATS_REQ       = 46,
   WLAN_HAL_GET_STATS_RSP       = 47,
   WLAN_HAL_UPDATE_CFG_REQ      = 48,
   WLAN_HAL_UPDATE_CFG_RSP      = 49,

   WLAN_HAL_MISSED_BEACON_IND           = 50,
   WLAN_HAL_UNKNOWN_ADDR2_FRAME_RX_IND  = 51,
   WLAN_HAL_MIC_FAILURE_IND             = 52,
   WLAN_HAL_FATAL_ERROR_IND             = 53,
   WLAN_HAL_SET_KEYDONE_MSG             = 54,
   
   //NV Interface
   WLAN_HAL_DOWNLOAD_NV_REQ             = 55,
   WLAN_HAL_DOWNLOAD_NV_RSP             = 56,

   WLAN_HAL_ADD_BA_SESSION_REQ          = 57,
   WLAN_HAL_ADD_BA_SESSION_RSP          = 58,
   WLAN_HAL_TRIGGER_BA_REQ              = 59,
   WLAN_HAL_TRIGGER_BA_RSP              = 60,
   WLAN_HAL_UPDATE_BEACON_REQ           = 61,
   WLAN_HAL_UPDATE_BEACON_RSP           = 62,
   WLAN_HAL_SEND_BEACON_REQ             = 63,
   WLAN_HAL_SEND_BEACON_RSP             = 64,

   WLAN_HAL_SET_BCASTKEY_REQ               = 65,
   WLAN_HAL_SET_BCASTKEY_RSP               = 66,
   WLAN_HAL_DELETE_STA_CONTEXT_IND         = 67,
   WLAN_HAL_UPDATE_PROBE_RSP_TEMPLATE_REQ  = 68,
   WLAN_HAL_UPDATE_PROBE_RSP_TEMPLATE_RSP  = 69,
   
  // PTT interface support
   WLAN_HAL_PROCESS_PTT_REQ   = 70,
   WLAN_HAL_PROCESS_PTT_RSP   = 71,
   
   // BTAMP related events
   WLAN_HAL_SIGNAL_BTAMP_EVENT_REQ  = 72,
   WLAN_HAL_SIGNAL_BTAMP_EVENT_RSP  = 73,
   WLAN_HAL_TL_HAL_FLUSH_AC_REQ     = 74,
   WLAN_HAL_TL_HAL_FLUSH_AC_RSP     = 75,

   WLAN_HAL_ENTER_IMPS_REQ           = 76,
   WLAN_HAL_EXIT_IMPS_REQ            = 77,
   WLAN_HAL_ENTER_BMPS_REQ           = 78,
   WLAN_HAL_EXIT_BMPS_REQ            = 79,
   WLAN_HAL_ENTER_UAPSD_REQ          = 80,
   WLAN_HAL_EXIT_UAPSD_REQ           = 81,
   WLAN_HAL_UPDATE_UAPSD_PARAM_REQ   = 82,
   WLAN_HAL_CONFIGURE_RXP_FILTER_REQ = 83,
   WLAN_HAL_ADD_BCN_FILTER_REQ       = 84,
   WLAN_HAL_REM_BCN_FILTER_REQ       = 85,
   WLAN_HAL_ADD_WOWL_BCAST_PTRN      = 86,
   WLAN_HAL_DEL_WOWL_BCAST_PTRN      = 87,
   WLAN_HAL_ENTER_WOWL_REQ           = 88,
   WLAN_HAL_EXIT_WOWL_REQ            = 89,
   WLAN_HAL_HOST_OFFLOAD_REQ         = 90,
   WLAN_HAL_SET_RSSI_THRESH_REQ      = 91,
   WLAN_HAL_GET_RSSI_REQ             = 92,
   WLAN_HAL_SET_UAPSD_AC_PARAMS_REQ  = 93,
   WLAN_HAL_CONFIGURE_APPS_CPU_WAKEUP_STATE_REQ = 94,

   WLAN_HAL_ENTER_IMPS_RSP           = 95,
   WLAN_HAL_EXIT_IMPS_RSP            = 96,
   WLAN_HAL_ENTER_BMPS_RSP           = 97,
   WLAN_HAL_EXIT_BMPS_RSP            = 98,
   WLAN_HAL_ENTER_UAPSD_RSP          = 99,
   WLAN_HAL_EXIT_UAPSD_RSP           = 100,
   WLAN_HAL_SET_UAPSD_AC_PARAMS_RSP  = 101,
   WLAN_HAL_UPDATE_UAPSD_PARAM_RSP   = 102,
   WLAN_HAL_CONFIGURE_RXP_FILTER_RSP = 103,
   WLAN_HAL_ADD_BCN_FILTER_RSP       = 104,
   WLAN_HAL_REM_BCN_FILTER_RSP       = 105,
   WLAN_HAL_SET_RSSI_THRESH_RSP      = 106,
   WLAN_HAL_HOST_OFFLOAD_RSP         = 107,
   WLAN_HAL_ADD_WOWL_BCAST_PTRN_RSP  = 108,
   WLAN_HAL_DEL_WOWL_BCAST_PTRN_RSP  = 109,
   WLAN_HAL_ENTER_WOWL_RSP           = 110,
   WLAN_HAL_EXIT_WOWL_RSP            = 111,
   WLAN_HAL_RSSI_NOTIFICATION_IND    = 112,
   WLAN_HAL_GET_RSSI_RSP             = 113,
   WLAN_HAL_CONFIGURE_APPS_CPU_WAKEUP_STATE_RSP = 114,

   //11k related events
   WLAN_HAL_SET_MAX_TX_POWER_REQ   = 115,
   WLAN_HAL_SET_MAX_TX_POWER_RSP   = 116,

   //11R related msgs
   WLAN_HAL_AGGR_ADD_TS_REQ        = 117,
   WLAN_HAL_AGGR_ADD_TS_RSP        = 118,

   //P2P  WLAN_FEATURE_P2P
   WLAN_HAL_SET_P2P_GONOA_REQ      = 119,
   WLAN_HAL_SET_P2P_GONOA_RSP      = 120,
   
   //WLAN Dump commands
   WLAN_HAL_DUMP_COMMAND_REQ       = 121,
   WLAN_HAL_DUMP_COMMAND_RSP       = 122,

   //OEM_DATA FEATURE SUPPORT
   WLAN_HAL_START_OEM_DATA_REQ   = 123,
   WLAN_HAL_START_OEM_DATA_RSP   = 124,

   //ADD SELF STA REQ and RSP
   WLAN_HAL_ADD_STA_SELF_REQ       = 125,
   WLAN_HAL_ADD_STA_SELF_RSP       = 126,

   //DEL SELF STA SUPPORT
   WLAN_HAL_DEL_STA_SELF_REQ       = 127,
   WLAN_HAL_DEL_STA_SELF_RSP       = 128,

   // Coex Indication
   WLAN_HAL_COEX_IND               = 129,

   // Tx Complete Indication 
   WLAN_HAL_OTA_TX_COMPL_IND       = 130,

   //Host Suspend/resume messages
   WLAN_HAL_HOST_SUSPEND_IND       = 131,
   WLAN_HAL_HOST_RESUME_REQ        = 132,
   WLAN_HAL_HOST_RESUME_RSP        = 133,

   WLAN_HAL_SET_TX_POWER_REQ       = 134,
   WLAN_HAL_SET_TX_POWER_RSP       = 135,
   WLAN_HAL_GET_TX_POWER_REQ       = 136,
   WLAN_HAL_GET_TX_POWER_RSP       = 137,

   WLAN_HAL_P2P_NOA_ATTR_IND       = 138,
   
   WLAN_HAL_ENABLE_RADAR_DETECT_REQ  = 139,
   WLAN_HAL_ENABLE_RADAR_DETECT_RSP  = 140,
   WLAN_HAL_GET_TPC_REPORT_REQ       = 141,
   WLAN_HAL_GET_TPC_REPORT_RSP       = 142,
   WLAN_HAL_RADAR_DETECT_IND         = 143,
   WLAN_HAL_RADAR_DETECT_INTR_IND    = 144,
   WLAN_HAL_KEEP_ALIVE_REQ           = 145,
   WLAN_HAL_KEEP_ALIVE_RSP           = 146,      

   /*PNO messages*/
   WLAN_HAL_SET_PREF_NETWORK_REQ     = 147,
   WLAN_HAL_SET_PREF_NETWORK_RSP     = 148,
   WLAN_HAL_SET_RSSI_FILTER_REQ      = 149,
   WLAN_HAL_SET_RSSI_FILTER_RSP      = 150,
   WLAN_HAL_UPDATE_SCAN_PARAM_REQ    = 151,
   WLAN_HAL_UPDATE_SCAN_PARAM_RSP    = 152,
   WLAN_HAL_PREF_NETW_FOUND_IND      = 153, 

   WLAN_HAL_SET_TX_PER_TRACKING_REQ  = 154,
   WLAN_HAL_SET_TX_PER_TRACKING_RSP  = 155,
   WLAN_HAL_TX_PER_HIT_IND           = 156,
   
   WLAN_HAL_8023_MULTICAST_LIST_REQ   = 157,
   WLAN_HAL_8023_MULTICAST_LIST_RSP   = 158,   

   WLAN_HAL_SET_PACKET_FILTER_REQ     = 159,
   WLAN_HAL_SET_PACKET_FILTER_RSP     = 160,   
   WLAN_HAL_PACKET_FILTER_MATCH_COUNT_REQ   = 161,
   WLAN_HAL_PACKET_FILTER_MATCH_COUNT_RSP   = 162,   
   WLAN_HAL_CLEAR_PACKET_FILTER_REQ         = 163,
   WLAN_HAL_CLEAR_PACKET_FILTER_RSP         = 164,  
   /*This is temp fix. Should be removed once 
    * Host and Riva code is in sync*/
   WLAN_HAL_INIT_SCAN_CON_REQ               = 165,
    
   WLAN_HAL_SET_POWER_PARAMS_REQ            = 166,
   WLAN_HAL_SET_POWER_PARAMS_RSP            = 167,

   WLAN_HAL_TSM_STATS_REQ                   = 168,
   WLAN_HAL_TSM_STATS_RSP                   = 169,

   // wake reason indication (WOW)
   WLAN_HAL_WAKE_REASON_IND                 = 170,
   // GTK offload support 
   WLAN_HAL_GTK_OFFLOAD_REQ                 = 171,
   WLAN_HAL_GTK_OFFLOAD_RSP                 = 172,
   WLAN_HAL_GTK_OFFLOAD_GETINFO_REQ         = 173,
   WLAN_HAL_GTK_OFFLOAD_GETINFO_RSP         = 174,

   WLAN_HAL_FEATURE_CAPS_EXCHANGE_REQ       = 175,
   WLAN_HAL_FEATURE_CAPS_EXCHANGE_RSP       = 176,
   WLAN_HAL_EXCLUDE_UNENCRYPTED_IND         = 177,

   WLAN_HAL_SET_THERMAL_MITIGATION_REQ      = 178,
   WLAN_HAL_SET_THERMAL_MITIGATION_RSP      = 179,

   WLAN_HAL_UPDATE_VHT_OP_MODE_REQ          = 182,
   WLAN_HAL_UPDATE_VHT_OP_MODE_RSP          = 183,

   WLAN_HAL_P2P_NOA_START_IND               = 184,

   WLAN_HAL_GET_ROAM_RSSI_REQ               = 185,
   WLAN_HAL_GET_ROAM_RSSI_RSP               = 186,

   WLAN_HAL_CLASS_B_STATS_IND               = 187,
   WLAN_HAL_DEL_BA_IND                      = 188,
   WLAN_HAL_DHCP_START_IND                  = 189,
   WLAN_HAL_DHCP_STOP_IND                   = 190,

  WLAN_HAL_MSG_MAX = WLAN_HAL_MSG_TYPE_MAX_ENUM_SIZE
}tHalHostMsgType;

/* Enumeration for Version */
typedef enum
{
   WLAN_HAL_MSG_VERSION0 = 0,
   WLAN_HAL_MSG_VERSION1 = 1,
   WLAN_HAL_MSG_WCNSS_CTRL_VERSION = 0x7FFF, /*define as 2 bytes data*/
   WLAN_HAL_MSG_VERSION_MAX_FIELD  = WLAN_HAL_MSG_WCNSS_CTRL_VERSION
}tHalHostMsgVersion;

/* Enumeration for Boolean - False/True, On/Off */
typedef enum tagAniBoolean 
{
    eANI_BOOLEAN_FALSE = 0,
    eANI_BOOLEAN_TRUE,
    eANI_BOOLEAN_OFF = 0,
    eANI_BOOLEAN_ON = 1,
    eANI_BOOLEAN_MAX_FIELD = 0x7FFFFFFF  /* define as 4 bytes data */
} eAniBoolean;

typedef enum
{
   eDRIVER_TYPE_PRODUCTION  = 0,
   eDRIVER_TYPE_MFG         = 1,
   eDRIVER_TYPE_DVT         = 2,
   eDRIVER_TYPE_MAX         = WLAN_HAL_MAX_ENUM_SIZE
} tDriverType;

typedef enum
{
   HAL_STOP_TYPE_SYS_RESET,
   HAL_STOP_TYPE_SYS_DEEP_SLEEP,
   HAL_STOP_TYPE_RF_KILL,
   HAL_STOP_TYPE_MAX = WLAN_HAL_MAX_ENUM_SIZE
}tHalStopType;

typedef enum
{
   eHAL_SYS_MODE_NORMAL,
   eHAL_SYS_MODE_LEARN,
   eHAL_SYS_MODE_SCAN,
   eHAL_SYS_MODE_PROMISC,
   eHAL_SYS_MODE_SUSPEND_LINK,
   eHAL_SYS_MODE_ROAM_SCAN,
   eHAL_SYS_MODE_ROAM_SUSPEND_LINK,
   eHAL_SYS_MODE_MAX = WLAN_HAL_MAX_ENUM_SIZE
} eHalSysMode;

typedef enum
{
    PHY_SINGLE_CHANNEL_CENTERED = 0,     // 20MHz IF bandwidth centered on IF carrier
    PHY_DOUBLE_CHANNEL_LOW_PRIMARY = 1,  // 40MHz IF bandwidth with lower 20MHz supporting the primary channel
    PHY_DOUBLE_CHANNEL_CENTERED = 2,     // 40MHz IF bandwidth centered on IF carrier
    PHY_DOUBLE_CHANNEL_HIGH_PRIMARY = 3, // 40MHz IF bandwidth with higher 20MHz supporting the primary channel
#ifdef WLAN_FEATURE_11AC
    PHY_QUADRUPLE_CHANNEL_20MHZ_LOW_40MHZ_CENTERED = 4, //20/40MHZ offset LOW 40/80MHZ offset CENTERED
    PHY_QUADRUPLE_CHANNEL_20MHZ_CENTERED_40MHZ_CENTERED = 5, //20/40MHZ offset CENTERED 40/80MHZ offset CENTERED
    PHY_QUADRUPLE_CHANNEL_20MHZ_HIGH_40MHZ_CENTERED = 6, //20/40MHZ offset HIGH 40/80MHZ offset CENTERED
    PHY_QUADRUPLE_CHANNEL_20MHZ_LOW_40MHZ_LOW = 7,//20/40MHZ offset LOW 40/80MHZ offset LOW
    PHY_QUADRUPLE_CHANNEL_20MHZ_HIGH_40MHZ_LOW = 8, //20/40MHZ offset HIGH 40/80MHZ offset LOW
    PHY_QUADRUPLE_CHANNEL_20MHZ_LOW_40MHZ_HIGH = 9, //20/40MHZ offset LOW 40/80MHZ offset HIGH
    PHY_QUADRUPLE_CHANNEL_20MHZ_HIGH_40MHZ_HIGH = 10,//20/40MHZ offset-HIGH 40/80MHZ offset HIGH
#endif
    PHY_CHANNEL_BONDING_STATE_MAX = WLAN_HAL_MAX_ENUM_SIZE
}ePhyChanBondState;

// Spatial Multiplexing(SM) Power Save mode
typedef enum eSirMacHTMIMOPowerSaveState
{
  eSIR_HT_MIMO_PS_STATIC = 0,    // Static SM Power Save mode
  eSIR_HT_MIMO_PS_DYNAMIC = 1,   // Dynamic SM Power Save mode
  eSIR_HT_MIMO_PS_NA = 2,        // reserved
  eSIR_HT_MIMO_PS_NO_LIMIT = 3,  // SM Power Save disabled
  eSIR_HT_MIMO_PS_MAX = WLAN_HAL_MAX_ENUM_SIZE
} tSirMacHTMIMOPowerSaveState;

/* each station added has a rate mode which specifies the sta attributes */
typedef enum eStaRateMode {
    eSTA_TAURUS = 0,
    eSTA_TITAN,
    eSTA_POLARIS,
    eSTA_11b,
    eSTA_11bg,
    eSTA_11a,
    eSTA_11n,
#ifdef WLAN_FEATURE_11AC
    eSTA_11ac,
#endif
    eSTA_INVALID_RATE_MODE = WLAN_HAL_MAX_ENUM_SIZE
} tStaRateMode, *tpStaRateMode;

#define SIR_NUM_11B_RATES           4  //1,2,5.5,11
#define SIR_NUM_11A_RATES           8  //6,9,12,18,24,36,48,54
#define SIR_NUM_POLARIS_RATES       3  //72,96,108

#define SIR_MAC_MAX_SUPPORTED_MCS_SET    16


typedef enum eSirBssType
{
    eSIR_INFRASTRUCTURE_MODE,
    eSIR_INFRA_AP_MODE,                    //Added for softAP support
    eSIR_IBSS_MODE,
    eSIR_BTAMP_STA_MODE,                   //Added for BT-AMP support
    eSIR_BTAMP_AP_MODE,                    //Added for BT-AMP support
    eSIR_AUTO_MODE,
    eSIR_DONOT_USE_BSS_TYPE = WLAN_HAL_MAX_ENUM_SIZE
} tSirBssType;

typedef enum eSirNwType
{
    eSIR_11A_NW_TYPE,
    eSIR_11B_NW_TYPE,
    eSIR_11G_NW_TYPE,
    eSIR_11N_NW_TYPE,
    eSIR_DONOT_USE_NW_TYPE = WLAN_HAL_MAX_ENUM_SIZE
} tSirNwType;

typedef u16 tSirMacBeaconInterval;

#define SIR_MAC_RATESET_EID_MAX            12

typedef enum eSirMacHTOperatingMode
{
  eSIR_HT_OP_MODE_PURE,                // No Protection
  eSIR_HT_OP_MODE_OVERLAP_LEGACY,      // Overlap Legacy device present, protection is optional
  eSIR_HT_OP_MODE_NO_LEGACY_20MHZ_HT,  // No legacy device, but 20 MHz HT present
  eSIR_HT_OP_MODE_MIXED,               // Protection is required
  eSIR_HT_OP_MODE_MAX = WLAN_HAL_MAX_ENUM_SIZE
} tSirMacHTOperatingMode;

/// Encryption type enum used with peer
typedef enum eAniEdType
{
    eSIR_ED_NONE,
    eSIR_ED_WEP40,
    eSIR_ED_WEP104,
    eSIR_ED_TKIP,
    eSIR_ED_CCMP,
    eSIR_ED_WPI,
    eSIR_ED_AES_128_CMAC,
    eSIR_ED_NOT_IMPLEMENTED = WLAN_HAL_MAX_ENUM_SIZE
} tAniEdType;

#define WLAN_MAX_KEY_RSC_LEN                16
#define WLAN_WAPI_KEY_RSC_LEN               16

/// MAX key length when ULA is used
#define SIR_MAC_MAX_KEY_LENGTH              32
#define SIR_MAC_MAX_NUM_OF_DEFAULT_KEYS     4

/// Enum to specify whether key is used
/// for TX only, RX only or both
typedef enum eAniKeyDirection
{
    eSIR_TX_ONLY,
    eSIR_RX_ONLY,
    eSIR_TX_RX,
    eSIR_TX_DEFAULT,
    eSIR_DONOT_USE_KEY_DIRECTION = WLAN_HAL_MAX_ENUM_SIZE
} tAniKeyDirection;

typedef enum eAniWepType
{
    eSIR_WEP_STATIC,
    eSIR_WEP_DYNAMIC,
    eSIR_WEP_MAX = WLAN_HAL_MAX_ENUM_SIZE
} tAniWepType;

typedef enum eSriLinkState {

    eSIR_LINK_IDLE_STATE        = 0,
    eSIR_LINK_PREASSOC_STATE    = 1,
    eSIR_LINK_POSTASSOC_STATE   = 2,
    eSIR_LINK_AP_STATE          = 3,
    eSIR_LINK_IBSS_STATE        = 4,

    /* BT-AMP Case */
    eSIR_LINK_BTAMP_PREASSOC_STATE  = 5,
    eSIR_LINK_BTAMP_POSTASSOC_STATE  = 6,
    eSIR_LINK_BTAMP_AP_STATE  = 7,
    eSIR_LINK_BTAMP_STA_STATE  = 8,
    
    /* Reserved for HAL Internal Use */
    eSIR_LINK_LEARN_STATE       = 9,
    eSIR_LINK_SCAN_STATE        = 10,
    eSIR_LINK_FINISH_SCAN_STATE = 11,
    eSIR_LINK_INIT_CAL_STATE    = 12,
    eSIR_LINK_FINISH_CAL_STATE  = 13,
#ifdef WLAN_FEATURE_P2P
    eSIR_LINK_LISTEN_STATE      = 14,
#endif
    eSIR_LINK_MAX = WLAN_HAL_MAX_ENUM_SIZE
} tSirLinkState;

typedef enum
{
    HAL_SUMMARY_STATS_INFO           = 0x00000001,
    HAL_GLOBAL_CLASS_A_STATS_INFO    = 0x00000002,
    HAL_GLOBAL_CLASS_B_STATS_INFO    = 0x00000004,
    HAL_GLOBAL_CLASS_C_STATS_INFO    = 0x00000008,
    HAL_GLOBAL_CLASS_D_STATS_INFO    = 0x00000010,
    HAL_PER_STA_STATS_INFO           = 0x00000020
}eHalStatsMask;

/* BT-AMP events type */
typedef enum 
{
    BTAMP_EVENT_CONNECTION_START,
    BTAMP_EVENT_CONNECTION_STOP,
    BTAMP_EVENT_CONNECTION_TERMINATED,
    BTAMP_EVENT_TYPE_MAX = WLAN_HAL_MAX_ENUM_SIZE, //This and beyond are invalid values
} tBtAmpEventType;

//***************************************************************


/*******************PE Statistics*************************/
typedef enum
{
    PE_SUMMARY_STATS_INFO           = 0x00000001,
    PE_GLOBAL_CLASS_A_STATS_INFO    = 0x00000002,
    PE_GLOBAL_CLASS_B_STATS_INFO    = 0x00000004,
    PE_GLOBAL_CLASS_C_STATS_INFO    = 0x00000008,
    PE_GLOBAL_CLASS_D_STATS_INFO    = 0x00000010,
    PE_PER_STA_STATS_INFO           = 0x00000020,
    PE_STATS_TYPE_MAX = WLAN_HAL_MAX_ENUM_SIZE //This and beyond are invalid values
}ePEStatsMask;

/*---------------------------------------------------------------------------
  Message definitons - All the messages below need to be packed
 ---------------------------------------------------------------------------*/



#if defined(__ANI_COMPILER_PRAGMA_PACK_STACK)
#pragma pack(push, 1)
#elif defined(__ANI_COMPILER_PRAGMA_PACK)
#pragma pack(1)
#else
#endif

/// Definition for HAL API Version.
struct wcnss_wlan_version
{
    u8                  revision;
    u8                  version;
    u8                  minor;
    u8                  major;
} __packed;

/// Definition for Encryption Keys
struct sir_keys
{
    u8                  keyId;
    u8                  unicast;  // 0 for multicast
    tAniKeyDirection         keyDirection;
    u8                  keyRsc[WLAN_MAX_KEY_RSC_LEN];  // Usage is unknown
    u8                  paeRole;  // =1 for authenticator,=0 for supplicant
    u16                 keyLength;
    u8                  key[SIR_MAC_MAX_KEY_LENGTH];
};


//SetStaKeyParams Moving here since it is shared by configbss/setstakey msgs
struct set_sta_key_params
{
    /*STA Index*/
    u16        staIdx;

    /*Encryption Type used with peer*/
    tAniEdType      encType;

    /*STATIC/DYNAMIC - valid only for WEP*/
    tAniWepType     wepType; 

    /*Default WEP key, valid only for static WEP, must between 0 and 3.*/
    u8         defWEPIdx;

    /* valid only for non-static WEP encyrptions */
    struct sir_keys        key[SIR_MAC_MAX_NUM_OF_DEFAULT_KEYS];
  
    /*Control for Replay Count, 1= Single TID based replay count on Tx
      0 = Per TID based replay count on TX */
    u8         singleTidRc;

};



/* 4-byte control message header used by HAL*/
struct hal_msg_header
{
   tHalHostMsgType  msgType:16;
   tHalHostMsgVersion msgVersion:16;
   u32         msgLen;
};

/* Config format required by HAL for each CFG item*/
struct hal_cfg
{
   /* Cfg Id. The Id required by HAL is exported by HAL
    * in shared header file between UMAC and HAL.*/
   u16   uCfgId;

   /* Length of the Cfg. This parameter is used to go to next cfg 
    * in the TLV format.*/
   u16   uCfgLen;

   /* Padding bytes for unaligned address's */
   u16   uCfgPadBytes;

   /* Reserve bytes for making cfgVal to align address */
   u16   uCfgReserve;

   /* Following the uCfgLen field there should be a 'uCfgLen' bytes
    * containing the uCfgValue ; u8 uCfgValue[uCfgLen] */
};

/*---------------------------------------------------------------------------
  WLAN_HAL_START_REQ
---------------------------------------------------------------------------*/

struct hal_mac_start_parameters
{
    /* Drive Type - Production or FTM etc */
    tDriverType  driverType;

    /*Length of the config buffer*/
    u32  uConfigBufferLen;

    /* Following this there is a TLV formatted buffer of length 
     * "uConfigBufferLen" bytes containing all config values. 
     * The TLV is expected to be formatted like this:
     * 0           15            31           31+CFG_LEN-1        length-1
     * |   CFG_ID   |   CFG_LEN   |   CFG_BODY    |  CFG_ID  |......|
     */
} __packed;

struct hal_mac_start_req_msg
{
   /* Note: The length specified in tHalMacStartReqMsg messages should be
    * header.msgLen = sizeof(tHalMacStartReqMsg) + uConfigBufferLen */
   struct hal_msg_header header;
   struct hal_mac_start_parameters startReqParams;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_START_RSP
---------------------------------------------------------------------------*/

struct hal_mac_start_rsp_params
{
   /*success or failure */
   u16  status;

   /*Max number of STA supported by the device*/
   u8     ucMaxStations;

   /*Max number of BSS supported by the device*/
   u8     ucMaxBssids;

   /*API Version */
   struct wcnss_wlan_version wcnssWlanVersion;

   /*CRM build information */
   u8     wcnssCrmVersionString[WLAN_HAL_VERSION_LENGTH];

   /*hardware/chipset/misc version information */
   u8     wcnssWlanVersionString[WLAN_HAL_VERSION_LENGTH];

};

struct hal_mac_start_rsp_msg
{
   struct hal_msg_header header;
   struct hal_mac_start_rsp_params startRspParams;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_STOP_REQ
---------------------------------------------------------------------------*/

struct hal_mac_stop_req_params
{
  /*The reason for which the device is being stopped*/
  tHalStopType   reason;

};

struct hal_mac_stop_req_msg
{
   struct hal_msg_header header;
   struct hal_mac_stop_req_params stopReqParams;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_STOP_RSP
---------------------------------------------------------------------------*/

struct hal_mac_stop_rsp_msg
{
   struct hal_msg_header header;

  /*success or failure */
  u32   status;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_UPDATE_CFG_REQ
---------------------------------------------------------------------------*/

struct hal_update_cfg_req_msg
{
   /* Note: The length specified in tHalUpdateCfgReqMsg messages should be
    * header.msgLen = sizeof(tHalUpdateCfgReqMsg) + uConfigBufferLen */
   struct hal_msg_header header;

    /* Length of the config buffer. Allows UMAC to update multiple CFGs */
    u32  uConfigBufferLen;

    /* Following this there is a TLV formatted buffer of length 
     * "uConfigBufferLen" bytes containing all config values. 
     * The TLV is expected to be formatted like this:
     * 0           15            31           31+CFG_LEN-1        length-1
     * |   CFG_ID   |   CFG_LEN   |   CFG_BODY    |  CFG_ID  |......|
     */

};

/*---------------------------------------------------------------------------
  WLAN_HAL_UPDATE_CFG_RSP
---------------------------------------------------------------------------*/

struct hal_update_cfg_rsp_msg
{
   struct hal_msg_header header;

  /* success or failure */
  u32   status;


};

/*---------------------------------------------------------------------------
  WLAN_HAL_INIT_SCAN_REQ
---------------------------------------------------------------------------*/

/// Frame control field format (2 bytes)
typedef  __ani_attr_pre_packed struct sSirMacFrameCtl
{

#ifndef ANI_LITTLE_BIT_ENDIAN

    u8 subType :4;
    u8 type :2;
    u8 protVer :2;

    u8 order :1;
    u8 wep :1;
    u8 moreData :1;
    u8 powerMgmt :1;
    u8 retry :1;
    u8 moreFrag :1;
    u8 fromDS :1;
    u8 toDS :1;

#else

    u8 protVer :2;
    u8 type :2;
    u8 subType :4;

    u8 toDS :1;
    u8 fromDS :1;
    u8 moreFrag :1;
    u8 retry :1;
    u8 powerMgmt :1;
    u8 moreData :1;
    u8 wep :1;
    u8 order :1;

#endif

} __ani_attr_packed  tSirMacFrameCtl, *tpSirMacFrameCtl;

/// Sequence control field
typedef __ani_attr_pre_packed struct sSirMacSeqCtl
{
    u8 fragNum : 4;
    u8 seqNumLo : 4;
    u8 seqNumHi : 8;
} __ani_attr_packed tSirMacSeqCtl, *tpSirMacSeqCtl;

/// Management header format
typedef __ani_attr_pre_packed struct sSirMacMgmtHdr
{
    tSirMacFrameCtl fc;
    u8           durationLo;
    u8           durationHi;
    u8              da[6];
    u8              sa[6];
    u8              bssId[6];
    tSirMacSeqCtl   seqControl;
} __ani_attr_packed tSirMacMgmtHdr, *tpSirMacMgmtHdr;

/// Scan Entry to hold active BSS idx's
typedef __ani_attr_pre_packed struct sSirScanEntry
{
    u8 bssIdx[HAL_NUM_BSSID];
    u8 activeBSScnt;
}__ani_attr_packed tSirScanEntry, *ptSirScanEntry;

struct hal_init_scan_req_msg
{
    struct hal_msg_header header;

    /*LEARN - AP Role
      SCAN - STA Role*/
    eHalSysMode scanMode;

    /*BSSID of the BSS*/
    tSirMacAddr bssid;

    /*Whether BSS needs to be notified*/
    u8 notifyBss;

    /*Kind of frame to be used for notifying the BSS (Data Null, QoS Null, or
      CTS to Self). Must always be a valid frame type.*/
    u8 frameType;

    /*UMAC has the option of passing the MAC frame to be used for notifying
      the BSS. If non-zero, HAL will use the MAC frame buffer pointed to by
      macMgmtHdr. If zero, HAL will generate the appropriate MAC frame based on
      frameType.*/
    u8 frameLength;

    /* Following the framelength there is a MAC frame buffer if frameLength 
       is non-zero. */
    tSirMacMgmtHdr macMgmtHdr;

    /*Entry to hold number of active BSS idx's*/
    tSirScanEntry scanEntry;
};

struct hal_init_scan_con_req_msg
{
    struct hal_msg_header header;


    /*LEARN - AP Role
      SCAN - STA Role*/
    eHalSysMode scanMode;

    /*BSSID of the BSS*/
    tSirMacAddr bssid;

    /*Whether BSS needs to be notified*/
    u8 notifyBss;

    /*Kind of frame to be used for notifying the BSS (Data Null, QoS Null, or
      CTS to Self). Must always be a valid frame type.*/
    u8 frameType;

    /*UMAC has the option of passing the MAC frame to be used for notifying
      the BSS. If non-zero, HAL will use the MAC frame buffer pointed to by
      macMgmtHdr. If zero, HAL will generate the appropriate MAC frame based on
      frameType.*/
    u8 frameLength;

    /* Following the framelength there is a MAC frame buffer if frameLength 
       is non-zero. */
    tSirMacMgmtHdr macMgmtHdr;

    /*Entry to hold number of active BSS idx's*/
    tSirScanEntry scanEntry;

    /* Single NoA usage in Scanning */
    u8 useNoA;

    /* Indicates the scan duration (in ms) */
    u16 scanDuration;

};


/*---------------------------------------------------------------------------
  WLAN_HAL_INIT_SCAN_RSP
---------------------------------------------------------------------------*/

struct hal_init_scan_rsp_msg
{
   struct hal_msg_header header;

  /*success or failure */
  u32   status;

};

/*---------------------------------------------------------------------------
  WLAN_HAL_START_SCAN_REQ
---------------------------------------------------------------------------*/

struct hal_start_scan_req_msg
{
   struct hal_msg_header header;

   /*Indicates the channel to scan*/
   u8 scanChannel;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_START_SCAN_RSP
---------------------------------------------------------------------------*/

struct hal_start_rsm_msg
{
   struct hal_msg_header header;

  /*success or failure */
  u32   status;

  u32 startTSF[2];
  u8 txMgmtPower;


};

/*---------------------------------------------------------------------------
  WLAN_HAL_END_SCAN_REQ
---------------------------------------------------------------------------*/

struct hal_end_scan_req_msg
{
   struct hal_msg_header header;

   /*Indicates the channel to stop scanning.  Not used really. But retained
    for symmetry with "start Scan" message. It can also help in error
    check if needed.*/
    u8 scanChannel;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_END_SCAN_RSP
---------------------------------------------------------------------------*/

struct hal_end_scan_rsp_msg
{
   struct hal_msg_header header;

  /*success or failure */
  u32   status;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_FINISH_SCAN_REQ
---------------------------------------------------------------------------*/

struct hal_finish_scan_req_msg
{
   struct hal_msg_header header;

    /* Identifies the operational state of the AP/STA
     * LEARN - AP Role SCAN - STA Role */
    eHalSysMode scanMode;

    /*Operating channel to tune to.*/
    u8 currentOperChannel;

    /*Channel Bonding state If 20/40 MHz is operational, this will indicate the
      40 MHz extension channel in combination with the control channel*/
    ePhyChanBondState cbState;

    /*BSSID of the BSS*/
    tSirMacAddr bssid;

    /*Whether BSS needs to be notified*/
    u8 notifyBss;

    /*Kind of frame to be used for notifying the BSS (Data Null, QoS Null, or
     CTS to Self). Must always be a valid frame type.*/
    u8 frameType;

    /*UMAC has the option of passing the MAC frame to be used for notifying
      the BSS. If non-zero, HAL will use the MAC frame buffer pointed to by
      macMgmtHdr. If zero, HAL will generate the appropriate MAC frame based on
      frameType.*/
    u8 frameLength;
    
    /*Following the framelength there is a MAC frame buffer if frameLength 
      is non-zero.*/    
    tSirMacMgmtHdr macMgmtHdr;

    /*Entry to hold number of active BSS idx's*/
    tSirScanEntry scanEntry;

};

/*---------------------------------------------------------------------------
  WLAN_HAL_FINISH_SCAN_RSP
---------------------------------------------------------------------------*/

struct hal_finish_scan_rsp_msg
{
   struct hal_msg_header header;

  /*success or failure */
  u32   status;

};

/*---------------------------------------------------------------------------
  WLAN_HAL_CONFIG_STA_REQ
---------------------------------------------------------------------------*/

struct sir_supported_rates {
    /*
    * For Self STA Entry: this represents Self Mode.
    * For Peer Stations, this represents the mode of the peer.
    * On Station:
    * --this mode is updated when PE adds the Self Entry.
    * -- OR when PE sends 'ADD_BSS' message and station context in BSS is used to indicate the mode of the AP.
    * ON AP:
    * -- this mode is updated when PE sends 'ADD_BSS' and Sta entry for that BSS is used
    *     to indicate the self mode of the AP.
    * -- OR when a station is associated, PE sends 'ADD_STA' message with this mode updated.
    */

    tStaRateMode        opRateMode;
    // 11b, 11a and aniLegacyRates are IE rates which gives rate in unit of 500Kbps
    u16             llbRates[SIR_NUM_11B_RATES];
    u16             llaRates[SIR_NUM_11A_RATES];
    u16             aniLegacyRates[SIR_NUM_POLARIS_RATES];
    u16             reserved;

    //Taurus only supports 26 Titan Rates(no ESF/concat Rates will be supported)
    //First 26 bits are reserved for those Titan rates and
    //the last 4 bits(bit28-31) for Taurus, 2(bit26-27) bits are reserved.
    u32             aniEnhancedRateBitmap; //Titan and Taurus Rates

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

struct config_sta_params
{
    /*BSSID of STA*/
    tSirMacAddr bssId;

    /*ASSOC ID, as assigned by UMAC*/
    u16 assocId;

    /* STA entry Type: 0 - Self, 1 - Other/Peer, 2 - BSSID, 3 - BCAST */
    u8 staType;

    /*Short Preamble Supported.*/
    u8 shortPreambleSupported;

    /*MAC Address of STA*/
    tSirMacAddr staMac;

    /*Listen interval of the STA*/
    u16 listenInterval;

    /*Support for 11e/WMM*/
    u8 wmmEnabled;

    /*11n HT capable STA*/
    u8 htCapable;

    /*TX Width Set: 0 - 20 MHz only, 1 - 20/40 MHz*/
    u8 txChannelWidthSet;

    /*RIFS mode 0 - NA, 1 - Allowed */
    u8 rifsMode;

    /*L-SIG TXOP Protection mechanism 
      0 - No Support, 1 - Supported
      SG - there is global field */
    u8 lsigTxopProtection;

    /*Max Ampdu Size supported by STA. TPE programming.
      0 : 8k , 1 : 16k, 2 : 32k, 3 : 64k */
    u8 maxAmpduSize;

    /*Max Ampdu density. Used by RA.  3 : 0~7 : 2^(11nAMPDUdensity -4)*/
    u8 maxAmpduDensity;

    /*Max AMSDU size 1 : 3839 bytes, 0 : 7935 bytes*/
    u8 maxAmsduSize;

    /*Short GI support for 40Mhz packets*/
    u8 fShortGI40Mhz;

    /*Short GI support for 20Mhz packets*/
    u8 fShortGI20Mhz;

    /*Robust Management Frame (RMF) enabled/disabled*/
    u8 rmfEnabled;

    /* The unicast encryption type in the association */
    u32 encryptType;

    /*HAL should update the existing STA entry, if this flag is set. UMAC
      will set this flag in case of RE-ASSOC, where we want to reuse the old
      STA ID. 0 = Add, 1 = Update*/
    u8 action;

    /*U-APSD Flags: 1b per AC.  Encoded as follows:
       b7 b6 b5 b4 b3 b2 b1 b0 =
       X  X  X  X  BE BK VI VO */
    u8 uAPSD;

    /*Max SP Length*/
    u8 maxSPLen;

    /*11n Green Field preamble support
      0 - Not supported, 1 - Supported */
    u8 greenFieldCapable;

    /*MIMO Power Save mode*/
    tSirMacHTMIMOPowerSaveState mimoPS;

    /*Delayed BA Support*/
    u8 delayedBASupport;

    /*Max AMPDU duration in 32us*/
    u8 us32MaxAmpduDuration;

    /*HT STA should set it to 1 if it is enabled in BSS. HT STA should set
      it to 0 if AP does not support it. This indication is sent to HAL and
      HAL uses this flag to pickup up appropriate 40Mhz rates.*/
    u8 fDsssCckMode40Mhz;

    /* Valid STA Idx when action=Update. Set to 0xFF when invalid!
       Retained for backward compalibity with existing HAL code*/
    u8 staIdx;

    /* BSSID of BSS to which station is associated. Set to 0xFF when invalid.
       Retained for backward compalibity with existing HAL code*/
    u8 bssIdx;

    u8  p2pCapableSta;

    /*Reserved to align next field on a dword boundary*/
    u8  reserved;

    /*These rates are the intersection of peer and self capabilities.*/
    struct sir_supported_rates supportedRates;

};

/*------------------------------------------------------------------------
 * WLAN_HAL_CONFIG_STA_REQ
 * ----------------------------------------------------------------------*/

struct sir_supported_rates_v1 {
    /*
    * For Self STA Entry: this represents Self Mode.
    * For Peer Stations, this represents the mode of the peer.
    * On Station:
    * --this mode is updated when PE adds the Self Entry.
    * -- OR when PE sends 'ADD_BSS' message and station context in BSS is used to indicate the mode of the AP.
    * ON AP:
    * -- this mode is updated when PE sends 'ADD_BSS' and Sta entry for that BSS is used
    *     to indicate the self mode of the AP.
    * -- OR when a station is associated, PE sends 'ADD_STA' message with this mode updated.
    */

    tStaRateMode        opRateMode;
    // 11b, 11a and aniLegacyRates are IE rates which gives rate in unit of 500Kbps
    u16             llbRates[SIR_NUM_11B_RATES];
    u16             llaRates[SIR_NUM_11A_RATES];
    u16             aniLegacyRates[SIR_NUM_POLARIS_RATES];
    u16             reserved;

    //Taurus only supports 26 Titan Rates(no ESF/concat Rates will be supported)
    //First 26 bits are reserved for those Titan rates and
    //the last 4 bits(bit28-31) for Taurus, 2(bit26-27) bits are reserved.
    u32             aniEnhancedRateBitmap; //Titan and Taurus Rates

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

    /*Indicate the highest VHT data rate that the STA is able to receive*/
    u16 vhtRxHighestDataRate;

    /* Indicates the Maximum MCS that can be transmitted  for each number
         * of spacial streams */
    u16 vhtTxMCSMap;

    /*Indicate the highest VHT data rate that the STA is able to transmit*/
    u16 vhtTxHighestDataRate;

};

struct config_sta_params_v1
{
    /*BSSID of STA*/
    tSirMacAddr bssId;

    /*ASSOC ID, as assigned by UMAC*/
    u16 assocId;

    /* STA entry Type: 0 - Self, 1 - Other/Peer, 2 - BSSID, 3 - BCAST */
    u8 staType;

    /*Short Preamble Supported.*/
    u8 shortPreambleSupported;

    /*MAC Address of STA*/
    tSirMacAddr staMac;

    /*Listen interval of the STA*/
    u16 listenInterval;

    /*Support for 11e/WMM*/
    u8 wmmEnabled;

    /*11n HT capable STA*/
    u8 htCapable;

    /*TX Width Set: 0 - 20 MHz only, 1 - 20/40 MHz*/
    u8 txChannelWidthSet;

    /*RIFS mode 0 - NA, 1 - Allowed */
    u8 rifsMode;

    /*L-SIG TXOP Protection mechanism
      0 - No Support, 1 - Supported
      SG - there is global field */
    u8 lsigTxopProtection;

    /*Max Ampdu Size supported by STA. TPE programming.
      0 : 8k , 1 : 16k, 2 : 32k, 3 : 64k */
    u8 maxAmpduSize;

    /*Max Ampdu density. Used by RA.  3 : 0~7 : 2^(11nAMPDUdensity -4)*/
    u8 maxAmpduDensity;

    /*Max AMSDU size 1 : 3839 bytes, 0 : 7935 bytes*/
    u8 maxAmsduSize;

    /*Short GI support for 40Mhz packets*/
    u8 fShortGI40Mhz;

    /*Short GI support for 20Mhz packets*/
    u8 fShortGI20Mhz;

    /*Robust Management Frame (RMF) enabled/disabled*/
    u8 rmfEnabled;

    /* The unicast encryption type in the association */
    u32 encryptType;
    
    /*HAL should update the existing STA entry, if this flag is set. UMAC 
      will set this flag in case of RE-ASSOC, where we want to reuse the old
      STA ID. 0 = Add, 1 = Update*/
    u8 action;

    /*U-APSD Flags: 1b per AC.  Encoded as follows:
       b7 b6 b5 b4 b3 b2 b1 b0 =
       X  X  X  X  BE BK VI VO */
    u8 uAPSD;

    /*Max SP Length*/
    u8 maxSPLen;

    /*11n Green Field preamble support
      0 - Not supported, 1 - Supported */
    u8 greenFieldCapable;

    /*MIMO Power Save mode*/
    tSirMacHTMIMOPowerSaveState mimoPS;

    /*Delayed BA Support*/
    u8 delayedBASupport;
    
    /*Max AMPDU duration in 32us*/
    u8 us32MaxAmpduDuration;
    
    /*HT STA should set it to 1 if it is enabled in BSS. HT STA should set
      it to 0 if AP does not support it. This indication is sent to HAL and
      HAL uses this flag to pickup up appropriate 40Mhz rates.*/
    u8 fDsssCckMode40Mhz;

    /* Valid STA Idx when action=Update. Set to 0xFF when invalid!
       Retained for backward compalibity with existing HAL code*/
    u8 staIdx;

    /* BSSID of BSS to which station is associated. Set to 0xFF when invalid.
       Retained for backward compalibity with existing HAL code*/
    u8 bssIdx;

    u8  p2pCapableSta;

    /*Reserved to align next field on a dword boundary*/
    u8 htLdpcEnabled:1;
    u8 vhtLdpcEnabled:1;
    u8 vhtTxBFEnabled:1;
    u8 reserved:5;

        /*These rates are the intersection of peer and self capabilities.*/
    struct sir_supported_rates_v1 supportedRates;

    u8  vhtCapable;
    u8  vhtTxChannelWidthSet;

};

struct config_sta_req_msg
{
   struct hal_msg_header header;
   PACKED_PRE union {
   struct config_sta_params configStaParams;
   struct config_sta_params_v1 configStaParams_V1;
   } uStaParams;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_CONFIG_STA_RSP
---------------------------------------------------------------------------*/

struct config_sta_rsp_params
{
  /*success or failure */
  u32   status;

  /* Station index; valid only when 'status' field value SUCCESS */
  u8 staIdx;

  /* BSSID Index of BSS to which the station is associated */
  u8 bssIdx;

  /* DPU Index for PTK */
  u8 dpuIndex;

  /* DPU Index for GTK */  
  u8 bcastDpuIndex;

  /*DPU Index for IGTK  */
  u8 bcastMgmtDpuIdx;

  /*PTK DPU signature*/
  u8 ucUcastSig;

  /*GTK DPU isignature*/
  u8 ucBcastSig;

  /* IGTK DPU signature*/
  u8 ucMgmtSig;

  u8  p2pCapableSta;

};

struct config_sta_rsp_msg
{
   struct hal_msg_header header;
   struct config_sta_rsp_params configStaRspParams;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_DELETE_STA_REQ
---------------------------------------------------------------------------*/

/* Delete STA Request message*/
struct delete_sta_req_msg
{
   struct hal_msg_header header;

   /* Index of STA to delete */
   u8    staIdx;

};

/*---------------------------------------------------------------------------
  WLAN_HAL_DELETE_STA_RSP
---------------------------------------------------------------------------*/

/* Delete STA Response message*/
struct delete_sta_rsp_msg
{
   struct hal_msg_header header;

   /*success or failure */
   u32   status;

   /* Index of STA deleted */
   u8    staId;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_CONFIG_BSS_REQ
---------------------------------------------------------------------------*/

//12 Bytes long because this structure can be used to represent rate
//and extended rate set IEs. The parser assume this to be at least 12
typedef __ani_attr_pre_packed struct sSirMacRateSet
{
    u8  numRates;
    u8  rate[SIR_MAC_RATESET_EID_MAX];
} __ani_attr_packed tSirMacRateSet;

// access category record
typedef __ani_attr_pre_packed struct sSirMacAciAifsn
{
#ifndef ANI_LITTLE_BIT_ENDIAN
    u8  rsvd  : 1;
    u8  aci   : 2;
    u8  acm   : 1;
    u8  aifsn : 4;
#else
    u8  aifsn : 4;
    u8  acm   : 1;
    u8  aci   : 2;
    u8  rsvd  : 1;
#endif
} __ani_attr_packed tSirMacAciAifsn;

// contention window size
typedef __ani_attr_pre_packed struct sSirMacCW
{
#ifndef ANI_LITTLE_BIT_ENDIAN
    u8  max : 4;
    u8  min : 4;
#else
    u8  min : 4;
    u8  max : 4;
#endif
} __ani_attr_packed tSirMacCW;

typedef __ani_attr_pre_packed struct sSirMacEdcaParamRecord
{
    tSirMacAciAifsn  aci;
    tSirMacCW        cw;
    u16         txoplimit;
} __ani_attr_packed tSirMacEdcaParamRecord;

typedef __ani_attr_pre_packed struct sSirMacSSid
{
    u8        length;
    u8        ssId[32];
} __ani_attr_packed tSirMacSSid;

// Concurrency role.  These are generic IDs that identify the various roles
// in the software system.
typedef enum {
    HAL_STA_MODE=0, 
    HAL_STA_SAP_MODE=1, // to support softAp mode . This is misleading. It means AP MODE only. 
    HAL_P2P_CLIENT_MODE,
    HAL_P2P_GO_MODE,
    HAL_MONITOR_MODE,
} tHalConMode;

//This is a bit pattern to be set for each mode
//bit 0 - sta mode
//bit 1 - ap mode
//bit 2 - p2p client mode
//bit 3 - p2p go mode
typedef enum
{
    HAL_STA=1, 
    HAL_SAP=2,
    HAL_STA_SAP=3, //to support sta, softAp  mode . This means STA+AP mode
    HAL_P2P_CLIENT=4,
    HAL_P2P_GO=8,
    HAL_MAX_CONCURRENCY_PERSONA=4
} tHalConcurrencyMode;

struct config_bss_params
{
    /* BSSID */
    tSirMacAddr bssId;

    /* Self Mac Address */
    tSirMacAddr  selfMacAddr;

    /* BSS type */
    tSirBssType bssType;

    /*Operational Mode: AP =0, STA = 1*/
    u8 operMode;

    /*Network Type*/
    tSirNwType nwType;

    /*Used to classify PURE_11G/11G_MIXED to program MTU*/
    u8 shortSlotTimeSupported;

    /*Co-exist with 11a STA*/
    u8 llaCoexist;

    /*Co-exist with 11b STA*/
    u8 llbCoexist;

    /*Co-exist with 11g STA*/
    u8 llgCoexist;

    /*Coexistence with 11n STA*/
    u8 ht20Coexist;

    /*Non GF coexist flag*/
    u8 llnNonGFCoexist;

    /*TXOP protection support*/
    u8 fLsigTXOPProtectionFullSupport;

    /*RIFS mode*/
    u8 fRIFSMode;

    /*Beacon Interval in TU*/
    tSirMacBeaconInterval beaconInterval;

    /*DTIM period*/
    u8 dtimPeriod;

    /*TX Width Set: 0 - 20 MHz only, 1 - 20/40 MHz*/
    u8 txChannelWidthSet;

    /*Operating channel*/
    u8 currentOperChannel;

    /*Extension channel for channel bonding*/
    u8 currentExtChannel;

    /*Reserved to align next field on a dword boundary*/
    u8 reserved;

    /*SSID of the BSS*/
    tSirMacSSid ssId;

    /*HAL should update the existing BSS entry, if this flag is set.
      UMAC will set this flag in case of reassoc, where we want to resue the
      the old BSSID and still return success 0 = Add, 1 = Update*/
    u8 action;

    /* MAC Rate Set */
    tSirMacRateSet rateSet;

    /*Enable/Disable HT capabilities of the BSS*/
    u8 htCapable;

    // Enable/Disable OBSS protection
    u8 obssProtEnabled;

    /*RMF enabled/disabled*/
    u8 rmfEnabled;

    /*HT Operating Mode operating mode of the 802.11n STA*/
    tSirMacHTOperatingMode htOperMode;

    /*Dual CTS Protection: 0 - Unused, 1 - Used*/
    u8 dualCTSProtection;

    /* Probe Response Max retries */
    u8   ucMaxProbeRespRetryLimit;

    /* To Enable Hidden ssid */
    u8   bHiddenSSIDEn;

    /* To Enable Disable FW Proxy Probe Resp */
    u8   bProxyProbeRespEn;

    /* Boolean to indicate if EDCA params are valid. UMAC might not have valid
       EDCA params or might not desire to apply EDCA params during config BSS.
       0 implies Not Valid ; Non-Zero implies valid*/
    u8   edcaParamsValid;

    /*EDCA Parameters for Best Effort Access Category*/
    tSirMacEdcaParamRecord acbe;

    /*EDCA Parameters forBackground Access Category*/
    tSirMacEdcaParamRecord acbk;

    /*EDCA Parameters for Video Access Category*/
    tSirMacEdcaParamRecord acvi;

    /*EDCA Parameters for Voice Access Category*/
    tSirMacEdcaParamRecord acvo;

#ifdef WLAN_FEATURE_VOWIFI_11R
    u8 extSetStaKeyParamValid; //Ext Bss Config Msg if set
    tSetStaKeyParams extSetStaKeyParam;  //SetStaKeyParams for ext bss msg
#endif

    /* Persona for the BSS can be STA,AP,GO,CLIENT value same as tHalConMode */
    u8   halPersona;

    u8 bSpectrumMgtEnable;

    /*HAL fills in the tx power used for mgmt frames in txMgmtPower*/
    s8     txMgmtPower;
    /*maxTxPower has max power to be used after applying the power constraint if any */
    s8     maxTxPower;
    /*Context of the station being added in HW
      Add a STA entry for "itself" -
      On AP  - Add the AP itself in an "STA context"
      On STA - Add the AP to which this STA is joining in an "STA context" */
    struct config_sta_params staContext;
};


/*--------------------------------------------------------------------------
 * WLAN_HAL_CONFIG_BSS_REQ
 *--------------------------------------------------------------------------*/
struct config_bss_params_v1
{
    /* BSSID */
    tSirMacAddr bssId;

    /* Self Mac Address */
    tSirMacAddr  selfMacAddr;

    /* BSS type */
    tSirBssType bssType;

    /*Operational Mode: AP =0, STA = 1*/
    u8 operMode;

    /*Network Type*/
    tSirNwType nwType;

    /*Used to classify PURE_11G/11G_MIXED to program MTU*/
    u8 shortSlotTimeSupported;

    /*Co-exist with 11a STA*/
    u8 llaCoexist;

    /*Co-exist with 11b STA*/
    u8 llbCoexist;

    /*Co-exist with 11g STA*/
    u8 llgCoexist;

    /*Coexistence with 11n STA*/
    u8 ht20Coexist;

    /*Non GF coexist flag*/
    u8 llnNonGFCoexist;

    /*TXOP protection support*/
    u8 fLsigTXOPProtectionFullSupport;
    /*RIFS mode*/
    u8 fRIFSMode;

    /*Beacon Interval in TU*/
    tSirMacBeaconInterval beaconInterval;

    /*DTIM period*/
    u8 dtimPeriod;

    /*TX Width Set: 0 - 20 MHz only, 1 - 20/40 MHz*/
    u8 txChannelWidthSet;

    /*Operating channel*/
    u8 currentOperChannel;

    /*Extension channel for channel bonding*/
    u8 currentExtChannel;

    /*Reserved to align next field on a dword boundary*/
    u8 reserved;

    /*SSID of the BSS*/
    tSirMacSSid ssId;

    /*HAL should update the existing BSS entry, if this flag is set.
      UMAC will set this flag in case of reassoc, where we want to resue the
      the old BSSID and still return success 0 = Add, 1 = Update*/
    u8 action;

    /* MAC Rate Set */
    tSirMacRateSet rateSet;

    /*Enable/Disable HT capabilities of the BSS*/
    u8 htCapable;

    // Enable/Disable OBSS protection
    u8 obssProtEnabled;

    /*RMF enabled/disabled*/
    u8 rmfEnabled;

    /*HT Operating Mode operating mode of the 802.11n STA*/
    tSirMacHTOperatingMode htOperMode;

    /*Dual CTS Protection: 0 - Unused, 1 - Used*/
    u8 dualCTSProtection;

    /* Probe Response Max retries */
    u8   ucMaxProbeRespRetryLimit;

    /* To Enable Hidden ssid */
    u8   bHiddenSSIDEn;

    /* To Enable Disable FW Proxy Probe Resp */
    u8   bProxyProbeRespEn;

    /* Boolean to indicate if EDCA params are valid. UMAC might not have valid 
       EDCA params or might not desire to apply EDCA params during config BSS. 
       0 implies Not Valid ; Non-Zero implies valid*/
    u8   edcaParamsValid;

    /*EDCA Parameters for Best Effort Access Category*/
    tSirMacEdcaParamRecord acbe;
    
    /*EDCA Parameters forBackground Access Category*/
    tSirMacEdcaParamRecord acbk;

    /*EDCA Parameters for Video Access Category*/
    tSirMacEdcaParamRecord acvi;

    /*EDCA Parameters for Voice Access Category*/
    tSirMacEdcaParamRecord acvo;

#ifdef WLAN_FEATURE_VOWIFI_11R
    u8 extSetStaKeyParamValid; //Ext Bss Config Msg if set
    tSetStaKeyParams extSetStaKeyParam;  //SetStaKeyParams for ext bss msg
#endif

    /* Persona for the BSS can be STA,AP,GO,CLIENT value same as tHalConMode */    
    u8   halPersona;
   
    u8 bSpectrumMgtEnable;

    /*HAL fills in the tx power used for mgmt frames in txMgmtPower*/
    s8     txMgmtPower;
    /*maxTxPower has max power to be used after applying the power constraint if any */
    s8     maxTxPower;
    /*Context of the station being added in HW
      Add a STA entry for "itself" -
      On AP  - Add the AP itself in an "STA context"
      On STA - Add the AP to which this STA is joining in an "STA context" */
    struct config_sta_params_v1 staContext;
  
    u8   vhtCapable;
    u8   vhtTxChannelWidthSet;
};

struct config_bss_req_msg
{
   struct hal_msg_header header;
   PACKED_PRE union {
   struct config_bss_params configBssParams;
   struct config_bss_params_v1 configBssParams_V1;
   }uBssParams;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_CONFIG_BSS_RSP
---------------------------------------------------------------------------*/

struct config_bss_rsp_params
{
    /* Success or Failure */
    u32   status;

    /* BSS index allocated by HAL */
    u8    bssIdx;

    /* DPU descriptor index for PTK */
    u8    dpuDescIndx;

    /* PTK DPU signature */
    u8    ucastDpuSignature;

    /* DPU descriptor index for GTK*/
    u8    bcastDpuDescIndx;

    /* GTK DPU signature */
    u8    bcastDpuSignature;

    /*DPU descriptor for IGTK*/
    u8    mgmtDpuDescIndx;

    /* IGTK DPU signature */
    u8    mgmtDpuSignature;

    /* Station Index for BSS entry*/
    u8     bssStaIdx;

    /* Self station index for this BSS */
    u8     bssSelfStaIdx;

    /* Bcast station for buffering bcast frames in AP role */
    u8     bssBcastStaIdx;

    /*MAC Address of STA(PEER/SELF) in staContext of configBSSReq*/
    tSirMacAddr   staMac;

    /*HAL fills in the tx power used for mgmt frames in this field. */
    s8     txMgmtPower;

};

struct config_bss_rsp_msg
{
   struct hal_msg_header header;
   struct config_bss_rsp_params configBssRspParams;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_DELETE_BSS_REQ
---------------------------------------------------------------------------*/

struct delete_bss_req_msg
{
   struct hal_msg_header header;

    /* BSS index to be deleted */
    u8 bssIdx;

};

/*---------------------------------------------------------------------------
  WLAN_HAL_DELETE_BSS_RSP
---------------------------------------------------------------------------*/

struct delete_bss_rsp_msg
{
   struct hal_msg_header header;

    /* Success or Failure */
    u32   status;

    /* BSS index that has been deleted */
    u8 bssIdx;

};

/*---------------------------------------------------------------------------
  WLAN_HAL_JOIN_REQ
---------------------------------------------------------------------------*/

struct hal_join_req_msg
{
   struct hal_msg_header header;

  /*Indicates the BSSID to which STA is going to associate*/
  tSirMacAddr     bssId; 

  /*Indicates the channel to switch to.*/
  u8         ucChannel;

  /* Self STA MAC */
  tSirMacAddr selfStaMacAddr;
     
  /*Local power constraint*/
  u8         ucLocalPowerConstraint;

  /*Secondary channel offset */
  ePhyChanBondState  secondaryChannelOffset;

  /*link State*/
  tSirLinkState   linkState;

  /* Max TX power */
  s8 maxTxPower;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_JOIN_RSP
---------------------------------------------------------------------------*/

struct hal_join_rsp_msg
{
   struct hal_msg_header header;

  /*success or failure */
  u32   status;

  /* HAL fills in the tx power used for mgmt frames in this field */
  u8 txMgmtPower;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_POST_ASSOC_REQ
---------------------------------------------------------------------------*/

struct post_assoc_req_msg
{
   struct hal_msg_header header;

   struct config_sta_params configStaParams;
   struct config_bss_params configBssParams;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_POST_ASSOC_RSP
---------------------------------------------------------------------------*/

struct post_assoc_rsp_msg
{
   struct hal_msg_header header;
   struct config_sta_rsp_params configStaRspParams;
   struct config_bss_rsp_params configBssRspParams;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_SET_BSSKEY_REQ
---------------------------------------------------------------------------*/

/*
 * This is used by PE to create a set of WEP keys for a given BSS.
 */
struct set_bss_key_req_msg
{
   struct hal_msg_header header;

    /*BSS Index of the BSS*/
    u8         bssIdx;

    /*Encryption Type used with peer*/
    tAniEdType      encType;

    /*Number of keys*/
    u8         numKeys;

    /*Array of keys.*/
    struct sir_keys        key[SIR_MAC_MAX_NUM_OF_DEFAULT_KEYS];
  
    /*Control for Replay Count, 1= Single TID based replay count on Tx
    0 = Per TID based replay count on TX */
    u8         singleTidRc;
};

/* tagged version of set bss key */
struct set_bss_key_req_msg_tagged
{
   struct set_bss_key_req_msg Msg;
   u32            Tag;
};

/*---------------------------------------------------------------------------
  WLAN_HAL_SET_BSSKEY_RSP
---------------------------------------------------------------------------*/
struct set_bss_key_rsp_msg
{
   struct hal_msg_header header;

  /*success or failure */
  u32   status;

};

/*---------------------------------------------------------------------------
   WLAN_HAL_SET_STAKEY_REQ,
---------------------------------------------------------------------------*/

/*
 * This is used by PE to configure the key information on a given station.
 * When the secType is WEP40 or WEP104, the defWEPIdx is used to locate
 * a preconfigured key from a BSS the station assoicated with; otherwise
 * a new key descriptor is created based on the key field.
 */

struct set_sta_key_req_msg
{
   struct hal_msg_header header;
   struct set_sta_key_params setStaKeyParams;
};

/*---------------------------------------------------------------------------
   WLAN_HAL_SET_STAKEY_RSP,
---------------------------------------------------------------------------*/
struct set_sta_key_rsp_msg
{
   struct hal_msg_header header;

  /*success or failure */
  u32   status;
};

/*---------------------------------------------------------------------------
   WLAN_HAL_RMV_BSSKEY_REQ,
---------------------------------------------------------------------------*/

struct remove_bss_key_req_msg
{
   struct hal_msg_header header;

    /*BSS Index of the BSS*/
    u8         bssIdx;
    
    /*Encryption Type used with peer*/
    tAniEdType      encType;

    /*Key Id*/
    u8         keyId;

    /*STATIC/DYNAMIC. Used in Nullifying in Key Descriptors for Static/Dynamic keys*/
    tAniWepType    wepType;

};

/*---------------------------------------------------------------------------
   WLAN_HAL_RMV_BSSKEY_RSP,
---------------------------------------------------------------------------*/
struct remove_bss_key_rsp_msg
{
   struct hal_msg_header header;

  /*success or failure */
  u32   status;

};

/*---------------------------------------------------------------------------
   WLAN_HAL_RMV_STAKEY_REQ,
---------------------------------------------------------------------------*/
/*
 * This is used by PE to Remove the key information on a given station.
 */
struct remove_sta_key_req_msg
{
   struct hal_msg_header header;

    /*STA Index*/
    u16         staIdx;

    /*Encryption Type used with peer*/
    tAniEdType      encType;

    /*Key Id*/
    u8           keyId;

    /*Whether to invalidate the Broadcast key or Unicast key. In case of WEP,
      the same key is used for both broadcast and unicast.*/
    u8    unicast;

};

/*---------------------------------------------------------------------------
   WLAN_HAL_RMV_STAKEY_RSP,
---------------------------------------------------------------------------*/
struct remove_sta_key_rsp_msg
{
   struct hal_msg_header header;

  /*success or failure */
  u32   status;

};

#ifdef FEATURE_OEM_DATA_SUPPORT

#ifndef OEM_DATA_REQ_SIZE
#define OEM_DATA_REQ_SIZE 134
#endif

#ifndef OEM_DATA_RSP_SIZE
#define OEM_DATA_RSP_SIZE 1968
#endif

/*-------------------------------------------------------------------------
WLAN_HAL_START_OEM_DATA_REQ
--------------------------------------------------------------------------*/
struct start_oem_data_req_msg
{
    struct hal_msg_header                header;

    u32                 status;
    tSirMacAddr              selfMacAddr;
    u8                 oemDataReq[OEM_DATA_REQ_SIZE];

};

/*-------------------------------------------------------------------------
WLAN_HAL_START_OEM_DATA_RSP
--------------------------------------------------------------------------*/

struct start_oem_data_rsp_msg
{
   struct hal_msg_header             header;

   u8                   oemDataRsp[OEM_DATA_RSP_SIZE];
};

#endif

/*---------------------------------------------------------------------------
WLAN_HAL_CH_SWITCH_REQ
---------------------------------------------------------------------------*/

struct switch_channel_req_msg
{
   struct hal_msg_header header;

    /* Channel number */
    u8 channelNumber;

    /* Local power constraint */
    u8 localPowerConstraint;

    /*Secondary channel offset */
    ePhyChanBondState secondaryChannelOffset;

    //HAL fills in the tx power used for mgmt frames in this field.
    u8 txMgmtPower;

    /* Max TX power */
    u8 maxTxPower;
    
    /* Self STA MAC */
    tSirMacAddr selfStaMacAddr;

    /*VO WIFI comment: BSSID needed to identify session. As the request has power constraints,
       this should be applied only to that session*/
    /* Since MTU timing and EDCA are sessionized, this struct needs to be sessionized and
     * bssid needs to be out of the VOWifi feature flag */
    /* V IMP: Keep bssId field at the end of this msg. It is used to mantain backward compatbility
     * by way of ignoring if using new host/old FW or old host/new FW since it is at the end of this struct
     */
    tSirMacAddr bssId;
};

/*---------------------------------------------------------------------------
WLAN_HAL_CH_SWITCH_RSP
---------------------------------------------------------------------------*/

struct switch_channel_rsp_msg
{
   struct hal_msg_header header;

    /* Status */
    u32 status;

    /* Channel number - same as in request*/
    u8 channelNumber;

    /* HAL fills in the tx power used for mgmt frames in this field */
    u8 txMgmtPower;

    /* BSSID needed to identify session - same as in request*/
    tSirMacAddr bssId;

};

/*---------------------------------------------------------------------------
WLAN_HAL_UPD_EDCA_PARAMS_REQ
---------------------------------------------------------------------------*/

struct update_edca_params_req_msg
{
   struct hal_msg_header header;

   /*BSS Index*/
   u16 bssIdx;

   /* Best Effort */
   tSirMacEdcaParamRecord acbe; 

   /* Background */
   tSirMacEdcaParamRecord acbk;
   
   /* Video */
   tSirMacEdcaParamRecord acvi;

   /* Voice */
   tSirMacEdcaParamRecord acvo;
};

/*---------------------------------------------------------------------------
WLAN_HAL_UPD_EDCA_PARAMS_RSP
---------------------------------------------------------------------------*/
struct update_edca_params_rsp_msg
{
   struct hal_msg_header header;

  /*success or failure */
  u32   status;
};



/*---------------------------------------------------------------------------
 * WLAN_HAL_GET_STATS_REQ
 *--------------------------------------------------------------------------*/

struct dpu_stats_params

{
    /* Index of STA to which the statistics */
    u16 staIdx;

    /* Encryption mode */
    u8 encMode;
    
    /* status */
    u32  status;
    
    /* Statistics */
    u32  sendBlocks;
    u32  recvBlocks;
    u32  replays;
    u8   micErrorCnt;
    u32  protExclCnt;
    u16  formatErrCnt;
    u16  unDecryptableCnt;
    u32  decryptErrCnt;
    u32  decryptOkCnt;
};

struct hal_stats_req_msg
{
   struct hal_msg_header        header;

   /* Valid STA Idx for per STA stats request */
   u32    staId;

   /* Categories of stats requested as specified in eHalStatsMask*/
   u32    statsMask;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_GET_STATS_RSP
 *--------------------------------------------------------------------------*/

struct ani_summary_stats_info
{
    u32 retry_cnt[4];          //Total number of packets(per AC) that were successfully transmitted with retries
    u32 multiple_retry_cnt[4]; //The number of MSDU packets and MMPDU frames per AC that the 802.11
                                    // station successfully transmitted after more than one retransmission attempt

    u32 tx_frm_cnt[4];         //Total number of packets(per AC) that were successfully transmitted 
                                    //(with and without retries, including multi-cast, broadcast)     
    u32 rx_frm_cnt;            //Total number of packets that were successfully received 
                                    //(after appropriate filter rules including multi-cast, broadcast)    
    u32 frm_dup_cnt;           //Total number of duplicate frames received successfully
    u32 fail_cnt[4];           //Total number packets(per AC) failed to transmit
    u32 rts_fail_cnt;          //Total number of RTS/CTS sequence failures for transmission of a packet
    u32 ack_fail_cnt;          //Total number packets failed transmit because of no ACK from the remote entity
    u32 rts_succ_cnt;          //Total number of RTS/CTS sequence success for transmission of a packet 
    u32 rx_discard_cnt;        //The sum of the receive error count and dropped-receive-buffer error count. 
                                    //HAL will provide this as a sum of (FCS error) + (Fail get BD/PDU in HW)
    u32 rx_error_cnt;          //The receive error count. HAL will provide the RxP FCS error global counter.
    u32 tx_byte_cnt;           //The sum of the transmit-directed byte count, transmit-multicast byte count 
                                    //and transmit-broadcast byte count. HAL will sum TPE UC/MC/BCAST global counters 
                                    //to provide this.
};

// defines tx_rate_flags
typedef enum eTxRateInfo
{
   eHAL_TX_RATE_LEGACY = 0x1,    /* Legacy rates */
   eHAL_TX_RATE_HT20   = 0x2,    /* HT20 rates */
   eHAL_TX_RATE_HT40   = 0x4,    /* HT40 rates */
   eHAL_TX_RATE_SGI    = 0x8,    /* Rate with Short guard interval */
   eHAL_TX_RATE_LGI    = 0x10    /* Rate with Long guard interval */
} tTxrateinfoflags;


struct ani_global_class_a_stats_info
{
    u32 rx_frag_cnt;              //The number of MPDU frames received by the 802.11 station for MSDU packets 
                                       //or MMPDU frames
    u32 promiscuous_rx_frag_cnt;  //The number of MPDU frames received by the 802.11 station for MSDU packets 
                                       //or MMPDU frames when a promiscuous packet filter was enabled
    u32 rx_input_sensitivity;     //The receiver input sensitivity referenced to a FER of 8% at an MPDU length 
                                       //of 1024 bytes at the antenna connector. Each element of the array shall correspond 
                                       //to a supported rate and the order shall be the same as the supporteRates parameter.
    u32 max_pwr;                  //The maximum transmit power in dBm upto one decimal. 
                                       //for eg: if it is 10.5dBm, the value would be 105 
    u32 sync_fail_cnt;            //Number of times the receiver failed to synchronize with the incoming signal 
                                       //after detecting the sync in the preamble of the transmitted PLCP protocol data unit. 

    u32 tx_rate;                  //Legacy transmit rate, in units of 500 kbit/sec, for the most 
                                       //recently transmitted frame    
    u32  mcs_index;               //mcs index for HT20 and HT40 rates
    u32  tx_rate_flags;           //to differentiate between HT20 and 
                                       //HT40 rates;  short and long guard interval  
};

struct ani_global_security_stats
{
    u32 rx_wep_unencrypted_frm_cnt;  //The number of unencrypted received MPDU frames that the MAC layer discarded when 
                                          //the IEEE 802.11 dot11ExcludeUnencrypted management information base (MIB) object 
                                          //is enabled
    u32 rx_mic_fail_cnt;             //The number of received MSDU packets that that the 802.11 station discarded 
                                          //because of MIC failures
    u32 tkip_icv_err;                //The number of encrypted MPDU frames that the 802.11 station failed to decrypt 
                                          //because of a TKIP ICV error
    u32 aes_ccmp_format_err;         //The number of received MPDU frames that the 802.11 discarded because of an 
                                          //invalid AES-CCMP format
    u32 aes_ccmp_replay_cnt;         //The number of received MPDU frames that the 802.11 station discarded because of 
                                          //the AES-CCMP replay protection procedure
    u32 aes_ccmp_decrpt_err;         //The number of received MPDU frames that the 802.11 station discarded because of 
                                          //errors detected by the AES-CCMP decryption algorithm
    u32 wep_undecryptable_cnt;       //The number of encrypted MPDU frames received for which a WEP decryption key was 
                                          //not available on the 802.11 station
    u32 wep_icv_err;                 //The number of encrypted MPDU frames that the 802.11 station failed to decrypt 
                                          //because of a WEP ICV error
    u32 rx_decrypt_succ_cnt;         //The number of received encrypted packets that the 802.11 station successfully 
                                          //decrypted
    u32 rx_decrypt_fail_cnt;         //The number of encrypted packets that the 802.11 station failed to decrypt

};
   
struct ani_global_class_b_stats_info
{
    struct ani_global_security_stats ucStats;
    struct ani_global_security_stats mcbcStats;
};

struct ani_global_class_c_stats_info
{
    u32 rx_amsdu_cnt;             //This counter shall be incremented for a received A-MSDU frame with the stations 
                                       //MAC address in the address 1 field or an A-MSDU frame with a group address in the 
                                       //address 1 field
    u32 rx_ampdu_cnt;             //This counter shall be incremented when the MAC receives an AMPDU from the PHY
    u32 tx_20_frm_cnt;            //This counter shall be incremented when a Frame is transmitted only on the 
                                       //primary channel
    u32 rx_20_frm_cnt;            //This counter shall be incremented when a Frame is received only on the primary channel
    u32 rx_mpdu_in_ampdu_cnt;     //This counter shall be incremented by the number of MPDUs received in the A-MPDU 
                                       //when an A-MPDU is received
    u32 ampdu_delimiter_crc_err;  //This counter shall be incremented when an MPDU delimiter has a CRC error when this 
                                       //is the first CRC error in the received AMPDU or when the previous delimiter has been 
                                       //decoded correctly
};

struct ani_per_sta_stats_info
{
    u32 tx_frag_cnt[4];        //The number of MPDU frames that the 802.11 station transmitted and acknowledged 
                                    //through a received 802.11 ACK frame
    u32 tx_ampdu_cnt;          //This counter shall be incremented when an A-MPDU is transmitted 
    u32 tx_mpdu_in_ampdu_cnt;  //This counter shall increment by the number of MPDUs in the AMPDU when an A-MPDU 
                                    //is transmitted
};

struct hal_stats_rsp_msg
{
   struct hal_msg_header  header;

   /* Success or Failure */
   u32 status;

   /* STA Idx */
   u32 staId;

   /* Categories of STATS being returned as per eHalStatsMask*/
   u32 statsMask;

   /* message type is same as the request type */
   u16 msgType;

   /* length of the entire request, includes the pStatsBuf length too */
   u16 msgLen;  

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_SET_LINK_ST_REQ
 *--------------------------------------------------------------------------*/
struct set_link_state_req_msg
{
   struct hal_msg_header header;

    tSirMacAddr bssid;
    tSirLinkState state;
    tSirMacAddr selfMacAddr;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_SET_LINK_ST_RSP
 *--------------------------------------------------------------------------*/

struct set_link_state_rsp_msg
{
   struct hal_msg_header header;

  /*success or failure */
  u32   status;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ADD_TS_REQ
 *--------------------------------------------------------------------------*/

/* TSPEC Params */
typedef __ani_attr_pre_packed struct sSirMacTSInfoTfc
{
#ifndef ANI_LITTLE_BIT_ENDIAN
    u16       ackPolicy : 2;
    u16       userPrio : 3;
    u16       psb : 1;
    u16       aggregation : 1;
    u16       accessPolicy : 2;
    u16       direction : 2;
    u16       tsid : 4;
    u16       trafficType : 1;
#else
    u16       trafficType : 1;
    u16       tsid : 4;
    u16       direction : 2;
    u16       accessPolicy : 2;
    u16       aggregation : 1;
    u16       psb : 1;
    u16       userPrio : 3;
    u16       ackPolicy : 2;
#endif
} __ani_attr_packed tSirMacTSInfoTfc;

/* Flag to schedule the traffic type */
typedef __ani_attr_pre_packed struct sSirMacTSInfoSch
{
#ifndef ANI_LITTLE_BIT_ENDIAN
    u8        rsvd : 7;
    u8        schedule : 1;
#else
    u8        schedule : 1;
    u8        rsvd : 7;
#endif
} __ani_attr_packed tSirMacTSInfoSch;

/* Traffic and scheduling info */
typedef __ani_attr_pre_packed struct sSirMacTSInfo
{
    tSirMacTSInfoTfc traffic;
    tSirMacTSInfoSch schedule;
} __ani_attr_packed tSirMacTSInfo;

/* Information elements */
typedef __ani_attr_pre_packed struct sSirMacTspecIE
{
    u8             type;
    u8             length;
    tSirMacTSInfo       tsinfo;
    u16            nomMsduSz;
    u16            maxMsduSz;
    u32            minSvcInterval;
    u32            maxSvcInterval;
    u32            inactInterval;
    u32            suspendInterval;
    u32            svcStartTime;
    u32            minDataRate;
    u32            meanDataRate;
    u32            peakDataRate;
    u32            maxBurstSz;
    u32            delayBound;
    u32            minPhyRate;
    u16            surplusBw;
    u16            mediumTime;
}__ani_attr_packed tSirMacTspecIE;

struct add_ts_req_msg
{
    struct hal_msg_header header;

    /* Station Index */
    u16 staIdx;

    /* TSPEC handler uniquely identifying a TSPEC for a STA in a BSS */
    u16 tspecIdx;

    /* To program TPE with required parameters */
    tSirMacTspecIE   tspec;

    /* U-APSD Flags: 1b per AC.  Encoded as follows:
     b7 b6 b5 b4 b3 b2 b1 b0 =
     X  X  X  X  BE BK VI VO */
    u8 uAPSD;

    /* These parameters are for all the access categories */
    u32 srvInterval[WLAN_HAL_MAX_AC];   // Service Interval
    u32 susInterval[WLAN_HAL_MAX_AC];   // Suspend Interval
    u32 delayInterval[WLAN_HAL_MAX_AC]; // Delay Interval
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ADD_TS_RSP
 *--------------------------------------------------------------------------*/

struct add_rs_rsp_msg
{
    struct hal_msg_header header;

    /*success or failure */
    u32   status;

};


/*---------------------------------------------------------------------------
 * WLAN_HAL_DEL_TS_REQ
 *--------------------------------------------------------------------------*/

struct del_ts_req_msg
{
    struct hal_msg_header header;

    /* Station Index */
    u16 staIdx;

    /* TSPEC identifier uniquely identifying a TSPEC for a STA in a BSS */
    u16 tspecIdx;

    /* To lookup station id using the mac address */
    tSirMacAddr bssId; 
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_DEL_TS_RSP
 *--------------------------------------------------------------------------*/

struct del_ts_rsp_msg
{
    struct hal_msg_header header;

    /*success or failure */
    u32   status;

};

/* End of TSpec Parameters */

/* Start of BLOCK ACK related Parameters */

/*---------------------------------------------------------------------------
 * WLAN_HAL_ADD_BA_SESSION_REQ
 *--------------------------------------------------------------------------*/

struct add_ba_session_req_msg
{
    struct hal_msg_header header;

    /* Station Index */
    u16 staIdx;

    /* Peer MAC Address */
    tSirMacAddr peerMacAddr;

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

/*---------------------------------------------------------------------------
 * WLAN_HAL_ADD_BA_SESSION_RSP
 *--------------------------------------------------------------------------*/

struct add_ba_session_rsp_msg
{
    struct hal_msg_header header;

    /*success or failure */
    u32   status;

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

/*---------------------------------------------------------------------------
 * WLAN_HAL_ADD_BA_REQ
 *--------------------------------------------------------------------------*/

struct add_ba_req_msg
{
    struct hal_msg_header header;

    /* Session Id */
    u8 baSessionID;

    /* Reorder Window Size */
    u8 winSize;

#ifdef FEATURE_ON_CHIP_REORDERING
    u8 isReorderingDoneOnChip;
#endif
};


/*---------------------------------------------------------------------------
 * WLAN_HAL_ADD_BA_RSP
 *--------------------------------------------------------------------------*/

struct add_ba_rsp_msg
{
    struct hal_msg_header header;

    /*success or failure */
    u32   status;

    /* Dialog token */
    u8 baDialogToken;
};


/*---------------------------------------------------------------------------
 * WLAN_HAL_TRIGGER_BA_REQ
 *--------------------------------------------------------------------------*/


typedef struct sAddBaInfo
{
    u16 fBaEnable : 1;
    u16 startingSeqNum: 12;
    u16 reserved : 3;
}tAddBaInfo, *tpAddBaInfo;

typedef struct sTriggerBaRspCandidate
{
    tSirMacAddr staAddr;
    tAddBaInfo baInfo[STACFG_MAX_TC];
}tTriggerBaRspCandidate, *tpTriggerBaRspCandidate;

typedef struct sTriggerBaCandidate
{
    u8  staIdx;
    u8 tidBitmap;
}tTriggerBaReqCandidate, *tptTriggerBaReqCandidate;

struct trigger_ba_req_msg
{
   struct hal_msg_header header;

    /* Session Id */
    u8 baSessionID;

    /* baCandidateCnt is followed by trigger BA 
     * Candidate List(tTriggerBaCandidate)
     */
    u16 baCandidateCnt;

};


/*---------------------------------------------------------------------------
 * WLAN_HAL_TRIGGER_BA_RSP
 *--------------------------------------------------------------------------*/

struct trigger_ba_rsp_msg
{
    struct hal_msg_header header;

    /* TO SUPPORT BT-AMP */
    tSirMacAddr  bssId; 

    /* success or failure */
    u32   status;

    /* baCandidateCnt is followed by trigger BA 
     * Rsp Candidate List(tTriggerRspBaCandidate)
     */
    u16 baCandidateCnt;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_DEL_BA_REQ
 *--------------------------------------------------------------------------*/

struct del_ba_req_msg
{
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

/*---------------------------------------------------------------------------
 * WLAN_HAL_DEL_BA_RSP
 *--------------------------------------------------------------------------*/

struct del_ba_rsp_msg
{
    struct hal_msg_header header;

    /* success or failure */
    u32   status;

};

#ifdef FEATURE_WLAN_CCX

/*---------------------------------------------------------------------------
 * WLAN_HAL_TSM_STATS_REQ
 *--------------------------------------------------------------------------*/
struct tsm_stats_req_msg
{
    struct hal_msg_header header;

    /* Traffic Id */
    u8 tsmTID;

    tSirMacAddr bssId;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_TSM_STATS_RSP
 *--------------------------------------------------------------------------*/
struct tsm_stats_rsp_msg
{
    struct hal_msg_header header;

    /*success or failure */
    u32   status;

    /* Uplink Packet Queue delay */ 
    u16      UplinkPktQueueDly;

    /* Uplink Packet Queue delay histogram */ 
    u16      UplinkPktQueueDlyHist[4];

    /* Uplink Packet Transmit delay */ 
    u32      UplinkPktTxDly;

    /* Uplink Packet loss */ 
    u16      UplinkPktLoss;

    /* Uplink Packet count */ 
    u16      UplinkPktCount;

    /* Roaming count */ 
    u8       RoamingCount;

    /* Roaming Delay */ 
    u16      RoamingDly;

};

#endif

/*---------------------------------------------------------------------------
 * WLAN_HAL_SET_KEYDONE_MSG
 *--------------------------------------------------------------------------*/

struct set_key_done_msg
{
   struct hal_msg_header header;

  /*bssid of the keys */
  u8   bssidx;
  u8   encType;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_DOWNLOAD_NV_REQ
 *--------------------------------------------------------------------------*/
struct hal_nv_img_download_req_msg
{
    /* Note: The length specified in tHalNvImgDownloadReqMsg messages should be
     * header.msgLen = sizeof(tHalNvImgDownloadReqMsg) + nvImgBufferSize */
    struct hal_msg_header header;

    /* Fragment sequence number of the NV Image. Note that NV Image might not
     * fit into one message due to size limitation of the SMD channel FIFO. UMAC
     * can hence choose to chop the NV blob into multiple fragments starting with 
     * seqeunce number 0, 1, 2 etc. The last fragment MUST be indicated by 
     * marking the isLastFragment field to 1. Note that all the NV blobs would be
     * concatenated together by HAL without any padding bytes in between.*/
    u16 fragNumber;

    /* Is this the last fragment? When set to 1 it indicates that no more fragments
     * will be sent by UMAC and HAL can concatenate all the NV blobs rcvd & proceed 
     * with the parsing. HAL would generate a WLAN_HAL_DOWNLOAD_NV_RSP to the
     * WLAN_HAL_DOWNLOAD_NV_REQ after it receives each fragment */
    u16 isLastFragment;

    /* NV Image size (number of bytes) */
    u32 nvImgBufferSize;

    /* Following the 'nvImageBufferSize', there should be nvImageBufferSize
     * bytes of NV Image i.e. u8[nvImageBufferSize] */

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_DOWNLOAD_NV_RSP
 *--------------------------------------------------------------------------*/
struct hal_nv_img_download_rsp_msg
{
    struct hal_msg_header header;

    /* Success or Failure. HAL would generate a WLAN_HAL_DOWNLOAD_NV_RSP
     * after each fragment */
    u32   status;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_STORE_NV_IND
 *--------------------------------------------------------------------------*/
struct hal_nv_store_ind
{
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

/*---------------------------------------------------------------------------
 * WLAN_HAL_MIC_FAILURE_IND
 *--------------------------------------------------------------------------*/

#define SIR_CIPHER_SEQ_CTR_SIZE 6


/* Definition for MIC failure indication
   MAC reports this each time a MIC failure occures on Rx TKIP packet
 */
struct mic_failure_ind_msg
{
   struct hal_msg_header header;

    tSirMacAddr         bssId;   // BSSID

    tSirMacAddr  srcMacAddr;     //address used to compute MIC 
    tSirMacAddr  taMacAddr;      //transmitter address
    tSirMacAddr  dstMacAddr;
    u8      multicast;             
    u8      IV1;            // first byte of IV
    u8      keyId;          // second byte of IV
    u8      TSC[SIR_CIPHER_SEQ_CTR_SIZE]; // sequence number
    tSirMacAddr  rxMacAddr;      // receive address
};

struct update_vht_op_mode_req_msg
{
    struct hal_msg_header header;

   u16  opMode;
   u16  staId;

};

struct update_vht_op_mode_params_rsp_msg
{
    struct hal_msg_header header;

    u32   status;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_UPDATE_BEACON_REQ
 *--------------------------------------------------------------------------*/
struct update_beacon_req_msg
{
    struct hal_msg_header header;

    u8  bssIdx;

    //shortPreamble mode. HAL should update all the STA rates when it
    //receives this message
    u8 fShortPreamble;
    //short Slot time.
    u8 fShortSlotTime;
    //Beacon Interval
    u16 beaconInterval;
    //Protection related
    u8 llaCoexist;
    u8 llbCoexist;
    u8 llgCoexist;
    u8 ht20MhzCoexist;
    u8 llnNonGFCoexist;
    u8 fLsigTXOPProtectionFullSupport;
    u8 fRIFSMode;

    u16 paramChangeBitmap;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_UPDATE_BEACON_RSP
 *--------------------------------------------------------------------------*/
struct update_beacon_rsp_msg
{
    struct hal_msg_header header;
    u32   status;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_SEND_BEACON_REQ
 *--------------------------------------------------------------------------*/
struct send_beacon_req_msg
{
  struct hal_msg_header header;

    u32 beaconLength; //length of the template.
    u8 beacon[BEACON_TEMPLATE_SIZE];     // Beacon data.
    tSirMacAddr bssId;
    u32 timIeOffset; //TIM IE offset from the beginning of the template.
    u16 p2pIeOffset; //P2P IE offset from the begining of the template
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_SEND_BEACON_RSP
 *--------------------------------------------------------------------------*/
struct send_beacon_rsp_msg
{
    struct hal_msg_header header;
    u32   status;
};

#ifdef FEATURE_5GHZ_BAND

/*---------------------------------------------------------------------------
 * WLAN_HAL_ENABLE_RADAR_DETECT_REQ
 *--------------------------------------------------------------------------*/
struct enable_radar_req_msg
{
    struct hal_msg_header header;

    tSirMacAddr BSSID;
    u8   channel; 
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ENABLE_RADAR_DETECT_RSP
 *--------------------------------------------------------------------------*/

struct enable_radar_rsp_msg
{
    struct hal_msg_header header;

    /* Link Parameters */
    tSirMacAddr BSSID;
    /* success or failure */
    u32   status;
};

/*---------------------------------------------------------------------------
 *WLAN_HAL_RADAR_DETECT_INTR_IND
 *--------------------------------------------------------------------------*/

struct radar_detect_intr_ind_msg
{
    struct hal_msg_header header;

    u8 radarDetChannel;

};

/*---------------------------------------------------------------------------
 *WLAN_HAL_RADAR_DETECT_IND
 *-------------------------------------------------------------------------*/

struct radar_detect_ind_msg
{
   struct hal_msg_header header;

    /*channel number in which the RADAR detected*/
    u8          channelNumber;

    /*RADAR pulse width*/
    u16         radarPulseWidth; // in usecond

    /*Number of RADAR pulses */
    u16         numRadarPulse;
};


/*---------------------------------------------------------------------------
 *WLAN_HAL_GET_TPC_REPORT_REQ
 *-------------------------------------------------------------------------*/
struct sir_get_tpc_report_req_msg
{
   struct hal_msg_header header;

   tSirMacAddr sta;
   u8     dialogToken;
   u8     txpower;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_GET_TPC_REPORT_RSP
 *--------------------------------------------------------------------------*/

struct sir_get_tpc_report_rsp_msg
{
    struct hal_msg_header header;

    /* success or failure */
    u32   status;
};

#endif

/*---------------------------------------------------------------------------
 *WLAN_HAL_UPDATE_PROBE_RSP_TEMPLATE_REQ
 *-------------------------------------------------------------------------*/

struct send_probe_resp_req_msg
{
    struct hal_msg_header header;

    u8      pProbeRespTemplate[BEACON_TEMPLATE_SIZE];
    u32     probeRespTemplateLen;
    u32     ucProxyProbeReqValidIEBmap[8];
    tSirMacAddr  bssId;
};

/*---------------------------------------------------------------------------
 *WLAN_HAL_UPDATE_PROBE_RSP_TEMPLATE_RSP 
 *--------------------------------------------------------------------------*/

struct send_probe_resp_rsp_msg
{
    struct hal_msg_header header;

    /* success or failure */
    u32   status;
};


/*---------------------------------------------------------------------------
 *WLAN_HAL_UNKNOWN_ADDR2_FRAME_RX_IND 
 *--------------------------------------------------------------------------*/

struct send_unknown_frame_rx_ind_msg
{
    struct hal_msg_header header;

    /* success or failure */
    u32   status;
};

/*---------------------------------------------------------------------------
 *WLAN_HAL_DELETE_STA_CONTEXT_IND
 *--------------------------------------------------------------------------*/

struct delete_sta_context_ind_msg
{
    struct hal_msg_header header;

    u16    assocId;
    u16    staId;
    tSirMacAddr bssId; // TO SUPPORT BT-AMP
                       // HAL copies bssid from the sta table.
    tSirMacAddr addr2;        //
    u16    reasonCode;   // To unify the keepalive / unknown A2 / tim-based disa    
};

struct indicate_del_sta
{
   struct hal_msg_header header;
   u8  assocId;
   u8  staIdx;
   u8  bssIdx;
   u8  uReasonCode;
   u32  uStatus;
};

/*---------------------------------------------------------------------------
 *WLAN_HAL_SIGNAL_BTAMP_EVENT_REQ
 *--------------------------------------------------------------------------*/

struct bt_amp_event_msg
{
    struct hal_msg_header header;

    tBtAmpEventType btAmpEventType;

};

/*---------------------------------------------------------------------------
*WLAN_HAL_SIGNAL_BTAMP_EVENT_RSP
*--------------------------------------------------------------------------*/

struct bt_amp_event_rsp
{
    struct hal_msg_header header;

    /* success or failure */
    u32   status;
};

/*---------------------------------------------------------------------------
 *WLAN_HAL_TL_HAL_FLUSH_AC_REQ
 *--------------------------------------------------------------------------*/

struct tl_hal_flush_ac_req_msg
{
    struct hal_msg_header header;

   // Station Index. originates from HAL
    u8  ucSTAId;

    // TID for which the transmit queue is being flushed
    u8   ucTid;

};

/*---------------------------------------------------------------------------
*WLAN_HAL_TL_HAL_FLUSH_AC_RSP
*--------------------------------------------------------------------------*/

struct tl_hal_flush_ac_rsp_msg
{
    struct hal_msg_header header;

    // Station Index. originates from HAL
    u8  ucSTAId;

    // TID for which the transmit queue is being flushed
    u8   ucTid;

    /* success or failure */
    u32   status;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ENTER_IMPS_REQ
 *--------------------------------------------------------------------------*/
struct hal_enter_imps_req_msg
{
   struct hal_msg_header header;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_EXIT_IMPS_REQ
 *--------------------------------------------------------------------------*/
struct hal_exit_imps_req
{
   struct hal_msg_header header;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ENTER_BMPS_REQ
 *--------------------------------------------------------------------------*/

struct hal_enter_bmps_req_msg
{
   struct hal_msg_header header;

   u8         bssIdx;
   //TBTT value derived from the last beacon
#ifndef BUILD_QWPTTSTATIC
   u64 tbtt;
#endif
   u8 dtimCount;
   //DTIM period given to HAL during association may not be valid,
   //if association is based on ProbeRsp instead of beacon.
   u8 dtimPeriod;

   // For CCX and 11R Roaming
   u32 rssiFilterPeriod;
   u32 numBeaconPerRssiAverage;
   u8  bRssiFilterEnable;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_EXIT_BMPS_REQ
 *--------------------------------------------------------------------------*/
struct hal_exit_bmps_req_msg
{
   struct hal_msg_header header;

   u8     sendDataNull;
   u8     bssIdx;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_MISSED_BEACON_IND
 *--------------------------------------------------------------------------*/
struct hal_missed_beacon_ind_msg
{
   struct hal_msg_header header;

   u8     bssIdx;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ADD_BCN_FILTER_REQ
 *--------------------------------------------------------------------------*/
/* Beacon Filtering data structures */

/* The above structure would be followed by multiple of below mentioned structure */
struct beacon_filter_ie
{
    u8         elementId;
    u8         checkIePresence;
    u8     offset;
    u8     value;
    u8     bitMask;
    u8     ref;
};

struct hal_add_bcn_filter_req_msg
{
   struct hal_msg_header header;

    u16    capabilityInfo;
    u16    capabilityMask;
    u16    beaconInterval;
    u16    ieNum;
    u8     bssIdx;
    u8     reserved;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_REM_BCN_FILTER_REQ
 *--------------------------------------------------------------------------*/
struct hal_rem_bcn_filter_req
{
   struct hal_msg_header header;

    u8  ucIeCount;
    u8  ucRemIeId[1];
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_HOST_OFFLOAD_REQ
 *--------------------------------------------------------------------------*/
#define HAL_IPV4_ARP_REPLY_OFFLOAD                  0
#define HAL_IPV6_NEIGHBOR_DISCOVERY_OFFLOAD         1
#define HAL_IPV6_NS_OFFLOAD                         2
#define HAL_IPV6_ADDR_LEN                           16
#define HAL_MAC_ADDR_LEN                            6
#define HAL_OFFLOAD_DISABLE                         0
#define HAL_OFFLOAD_ENABLE                          1
#define HAL_OFFLOAD_BCAST_FILTER_ENABLE             0x2
#define HAL_OFFLOAD_ARP_AND_BCAST_FILTER_ENABLE     (HAL_OFFLOAD_ENABLE|HAL_OFFLOAD_BCAST_FILTER_ENABLE)

struct hal_ns_offload_params
{
   u8 srcIPv6Addr[HAL_IPV6_ADDR_LEN];
   u8 selfIPv6Addr[HAL_IPV6_ADDR_LEN];
   //Only support 2 possible Network Advertisement IPv6 address
   u8 targetIPv6Addr1[HAL_IPV6_ADDR_LEN];
   u8 targetIPv6Addr2[HAL_IPV6_ADDR_LEN];
   u8 selfMacAddr[HAL_MAC_ADDR_LEN];
   u8 srcIPv6AddrValid : 1;
   u8 targetIPv6Addr1Valid : 1;
   u8 targetIPv6Addr2Valid : 1;
   u8 reserved1 : 5;
   u8 reserved2;   //make it DWORD aligned
   u32 slotIndex; // slot index for this offload
   u8 bssIdx;
};

struct hal_host_offload_req
{
    u8 offloadType;
    u8 enableOrDisable;
    PACKED_PRE union
    {
        u8 hostIpv4Addr [4];
        u8 hostIpv6Addr [HAL_IPV6_ADDR_LEN];
    } params;
};

struct hal_host_offload_req_msg
{
   struct hal_msg_header header;
   struct hal_host_offload_req hostOffloadParams;
   struct hal_ns_offload_params nsOffloadParams;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_KEEP_ALIVE_REQ
 *--------------------------------------------------------------------------*/
/* Packet Types. */
#define HAL_KEEP_ALIVE_NULL_PKT              1
#define HAL_KEEP_ALIVE_UNSOLICIT_ARP_RSP     2

/* Enable or disable keep alive */
#define HAL_KEEP_ALIVE_DISABLE   0
#define HAL_KEEP_ALIVE_ENABLE    1

/* Keep Alive request. */
struct hal_keep_alive_req_msg
{
   struct hal_msg_header header;

    u8          packetType;
    u32         timePeriod;
    tHalIpv4Addr     hostIpv4Addr; 
    tHalIpv4Addr     destIpv4Addr;
    tSirMacAddr      destMacAddr;
    u8          bssIdx;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_SET_RSSI_THRESH_REQ
 *--------------------------------------------------------------------------*/
struct hal_rssi_threshold_req_msg
{
   struct hal_msg_header header;

    s8   ucRssiThreshold1     : 8;
    s8   ucRssiThreshold2     : 8;
    s8   ucRssiThreshold3     : 8;
    u8   bRssiThres1PosNotify : 1;
    u8   bRssiThres1NegNotify : 1;
    u8   bRssiThres2PosNotify : 1;
    u8   bRssiThres2NegNotify : 1;
    u8   bRssiThres3PosNotify : 1;
    u8   bRssiThres3NegNotify : 1;
    u8   bReserved10          : 2;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ENTER_UAPSD_REQ
 *--------------------------------------------------------------------------*/
struct hal_enter_uapsd_req_msg
{
   struct hal_msg_header header;

    u8     bkDeliveryEnabled:1;
    u8     beDeliveryEnabled:1;
    u8     viDeliveryEnabled:1;
    u8     voDeliveryEnabled:1;
    u8     bkTriggerEnabled:1;
    u8     beTriggerEnabled:1;
    u8     viTriggerEnabled:1;
    u8     voTriggerEnabled:1;
    u8     bssIdx;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_EXIT_UAPSD_REQ
 *--------------------------------------------------------------------------*/
struct hal_exit_uapsd_req_msg
{
   struct hal_msg_header header;
   u8       bssIdx;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ADD_WOWL_BCAST_PTRN
 *--------------------------------------------------------------------------*/
#define HAL_WOWL_BCAST_PATTERN_MAX_SIZE 128
#define HAL_WOWL_BCAST_MAX_NUM_PATTERNS 16

struct hal_wowl_add_bcast_ptrn_req_msg
{
   struct hal_msg_header header;

    u8  ucPatternId;           // Pattern ID
    // Pattern byte offset from beginning of the 802.11 packet to start of the
    // wake-up pattern
    u8  ucPatternByteOffset;   
    u8  ucPatternSize;         // Non-Zero Pattern size
    u8  ucPattern[HAL_WOWL_BCAST_PATTERN_MAX_SIZE]; // Pattern
    u8  ucPatternMaskSize;     // Non-zero pattern mask size
    u8  ucPatternMask[HAL_WOWL_BCAST_PATTERN_MAX_SIZE]; // Pattern mask
    u8  ucPatternExt[HAL_WOWL_BCAST_PATTERN_MAX_SIZE]; // Extra pattern
    u8  ucPatternMaskExt[HAL_WOWL_BCAST_PATTERN_MAX_SIZE]; // Extra pattern mask
    u8  bssIdx;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_DEL_WOWL_BCAST_PTRN
 *--------------------------------------------------------------------------*/
struct hal_wow_del_bcast_ptrn_req_msg
{
   struct hal_msg_header header;

    /* Pattern ID of the wakeup pattern to be deleted */
    u8  ucPatternId;
    u8  bssIdx;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ENTER_WOWL_REQ
 *--------------------------------------------------------------------------*/
struct hal_wowl_enter_req_msg
{
   struct hal_msg_header header;

    /* Enables/disables magic packet filtering */
    u8   ucMagicPktEnable; 

    /* Magic pattern */
    tSirMacAddr magicPtrn;

    /* Enables/disables packet pattern filtering in firmware. 
       Enabling this flag enables broadcast pattern matching 
       in Firmware. If unicast pattern matching is also desired,  
       ucUcastPatternFilteringEnable flag must be set tot true 
       as well 
    */
    u8   ucPatternFilteringEnable;

    /* Enables/disables unicast packet pattern filtering. 
       This flag specifies whether we want to do pattern match 
       on unicast packets as well and not just broadcast packets. 
       This flag has no effect if the ucPatternFilteringEnable 
       (main controlling flag) is set to false
    */
    u8   ucUcastPatternFilteringEnable;                     

    /* This configuration is valid only when magicPktEnable=1. 
     * It requests hardware to wake up when it receives the 
     * Channel Switch Action Frame.
     */
    u8   ucWowChnlSwitchRcv;

    /* This configuration is valid only when magicPktEnable=1. 
     * It requests hardware to wake up when it receives the 
     * Deauthentication Frame. 
     */
    u8   ucWowDeauthRcv;

    /* This configuration is valid only when magicPktEnable=1. 
     * It requests hardware to wake up when it receives the 
     * Disassociation Frame. 
     */
    u8   ucWowDisassocRcv;

    /* This configuration is valid only when magicPktEnable=1. 
     * It requests hardware to wake up when it has missed
     * consecutive beacons. This is a hardware register
     * configuration (NOT a firmware configuration). 
     */
    u8   ucWowMaxMissedBeacons;

    /* This configuration is valid only when magicPktEnable=1. 
     * This is a timeout value in units of microsec. It requests
     * hardware to unconditionally wake up after it has stayed
     * in WoWLAN mode for some time. Set 0 to disable this feature.      
     */
    u8   ucWowMaxSleepUsec;
    
    /* This configuration directs the WoW packet filtering to look for EAP-ID
     * requests embedded in EAPOL frames and use this as a wake source.
     */
    u8   ucWoWEAPIDRequestEnable;

    /* This configuration directs the WoW packet filtering to look for EAPOL-4WAY
     * requests and use this as a wake source.
     */
    u8   ucWoWEAPOL4WayEnable;

    /* This configuration allows a host wakeup on an network scan offload match.
     */
    u8   ucWowNetScanOffloadMatch;

    /* This configuration allows a host wakeup on any GTK rekeying error.
     */
    u8   ucWowGTKRekeyError;

    /* This configuration allows a host wakeup on BSS connection loss.
     */
    u8   ucWoWBSSConnLoss;

    u8   bssIdx;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_EXIT_WOWL_REQ
 *--------------------------------------------------------------------------*/

struct hal_wowl_exit_req_msg
{
   struct hal_msg_header     header;

    u8   bssIdx;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_GET_RSSI_REQ
 *--------------------------------------------------------------------------*/
struct hal_get_rssi_req_msg
{
   struct hal_msg_header header;
};


/*---------------------------------------------------------------------------
 * WLAN_HAL_GET_ROAM_RSSI_REQ
 *--------------------------------------------------------------------------*/
struct hal_get_roam_rssi_req_msg
{
   struct hal_msg_header header;

   /* Valid STA Idx for per STA stats request */
   u32    staId;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_SET_UAPSD_AC_PARAMS_REQ
 *--------------------------------------------------------------------------*/
struct hal_set_uapsd_ac_params_req_msg
{
   struct hal_msg_header header;

    u8  staidx;        // STA index
    u8  ac;            // Access Category
    u8  up;            // User Priority
    u32 srvInterval;   // Service Interval
    u32 susInterval;   // Suspend Interval
    u32 delayInterval; // Delay Interval
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_CONFIGURE_RXP_FILTER_REQ
 *--------------------------------------------------------------------------*/
struct hal_configure_rxp_filter_req_msg
{
   struct hal_msg_header header;

    u8 setMcstBcstFilterSetting;
    u8 setMcstBcstFilter;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ENTER_IMPS_RSP
 *--------------------------------------------------------------------------*/
struct hal_enter_imps_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_EXIT_IMPS_RSP
 *--------------------------------------------------------------------------*/
struct hal_exit_imps_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ENTER_BMPS_RSP
 *--------------------------------------------------------------------------*/
struct hal_enter_bmps_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;
    u8    bssIdx;
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_EXIT_BMPS_RSP
 *--------------------------------------------------------------------------*/
struct hal_exit_bmps_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;
    u8    bssIdx;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ENTER_UAPSD_RSP
 *--------------------------------------------------------------------------*/
struct hal_enter_uapsd_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32    status;
    u8     bssIdx;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_EXIT_UAPSD_RSP
 *--------------------------------------------------------------------------*/
struct hal_exit_uapsd_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;
    u8    bssIdx;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_RSSI_NOTIFICATION_IND
 *--------------------------------------------------------------------------*/
struct hal_rssi_notification_ind_msg
{
   struct hal_msg_header header;

    u32             bRssiThres1PosCross : 1;
    u32             bRssiThres1NegCross : 1;
    u32             bRssiThres2PosCross : 1;
    u32             bRssiThres2NegCross : 1;
    u32             bRssiThres3PosCross : 1;
    u32             bRssiThres3NegCross : 1;
    u32             avgRssi             : 8;
    u32             bReserved           : 18;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_GET_RSSI_RSP
 *--------------------------------------------------------------------------*/
struct hal_get_rssio_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;
    s8    rssi;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_GET_ROAM_RSSI_RSP
 *--------------------------------------------------------------------------*/
struct hal_get_roam_rssi_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;

    u8    staId;
    s8    rssi;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ENTER_WOWL_RSP
 *--------------------------------------------------------------------------*/
struct hal_wowl_enter_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
   u32   status;
   u8    bssIdx;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_EXIT_WOWL_RSP
 *--------------------------------------------------------------------------*/
struct hal_wowl_exit_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
   u32   status;
   u8    bssIdx;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ADD_BCN_FILTER_RSP
 *--------------------------------------------------------------------------*/
struct hal_add_bcn_filter_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_REM_BCN_FILTER_RSP
 *--------------------------------------------------------------------------*/
struct hal_rem_bcn_filter_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_ADD_WOWL_BCAST_PTRN_RSP
 *--------------------------------------------------------------------------*/
struct hal_add_wowl_bcast_ptrn_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
   u32   status;
   u8    bssIdx;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_DEL_WOWL_BCAST_PTRN_RSP
 *--------------------------------------------------------------------------*/
struct hal_del_wowl_bcast_ptrn_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
   u32   status;
   u8    bssIdx;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_HOST_OFFLOAD_RSP
 *--------------------------------------------------------------------------*/
struct hal_host_offload_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_KEEP_ALIVE_RSP
 *--------------------------------------------------------------------------*/
struct hal_keep_alive_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_SET_RSSI_THRESH_RSP
 *--------------------------------------------------------------------------*/
struct hal_set_rssi_thresh_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_SET_UAPSD_AC_PARAMS_RSP
 *--------------------------------------------------------------------------*/
struct hal_set_uapsd_ac_params_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_CONFIGURE_RXP_FILTER_RSP
 *--------------------------------------------------------------------------*/
struct hal_configure_rxp_filter_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;

};

/*---------------------------------------------------------------------------
 *WLAN_HAL_SET_MAX_TX_POWER_REQ
 *--------------------------------------------------------------------------*/

struct set_max_tx_pwr_req
{
    struct hal_msg_header header;

    tSirMacAddr bssId;  // BSSID is needed to identify which session issued this request. As
                        //the request has power constraints, this should be applied only to that session
    tSirMacAddr selfStaMacAddr;
    //In request,
    //power == MaxTx power to be used.
    u8  power;
};

/*---------------------------------------------------------------------------
*WLAN_HAL_SET_MAX_TX_POWER_RSP
*--------------------------------------------------------------------------*/

struct set_max_tx_pwr_rsp_msg
{
    struct hal_msg_header header;

    //power == tx power used for management frames.
    u8  power;

    /* success or failure */
    u32   status;
};

/*---------------------------------------------------------------------------
 *WLAN_HAL_SET_TX_POWER_REQ
 *--------------------------------------------------------------------------*/

struct set_tx_pwr_req_msg
{
    struct hal_msg_header header;

    /* TX Power in milli watts */
    u32  txPower;
    u8   bssIdx;
};

/*---------------------------------------------------------------------------
*WLAN_HAL_SET_TX_POWER_RSP
*--------------------------------------------------------------------------*/

struct set_tx_pwr_rsp_msg
{
    struct hal_msg_header header;

    /* success or failure */
    u32   status;
};

/*---------------------------------------------------------------------------
 *WLAN_HAL_GET_TX_POWER_REQ
 *--------------------------------------------------------------------------*/

struct get_tx_pwr_req_msg
{
    struct hal_msg_header header;

    u8  staId;
};

/*---------------------------------------------------------------------------
*WLAN_HAL_GET_TX_POWER_RSP
*--------------------------------------------------------------------------*/


struct get_tx_pwr_rsp_msg
{
    struct hal_msg_header header;

    /* success or failure */
    u32   status;

    /* TX Power in milli watts */
    u32   txPower;
};

#ifdef WLAN_FEATURE_P2P
/*---------------------------------------------------------------------------
 *WLAN_HAL_SET_P2P_GONOA_REQ
 *--------------------------------------------------------------------------*/

struct set_p2p_gonoa_req_msg
{
    struct hal_msg_header header;

  u8   opp_ps;
  u32  ctWindow;
  u8   count; 
  u32  duration;
  u32  interval;
  u32  single_noa_duration;
  u8   psSelection;
};

/*---------------------------------------------------------------------------
*WLAN_HAL_SET_P2P_GONOA_RSP
*--------------------------------------------------------------------------*/

struct set_p2p_gonoa_rsp_msg
{
    struct hal_msg_header header;

    /* success or failure */
    u32   status;

};

#endif

/*---------------------------------------------------------------------------
 *WLAN_HAL_ADD_SELF_STA_REQ
 *--------------------------------------------------------------------------*/

struct add_sta_self_req
{
    struct hal_msg_header header;

  tSirMacAddr selfMacAddr;
  u32    status;
};

/*---------------------------------------------------------------------------
*WLAN_HAL_ADD_SELF_STA_RSP
*--------------------------------------------------------------------------*/

struct add_sta_self_rsp_msg
{
    struct hal_msg_header header;

    /* success or failure */
    u32   status;

   /*Self STA Index */
   u8    selfStaIdx;

   /* DPU Index (IGTK, PTK, GTK all same) */
   u8 dpuIdx;

   /* DPU Signature */
   u8 dpuSignature;
};


/*---------------------------------------------------------------------------
  WLAN_HAL_DEL_STA_SELF_REQ
---------------------------------------------------------------------------*/

struct del_sta_self_req_msg
{
   struct hal_msg_header header;

   tSirMacAddr selfMacAddr;

};


/*---------------------------------------------------------------------------
  WLAN_HAL_DEL_STA_SELF_RSP
---------------------------------------------------------------------------*/

struct del_sta_self_rsp_msg
{
   struct hal_msg_header header;

  /*success or failure */
  u32   status;

  tSirMacAddr selfMacAddr;

};

#ifdef WLAN_FEATURE_VOWIFI_11R

/*---------------------------------------------------------------------------
 *WLAN_HAL_AGGR_ADD_TS_REQ
 *--------------------------------------------------------------------------*/

struct
{
    /* Station Index */
    u16 staIdx;

    /* TSPEC handler uniquely identifying a TSPEC for a STA in a BSS */
    /* This will carry the bitmap with the bit positions representing different AC.s*/
    u16 tspecIdx;

    /*  Tspec info per AC To program TPE with required parameters */
    tSirMacTspecIE   tspec[WLAN_HAL_MAX_AC];

    /* U-APSD Flags: 1b per AC.  Encoded as follows:
     b7 b6 b5 b4 b3 b2 b1 b0 =
     X  X  X  X  BE BK VI VO */
    u8 uAPSD;

    /* These parameters are for all the access categories */
    u32 srvInterval[WLAN_HAL_MAX_AC];   // Service Interval
    u32 susInterval[WLAN_HAL_MAX_AC];   // Suspend Interval
    u32 delayInterval[WLAN_HAL_MAX_AC]; // Delay Interval

}tAggrAddTsParams, *tpAggrAddTsParams;


struct
{
    struct hal_msg_header header;
    tAggrAddTsParams aggrAddTsParam;
}tAggrAddTsReq, *tpAggrAddTsReq;

/*---------------------------------------------------------------------------
*WLAN_HAL_AGGR_ADD_TS_RSP
*--------------------------------------------------------------------------*/

struct
{
    /* success or failure */
    u32   status0;
    /* FIXME PRIMA for future use for 11R */
    u32   status1;
}tAggrAddTsRspParams, *tpAggrAddTsRspParams;

struct
{
    struct hal_msg_header header;
    tAggrAddTsRspParams aggrAddTsRspParam;
}tAggrAddTsRspMsg, *tpAggrAddTsRspMsg;

#endif

/*---------------------------------------------------------------------------
 * WLAN_HAL_CONFIGURE_APPS_CPU_WAKEUP_STATE_REQ
 *--------------------------------------------------------------------------*/
struct hal_configure_apps_cpu_wakeup_state_req_msg
{
   struct hal_msg_header header;

   u8   isAppsCpuAwake;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_CONFIGURE_APPS_CPU_WAKEUP_STATE_RSP
 *--------------------------------------------------------------------------*/
struct hal_configure_apps_cpu_wakeup_state_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_DUMP_COMMAND_REQ
 *--------------------------------------------------------------------------*/

struct hal_dump_cmd_req_msg
{
   struct hal_msg_header          header;

   u32    argument1;
   u32    argument2;
   u32    argument3;
   u32    argument4;
   u32    argument5;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_DUMP_COMMAND_RSP
 *--------------------------------------------------------------------------*/

struct hal_dump_cmd_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;
    /*Length of the responce message*/
    u32   rspLength;
    /*FiXME: Currently considering the  the responce will be less than 100bytes */
    u8    rspBuffer[DUMPCMD_RSP_BUFFER];

};

/*---------------------------------------------------------------------------
 *WLAN_HAL_COEX_IND
 *-------------------------------------------------------------------------*/
#define WLAN_COEX_IND_DATA_SIZE (4)
#define WLAN_COEX_IND_TYPE_DISABLE_HB_MONITOR (0)
#define WLAN_COEX_IND_TYPE_ENABLE_HB_MONITOR (1)

struct coex_ind_msg
{
   struct hal_msg_header   header;

    /*Coex Indication Type*/
    u32   coexIndType;

    /*Coex Indication Data*/
    u32   coexIndData[WLAN_COEX_IND_DATA_SIZE];
};

/*---------------------------------------------------------------------------
 *WLAN_HAL_OTA_TX_COMPL_IND
 *-------------------------------------------------------------------------*/

struct tx_compl_ind_msg
{
   struct hal_msg_header   header;

   /*Tx Complete Indication Success or Failure*/
   u32   status;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_HOST_SUSPEND_IND
 *-------------------------------------------------------------------------*/

struct hal_wlan_host_suspend_ind_msg
{
   struct hal_msg_header header;

    u32 configuredMcstBcstFilterSetting;
    u32 activeSessionCount; 
};

/*---------------------------------------------------------------------------
 * WLAN_HAL_EXCLUDE_UNENCRYTED_IND
 *-------------------------------------------------------------------------*/

struct hal_wlan_exclude_unencrpted_ind_msg
{
   struct hal_msg_header header;

    u8 bDot11ExcludeUnencrypted;
    tSirMacAddr bssId; 
};

#ifdef WLAN_FEATURE_P2P
/*---------------------------------------------------------------------------
 *WLAN_HAL_NOA_ATTR_IND
 *-------------------------------------------------------------------------*/

struct
{
   u8      index ;
   u8      oppPsFlag ;
   u16     ctWin  ;

   u16      uNoa1IntervalCnt;
   u16      bssIdx;
   u32      uNoa1Duration;
   u32      uNoa1Interval;
   u32      uNoa1StartTime;

   u16      uNoa2IntervalCnt;
   u16      rsvd2;
   u32      uNoa2Duration;
   u32      uNoa2Interval;
   u32      uNoa2StartTime;

   u32   status;
}tNoaAttrIndParams, *tpNoaAttrIndParams;

struct
{
   struct hal_msg_header      header;
   tNoaAttrIndParams  noaAttrIndParams; 
}tNoaAttrIndMsg, *tpNoaAttrIndMsg;

/*---------------------------------------------------------------------------
 *WLAN_HAL_NOA_START_IND
 *-------------------------------------------------------------------------*/

struct
{
    u32             status;
    u32             bssIdx;
}tNoaStartIndParams, *tpNoaStartIndParams;

struct
{
   struct hal_msg_header      header;
   tNoaStartIndParams noaStartIndParams;
}tNoaStartIndMsg, tpNoaStartIndMsg;
#endif

/*---------------------------------------------------------------------------
 * WLAN_HAL_HOST_RESUME_REQ
 *-------------------------------------------------------------------------*/

struct hal_wlan_host_resume_req_msg
{
   struct hal_msg_header header;

    u8 configuredMcstBcstFilterSetting;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_HOST_RESUME_RSP
 *--------------------------------------------------------------------------*/
struct hal_host_resume_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;

};

struct hal_del_ba_ind_msg
{
    struct hal_msg_header   header;

	u16 staIdx;
	// Peer MAC Address, whose BA session has timed out
	tSirMacAddr peerMacAddr;
	// TID for which a BA session timeout is being triggered
	u8 baTID;
	// DELBA direction
	// 1 - Originator
	// 0 - Recipient
	u8 baDirection;
	u32 reasonCode;
	tSirMacAddr  bssId; // TO SUPPORT BT-AMP

};

/*---------------------------------------------------------------------------
 *PNO Messages
 *-------------------------------------------------------------------------*/
/*Max number of channels that a network can be found on*/
#define WLAN_HAL_PNO_MAX_NETW_CHANNELS  26

/*Max number of channels that a network can be found on*/
#define WLAN_HAL_PNO_MAX_NETW_CHANNELS_EX  60

/*Maximum numbers of networks supported by PNO*/
#define WLAN_HAL_PNO_MAX_SUPP_NETWORKS  16

/*The number of scan time intervals that can be programmed into PNO*/
#define WLAN_HAL_PNO_MAX_SCAN_TIMERS    10

/*Maximum size of the probe template*/
#define WLAN_HAL_PNO_MAX_PROBE_SIZE     450

/*Type of PNO enabling 
  Immediate - scanning will start immediately and PNO procedure will
  be repeated based on timer
  Suspend - scanning will start at suspend
  Resume - scanning will start on system resume*/
typedef enum
{
   ePNO_MODE_IMMEDIATE,
   ePNO_MODE_ON_SUSPEND,
   ePNO_MODE_ON_RESUME,
   ePNO_MODE_MAX = WLAN_HAL_MAX_ENUM_SIZE
} ePNOMode;

/*Authentication type*/
typedef enum 
{
    eAUTH_TYPE_ANY                   = 0,    
    eAUTH_TYPE_OPEN_SYSTEM           = 1,
    
    // Upper layer authentication types
    eAUTH_TYPE_WPA                   = 2,
    eAUTH_TYPE_WPA_PSK               = 3,
    
    eAUTH_TYPE_RSN                   = 4,
    eAUTH_TYPE_RSN_PSK               = 5,
    eAUTH_TYPE_FT_RSN                = 6,
    eAUTH_TYPE_FT_RSN_PSK            = 7,
    eAUTH_TYPE_WAPI_WAI_CERTIFICATE  = 8,
    eAUTH_TYPE_WAPI_WAI_PSK          = 9,
    
    eAUTH_TYPE_MAX = WLAN_HAL_MAX_ENUM_SIZE

}tAuthType;

/* Encryption type */
typedef enum eEdType
{
    eED_ANY           = 0,
    eED_NONE          = 1,
    eED_WEP           = 2,
    eED_TKIP          = 3,
    eED_CCMP          = 4,
    eED_WPI           = 5,
        
    eED_TYPE_MAX = WLAN_HAL_MAX_ENUM_SIZE
} tEdType;

/* SSID broadcast  type */
typedef enum eSSIDBcastType
{
  eBCAST_UNKNOWN      = 0,
  eBCAST_NORMAL       = 1,
  eBCAST_HIDDEN       = 2,

  eBCAST_TYPE_MAX     = WLAN_HAL_MAX_ENUM_SIZE
} tSSIDBcastType;

/* 
  The network description for which PNO will have to look for
*/
struct network_type
{
  /*SSID of the BSS*/
  tSirMacSSid ssId;

  /*Authentication type for the network*/
  tAuthType   authentication; 

  /*Encryption type for the network*/
  tEdType     encryption; 

  /*Indicate the channel on which the Network can be found 
    0 - if all channels */
  u8     ucChannelCount;
  u8     aChannels[WLAN_HAL_PNO_MAX_NETW_CHANNELS];

  /*Indicates the RSSI threshold for the network to be considered*/
  u8     rssiThreshold;
}; 

struct scan_timer
{
  /*How much it should wait */
  u32    uTimerValue; 

  /*How many times it should repeat that wait value 
    0 - keep using this timer until PNO is disabled*/
  u32    uTimerRepeat; 

  /*e.g:   2 3 
           4 0 
    - it will wait 2s between consecutive scans for 3 times
    - after that it will wait 4s between consecutive scans until disabled*/
}; 

/* 
  The network parameters to be sent to the PNO algorithm
*/
struct scan_timers_type
{
  /*set to 0 if you wish for PNO to use its default telescopic timer*/
  u8     ucScanTimersCount; 

  /*A set value represents the amount of time that PNO will wait between 
    two consecutive scan procedures
    If the desired is for a uniform timer that fires always at the exact same
    interval - one single value is to be set
    If there is a desire for a more complex - telescopic like timer multiple
    values can be set - once PNO reaches the end of the array it will
    continue scanning at intervals presented by the last value*/
  struct scan_timer  aTimerValues[WLAN_HAL_PNO_MAX_SCAN_TIMERS]; 

};

/*
  Preferred network list request 
*/
struct set_pref_netw_list_req
{
   struct hal_msg_header header;


    /*Enable PNO*/
    u32          enable;

    /*Immediate,  On Suspend,   On Resume*/
    ePNOMode         modePNO;
    
    /*Number of networks sent for PNO*/
    u32          ucNetworksCount;

    /*The networks that PNO needs to look for*/
    struct network_type     aNetworks[WLAN_HAL_PNO_MAX_SUPP_NETWORKS];

    /*The scan timers required for PNO*/
    struct scan_timers_type  scanTimers; 

    /*Probe template for 2.4GHz band*/
    u16         us24GProbeSize; 
    u8          a24GProbeTemplate[WLAN_HAL_PNO_MAX_PROBE_SIZE];

    /*Probe template for 5GHz band*/
    u16         us5GProbeSize; 
    u8          a5GProbeTemplate[WLAN_HAL_PNO_MAX_PROBE_SIZE];
};


/* 
  The network description for which PNO will have to look for
*/
struct network_type_new
{
  /*SSID of the BSS*/
  tSirMacSSid ssId;

  /*Authentication type for the network*/
  tAuthType   authentication; 

  /*Encryption type for the network*/
  tEdType     encryption; 

  /*SSID broadcast type, normal, hidden or unknown*/
  tSSIDBcastType bcastNetworkType;

  /*Indicate the channel on which the Network can be found 
    0 - if all channels */
  u8     ucChannelCount;
  u8     aChannels[WLAN_HAL_PNO_MAX_NETW_CHANNELS];

  /*Indicates the RSSI threshold for the network to be considered*/
  u8     rssiThreshold;
}; 

/*
  Preferred network list request new
*/
struct set_pref_netw_list_req_new
{
   struct hal_msg_header header;


    /*Enable PNO*/
    u32          enable;

    /*Immediate,  On Suspend,   On Resume*/
    ePNOMode         modePNO;
    
    /*Number of networks sent for PNO*/
    u32         ucNetworksCount; 

    /*The networks that PNO needs to look for*/
    struct network_type_new  aNetworks[WLAN_HAL_PNO_MAX_SUPP_NETWORKS];

    /*The scan timers required for PNO*/
    struct scan_timers_type  scanTimers; 

    /*Probe template for 2.4GHz band*/
    u16         us24GProbeSize; 
    u8          a24GProbeTemplate[WLAN_HAL_PNO_MAX_PROBE_SIZE];

    /*Probe template for 5GHz band*/
    u16         us5GProbeSize; 
    u8          a5GProbeTemplate[WLAN_HAL_PNO_MAX_PROBE_SIZE];


};

/*
  Preferred network list response 
*/
struct set_pref_netw_list_resp
{
   struct hal_msg_header header;

   /*status of the request - just to indicate that PNO has acknowledged 
     the request and will start scanning*/
   u32   status;
};

/*
  Preferred network found indication
*/
struct pref_netw_found_ind {

   struct hal_msg_header header;

  /*Network that was found with the highest RSSI*/
  tSirMacSSid ssId;
  
  /*Indicates the RSSI */
  u8     rssi;
};

/*
  RSSI Filter request 
*/
struct set_rssi_filter_req
{
   struct hal_msg_header header;

  /*RSSI Threshold*/
  u8          ucRssiThreshold;

};

/*
 Set RSSI filter resp
*/
struct set_rssi_filter_resp {
   struct hal_msg_header header;
   /*status of the request */
   u32   status;
};

/*
  Update scan params - sent from host to PNO
  to be used during PNO scanning 
*/
struct update_scan_params_req {

   struct hal_msg_header header;

  /*Host setting for 11d*/
  u8   b11dEnabled; 

  /*Lets PNO know that host has determined the regulatory domain*/
  u8   b11dResolved;

  /*Channels on which PNO is allowed to scan*/
  u8   ucChannelCount; 
  u8   aChannels[WLAN_HAL_PNO_MAX_NETW_CHANNELS]; 

  /*Minimum channel time*/
  u16  usActiveMinChTime; 

  /*Maximum channel time*/
  u16  usActiveMaxChTime; 

  /*Minimum channel time*/
  u16  usPassiveMinChTime; 

  /*Maximum channel time*/
  u16  usPassiveMaxChTime; 

  /*Cb State*/
  ePhyChanBondState cbState;
};

/*
  Update scan params - sent from host to PNO
  to be used during PNO scanning 
*/
struct update_scan_params_req_ex{

   struct hal_msg_header header;


  /*Host setting for 11d*/
  u8   b11dEnabled; 

  /*Lets PNO know that host has determined the regulatory domain*/
  u8   b11dResolved;

  /*Channels on which PNO is allowed to scan*/
  u8   ucChannelCount; 
  u8   aChannels[WLAN_HAL_PNO_MAX_NETW_CHANNELS_EX]; 

  /*Minimum channel time*/
  u16  usActiveMinChTime; 

  /*Maximum channel time*/
  u16  usActiveMaxChTime; 

  /*Minimum channel time*/
  u16  usPassiveMinChTime; 

  /*Maximum channel time*/
  u16  usPassiveMaxChTime; 

  /*Cb State*/
  ePhyChanBondState cbState;

};

/*
  Update scan params - sent from host to PNO
  to be used during PNO scanning 
*/
struct update_scan_params_resp {

   struct hal_msg_header header;

   /*status of the request */
   u32   status;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_SET_TX_PER_TRACKING_REQ
 *--------------------------------------------------------------------------*/
struct hal_set_tx_per_tracking_req_msg
{
   struct hal_msg_header header;

    u8  ucTxPerTrackingEnable;           /* 0: disable, 1:enable */
    u8  ucTxPerTrackingPeriod;           /* Check period, unit is sec. */
    u8  ucTxPerTrackingRatio;            /* (Fail TX packet)/(Total TX packet) ratio, the unit is 10%. */
    u32 uTxPerTrackingWatermark;         /* A watermark of check number, once the tx packet exceed this number, we do the check, default is 5 */

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_SET_TX_PER_TRACKING_RSP
 *--------------------------------------------------------------------------*/
struct hal_set_tx_per_tracking_rsp_msg
{
   struct hal_msg_header header;

    /* success or failure */
    u32   status;

};

/*---------------------------------------------------------------------------
 * WLAN_HAL_TX_PER_HIT_IND
 *--------------------------------------------------------------------------*/
struct tx_per_hit_ind_msg
{
   struct hal_msg_header   header;
};

/*---------------------------------------------------------------------------
 *******************Packet Filtering Definitions Begin*******************
 *--------------------------------------------------------------------------*/
#define    HAL_PROTOCOL_DATA_LEN                  8
#define    HAL_MAX_NUM_MULTICAST_ADDRESS        240
#define    HAL_MAX_NUM_FILTERS                   20
#define    HAL_MAX_CMP_PER_FILTER                10

typedef enum
{
  HAL_RCV_FILTER_TYPE_INVALID,
  HAL_RCV_FILTER_TYPE_FILTER_PKT,
  HAL_RCV_FILTER_TYPE_BUFFER_PKT,
  HAL_RCV_FILTER_TYPE_MAX_ENUM_SIZE
}tHalReceivePacketFilterType;

typedef enum 
{
  HAL_FILTER_PROTO_TYPE_INVALID,
  HAL_FILTER_PROTO_TYPE_MAC,
  HAL_FILTER_PROTO_TYPE_ARP,
  HAL_FILTER_PROTO_TYPE_IPV4,
  HAL_FILTER_PROTO_TYPE_IPV6,
  HAL_FILTER_PROTO_TYPE_UDP,
  HAL_FILTER_PROTO_TYPE_MAX
}tHalRcvPktFltProtocolType;

typedef enum 
{
  HAL_FILTER_CMP_TYPE_INVALID,
  HAL_FILTER_CMP_TYPE_EQUAL,
  HAL_FILTER_CMP_TYPE_MASK_EQUAL,
  HAL_FILTER_CMP_TYPE_NOT_EQUAL,
  HAL_FILTER_CMP_TYPE_MAX
}tHalRcvPktFltCmpFlagType;

struct hal_rcv_pkt_filter_params
{
    u8                          protocolLayer;
    u8                          cmpFlag;
    u16                         dataLength; /* Length of the data to compare */
    u8                          dataOffset; /* from start of the respective frame header */
    u8                          reserved; /* Reserved field */
    u8                          compareData[HAL_PROTOCOL_DATA_LEN];  /* Data to compare */
    u8                          dataMask[HAL_PROTOCOL_DATA_LEN];   /* Mask to be applied on the received packet data before compare */
};

struct hal_sessionized_rcv_pkt_filter_cfg_type
{
    u8                         filterId;
    u8                         filterType;
    u8                         numParams; 
    u32                        coleasceTime; 
    u8                         bssIdx;
    struct hal_rcv_pkt_filter_params          paramsData[1];
};

struct hal_set_rcv_pkt_filter_req_msg
{
  struct hal_msg_header                 header;

    u8                         filterId;
    u8                         filterType;
    u8                         numParams; 
    u32                        coalesceTime;
    struct hal_rcv_pkt_filter_params          paramsData[1];

};

struct hal_rcv_flt_mc_addr_list_type
{
    u8         dataOffset; /* from start of the respective frame header */
    u32       cMulticastAddr;
    tSirMacAddr    multicastAddr[HAL_MAX_NUM_MULTICAST_ADDRESS];
    u8        bssIdx;
};

struct hal_set_pkt_filter_rsp_msg
{
   struct hal_msg_header               header;

    /* success or failure */
    u32   status;
    u8    bssIdx;
};

struct hal_rcv_flt_pkt_match_cnt_req_msg
{
   struct hal_msg_header        header;

   u8    bssIdx;


};


struct hal_rcv_flt_pkt_match_cnt
{
   u8    filterId;
   u32   matchCnt;
};

struct hal_rcv_flt_pkt_match_cnt_rsp_msg
{
   struct hal_msg_header  header;

   /* Success or Failure */
   u32                 status;
   u32                 matchCnt;   
   struct hal_rcv_flt_pkt_match_cnt    filterMatchCnt[HAL_MAX_NUM_FILTERS]; 
   u8                  bssIdx;
};

struct hal_rcv_flt_pkt_clear_param
{
    u32   status;  /* only valid for response message */
    u8    filterId;
    u8    bssIdx;
};

struct hal_rcv_flt_pkt_clear_req_msg
{
    struct hal_msg_header               header;
     struct hal_rcv_flt_pkt_clear_param     filterClearParam;
};

struct hal_rcv_flt_pkt_clear_rsp_msg
{
   struct hal_msg_header              header;
   struct hal_rcv_flt_pkt_clear_param    filterClearParam;
};

struct hal_rcv_flt_pkt_set_mc_list_req_msg
{
    struct hal_msg_header                    header;
    struct hal_rcv_flt_mc_addr_list_type         mcAddrList;
};

struct hal_rcv_flt_pkt_set_mc_list_rsp_msg
{
   struct hal_msg_header                    header;
    u32   status;  
    u8    bssIdx;
};


/*---------------------------------------------------------------------------
 *******************Packet Filtering Definitions End*******************
 *--------------------------------------------------------------------------*/

struct set_power_params_req_msg
{
    struct hal_msg_header               header;


   /*  Ignore DTIM */
  u32 uIgnoreDTIM;

  /*DTIM Period*/
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

   /*status of the request */
   u32   status;

};

/*---------------------------------------------------------------------------
 ****************Capability bitmap exchange definitions and macros starts*************
 *--------------------------------------------------------------------------*/

typedef enum {
    MCC        = 0,
    P2P        = 1,
    DOT11AC    = 2,
    SLM_SESSIONIZATION = 3,
    DOT11AC_OPMODE     = 4,
    SAP32STA = 5,
    TDLS       = 6,
    P2P_GO_NOA_DECOUPLE_INIT_SCAN = 7,
    WLANACTIVE_OFFLOAD = 8,
    BEACON_OFFLOAD     = 9,
    SCAN_OFFLOAD       = 10,
    ROAM_OFFLOAD       = 11,
    BCN_MISS_OFFLOAD   = 12,
    STA_POWERSAVE      = 13,
    STA_ADVANCED_PWRSAVE = 14,
    AP_UAPSD           = 15,
    AP_DFS             = 16,
    BLOCKACK           = 17,
    PHY_ERR            = 18,
    BCN_FILTER         = 19,
    RTT                = 20,
    RATECTRL           = 21,
    WOW                = 22,
    MAX_FEATURE_SUPPORTED = 128,
} placeHolderInCapBitmap;

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

/*---------------------------------------------------------------------------
 * WLAN_HAL_WAKE_REASON_IND
 *--------------------------------------------------------------------------*/

/* status codes to help debug rekey failures */
typedef enum
{
    WLAN_HAL_GTK_REKEY_STATUS_SUCCESS            = 0,
    WLAN_HAL_GTK_REKEY_STATUS_NOT_HANDLED        = 1, /* rekey detected, but not handled */
    WLAN_HAL_GTK_REKEY_STATUS_MIC_ERROR          = 2, /* MIC check error on M1 */
    WLAN_HAL_GTK_REKEY_STATUS_DECRYPT_ERROR      = 3, /* decryption error on M1  */
    WLAN_HAL_GTK_REKEY_STATUS_REPLAY_ERROR       = 4, /* M1 replay detected */
    WLAN_HAL_GTK_REKEY_STATUS_MISSING_KDE        = 5, /* missing GTK key descriptor in M1 */
    WLAN_HAL_GTK_REKEY_STATUS_MISSING_IGTK_KDE   = 6, /* missing iGTK key descriptor in M1 */
    WLAN_HAL_GTK_REKEY_STATUS_INSTALL_ERROR      = 7, /* key installation error */
    WLAN_HAL_GTK_REKEY_STATUS_IGTK_INSTALL_ERROR = 8, /* iGTK key installation error */
    WLAN_HAL_GTK_REKEY_STATUS_RESP_TX_ERROR      = 9, /* GTK rekey M2 response TX error */

    WLAN_HAL_GTK_REKEY_STATUS_GEN_ERROR          = 255 /* non-specific general error */
} tGTKRekeyStatus;

/* wake reason types */
typedef enum
{
    WLAN_HAL_WAKE_REASON_NONE             = 0,
    WLAN_HAL_WAKE_REASON_MAGIC_PACKET     = 1,  /* magic packet match */
    WLAN_HAL_WAKE_REASON_PATTERN_MATCH    = 2,  /* host defined pattern match */
    WLAN_HAL_WAKE_REASON_EAPID_PACKET     = 3,  /* EAP-ID frame detected */
    WLAN_HAL_WAKE_REASON_EAPOL4WAY_PACKET = 4,  /* start of EAPOL 4-way handshake detected */
    WLAN_HAL_WAKE_REASON_NETSCAN_OFFL_MATCH = 5, /* network scan offload match */
    WLAN_HAL_WAKE_REASON_GTK_REKEY_STATUS = 6,  /* GTK rekey status wakeup (see status) */
    WLAN_HAL_WAKE_REASON_BSS_CONN_LOST    = 7,  /* BSS connection lost */
} tWakeReasonType;

/*
  Wake Packet which is saved at tWakeReasonParams.DataStart
  This data is sent for any wake reasons that involve a packet-based wakeup :

  WLAN_HAL_WAKE_REASON_TYPE_MAGIC_PACKET
  WLAN_HAL_WAKE_REASON_TYPE_PATTERN_MATCH
  WLAN_HAL_WAKE_REASON_TYPE_EAPID_PACKET
  WLAN_HAL_WAKE_REASON_TYPE_EAPOL4WAY_PACKET
  WLAN_HAL_WAKE_REASON_TYPE_GTK_REKEY_STATUS

  The information is provided to the host for auditing and debug purposes

*/

/*
  Wake reason indication
*/
struct hal_wake_reason_ind
{
    struct hal_msg_header       header;

    u32  ulReason;        /* see tWakeReasonType */
    u32  ulReasonArg;     /* argument specific to the reason type */
    u32  ulStoredDataLen; /* length of optional data stored in this message, in case
                              HAL truncates the data (i.e. data packets) this length
                              will be less than the actual length */
    u32  ulActualDataLen; /* actual length of data */
    u8   aDataStart[1];   /* variable length start of data (length == storedDataLen)
                             see specific wake type */

    u32            uBssIdx : 8;
    u32            bReserved : 24;
};

/*---------------------------------------------------------------------------
* WLAN_HAL_GTK_OFFLOAD_REQ
*--------------------------------------------------------------------------*/

#define HAL_GTK_KEK_BYTES 16
#define HAL_GTK_KCK_BYTES 16

#define WLAN_HAL_GTK_OFFLOAD_FLAGS_DISABLE (1 << 0)

#define GTK_SET_BSS_KEY_TAG  0x1234AA55

struct hal_gtk_offload_req_msg
{
   struct hal_msg_header header;

  u32     ulFlags;             /* optional flags */
  u8      aKCK[HAL_GTK_KCK_BYTES];  /* Key confirmation key */ 
  u8      aKEK[HAL_GTK_KEK_BYTES];  /* key encryption key */
  u64     ullKeyReplayCounter; /* replay counter */
  u8      bssIdx;

};

/*---------------------------------------------------------------------------
* WLAN_HAL_GTK_OFFLOAD_RSP
*--------------------------------------------------------------------------*/
struct hal_gtk_offload_rsp_msg
{
   struct hal_msg_header header;

    u32   ulStatus;   /* success or failure */
    u8    bssIdx;
};


/*---------------------------------------------------------------------------
* WLAN_HAL_GTK_OFFLOAD_GETINFO_REQ
*--------------------------------------------------------------------------*/
struct hal_gtk_offload_get_info_req_msg
{
   struct hal_msg_header header;
   u8    bssIdx;
};

/*---------------------------------------------------------------------------
* WLAN_HAL_GTK_OFFLOAD_GETINFO_RSP
*--------------------------------------------------------------------------*/
struct hal_gtk_offload_get_info_rsp_msg
{
   struct hal_msg_header header;

   u32   ulStatus;             /* success or failure */
   u32   ulLastRekeyStatus;    /* last rekey status when the rekey was offloaded */
   u64   ullKeyReplayCounter;  /* current replay counter value */
   u32   ulTotalRekeyCount;    /* total rekey attempts */
   u32   ulGTKRekeyCount;      /* successful GTK rekeys */
   u32   ulIGTKRekeyCount;     /* successful iGTK rekeys */
   u8    bssIdx;

};

/*---------------------------------------------------------------------------
* WLAN_HAL_DHCP_IND
*--------------------------------------------------------------------------*/
struct dhcp_info
{
   /*Indicates the device mode which indicates about the DHCP activity */
    u8 device_mode;
    tSirMacAddr macAddr;
};

struct dhcp_ind_status
{
   struct hal_msg_header  header;
   u32       status;  /* success or failure */
};

/*
   Thermal Mitigation mode of operation.
   HAL_THERMAL_MITIGATION_MODE_0 - Based on AMPDU disabling aggregation
   HAL_THERMAL_MITIGATION_MODE_1 - Based on AMPDU disabling aggregation and
   reducing transmit power
   HAL_THERMAL_MITIGATION_MODE_2 - Not supported
*/
typedef enum
{
  HAL_THERMAL_MITIGATION_MODE_INVALID = -1,
  HAL_THERMAL_MITIGATION_MODE_0,
  HAL_THERMAL_MITIGATION_MODE_1,
  HAL_THERMAL_MITIGATION_MODE_2,
  HAL_THERMAL_MITIGATION_MODE_MAX = WLAN_HAL_MAX_ENUM_SIZE,
}tHalThermalMitigationModeType;
//typedef tANI_S16 tHalThermalMitigationModeType;

/*
   Thermal Mitigation level.
   Note the levels are incremental i.e HAL_THERMAL_MITIGATION_LEVEL_2 =
   HAL_THERMAL_MITIGATION_LEVEL_0 + HAL_THERMAL_MITIGATION_LEVEL_1

   HAL_THERMAL_MITIGATION_LEVEL_0 - lowest level of thermal mitigation. This
   level indicates normal mode of operation
   HAL_THERMAL_MITIGATION_LEVEL_1 - 1st level of thermal mitigation
   HAL_THERMAL_MITIGATION_LEVEL_2 - 2nd level of thermal mitigation
   HAL_THERMAL_MITIGATION_LEVEL_3 - 3rd level of thermal mitigation
   HAL_THERMAL_MITIGATION_LEVEL_4 - 4th level of thermal mitigation
*/
typedef enum 
{
  HAL_THERMAL_MITIGATION_LEVEL_INVALID = -1,
  HAL_THERMAL_MITIGATION_LEVEL_0,
  HAL_THERMAL_MITIGATION_LEVEL_1,
  HAL_THERMAL_MITIGATION_LEVEL_2,
  HAL_THERMAL_MITIGATION_LEVEL_3,
  HAL_THERMAL_MITIGATION_LEVEL_4,
  HAL_THERMAL_MITIGATION_LEVEL_MAX = WLAN_HAL_MAX_ENUM_SIZE,
}tHalThermalMitigationLevelType;
//typedef tANI_S16 tHalThermalMitigationLevelType;

/* WLAN_HAL_SET_THERMAL_MITIGATION_REQ */
struct set_thermal_mitigation_req_msg
{
    struct hal_msg_header               header;

   /* Thermal Mitigation Operation Mode */
   tHalThermalMitigationModeType thermalMitMode;

   /* Thermal Mitigation Level */
   tHalThermalMitigationLevelType thermalMitLevel;

};

struct set_thermal_mitigation_resp {

   struct hal_msg_header header;

   /*status of the request */
   u32   status;

};

/* Per STA Class B Statistics. Class B statistics are STA TX/RX stats  
provided to FW from Host via periodic messages */
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

#if defined(__ANI_COMPILER_PRAGMA_PACK_STACK)
#pragma pack(pop)
#elif defined(__ANI_COMPILER_PRAGMA_PACK)
#else
#endif

#endif /* _HAL_H_ */

