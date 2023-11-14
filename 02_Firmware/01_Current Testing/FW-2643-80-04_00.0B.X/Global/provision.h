#ifndef __CENTERO_PROVISION_H_
#define __CENTERO_PROVISION_H_


#include "typedef.h"
#include "../MS/MS_Data.h"
#include "../WirelessHART_AppLayer/App_WH_Tables.h"
#include "../WiredHart/wired_hart_link_layer.h"
#include "../Lib/WirelessHART_Lib.h"
#include "../Platform/nvm.h"

// TO BE UPDATED EVERY BUILD
#define CENTERO_MAJOR            1 ///< Major Firmware Version
#define CENTERO_MINOR            0 ///< Minor Firmware Version
#define CENTERO_REVISION_NUMBER  0 ///< Revision Firmware Number
//
#define CENTERO_REVISION        (CENTERO_REVISION_NUMBER & 0x7F) ///< Revision Firmware Version
#define CENTERO_BUILD_LETTER    'd' ///< Firmware Version Build Letter

#define NO_OF_BYTES_FOR_CRC     62 ///< Number of bytes used to calculate the CRC number for each memory page.
#define MAX_BURST_MSG_NO        3  ///< Maximum number of bursts messages.

#define MAX_EVENTS_NO           1  ///< Maximum number of events.
#define MAX_EVENTS_PENDING      5  ///< Maximum number of pending events.

#define MAX_BW_SERVICES_NO      (MAX_BURST_MSG_NO + MAX_EVENTS_NO) ///< Maximum bandwidth service allocation.

/** Preprocessor directive used for determining if the event queue is full. */
#define EVENT_QUEUE_IS_FULL     ( (g_pEventQueueEnd < g_stEventQueue + MAX_EVENTS_PENDING) ? 0 : 1 )
/** Preprocessor directive used for determining if the event queue has at least one element. */
#define EVENT_QUEUE_NOT_EMPTY   ( (g_pEventQueueEnd > g_stEventQueue) ? 1 : 0 )

#define EVENT_SERVICE_CHANNEL   MAX_BURST_MSG_NO ///< Event service channel definition.

///@{
/** Mandatory Device Variables Indexes */
enum {
    PV_IDX = 0,
    SV_IDX = 1,
    TV_IDX = 2,
    QV_IDX = 3
};
///@}


#define CST_DEV_VARS_NO	        6  ///< Custom Device Variables Number
#define STD_DEV_VARS_NO	        7  ///< Standard Device Variables Number
#define MAX_DEV_VARS_NO	        (CST_DEV_VARS_NO + STD_DEV_VARS_NO)  ///< Total Number of Device Variables = 6 (custom) + 7 (standardized, see table 34 within HCF_SPEC-183)

#define MAX_CONFIGURABLE_DYN_VAR_NO     4 /**< Dynamic Variables Number ... i.e. [PV, SV, TV & QV] */



#define DEVICE_VARIABLE_0       0  ///< DEVICE_VARIABLE_0 stores Material thickness raw.
#define DEVICE_VARIABLE_1       1  ///< DEVICE_VARIABLE_1 stores Material thickness tc
#define DEVICE_VARIABLE_2       2  ///< DEVICE_VARIABLE_2 stores Material temperature
#define DEVICE_VARIABLE_3       3  ///< DEVICE_VARIABLE_3 stores Percentage battery life
#define DEVICE_VARIABLE_4       4  ///< DEVICE_VARIABLE_4 stores DSI_temperature.
#define DEVICE_VARIABLE_5       5  ///< DEVICE_VARIABLE_5 stores Battery remaining days.
#define DEVICE_VARIABLE_6       6  ///< DEVICE_VARIABLE_5 stores Measurement Interval Hours.

#define PROV_unManufacturerIDCode               (0x610CUL) ///< Manufacturer ID Code is Centero.
#define PROV_umPrivateLabelDistributorCode      (0x610CUL) ///< Private Label Distributor Code is Centero.
#define PROV_ucDeviceRevisionLevel              (0x01)     ///< Device revision.
#define PROV_ucHardwareRevisionLevel            (0x00)     ///< Hardware revision.
#define PROV_ucMaxNoOfDeviceVars                (0x06)     ///< Maximum number of device variables.
#define PROV_ucDeviceProfile                    (DeviceProfileCodes_WIRELESSHART_DISCRETE_DEVICE) ///< Device Profile.
#define PROV_ucPowerSource                      (DevicePowerSource_BatteryPower) ///< Device Power Source.
#define PROV_unExpandedDeviceType               (0xE586UL) ///< Expanded Device Type.

#define PV_LOWER_LIMIT  0.0254f   
#define DEFAULT_PV_UPPER_LIMIT  25.4f
#define MAX_PV_UPPER_LIMIT  254.0f

/**
 *  Device Variable Structure
 */
typedef struct
{
    DeviceVariableStatus  m_ucStatus;/*!< Device variable status */
    uint8                 m_ucCode;/*!< Device variable code */
    uint32                m_ulTransducerSN;/*!< Device variable transducer serial number*/
    uint8                 m_ucUnit;/*!< Device variable measurement unit */
    fp32                  m_fUpperLimit;/*!< Device variable upper limit value */
    fp32                  m_fLowerLimit;/*!< Device variable lower limit value */
    fp32                  m_fDampingValue;/*!< Device variable damping value */
    fp32                  m_fMinSpan;/*!< Device variable minimum span */
    uint8                 m_ucClassification;/*!< Device variable classification code */
    uint8                 m_ucFamily;/*!< Device variable family code */
    uint32                m_ulUpdateTimePeriod;/*!< Device variable update time period */
} NV_DEVICE_VARIABLE_T;


/**
 * Device Variable Value Structure
 */
typedef union {
  fp32 m_fValue;
  uint32 m_ulValue;
  struct {
    uint16 m_unValueH;
    uint16 m_unValueL;
  };
  uint8 type;
} DEVICE_VARIABLE_VALUE_T;

// Device Variable Value and Operation Mode
typedef struct
{
  DEVICE_VARIABLE_VALUE_T m_stValue;
  DEVICE_VARIABLE_VALUE_T m_stLastValue;
  // see Common Practice Commands, Cmd79 Write Device Variable
  uint8 m_ucNormalOperation; // =0 -> normal mode; != 0 -> force the engineering value to a certain value                            
} DEVICE_VARIABLE_T;


/**
 * Burst Data Structure
 */
typedef struct
{
  uint8     m_ucBurstModeControlCode;     ///< Burst Mode Control (see CommonTable 9).
  uint8     m_aucDeviceVariableCodes[8];  ///< The published variables(Cmd9 or Cmd33).
  uint8     m_ucBurstTriggerModeCode;     ///< Trigger mode (see CommonTable 33).
  uint8     m_ucClassification;           ///< Device variable classification for trigger value.
  uint8     m_ucUnit;                     ///< Units code.
  uint8     m_ucCmdType;                  ///< Command type (wired or wireless).
  uint16    m_unCmdNo;                    ///< The published command number.
  uint32    m_ulUpdateTime;               ///< Normal update interval.
  uint32    m_ulMaxUpdateTime;            ///< Maximum update interval.
  fp32      m_fTriggerValue;              ///< Trigger value.
}
NV_BURST_T;

/**
 * Event Data Structure
 */
typedef struct
{
  uint8     m_ucEventControlCode;   ///< Event notification control code(see Common Table 35).
  uint8     m_ucMaskSize;           ///< Maximum mask size(25).
  uint8     m_ucDeviceStatusMask;   ///< Device Status mask.
  uint8     m_aucEventMasks[25];    ///< Events mask.
  uint32    m_ulRetryTime;          ///< Retry time.
  uint32    m_ulMaxUpdateTime;      ///< Maximum update time.
  uint32    m_ulDebounceInterval;   ///< Event debounce interval.
}
NV_EVENT_T;

/**
 * Non-volatile storage data signature
 */
typedef union
{
    uint8  m_Bytes[4];
    uint16 m_16[2];
    uint32 m_32;
} SIGNATURE_T;

/**
 * Pending Event Data Structure
 */
typedef struct
{
    TIME       m_ulTimestamp;         ///< Time within the day [1/32 ms]
    uint16     m_ucCCCounter;         ///< Configuration changed counter
    uint8      m_ucEventNo;           ///< Event number
    uint8      m_ucEventStatus;       ///< Event status(not shifted !)
    uint8      m_ucAckReceived;       ///< 0xFF -> not triggered, !=0xFF -> triggered
    uint8      m_ucCCRequested;       ///< 0x55 = CC requested ..waiting for response ... 0xAA = response valid
    uint8      m_ucDeviceStatus;      ///< Device status byte
    uint8      m_aucEvents[25];       ///< Command48 data.
} EVENT_PENDING_T;

typedef struct {
    uint8 m_ucOffset;  // offset of a certain NVM Variable within its structure
    uint8 m_ucSize; // size of a certain NVM Variable
}NVM_TABLE_T;

typedef struct{
    SIGNATURE_T signature;
    T_WIRED_HART_GLOBALS m_sWiredHart;
    NV_BURST_T m_stBurstNVM[MAX_BURST_MSG_NO];
    NV_EVENT_T m_stEventNVM;
    NV_DEVICE_VARIABLE_T m_astDeviceVars[MAX_DEV_VARS_NO];
    DEVICE_VARIABLE_T m_astDeviceVarsValues[MAX_DEV_VARS_NO];
    uint8 m_aucHartDynVarsAssignCodes[MAX_CONFIGURABLE_DYN_VAR_NO];
    RadioCharge m_stRadioCharge;
    MSSystemCharge m_stMSSystemCharge;
    TotalRadioCharge m_stTotalRadioCharge;
}PROVISION_T;

extern PROVISION_T g_stProv;

void NVM_ReadAllData();
void NVM_LoadDefaultData();
Boolean SignatureAreaGood(void);

extern const NV_DEVICE_VARIABLE_T g_astNVDevVarInitData[MAX_DEV_VARS_NO];   ///< Structure used for initialization of the Device Variables data.
extern EVENT_PENDING_T  g_stEventQueue[MAX_EVENTS_PENDING]; ///< Pending events queue.
extern EVENT_PENDING_T* g_pEventQueueEnd; /// Pointer to the last element of the pending event queue.

#endif // __CENTERO_PROVISION_H_