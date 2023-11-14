#ifndef __CENTERO_APP_WH_TYPES_H_
#define __CENTERO_APP_WH_TYPES_H_

#include "../Global/typedef.h"
#include "../Global/provision.h"
#include "App_WH_Tables.h"

#define ENG_MODE_NONE       0
#define ENG_MODE_TX_MOD     1
#define ENG_MODE_TX_UNMOD   2
#define ENG_MODE_RX         3
#define ENG_MODE_TX_PACKET  4
#define ENG_MODE_RX_PACKET  5

#define MAX_DEVICE_VARIABLES            8

/** Command Response Codes
 */
typedef enum {
    RCS_N00_Success = 0, /*!< 0 */
    RCS_E01_Undefined1 = 1, /*!< 1 */
    RCS_E02_InvalidSelection = 2, /*!< 2 */
    RCS_E03_PassedParameterTooLarge = 3, /*!< 3 */
    RCS_E04_PassedParameterTooSmall = 4, /*!< 4 */
    RCS_E05_TooFewDataBytesReceived = 5, /*!< 5 */
    RCS_E06_DeviceSpecificCommandError = 6, /*!< 6 */
    RCS_E07_InWriteProtectMode = 7, /*!< 7 */
    RCM_W08_UpdateFailure = 8, /*!< 8 */
    RCM_W08_UpdateTimesAdjusted = 8, /*!< 8 */
    RCM_W08_UpdatePeriodIncreased = 8, /*!< 8 */
    RCM_W08_SetToNearestPossibleValue = 8, /*!< 8 */
    RCM_W08_BurstConditionConflict = 8, /*!< 8 */
    RCM_W08_UpdatePeriodOrDebounceIntervalAdjusted = 8, /*!< 8 */
    RCM_W08_NotAllEventsCleared = 8, /*!< 8 */
    RCM_W08_UpdateInProgress = 8, /*!< 8 */
    RCM_E09_LowerRangeValueTooHigh = 9, /*!< 9 */
    RCM_E09_InvalidBurstMessage = 9, /*!< 9 */
    RCM_E09_InsufficientBandwidth = 9, /*!< 9 */
    RCM_E09_InvalidCommandRequested = 9, /*!< 9 */
    RCM_E09_BadTriggerAction = 9, /*!< 9 */
    RCM_E10_LowerRangeValueTooLow = 10, /*!< 10 */
    RCM_E10_InvalidWriteDeviceVariableCode = 10, /*!< 10 */
    RCM_E10_BadCommandData = 10, /*!< 10 */
    RCM_E11_UpperRangeValueTooHigh = 11, /*!< 11 */
    RCM_E11_InvalidDeviceVariableCode = 11, /*!< 11 */
    RCM_E11_InvalidDeviceVariableClassification = 11, /*!< 11 */
    RCM_E12_UpperRangeValueTooLow = 12, /*!< 12 */
    RCM_E12_InvalidUnitsCode = 12, /*!< 12 */
    RCM_E12_InvalidModeSelection = 12, /*!< 12 */
    RCM_E12_InvalidDeviceVariable = 12, /*!< 12 */
    RCM_E12_CommandNumberNotSupported = 12, /*!< 12 */ // !!! C113
    RCM_E13_InvalidTransferFunctionCode = 13, /*!< 13 */
    RCM_E13_UpperAndLowerRangeValuesOutOfLimits = 13, /*!< 13 */
    RCM_E13_ComputationError = 13, /*!< 13 */
    RCM_E13_InvalidBurstTriggerModeSelectionCode = 13, /*!< 13 */
    RCM_W14_DynamicVarsReturnedForDeviceVars = 14, /*!< 14 */
    RCM_W14_UpdateRateUncertain = 14, /*!< 14 */
    RCM_W14_NewLowerRangeValuePushedUpperRangeOver = 14, /*!< 14 */
    RCM_W14_CommBandwidthLessThanRequested = 14, /*!< 14 */
    RCM_W14_StatusBytesMismatch = 14, /*!< 14 */
    RCM_E15_InvalidAnalogChannelCodeNumber = 15, /*!< 15 */
    RCS_E16_AccessRestricted = 16, /*!< 16 */
    RCS_E17_InvalidDeviceVariableIndex = 17, /*!< 17 */
    RCS_E18_InvalidUnitsCode = 18, /*!< 18 */
    RCS_E19_DeviceVariableIndexNotAllowed = 19, /*!< 19 */
    RCS_E20_InvalidExtendedCommandNumber = 20, /*!< 20 */
    RCM_E28_InvalidRangeUnitsCode = 28, /*!< 28 */
    RCM_W30_CommandResponseTruncated = 30, /*!< 30 */
    RCS_E32_Busy = 32, /*!< 32 */
    RCS_E33_DelayedResponseInitiated = 33, /*!< 33 */
    RCS_E34_DelayedResponseRunning = 34, /*!< 34 */
    RCS_E35_DelayedResponseDead = 35, /*!< 35 */
    RCS_E36_DelayedResponseConflict = 36, /*!< 36 */
    RCS_E60_PayloadTooLong = 60, /*!< 60 */
    RCS_E64_CommandNotImplemented = 64, /*!< 64 */
    RCM_E65_ServiceRequestDenied = 65, /*!< 65 */
    RCS_N254_ResponseAck = 254, /*!< 254 */
    RCS_N255_ResponseNack = 255, /*!< 255 */
} __attribute__((packed)) CommandResponseCodes;

/** Application Processor local implemented commands codes
 */
enum {
    CMDID_C001_ReadPV = 1, /*!< 1 */
    CMDID_C002_ReadLoopCurrAndPercentRange = 2, /*!< 2 */
    CMDID_C003_ReadDynamicVarsAndLoopCurrent = 3, /*!< 3 */
    CMDID_C006_WritePollingAddress = 6, /*!< 6 */
    CMDID_C007_ReadLoopConfiguration = 7, /*!< 7 */
    CMDID_C008_ReadDynamicVarsClassif = 8, /*!< 8 */
    CMDID_C009_ReadDeviceVarsWithStatus = 9, /*!< 9 */
    CMDID_C014_ReadPrimaryVarTransducerInfo = 14, /*!< 14 */
    CMDID_C015_ReadDeviceInfo = 15, /*!< 15 */
    CMDID_C033_ReadDeviceVars = 33, /*!< 33 */
    CMDID_C035_WritePVRangeValues = 35, /*!< 35 */
    CMDID_C044_WritePVUnits = 44, /*!< 44 */
    CMDID_C048_ReadAdditionalDeviceStatus = 48, /*!< 48 */
    CMDID_C050_ReadDynamicVariableAssignments = 50, /*!< 50 */
    CMDID_C051_WriteDynamicVariableAssignments = 51, /*!< 51 */
    CMDID_C054_ReadDeviceVariableInformation = 54, /*!< 54 */
    CMDID_C059_WriteNumberOfResponsePreambles = 59, /*!< 59 */
    CMDID_C079_WriteDeviceVariable_Req = 79, /*!< 79 */
    CMDID_C103_WriteBurstPeriod = 103, /*!< 103 */
    CMDID_C104_WriteBurstTrigger = 104, /*!< 104 */
    CMDID_C105_ReadBurstModeConfiguration = 105, /*!< 105 */
    CMDID_C107_WriteBurstDeviceVars = 107, /*!< 107 */
    CMDID_C108_WriteBurstModeCommandNo = 108, /*!< 108 */
    CMDID_C109_BurstModeCtrl = 109, /*!< 109 */
    CMDID_C115_ReadEventNotificationSummary = 115, /*!< 115 */
    CMDID_C116_WriteEventNotificationBitMask = 116, /*!< 116 */
    CMDID_C117_WriteEventNotificationTiming = 117, /*!< 117 */
    CMDID_C118_EventNotificationControl = 118, /*!< 118 */
    CMDID_C119_AcknowledgeEventNotification = 119, /*!< 119 */
    CMDID_C150_ReadAppFwVersion = 150, /*!< 150 */
    CMDID_C151_RFEngineeringMode = 151, /*!< 151 */
    CMDID_C152_EventsManagement = 152, /*!< 152 */
    CMDID_C180_MakeMeasurement = 180, /*!< 180 */
    CMDID_C181_WriteUTMParameters = 181, /*!< 181 */
    CMDID_C182_ReadUTMParameters = 182, /*!< 182 */
    CMDID_C183_WriteUTGParameters = 183, /*!< 183 */
    CMDID_C184_ReadUTGParameters = 184, /*!< 184 */
    CMDID_C185_WriteUTTParameters = 185, /*!< 185 */
    CMDID_C186_ReadUTTParameters = 186, /*!< 186 */
    CMDID_C188_ResetBatteryLife = 188, /*!< 188 */
    CMDID_C195_AccessPassword = 195, /*!< 195 */
    CMDID_C196_WritePassword = 196, /*!< 196 */
    CMDID_C197_DisablePassword = 197, /*!< 197 */
    CMDID_C198_WriteAlarmMode = 198, /*!< 198 */
    CMDID_C201_WriteMSSerialNumber = 201, /*!< 201 */
    CMDID_C202_ReadMSSerialNumber = 202, /*!< 202 */
    CMDID_C203_WriteCommissionDate = 203, /*!< 203 */
    CMDID_C204_ReadCommissionDate = 204, /*!< 204*/
    CMDID_C205_WriteMeasurementTimeInterval = 205, /*!< 205 */
    CMDID_C206_ReadMeasurementTimeInterval = 206, /*!< 206 */
    CMDID_C210_SetFactoryDefaults = 210, /*!< 210 */
    CMDID_C778_ReadBatteryLife = 778 /*!< 778 */
};

/** Application Processor local implemented commands request sizes
 */
enum {
    C001_ReqSize = 0,
    C002_ReqSize = 0,
    C003_ReqSize = 0,
    C006_ReqSize = 1, // 1, 2, ... or more comp
    C007_ReqSize = 0,
    C008_ReqSize = 0,
    C009_ReqSize = 1, // 1, 2, 3, ... up to 8 (variable size)
    C014_ReqSize = 0,
    C015_ReqSize = 0,
    C033_ReqSize = 1, // 1,2,3,4, ... or more fwd comp
    C035_ReqSize = 9,
    C044_ReqSize = 1,
    C048_ReqSize = 0, // 0, ... 25
    C050_ReqSize = 0,
    C051_ReqSize = 1, // 1,2,3 or 4
    C054_ReqSize = 1,
    C059_ReqSize = 1, // 1, ... or more comp
    C079_ReqSize = 8,
    C103_ReqSize = 9,
    C104_ReqSize = 8,
    C105_ReqSize = 0,
    C107_ReqSize = 1,
    C108_ReqSize = 1, // 1 byte burst message
    C109_ReqSize = 1, // Only m_burstModeControlCode myght be present
    C115_ReqSize = 1,
    C116_ReqSize = 1, // up to 28 ... can be truncated
    C117_ReqSize = 13,
    C118_ReqSize = 2,
    C119_ReqSize = 1, // up to 33 ... can be truncated
    C150_ReqSize = 0,
    C151_ReqSize = 1, // up to 12
    C152_ReqSize = 4, //
    C180_ReqSize = 0,
    C181_ReqSize = 16,
    C182_ReqSize = 0,
    C183_ReqSize = 26,
    C184_ReqSize = 0,
    C185_ReqSize = 12,
    C186_ReqSize = 0,
    C188_ReqSize = 0,
//    C190_ReqSize = 0,
//    C191_ReqSize = 42,
//    C192_ReqSize = 0,
//    C193_ReqSize = 12,
//    C194_ReqSize = 0,
    C195_ReqSize = 8,
    C196_ReqSize = 8,
    C197_ReqSize = 1,
    C198_ReqSize = 1,   // Not used To be removed
    C201_ReqSize = 14,
    C202_ReqSize = 0,
    C203_ReqSize = 0,
    C204_ReqSize = 3,
    C205_ReqSize = 2,
    C206_ReqSize = 0,
    C210_ReqSize = 0,
    C778_ReqSize = 0
};


#define C006_RespSize 2
#define C007_RespSize 2
#define C008_RespSize 4 // response size for command 8 - read dynamic variable classification
#define C014_RespSize 16 // response size for command 14 - read primary variable transducer information
#define C015_RespSize 18 // response size for command 15 - read device information
#define C035_RespSize 9  // response size for command 35 - write primary variable range values
#define C044_RespSize 1  // response size for command 44 - write primary variable units
#define C054_RespSize 28 // response size for command 54 - read device variable information
#define C059_RespSize 1
#define C079_RespSize 8
#define C105_RespSize 29 // response size for command 105 is 27 bytes
#define C107_RespSize 9
#define C115_RespSize 19 // up to 45 ... can be truncated
#define C116_RespSize 1  // up to 28 ... ??? can be truncated ???
#define C117_RespSize 13
#define C118_RespSize 2
#define C119_RespSize 9  // up to 33 ... can be truncated
#define C150_RespSize 25
#define C152_RespSize 4
#define C180_RespSize ((uint8)16)
#define C181_RespSize ((uint8)16) 
#define C182_RespSize ((uint8)16)
#define C183_RespSize ((uint8)26)
#define C184_RespSize ((uint8)26)
#define C185_RespSize ((uint8)12)
#define C186_RespSize ((uint8)12)
//#define C190_RespSize ((uint8)16)
//#define C191_RespSize ((uint8)42) 
//#define C192_RespSize ((uint8)42)
//#define C193_RespSize ((uint8)12)
//#define C194_RespSize ((uint8)12)
#define C195_RespSize ((uint8)8)
#define C196_RespSize ((uint8)8)
#define C197_RespSize ((uint8)1)
#define C198_RespSize ((uint8)2)
#define C201_RespSize ((uint8)14)
#define C202_RespSize ((uint8)14)
#define C203_RespSize ((uint8)3)
#define C204_RespSize ((uint8)3)
#define C205_RespSize ((uint8)2)
#define C206_RespSize ((uint8)2)
#define C205_RespSize ((uint8)2)
#define C778_RespSize ((uint8)2)


#define C048_SIZE_09   9
#define C048_SIZE_14  14
#define C048_SIZE_25  25


// Max request size
#define C006_MAX_REQ_SIZE 2
#define C009_MAX_REQ_SIZE 8
#define C033_MAX_REQ_SIZE 4
#define C059_MAX_REQ_SIZE 1
#define C192_MAX_REQ_SIZE 5
#define C193_MAX_REQ_SIZE 3

/**
 * Not Used Value data
 */
typedef union {
    fp32 m_fValue;
    uint8 m_ucValue[4];
} NOT_USED_VALUE;

/**
 * Empty Command Request Data Structure
 */
typedef struct {
    uint8 unusedByte;
} EmptyCommand_Req;

/**
 * The Time consists of a unsigned 32-bit binary integer with the least significant bit representing 1/32 of a millisecond (i.e., 0.03125 milliseconds).
 */
typedef union {
    uint32 u32;

    struct {
        uint32 scalemiliseconds : 5; //represents 1/32 of a millisecond
        uint32 miliseconds : 27;
    } tm;
} Time_t;

/**
 * Command 9 Device Variable Slot data definition.
 */
typedef struct {
    uint8 m_ucCode;
    DeviceVariableClassificationCodes m_ucClassification;
    UnitsCodes m_ucUnit;
    fp32 m_fValue;
    DeviceVariableStatus m_ucStatus;
} C009_Slot_t;

/**
 * Command 33 Device Variable Slot data definition.
 */
typedef struct {
    uint8 m_ucCode;
    UnitsCodes m_ucUnit;
    fp32 m_fValue;
} C033_Slot_t;

/**
 * Maxmimum Number of Read Evemts.
 */
enum {
    MaxNoOfReadEvents = 25
};

/**
 * Maxmimum Number of Read Evemts.
 */
enum {
    MaxNoOfWriteEvents = 25
};

/**
 * Command 48 maximum data length.
 */
enum {
    MaxLengthOf48Command = 25
};



/**
 * C001 Request data structure - Read Primary Variable
 */
typedef EmptyCommand_Req C001_ReadPrimaryVariable_Req;

/**
 * C001 Response data structure - Read Primary Variable
 */
typedef struct {
    UnitsCodes m_ucPrimaryVarUnit; /**< Primary Variable Units */
    float m_fPrimaryVar; /**< Primary Variable */
    //#ifdef PUBLISH_MONOTONIC
    //    uint32      m_unMonotonic;
    //#endif
} C001_ReadPrimaryVariable_Resp;


/**
 * C002 Request data structure - Read Loop Current and Percent Of Range
 */
typedef EmptyCommand_Req C002_ReadLoopCurrentAndPercentOfRange_Req;

/**
 * C002 Response data structure - Read Loop Current and Percent Of Range
 */
typedef struct {
    fp32 m_fPrimaryVariableLoopCurrent; /**< Primary Variable Loop Current */
    fp32 m_fPrimaryVariablePercentOfRange; /**< Primary Variable Percent Of Range */
} C002_ReadLoopCurrentAndPercentOfRange_Resp;


/**
 * C003 Request data structure - Read Dynamic Variables And Loop Current
 */
typedef EmptyCommand_Req C003_ReadDynamicVariablesAndLoopCurrent_Req;

/**
 * C003 Response data structure - Read Dynamic Variables And Loop Current
 */
typedef struct {
    fp32 m_fLoopCurrent; /**< Primary Variable Loop Current */
    UnitsCodes m_ucPvUnitsCode; /**< Primary Variable Units Code */
    fp32  m_PvValue; /**< Primary Variable */
    UnitsCodes m_ucSvUnitsCode; /**< Secondary Variable Units Code */
    fp32  m_SvValue; /**< Secondary Variable */
    UnitsCodes m_ucTvUnitsCode; /**< Tertiary Variable Units Code */
    fp32  m_TvValue; /**<Tertiary Variable */
    UnitsCodes m_ucQvUnitsCode; /**< Quaternary Variable Units Code */
    fp32  m_QvValue; //**< Quaternary Variable */
} C003_ReadDynamicVariablesAndLoopCurrent_Resp;

/**
 * C006 Request data structure - Write Polling Address
 */
typedef struct {
    uint8 m_ucPollingAddress; /**< Polling Address of Device */
    uint8 m_ucLoopCurrentMode; /**< Loop Current Mode */
} C006_WritePollingAddress_Req;

/**
 * C006 Response data structure - Write Polling Address
 */
typedef struct {
    uint8 m_ucPollingAddress; /**< Polling Address of Device */
    uint8 m_ucLoopCurrentMode; /**< Loop Current Mode */
} C006_WritePollingAddress_Resp;


/**
 * C007 Request Data Structure - Read Loop Configuration
 */
typedef EmptyCommand_Req C007_ReadLoopConfiguration_Req;

/**
 * C007 Response Data Structure - Read Loop Configuration
 */
typedef struct {
    uint8 m_ucPollingAddress; /**< Polling Address of Device */
    uint8 m_ucLoopCurrentMode; /**< Loop Current Mode */
} C007_ReadLoopConfiguration_Resp;


/**
 * C008 Request Data Structure - Read Dynamic Variable Classification
 */
typedef EmptyCommand_Req C008_ReadDynamicVariableClassification_Req;

/**
 * C008 Response Data Structure - Read Dynamic Variable Classification
 */
typedef struct {
    DeviceVariableClassificationCodes m_ucPrimaryVariableClassification; /**< Primary Variable Classification */
    DeviceVariableClassificationCodes m_ucSecondaryVariableClassification; /**< Secondary Variable Classification */
    DeviceVariableClassificationCodes m_ucTertiaryVariableClassification; /**< Tertiary Variable Classification */
    DeviceVariableClassificationCodes m_ucQuaternaryVariableClassification; /**< Quaternary Variable Classification */
} C008_ReadDynamicVariableClassification_Resp;

/**
 * C009 Request Data Structure - Read Device Variables With Status
 */
typedef struct {
    uint8 m_aucDeviceVarCode[C009_MAX_REQ_SIZE]; /**< Array of Device Variables Code */
    uint8 m_ucNoDeviceVariables; /**< Number of Device Variables*/
} C009_ReadDeviceVarsWithStatus_Req;

/**
 * C009 Response Data Structure - Read Device Variables With Status
 */
typedef struct {
    uint8 m_ucExtendedFieldDeviceStatus; /**< Extended Field Device Status */
    C009_Slot_t m_astSlot[C009_MAX_REQ_SIZE]; /**< Array of Slots */
    Time_t m_stSlot0DataTimeStamp; /**< Slot 0 Data time stamp */
    uint8 m_ucNoDeviceVariables; /**< Number of Device Variables*/
} C009_ReadDeviceVarsWithStatus_Resp;

/**
 * C014 Request Data Structure - Read Dynamic Variables Classification
 */
typedef EmptyCommand_Req C014_ReadDynamicVariableClassification_Req;

/**
 * C014 Response Data Structure - Read Dynamic Variables Classification
 */
typedef struct {
    uint32 m_ulTransducerSerialNumber; /**<  Transducer Serial Number */
    UnitsCodes m_ucUnitsCode; /**<  Transducer Limits and Minumum Span Units Code */
    float m_fUpperTransducerLimit; /**< Upper Transducer Limit */
    float m_fLowerTransducerLimit; /**< Lower Transducer Limit */
    float m_fMinimumSpan; /**< Minimum Span */
} C014_ReadDynamicVariableClassification_Resp;


/**
 * C015 Request Data Structure - Read Device Info
 */
typedef EmptyCommand_Req C015_ReadDeviceInfo_Req;

/**
 * C015 Response Data Structure - Read Device Info
 */
typedef struct {
    AlarmSelectionCodes pvAlarmSelectionCodes; /**< PV Alarm Selection Code */
    TransferFunctionCodes pvTransferFunctionCode; /**< PV Transfer Function Code */
    UnitsCodes pvUpperLowerRangeValuesUnitsCode; /**< PV Upper and Lower Range Values Units Code */
    float pvUpperRangeValue; /**< PV Upper Range Value */
    float pvLowerRangeValue; /**< PV Lower Range Value */
    float pvDampingValue; /**< PV Damping Value*/
    WriteProtectCodes writeProtectCode; /**< Write Protect Code*/
    uint8 reservedValue; /**< Reserved must be set to "250", Not Used */
    uint8 pvAnalogChannelFlags; /**< PV Analog Channel Flags */
} C015_ReadDeviceInfo_Resp;

/**
 * C033 Request Data Structure - Read Device Variables
 */
typedef struct {
    uint8 m_aucDeviceVarCode[C033_MAX_REQ_SIZE]; /**< Array of Device Variable Code */
    uint8 m_ucNoDeviceVariables; /**< Number of Device Variables*/
} C033_ReadDeviceVars_Req;

/**
 * C033 Response Data Structure - Read Device Variables
 */
typedef struct {
    C033_Slot_t m_astSlot[C033_MAX_REQ_SIZE]; /**< Array of Device Variable Code */
    uint8 m_ucNoDeviceVariables; /**< Number of Device Variable */
} C033_ReadDeviceVars_Resp;

/**
 * C035 Request Data Structure - Write Primary Variable Range Values
 */
typedef struct {
    uint8 m_ucUnitsCode;
    fp32 m_fUpperRangeValue;
    fp32 m_fLowerRangeValue;    
}__attribute__((packed))C035_WritePVRangeValues_Req;

/**
 * C035 Response Data Structure - Write Primary Variable Range Values
 */
typedef C035_WritePVRangeValues_Req C035_WritePVRangeValues_Resp;

/**
 * C044 Request Data Structure - Write Primary Variable Units
 */
typedef struct{
    uint8 m_ucUnitsCode;
}C044_WritePVUnits_Req;

/**
 * C044 Response Data Structure - Write Primary Variable Units
 */
typedef C044_WritePVUnits_Req C044_WritePVUnits_Resp;

/**
 * C048 Request Data Structure - Read Additional Device Status
 */
typedef struct {
    uint8 deviceSpecificStatus1[6]; /**< Device Specific Status */
    uint8 extendedDeviceStatus; /**< Extended Device Status */
    uint8 deviceOperatingMode; /**< Device Operating Mode*/ // Table 14. Operating Mode Codes Code Operating Mode Description This Table is Reserved
    uint8 standardizedStatus0; /**< Standardized Status 0 */
    uint8 standardizedStatus1; /**<  Standardized Status 1 */// Table 30. Standardized Status 1 Code Description This Table is Reserved
    uint8 analogChannelSaturatedCode; /**< Analog Channel Saturated */
    uint8 standardizedStatus2; /**< Standardized Status 2 */
    uint8 standardizedStatus3; /**< Standardized Status 3 */
    uint8 analogChannelFixedCode; /**< Analog Channel Fixed */
    uint8 deviceSpecificStatus2[11]; /**< Device Specific Status */
    uint8 commandSize; /**< For internal use only */
} C048_ReadAdditionalDeviceStatus_Req;

/**
 * C048 Response Data Structure - Read Additional Device Status
 */
typedef C048_ReadAdditionalDeviceStatus_Req C048_ReadAdditionalDeviceStatus_Resp;


/**
 * C050 Request Data Structure - Read Dynamic Variables Assignment
 */
typedef EmptyCommand_Req C050_ReadDynamicVariableAssignments_Req;

/**
 * C050 Response Data Structure - Read Dynamic Variables Assignment
 */
typedef struct {
    uint8 m_aucVariables[PROV_ucMaxNoOfDeviceVars + 1]; /**< Device Variables assigned to the [Primary->Quaternary] Variable */
} C050_ReadDynamicVariableAssignments_Resp;

/**
 * C051 Request Data Structure - Write Dynamic Variable Asignments
 */
typedef struct {
    uint8 m_aucVariables[PROV_ucMaxNoOfDeviceVars + 1]; /**< Device Variables assigned to the [Primary->Quaternary] Variable */
    uint8 p_ucNoReqDataBytes; /**< Number of bytes requested - used for Backward Compatibility */
} C051_WriteDynamicVariableAssignments_Req;

/**
 * C051 Response Data Structure - Write Dynamic Variable Asignments
 */
typedef struct {
    uint8 m_aucVariables[PROV_ucMaxNoOfDeviceVars + 1]; /**< Device Variables assigned to the [Primary->Quaternary] Variable */
} C051_WriteDynamicVariableAssignments_Resp;

/**
 * C054 Request Data Structure - Read Device Variable Information
 */
typedef struct {
    uint8 m_ucDeviceVariableCode; /**< Device Variable Code */
} C054_ReadDeviceVarsInfo_Req;

/**
 * C054 Response Data Structure - Read Device Variable Information
 */
typedef struct {
    uint8 m_ucDeviceVariableCode; /**< Device Variable Code */
    uint32 m_ulTransducerSerialNumber; /**< Transducer Serial Number */
    UnitsCodes m_ucUnitsCode; /**< Device Variable Limits/Minimum Span Units Code */
    fp32 m_fUpperTransducerLimit; /**< Upper Transducer Limit */
    fp32 m_fLowerTransducerLimit; /**< Lower Transducer Limit */
    fp32 m_fDeviceVariableDampingValue; /**< Device Variable Damping Value */
    fp32 m_fDeviceVariableMinimumSpan; /**< Device Variable Minimum Span */
    DeviceVariableClassificationCodes m_ucClassificationCode; /**< Device Variable Classification */
    DeviceVariableFamilyCodes m_ucDeviceVariableFamily; /**< Device Variable Family */
    uint32 m_ulUpdateTimePeriod; /**< Indicates the maximum period between Device Variable updates */
    uint8 m_ucDevVarProp; /**< Device Variable Properties. Common Table 65. */
} C054_ReadDeviceVarsInfo_Resp;

/**
 * C059 Request Data Structure - Write number Of Response Preambles
 */
typedef struct {
    uint8 m_ucNPreambles; //*< Number of preambles to be sent with the response message from the Slave to the Master */
} C059_WriteNumberOfResponsePreambles_Req;

/**
 * C059 Response Data Structure - Write number Of Response Preambles
 */
typedef struct {
    uint8 m_ucNPreambles; //*< Number of preambles to be sent with the response message from the Slave to the Master */
} C059_WriteNumberOfResponsePreambles_Resp;

/**
 * C079 Request Data Structure - Write Device Variable
 */
typedef struct {
    uint8 m_ucDeviceVariableCode; //*< Device Variable Code */
    WriteDeviceVariableCodes m_ucWriteCommandCode; //*< Write Device Variable Command Code */
    UnitsCodes m_ucUnitsCode; /**< Units Code */
    fp32 m_fValue; //*< Device Variable value */
    DeviceVariableStatus m_ucDeviceVariableStatus; //*< Device Variable status */
} C079_WriteDeviceVariable_Req;

/**
 * C079 Response Data Structure - Write Device Variable
 */
typedef struct {
    uint8 m_ucDeviceVariableCode; //*< Device Variable Code */
    WriteDeviceVariableCodes m_ucWriteCommandCode; //*< Write Device Variable Command Code */
    UnitsCodes m_ucUnitsCode; /**< Units Code */
    fp32 m_fValue; //*< Device Variable value */
    DeviceVariableStatus m_ucDeviceVariableStatus; //*< Device Variable status */
} C079_WriteDeviceVariable_Resp;

/**
 * C103 Request Data Structure - Write Burst Period
 */
// C103

typedef struct {
    uint8 m_ucBurstMessage; /**< Burst Message */
    uint32 m_ulUpdatePeriod; /**< Update Period in 1/32 of a millisecond. Update Period must not exceed 3600 seconds. */
    uint32 m_ulMaxUpdatePeriod; /**< Maximum Update Period in 1/32 of a millisecond. Maximum Update Period must not exceed 3600 seconds*/
} C103_WriteBurstPeriod_Req;

/**
 * C103 Response Data Structure - Write Burst Period
 */
typedef C103_WriteBurstPeriod_Req C103_WriteBurstPeriod_Resp;

/**
 * C104 Request Data Structure - Write Burst Trigger
 */
typedef struct {
    uint8 m_ucBurstMessage; /**< Burst Message */
    BurstMessageTriggerMode m_ucSelectionCode; /**< Burst Trigger Mode Selection Code */
    DeviceVariableClassificationCodes m_ucClassificationCode; /**< Device Variable Classification for Trigger Level */
    UnitsCodes m_ucUnitsCode; /**< Units Code */
    float m_ulTriggerLevel; /**< Trigger Level */
} C104_WriteBurstTrigger_Req;

/**
 * C104 Response Data Structure - Write Burst Trigger
 */
typedef C104_WriteBurstTrigger_Req C104_WriteBurstTrigger_Resp;

/**
 * C105 Request Data Structure - Read Burst Mode Configuration
 */
typedef struct {
    uint8 m_ucBurstMessage; /**< Burst Message */
    uint8 m_unRequestSize; /**< Request Size */
} C105_ReadBurstModeConfiguration_Req;

/**
 * C105 Response Data Structure - Read Burst Mode Configuration
 */
typedef struct {
    uint8 m_ucBurstModeControlCode; /**< Burst Mode Control Code */
    uint8 m_ucCommandNumberExpansionFlag; /**< Command Number Expansion Flag */
    uint8 m_aucDeviceVariableCodes[8]; /**< Device Variables Codes assigned to Slot[0-7] */
    uint8 m_ucBurstMessage; /**< Burst Message */
    uint8 m_ucMaxNumberOfBurstMessages; /**< Maximum number of burst Messages supported by the device */
    uint16 m_unExtendedCommandNumber; /**< Extended Command Number */
    uint32 m_ulUpdatePeriod; /**< Update Time in 1/32 of a millisecond */
    uint32 m_ulMaxUpdatePeriod; /**< Maximum Update Time in 1/32 of a millisecond */
    BurstMessageTriggerMode m_ucSelectionCode; /**< Burst Trigger Mode Code */
    DeviceVariableClassificationCodes m_ucClassificationCode; /**< Device Variable Classification for Trigger Value */
    UnitsCodes m_ucUnitsCode; /**< Units Code */
    float m_ulTriggerLevel; /**< Trigger Value */
} C105_ReadBurstModeConfiguration_Resp;

/**
 * C107 Request Data Structure - Write Burst Device Variables
 */
typedef struct {
    uint8 m_ucNoOfDeviceVariables; //*< Number of Variables used for Backward Compatibility */
    DEVICE_VARIABLE_CODE m_aDeviceVariableCode[MAX_DEVICE_VARIABLES]; /**< Device Variable Code assigned to Slot[0-7]*/
    uint8 m_ucBurstMessage; /**< Burst Message */
} C107_WriteBurstDeviceVars_Req;

/**
 * C107 Response Data Structure - Write Burst Device Variables
 */
typedef struct {
    DEVICE_VARIABLE_CODE m_aDeviceVariableCode[MAX_DEVICE_VARIABLES]; /**< Device Variable Code assigned to Slot[0-7]*/
    uint8 m_ucBurstMessage; /**< Burst Message */
} C107_WriteBurstDeviceVars_Resp;

/**
 * C108 Request Data Structure - Write Burst Mode Command Number
 */
typedef struct {
    uint16 m_unCommandNumber; /**< 16-bit Command number of the response message to be transmitted */
    uint8 m_ucMode; /**< flag to indicate the mode( 0 for backward compatibility or 1 for normal mode) */
    uint8 m_ucBurstMessage; /**< Burst Message*/
} C108_WriteBurstModeCommandNo_Req;

/**
 * C108 Response Data Structure - Write Burst Mode Command Number
 */
typedef C108_WriteBurstModeCommandNo_Req C108_WriteBurstModeCommandNo_Resp;

/**
 * C109 Request Data Structure - Burst Mode Control
 */
typedef struct {
    BURST_MODE_CONTROL_CODE m_burstModeControlCode; /**< Burst Mode Control Code */
    uint8 m_ucBurstMessage; /**< Burst Message */
} C109_BurstModeCtrl_Req;

/**
 * C109 Response Data Structure - Burst Mode Control
 */
typedef C109_BurstModeCtrl_Req C109_BurstModeCtrl_Resp;

/**
 * C115 Request Data Structure - Read Event notification Summary
 */
typedef struct {
    uint8 eventNo; /**< Event Specification number*/
} C115_ReadEventNotificationSummary_Req;

/**
 * C115 Response Data Structure - Read Event notification Summary
 */
typedef struct {
    uint8 noOfReadEvents; /**< Number of read events */
    uint8 eventNo; /**< Event Specification Number */
    uint8 noOfEventsSupported; /**< number of Events Supported */
    uint8 eventStatus; /**< Most Significant 4 Bits - eventStatus and eventNotificationControlCode share same byte */
    EventNotificationControlCodes eventNotificationControlCode; //*< Least Significant 4 bits */
    Time_t firstUnacknowledged; /**< Time wehn first unacknowledged event was triggered (must be set to 0xFFFFFFFF when no events are pending) */
    Time_t eventNotificationRetryTime; /**< Event Notification retry Time */
    Time_t maximumUpdateTime; /**< Maximum Update Time */
    Time_t eventDeBounceInterval; /**< Event De-bounce Interval */
    uint8 DeviceStatusMask; /**< Device Status Mask */
    uint8 eventMask[MaxNoOfReadEvents]; /**< Event Mask. The mask for triggering on bits set in the Command 48 Response */
} C115_ReadEventNotificationSummary_Resp;

/**
 * C116 Request Data Structure - Write Event Notification Bit Mask
 */
typedef struct {
    uint8 noOfWriteEvents; /**< Number of Write Events */
    uint8 eventNo; /**< Event Specification number */
    uint8 DeviceStatusMask; /**< Device Status Mask */
    uint8 eventMask[MaxNoOfWriteEvents]; /**< Event Mask. The mask for triggering on bits set in the Command 48 Response */
} C116_WriteEventNotificationBitMask_Req;

/**
 * C116 Response Data Structure - Write Event Notification Bit Mask
 */
typedef C116_WriteEventNotificationBitMask_Req C116_WriteEventNotificationBitMask_Resp;

/**
 * C117 Request Data Structure - Write Event Notification Timing
 */
typedef struct {
    uint8 eventNo; /**< Event Specification NUmber */
    Time_t eventNotificationRetryTime; /**< Event Notification Retry Time */
    Time_t maximumUpdateTime; /**< Maximum Update Time */
    Time_t eventDeBounceInterval; /**< Event De-bounce Interval */
} C117_WriteEventNotificationTiming_Req;

/**
 * C117 Request Data Structure - Write Event Notification Timing
 */
typedef C117_WriteEventNotificationTiming_Req C117_WriteEventNotificationTiming_Resp;

/**
 * C118 Request Data Structure - Event Notification Control
 */
typedef struct {
    uint8 eventNo; //*< Event Specification number */
    EventNotificationControlCodes eventNotificationControlCode; /**< Event Notification Control Code */
} C118_EventNotificationControl_Req;

/**
 * C118 Response Data Structure - Event Notification Control
 */
typedef C118_EventNotificationControl_Req C118_EventNotificationControl_Resp;

/**
 * C119 Request Data Structure - Acknowledge Event Notification
 */
typedef struct {
    uint8 eventNo; //*< Event Specification number */
    Time_t firstUnacknowledged; /**< Time wehn first unacknowledged event was triggered (must be set to 0xFFFFFFFF when no events are pending) */
    uint16 configurationChangedCounter; /**< Configuration Changed Counter*/
    uint8 deviceStatus; /**< Device Status */
    uint8 command48Data[MaxLengthOf48Command]; /**< Command 48 data */
} C119_AcknowledgeEventNotification_Req;

/**
 * C119 Response Data Structure - Acknowledge Event Notification
 */
typedef C119_AcknowledgeEventNotification_Req C119_AcknowledgeEventNotification_Resp;


/**
 * C150 Request Data Structure - vendor-specific commands - Read Application Processor Firmware Version
 */
typedef EmptyCommand_Req C150_ReadAppFwVersion_Req;

/**
 * C150 Response Data Structure - vendor-specific commands - Read Application Processor Firmware Version
 */
typedef struct {
    uint8 ucMajor; /**< Application Processor Firmware Build Major Revision */
    uint8 ucMinor; /**< Application Processor Firmware Build Minor Revision */
    uint8 ucRevision; /**< Application Processor Firmware Build Revision */
    uint8 ucBuildLetter; /**< Application Processor Firmware Build Letter */
    char * pcDate; /**< Application Processor Firmware Build Date */
    char * pcTime; /**< Application Processor Firmware Build Time */
} C150_ReadAppFwVersion_Resp;

/**
 * C151 Request Data Structure - vendor-specific commands - RF Test Mode
 */
typedef struct {
    uint8 m_ucRFTestMode;
    uint8 m_ucRFChannel;
    uint16 m_unRFOutputPowerLevel;
    uint16 m_unTimeout;
    uint8 m_aRxEUIFilter[8];
} C151_RFEngineeringMode_Req;

/**
 * C151 Response Data Structure - vendor-specific commands - RF Test Mode
 */
typedef struct {
    uint8 m_ucRFTestMode;
    uint8 m_ucRFChannel;
    uint16 m_unRFOutputPowerLevel;
    uint16 m_unTimeout;
    uint8 m_aRxEUIFilter[8];
} C151_RFEngineeringMode_Resp;

/**
 * C152 Request Data Structure - vendor-specific commands - Events Management
 */
typedef struct {
    uint8 m_ucPassword[3];
    uint8 m_ucAction;
} C152_EventsManagement_Req;

/**
 * C152 Response Data Structure - vendor-specific commands - Events Management
 */
typedef struct {
    uint8 m_ucPassword[3];
    uint8 m_ucAction;
} C152_EventsManagement_Resp;

/**
 * C180 Request Data Structure - vendor-specific commands - Execute Immediate Measurement
 */
typedef struct {
    uint8 m_ucMesStrtStat;
} C180_MakeMeasurement_Req;

/**
 * C180 Response Data Structure - vendor-specific commands - Execute Immediate Measurement
 */
typedef struct {
    float h_fMat_Thick_Raw;
    float h_fMat_Thick_TC;
    float h_fMat_Temp;
    float h_fDSI_Temp ;
} C180_MakeMeasurement_Resp;


/**
 * C181 Request Data Structure - vendor-specific commands - Write Measurement Parameters
 */
typedef struct {
    float h_cal_0_offset;
    float h_material_velocity;
    float h_a_scan_start;
    uint16 h_pulsar_width;
    uint16 h_gain;
} C181_WriteUTMParameters_Req;

/**
 * C181 Response Data Structure - vendor-specific commands - Write Measurement Parameters
 */
typedef struct {
    float h_cal_0_offset;
    float h_material_velocity;
    float h_a_scan_start;
    uint16 h_pulsar_width;
    uint16 h_gain;    
} C181_WriteUTMParameters_Resp;

/**
 * C182 Request Data Structure - vendor-specific commands - Read Measurement Parameters
 */
typedef struct {
    float h_cal_0_offset;
    float h_material_velocity;
    float h_a_scan_start;
    uint16 h_pulsar_width;
    uint16 h_gain;
} C182_ReadUTMParameters_Req;

/**
 * C182 Response Data Structure - vendor-specific commands - Read Measurement Parameters
 */
typedef struct {
    float h_cal_0_offset;
    float h_material_velocity;
    float h_a_scan_start;
    uint16 h_pulsar_width;
    uint16 h_gain;
} C182_ReadUTMParameters_Resp;

/**
 * C183 Request Data Structure - vendor-specific commands - Write Gate Parameters
 */
typedef struct {
    float h_g1_start;
    float h_g1_width;
    float h_g1_thershold;
    float h_g2_start;
    float h_g2_width;
    float h_g2_thershold;
    uint16 h_measurement_mode;
} C183_WriteUTGParameters_Req;

/**
 * C183 Response Data Structure - vendor-specific commands - Write Measurement Parameters
 */
typedef struct {
    float h_g1_start;
    float h_g1_width;
    float h_g1_thershold;
    float h_g2_start;
    float h_g2_width;
    float h_g2_thershold;
    uint16 h_measurement_mode;
    
} C183_WriteUTGParameters_Resp;

/**
 * C184 Request Data Structure - vendor-specific commands - Read Measurement Parameters
 */
typedef struct {
    float h_g1_start;
    float h_g1_width;
    float h_g1_thershold;
    float h_g2_start;
    float h_g2_width;
    float h_g2_thershold;
    uint16 h_measurement_mode;
} C184_ReadUTGParameters_Req;

/**
 * C184 Response Data Structure - vendor-specific commands - Read Measurement Parameters
 */
typedef struct {
    float h_g1_start;
    float h_g1_width;
    float h_g1_thershold;
    float h_g2_start;
    float h_g2_width;
    float h_g2_thershold;
    uint16 h_measurement_mode;
} C184_ReadUTGParameters_Resp;

/**
 * C185 Request Data Structure - vendor-specific commands - Write Temp. Comp Parameters
 */

typedef struct {
    float h_Ref_temp;
    float h_material_velocity;
    float h_Comp_factor;
} C185_WriteUTTParameters_Req;

/**
 * C185 Response Data Structure - vendor-specific commands - Write Temp. Comp Parameters
 */
typedef struct {
    float h_Ref_temp;
    float h_material_velocity;
    float h_Comp_factor;
} C185_WriteUTTParameters_Resp;

/**
 * C186 Request Data Structure - vendor-specific commands - Read Temp. Comp Parameters
 */

typedef EmptyCommand_Req C186_ReadUTTParameters_Req;

/**
 * C186 Response Data Structure - vendor-specific commands - Read Temp. Comp Parameters
 */
typedef struct {
    float h_Ref_temp;
    float h_material_velocity;
    float h_Comp_factor;
} C186_ReadUTTParameters_Resp;


/**
 * C188 Request Data Structure - vendor-specific commands - Reset battery Life
 */
typedef EmptyCommand_Req C188_ResetBatteryLife_Req;

/**
 * C188 Response Data Structure - vendor-specific commands - Reset battery Life
 */
typedef struct {
    uint16 BatteryLifeDays;
} C188_ResetBatteryLife_Resp;

///**
// * C190 Request Data Structure - vendor-specific commands - Execute Immediate Measurement
// */
//typedef struct {
//    uint8 m_ucMesStrtStat;
//} C190_MakeMeasurement_Req;
//
///**
// * C190 Response Data Structure - vendor-specific commands - Execute Immediate Measurement
// */
//typedef struct {
//    float h_fMat_Thick_Raw;
//    float h_fMat_Thick_TC;
//    float h_fMat_Temp;
//    float h_fDSI_Temp ;
//} C190_MakeMeasurement_Resp;


///**
// * C191 Request Data Structure - vendor-specific commands - Write Measurement Parameters
// */
//typedef struct {
//    float h_cal_0_offset;
//    float h_material_velocity;
//    float h_a_scan_start;
//    uint16 h_pulsar_width;
//    uint16 h_gain;
//    float h_g1_start;
//    float h_g1_width;
//    float h_g1_thershold;
//    float h_g2_start;
//    float h_g2_width;
//    float h_g2_thershold;
//    uint16 h_measurement_mode;
//} C191_WriteUTMParameters_Req;
//
///**
// * C191 Response Data Structure - vendor-specific commands - Write Measurement Parameters
// */
//typedef struct {
//    float h_cal_0_offset;
//    float h_material_velocity;
//    float h_a_scan_start;
//    uint16 h_pulsar_width;
//    uint16 h_gain;
//    float h_g1_start;
//    float h_g1_width;
//    float h_g1_thershold;
//    float h_g2_start;
//    float h_g2_width;
//    float h_g2_thershold;
//    uint16 h_measurement_mode;
//    
//} C191_WriteUTMParameters_Resp;

///**
// * C192 Request Data Structure - vendor-specific commands - Read Measurement Parameters
// */
//typedef struct {
//    float h_cal_0_offset;
//    float h_material_velocity;
//    float h_a_scan_start;
//    uint16 h_pulsar_width;
//    uint16 h_gain;
//    float h_g1_start;
//    float h_g1_width;
//    float h_g1_thershold;
//    float h_g2_start;
//    float h_g2_width;
//    float h_g2_thershold;
//    uint16 h_measurement_mode;
//} C192_ReadUTMParameters_Req;
//
///**
// * C192 Response Data Structure - vendor-specific commands - Read Measurement Parameters
// */
//typedef struct {
//    float h_cal_0_offset;
//    float h_material_velocity;
//    float h_a_scan_start;
//    uint16 h_pulsar_width;
//    uint16 h_gain;
//    float h_g1_start;
//    float h_g1_width;
//    float h_g1_thershold;
//    float h_g2_start;
//    float h_g2_width;
//    float h_g2_thershold;
//    uint16 h_measurement_mode;
//} C192_ReadUTMParameters_Resp;


///**
// * C193 Request Data Structure - vendor-specific commands - Write Temp. Comp Parameters
// */
//
//typedef struct {
//    float h_Ref_temp;
//    float h_material_velocity;
//    float h_Comp_factor;
//} C193_WriteUTTParameters_Req;
//
///**
// * C193 Response Data Structure - vendor-specific commands - Write Temp. Comp Parameters
// */
//typedef struct {
//    float h_Ref_temp;
//    float h_material_velocity;
//    float h_Comp_factor;
//} C193_WriteUTTParameters_Resp;

///**
// * C194 Request Data Structure - vendor-specific commands - Read Temp. Comp Parameters
// */
//
//typedef EmptyCommand_Req C194_ReadUTTParameters_Req;
//
///**
// * C194 Response Data Structure - vendor-specific commands - Read Temp. Comp Parameters
// */
//typedef struct {
//    float h_Ref_temp;
//    float h_material_velocity;
//    float h_Comp_factor;
//} C194_ReadUTTParameters_Resp;

/**
 * C195 Request Data Structure - vendor-specific commands - Access Password
 */
typedef struct {
    uint8 m_aucPassword[8];
} C195_AccessPassword_Req;

/**
 * C194 Response Data Structure - vendor-specific commands - Access Password
 */
typedef struct {
    uint8 m_aucPassword[8];
} C195_AccessPassword_Resp;

/**
 * C196 Request Data Structure - vendor-specific commands - Write Password
 */
typedef struct {
    uint8 m_aucLevel1Password[8];
} C196_WritePassword_Req;

/**
 * C196 Response Data Structure - vendor-specific commands - Write Password
 */
typedef struct {
    uint8 m_aucLevel1Password[8];
} C196_WritePassword_Resp;

/**
 * C197 Request Data Structure - vendor-specific commands - Disable Password
 */
typedef struct{
    uint8 m_ucEnableOrDisable;
}C197_DisablePassword_Req;

/**
 * C197 Response Data Structure - vendor-specific commands - Disable Password
 */
typedef struct {
    uint8 m_ucEnableOrDisable;
} C197_DisablePassword_Resp;

/**
 * C198 Request Data Structure - vendor-specific commands - Write Alarm Mode
 */
typedef struct {
    uint8 m_ucAlarmMode;
} C198_WriteAlarmMode_Req;

/**
 * C198 Response Data Structure - vendor-specific commands - Write Alarm Mode
 */
typedef struct {
    uint8 m_ucAlarmMode;
} C198_WriteAlarmMode_Resp;

/**
 * C201 Request Data Structure - vendor-specific commands - Write MS Serial Number
 */
typedef struct
{
    uint8 m_aucMSSerialNumber[14];
}C201_WriteMSSerialNumber_Req;
/**
 * C201 Response Data Structure - vendor-specific commands - Write MS Serial Number
 */
typedef struct {
    uint8 m_aucMSSerialNumber[14];
} C201_WriteMSSerialNumber_Resp;

/**
 * C202 Request Data Structure - vendor-specific commands - Read MS Serial Number
 */
typedef EmptyCommand_Req C202_ReadMSSerialNumber_Req;

/**
 * C202 Response Data Structure - vendor-specific commands - Read MS Serial Number
 */
typedef struct {
    uint8 m_aucMSSerialNumber[14];
} C202_ReadMSSerialNumber_Resp;

/**
 * C203 Request Data Structure - vendor-specific commands - Read Commission Date
 */
//typedef EmptyCommand_Req C203_ReadCommissionDate_Req;

/**
 * C203 Response Data Structure - vendor-specific commands - Read Commission Date
 */
typedef struct {
    uint8 m_ucMonth;
    uint8 m_ucDay;    
    uint8 m_ucYear;
} C203_WriteCommissionDate_Resp;

/**
 * C203 Request Data Structure - vendor-specific commands - Write Commission Date
 */
typedef struct
{   uint8 m_ucMonth;
    uint8 m_ucDay;    
    uint8 m_ucYear;
}C203_WriteCommissionDate_Req;
        

/**
 * C204 Response Data Structure - vendor-specific commands - Read Commission Date
 */
typedef EmptyCommand_Req C204_ReadCommissionDate_Req;


/**
 * C204 Response Data Structure - vendor-specific commands - Read Commission Date
 */
typedef struct {
    uint8 m_ucMonth;
    uint8 m_ucDay;    
    uint8 m_ucYear;
} C204_ReadCommissionDate_Resp;

/**
 * C205 Request Data Structure - vendor-specific commands - Write Measurement Time Interval
 */
typedef struct
{
    uint16 m_unMeasurementTimeInterval;
}C205_WriteMeasurementTimeInterval_Req;

/**
 * C205 Response Data Structure - vendor-specific commands - Write Measurement Time Interval
 */
typedef struct
{
    uint16 m_unMeasurementTimeInterval;
}C205_WriteMeasurementTimeInterval_Resp;


/**
 * C206 Request Data Structure - vendor-specific commands - Read Measurement Time Interval
 */
typedef EmptyCommand_Req C206_ReadMeasurementTimeInterval_Req;

/**
 * C204 Response Data Structure - vendor-specific commands - SetTestMode
 */
typedef struct
{
    uint16 m_unMeasurementTimeInterval;
}C206_ReadMeasurementTimeInterval_Resp;


/**
 * C210 Request Data Structure - vendor-specific commands - Factory Defaults
 */

typedef EmptyCommand_Req C210_FactoryDefaults_Req;

/**
 * C210 Response Data Structure - vendor-specific commands - Factory Defaults
 */

typedef C210_FactoryDefaults_Req C210_FactoryDefaults_Resp;

/**
 * C778 Request Data Structure - vendor-specific commands - Read Battery Life
 */
typedef EmptyCommand_Req C778_ReadBatteryLife_Req;

/**
 * C778 Response Data Structure - vendor-specific commands - Read Battery Life
 */
typedef struct {
    uint16 BatteryLifeDays;
} C778_ReadBatteryLife_Resp;



#endif // __CENTERO_APP_WH_TYPES_H_
