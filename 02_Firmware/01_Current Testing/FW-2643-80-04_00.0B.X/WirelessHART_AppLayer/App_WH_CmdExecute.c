/*************************************************************************
 * File:
 * Description:
 *************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../Global/provision.h"
#include "../Global/SoftwareCrc.h"
#include "../Platform/nvm.h"
#include "../Platform/gpio.h"
//#include "../Platform/app_init.h"
#include "../MS/MS_Data.h"
#include "../MS/MS_Process.h"
#include "../WirelessHART_Full_API/wh_full_api.h"
#include "../WiredHart/wired_hart_link_layer.h"
#include "App_WH_CmdExecute.h"
#include "App_WH_Tables.h"
#include "App_WH_Types.h"
#include "App_WH_User.h"
#include "../Platform/measurement.h"
#include "../Platform/GlobalVariables.h"
#ifdef PUBLISH_MONOTONIC
extern volatile uint32 g_ulMonotonic; // 4 bytes monotonic variable
#endif

NOT_USED_VALUE NotUsedValue;

extern HRTUT_parameters HRT_Para;
extern MBUT_parameters MB_Para;



#define LOOP_HIGH 0
#define ANALOG_CHANNEL_FLAGS 0
#define NO_CORRECTION        0
#define NEEDED_CORRECTION    1
#define ADJUST_UPDATE_TIME(arg) ((arg<=59999)?((arg<=31999)?((arg<=15999)?((arg<=7999)?\
                                ((arg<=3999)?((arg<=1999)?((arg<=999)?((arg<=499)?\
                                ((arg<=249)?100:250):500):1000):2000):4000):8000):16000):32000):60000)

uint8 GetClassificationFromCode(uint8 ucDevCode);
uint8 GetUnitsFromCode(uint8 ucDevCode);
uint8 UpdatePeriodCorrection(uint32 p_unUpdatePeriod, uint32 *p_unCorrectedVal);



/**
 * @brief Function that gets the classification from Device Variable Code.
 *
 * Used for Burst Conflict condition on commands 9 and 33.
 */

uint8 GetClassificationFromCode(uint8 p_ucDevCode)
{
uint8 ucCodeIndex = 0;

    if (p_ucDevCode >= CST_DEV_VARS_NO)
    { // NOT Device Variable Code
        if ((p_ucDevCode >= BatteryLife ) && (p_ucDevCode <= QuaternaryVariable ))
        { // HART Standardized variables
            if (p_ucDevCode <= LoopCurrent)
            { // BatteryLife, PercentRange, LoopCurrent
                ucCodeIndex = p_ucDevCode;
            }
            else
            { // PV, SV, TV or QV
                ucCodeIndex = p_ucDevCode - PrimaryVariable;
                switch (g_stProv.m_aucHartDynVarsAssignCodes[ucCodeIndex])
                {
                case DEVICE_VARIABLE_0:
                case DEVICE_VARIABLE_1:
                case DEVICE_VARIABLE_2:
                case DEVICE_VARIABLE_3:
                case DEVICE_VARIABLE_4:
                case DEVICE_VARIABLE_5:
                    ucCodeIndex = g_stProv.m_aucHartDynVarsAssignCodes[ucCodeIndex];
                    break;
                case BatteryLife:
                    ucCodeIndex = DEVICE_VARIABLE_4;
                    break;
                default:
                    return DeviceVariableClassificationCodes_NotUsed;
                }
            }
        }
        else
        { // p_ucCode ... between CST_DEV_VARS_NO and BatteryLife ... or ... > QuaternaryVariable => illegal values
            return DeviceVariableClassificationCodes_NotUsed;
        }
    }
    else
    { // Device Variable Code
        ucCodeIndex = p_ucDevCode;
    }

    switch(ucCodeIndex)
    {
    case DEVICE_VARIABLE_0:
        return DeviceVariableClassificationCodes_DeviceVariableNotClassified;
    case DEVICE_VARIABLE_1:
        return DeviceVariableClassificationCodes_Time;
    case DEVICE_VARIABLE_2:
        return DeviceVariableClassificationCodes_DeviceVariableNotClassified;
    case DEVICE_VARIABLE_3:
        return DeviceVariableClassificationCodes_DeviceVariableNotClassified;
    case DEVICE_VARIABLE_4:
        return DeviceVariableClassificationCodes_Time;
    case DEVICE_VARIABLE_5:
        return DeviceVariableClassificationCodes_DeviceVariableNotClassified;
    case BatteryLife:
        return DeviceVariableClassificationCodes_Time;
    case PercentRange:
        return DeviceVariableClassificationCodes_DeviceVariableNotClassified;
    case LoopCurrent:
        return DeviceVariableClassificationCodes_DeviceVariableNotClassified;
    default:
        return DeviceVariableClassificationCodes_NotUsed;
    }
}


/**
 * @brief Function that gets the units from Device Variable Code.
 *
 * Used for Burst Conflict condition on commands 9 and 33.
 *
 */
uint8 GetUnitsFromCode(uint8 p_ucDevCode)
{
uint8 ucCodeIndex = 0;

    if (p_ucDevCode >= CST_DEV_VARS_NO)
    { // NOT Device Variable Code
        if ((p_ucDevCode >= BatteryLife ) && (p_ucDevCode <= QuaternaryVariable ))
        { // HART Standardized variables
            if (p_ucDevCode <= LoopCurrent)
            { // BatteryLife, PercentRange, LoopCurrent
                ucCodeIndex = p_ucDevCode;
            }
            else
            { // PV, SV, TV or QV
                ucCodeIndex = p_ucDevCode - PrimaryVariable;
                switch (g_stProv.m_aucHartDynVarsAssignCodes[ucCodeIndex])
                {
                case DEVICE_VARIABLE_0:
                case DEVICE_VARIABLE_1:
                case DEVICE_VARIABLE_2:
                case DEVICE_VARIABLE_3:
                case DEVICE_VARIABLE_4:
                case DEVICE_VARIABLE_5:
                    ucCodeIndex = g_stProv.m_aucHartDynVarsAssignCodes[ucCodeIndex];
                    break;
                case BatteryLife:
                    ucCodeIndex = DEVICE_VARIABLE_4;
                    break;
                default:
                    return UnitsCodes_NotUsed;
                }
            }
        }
        else
        { // p_ucCode ... between CST_DEV_VARS_NO and BatteryLife ... or ... > QuaternaryVariable => illegal values
            return UnitsCodes_NotUsed;
        }
    }
    else
    { // Device Variable Code
        ucCodeIndex = p_ucDevCode;
    }

    switch(ucCodeIndex)
    {
    case DEVICE_VARIABLE_0:
        return g_stProv.m_astDeviceVars[0].m_ucCode;
    case DEVICE_VARIABLE_1:
        return UnitsCodes_Hours;
    case DEVICE_VARIABLE_2:
        return UnitsCodes_None;
    case DEVICE_VARIABLE_3:
        return UnitsCodes_MilsPerYear;
    case DEVICE_VARIABLE_4:
        return UnitsCodes_Days;
    case DEVICE_VARIABLE_5:
        return UnitsCodes_Hours;
    case BatteryLife:
        return UnitsCodes_Days;
    case PercentRange:
        return UnitsCodes_Percent;
    case LoopCurrent:
        return UnitsCodes_NotUsed;
    default:
        return UnitsCodes_NotUsed;
    }
}


/**
 * @brief Updates period of a burst message.
 * @param[in] p_unUpdatePeriod  - Period of a burst message.
 * @param[in] *p_unCorrectedVal - Pointer to the corrected value of the burst message.
 */
uint8 UpdatePeriodCorrection(uint32 p_unUpdatePeriod, uint32 *p_unCorrectedVal)
{
    p_unUpdatePeriod /=  32; // switch to miliSeconds here

    switch(p_unUpdatePeriod)
    {
    case 100: // 100 ms
        *p_unCorrectedVal = 100;
        return NO_CORRECTION;
    case 250: // 250 ms
        *p_unCorrectedVal = 250;
        return NO_CORRECTION;
    case 500: // 500 ms,
        *p_unCorrectedVal = 500;
        return NO_CORRECTION;
    case 1000: // 1000 ms
        *p_unCorrectedVal = 1000;
        return NO_CORRECTION;
    case 2000: // 2000 ms
        *p_unCorrectedVal = 2000;
        return NO_CORRECTION;
        break;
    case 4000: // 4000 ms,
        *p_unCorrectedVal = 4000;
        return NO_CORRECTION;
    case 8000: // 8000 ms
        *p_unCorrectedVal = 8000;
        return NO_CORRECTION;
    case 16000: // 16000 ms
        *p_unCorrectedVal = 16000;
        return NO_CORRECTION;
    case 32000: // 32000 ms
        *p_unCorrectedVal = 32000;
        return NO_CORRECTION;
    case 60000: // 60000 ms
        *p_unCorrectedVal = 60000;
        return NO_CORRECTION;
    case 3600000: // 3600000 ms
        *p_unCorrectedVal = 3600000;
        return NO_CORRECTION;
    }

    if (p_unUpdatePeriod > 3600000) // value > 3600 s not allowed
    {
        *p_unCorrectedVal = 3600000;
        return NEEDED_CORRECTION;
    }

    if ((60000 < p_unUpdatePeriod) && (p_unUpdatePeriod < 3600000))
    {
        uint8 ucMod = p_unUpdatePeriod % 1000;
        *p_unCorrectedVal = p_unUpdatePeriod - ucMod; // only Seconds values
        if (ucMod)
        {
            return NEEDED_CORRECTION;
        }
        else
        {
            return NO_CORRECTION;
        }
    }

    *p_unCorrectedVal = ADJUST_UPDATE_TIME(p_unUpdatePeriod);
    return NEEDED_CORRECTION;
}



//
// C001
//
/**
 * @brief Executes Cmd001.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C001_ReadPrimaryVariable(C001_ReadPrimaryVariable_Req *p_pRequest, C001_ReadPrimaryVariable_Resp *p_pResponse, uint8 p_ucCmdType)
{
NV_DEVICE_VARIABLE_T* pstDevVar;
DEVICE_VARIABLE_T*    pstDevVarValue;

    // The STD PV
    if (((pstDevVar = App_GetDeviceVar(PrimaryVariable)) != NULL) && ((pstDevVarValue =  App_GetDeviceVarValue(PrimaryVariable)) != NULL))
    { // Populate the response structure
        p_pResponse->m_ucPrimaryVarUnit  = (UnitsCodes)(pstDevVar->m_ucUnit);
        p_pResponse->m_fPrimaryVar       = pstDevVarValue->m_stValue.m_fValue;
        
        if (pstDevVarValue->m_ucNormalOperation == WriteDeviceVariableCodes_FixedValue)
        { // Device Variable will be temporarily forced to a fixed value
            return RCM_W08_UpdateFailure;
        }
    }

    //#ifdef PUBLISH_MONOTONIC
    //    p_pResponse->m_unMonotonic = g_ulMonotonic;
    //#endif
    return RCS_N00_Success;
}



//
// C002
//
/**
 * @brief Executes Cmd002.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C002_ReadLoopCurrAndPercentRange(C002_ReadLoopCurrentAndPercentOfRange_Req *p_pRequest, C002_ReadLoopCurrentAndPercentOfRange_Resp *p_pResponse, uint8 p_ucCmdType)
{
DEVICE_VARIABLE_T* pstDevVarValue;

    NotUsedValue.m_ucValue[0]=0x00;
    NotUsedValue.m_ucValue[1]=0x00;
    NotUsedValue.m_ucValue[2]=0xA0;
    NotUsedValue.m_ucValue[3]=0x7F;

    p_pResponse->m_fPrimaryVariableLoopCurrent = NotUsedValue.m_fValue;
    if ((pstDevVarValue =  App_GetDeviceVarValue(PercentRange)) != NULL)
    {
        p_pResponse->m_fPrimaryVariablePercentOfRange = pstDevVarValue->m_stValue.m_fValue;
    }
    else
    {
        return RCS_E06_DeviceSpecificCommandError;
    }

    if (pstDevVarValue->m_ucNormalOperation == WriteDeviceVariableCodes_FixedValue)
    { // Device Variable will be temporarily forced to a fixed value
        return RCM_W08_UpdateFailure;
    }

    return RCS_N00_Success;
}



//
// C003
//
/**
 * @brief Executes Cmd003.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C003_ReadDynamicVarsAndLoopCurrent(C003_ReadDynamicVariablesAndLoopCurrent_Req *p_pRequest, C003_ReadDynamicVariablesAndLoopCurrent_Resp *p_pResponse, uint8 p_ucCmdType)
{
DEVICE_VARIABLE_T* pstDevVarValue;
NV_DEVICE_VARIABLE_T* pstDevVar;
uint8 ucOperationMode; // Normal or forced to a fixed value?

    NotUsedValue.m_ucValue[0]=0x00;
    NotUsedValue.m_ucValue[1]=0x00;
    NotUsedValue.m_ucValue[2]=0xA0;
    NotUsedValue.m_ucValue[3]=0x7F;
    pstDevVar       = NULL;
    pstDevVarValue  = NULL;
    ucOperationMode = RCS_N00_Success;

    p_pResponse->m_fLoopCurrent = NotUsedValue.m_fValue;

    // The STD PV
    if (((pstDevVar = App_GetDeviceVar(PrimaryVariable)) != NULL) && ((pstDevVarValue =  App_GetDeviceVarValue(PrimaryVariable)) != NULL))
    { // Populate the response structure
        p_pResponse->m_ucPvUnitsCode  = (UnitsCodes)(pstDevVar->m_ucUnit);
        p_pResponse->m_PvValue        = pstDevVarValue->m_stValue.m_fValue;

        if (pstDevVarValue->m_ucNormalOperation == WriteDeviceVariableCodes_FixedValue)
        { // Device Variable will be temporarily forced to a fixed value
            ucOperationMode = RCM_W08_UpdateFailure;
        }
    }
    else
    {
        return RCS_E06_DeviceSpecificCommandError;
    }

    // The STD SV
    if (((pstDevVar = App_GetDeviceVar(SecondaryVariable)) != NULL) && ((pstDevVarValue =  App_GetDeviceVarValue(SecondaryVariable)) != NULL))
    { // Populate the response structure
        p_pResponse->m_ucSvUnitsCode  = (UnitsCodes)(pstDevVar->m_ucUnit);
        p_pResponse->m_SvValue        = pstDevVarValue->m_stValue.m_fValue;

        if (pstDevVarValue->m_ucNormalOperation == WriteDeviceVariableCodes_FixedValue)
        { // Device Variable will be temporarily forced to a fixed value
            ucOperationMode = RCM_W08_UpdateFailure;
        }
    }
    else
    {
        return RCS_E06_DeviceSpecificCommandError;
    }

    // The STD TV
    if (((pstDevVar = App_GetDeviceVar(TertiaryVariable)) != NULL) && ((pstDevVarValue =  App_GetDeviceVarValue(TertiaryVariable)) != NULL))
    { // Populate the response structure
        p_pResponse->m_ucTvUnitsCode  = (UnitsCodes)(pstDevVar->m_ucUnit);
        p_pResponse->m_TvValue        = pstDevVarValue->m_stValue.m_fValue;

        if (pstDevVarValue->m_ucNormalOperation == WriteDeviceVariableCodes_FixedValue)
        { // Device Variable will be temporarily forced to a fixed value
            ucOperationMode = RCM_W08_UpdateFailure;
        }
    }
    else
    {
        return RCS_E06_DeviceSpecificCommandError;
    }

    // The STD QV
    if (((pstDevVar = App_GetDeviceVar(QuaternaryVariable)) != NULL) && ((pstDevVarValue =  App_GetDeviceVarValue(QuaternaryVariable)) != NULL))
    { // Populate the response structure
        p_pResponse->m_ucQvUnitsCode  = (UnitsCodes)(pstDevVar->m_ucUnit);
        p_pResponse->m_QvValue        = pstDevVarValue->m_stValue.m_fValue;

        if (pstDevVarValue->m_ucNormalOperation == WriteDeviceVariableCodes_FixedValue)
        { // Device Variable will be temporarily forced to a fixed value
            ucOperationMode = RCM_W08_UpdateFailure;
        }
    }
    else
    {
        return RCS_E06_DeviceSpecificCommandError;
    }

    return (CommandResponseCodes)ucOperationMode;
}



//
// C006
//
/**
 * @brief Executes Cmd006.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C006_WritePollingAddress(C006_WritePollingAddress_Req *p_pRequest, C006_WritePollingAddress_Resp *p_pResponse, uint8 p_ucCmdType)
{
    if ( Wired_HART_WritePollingAddress(p_pRequest->m_ucPollingAddress, p_pRequest->m_ucLoopCurrentMode) )
        return RCS_E01_Undefined1;

    // Populate the response structure
    p_pResponse->m_ucPollingAddress  = p_pRequest->m_ucPollingAddress;
    p_pResponse->m_ucLoopCurrentMode =   0; // allways Zero = p_pRequest->m_ucLoopCurrentMode; FCG requested this in mail to Toma

    return RCS_N00_Success;
}



//
// C007
//
/**
 * @brief Executes Cmd007.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C007_ReadLoopConfiguration(C007_ReadLoopConfiguration_Req *p_pRequest, C007_ReadLoopConfiguration_Resp *p_pResponse, uint8 p_ucCmdType)
{
    // Populate the response structure
    p_pResponse->m_ucPollingAddress  = 0 | g_stProv.m_sWiredHart.m_Device.m_PollingAddr.m_AddrBits;
    p_pResponse->m_ucLoopCurrentMode = 0;

    return RCS_N00_Success;
}



//
// C008
//
/**
 * @brief Executes Cmd008.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C008_ReadDynamicVarsClassif(C008_ReadDynamicVariableClassification_Req *p_pRequest, C008_ReadDynamicVariableClassification_Resp *p_pResponse, uint8 p_ucCmdType)
{
NV_DEVICE_VARIABLE_T * pstDevVar;

    // Primary Variable
    pstDevVar = App_GetDeviceVar(PrimaryVariable);
    if(pstDevVar != NULL)
    { // Device Variable Code OK
        p_pResponse->m_ucPrimaryVariableClassification = (DeviceVariableClassificationCodes)pstDevVar->m_ucClassification;
    }
    else
    { // Device Variable Code not OK
        p_pResponse->m_ucPrimaryVariableClassification = DeviceVariableClassificationCodes_NotUsed;
    }

    // Secondary Variable
    pstDevVar = App_GetDeviceVar(SecondaryVariable);
    if(pstDevVar != NULL)
    { // Device Variable Code OK
        p_pResponse->m_ucSecondaryVariableClassification = (DeviceVariableClassificationCodes)pstDevVar->m_ucClassification;
    }
    else
    { // Device Variable Code not OK
        p_pResponse->m_ucSecondaryVariableClassification = DeviceVariableClassificationCodes_NotUsed;
    }

    // Tertiary Variable
    pstDevVar = App_GetDeviceVar(TertiaryVariable);
    if(pstDevVar != NULL)
    { // Device Variable Code OK
        p_pResponse->m_ucTertiaryVariableClassification = (DeviceVariableClassificationCodes)pstDevVar->m_ucClassification;
    }
    else
    { // Device Variable Code not OK
        p_pResponse->m_ucTertiaryVariableClassification = DeviceVariableClassificationCodes_NotUsed;
    }

    // Quaternary Variable
    pstDevVar = App_GetDeviceVar(QuaternaryVariable);
    if(pstDevVar != NULL)
    { // Device Variable Code OK
        p_pResponse->m_ucQuaternaryVariableClassification = (DeviceVariableClassificationCodes)pstDevVar->m_ucClassification;
    }
    else
    { // Device Variable Code not OK
        p_pResponse->m_ucQuaternaryVariableClassification = DeviceVariableClassificationCodes_NotUsed;
    }

    return RCS_N00_Success;
}



//
// C009
//
/**
 * @brief Executes Cmd009.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C009_ReadDeviceVarsWithStatus(C009_ReadDeviceVarsWithStatus_Req *p_pRequest, C009_ReadDeviceVarsWithStatus_Resp *p_pResponse, uint8 p_ucCmdType)
{
NV_DEVICE_VARIABLE_T* pstDevVar;
DEVICE_VARIABLE_T*    pstDevVarValue;
uint8 ucIdx;
uint8 ucOperationMode; // Normal or forced to a fixed value?

    NotUsedValue.m_ucValue[0]=0x00;
    NotUsedValue.m_ucValue[1]=0x00;
    NotUsedValue.m_ucValue[2]=0xA0;
    NotUsedValue.m_ucValue[3]=0x7F;

    // Echo back the payoad size
    p_pResponse->m_ucNoDeviceVariables = p_pRequest->m_ucNoDeviceVariables;
    p_pResponse->m_ucExtendedFieldDeviceStatus = 0x00;
    ucOperationMode = RCS_N00_Success;

    for (ucIdx = 0; ucIdx < p_pRequest->m_ucNoDeviceVariables; ucIdx++)
    {
        p_pResponse->m_astSlot[ucIdx].m_ucCode = p_pRequest->m_aucDeviceVarCode[ucIdx];
        if (((pstDevVar = App_GetDeviceVar(p_pRequest->m_aucDeviceVarCode[ucIdx])) != NULL) && ((pstDevVarValue =  App_GetDeviceVarValue(p_pRequest->m_aucDeviceVarCode[ucIdx])) != NULL))
        { // The Device Variable with the p_pRequest->m_aucDeviceVarCode[ucIdx] was found, compose the response structure
            p_pResponse->m_astSlot[ucIdx].m_ucClassification  = (DeviceVariableClassificationCodes)(pstDevVar->m_ucClassification);
            p_pResponse->m_astSlot[ucIdx].m_ucUnit            = (UnitsCodes)(pstDevVar->m_ucUnit);
            p_pResponse->m_astSlot[ucIdx].m_fValue            = pstDevVarValue->m_stValue.m_fValue;
            p_pResponse->m_astSlot[ucIdx].m_ucStatus          = pstDevVar->m_ucStatus;

            if (pstDevVarValue->m_ucNormalOperation == WriteDeviceVariableCodes_FixedValue)
            { // Device Variable will be temporarily forced to a fixed value
                ucOperationMode = RCM_W08_UpdateFailure;
            }
        }
        else
        {
            p_pResponse->m_astSlot[ucIdx].m_ucClassification  = DeviceVariableClassificationCodes_DeviceVariableNotClassified;
            p_pResponse->m_astSlot[ucIdx].m_ucUnit            = UnitsCodes_NotUsed;
            p_pResponse->m_astSlot[ucIdx].m_fValue            = NotUsedValue.m_fValue;
            p_pResponse->m_astSlot[ucIdx].m_ucStatus.process_quality = ProcessDataQuality_Bad;
            p_pResponse->m_astSlot[ucIdx].m_ucStatus.limit_status = LimitStatus_Constant;
            p_pResponse->m_astSlot[ucIdx].m_ucStatus.more_status_available = 0;
            p_pResponse->m_astSlot[ucIdx].m_ucStatus.device_family_status = 0;
        }
    }

    #ifdef PUBLISH_MONOTONIC
        p_pResponse->m_stSlot0DataTimeStamp.u32 = g_ulMonotonic * 320; // timestamp
    #endif

    return (CommandResponseCodes)ucOperationMode;
}



//
// C014
//
/**
 * @brief Executes Cmd014.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C014_ReadPrimaryVarTransducerInfo(C014_ReadDynamicVariableClassification_Req *p_pRequest, C014_ReadDynamicVariableClassification_Resp *p_pResponse, uint8 p_ucCmdType)
{
NV_DEVICE_VARIABLE_T * pstDevVar;
    // Primary Variable
    pstDevVar = App_GetDeviceVar(PrimaryVariable);
    if(pstDevVar != NULL)
    { // Device Variable Code OK
        p_pResponse->m_ulTransducerSerialNumber = pstDevVar->m_ulTransducerSN;
        p_pResponse->m_ucUnitsCode = (UnitsCodes)(pstDevVar->m_ucUnit);
        p_pResponse->m_fUpperTransducerLimit = pstDevVar->m_fUpperLimit;
        p_pResponse->m_fLowerTransducerLimit = pstDevVar->m_fLowerLimit;
        p_pResponse->m_fMinimumSpan = pstDevVar->m_fMinSpan;
    }
    else
    { // Device Variable Code not OK
        return RCS_E02_InvalidSelection;
    }

    return RCS_N00_Success;
}



//
// C015
//
/**
 * @brief Executes Cmd015.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C015_ReadDeviceInfo(C015_ReadDeviceInfo_Req *p_pRequest, C015_ReadDeviceInfo_Resp *p_pResponse, uint8 p_ucCmdType)
{
NV_DEVICE_VARIABLE_T * pstDevVar;
    // Primary Variable
    pstDevVar = App_GetDeviceVar(PrimaryVariable);
    if(pstDevVar != NULL)
    { // Device Variable Code OK
        p_pResponse->pvAlarmSelectionCodes = (AlarmSelectionCodes)LOOP_HIGH;
        p_pResponse->pvTransferFunctionCode = TransferFunctionCodes_Linear;
        p_pResponse->pvUpperLowerRangeValuesUnitsCode = (UnitsCodes) pstDevVar->m_ucUnit;
        p_pResponse->pvUpperRangeValue = pstDevVar->m_fUpperLimit;
        p_pResponse->pvLowerRangeValue = pstDevVar->m_fLowerLimit;
        p_pResponse->pvDampingValue = pstDevVar->m_fDampingValue;
        p_pResponse->writeProtectCode = WriteProtectCodes_NotUsed;
        p_pResponse->reservedValue = 250;
        p_pResponse->pvAnalogChannelFlags = ANALOG_CHANNEL_FLAGS;
    }
    else
    { // Device Variable Code not OK
        return RCS_E02_InvalidSelection;
    }

    return RCS_N00_Success;
}



//
// C033
//
/**
 * @brief Executes Cmd033.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C033_ReadDeviceVars(C033_ReadDeviceVars_Req *p_pRequest, C033_ReadDeviceVars_Resp *p_pResponse, uint8 p_ucCmdType)
{
NV_DEVICE_VARIABLE_T* pstDevVar;
DEVICE_VARIABLE_T*    pstDevVarValue;
uint8 ucIdx;

    NotUsedValue.m_ucValue[0]=0x00;
    NotUsedValue.m_ucValue[1]=0x00;
    NotUsedValue.m_ucValue[2]=0xA0;
    NotUsedValue.m_ucValue[3]=0x7F;

    // Echo back the payoad size
    p_pResponse->m_ucNoDeviceVariables = p_pRequest->m_ucNoDeviceVariables;

    for (ucIdx = 0; ucIdx < p_pRequest->m_ucNoDeviceVariables; ucIdx++)
    {
        p_pResponse->m_astSlot[ucIdx].m_ucCode = p_pRequest->m_aucDeviceVarCode[ucIdx];
        if (((pstDevVar = App_GetDeviceVar(p_pRequest->m_aucDeviceVarCode[ucIdx])) != NULL) && ((pstDevVarValue =  App_GetDeviceVarValue(p_pRequest->m_aucDeviceVarCode[ucIdx])) != NULL))
        { // The Device Variable with the p_pRequest->m_aucDeviceVarCode[ucIdx] was found, compose the response structure
            p_pResponse->m_astSlot[ucIdx].m_ucUnit            = (UnitsCodes)(pstDevVar->m_ucUnit);
            p_pResponse->m_astSlot[ucIdx].m_fValue            = pstDevVarValue->m_stValue.m_fValue;;
        }
        else
        {
            p_pResponse->m_astSlot[ucIdx].m_ucUnit            = UnitsCodes_NotUsed;
            p_pResponse->m_astSlot[ucIdx].m_fValue            = NotUsedValue.m_fValue;
        }
    }

    return RCS_N00_Success;
}


//
// C035
//
/**
 * @brief Executes Cmd035.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C035_WritePVRangeValues(C035_WritePVRangeValues_Req *p_pRequest, C035_WritePVRangeValues_Resp *p_pResponse, uint8 p_ucCmdType)
{
    g_stProv.m_astDeviceVars[PV_IDX].m_fLowerLimit = p_pRequest->m_fLowerRangeValue;
    g_stProv.m_astDeviceVars[PV_IDX].m_fUpperLimit = p_pRequest->m_fUpperRangeValue;

    FLASH_UPDATE_NEEDED();
                
    p_pResponse->m_ucUnitsCode = p_pRequest->m_ucUnitsCode;
    p_pResponse->m_fUpperRangeValue = p_pRequest->m_fUpperRangeValue;
    p_pResponse->m_fLowerRangeValue = p_pRequest->m_fLowerRangeValue;
    
    return RCS_N00_Success;
}


//
// C044
//
/**
 * @brief Executes Cmd044.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C044_WritePVUnits(C044_WritePVUnits_Req *p_pRequest, C044_WritePVUnits_Resp *p_pResponse, uint8 p_ucCmdType)
{
    g_stProv.m_astDeviceVars[PV_IDX].m_ucUnit = p_pRequest->m_ucUnitsCode;
    FLASH_UPDATE_NEEDED();
    
    p_pResponse->m_ucUnitsCode  = p_pRequest->m_ucUnitsCode;
    
    return RCS_N00_Success;
}


//
// C048
//
/**
 * @brief Executes Cmd048.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C048_ReadAdditionalDeviceStatus(C048_ReadAdditionalDeviceStatus_Req *p_pRequest, C048_ReadAdditionalDeviceStatus_Resp *p_pResponse, uint8 p_ucCmdType)
{
    uint8 ucCmd48Size = C048_SIZE_25; // all data will be sent for the moment
    uint8 x;
    uint8 i;

    // device must return the current values
    memcpy(p_pResponse, &g_stCmd48Data.data, ucCmd48Size);
    p_pResponse->commandSize = ucCmd48Size;

    // compare the request data bytes to the current value contained in the device
    if(ucCmd48Size > p_pRequest->commandSize)
    {
        return RCS_N00_Success;
    }

    if(p_pRequest->commandSize && memcmp(p_pRequest, &g_stCmd48Data.data, ucCmd48Size) )
    {
        return RCM_W14_StatusBytesMismatch;
    }

    
    x = 0;
    for ( i = 0; i < p_pRequest->commandSize; i++ )
    {
        x |= g_stCmd48Data.byte[i];
    }
    if ( !x )
    {
        g_ucDeviceStatus[MASTER_WISA] &= (~FieldDeviceStatusesMask_MoreStatusAvailable);
        g_ucDeviceStatus[MASTER_PRIMARY] &= (~FieldDeviceStatusesMask_MoreStatusAvailable);
        g_ucDeviceStatus[MASTER_SECONDARY] &= (~FieldDeviceStatusesMask_MoreStatusAvailable);
        return RCS_N00_Success;
    }


    // If here there is an exact match => clear MSA ...

    // reaching this point means exact match => reset More Status Available bit
    if (p_ucCmdType == DAQ_FW_WIRELESS)
    {
        g_ucDeviceStatus[MASTER_WISA] &= (~FieldDeviceStatusesMask_MoreStatusAvailable);
    }
    else if (g_stProv.m_sWiredHart.m_RxMasterBurst & HART_DELIMITER_ADDRESS_MASK)
    {
        g_ucDeviceStatus[MASTER_PRIMARY] &= (~FieldDeviceStatusesMask_MoreStatusAvailable);
    }
    else
    {
        g_ucDeviceStatus[MASTER_SECONDARY] &= (~FieldDeviceStatusesMask_MoreStatusAvailable);
    }

    // APP_ResetDeviceStatusFlag(srcAddr, control, RADIO_DEV_STATUS_MASK | AP_DEV_STATUS_MASK, MORE_STATUS_AVAILABLE);
    // inform WISA to clear MSA => [???] => to do if needed
    // SendMessage(STACK_SPECIFIC, HART_CLEAR_MORE_STATUS_AVAILABLE, API_REQ, 0, 0, NULL);

    return RCS_N00_Success;
}




//
// C050
//
/**
 * @brief Executes Cmd050.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C050_ReadDynamicVariableAssignments(C050_ReadDynamicVariableAssignments_Req *p_pRequest, C050_ReadDynamicVariableAssignments_Resp *p_pResponse, uint8 p_ucCmdType)
{
    // copy to response structure the g_stProv.m_aucHartDynVarsAssignCodes table
    memcpy((uint8*)p_pResponse,(uint8 *)g_stProv.m_aucHartDynVarsAssignCodes, MAX_CONFIGURABLE_DYN_VAR_NO);
    return RCS_N00_Success;
}



//
// C051
//
/**
 * @brief Executes Cmd051.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C051_WriteDynamicVariableAssignments(C051_WriteDynamicVariableAssignments_Req *p_pRequest, C051_WriteDynamicVariableAssignments_Resp *p_pResponse, uint8 p_ucCmdType)
{
    // p_ucNoReqDataBytes checked in parsing
    memcpy((uint8*)g_stProv.m_aucHartDynVarsAssignCodes,(uint8 *)(p_pRequest->m_aucVariables), p_pRequest->p_ucNoReqDataBytes);
    // Save to NVM - Dynamic Variables Assignment
    FLASH_UPDATE_NEEDED();
    // Respond with ALL Dyn. Vars. Assignments
    memcpy((uint8*)p_pResponse,(uint8 *)g_stProv.m_aucHartDynVarsAssignCodes, MAX_CONFIGURABLE_DYN_VAR_NO);
    return RCS_N00_Success;
}



//
// C054
//
/**
 * @brief Executes Cmd054.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C054_ReadDeviceVarsInfo(C054_ReadDeviceVarsInfo_Req *p_pRequest, C054_ReadDeviceVarsInfo_Resp *p_pResponse, uint8 p_ucCmdType)
{
NV_DEVICE_VARIABLE_T * pstDevVar;

    pstDevVar = App_GetDeviceVar(p_pRequest->m_ucDeviceVariableCode);

    if (pstDevVar != NULL)
    { // Device Variable Code OK
        p_pResponse->m_ucDeviceVariableCode = pstDevVar->m_ucCode;
        p_pResponse->m_ulTransducerSerialNumber = pstDevVar->m_ulTransducerSN;
        p_pResponse->m_ucUnitsCode = (UnitsCodes)(pstDevVar->m_ucUnit);
        p_pResponse->m_fUpperTransducerLimit = pstDevVar->m_fUpperLimit;
        p_pResponse->m_fLowerTransducerLimit = pstDevVar->m_fLowerLimit;
        p_pResponse->m_fDeviceVariableDampingValue = pstDevVar->m_fDampingValue;
        p_pResponse->m_fDeviceVariableMinimumSpan = pstDevVar->m_fMinSpan;
        p_pResponse->m_ucClassificationCode = (DeviceVariableClassificationCodes)(pstDevVar->m_ucClassification);
        p_pResponse->m_ucDeviceVariableFamily =(DeviceVariableFamilyCodes)(pstDevVar->m_ucFamily);
        p_pResponse->m_ulUpdateTimePeriod = pstDevVar->m_ulUpdateTimePeriod;
        p_pResponse->m_ucDevVarProp = 0x00;
    }
    else
    { // Device Variable Code not OK
        return RCS_E02_InvalidSelection;
    }

    return RCS_N00_Success;
}



//
// C059
//
/**
 * @brief Executes Cmd059.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C059_WriteNumberOfResponsePreambles(C059_WriteNumberOfResponsePreambles_Req *p_pRequest, C059_WriteNumberOfResponsePreambles_Resp *p_pResponse, uint8 p_ucCmdType)
{
CommandResponseCodes ucRC;

    ucRC = (CommandResponseCodes) Wired_HART_WriteNumberOfResponsePreambles(p_pRequest->m_ucNPreambles);
    if ( ucRC )
        return ucRC;

    // Populate the response structure
    p_pResponse->m_ucNPreambles  = p_pRequest->m_ucNPreambles;

    return RCS_N00_Success;
}



//
// C079
//
/**
 * @brief Executes Cmd079.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C079_WriteDeviceVariable(C079_WriteDeviceVariable_Req *p_pRequest, C079_WriteDeviceVariable_Resp *p_pResponse, uint8 p_ucCmdType)
{
// CMD 79 allows a Device Variable to be temporarily forced to a fixed value
NV_DEVICE_VARIABLE_T* pstDevVar;
DEVICE_VARIABLE_T*    pstDevVarValue;

    if (((pstDevVar = App_GetDeviceVar(p_pRequest->m_ucDeviceVariableCode)) != NULL) &&
        ((pstDevVarValue =  App_GetDeviceVarValue(p_pRequest->m_ucDeviceVariableCode)) != NULL))
    { // Device Variable Code OK

        if (p_pRequest->m_ucWriteCommandCode == WriteDeviceVariableCodes_FixedValue)
        { // Device Variable will be temporarily forced to a fixed value
            pstDevVarValue->m_stValue.m_fValue      = p_pRequest->m_fValue;
            pstDevVarValue->m_stLastValue.m_fValue  = p_pRequest->m_fValue;
            pstDevVarValue->m_ucNormalOperation     = WriteDeviceVariableCodes_FixedValue;
            pstDevVar->m_ucUnit                     = p_pRequest->m_ucUnitsCode;
            pstDevVar->m_ucStatus                   = p_pRequest->m_ucDeviceVariableStatus;
        }
        else
        { // Only the units are changing, make sure that we convert the value
            pstDevVarValue->m_stValue.m_fValue      = ConvertToNewUnit(p_pRequest->m_ucDeviceVariableCode, pstDevVarValue->m_stValue.m_fValue, pstDevVar->m_ucUnit, p_pRequest->m_ucUnitsCode);
            pstDevVarValue->m_ucNormalOperation     = WriteDeviceVariableCodes_Normal;
            pstDevVar->m_ucUnit                     = p_pRequest->m_ucUnitsCode;
            //pstDevVar->m_ucStatus                   = p_pRequest->m_ucDeviceVariableStatus; // ??? bad status from DD ???

            FLASH_UPDATE_NEEDED();
        }
        // ??? double check
        // ??? do not update vals anymore?
        // Populate response structure
        p_pResponse->m_ucDeviceVariableCode   = p_pRequest->m_ucDeviceVariableCode;
        p_pResponse->m_ucWriteCommandCode     = (WriteDeviceVariableCodes)(pstDevVarValue->m_ucNormalOperation);
        p_pResponse->m_ucUnitsCode            = p_pRequest->m_ucUnitsCode;
        p_pResponse->m_fValue                 = pstDevVarValue->m_stValue.m_fValue;
        p_pResponse->m_ucDeviceVariableStatus = pstDevVar->m_ucStatus;
    }
    else
    { // Device Variable Code not OK
        return RCM_E10_InvalidWriteDeviceVariableCode;
    }

    return RCS_N00_Success;
}



//
// C103
//
/**
 * @brief Executes Cmd103.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C103_WriteBurstPeriod(C103_WriteBurstPeriod_Req *p_pRequest, C103_WriteBurstPeriod_Resp *p_pResponse, uint8 p_ucCmdType)
{
uint8 ucReturnPeriod,ucReturnMaxPeriod;
uint32 unCorrectedPeriod;
uint32 unCorrectedMaxPeriod;// local variables to save the correct time values into the response parameters
uint8 ucSourceAPHart=0;

    if ((p_pRequest->m_ucBurstMessage) >= MAX_BURST_MSG_NO)
        return RCM_E09_InvalidBurstMessage; // invalid burst message

    if ((g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ucSrvcReqStatus == SRVC_REQ_C799) || (g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ucSrvcReqStatus == SRVC_REQ_C801))
    { // Return error code
        return RCS_E32_Busy; // service request was sent, burst pending
    }

    if (p_ucCmdType == 0xA5)
    {
        p_ucCmdType = 0;
        ucSourceAPHart=1;
    }

    // check m_ulUpdatePeriod == 0 ???
    ucReturnPeriod = UpdatePeriodCorrection (p_pRequest->m_ulUpdatePeriod, &unCorrectedPeriod);             // correction to the update period
    ucReturnMaxPeriod = UpdatePeriodCorrection (p_pRequest->m_ulMaxUpdatePeriod, &unCorrectedMaxPeriod);    // correction to the max update period

    if (unCorrectedMaxPeriod < unCorrectedPeriod)
    {
        unCorrectedMaxPeriod = unCorrectedPeriod;
        ucReturnMaxPeriod |= 1;
    }

    p_pResponse->m_ucBurstMessage = p_pRequest->m_ucBurstMessage;
    p_pResponse->m_ulUpdatePeriod = unCorrectedPeriod * 32;
    p_pResponse->m_ulMaxUpdatePeriod = unCorrectedMaxPeriod * 32;

    if (g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ucSrvcModeControlCode == EnableOnTDMADataLinkLayer) // burst message enabled
    {
        if (g_ucAppProcessorStatus >= API_STATE_NOTIFY_JOIN_OK)
        { // the device has already joined the network
            if (g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ulUpdateTime != unCorrectedPeriod) // modified period
            {
                if (App_GetSrvcReq((uint16)CMDID_C103_WriteBurstPeriod, p_pResponse->m_ucBurstMessage, p_pResponse->m_ulUpdatePeriod, ucSourceAPHart) == API_SUCCESS) // request for service
                {
                    g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ucSrvcReqStatus = SRVC_REQ_C799; // Set SrvReq true
                    g_stRFBWService[p_pRequest->m_ucBurstMessage].m_unSrvcCommandNumber = CMDID_C103_WriteBurstPeriod; // put the value of the command Id into the g_ucCommandnumber
                    g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ulSrvcRequestPeriod = p_pResponse->m_ulUpdatePeriod;
                    g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ulMaxUpdateTime = unCorrectedMaxPeriod; // for now, save the max time
                    if (!g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse)
                    {
                        g_bNeedSendRsp = TRUE; // Set flag - allow sending a command response to the VN radio when the srvc response arrives
                    }
                    if (ucSourceAPHart)
                    {
                        API_Tx_Handler(1);
                    }
                    return RCS_N254_ResponseAck;
                }
                else
                {
                    return  RCS_N255_ResponseNack;
                }
            }
        } // if (g_ucAppProcessorStatus >= API_STATE_NOTIFY_JOIN_OK)
    }

    // update the current time values with the new ones
    if ((unCorrectedPeriod != g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ulUpdateTime) || (unCorrectedMaxPeriod != g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ulMaxUpdateTime))
    {
        g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ulUpdateTime = unCorrectedPeriod;
        g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ulMaxUpdateTime = unCorrectedMaxPeriod; // update the time values

        // Set configuration changed in Status Byte
        SET_CONFIGURATION_CHANGED();

        //save the new time values into the external NVM
        FLASH_UPDATE_NEEDED();
    }

    if ((ucReturnPeriod) || (ucReturnMaxPeriod))
    { //
        return RCM_W08_UpdateTimesAdjusted;
    }

    return RCS_N00_Success;
}



//
// C104
//
/**
 * @brief Executes Cmd104.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C104_WriteBurstTrigger(C104_WriteBurstTrigger_Req *p_pRequest, C104_WriteBurstTrigger_Resp *p_pResponse, uint8 p_ucCmdType)
{
uint8 ucLocalStatus=RCS_N00_Success;

    if( p_pRequest->m_ucBurstMessage >= MAX_BURST_MSG_NO)
        return RCM_E09_InvalidBurstMessage;

    // Common Table 33
    if (p_pRequest->m_ucSelectionCode > 4)
        return RCM_E13_InvalidBurstTriggerModeSelectionCode;

    // Common Table 21
    if (!(
           (p_pRequest->m_ucClassificationCode == 0)
             ||
           ( (p_pRequest->m_ucClassificationCode>=64) && (p_pRequest->m_ucClassificationCode<=111) )
        ) )
        return RCM_E11_InvalidDeviceVariableClassification;

    switch (p_pRequest->m_ucClassificationCode)
    {
        case DeviceVariableClassificationCodes_DeviceVariableNotClassified:
        case DeviceVariableClassificationCodes_Time:
        case DeviceVariableClassificationCodes_NotUsed:
            break;
        default:
            return RCM_E11_InvalidDeviceVariableClassification;
    }

    // Common Table 2
    switch (p_pRequest->m_ucUnitsCode)
    {
        case UnitsCodes_DegreesCelsius:
        case UnitsCodes_DegreesFahrenheit:
        case UnitsCodes_DegreesRankine:
        case UnitsCodes_Kelvin:
        case UnitsCodes_Minutes:
        case UnitsCodes_Seconds:
        case UnitsCodes_Hours:
        case UnitsCodes_Days:
        case UnitsCodes_Milliamperes:
        case UnitsCodes_Percent:
        case UnitsCodes_Millivolts:
        case UnitsCodes_Volts:
        case UnitsCodes_PartsPerMillion:
        case UnitsCodes_VolumePercent:
        case UnitsCodes_PercentLowerExplosionLevel:
        case UnitsCodes_PartsPerBillion:
        case UnitsCodes_MilsPerYear:
        case UnitsCodes_MMPerYear:
        case UnitsCodes_Millimeters:
        case UnitsCodes_Inches:
        case UnitsCodes_NotUsed:
        case UnitsCodes_None:
            break;
        default:
            return RCM_E12_InvalidUnitsCode;
    }

    if (p_pRequest->m_ulTriggerLevel >= 10000000.0f)
        return RCS_E03_PassedParameterTooLarge;

    if (p_pRequest->m_ulTriggerLevel <= -10000000.0f)
        return RCS_E04_PassedParameterTooSmall;

    *p_pResponse = *(C104_WriteBurstTrigger_Resp *)p_pRequest; // copy the information from request into response

    NV_BURST_T *pCrtBurst = g_stProv.m_stBurstNVM + p_pRequest->m_ucBurstMessage;

    // check if actual configuration is different from new configuration
    if  ((p_pRequest->m_ucSelectionCode != pCrtBurst->m_ucBurstTriggerModeCode)
            || (p_pRequest->m_ucClassificationCode != pCrtBurst->m_ucClassification)
            || (p_pRequest->m_ucUnitsCode != pCrtBurst->m_ucUnit)
            || (p_pRequest->m_ulTriggerLevel != pCrtBurst->m_fTriggerValue))
    {
        if( p_pRequest->m_ucSelectionCode > BurstMessageTriggerMode_OnChange )
        { // unsupported trigger mode -> continuous mode, value=0
            //  HCF-SPEC-151: "If the trigger mode is non-zero in Command 104 and the trigger source's Device Variable
            //  Classification does not match the new Slot 0 Device Variable the new values will be accepted and
            //  Response Code "Burst Condition Conflict" will be returned. The field device must correct the
            //  classification, unit codes, reset to Trigger Mode 0 and publish continuously at the Update Period
            //  until it receives another Command 104."
            pCrtBurst->m_ucBurstTriggerModeCode = BurstMessageTriggerMode_Continuous;
            pCrtBurst->m_fTriggerValue = 0;
            p_pResponse->m_ucSelectionCode = BurstMessageTriggerMode_Continuous;
            p_pResponse->m_ulTriggerLevel = 0;
            ucLocalStatus = RCM_E13_InvalidBurstTriggerModeSelectionCode;
        }
        else
        {
            pCrtBurst->m_ucBurstTriggerModeCode = p_pRequest->m_ucSelectionCode;
            pCrtBurst->m_fTriggerValue = p_pRequest->m_ulTriggerLevel;
        }

        pCrtBurst->m_ucClassification = p_pRequest->m_ucClassificationCode;
        pCrtBurst->m_ucUnit = p_pRequest->m_ucUnitsCode;

        // Set configuration changed in Status Byte
        SET_CONFIGURATION_CHANGED();
        
        // Save the new configuration in NVM
        FLASH_UPDATE_NEEDED();
        
    }

    return ucLocalStatus;
}



//
// C105
//
/**
 * @brief Executes Cmd105.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C105_ReadBurstModeConfiguration(C105_ReadBurstModeConfiguration_Req *p_pRequest, C105_ReadBurstModeConfiguration_Resp *p_pResponse, uint8 p_ucCmdType)
{
uint8 ucIndex;

    if( p_pRequest->m_ucBurstMessage >= MAX_BURST_MSG_NO )
        return RCM_E09_InvalidBurstMessage;

    ucIndex = p_pRequest->m_ucBurstMessage;
    p_pResponse->m_ucBurstModeControlCode = g_stProv.m_stBurstNVM[ucIndex].m_ucBurstModeControlCode;

    if (p_pRequest->m_unRequestSize == 1)
    {
        p_pResponse->m_ucCommandNumberExpansionFlag = 0x1F; // command number expansion flag=31
    }
    else
    {
        p_pResponse->m_ucCommandNumberExpansionFlag = g_stProv.m_stBurstNVM[ucIndex].m_unCmdNo & 0x00FF; // if there are no request data bytes then this byte must return the LSB of the burst command number
    }

    memcpy(&p_pResponse->m_aucDeviceVariableCodes, &g_stProv.m_stBurstNVM[ucIndex].m_aucDeviceVariableCodes, 8); // copy 8 bytes of data into response  the published variables(command 105)

    p_pResponse->m_ucBurstMessage = ucIndex;
    p_pResponse->m_ucMaxNumberOfBurstMessages = MAX_BURST_MSG_NO; // max number of bursts
    p_pResponse->m_unExtendedCommandNumber = g_stProv.m_stBurstNVM[ucIndex].m_unCmdNo; // the published  command number
    p_pResponse->m_ulUpdatePeriod = g_stProv.m_stBurstNVM[ucIndex].m_ulUpdateTime * 32; // normal update interval expressed in 1/32 msec
    p_pResponse->m_ulMaxUpdatePeriod = g_stProv.m_stBurstNVM[ucIndex].m_ulMaxUpdateTime * 32; // maximum update interval expressed in 1/32 msec
    p_pResponse->m_ucSelectionCode = (BurstMessageTriggerMode)(g_stProv.m_stBurstNVM[ucIndex].m_ucBurstTriggerModeCode); // triger mode, see table 33
    p_pResponse->m_ucClassificationCode = (DeviceVariableClassificationCodes)(g_stProv.m_stBurstNVM[ucIndex].m_ucClassification); // device variable classification for trigger value
    p_pResponse->m_ucUnitsCode = (UnitsCodes)(g_stProv.m_stBurstNVM[ucIndex].m_ucUnit); // units code
    p_pResponse->m_ulTriggerLevel = g_stProv.m_stBurstNVM[ucIndex].m_fTriggerValue; // trigger value

    return RCS_N00_Success;
}



//
// C107
//
/**
 * @brief Executes Cmd107.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C107_WriteBurstDeviceVars(C107_WriteBurstDeviceVars_Req *p_pRequest, C107_WriteBurstDeviceVars_Resp *p_pResponse, uint8 p_ucCmdType)
{
    if(p_pRequest->m_ucBurstMessage >= MAX_BURST_MSG_NO)
        return RCM_E09_InvalidBurstMessage;

    uint8 ucBurstIdx = p_pRequest->m_ucBurstMessage;
    uint8 ucLocalStatus;
    uint8 ucIdx;

     ucLocalStatus = RCS_N00_Success;

    // Check if the Device Variable Codes from the request are valid
    for (ucIdx = 0; ucIdx < p_pRequest->m_ucNoOfDeviceVariables; ucIdx++)
    {
        if ( ! ((p_pRequest->m_aDeviceVariableCode[ucIdx] <= PROV_ucMaxNoOfDeviceVars) ||
               ((p_pRequest->m_aDeviceVariableCode[ucIdx] >= BatteryLife ) && (p_pRequest->m_aDeviceVariableCode[ucIdx] <= QuaternaryVariable )) ||
               (p_pRequest->m_aDeviceVariableCode[ucIdx] == NotUsed))
            )
        {
            return RCS_E02_InvalidSelection;
        }
    }

    // If request Device Variable Codes different from the ones already assigned to the current burst
    if(memcmp(p_pRequest->m_aDeviceVariableCode, g_stProv.m_stBurstNVM[ucBurstIdx].m_aucDeviceVariableCodes, sizeof(p_pRequest->m_aDeviceVariableCode)))
    {
        //Set all Device Variable Codes for the specified burst to NotUsed
        memset((uint8 *)(g_stProv.m_stBurstNVM[ucBurstIdx].m_aucDeviceVariableCodes), 250, MAX_DEVICE_VARIABLES);
        //Set current burst Device Variable Codes
        memcpy((uint8 *)(g_stProv.m_stBurstNVM[ucBurstIdx].m_aucDeviceVariableCodes), (uint8 *)(p_pRequest->m_aDeviceVariableCode), p_pRequest->m_ucNoOfDeviceVariables);

        // Set configuration changed in Status Byte
        SET_CONFIGURATION_CHANGED();
    }

    // Check for "Burst Condition Conflict"
    // HCF-SPEC-151: "If the trigger mode is non-zero in Command 104 and the trigger source's Device Variable
    // Classification does not match the new Slot 0 Device Variable the new values will be accepted and
    // Response Code "Burst Condition Conflict" will be returned. The field device must correct the
    // classification, unit codes, reset to Trigger Mode 0 and publish continuously at the Update Period
    // until it receives another Command 104."
    if(g_stProv.m_stBurstNVM[ucBurstIdx].m_ucBurstTriggerModeCode != BurstMessageTriggerMode_Continuous)
    { // "the trigger mode is non-zero"
        if(g_stProv.m_stBurstNVM[ucBurstIdx].m_ucClassification != g_stProv.m_astDeviceVars[0].m_ucClassification)
        { // "the trigger source's Device Variable Classification does not match the new Slot 0 Device Variable"
            g_stProv.m_stBurstNVM[ucBurstIdx].m_ucClassification       = g_stProv.m_astDeviceVars[0].m_ucClassification;
            g_stProv.m_stBurstNVM[ucBurstIdx].m_ucUnit                 = g_stProv.m_astDeviceVars[0].m_ucUnit;
            g_stProv.m_stBurstNVM[ucBurstIdx].m_ucBurstTriggerModeCode = BurstMessageTriggerMode_Continuous;
            ucLocalStatus = RCM_W08_BurstConditionConflict;

            // Set configuration changed in Status Byte
            SET_CONFIGURATION_CHANGED();
        }
    }

    // Copy burst Device Variable Codes to response structure
    memcpy(p_pResponse->m_aDeviceVariableCode, g_stProv.m_stBurstNVM[ucBurstIdx].m_aucDeviceVariableCodes, p_pRequest->m_ucNoOfDeviceVariables);
    p_pResponse->m_ucBurstMessage = p_pRequest->m_ucBurstMessage;

    // Save burst structure to NVM
    FLASH_UPDATE_NEEDED();

    return ucLocalStatus;
}



//
// C108
//
/**
 * @brief Executes Cmd108.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C108_WriteBurstModeCommandNo(C108_WriteBurstModeCommandNo_Req *p_pRequest, C108_WriteBurstModeCommandNo_Resp *p_pResponse, uint8 p_ucCmdType)
{
uint8 ucLocalStatus = RCS_N00_Success;

    if(p_pRequest->m_ucBurstMessage >= MAX_BURST_MSG_NO)
        return RCM_E09_InvalidBurstMessage;

    // Check if Burst Mode Command Number is supported
    if( (p_pRequest->m_unCommandNumber != CMDID_C001_ReadPV)                         &&
        (p_pRequest->m_unCommandNumber != CMDID_C002_ReadLoopCurrAndPercentRange)    &&
        (p_pRequest->m_unCommandNumber != CMDID_C003_ReadDynamicVarsAndLoopCurrent)  &&
        (p_pRequest->m_unCommandNumber != CMDID_C009_ReadDeviceVarsWithStatus)       &&
        (p_pRequest->m_unCommandNumber != CMDID_C033_ReadDeviceVars)                 &&
        (p_pRequest->m_unCommandNumber != CMDID_C048_ReadAdditionalDeviceStatus)
       )
    {
        return RCS_E02_InvalidSelection;
    }

    if (p_pRequest->m_unCommandNumber != g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_unCmdNo)
    {
        // Set configuration changed in Status Byte
        SET_CONFIGURATION_CHANGED();

        // Set command number in burst structure
        g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_unCmdNo = p_pRequest->m_unCommandNumber;

        // Check for "Burst Condition Conflict"
        // HCF-SPEC-151: If the trigger mode is non-zero in Command 104 and the trigger source's Device Variable
        // Classification does not match for the new command number the new command number will be
        // accepted and Response Code "Burst Condition Conflict" will be returned. The field device must
        // correct the classification, unit codes, reset to Trigger Mode 0 and publish continuously at the Update
        // Period until it receives another Command 104.
        if(g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucBurstTriggerModeCode != BurstMessageTriggerMode_Continuous)
        {
            // Trigger sources for commands:
            // Command Number        Trigger Source
            //
            // Command 1                  PV
            // Command 2              PercentOfRange
            // Command 3                  PV
            // Command 9             Dev Var in Slot 0
            // Command 33            Dev Var in Slot 0
            // Command 48                 None
            switch(p_pRequest->m_unCommandNumber)
            {
            case CMDID_C001_ReadPV:
            case CMDID_C003_ReadDynamicVarsAndLoopCurrent:

                // Check if the current Device Variable Classification does not match the PV
                if(g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucClassification != GetClassificationFromCode(PrimaryVariable))
                {
                    // Correct the classification
                    g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucClassification = GetClassificationFromCode(PrimaryVariable);
                    // Correct the units code
                    g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucUnit = GetUnitsFromCode(PrimaryVariable);
                    // Reset to Trigger Mode 0
                    g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucBurstTriggerModeCode = BurstMessageTriggerMode_Continuous;
                    //
                    ucLocalStatus = RCM_W08_BurstConditionConflict;
                }
                break;

            case CMDID_C002_ReadLoopCurrAndPercentRange:
                // Check if the current Device Variable Classification does not match the PercentOfRange classification
                if(g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucClassification !=  GetClassificationFromCode(PercentRange))
                {
                    // Correct the classification
                    g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucClassification = GetClassificationFromCode(PercentRange);
                    // Correct the units code
                    g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucUnit = GetUnitsFromCode(PercentRange);
                    // Reset to Trigger Mode 0
                    g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucBurstTriggerModeCode = BurstMessageTriggerMode_Continuous;
                    //
                    ucLocalStatus = RCM_W08_BurstConditionConflict;
                }
                break;


            case CMDID_C009_ReadDeviceVarsWithStatus:
            case CMDID_C033_ReadDeviceVars:

                // Check if the current Device Variable Classification does not match the Dev Var in Slot 0
                if(g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucClassification != GetClassificationFromCode(g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_aucDeviceVariableCodes[0]))
                {
                    // Correct the classification
                    g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucClassification = GetClassificationFromCode(g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_aucDeviceVariableCodes[0]);
                    // Correct the units code
                    g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucUnit = GetUnitsFromCode(g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_aucDeviceVariableCodes[0]);
                    // Reset to Trigger Mode 0
                    g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucBurstTriggerModeCode = BurstMessageTriggerMode_Continuous;
                    //
                    ucLocalStatus = RCM_W08_BurstConditionConflict;
                }
                break;

            case CMDID_C048_ReadAdditionalDeviceStatus:
                // Check if the current Device Variable Classification does not match Not_Used
                if(g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucClassification != DeviceVariableClassificationCodes_NotUsed)
                {
                    // Correct the classification
                    g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucClassification = DeviceVariableClassificationCodes_NotUsed;
                    // Correct the units code
                    g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucUnit = UnitsCodes_None; // UnitsCodes_NotUsed;
                    // Reset to Trigger Mode 0
                    g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucBurstTriggerModeCode = BurstMessageTriggerMode_Continuous;
                    //
                    ucLocalStatus = RCM_W08_BurstConditionConflict;
                }
                break;
            }
        }

        // Save burst structure to NVM
        FLASH_UPDATE_NEEDED();
    }

    // Compose response
    memcpy(p_pResponse, p_pRequest, sizeof(C108_WriteBurstModeCommandNo_Req));
    return ucLocalStatus;
}



//
// C109
//
/**
 * @brief Executes Cmd109.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C109_BurstModeCtrl(C109_BurstModeCtrl_Req *p_pRequest, C109_BurstModeCtrl_Resp *p_pResponse, uint8 p_ucCmdType)
{
uint8 ucBurstMsg;
uint32 ulBurstPeriod; // this is the actual period that is required by WHART specification; it is obtained by multiplying the UpdateTime [within burst struct] with 32
uint8 ucSourceAPHart=0;

    if( p_pRequest->m_ucBurstMessage >= MAX_BURST_MSG_NO )
        return RCS_E02_InvalidSelection; // RCS_E02_InvalidSelection

    if((p_pRequest->m_burstModeControlCode != Off) && (p_pRequest->m_burstModeControlCode != EnableOnTDMADataLinkLayer))
    {
        return RCS_E02_InvalidSelection;
    }

    if (p_ucCmdType == 0xA5)
    {
        p_ucCmdType = 0;
        ucSourceAPHart=1;
    }

    // if this command request comes before the device is operational [i.e. before joining the network; possible on wired HART medium], only save the values into NVM
    if (g_ucAppProcessorStatus < API_STATE_NOTIFY_JOIN_OK)
    { // just update NVM ... RFBWSservice will be requested after join
        g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucBurstModeControlCode = p_pRequest->m_burstModeControlCode;       
        // Set in NVM structure
        FLASH_UPDATE_NEEDED();
        // Set configuration changed in Status Byte
        SET_CONFIGURATION_CHANGED();
    }
    else
    {
        if (g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ucSrvcDenied == 0x41)
        {
            g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ucSrvcDenied = 0;
            return RCM_E65_ServiceRequestDenied;
        }

        // If burst is disabled and request to EnableOnTDMADataLinkLayer
        if((g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ucSrvcModeControlCode == Off) &&
            (p_pRequest->m_burstModeControlCode == EnableOnTDMADataLinkLayer))
        {
            if (g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ucSrvcReqStatus == SRVC_REQ_C801)
            { // Return error code
                return RCS_E32_Busy; // service request was sent, burst pendingF
            }
            else
            {
                // g_stBurstNVM[i].m_ulUpdateTime was divided by 32 in CMD103! so multiply the value by 32 when requiring a service
                ulBurstPeriod = (uint32)(g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ulUpdateTime * 32);
                if(App_GetSrvcReq(CMDID_C109_BurstModeCtrl, p_pRequest->m_ucBurstMessage, ulBurstPeriod, ucSourceAPHart) == API_SUCCESS)
                {
                    g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ucSrvcReqStatus = SRVC_REQ_C799;                    // Set SrvReq true
                    g_stRFBWService[p_pRequest->m_ucBurstMessage].m_unSrvcCommandNumber = CMDID_C109_BurstModeCtrl;     // put the value of the command Id into the g_ucCommandnumber
                    g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ulSrvcRequestPeriod = ulBurstPeriod;                // save the burst period in order to store the value when the 2nd service request is sent
                    if (!g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse)
                    {
                        g_bNeedSendRsp = TRUE; // Set flag - allow sending a command response to the VN radio when the srvc response arrives
                    }
                    if (ucSourceAPHart)
                        API_Tx_Handler(1);

                    return RCS_N254_ResponseAck;
                }
                else
                {
                    return RCS_N255_ResponseNack;
                }
            }
        }

        // Burst is currently enabled and request is to disable
        if (p_pRequest->m_burstModeControlCode == Off)
        {
            if (g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ucSrvcModeControlCode == EnableOnTDMADataLinkLayer)
            {
                // when burst is to be turn off, send a srvc del request; though response from Radio is not quite vital. immediate burst stop is of higher priority!
                ucBurstMsg = p_pRequest->m_ucBurstMessage;
                App_DelSrvcReq(CMDID_C109_BurstModeCtrl, ucBurstMsg);
            }

            g_stProv.m_stBurstNVM[p_pRequest->m_ucBurstMessage].m_ucBurstModeControlCode = Off;              // Set in NVM structure
            g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ucSrvcModeControlCode = Off;            // Set in Ram structure
            g_stRFBWService[p_pRequest->m_ucBurstMessage].m_ucSrvcDenied = 0;

            // update data in NVM
            FLASH_UPDATE_NEEDED();
            // Set configuration changed in Status Byte
            SET_CONFIGURATION_CHANGED();
        }
    }

    // Compose response
    memcpy(p_pResponse, p_pRequest, sizeof(C109_BurstModeCtrl_Req));
    return RCS_N00_Success;
}



//
// C115
//
/**
 * @brief Executes Cmd115.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C115_ReadEventNotificationSummary(C115_ReadEventNotificationSummary_Req *p_pRequest, C115_ReadEventNotificationSummary_Resp *p_pResponse, uint8 p_ucCmdType)
{
    if(p_pRequest->eventNo >= MAX_EVENTS_NO)
        return RCS_E02_InvalidSelection;

    NV_EVENT_T * pCrtEvent = &g_stProv.m_stEventNVM;

    p_pResponse->eventNo = p_pRequest->eventNo;
    p_pResponse->noOfEventsSupported = MAX_EVENTS_NO;

    if( EVENT_QUEUE_NOT_EMPTY )
    { // have an event triggered
        EVENT_PENDING_T* pEvent = g_stEventQueue;                                           // active event
        p_pResponse->firstUnacknowledged.u32 = pEvent->m_ulTimestamp;                       // [1/32 ms]
        for(; pEvent < g_pEventQueueEnd; pEvent++)                                          // get event status
        {
            p_pResponse->eventStatus |= pEvent->m_ucEventStatus;
        }
    }
    else
    { // no event was triggered
        p_pResponse->firstUnacknowledged.u32 = 0xFFFFFFFF;
        p_pResponse->eventStatus = 0x00;
    }

    p_pResponse->eventNotificationControlCode = (EventNotificationControlCodes)pCrtEvent->m_ucEventControlCode;
    p_pResponse->eventNotificationRetryTime.u32 = pCrtEvent->m_ulRetryTime * 32;     // [1/32 ms]
    p_pResponse->maximumUpdateTime.u32 = pCrtEvent->m_ulMaxUpdateTime * 32;          // [1/32 ms]
    p_pResponse->eventDeBounceInterval.u32 = pCrtEvent->m_ulDebounceInterval * 32;   // [1/32 ms]
    p_pResponse->DeviceStatusMask = pCrtEvent->m_ucDeviceStatusMask;
    p_pResponse->noOfReadEvents = pCrtEvent->m_ucMaskSize;

    uint8 ucIdx;
    for(ucIdx=0; ucIdx < pCrtEvent->m_ucMaskSize; ucIdx++)
    {
        p_pResponse->eventMask[ucIdx] = pCrtEvent->m_aucEventMasks[ucIdx];
    }

    return RCS_N00_Success;
}



//
// C116
//
/**
 * @brief Executes Cmd116.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C116_WriteEventNotificationBitMask(C116_WriteEventNotificationBitMask_Req *p_pRequest, C116_WriteEventNotificationBitMask_Resp *p_pResponse, uint8 p_ucCmdType)
{
    if(p_pRequest->eventNo >= MAX_EVENTS_NO)
        return RCS_E02_InvalidSelection;

    uint8 ucNofEvents = p_pRequest->noOfWriteEvents;
    *p_pResponse = *p_pRequest;
    memcpy(p_pResponse->eventMask, p_pRequest->eventMask, ucNofEvents);

    // This command is truncatable at any point after the Extended Field Device Status
    // (byte number 6 of Command 48)
    if (ucNofEvents < 7)
    {
        return RCS_E05_TooFewDataBytesReceived;
    }

    if(ucNofEvents < MaxNoOfWriteEvents)
    { // truncated mask means the rest must be set on 0
        memset(p_pRequest->eventMask + ucNofEvents, 0, MaxNoOfWriteEvents-ucNofEvents);
    }

    NV_EVENT_T * pCrtEvent = &g_stProv.m_stEventNVM;

    if( memcmp(p_pRequest->eventMask, pCrtEvent->m_aucEventMasks, MaxNoOfWriteEvents) ) // compare all mask, not just req size, to include truncation scenario
    {
        pCrtEvent->m_ucMaskSize = p_pRequest->noOfWriteEvents;
        memcpy(pCrtEvent->m_aucEventMasks, p_pRequest->eventMask, MaxNoOfWriteEvents);
       
        //update data in NVM
        FLASH_UPDATE_NEEDED();
                
        // set configuration changed
        SET_CONFIGURATION_CHANGED();
    }

    return RCS_N00_Success;
}



//
// C117
//
/**
 * @brief Executes Cmd117.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C117_WriteEventNotificationTiming(C117_WriteEventNotificationTiming_Req *p_pRequest, C117_WriteEventNotificationTiming_Resp *p_pResponse, uint8 p_ucCmdType)
{
    if( p_pRequest->eventNo >= MAX_EVENTS_NO )
        return RCS_E02_InvalidSelection; //RCM_E09_InvalidCommandRequested;

    if( p_pRequest->eventNotificationRetryTime.u32 >  p_pRequest->maximumUpdateTime.u32 )
        return RCS_E02_InvalidSelection;

    // Get data from request
    uint32 ulCorrectedRetryTime; // in mSeconds
    uint32 ulCorrectedMaxUpdateTime; // in mSeconds
    uint32 ulCorrectedDebounceInterval; // in mSeconds

    uint8 ucReturnRT = UpdatePeriodCorrection (p_pRequest->eventNotificationRetryTime.u32, &ulCorrectedRetryTime);
    uint8 ucReturnMUT = UpdatePeriodCorrection (p_pRequest->maximumUpdateTime.u32, &ulCorrectedMaxUpdateTime);
    uint8 ucReturnDI = UpdatePeriodCorrection (p_pRequest->eventDeBounceInterval.u32, &ulCorrectedDebounceInterval);
    uint8 ucSourceAPHart=0;

    if (p_ucCmdType == 0xA5)
    {
        p_ucCmdType = 0;
        ucSourceAPHart=1;
    }

    // prepare response
    p_pResponse->eventNo = p_pRequest->eventNo;
    p_pResponse->eventNotificationRetryTime.u32 = ulCorrectedRetryTime * 32; // in WH Time_t
    p_pResponse->maximumUpdateTime.u32 = ulCorrectedMaxUpdateTime * 32; // in WH Time_t
    p_pResponse->eventDeBounceInterval.u32 = ulCorrectedDebounceInterval * 32; // in WH Time_t

    if ( g_stRFBWService[EVENT_SERVICE_CHANNEL].m_ucSrvcModeControlCode == EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer )
    { // I have Event enabled
        if (g_ucAppProcessorStatus >= API_STATE_NOTIFY_JOIN_OK)
        { // the device has already joined the network
            if ( ulCorrectedRetryTime != g_stProv.m_stEventNVM.m_ulRetryTime )
            { // new period is less than  curent one saved period => so more i need more bandwidth ... because i need to publish more quickly
                if (App_GetSrvcReq((uint16)CMDID_C117_WriteEventNotificationTiming, EVENT_SERVICE_CHANNEL, p_pResponse->eventNotificationRetryTime.u32, ucSourceAPHart) == API_SUCCESS) // request for service
                {
                    g_stRFBWService[EVENT_SERVICE_CHANNEL].m_ucSrvcReqStatus = SRVC_REQ_C799; // Set SrvReq true
                    g_stRFBWService[EVENT_SERVICE_CHANNEL].m_unSrvcCommandNumber = CMDID_C117_WriteEventNotificationTiming;
                    g_stRFBWService[EVENT_SERVICE_CHANNEL].m_ulSrvcRequestPeriod = p_pResponse->eventNotificationRetryTime.u32; // in WH Time_t
                    // prepare the other values to be saved in NVM when we receive the BW service approved
                    g_stProv.m_stEventNVM.m_ulMaxUpdateTime = ulCorrectedMaxUpdateTime; // in mS
                    g_stProv.m_stEventNVM.m_ulDebounceInterval = ulCorrectedDebounceInterval; // in mS
                    if (!g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse)
                    {
                        g_bNeedSendRsp = TRUE; // Set flag - allow sending a command response to the VN radio when the srvc response arrives
                    }
                    if (ucSourceAPHart)
                    {
                        API_Tx_Handler(1);
                    }
                    return RCS_N254_ResponseAck;
                }
            }
        }
    }

    if (
        (g_stProv.m_stEventNVM.m_ulRetryTime != ulCorrectedRetryTime) ||
        (g_stProv.m_stEventNVM.m_ulMaxUpdateTime != ulCorrectedMaxUpdateTime) ||
        (g_stProv.m_stEventNVM.m_ulDebounceInterval != ulCorrectedDebounceInterval)
       )
    {
        g_stProv.m_stEventNVM.m_ulRetryTime = ulCorrectedRetryTime;
        g_stProv.m_stEventNVM.m_ulMaxUpdateTime = ulCorrectedMaxUpdateTime;
        g_stProv.m_stEventNVM.m_ulDebounceInterval = ulCorrectedDebounceInterval;

        // update data in NVM
        FLASH_UPDATE_NEEDED();

        // SET_CONFIG_CHANGED => Set configuration changed in Status Byte
        SET_CONFIGURATION_CHANGED();
    }

    if ( ucReturnRT || ucReturnMUT || ucReturnDI )
    {
        return RCM_W08_UpdateTimesAdjusted;
    }

    return RCS_N00_Success;
}



//
// C118
//
/**
 * @brief Executes Cmd118.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C118_EventNotificationControl(C118_EventNotificationControl_Req *p_pRequest, C118_EventNotificationControl_Resp *p_pResponse, uint8 p_ucCmdType)
{
uint32 ulRetryTime; // this is the actual period that is required by WHART specification; it is obtained by multiplying the UpdateTime [within burst struct] with 32
uint8 ucSourceAPHart=0;

    if( p_pRequest->eventNo >= MAX_EVENTS_NO )
        return RCS_E02_InvalidSelection; // RCM_E09_InvalidCommandRequested;

    if(
        (p_pRequest->eventNotificationControlCode != EventNotificationControlCodes_Off) &&
        (p_pRequest->eventNotificationControlCode != EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer)
       )
    {
        return RCS_E02_InvalidSelection;
    }

    if (p_ucCmdType == 0xA5)
    {
        p_ucCmdType = 0;
        ucSourceAPHart=1;
    }

    // if this command request comes before the device is operational [i.e. before joining the network; possible on wired HART medium], only save the values into NVM
    if (g_ucAppProcessorStatus < API_STATE_NOTIFY_JOIN_OK)
    { // just update NVM ... RFBWSservice will be requested after join
        g_stProv.m_stEventNVM.m_ucEventControlCode = p_pRequest->eventNotificationControlCode; // Set in NVM structure
        //update data in NVM
        FLASH_UPDATE_NEEDED();
        // Set configuration changed in Status Byte
        SET_CONFIGURATION_CHANGED();
    }
    else
    {
        // If burst is disabled and request to EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer
        if((g_stRFBWService[EVENT_SERVICE_CHANNEL].m_ucSrvcModeControlCode == Off) &&
            (p_pRequest->eventNotificationControlCode == EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer))
        {
            if (g_stRFBWService[EVENT_SERVICE_CHANNEL].m_ucSrvcReqStatus == SRVC_REQ_C801)
            { // Return error code
                return RCS_E32_Busy; // service request was sent, burst pendingF
            }
            else
            {
                // RetryTime was divided by 32 in CMD117 ! so multiply the value by 32 when requiring a service
                ulRetryTime = g_stProv.m_stEventNVM.m_ulRetryTime * 32; // in WH_Time_t
                if(App_GetSrvcReq(CMDID_C118_EventNotificationControl,EVENT_SERVICE_CHANNEL, ulRetryTime, ucSourceAPHart) == API_SUCCESS)
                {
                    g_stRFBWService[EVENT_SERVICE_CHANNEL].m_ucSrvcReqStatus = SRVC_REQ_C799;                           // Set SrvReq true
                    g_stRFBWService[EVENT_SERVICE_CHANNEL].m_unSrvcCommandNumber = CMDID_C118_EventNotificationControl; // put the value of the command Id into the g_ucCommandnumber
                    g_stRFBWService[EVENT_SERVICE_CHANNEL].m_ulSrvcRequestPeriod = ulRetryTime;                         // save the burst period in order to store the value when the 2nd service request is sent
                    if (!g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse)
                    {
                        g_bNeedSendRsp = TRUE; // Set flag - allow sending a command response to the VN radio when the srvc response arrives
                    }
                    if (ucSourceAPHart)
                        API_Tx_Handler(1);

                    return RCS_N254_ResponseAck;
                }
                else
                {
                    return RCS_N255_ResponseNack;
                }
            }
        }

        // Burst is currently enabled and request is to disable
        if (p_pRequest->eventNotificationControlCode == EventNotificationControlCodes_Off)
        {
            if (g_stRFBWService[EVENT_SERVICE_CHANNEL].m_ucSrvcModeControlCode == EnableOnTDMADataLinkLayer)
            {
                // when burst is to be turn off, send a srvc del request; though response from Radio is not quite vital. immediate burst stop is of higher priority!
                App_DelSrvcReq(CMDID_C118_EventNotificationControl, EVENT_SERVICE_CHANNEL);
            }

            g_stProv.m_stEventNVM.m_ucEventControlCode = Off;                                            // Set in NVM structure
            g_stRFBWService[EVENT_SERVICE_CHANNEL].m_ucSrvcModeControlCode = Off;               // Set in Ram structure
            g_stRFBWService[EVENT_SERVICE_CHANNEL].m_ucSrvcDenied = 0;

            //update data in NVM
            FLASH_UPDATE_NEEDED();
            // Set configuration changed in Status Byte
            SET_CONFIGURATION_CHANGED();
        }
    }

    // Compose response
    memcpy(p_pResponse, p_pRequest, sizeof(C118_EventNotificationControl_Req));

    return RCS_N00_Success;
}



//
// C119
//
/**
 * @brief Executes Cmd119.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C119_AcknowledgeEventNotification(C119_AcknowledgeEventNotification_Req *p_pRequest, C119_AcknowledgeEventNotification_Resp *p_pResponse, uint8 p_ucCmdType)
{
uint8 ucRC = RCS_N00_Success;

    if(p_pRequest->eventNo >= MAX_EVENTS_NO)
        return RCS_E02_InvalidSelection;

    if( g_ucIsEventAck )
    { // device received a Cmd119 request
        if( (p_pRequest->firstUnacknowledged.u32 == g_stEventQueue[0].m_ulTimestamp) &&
            (p_pRequest->configurationChangedCounter == g_stEventQueue[0].m_ucCCCounter) &&
            (p_pRequest->eventNo == g_stEventQueue[0].m_ucEventNo) &&
            (p_pRequest->deviceStatus == g_stEventQueue[0].m_ucDeviceStatus) &&
            !memcmp(p_pRequest->command48Data, g_stEventQueue[0].m_aucEvents, g_stProv.m_stEventNVM.m_ucMaskSize))
        { // CC counter, device status, Cmd48 data and timestamp match -> event ist acknowledged
            g_stEventQueue[0].m_ucAckReceived = 1; // set the event acknowledged flag
            // reset event status bits (see common table 36) -> not necessary here
            // look for other events into the queue
            if( (g_pEventQueueEnd - g_stEventQueue) > 1 )
            {
                ucRC = RCM_W08_NotAllEventsCleared;
            }
        }
        else
        { // Cmd119 request != published event -> event remains active and will be sent as response

            if( EVENT_QUEUE_NOT_EMPTY )
            {
                p_pResponse->firstUnacknowledged.u32 = g_stEventQueue[0].m_ulTimestamp;
                p_pResponse->configurationChangedCounter = g_stEventQueue[0].m_ucCCCounter;
                p_pResponse->eventNo = g_stEventQueue[0].m_ucEventNo;
                p_pResponse->deviceStatus = g_stEventQueue[0].m_ucDeviceStatus;
                memcpy(p_pResponse->command48Data, g_stEventQueue[0].m_aucEvents, MaxLengthOf48Command);

                // clear Cmd119 request flag
                g_ucIsEventAck = 0;

                return RCM_W08_NotAllEventsCleared;
            }
        }

        // clear Cmd119 request flag
        g_ucIsEventAck = 0;

        // compose the response
        *p_pResponse = *p_pRequest;
    }
    else
    { // send the triggered event
        p_pResponse->firstUnacknowledged.u32 = g_stEventQueue[0].m_ulTimestamp;
        p_pResponse->configurationChangedCounter = g_stEventQueue[0].m_ucCCCounter;
        p_pResponse->eventNo = g_stEventQueue[0].m_ucEventNo;
        p_pResponse->deviceStatus = g_stEventQueue[0].m_ucDeviceStatus;
        memcpy(p_pResponse->command48Data, g_stEventQueue[0].m_aucEvents, MaxLengthOf48Command);
    }

    return ucRC;
}


//
// C150
//
/**
 * @brief Executes Cmd150.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C150_ReadAppFwVersion(C150_ReadAppFwVersion_Req *p_pRequest, C150_ReadAppFwVersion_Resp *p_pResponse, uint8 p_ucCmdType)
{
    p_pResponse->ucMajor = CENTERO_MAJOR;
    p_pResponse->ucMinor = CENTERO_MINOR;
    p_pResponse->ucRevision = CENTERO_REVISION;
    p_pResponse->ucBuildLetter = CENTERO_BUILD_LETTER;
    p_pResponse->pcDate = __DATE__;
    p_pResponse->pcTime = __TIME__;
    return RCS_N00_Success;
}

//
// C151
//
/**
 * @brief Executes Cmd151.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C151_RFEngineeringMode(C151_RFEngineeringMode_Req *p_pRequest, C151_RFEngineeringMode_Resp *p_pResponse, uint8 p_ucCmdType)
{
uint8 aucReqBuf[12];
uint8* pucBuf = aucReqBuf;

    switch (p_pRequest->m_ucRFTestMode)
    {
    case 0x00: // ENG_MODE_NONE

        *(pucBuf++) = p_pRequest->m_ucRFTestMode;
        p_pResponse->m_ucRFTestMode = p_pRequest->m_ucRFTestMode;
        break;

#ifdef RF_TEST_MODE_ENABLED
    case 0x01: // ENG_MODE_TX_MOD
    case 0x02: // ENG_MODE_TX_UNMOD
    case 0x04: // ENG_MODE_TX_PACKET

        *(pucBuf++) = p_pRequest->m_ucRFTestMode;
        *(pucBuf++) = p_pRequest->m_ucRFChannel;
        *(pucBuf++) = p_pRequest->m_unRFOutputPowerLevel >> 8;
        *(pucBuf++) = p_pRequest->m_unRFOutputPowerLevel;
        *(pucBuf++) = p_pRequest->m_unTimeout >> 8;
        *(pucBuf++) = p_pRequest->m_unTimeout;
        p_pResponse->m_ucRFTestMode = p_pRequest->m_ucRFTestMode;
        p_pResponse->m_ucRFChannel = p_pRequest->m_ucRFChannel;
        p_pResponse->m_unRFOutputPowerLevel = p_pRequest->m_unRFOutputPowerLevel;
        p_pResponse->m_unTimeout = p_pRequest->m_unTimeout;
        break;

    case 0x03: // ENG_MODE_RX

        *(pucBuf++) = p_pRequest->m_ucRFTestMode;
        *(pucBuf++) = p_pRequest->m_ucRFChannel;
        *(pucBuf++) = p_pRequest->m_unTimeout >> 8;
        *(pucBuf++) = p_pRequest->m_unTimeout;
        p_pResponse->m_ucRFTestMode = p_pRequest->m_ucRFTestMode;
        p_pResponse->m_ucRFChannel = p_pRequest->m_ucRFChannel;
        p_pResponse->m_unTimeout = p_pRequest->m_unTimeout;
        break;

    case 0x05: // ENG_MODE_RX_PACKET

        *(pucBuf++) = p_pRequest->m_ucRFTestMode;
        *(pucBuf++) = p_pRequest->m_ucRFChannel;
        memcpy(pucBuf, p_pRequest->m_aRxEUIFilter, 8);
        pucBuf += 8;
        *(pucBuf++) = p_pRequest->m_unTimeout >> 8;
        *(pucBuf++) = p_pRequest->m_unTimeout;
        p_pResponse->m_ucRFTestMode = p_pRequest->m_ucRFTestMode;
        p_pResponse->m_ucRFChannel = p_pRequest->m_ucRFChannel;
        memcpy(p_pResponse->m_aRxEUIFilter, p_pRequest->m_aRxEUIFilter, 8);
        p_pResponse->m_unTimeout = p_pRequest->m_unTimeout;
        break;
#endif

    default:
        return RCS_E02_InvalidSelection;
    }

#ifdef RF_TEST_MODE_ENABLED
    if ( API_SUCCESS == API_SendRequest(CLS_API_COMMANDS, API_RF_TEST_MODE, 0, pucBuf - aucReqBuf, aucReqBuf, 1 ) )
    {
        API_Tx_Handler(1);
        return RCS_N00_Success;
    }
    else
        return RCS_E01_Undefined1;
#else
    return RCS_N00_Success;
#endif
}



//
// C152
//
/**
 * @brief Executes Cmd152.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C152_EventsManagement(C152_EventsManagement_Req *p_pRequest, C152_EventsManagement_Resp *p_pResponse, uint8 p_ucCmdType)
{
    if (
         ( p_pRequest->m_ucPassword[0] == 'E' ) &&
         ( p_pRequest->m_ucPassword[1] == 'V' ) &&
         ( p_pRequest->m_ucPassword[2] == 'T' )
       )
    {
        if ( p_pRequest->m_ucAction == 0x01 )
        {
            // reset all cmd 48 events flags ...
            // ... here should be implemented in diagnostics dependent way
            // ... not all flags should be cleared every time ... depends on project request
            memset( &g_stCmd48Data, 0, sizeof(g_stCmd48Data));

            g_ucDeviceStatus[MASTER_WISA] |= FieldDeviceStatusesMask_MoreStatusAvailable;
            g_ucDeviceStatus[MASTER_PRIMARY] |= FieldDeviceStatusesMask_MoreStatusAvailable;
            g_ucDeviceStatus[MASTER_SECONDARY] |= FieldDeviceStatusesMask_MoreStatusAvailable;

            p_pResponse->m_ucPassword[0] = '*';
            p_pResponse->m_ucPassword[1] = '*';
            p_pResponse->m_ucPassword[2] = '*';
            p_pResponse->m_ucAction = 0x00;
            return RCS_N00_Success;
        }
    }

    return RCS_E02_InvalidSelection;
}
uint8 Execute_C180_MakeMeasurement(C180_MakeMeasurement_Req *p_pRequest, C180_MakeMeasurement_Resp *p_pResponse, uint8 p_ucCmdType)
{
//    if(strt_measurement(1)){  // to be edited
//    p_pResponse->m_ucMesStrtStat = 1; }
//    else {p_pResponse->m_ucMesStrtStat = 0;}
     if (!c180_stat && !g_ucMeasurementInProgress)
     {
         switch (ch) {
             case 0:
                 p_pResponse->h_fMat_Thick_Raw = mat_thick_raw_0;
                 p_pResponse->h_fMat_Thick_TC = mat_thick_TC_0;
                 p_pResponse ->h_fMat_Temp = mat_temp_0 ;
                 p_pResponse ->h_fDSI_Temp = dsi_t_0 ;
                 break;
                 
             case 1:
                 p_pResponse->h_fMat_Thick_Raw = mat_thick_raw_1;
                 p_pResponse->h_fMat_Thick_TC = mat_thick_TC_1;
                 p_pResponse ->h_fMat_Temp = mat_temp_1 ;
                 p_pResponse ->h_fDSI_Temp = dsi_t_1 ;
                 break;
                 
             case 2:
                 p_pResponse->h_fMat_Thick_Raw = mat_thick_raw_2;
                 p_pResponse->h_fMat_Thick_TC = mat_thick_TC_2;
                 p_pResponse ->h_fMat_Temp = mat_temp_2 ;
                 p_pResponse ->h_fDSI_Temp = dsi_t_2 ;
                 break;
                 
             case 3:
                 p_pResponse->h_fMat_Thick_Raw = mat_thick_raw_3;
                 p_pResponse->h_fMat_Thick_TC = mat_thick_TC_3;
                 p_pResponse ->h_fMat_Temp = mat_temp_3 ;
                 p_pResponse ->h_fDSI_Temp = dsi_t_3 ;
                 break;
         }
     }
    return RCS_N00_Success;
}
uint8 Execute_C181_WriteUTMParameters(C181_WriteUTMParameters_Req *p_pRequest, C181_WriteUTMParameters_Resp *p_pResponse, uint8 p_ucCmdType)
{
    p_pResponse->h_cal_0_offset = HRT_Para.h_fCal_0_offset;
    p_pResponse->h_material_velocity = HRT_Para.h_fMaterial_velocity;
    p_pResponse->h_a_scan_start = HRT_Para.h_fA_scan_start;
    p_pResponse->h_pulsar_width = HRT_Para.h_uiPulsar_width;
    p_pResponse->h_gain = HRT_Para.h_uiGain;
    
    return RCS_N00_Success;
}

uint8 Execute_C182_ReadUTMParameters(C182_ReadUTMParameters_Req *p_pRequest, C182_ReadUTMParameters_Resp *p_pResponse, uint8 p_ucCmdType)
{
    p_pResponse->h_cal_0_offset = HRT_Para.h_fCal_0_offset;;
    p_pResponse->h_material_velocity = HRT_Para.h_fMaterial_velocity;
    p_pResponse->h_a_scan_start = HRT_Para.h_fA_scan_start;
    p_pResponse->h_pulsar_width = HRT_Para.h_uiPulsar_width;
    p_pResponse->h_gain = HRT_Para.h_uiGain;
    return RCS_N00_Success;
    
}
uint8 Execute_C183_WriteUTGParameters(C183_WriteUTGParameters_Req *p_pRequest, C183_WriteUTGParameters_Resp *p_pResponse, uint8 p_ucCmdType)
{
    p_pResponse->h_g1_start = HRT_Para.h_fG1_start;
    p_pResponse->h_g1_width = HRT_Para.h_fG1_width;
    p_pResponse->h_g1_thershold = HRT_Para.h_fG1_thershold;
    p_pResponse->h_g2_start = HRT_Para.h_fG2_start;
    p_pResponse->h_g2_width = HRT_Para.h_fG2_width;
    p_pResponse->h_g2_thershold = HRT_Para.h_fG2_thershold;
    p_pResponse->h_measurement_mode = HRT_Para.h_uiMeasurement_mode;
    
    return RCS_N00_Success;
}

uint8 Execute_C184_ReadUTGParameters(C184_ReadUTGParameters_Req *p_pRequest, C184_ReadUTGParameters_Resp *p_pResponse, uint8 p_ucCmdType)
{
    p_pResponse->h_g1_start = HRT_Para.h_fG1_start;
    p_pResponse->h_g1_width = HRT_Para.h_fG1_width;
    p_pResponse->h_g1_thershold = HRT_Para.h_fG1_thershold;
    p_pResponse->h_g2_start = HRT_Para.h_fG2_start;
    p_pResponse->h_g2_width = HRT_Para.h_fG2_width;
    p_pResponse->h_g2_thershold = HRT_Para.h_fG2_thershold;
    p_pResponse->h_measurement_mode = HRT_Para.h_uiMeasurement_mode;
    return RCS_N00_Success;
    
}

uint8 Execute_C185_WriteUTTParameters(C185_WriteUTTParameters_Req *pRequest, C185_WriteUTTParameters_Resp *p_pResponse, uint8 p_ucCmdType)
{    
//    unsigned char MRes;
    
    HRT_Para.h_fTempCompRefTemp = pRequest->h_Ref_temp;
    HRT_Para.h_fMaterial_velocity = pRequest->h_material_velocity;
    HRT_Para.h_fTempCompFactor = pRequest->h_Comp_factor;
    
//    MRes = strt_measurement(4);
    
    p_pResponse->h_Ref_temp = HRT_Para.h_fTempCompRefTemp;
    p_pResponse->h_material_velocity = HRT_Para.h_fMaterial_velocity;
    p_pResponse->h_Comp_factor = HRT_Para.h_fTempCompFactor;
   
    return RCS_N00_Success;
}

uint8 Execute_C186_ReadUTTParameters(C186_ReadUTTParameters_Req *pRequest, C186_ReadUTTParameters_Resp *p_pResponse, uint8 p_ucCmdType)
{
    p_pResponse->h_Ref_temp = HRT_Para.h_fTempCompRefTemp; 
    p_pResponse->h_material_velocity = HRT_Para.h_fMaterial_velocity;
    p_pResponse->h_Comp_factor = HRT_Para.h_fTempCompFactor;
    
    return RCS_N00_Success;
}




//
// C188
//
/**
 * @brief Executes Cmd188.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C188_ResetBatteryLife(C188_ResetBatteryLife_Req *p_pRequest, C188_ResetBatteryLife_Resp *p_pResponse, uint8 p_ucCmdType)
{
    return ResetBatteryLife();
}


uint8 Execute_C195_AccessPassword(C195_AccessPassword_Req *p_pRequest, C195_AccessPassword_Resp *p_pResponse, uint8 p_ucCmdType)
{
    memcpy(p_pResponse->m_aucPassword, p_pRequest->m_aucPassword, sizeof(p_pRequest->m_aucPassword));
    return SetAccessPassword(p_pRequest->m_aucPassword, sizeof(p_pRequest->m_aucPassword));
    
}

uint8 Execute_C196_WritePassword(C196_WritePassword_Req *p_pRequest, C196_WritePassword_Resp *p_pResponse, uint8 p_ucCmdType)
{
    memcpy(p_pResponse->m_aucLevel1Password, p_pRequest->m_aucLevel1Password, sizeof(p_pRequest->m_aucLevel1Password));
    return UpdateAccessPassword(p_pRequest->m_aucLevel1Password, sizeof(p_pRequest->m_aucLevel1Password));
}

uint8 Execute_C197_DisablePassword(C197_DisablePassword_Req *p_pRequest, C197_DisablePassword_Resp *p_pResponse, uint8 p_ucCmdType)
{
    p_pResponse->m_ucEnableOrDisable = p_pRequest->m_ucEnableOrDisable;    
    return SetPasswordEnableStatus(p_pRequest->m_ucEnableOrDisable);
}

uint8 Execute_C198_WriteAlarmMode(C198_WriteAlarmMode_Req *p_pRequest, C198_WriteAlarmMode_Resp *p_pResponse, uint8 p_ucCmdType)
{
    p_pResponse->m_ucAlarmMode = p_pRequest->m_ucAlarmMode;
    
    return SetAlarmMode(p_pRequest->m_ucAlarmMode);
}

uint8 Execute_C201_WriteMSSerialNumber(C201_WriteMSSerialNumber_Req *p_pRequest, C201_WriteMSSerialNumber_Resp *p_pResponse, uint8 p_ucCmdType)
{
    memcpy(p_pResponse->m_aucMSSerialNumber, p_pRequest->m_aucMSSerialNumber, sizeof(p_pRequest->m_aucMSSerialNumber));
    return SetSerialNumber(p_pRequest->m_aucMSSerialNumber, sizeof(p_pRequest->m_aucMSSerialNumber));
}

uint8 Execute_C202_ReadMSSerialNumber(C202_ReadMSSerialNumber_Req *p_pRequest, C202_ReadMSSerialNumber_Resp *p_pResponse, uint8 p_ucCmdType )
{
    memcpy(p_pResponse->m_aucMSSerialNumber, g_stDeviceProcessVariables.m_aucMSSerialNumber, sizeof(g_stDeviceProcessVariables.m_aucMSSerialNumber));
    return RCS_N00_Success;
}

uint8 Execute_C203_WriteCommissionDate(C203_WriteCommissionDate_Req *p_pRequest, C203_WriteCommissionDate_Resp  *p_pResponse, uint8 p_ucCmdType)
{
    p_pResponse->m_ucMonth = p_pRequest->m_ucMonth;
    p_pResponse->m_ucDay = p_pRequest->m_ucDay;    
    p_pResponse->m_ucYear = p_pRequest->m_ucYear;
    
    return SetCommissionDate((const uint8*)p_pRequest, sizeof(C203_WriteCommissionDate_Req));    
}

uint8 Execute_C204_ReadCommissionDate(C204_ReadCommissionDate_Req *p_pRequest, C204_ReadCommissionDate_Resp *p_pResponse, uint8 p_ucCmdType)
{
    p_pResponse->m_ucMonth = g_stDeviceProcessVariables.g_stCommissionDate.m_ucMonth;
    p_pResponse->m_ucDay = g_stDeviceProcessVariables.g_stCommissionDate.m_ucDay;    
    p_pResponse->m_ucYear = g_stDeviceProcessVariables.g_stCommissionDate.m_ucYear;    
    return RCS_N00_Success;
}

uint8 Execute_C205_WriteMeasurementTimeInterval(C205_WriteMeasurementTimeInterval_Req *p_pRequest, C205_WriteMeasurementTimeInterval_Resp *p_pResponse, uint8 p_ucCmdType)
{
    p_pResponse->m_unMeasurementTimeInterval = (0x00FF & (p_pRequest->m_unMeasurementTimeInterval >> 8)) | (0xFF00 & (p_pRequest->m_unMeasurementTimeInterval << 8)); 
    return SetMeasurementTimeInterval(p_pRequest->m_unMeasurementTimeInterval);
}

uint8 Execute_C206_ReadMeasurementTimeInterval(C206_ReadMeasurementTimeInterval_Req *p_pRequest, C206_ReadMeasurementTimeInterval_Resp *p_pResponse, uint8 p_ucCmdType)
{
    p_pResponse->m_unMeasurementTimeInterval = (0x00FF & (g_stDeviceProcessVariables.m_unMeasurementTimeInterval >> 8)) | (0xFF00 & (g_stDeviceProcessVariables.m_unMeasurementTimeInterval << 8));
    return RCS_N00_Success;
}

uint8 Execute_C210_SetFactoryDefaults(C210_FactoryDefaults_Req *p_pRequest, C210_FactoryDefaults_Resp *p_pResponse, uint8 p_ucCmdType)
{
    NVM_LoadPVDefaultData();
    return RCS_N00_Success;
}



//
// C778
//
/**
 * @brief Executes Cmd778.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in]  p_ucCmdType - Wireless or Wired message.
 */
uint8 Execute_C778_ReadBatteryLife(C778_ReadBatteryLife_Req *p_pRequest, C778_ReadBatteryLife_Resp *p_pResponse, uint8 p_ucCmdType)
{
    uint16 unTemp = (uint16) EnergyLeftDays;

    p_pResponse->BatteryLifeDays = (0x00FF & (unTemp >> 8)) | (0xFF00 & (unTemp << 8));
    return RCS_N00_Success;
}
