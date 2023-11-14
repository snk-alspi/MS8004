#include <string.h>
#include "../Global/typedef.h"
#include "App_WH_Tables.h"
#include "App_WH_Types.h"
#include "App_WH_CmdCompose.h"
#include "App_WH_User.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// C001
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd001 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_pLen - Response data length.
 */
uint8 Compose_C001_ReadPrimaryVariable (C001_ReadPrimaryVariable_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucPrimaryVarUnit, 1);
    p_pucStream++;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_fPrimaryVar, sizeof(fp32));
    //#ifdef PUBLISH_MONOTONIC
    //    p_pucStream += 4;
    //    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_unMonotonic, sizeof(fp32));
    //    *p_pLen = 9;
    //#else
        *p_pLen = 5;
    //#endif
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C002
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd002 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_pLen - Response data length.
 */
uint8 Compose_C002_ReadLoopCurrAndPercentRange(C002_ReadLoopCurrentAndPercentOfRange_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->m_fPrimaryVariableLoopCurrent), sizeof(fp32));
    p_pucStream += sizeof(fp32);
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->m_fPrimaryVariablePercentOfRange), sizeof(fp32));
    *p_pLen = sizeof(fp32) + sizeof(fp32);
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C003
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd003 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_pLen - Response data length.
 */
uint8 Compose_C003_ReadDynamicVarsAndLoopCurrent(C003_ReadDynamicVariablesAndLoopCurrent_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->m_fLoopCurrent), sizeof(fp32));
    p_pucStream += sizeof(fp32);

    *p_pucStream = p_pResponse->m_ucPvUnitsCode;
    p_pucStream++;
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->m_PvValue), sizeof(fp32));
    p_pucStream += sizeof(fp32);

    *p_pucStream = p_pResponse->m_ucSvUnitsCode;
    p_pucStream++;
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->m_SvValue), sizeof(fp32));
    p_pucStream += sizeof(fp32);

    *p_pucStream = p_pResponse->m_ucTvUnitsCode;
    p_pucStream++;
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->m_TvValue), sizeof(fp32));
    p_pucStream += sizeof(fp32);

    *p_pucStream = p_pResponse->m_ucQvUnitsCode;
    p_pucStream++;
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->m_QvValue), sizeof(fp32));

    *p_pLen = 24; // See C003_ReadDynamicVariablesAndLoopCurrent_Resp definion
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C006
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd006 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_pLen - Response data length.
 */
uint8 Compose_C006_WritePollingAddress(C006_WritePollingAddress_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucPollingAddress, 1);
    p_pucStream++;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucLoopCurrentMode, 1);
    *p_pLen = 2;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C007
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd007 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_pLen - Response data length.
 */
uint8 Compose_C007_ReadLoopConfiguration(C007_ReadLoopConfiguration_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucPollingAddress, 1);
    p_pucStream++;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucLoopCurrentMode, 1);
    *p_pLen = 2;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C008
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd008 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_pLen - Response data length.
 */
uint8 Compose_C008_ReadDynamicVarsClassif(C008_ReadDynamicVariableClassification_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
    // the compiler will use the smallest type required to hold enum constants, preferring signed rather than unsigned
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucPrimaryVariableClassification, 1);
    p_pucStream++;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucSecondaryVariableClassification, 1);
    p_pucStream++;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucTertiaryVariableClassification, 1);
    p_pucStream++;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucQuaternaryVariableClassification, 1);
    *p_pLen = C008_RespSize;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C009
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd009 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_pLen - Response data length.
 */
uint8 Compose_C009_ReadDeviceVarsWithStatus(C009_ReadDeviceVarsWithStatus_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
uint8 ucIdx;

    // Extended Field Device Status
    *p_pucStream = p_pResponse->m_ucExtendedFieldDeviceStatus;
    p_pucStream++;

    // Device Vars
    for (ucIdx = 0; ucIdx < p_pResponse->m_ucNoDeviceVariables; ucIdx++)
    {
        *p_pucStream = p_pResponse->m_astSlot[ucIdx].m_ucCode;
        p_pucStream++;
        *p_pucStream = p_pResponse->m_astSlot[ucIdx].m_ucClassification;
        p_pucStream++;
        *p_pucStream = p_pResponse->m_astSlot[ucIdx].m_ucUnit;
        p_pucStream++;
        InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->m_astSlot[ucIdx].m_fValue), sizeof(fp32));
        p_pucStream += sizeof(fp32);
        memcpy(p_pucStream, &p_pResponse->m_astSlot[ucIdx].m_ucStatus, sizeof(DeviceVariableStatus));
        p_pucStream++;
    }

    // Slot 0 Data Time Stamp
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->m_stSlot0DataTimeStamp), 4 /*sizeof(Time_t) - not used, counts PADDING*/ );

    *p_pLen = sizeof(uint8) +                                                                                           // Extended Field Device Status length
           (p_pResponse->m_ucNoDeviceVariables) * 8 /*sizeof(C009_Slot_t) - not used, counts PADDING*/ +                // Device Vars length
           4 /*sizeof(Time_t) - not used, counts PADDING*/;                                                             // Slot 0 Data Time Stamp length

    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C014
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd014 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_pLen - Response data length.
 */
uint8 Compose_C014_ReadPrimaryVarTransducerInfo(C014_ReadDynamicVariableClassification_Resp  *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
    // the compiler will use the smallest type required to hold enum constants, preferring signed rather than unsigned
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_ulTransducerSerialNumber, 3);
    p_pucStream += 3;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucUnitsCode, 1);
    p_pucStream++;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_fUpperTransducerLimit, 4);
    p_pucStream += 4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_fLowerTransducerLimit, 4);
    p_pucStream += 4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_fMinimumSpan, 4);
    *p_pLen = C014_RespSize;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C015
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd015 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_pLen - Response data length.
 */
uint8 Compose_C015_ReadDeviceInfo(C015_ReadDeviceInfo_Resp  *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
    // the compiler will use the smallest type required to hold enum constants, preferring signed rather than unsigned
    memcpy(p_pucStream, (uint8*)&p_pResponse->pvAlarmSelectionCodes, 1);
    p_pucStream += 1;
    memcpy(p_pucStream, (uint8*)&p_pResponse->pvTransferFunctionCode, 1);
    p_pucStream += 1;
    memcpy(p_pucStream, (uint8*)&p_pResponse->pvUpperLowerRangeValuesUnitsCode, 1);
    p_pucStream += 1;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->pvUpperRangeValue, 4);
    p_pucStream += 4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->pvLowerRangeValue, 4);
    p_pucStream += 4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->pvDampingValue, 4);
    p_pucStream += 4;
    memcpy(p_pucStream, (uint8*)&p_pResponse->writeProtectCode, 1);
    p_pucStream += 1;
    memcpy(p_pucStream, (uint8*)&p_pResponse->reservedValue, 1);
    p_pucStream += 1;
    memcpy(p_pucStream, (uint8*)&p_pResponse->pvAnalogChannelFlags, 1);
    *p_pLen = C015_RespSize;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C033
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd033 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_pLen - Response data length.
 */
uint8 Compose_C033_ReadDeviceVars(C033_ReadDeviceVars_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
uint8 ucIdx;

    // Device Vars
    for (ucIdx = 0; ucIdx < p_pResponse->m_ucNoDeviceVariables; ucIdx++)
    {
        *p_pucStream = p_pResponse->m_astSlot[ucIdx].m_ucCode;
        p_pucStream++;
        *p_pucStream = p_pResponse->m_astSlot[ucIdx].m_ucUnit;
        p_pucStream++;
        InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->m_astSlot[ucIdx].m_fValue), sizeof(fp32));
        p_pucStream += sizeof(fp32);
    }

    *p_pLen = (p_pResponse->m_ucNoDeviceVariables) * 6 /*sizeof(C033_Slot_t) - not used, counts PADDING*/ ;
    return RCS_N00_Success;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// C035
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd035 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_pLen - Response data length.
 */
uint8 Compose_C035_WritePVRangeValues(C035_WritePVRangeValues_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
    *p_pucStream = p_pResponse->m_ucUnitsCode;
    p_pucStream++;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_fUpperRangeValue, sizeof(fp32));
    p_pucStream += 4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_fLowerRangeValue, sizeof(fp32));
    *p_pLen = sizeof(C035_WritePVRangeValues_Resp);
    return RCS_N00_Success;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// C044
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd044 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_pLen - Response data length.
 */
uint8 Compose_C044_WritePVUnits(C044_WritePVUnits_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pLen)
{
    *p_pucStream = p_pResponse->m_ucUnitsCode;
    *p_pLen = C044_RespSize;
    
    return RCS_N00_Success;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// C048
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd048 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_pLen - Response data length.
 */
uint8 Compose_C048_ReadAdditionalDeviceStatus(C048_ReadAdditionalDeviceStatus_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
    memcpy(p_pucStream, (uint8*)&p_pResponse->deviceSpecificStatus1, sizeof(p_pResponse->deviceSpecificStatus1));
    p_pucStream += sizeof(p_pResponse->deviceSpecificStatus1);
    *p_pucStream = p_pResponse->extendedDeviceStatus;
    p_pucStream++;
    *p_pucStream = p_pResponse->deviceOperatingMode;
    p_pucStream++;
    *p_pucStream = p_pResponse->standardizedStatus0;
    p_pucStream++;

    if(C048_SIZE_09 == p_pResponse->commandSize) // in this moment this will not be true ... only all 25 bytes are sent ... but in the future could be
    {
        *p_pLen = C048_SIZE_09;
        return RCS_N00_Success;
    }

    *p_pucStream = p_pResponse->standardizedStatus1;
    p_pucStream++;
    *p_pucStream = p_pResponse->analogChannelSaturatedCode;
    p_pucStream++;
    *p_pucStream = p_pResponse->standardizedStatus2;
    p_pucStream++;
    *p_pucStream = p_pResponse->standardizedStatus3;
    p_pucStream++;
    *p_pucStream = p_pResponse->analogChannelFixedCode;
    p_pucStream++;

    if(C048_SIZE_14 == p_pResponse->commandSize)  // in this moment this will not be true ... only all 25 bytes are sent ... but in the future could be
    {
        *p_pLen = C048_SIZE_14;
        return RCS_N00_Success;
    }

    memcpy(p_pucStream, (uint8*)&p_pResponse->deviceSpecificStatus2, sizeof(p_pResponse->deviceSpecificStatus2));
    p_pucStream += sizeof(p_pResponse->deviceSpecificStatus2);

    *p_pLen = C048_SIZE_25;
    return RCS_N00_Success;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// C050
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd050 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C050_ReadDynamicVariableAssignments(C050_ReadDynamicVariableAssignments_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    memcpy((uint8 *)p_pucStream, (uint8*)p_pResponse, MAX_CONFIGURABLE_DYN_VAR_NO);
    *p_pucLen = MAX_CONFIGURABLE_DYN_VAR_NO;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C051
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd051 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C051_WriteDynamicVariableAssignments(C051_WriteDynamicVariableAssignments_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    memcpy((uint8 *)p_pucStream, (uint8*)p_pResponse, MAX_CONFIGURABLE_DYN_VAR_NO);
    *p_pucLen = MAX_CONFIGURABLE_DYN_VAR_NO;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C054
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd054 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pLen - Pointer to response data length.
 */
uint8 Compose_C054_ReadDeviceVarsInfo(C054_ReadDeviceVarsInfo_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen )
{
    // the compiler will use the smallest type required to hold enum constants, preferring signed rather than unsigned
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucDeviceVariableCode, 1);
    p_pucStream += 1;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_ulTransducerSerialNumber, 3);
    p_pucStream += 3;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucUnitsCode, 1);
    p_pucStream += 1;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_fUpperTransducerLimit, 4);
    p_pucStream += 4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_fLowerTransducerLimit, 4);
    p_pucStream += 4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_fDeviceVariableDampingValue, 4);
    p_pucStream += 4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_fDeviceVariableMinimumSpan, 4);
    p_pucStream += 4;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucClassificationCode, 1);
    p_pucStream += 1;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucDeviceVariableFamily, 1);
    p_pucStream += 1;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_ulUpdateTimePeriod, 4);
    p_pucStream += 4;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucDevVarProp, 1);
    p_pucStream += 1;
    *p_pLen = C054_RespSize; // response size for command 54 read device variable information
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C059
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd059 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pLen - Pointer to response data length.
 */
uint8 Compose_C059_WriteNumberOfResponsePreambles(C059_WriteNumberOfResponsePreambles_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucNPreambles, 1);
    p_pucStream++;
    *p_pLen = 1;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C079
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd079 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pLen - Pointer to response data length.
 */
uint8 Compose_C079_WriteDeviceVariable(C079_WriteDeviceVariable_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen)
{
    *p_pucStream = p_pResponse->m_ucDeviceVariableCode;
    p_pucStream++;
    *p_pucStream = p_pResponse->m_ucWriteCommandCode;
    p_pucStream++;
    *p_pucStream = p_pResponse->m_ucUnitsCode;
    p_pucStream++;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_fValue, sizeof(fp32));
    p_pucStream += sizeof(fp32);
    memcpy(p_pucStream, &p_pResponse->m_ucDeviceVariableStatus, sizeof(DeviceVariableStatus));
    *p_pLen = C079_RespSize;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C103
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd103 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C103_WriteBurstPeriod(C103_WriteBurstPeriod_Resp *p_pResponse,uint8* p_pucStream, uint8* p_pucLen )
{
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucBurstMessage, 1);
    p_pucStream++;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_ulUpdatePeriod, 4);
    p_pucStream += 4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_ulMaxUpdatePeriod, 4);
    *p_pucLen = C103_ReqSize; // req size= resp size
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C104
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd104 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C104_WriteBurstTrigger(C104_WriteBurstTrigger_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucBurstMessage, 1);
    p_pucStream++;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucSelectionCode, 1);
    p_pucStream += 1;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucClassificationCode, 1);
    p_pucStream += 1;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucUnitsCode, 1);
    p_pucStream += 1;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_ulTriggerLevel, 4);
    *p_pucLen = C104_ReqSize; // req size= resp size
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C105
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd105 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C105_ReadBurstModeConfiguration(C105_ReadBurstModeConfiguration_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucBurstModeControlCode, 1);
    p_pucStream++;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucCommandNumberExpansionFlag, 1);
    p_pucStream += 1;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_aucDeviceVariableCodes, 8);
    p_pucStream += 8;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucBurstMessage, 1);
    p_pucStream += 1;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucMaxNumberOfBurstMessages, 1);
    p_pucStream += 1;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_unExtendedCommandNumber, 2);
    p_pucStream += 2;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_ulUpdatePeriod, 4);
    p_pucStream += 4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_ulMaxUpdatePeriod, 4);
    p_pucStream += 4;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucSelectionCode, 1);
    p_pucStream += 1;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucClassificationCode, 1);
    p_pucStream += 1;
    memcpy(p_pucStream, (uint8*)&p_pResponse->m_ucUnitsCode, 1);
    p_pucStream += 1;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->m_ulTriggerLevel, 4);
    *p_pucLen = C105_RespSize; // req size != resp size
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C107
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd107 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C107_WriteBurstDeviceVars(C107_WriteBurstDeviceVars_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
uint8 ucIdx;

    *p_pucStream = p_pResponse->m_aDeviceVariableCode[0];
    p_pucStream++;

    for (ucIdx = 1; ucIdx < MAX_DEVICE_VARIABLES; ucIdx++)
    {
        *p_pucStream = p_pResponse->m_aDeviceVariableCode[ucIdx];
        p_pucStream++;
    }

    *p_pucStream = p_pResponse->m_ucBurstMessage;
    *p_pucLen = C107_RespSize;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C108
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd108 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C108_WriteBurstModeCommandNo(C108_WriteBurstModeCommandNo_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
   //m_ucMode - flag to indicate the mode( 0 for backward compatibility or 1 for normal mode)
    if(p_pResponse->m_ucMode == 0) // backward compatibility mode
    {
        *p_pucStream = p_pResponse->m_unCommandNumber;
        *p_pucLen = 1; // size of the response
    }
    else if (p_pResponse->m_ucMode == 1)  // normal compatibility mode
    {
        InverseMemcpy(p_pucStream, (uint8 *)&p_pResponse->m_unCommandNumber, 2);
        p_pucStream += 2;
        *p_pucStream = p_pResponse->m_ucBurstMessage;
        *p_pucLen = 3; // 2 bytes command number and 1 byte command number
    }
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C109
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd109 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C109_BurstModeCtrl(C109_BurstModeCtrl_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    *p_pucStream = p_pResponse->m_burstModeControlCode;
    p_pucStream++;
    *p_pucStream = p_pResponse->m_ucBurstMessage;
    *p_pucLen = 2;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C115
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd115 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C115_ReadEventNotificationSummary(C115_ReadEventNotificationSummary_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
uint8* pucStart = p_pucStream;
uint8 eventData = 0;

    *p_pucStream = p_pResponse->eventNo;
	p_pucStream++;
    *p_pucStream = p_pResponse->noOfEventsSupported;
    p_pucStream++;

	eventData = eventData | p_pResponse->eventStatus;
	eventData = eventData << 4;
	eventData = eventData | p_pResponse->eventNotificationControlCode;

    *p_pucStream = eventData;
    p_pucStream++;

    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->firstUnacknowledged), 4);
    p_pucStream+=4;
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->eventNotificationRetryTime), 4);
    p_pucStream+=4;
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->maximumUpdateTime), 4);
    p_pucStream+=4;
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->eventDeBounceInterval), 4);
    p_pucStream+=4;

    *p_pucStream = p_pResponse->DeviceStatusMask;
    p_pucStream++;

    uint8 i;
	for (i = 0; i < p_pResponse->noOfReadEvents; i++)
	{ // truncatable
        *p_pucStream = p_pResponse->eventMask[i];
        p_pucStream++;
	}

    *p_pucLen = (p_pucStream - pucStart);
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C116
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd116 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C116_WriteEventNotificationBitMask(C116_WriteEventNotificationBitMask_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
uint8* pucStart = p_pucStream;

    *p_pucStream = p_pResponse->eventNo;
	p_pucStream++;
    *p_pucStream = p_pResponse->DeviceStatusMask;
	p_pucStream++;
    
    uint8 i;
	for (i = 0; i < p_pResponse->noOfWriteEvents; i++)
	{ // truncatable
        *p_pucStream = p_pResponse->eventMask[i];
        p_pucStream++;
	}

    *p_pucLen = (p_pucStream - pucStart);
	return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C117
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd117 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C117_WriteEventNotificationTiming(C117_WriteEventNotificationTiming_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
uint8* pucStart = p_pucStream;

    *p_pucStream = p_pResponse->eventNo;
    p_pucStream++;
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->eventNotificationRetryTime), 4);
    p_pucStream+=4;
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->maximumUpdateTime), 4);
    p_pucStream+=4;
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->eventDeBounceInterval), 4);
    p_pucStream+=4;
	*p_pucLen = (p_pucStream - pucStart);
	return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C118
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd118 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C118_EventNotificationControl(C118_EventNotificationControl_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    *p_pucStream = p_pResponse->eventNo;
    p_pucStream++;
     *p_pucStream = p_pResponse->eventNotificationControlCode;
    p_pucStream++;
	*p_pucLen = 2;
	return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C119
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd119 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C119_AcknowledgeEventNotification(C119_AcknowledgeEventNotification_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
uint8* pucStart = p_pucStream;

    *p_pucStream = p_pResponse->eventNo;
    p_pucStream++;
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->firstUnacknowledged), 4);
    p_pucStream+=4;
    InverseMemcpy(p_pucStream, (uint8*)&(p_pResponse->configurationChangedCounter), 2);
    p_pucStream+=2;
    *p_pucStream = p_pResponse->deviceStatus;
    p_pucStream++;
    
    uint8 i;
	for (i = 0; i < MaxLengthOf48Command; i++)
	{ // truncatable
        *p_pucStream = p_pResponse->command48Data[i];
        p_pucStream++;
	}
    *p_pucLen = (p_pucStream - pucStart);
	return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C150
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd150 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C150_ReadAppFwVersion(C150_ReadAppFwVersion_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    *p_pucStream = p_pResponse->ucMajor;
    p_pucStream++;
    *p_pucStream = p_pResponse->ucMinor;
    p_pucStream++;
    *p_pucStream = p_pResponse->ucRevision;
    p_pucStream++;
    *p_pucStream = p_pResponse->ucBuildLetter;
    p_pucStream++;
    *p_pucStream = ' ';
    p_pucStream++;
    memcpy( p_pucStream, p_pResponse->pcDate, sizeof(__DATE__));
    p_pucStream += 11;  //__DATE__
    *p_pucStream = ' ';
    p_pucStream++;
    memcpy( p_pucStream, p_pResponse->pcTime, sizeof(__TIME__));
    p_pucStream += 8; // __TIME__
    *p_pucLen = C150_RespSize;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C151
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd151 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C151_RFEngineeringMode(C151_RFEngineeringMode_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    switch (p_pResponse->m_ucRFTestMode)
    {
    case 0x00: // ENG_MODE_NONE

        *p_pucStream = p_pResponse->m_ucRFTestMode;
        p_pucStream++;
        *p_pucLen = 1;
        break;

#ifdef RF_TEST_MODE_ENABLED
    case 0x01: // ENG_MODE_TX_MOD
    case 0x02: // ENG_MODE_TX_UNMOD
    case 0x04: // ENG_MODE_TX_PACKET

        *p_pucStream = p_pResponse->m_ucRFTestMode;
        p_pucStream++;
        *p_pucStream = p_pResponse->m_ucRFChannel;
        p_pucStream++;
        *p_pucStream = p_pResponse->m_unRFOutputPowerLevel >> 8;
        p_pucStream++;
        *p_pucStream = p_pResponse->m_unRFOutputPowerLevel;
        p_pucStream++;
        *p_pucStream = p_pResponse->m_unTimeout >> 8;
        p_pucStream++;
        *p_pucStream = p_pResponse->m_unTimeout;
        p_pucStream++;
        *p_pucLen = 6;
        break;

    case 0x03: // ENG_MODE_RX

        *p_pucStream = p_pResponse->m_ucRFTestMode;
        p_pucStream++;
        *p_pucStream = p_pResponse->m_ucRFChannel;
        p_pucStream++;
        *p_pucStream = p_pResponse->m_unTimeout >> 8;
        p_pucStream++;
        *p_pucStream = p_pResponse->m_unTimeout;
        p_pucStream++;
        *p_pucLen = 4;
        break;

    case 0x05: // ENG_MODE_RX_PACKET

        *p_pucStream = p_pResponse->m_ucRFTestMode;
        p_pucStream++;
        *p_pucStream = p_pResponse->m_ucRFChannel;
        p_pucStream++;
        memcpy(p_pucStream, p_pResponse->m_aRxEUIFilter, 8);
        p_pucStream += 8;
        *p_pucStream = p_pResponse->m_unTimeout >> 8;
        p_pucStream++;
        *p_pucStream = p_pResponse->m_unTimeout;
        p_pucStream++;
        *p_pucLen = 12;
        break;
#endif

    default:
        return RCS_E02_InvalidSelection;
    }

    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C152
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd152 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C152_EventsManagement(C152_EventsManagement_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    *p_pucStream = p_pResponse->m_ucPassword[0];
    p_pucStream++;
    *p_pucStream = p_pResponse->m_ucPassword[1];
    p_pucStream++;
    *p_pucStream = p_pResponse->m_ucPassword[2];
    p_pucStream++;
    *p_pucStream = p_pResponse->m_ucAction;
    *p_pucLen = C152_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C180_MakeMeasurement(C180_MakeMeasurement_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_fMat_Thick_Raw, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_fMat_Thick_TC, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_fMat_Temp, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_fDSI_Temp, 4); p_pucStream = p_pucStream+4;
 
    *p_pucLen = C180_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C181_WriteUTMParameters(C181_WriteUTMParameters_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_cal_0_offset, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_material_velocity, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_a_scan_start, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_pulsar_width, 2); p_pucStream = p_pucStream+2;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_gain, 2); p_pucStream = p_pucStream+2;
    
    *p_pucLen = C181_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C182_ReadUTMParameters(C182_ReadUTMParameters_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_cal_0_offset, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_material_velocity, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_a_scan_start, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_pulsar_width, 2); p_pucStream = p_pucStream+2;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_gain, 2); p_pucStream = p_pucStream+2;
 
    *p_pucLen = C182_RespSize;
    return RCS_N00_Success;
}
uint8 Compose_C183_WriteUTGParameters(C183_WriteUTGParameters_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g1_start, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g1_width, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g1_thershold, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g2_start, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g2_width, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g2_thershold, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_measurement_mode, 2); p_pucStream = p_pucStream+2;
    
    *p_pucLen = C183_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C184_ReadUTGParameters(C184_ReadUTGParameters_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g1_start, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g1_width, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g1_thershold, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g2_start, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g2_width, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g2_thershold, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_measurement_mode, 2); p_pucStream = p_pucStream+2;
 
    *p_pucLen = C184_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C185_WriteUTTParameters(C185_WriteUTTParameters_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_Ref_temp, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_material_velocity, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_Comp_factor, 4); p_pucStream = p_pucStream+4;
    *p_pucLen = C185_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C186_ReadUTTParameters(C186_ReadUTTParameters_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{

    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_Ref_temp, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_material_velocity, 4); p_pucStream = p_pucStream+4;
    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_Comp_factor, 4); p_pucStream = p_pucStream+4;
    *p_pucLen = C186_RespSize;
    return RCS_N00_Success;
}





////////////////////////////////////////////////////////////////////////////////////////////////////
// C188
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd188 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C188_ResetBatteryLife(C188_ResetBatteryLife_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    memcpy( p_pucStream, &p_pResponse->BatteryLifeDays, sizeof(p_pResponse->BatteryLifeDays));
    *p_pucLen = C778_RespSize;
    return RCS_N00_Success;
}

//uint8 Compose_C190_MakeMeasurement(C190_MakeMeasurement_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
//{
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_fMat_Thick_Raw, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_fMat_Thick_TC, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_fMat_Temp, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_fDSI_Temp, 4); p_pucStream = p_pucStream+4;
// 
//    *p_pucLen = C190_RespSize;
//    return RCS_N00_Success;
//}
//
//uint8 Compose_C191_WriteUTMParameters(C191_WriteUTMParameters_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
//{
////    memcpy (&p_pucStream, &p_pResponse, sizeof(p_pResponse));
//    
////    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_cal_0_offset, 4); p_pucStream = p_pucStream+4;
////    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_material_velocity, 4); p_pucStream = p_pucStream+4;
////    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_a_scan_start, 4); p_pucStream = p_pucStream+4;
////    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_pulsar_width, 2); p_pucStream = p_pucStream+2;
////    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_gain, 2); p_pucStream = p_pucStream+2;
////    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g1_start, 4); p_pucStream = p_pucStream+4;
////    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g1_width, 4); p_pucStream = p_pucStream+4;
////    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g1_thershold, 4); p_pucStream = p_pucStream+4;
////    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g2_start, 4); p_pucStream = p_pucStream+4;
////    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g2_width, 4); p_pucStream = p_pucStream+4;
////    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g2_thershold, 4); p_pucStream = p_pucStream+4;
////    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_measurement_mode, 2); p_pucStream = p_pucStream+2;
//    
//    *p_pucLen = C191_RespSize;
//    return RCS_N00_Success;
//}
//
//uint8 Compose_C192_ReadUTMParameters(C192_ReadUTMParameters_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
//{
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_cal_0_offset, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_material_velocity, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_a_scan_start, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_pulsar_width, 2); p_pucStream = p_pucStream+2;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_gain, 2); p_pucStream = p_pucStream+2;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g1_start, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g1_width, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g1_thershold, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g2_start, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g2_width, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_g2_thershold, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_measurement_mode, 2); p_pucStream = p_pucStream+2;
// 
//    *p_pucLen = C192_RespSize;
//    return RCS_N00_Success;
//}
//
//uint8 Compose_C193_WriteUTTParameters(C193_WriteUTTParameters_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
//{
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_Ref_temp, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_material_velocity, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_Comp_factor, 4); p_pucStream = p_pucStream+4;
//    *p_pucLen = C193_RespSize;
//    return RCS_N00_Success;
//}
//
//uint8 Compose_C194_ReadUTTParameters(C194_ReadUTTParameters_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
//{
//
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_Ref_temp, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_material_velocity, 4); p_pucStream = p_pucStream+4;
//    InverseMemcpy(p_pucStream, (uint8*)&p_pResponse->h_Comp_factor, 4); p_pucStream = p_pucStream+4;
//    *p_pucLen = C194_RespSize;
//    return RCS_N00_Success;
//}

uint8 Compose_C195_AccessPassword(C195_AccessPassword_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    memcpy(p_pucStream, p_pResponse->m_aucPassword, sizeof(p_pResponse->m_aucPassword));
    *p_pucLen = C195_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C196_WritePassword(C196_WritePassword_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    memcpy(p_pucStream, p_pResponse->m_aucLevel1Password, sizeof(p_pResponse->m_aucLevel1Password));
    *p_pucLen = C196_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C197_DisablePassword(C197_DisablePassword_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    *p_pucStream = p_pResponse->m_ucEnableOrDisable;
    *p_pucLen = C197_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C198_WriteAlarmMode(C198_WriteAlarmMode_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    *p_pucStream = p_pResponse->m_ucAlarmMode;
    *p_pucLen = C198_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C201_WriteMSSerialNumber(C201_WriteMSSerialNumber_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    memcpy(p_pucStream, p_pResponse->m_aucMSSerialNumber, sizeof(p_pResponse->m_aucMSSerialNumber));
    *p_pucLen = C201_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C202_ReadMSSerialNumber(C202_ReadMSSerialNumber_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    memcpy(p_pucStream, p_pResponse->m_aucMSSerialNumber, sizeof(p_pResponse->m_aucMSSerialNumber));
   *p_pucLen = C202_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C203_WriteCommissionDate(C203_WriteCommissionDate_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    *p_pucStream++ = p_pResponse->m_ucMonth;
    *p_pucStream++ = p_pResponse->m_ucDay;
    *p_pucStream++ = p_pResponse->m_ucYear;
    *p_pucLen = C203_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C204_ReadCommissionDate(C204_ReadCommissionDate_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    *p_pucStream++ = p_pResponse->m_ucMonth;
    *p_pucStream++ = p_pResponse->m_ucDay;
    *p_pucStream++ = p_pResponse->m_ucYear;
    *p_pucLen = C204_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C205_WriteMeasurementTimeInterval(C205_WriteMeasurementTimeInterval_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    memcpy(p_pucStream, &p_pResponse->m_unMeasurementTimeInterval, sizeof(p_pResponse->m_unMeasurementTimeInterval));
    *p_pucLen = C205_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C206_ReadMeasurementTimeInterval(C206_ReadMeasurementTimeInterval_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    memcpy(p_pucStream, &p_pResponse->m_unMeasurementTimeInterval, sizeof(p_pResponse->m_unMeasurementTimeInterval));
    *p_pucLen = C206_RespSize;
    return RCS_N00_Success;
}

uint8 Compose_C210_SetFactoryDefaults(C210_FactoryDefaults_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    //*p_pucStream = 0x00;
    *p_pucLen = 0x00;
    return RCS_N00_Success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C778
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Composes a Cmd778 Response.
 * @param[in] *p_pResponse - Pointer to the specific command response data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] *p_pucLen - Pointer to response data length.
 */
uint8 Compose_C778_ReadBatteryLife(C778_ReadBatteryLife_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen)
{
    memcpy( p_pucStream, &p_pResponse->BatteryLifeDays, sizeof(p_pResponse->BatteryLifeDays));
    *p_pucLen = C778_RespSize;
    return RCS_N00_Success;
}