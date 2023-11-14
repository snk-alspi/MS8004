/*************************************************************************
 * File: provision.c
 * Description: Define the persistent area management interface
 *************************************************************************/
#include "xc.h"
#include "provision.h"
#include "SoftwareCrc.h"
#include "string.h"
#include "../Platform/nvm.h"
#include "../MS/MS_Data.h"
#include "../WirelessHART_AppLayer/App_WH_User.h"
#include "../WirelessHART_AppLayer/App_WH_CmdParse.h"
#include "../WirelessHART_AppLayer/App_WH_CmdExecute.h"
#include "../WirelessHART_AppLayer/App_WH_CmdCompose.h"


extern NOT_USED_VALUE NotUsedValue; ///< Not Used Value.

// Device Variables Structures
PROVISION_T g_stProv;   // holds the provisioned data


const NV_DEVICE_VARIABLE_T g_astNVDevVarInitData[MAX_DEV_VARS_NO] = {

    //IDX0 STD, Device Variable 0 - Material thickness raw
    {
        { 0, 0, LimitStatus_NotLimited, ProcessDataQuality_Good }, // m_ucStatus;
        DEVICE_VARIABLE_0, // m_ucCode;
        0, // m_ulTransducerSN;
        UnitsCodes_Millimeters, // m_ucUnit;
        DEFAULT_PV_UPPER_LIMIT, // m_fUpperLimit;
        PV_LOWER_LIMIT, // m_fLowerLimit;
        0.1, // m_fDampingValue;
        0.0254,  //0.006003937, // m_fMinSpan; /* To be checked Modified on 03272023*/
        DeviceVariableClassificationCodes_Length,  //DeviceVariableClassificationCodes_DeviceVariableNotClassified, // m_ucClassification;
        DeviceVariableFamilyCodes_NotUsed, // m_ucFamily;
        1800 //300 // sec // m_ulUpdateTimePeriod;
    },

    //IDX1 STD, Device Variable 1 - Material thickness tc
    {
        { 0, 0, LimitStatus_NotLimited, ProcessDataQuality_Good }, // m_ucStatus;
        DEVICE_VARIABLE_1, // m_ucCode;
        0, // m_ulTransducerSN;
        UnitsCodes_Millimeters, // m_ucUnit;
        254, // m_fUpperLimit;
        0.0254, // m_fLowerLimit;
        0.0, // m_fDampingValue;
        0.0254, // m_fMinSpan; /* To be checked Modified on 03272023*/
        DeviceVariableClassificationCodes_Length,  //DeviceVariableClassificationCodes_Time, // m_ucClassification;
        DeviceVariableFamilyCodes_NotUsed, // m_ucFamily;
        1800 // 300 // sec // m_ulUpdateTimePeriod;
    },

    //IDX2 STD, Device Variable 2 - Material temperature
    {
        { 0, 0, LimitStatus_NotLimited, ProcessDataQuality_Good }, // m_ucStatus;
        DEVICE_VARIABLE_2, // m_ucCode;
        0, // m_ulTransducerSN;
        UnitsCodes_DegreesCelsius, // m_ucUnit;
        999.0, // m_fUpperLimit;
        -40.0, // m_fLowerLimit;
        0.0, // m_fDampingValue;
        0.0, // m_fMinSpan;
        DeviceVariableClassificationCodes_Temperature, //DeviceVariableClassificationCodes_DeviceVariableNotClassified, // m_ucClassification;
        DeviceVariableFamilyCodes_Temperature, //DeviceVariableFamilyCodes_NotUsed, // m_ucFamily;
        1800 //300 // sec // m_ulUpdateTimePeriod;
    },

    //IDX3 STD, Device Variable 3 - percentage Battery remaining
    {
        { 0, 0, LimitStatus_NotLimited, ProcessDataQuality_Good }, // m_ucStatus;
        DEVICE_VARIABLE_3, // m_ucCode;
        0, // m_ulTransducerSN;
        UnitsCodes_Percent, // m_ucUnit;
        100.0, // m_fUpperLimit;
        0.0, // m_fLowerLimit;
        0.0, // m_fDampingValue;
        0.01, // m_fMinSpan;
        DeviceVariableClassificationCodes_DeviceVariableNotClassified, // m_ucClassification;
        DeviceVariableFamilyCodes_NotUsed, // m_ucFamily;
        1800 //300 // sec // m_ulUpdateTimePeriod;
    },

    //IDX4 STD, Device Variable 4 - DSI temperature
    {
        { 0, 0, LimitStatus_NotLimited, ProcessDataQuality_Good }, // m_ucStatus;
        DEVICE_VARIABLE_4, // m_ucCode;
        0, // m_ulTransducerSN;
        UnitsCodes_DegreesCelsius, // m_ucUnit;
        999.0, // m_fUpperLimit;
        -40.0, // m_fLowerLimit;
        0.0, // m_fDampingValue;
        0.0, // m_fMinSpan;
        DeviceVariableClassificationCodes_Temperature,//DeviceVariableClassificationCodes_Time, // m_ucClassification;
        DeviceVariableFamilyCodes_Temperature, //DeviceVariableFamilyCodes_NotUsed, // m_ucFamily;
        1800 //300 // sec // m_ulUpdateTimePeriod;
    },

    //IDX5 STD, Device Variable 5 - Remaining Battery Life in days
    {
        { 0, 0, LimitStatus_NotLimited, ProcessDataQuality_Good }, // m_ucStatus;
        DEVICE_VARIABLE_5, // m_ucCode;
        0, // m_ulTransducerSN;
        UnitsCodes_Days, // m_ucUnit;
        2555.0, // m_fUpperLimit;
        0.0, // m_fLowerLimit;
        0.0, // m_fDampingValue;
        0.0, // m_fMinSpan;
        DeviceVariableClassificationCodes_Time, //DeviceVariableClassificationCodes_DeviceVariableNotClassified, // m_ucClassification;
        DeviceVariableFamilyCodes_NotUsed, // m_ucFamily;
        1800 //300 // sec // m_ulUpdateTimePeriod;
    },

    //IDX6 STD, Measurement Interval time
    {
        { 0, 0, LimitStatus_NotLimited, ProcessDataQuality_Good }, // m_ucStatus;
        BatteryLife, // m_ucCode;
        0, // m_ulTransducerSN;
        UnitsCodes_Hours, // m_ucUnit;
        0xFFFFFFFF, // m_fUpperLimit; // forever
        0.0, // m_fLowerLimit;
        0.0, // m_fDampingValue;
        0.0, // m_fMinSpan;
        DeviceVariableClassificationCodes_Time, // m_ucClassification;
        DeviceVariableFamilyCodes_NotUsed, // m_ucFamily;
        1800 //300 // sec // m_ulUpdateTimePeriod;
    },

    //IDX7 STD, Percent Range
    {
        { 0, 0, LimitStatus_NotLimited, ProcessDataQuality_Good }, // m_ucStatus;
        PercentRange, // m_ucCode;
        0, // m_ulTransducerSN;
        UnitsCodes_Percent, // m_ucUnit;
        100.0, // m_fUpperLimit;
        0.0, // m_fLowerLimit;
        0.0, // m_fDampingValue;
        0.0, // m_fMinSpan;
        DeviceVariableClassificationCodes_DeviceVariableNotClassified, // m_ucClassification;
        DeviceVariableFamilyCodes_NotUsed, // m_ucFamily;
        1800 //300 // sec // m_ulUpdateTimePeriod;
    },

    //IDX8 STD, Loop Current
    {
        { 0, 0, LimitStatus_Constant, ProcessDataQuality_Good }, // m_ucStatus;
        LoopCurrent, // m_ucCode;
        0, // m_ulTransducerSN;
        UnitsCodes_Milliamperes, // m_ucUnit;
        100.0, // m_fUpperLimit;
        0.0, // m_fLowerLimit;
        0.0, // m_fDampingValue;
        0.0, // m_fMinSpan;
        DeviceVariableClassificationCodes_DeviceVariableNotClassified, // m_ucClassification;
        DeviceVariableFamilyCodes_NotUsed, // m_ucFamily;
        1800 //300 // sec // m_ulUpdateTimePeriod;
    },

    //IDX9 STD, Primary Variable - Material thickness raw
    {
        { 0, 0, LimitStatus_NotLimited, ProcessDataQuality_Good }, // m_ucStatus;
        PrimaryVariable, // m_ucCode;
        0, // m_ulTransducerSN;
        UnitsCodes_Millimeters, // m_ucUnit;
        254.0, // m_fUpperLimit;
        0.0254, // m_fLowerLimit;
        0.0, // m_fDampingValue;
        0.0254, // m_fMinSpan;
        DeviceVariableClassificationCodes_Length, // m_ucClassification;
        DeviceVariableFamilyCodes_NotUsed, // m_ucFamily;
        1800 //300 // sec // m_ulUpdateTimePeriod;
    },

    //IDX5 STD, Secondary Variable - Material thickness tc
    {
        { 0, 0, LimitStatus_NotLimited, ProcessDataQuality_Good }, // m_ucStatus;
        SecondaryVariable, // m_ucCode;
        0, // m_ulTransducerSN;
        UnitsCodes_Millimeters, // m_ucUnit;
        254.0, // m_fUpperLimit;
        0.0254, // m_fLowerLimit;
        0.0, // m_fDampingValue;
        0.0, // m_fMinSpan;
        DeviceVariableClassificationCodes_Length, // m_ucClassification;
        DeviceVariableFamilyCodes_NotUsed, // m_ucFamily;
        1800 //300 // sec // m_ulUpdateTimePeriod;
    },

    //IDX6 STD, Tertiary Variable - Material temperature
    {
        { 0, 0, LimitStatus_NotLimited, ProcessDataQuality_Good }, // m_ucStatus;
        TertiaryVariable, // m_ucCode;
        0, // m_ulTransducerSN;
        UnitsCodes_DegreesCelsius, // m_ucUnit;
        999.0, // m_fUpperLimit;
        -40.0, // m_fLowerLimit;
        0.0, // m_fDampingValue;
        0.0, // m_fMinSpan;
        DeviceVariableClassificationCodes_Temperature, // m_ucClassification;
        DeviceVariableFamilyCodes_NotUsed, // m_ucFamily;
        1800 //300 // sec // m_ulUpdateTimePeriod;
    },

    //IDX7 STD, Quaternary Variable - percentage battery life
    {
        { 0, 0, LimitStatus_NotLimited, ProcessDataQuality_Good }, // m_ucStatus;
        QuaternaryVariable, // m_ucCode;
        0, // m_ulTransducerSN;
        UnitsCodes_Percent, // m_ucUnit;
        0.0, // m_fUpperLimit;
        0.0, // m_fLowerLimit;
        0.0, // m_fDampingValue;
        0.0, // m_fMinSpan;
        DeviceVariableClassificationCodes_DeviceVariableNotClassified, // m_ucClassification;
        DeviceVariableFamilyCodes_NotUsed, // m_ucFamily;
        1800 //300 // sec // m_ulUpdateTimePeriod;
    }
};

uint8 NVM_LoadArea(uint32 p_unAreaAddress);

/**
* @brief Loads the current data within NVM into the RAM image- set of structures- that will be used at run-time.
*/
void NVM_ReadAllData()
{
    // Reset this structure first
    memset((void*) &g_stProv.m_sWiredHart, 0, sizeof(g_stProv.m_sWiredHart));
    
    NVMReadPage(NVM_START_ADDRESS, g_aucFlashCopy);
    
    Boolean bArea1Signature = SignatureAreaGood();    

    if (bArea1Signature == 0x01)
    {
       memcpy(&g_stProv, (uint8*)&g_aucFlashCopy, sizeof(g_stProv));
        return;
    }

    // No page is valid, load default settings and save them into both areas
    NVM_LoadDefaultData();
}

/**
* @brief The purpose of this function is to populate the RAM image structures with default values which are then copied into the main and back-up NVM areas.
*/
void NVM_LoadDefaultData()
{
    uint8 ucIndex = 0;
    SIGNATURE_T sBadSignature;
    SIGNATURE_T sGoodSignature;

    sBadSignature.m_32  = 0xFFFFFFFFUL;
    sGoodSignature.m_32 = MEM_SIGNATURE;

    NV_BURST_T* pCrtBurst = g_stProv.m_stBurstNVM;
    for (; pCrtBurst < g_stProv.m_stBurstNVM + MAX_BURST_MSG_NO; pCrtBurst++)
    {
        pCrtBurst->m_ucBurstModeControlCode = Off; // burst is OFF

        for(ucIndex = 0; ucIndex < 8; ucIndex++)
        {
            pCrtBurst->m_aucDeviceVariableCodes[ucIndex] = NotUsed;
        }

        pCrtBurst->m_ucBurstTriggerModeCode = BurstMessageTriggerMode_Continuous;
        pCrtBurst->m_ucClassification = DeviceVariableClassificationCodes_NotUsed;
        pCrtBurst->m_ucUnit = UnitsCodes_NotUsed;
        pCrtBurst->m_ucCmdType = DAQ_FW_WIRELESS; // on Armstrong we support only Wireless command type
        pCrtBurst->m_unCmdNo = 1;
        pCrtBurst->m_ulUpdateTime = 60000ul;//60 sec default
        pCrtBurst->m_ulMaxUpdateTime = 0;
        pCrtBurst->m_fTriggerValue = 0;
    }

    // Default burst commands 3/9/48 at 30 minutes
    g_stProv.m_stBurstNVM[0].m_ucBurstModeControlCode = EnableOnTDMADataLinkLayer;
    g_stProv.m_stBurstNVM[0].m_unCmdNo = 3;
    g_stProv.m_stBurstNVM[0].m_ulUpdateTime = 1800000ul;//30 minutes default
    g_stProv.m_stBurstNVM[0].m_ulMaxUpdateTime = 1800000ul;//30 minutes default
    g_stProv.m_stBurstNVM[0].m_aucDeviceVariableCodes[0] = PrimaryVariable;
    g_stProv.m_stBurstNVM[0].m_aucDeviceVariableCodes[1] = SecondaryVariable;
    g_stProv.m_stBurstNVM[0].m_aucDeviceVariableCodes[2] = TertiaryVariable;
    g_stProv.m_stBurstNVM[0].m_aucDeviceVariableCodes[3] = QuaternaryVariable;

    g_stProv.m_stBurstNVM[1].m_ucBurstModeControlCode = EnableOnTDMADataLinkLayer;
    g_stProv.m_stBurstNVM[1].m_unCmdNo = 9;
    g_stProv.m_stBurstNVM[1].m_ulUpdateTime = 1800000ul;//30 minutes default
    g_stProv.m_stBurstNVM[1].m_ulMaxUpdateTime = 1800000ul;//30 minutes default
    g_stProv.m_stBurstNVM[1].m_aucDeviceVariableCodes[0] = PrimaryVariable;
    g_stProv.m_stBurstNVM[1].m_aucDeviceVariableCodes[1] = SecondaryVariable;
    g_stProv.m_stBurstNVM[1].m_aucDeviceVariableCodes[2] = TertiaryVariable;
    g_stProv.m_stBurstNVM[1].m_aucDeviceVariableCodes[3] = QuaternaryVariable;

    g_stProv.m_stBurstNVM[2].m_ucBurstModeControlCode = EnableOnTDMADataLinkLayer;
    g_stProv.m_stBurstNVM[2].m_unCmdNo = 48;
    g_stProv.m_stBurstNVM[2].m_ulUpdateTime = 1800000ul;//30 minutes default
    g_stProv.m_stBurstNVM[2].m_ulMaxUpdateTime = 1800000ul;//30 minutes default
    g_stProv.m_stBurstNVM[2].m_aucDeviceVariableCodes[0] = PrimaryVariable;
    g_stProv.m_stBurstNVM[2].m_aucDeviceVariableCodes[1] = SecondaryVariable;
    g_stProv.m_stBurstNVM[2].m_aucDeviceVariableCodes[2] = TertiaryVariable;
    g_stProv.m_stBurstNVM[2].m_aucDeviceVariableCodes[3] = QuaternaryVariable;

     memset((uint8 *)&g_stProv.m_stEventNVM, 0, sizeof(NV_EVENT_T));
     
    // Wired HART settings
    g_stProv.m_sWiredHart.m_Device.m_ucPreambleLength = 5;
    g_stProv.m_sWiredHart.m_Device.m_ucLoopConfig = 0;
    // Wireless Devices Specification [spec_290], Chapter 6.2 Maintenance Port
    // By default WirelessHART field devices should be configured for polling address 1.
    g_stProv.m_sWiredHart.m_Device.m_PollingAddr.m_AddrBits = 0x01;

    // Load default Device Variable properties (just the custom dev. vars. will be affected - just the ones saved in NV memory)
    memcpy((uint8 *)g_stProv.m_astDeviceVars, (uint8 *)g_astNVDevVarInitData, sizeof(g_stProv.m_astDeviceVars));

    //Load default battery data
    memset( (void*) &g_stProv.m_stRadioCharge, 0, sizeof(g_stRadioCharge));
    memset( (void*) &g_stProv.m_stMSSystemCharge, 0, sizeof(g_stMSSystemCharge));
    memset( (void*) &g_stProv.m_stTotalRadioCharge, 0, sizeof(g_stTotalRadioCharge));

    // Load default sensor values
    memset((uint8 *)g_stProv.m_astDeviceVarsValues, 0, sizeof(g_stProv.m_astDeviceVarsValues));
    
    // Set LoopCurrent as not used
    NotUsedValue.m_ucValue[0]=0x00;
    NotUsedValue.m_ucValue[1]=0x00;
    NotUsedValue.m_ucValue[2]=0xA0;
    NotUsedValue.m_ucValue[3]=0x7F;
    g_stProv.m_astDeviceVarsValues[8].m_stValue.m_fValue = NotUsedValue.m_fValue;     //(float)0x7FA00000;
    g_stProv.m_astDeviceVarsValues[8].m_stLastValue.m_fValue = NotUsedValue.m_fValue; //(float)0x7FA00000;

    g_stProv.m_aucHartDynVarsAssignCodes[PV_IDX] = DEVICE_VARIABLE_0;   //Material thickness raw
    g_stProv.m_aucHartDynVarsAssignCodes[SV_IDX] = DEVICE_VARIABLE_1;   // Material thickness tc
    g_stProv.m_aucHartDynVarsAssignCodes[TV_IDX] = DEVICE_VARIABLE_2;   // Material temperature
    g_stProv.m_aucHartDynVarsAssignCodes[QV_IDX] = DEVICE_VARIABLE_3;   // % Battery life
    
    g_stProv.signature = sGoodSignature;
    
    memcpy(g_aucFlashCopy, (uint8*)&g_stProv, sizeof(g_stProv));
    FLASH_UPDATE_NEEDED();
    
}

/**
* @brief  This function checks if data stored in the NVM is valid or not. Validity is given by the NVM signature stored in the first page.
*/
Boolean SignatureAreaGood()
{
  uint8 aucRead[4] = {0};
  uint32 MEM_valid = 0;

  memcpy(aucRead, g_aucFlashCopy, 4);
  MEM_valid |= aucRead[3];
  MEM_valid <<= 8;
  MEM_valid |= aucRead[2];
  MEM_valid <<= 8;
  MEM_valid |= aucRead[1];
  MEM_valid <<= 8;
  MEM_valid |= aucRead[0];  

  return (Boolean)(MEM_valid == MEM_SIGNATURE);
}

