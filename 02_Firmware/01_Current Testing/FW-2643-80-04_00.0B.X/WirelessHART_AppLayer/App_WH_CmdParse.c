/*************************************************************************
 * File:
 * Description:
 *************************************************************************/

#include <string.h>
#include "../Global/global.h"
#include "../WirelessHART_Full_API/wh_full_api.h"
#include "App_WH_User.h"
#include "App_WH_CmdParse.h"
#include "App_WH_Tables.h"
#include "../Platform/GlobalVariables.h"
#include "../Platform/measurement.h"

extern HRTUT_parameters HRT_Para;
extern MBUT_parameters MB_Para;

////////////////////////////////////////////////////////////////////////////////////////////////////
// C001
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd001 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C001_ReadPrimaryVariable(C001_ReadPrimaryVariable_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    return RCS_N00_Success; // Request Data Bytes: None
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C002
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd002 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C002_ReadLoopCurrAndPercentRange(C002_ReadLoopCurrentAndPercentOfRange_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    return RCS_N00_Success; // Request Data Bytes: None
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C003
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd003 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C003_ReadDynamicVarsAndLoopCurrent(C003_ReadDynamicVariablesAndLoopCurrent_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    return RCS_N00_Success; // Request Data Bytes: None
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C006
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd006 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C006_WritePollingAddress(C006_WritePollingAddress_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if (p_pucStream[0] > 0x3F)
    { // max pooling address in 0x3F ... legal polling addresses are from 0 to 63 ... any other address is invalid
        return RCS_E02_InvalidSelection;
    }

    switch (p_ucCmdSize)
    {
        case 1:
            // older HART master backward compatibility
            p_pRequest->m_ucPollingAddress = p_pucStream[0];
            if (p_pRequest->m_ucPollingAddress == 0)
            {
                p_pRequest->m_ucLoopCurrentMode = LoopCurrent_Enabled;
            }
            else
            {
                p_pRequest->m_ucLoopCurrentMode = LoopCurrent_Disabled;
            }
            break;
        case 2:
            // native revision
            if (p_pucStream[1] > LoopCurrent_Enabled)
            { // legal Loop Current Mode Codes are in Common Table 16.
                return RCS_E02_InvalidSelection;
            }
            p_pRequest->m_ucPollingAddress = p_pucStream[0];
            p_pRequest->m_ucLoopCurrentMode = p_pucStream[1];
            break;
        // default:
        // no need default accept other payload len for forward compatibility ... as HART certif tests requires
    }
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C007
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd007 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C007_ReadLoopConfiguration(C007_ReadLoopConfiguration_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    return RCS_N00_Success; // Request Data Bytes: None
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C008
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd008 Request.
 * @param[in] *p_pCommand - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C008_ReadDynamicVarsClassif(void* p_pCommand, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    return RCS_N00_Success; // Request Data Bytes: None
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C009
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd009 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C009_ReadDeviceVarsWithStatus(C009_ReadDeviceVarsWithStatus_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
uint8 l_ucRespCode = RCS_N00_Success;

    p_pRequest->m_ucNoDeviceVariables = p_ucCmdSize;
    if (
        (p_pRequest->m_ucNoDeviceVariables == 4)
            &&
        ((p_pucStream[0] == 0xFF) && (p_pucStream[1] == 0xFF) && (p_pucStream[2] == 0xFF) && (p_pucStream[3] == 0xFF))
        )
    {
        return RCS_E02_InvalidSelection;
    }

    if (p_pRequest->m_ucNoDeviceVariables <= C009_MAX_REQ_SIZE)
    {
        // Parse Command 9 Request
        memcpy((uint8*)&(p_pRequest->m_aucDeviceVarCode), p_pucStream, p_pRequest->m_ucNoDeviceVariables);
    }
    else
    {
        p_pRequest->m_ucNoDeviceVariables = C009_MAX_REQ_SIZE;
        memcpy((uint8*)&(p_pRequest->m_aucDeviceVarCode), p_pucStream, p_pRequest->m_ucNoDeviceVariables);
        l_ucRespCode = RCM_W30_CommandResponseTruncated;
    }

    return l_ucRespCode;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C014
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd014 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C014_ReadPrimaryVarTransducerInfo(C014_ReadDynamicVariableClassification_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    return RCS_N00_Success; // Request Data Bytes: None
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C015
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd015 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C015_ReadDeviceInfo(C015_ReadDeviceInfo_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    return RCS_N00_Success; // Request Data Bytes: None
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C033
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd033 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C033_ReadDeviceVars(C033_ReadDeviceVars_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    p_pRequest->m_ucNoDeviceVariables = MIN_VALUE(p_ucCmdSize,C033_MAX_REQ_SIZE);
    if (p_pRequest->m_ucNoDeviceVariables <= C033_MAX_REQ_SIZE)
    {
        if (
            (p_pucStream[0] == 0xFF) &&
            (p_pucStream[1] == 0xFF) &&
            (p_pucStream[2] == 0xFF) &&
            (p_pucStream[3] == 0xFF)
        )
        {
            return RCS_E02_InvalidSelection;
        }
        // Parse Command Request
        memcpy((uint8*)&(p_pRequest->m_aucDeviceVarCode), p_pucStream, p_pRequest->m_ucNoDeviceVariables);
    }
    else
    {
        // Error too many vars requested
        return RCS_E60_PayloadTooLong;
    }

    return RCS_N00_Success;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// C035
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd035 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C035_WritePVRangeValues(C035_WritePVRangeValues_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    float fLowerValue, fUpperValue;
    if(p_ucCmdSize < C035_ReqSize)
    {
        return RCS_E05_TooFewDataBytesReceived;
    }

    if(*p_pucStream == UnitsCodes_Millimeters || *p_pucStream == UnitsCodes_Inches )
    {
        p_pRequest->m_ucUnitsCode = *p_pucStream;
        p_pucStream++;
          
    }
    else
    {
        return RCS_E02_InvalidSelection;
    }
    
    InverseMemcpy((uint8*)&fUpperValue, p_pucStream, sizeof(fp32));
    p_pucStream+= sizeof(fp32);
    InverseMemcpy((uint8*)&fLowerValue, p_pucStream, sizeof(fp32));
    
    if(fUpperValue < fLowerValue)        
    {
        return RCS_E02_InvalidSelection;
    }    
    
    if(fLowerValue < PV_LOWER_LIMIT)
    {
        return RCM_E10_LowerRangeValueTooLow;
    }
    
    if(fUpperValue > DEFAULT_PV_UPPER_LIMIT)
    {
        return RCM_E11_UpperRangeValueTooHigh;
    }
    
    memcpy(&p_pRequest->m_fUpperRangeValue, &fUpperValue, sizeof(fUpperValue));
    memcpy(&p_pRequest->m_fLowerRangeValue, &fLowerValue, sizeof(fLowerValue));
    
    return RCS_N00_Success;    
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C044
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd044 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C044_WritePVUnits(C044_WritePVUnits_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize) 
{
    if (p_ucCmdSize < C044_ReqSize) {
        return RCS_E05_TooFewDataBytesReceived;
    }
    if ((*p_pucStream == UnitsCodes_Millimeters) || (*p_pucStream == UnitsCodes_Inches)) {
        p_pRequest->m_ucUnitsCode = *p_pucStream;
        return RCS_N00_Success;
    }

    return RCS_E02_InvalidSelection;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// C048
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd048 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C048_ReadAdditionalDeviceStatus(C048_ReadAdditionalDeviceStatus_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    p_pRequest->commandSize = p_ucCmdSize;

    if (p_pRequest->commandSize == 0)
        return RCS_N00_Success;

    if (p_pRequest->commandSize < C048_SIZE_09)
        return RCS_E05_TooFewDataBytesReceived;

    memcpy((uint8*)&(p_pRequest->deviceSpecificStatus1), p_pucStream, sizeof(p_pRequest->deviceSpecificStatus1) );
    p_pucStream += sizeof(p_pRequest->deviceSpecificStatus1);
	p_pRequest->extendedDeviceStatus = *p_pucStream;
    p_pucStream++;
	p_pRequest->deviceOperatingMode = *p_pucStream;
    p_pucStream++;
	p_pRequest->standardizedStatus0 = *p_pucStream;
    p_pucStream++;

    if (p_pRequest->commandSize == C048_SIZE_09)
        return RCS_N00_Success;

    if (p_pRequest->commandSize < C048_SIZE_14)
        return RCS_E05_TooFewDataBytesReceived;

    p_pRequest->standardizedStatus1 = *p_pucStream;
    p_pucStream++;
    p_pRequest->analogChannelSaturatedCode = *p_pucStream;
    p_pucStream++;
	p_pRequest->standardizedStatus2 = *p_pucStream;
    p_pucStream++;
	p_pRequest->standardizedStatus3 = *p_pucStream;
    p_pucStream++;
    p_pRequest->analogChannelFixedCode = *p_pucStream;
    p_pucStream++;

    if (p_pRequest->commandSize == C048_SIZE_14)
        return RCS_N00_Success;

    uint8 len = p_pRequest->commandSize - C048_SIZE_14;

    if ( len > 11 )
        len = 11;

    uint8 i;
    for (i = 0; i < len; i++)
    {
        p_pRequest->deviceSpecificStatus2[i] = *p_pucStream;;
        p_pucStream++;
    }
    return RCS_N00_Success;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// C050
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd050 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C050_ReadDynamicVariableAssignments(C050_ReadDynamicVariableAssignments_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
  return RCS_N00_Success; // Request Data Bytes: None
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C051
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd051 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C051_WriteDynamicVariableAssignments(C051_WriteDynamicVariableAssignments_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{

    // HCF-SPEC-151: Previously this command was truncatable. In other words, an older Master may only map 1, 2, 3 or
    // 4 Dynamic Variables. The Field Device must answer these Master requests without returning
    // Response Code 5, Too Few Data Bytes Received.. If the Field Device receives 1, 2 or 3 Request
    // Data Bytes, it must return all Response Data Bytes.

    if (p_ucCmdSize > MAX_CONFIGURABLE_DYN_VAR_NO)
    {
        return RCS_E06_DeviceSpecificCommandError;
    }

    switch(p_ucCmdSize)
    {
        case 4:
            if (
                (p_pucStream[QV_IDX] != DEVICE_VARIABLE_0) &&
                (p_pucStream[QV_IDX] != DEVICE_VARIABLE_1) &&
                (p_pucStream[QV_IDX] != DEVICE_VARIABLE_2) &&
                (p_pucStream[QV_IDX] != DEVICE_VARIABLE_3) &&
                (p_pucStream[QV_IDX] != DEVICE_VARIABLE_4) &&
                (p_pucStream[QV_IDX] != DEVICE_VARIABLE_5) &&
                (p_pucStream[QV_IDX] != BatteryLife)
                )
            {
                return RCS_E02_InvalidSelection;
            }
            else
            {
                p_pRequest->m_aucVariables[QV_IDX] = p_pucStream[QV_IDX];
            }
            // no break;
        case 3:
            if (
                (p_pucStream[TV_IDX] != DEVICE_VARIABLE_0) &&
                (p_pucStream[TV_IDX] != DEVICE_VARIABLE_1) &&
                (p_pucStream[TV_IDX] != DEVICE_VARIABLE_2) &&
                (p_pucStream[TV_IDX] != DEVICE_VARIABLE_3) &&
                (p_pucStream[TV_IDX] != DEVICE_VARIABLE_4) &&
                (p_pucStream[TV_IDX] != DEVICE_VARIABLE_5) &&
                (p_pucStream[TV_IDX] != BatteryLife)
                )
            {
                return RCS_E02_InvalidSelection;
            }
            else
            {
                p_pRequest->m_aucVariables[TV_IDX] = p_pucStream[TV_IDX];
            }
            // no break;
        case 2:
            if (
                (p_pucStream[SV_IDX] != DEVICE_VARIABLE_0) &&
                (p_pucStream[SV_IDX] != DEVICE_VARIABLE_1) &&
                (p_pucStream[SV_IDX] != DEVICE_VARIABLE_2) &&
                (p_pucStream[SV_IDX] != DEVICE_VARIABLE_3) &&
                (p_pucStream[SV_IDX] != DEVICE_VARIABLE_4) &&
                (p_pucStream[SV_IDX] != DEVICE_VARIABLE_5) &&
                (p_pucStream[SV_IDX] != BatteryLife)
                )
            {
                return RCS_E02_InvalidSelection;
            }
            else
            {
                p_pRequest->m_aucVariables[SV_IDX] = p_pucStream[SV_IDX];
            }
            // no break;
        case 1:
            if (
                (p_pucStream[PV_IDX] != DEVICE_VARIABLE_0) &&
                (p_pucStream[PV_IDX] != DEVICE_VARIABLE_1) &&
                (p_pucStream[PV_IDX] != DEVICE_VARIABLE_2) &&
                (p_pucStream[PV_IDX] != DEVICE_VARIABLE_3) &&
                (p_pucStream[PV_IDX] != DEVICE_VARIABLE_4) &&
                (p_pucStream[PV_IDX] != DEVICE_VARIABLE_5) &&
                (p_pucStream[PV_IDX] != BatteryLife)
                )
            {
                return RCS_E02_InvalidSelection;
            }
            else
            {
                p_pRequest->m_aucVariables[PV_IDX] = p_pucStream[PV_IDX];
            }
            break;
        default:
            return RCS_E06_DeviceSpecificCommandError;
    }
    p_pRequest->p_ucNoReqDataBytes = p_ucCmdSize;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C054
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd054 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C054_ReadDeviceVarsInfo(C054_ReadDeviceVarsInfo_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    memcpy((uint8*)&p_pRequest->m_ucDeviceVariableCode, p_pucStream, 1);
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C059
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd059 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C059_WriteNumberOfResponsePreambles(C059_WriteNumberOfResponsePreambles_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    p_pRequest->m_ucNPreambles = p_pucStream[0];
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C079
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd079 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C079_WriteDeviceVariable(C079_WriteDeviceVariable_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    p_pRequest->m_ucDeviceVariableCode = *p_pucStream;
    p_pucStream++;
    p_pRequest->m_ucWriteCommandCode = (WriteDeviceVariableCodes)(*p_pucStream);
    p_pucStream++;
    p_pRequest->m_ucUnitsCode = (UnitsCodes)(*p_pucStream);
    p_pucStream++;
    InverseMemcpy((uint8*)&p_pRequest->m_fValue, p_pucStream, sizeof(fp32));
    p_pucStream += sizeof(fp32);
    memcpy(p_pucStream, &p_pRequest->m_ucDeviceVariableStatus, sizeof(DeviceVariableStatus));
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C103
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd103 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C103_WriteBurstPeriod(C103_WriteBurstPeriod_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    memcpy((uint8*)&p_pRequest->m_ucBurstMessage, p_pucStream, 1);
    p_pucStream++;
    InverseMemcpy((uint8*)&p_pRequest->m_ulUpdatePeriod, p_pucStream, 4);
    p_pucStream += 4;
    InverseMemcpy((uint8*)&p_pRequest->m_ulMaxUpdatePeriod, p_pucStream,4);
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C104
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd104 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C104_WriteBurstTrigger(C104_WriteBurstTrigger_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    memcpy((uint8*)&p_pRequest->m_ucBurstMessage, p_pucStream, 1);
    p_pucStream++;
    memcpy((uint8*)&p_pRequest-> m_ucSelectionCode, p_pucStream, 1);
    p_pucStream += 1;
    memcpy((uint8*)&p_pRequest->m_ucClassificationCode, p_pucStream,1);
    p_pucStream += 1;
    memcpy((uint8*)&p_pRequest->m_ucUnitsCode, p_pucStream,1);
    p_pucStream += 1;
    InverseMemcpy((uint8*)&p_pRequest-> m_ulTriggerLevel, p_pucStream, 4);
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C105
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd105 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C105_ReadBurstModeConfiguration(C105_ReadBurstModeConfiguration_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
   if (p_ucCmdSize == 0)
    {
        p_pRequest->m_ucBurstMessage = 0;
        p_pRequest->m_unRequestSize = 0;
        return RCS_N00_Success;
    }
    p_pRequest->m_unRequestSize = 1;
    p_pRequest->m_ucBurstMessage = *p_pucStream;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C107
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd107 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C107_WriteBurstDeviceVars(C107_WriteBurstDeviceVars_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if (p_ucCmdSize == 0)
    {
        return RCS_E02_InvalidSelection;
    }
    if ( (p_ucCmdSize > 4) && (p_ucCmdSize < 9) )
    { // 5,6,7,8
        return RCS_E05_TooFewDataBytesReceived;
    }
    if (p_ucCmdSize > 9)
    {
        p_ucCmdSize = 9;
    }

    memset((uint8*)(p_pRequest->m_aDeviceVariableCode), 0xFA, MAX_DEVICE_VARIABLES);

    if (p_ucCmdSize <= 4)
    { // assume is channel 0
        p_pRequest->m_ucNoOfDeviceVariables = p_ucCmdSize;
        memcpy((uint8*)(p_pRequest->m_aDeviceVariableCode), p_pucStream, p_pRequest->m_ucNoOfDeviceVariables);
        p_pRequest->m_ucBurstMessage = 0;
    }
    else
    {
        p_pRequest->m_ucNoOfDeviceVariables = (uint8)(p_ucCmdSize - 1);
        memcpy((uint8*)(p_pRequest->m_aDeviceVariableCode), p_pucStream, p_pRequest->m_ucNoOfDeviceVariables);
        p_pucStream += p_pRequest->m_ucNoOfDeviceVariables;
        p_pRequest->m_ucBurstMessage = *p_pucStream;
    }

    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C108
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd108 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C108_WriteBurstModeCommandNo(C108_WriteBurstModeCommandNo_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if (p_ucCmdSize == 1)
    {
        p_pRequest->m_ucMode = 0; // backward compatibility mode
        p_pRequest->m_unCommandNumber = *p_pucStream;
        p_pucStream += 1;
        p_pRequest->m_ucBurstMessage  = 0; // backward compatibilty mode
    }
    else if (p_ucCmdSize == 3)
    {
        p_pRequest->m_ucMode = 1; // normal mode
        InverseMemcpy((uint8*)&p_pRequest->m_unCommandNumber, p_pucStream, 2);
        p_pucStream += 2;
        p_pRequest->m_ucBurstMessage  = *p_pucStream;
    }
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C109
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd109 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C109_BurstModeCtrl(C109_BurstModeCtrl_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    p_pRequest->m_burstModeControlCode  = (BURST_MODE_CONTROL_CODE)*p_pucStream++;
    if (p_ucCmdSize == 1)
    {
        p_pRequest->m_ucBurstMessage = 0;
        return RCS_N00_Success;
    }
    p_pRequest->m_ucBurstMessage = *p_pucStream;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C115
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd115 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C115_ReadEventNotificationSummary(C115_ReadEventNotificationSummary_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    p_pRequest->eventNo = *p_pucStream;
    return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C116
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd116 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C116_WriteEventNotificationBitMask(C116_WriteEventNotificationBitMask_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    p_pRequest->eventNo = *p_pucStream;
    p_pucStream++;
	p_pRequest->DeviceStatusMask = *p_pucStream;
    p_pucStream++;

    // This command is truncatable at any point after the Extended Field Device Status
    // (byte 6 of Command 48)
    // after byte 8 in this 116 command
    p_pRequest->noOfWriteEvents = p_ucCmdSize - 2;
    if (p_pRequest->noOfWriteEvents > MaxNoOfWriteEvents)
    {
        p_pRequest->noOfWriteEvents = MaxNoOfWriteEvents;
    }

    uint8 i;
	for (i = 0; i < p_pRequest->noOfWriteEvents; i++)
	{ // truncatable
        p_pRequest->eventMask[i] =  *p_pucStream;
        p_pucStream++;
	}
	return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C117
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd117 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C117_WriteEventNotificationTiming(C117_WriteEventNotificationTiming_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    p_pRequest->eventNo = *p_pucStream;
    p_pucStream++;
    InverseMemcpy((uint8*)&p_pRequest->eventNotificationRetryTime, p_pucStream, 4);
    p_pucStream += 4;
    InverseMemcpy((uint8*)&p_pRequest->maximumUpdateTime, p_pucStream, 4);
    p_pucStream += 4;
    InverseMemcpy((uint8*)&p_pRequest->eventDeBounceInterval, p_pucStream, 4);
    p_pucStream += 4;
	return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C118
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd118 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C118_EventNotificationControl(C118_EventNotificationControl_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    p_pRequest->eventNo = *p_pucStream;
    p_pucStream++;
    p_pRequest->eventNotificationControlCode = (EventNotificationControlCodes) *p_pucStream;
    p_pucStream++;
	return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C119
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd119 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C119_AcknowledgeEventNotification(C119_AcknowledgeEventNotification_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
uint8 ucNo;

    p_pRequest->eventNo = *p_pucStream;
    p_pucStream++;
    InverseMemcpy((uint8*)&p_pRequest->firstUnacknowledged, p_pucStream, 4);
    p_pucStream += 4;
    InverseMemcpy((uint8*)&p_pRequest->configurationChangedCounter, p_pucStream, 2);
    p_pucStream += 2;
    p_pRequest->deviceStatus = *p_pucStream;
    p_pucStream++;
    ucNo = p_ucCmdSize - 8;

    uint8 i;
	for (i = 0; i < ucNo; i++)
	{ // truncatable
        p_pRequest->command48Data[i] =  *p_pucStream;
        p_pucStream++;
	}
	return RCS_N00_Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C150
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd150 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C150_ReadAppFwVersion(C150_ReadAppFwVersion_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    return RCS_N00_Success; // Request bytes: None
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C151
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd151 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C151_RFEngineeringMode(C151_RFEngineeringMode_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
uint8 ucRfEngMode = *p_pucStream++;

    switch (ucRfEngMode)
    {
    case 0x00: // ENG_MODE_NONE

        if (p_ucCmdSize != 1) return RCS_E02_InvalidSelection;
        p_pRequest->m_ucRFTestMode = ucRfEngMode;
        break;

#ifdef RF_TEST_MODE_ENABLED
    case 0x01: // ENG_MODE_TX_MOD
    case 0x02: // ENG_MODE_TX_UNMOD
    case 0x04: // ENG_MODE_TX_PACKET

        if (p_ucCmdSize != 6) return RCS_E02_InvalidSelection;
        p_pRequest->m_ucRFTestMode = ucRfEngMode;
        p_pRequest->m_ucRFChannel = *p_pucStream++;
        InverseMemcpy((uint8*)&p_pRequest->m_unRFOutputPowerLevel, p_pucStream, 2);
        p_pucStream += 2;
        InverseMemcpy((uint8*)&p_pRequest->m_unTimeout, p_pucStream, 2);
        p_pucStream += 2;
        if (p_pRequest->m_ucRFChannel >= 16) return RCS_E02_InvalidSelection;
        break;

    case 0x03: // ENG_MODE_RX

        if (p_ucCmdSize != 4) return RCS_E02_InvalidSelection;
        p_pRequest->m_ucRFTestMode = ucRfEngMode;
        p_pRequest->m_ucRFChannel = *p_pucStream++;
        InverseMemcpy((uint8*)&p_pRequest->m_unTimeout, p_pucStream, 2);
        p_pucStream += 2;
        if (p_pRequest->m_ucRFChannel >= 16) return RCS_E02_InvalidSelection;
        break;

    case 0x05: // ENG_MODE_RX_PACKET

        if (p_ucCmdSize != 12) return RCS_E02_InvalidSelection;
        p_pRequest->m_ucRFTestMode = ucRfEngMode;
        p_pRequest->m_ucRFChannel = *p_pucStream++;
        memcpy((uint8*)(p_pRequest->m_aRxEUIFilter), p_pucStream, 8);
        p_pucStream += 8;
        InverseMemcpy((uint8*)&p_pRequest->m_unTimeout, p_pucStream, 2);
        p_pucStream += 2;
        if (p_pRequest->m_ucRFChannel >= 16) return RCS_E02_InvalidSelection;
        break;
#endif

    default:

        return RCS_E02_InvalidSelection;
    }

    return RCS_N00_Success; // Request bytes: None
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// C152
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd152 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C152_EventsManagement(C152_EventsManagement_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if (p_ucCmdSize > C152_ReqSize)
        return RCS_E02_InvalidSelection;

    p_pRequest->m_ucPassword[0] = *p_pucStream++;
    p_pRequest->m_ucPassword[1] = *p_pucStream++;
    p_pRequest->m_ucPassword[2] = *p_pucStream++;
    p_pRequest->m_ucAction = *p_pucStream++;

    return RCS_N00_Success; // Request bytes: None
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// C180
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C180_MakeMeasurement(C180_MakeMeasurement_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if(c180_stat && g_ucMeasurementInProgress)
    {
        return RCS_E34_DelayedResponseRunning;
    }
    else if(c181_stat || c183_stat || c185_stat)
    {
        return RCS_E36_DelayedResponseConflict;
    }
    else if(g_ucMeasurementInProgress || m_ucHartReqInProgress)
    {
        return RCS_E32_Busy;
    }   
     if (!c180_stat && !g_ucMeasurementInProgress)
    {
      c180_stat=1;
      InverseMemcpy((uint8*)&HRT_Para.channel, p_pucStream, 1); p_pucStream ++;
      ch = HRT_Para.channel;
      strt_measurement(1);
     return RCS_E33_DelayedResponseInitiated;
             
    }
    else if (c180_stat && !g_ucMeasurementInProgress)
    {
      c180_stat=0;             
    }
    
    
    return RCS_N00_Success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C181
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C181_WriteUTMParameters(C181_WriteUTMParameters_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if(g_stDeviceProcessVariables.m_ucPasswordDisabled || !g_ucPasswordMatch)
    {
        return RCS_E16_AccessRestricted;
    }
    if(p_ucCmdSize > C181_ReqSize)
    {
        return RCS_E05_TooFewDataBytesReceived;
    }
    if(c181_stat && m_ucHartReqInProgress)
    {
        return RCS_E34_DelayedResponseRunning;
    }
    else if(c183_stat || c180_stat || c185_stat)
    {
        return RCS_E36_DelayedResponseConflict;
    }
    else if(g_ucMeasurementInProgress || m_ucHartReqInProgress)
    {
        return RCS_E32_Busy;
    }
         
    if (!c181_stat && !m_ucHartReqInProgress)
    {
    
        InverseMemcpy((uint8*)&HRT_Para.channel, p_pucStream, 1); p_pucStream ++;
        ch = HRT_Para.channel;
        InverseMemcpy((uint8*)&HRT_Para.h_fCal_0_offset, p_pucStream, 4); p_pucStream += 4;
        InverseMemcpy((uint8*)&HRT_Para.h_fMaterial_velocity, p_pucStream, 4); p_pucStream += 4;
        InverseMemcpy((uint8*)&HRT_Para.h_fA_scan_start, p_pucStream, 4); p_pucStream += 4;
        InverseMemcpy((uint8*)&HRT_Para.h_uiPulsar_width, p_pucStream, 2); p_pucStream += 2;
        InverseMemcpy((uint8*)&HRT_Para.h_uiGain, p_pucStream, 2); p_pucStream += 2;
        
//        BYTE3(HRT_Para.h_fCal_0_offset)=p_pucStream++; BYTE2(HRT_Para.h_fCal_0_offset)=p_pucStream++; BYTE1(HRT_Para.h_fCal_0_offset)=p_pucStream++; BYTE0(HRT_Para.h_fCal_0_offset)=p_pucStream++;
//        BYTE3(HRT_Para.h_fMaterial_velocity)=p_pucStream++; BYTE2(HRT_Para.h_fMaterial_velocity)=p_pucStream++; BYTE1(HRT_Para.h_fMaterial_velocity)=p_pucStream++; BYTE0(HRT_Para.h_fMaterial_velocity)=p_pucStream++;
//        BYTE3(HRT_Para.h_fA_scan_start)=p_pucStream++; BYTE2(HRT_Para.h_fA_scan_start)=p_pucStream++; BYTE1(HRT_Para.h_fA_scan_start)=p_pucStream++; BYTE0(HRT_Para.h_fA_scan_start)=p_pucStream++;
//        BYTE1(HRT_Para.h_uiPulsar_width)=p_pucStream++; BYTE0(HRT_Para.h_uiPulsar_width)=p_pucStream++;
//        BYTE1(HRT_Para.h_uiGain)=p_pucStream++; BYTE0(HRT_Para.h_uiGain)=p_pucStream++;
    
    if(p_pRequest->h_cal_0_offset > 9999 || p_pRequest->h_material_velocity > 100000 ||
       p_pRequest->h_a_scan_start > 9999 || p_pRequest->h_pulsar_width > 9999 ||
       p_pRequest->h_gain > 9999 )
    {
        return RCS_E03_PassedParameterTooLarge;
    }
    
    if(p_pRequest->h_cal_0_offset < 0 || p_pRequest->h_material_velocity < 0 ||
       p_pRequest->h_a_scan_start < 0 || p_pRequest->h_pulsar_width < -32768 ||
       p_pRequest->h_gain < -32768 )
    {
        return RCS_E04_PassedParameterTooSmall;
    }

     c181_stat=1;
     strt_measurement(2);
     return RCS_E33_DelayedResponseInitiated;
             
    }
    else if (c181_stat && !m_ucHartReqInProgress)
    {
      c181_stat=0;             
    }

    
    return RCS_N00_Success ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C182
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C182_ReadUTMParameters(C182_ReadUTMParameters_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
       
    return RCS_N00_Success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C181
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C183_WriteUTGParameters(C183_WriteUTGParameters_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if(g_stDeviceProcessVariables.m_ucPasswordDisabled || !g_ucPasswordMatch)
    {
        return RCS_E16_AccessRestricted;
    }
    if(p_ucCmdSize > C183_ReqSize)
    {
        return RCS_E05_TooFewDataBytesReceived;
    }
    if(c183_stat && m_ucHartReqInProgress)
    {
        return RCS_E34_DelayedResponseRunning;
    }
    else if(c181_stat || c180_stat || c185_stat)
    {
        return RCS_E36_DelayedResponseConflict;
    }
    else if(g_ucMeasurementInProgress || m_ucHartReqInProgress)
    {
        return RCS_E32_Busy;
    }
         
    if (!c183_stat && !m_ucHartReqInProgress)
    {
        
//       BYTE3(HRT_Para.h_fG1_start)=p_pucStream++; BYTE2(HRT_Para.h_fG1_start)=p_pucStream++; BYTE1(HRT_Para.h_fG1_start)=p_pucStream++; BYTE0(HRT_Para.h_fG1_start)=p_pucStream++;
//        BYTE3(HRT_Para.h_fG1_width)=p_pucStream++; BYTE2(HRT_Para.h_fG1_width)=p_pucStream++; BYTE1(HRT_Para.h_fG1_width)=p_pucStream++; BYTE0(HRT_Para.h_fG1_width)=p_pucStream++;
//        BYTE3(HRT_Para.h_fG1_thershold)=p_pucStream++; BYTE2(HRT_Para.h_fG1_thershold)=p_pucStream++; BYTE1(HRT_Para.h_fG1_thershold)=p_pucStream++; BYTE0(HRT_Para.h_fG1_thershold)=p_pucStream++;
//        BYTE3(HRT_Para.h_fG2_start)=p_pucStream++; BYTE2(HRT_Para.h_fG2_start)=p_pucStream++; BYTE1(HRT_Para.h_fG2_start)=p_pucStream++; BYTE0(HRT_Para.h_fG2_start)=p_pucStream++;
//        BYTE3(HRT_Para.h_fG2_width)=p_pucStream++; BYTE2(HRT_Para.h_fG2_width)=p_pucStream++; BYTE1(HRT_Para.h_fG2_width)=p_pucStream++; BYTE0(HRT_Para.h_fG2_width)=p_pucStream++;
//        BYTE3(HRT_Para.h_fG2_thershold)=p_pucStream++; BYTE2(HRT_Para.h_fG2_thershold)=p_pucStream++; BYTE1(HRT_Para.h_fG2_thershold)=p_pucStream++; BYTE0(HRT_Para.h_fG2_thershold)=p_pucStream++;
//        BYTE1(HRT_Para.h_uiMeasurement_mode)=p_pucStream++; BYTE0(HRT_Para.h_uiMeasurement_mode)=p_pucStream++;

        InverseMemcpy((uint8*)&HRT_Para.channel, p_pucStream, 1); p_pucStream ++; 
        ch = HRT_Para.channel;
        InverseMemcpy((uint8*)&HRT_Para.h_fG1_start, p_pucStream, 4); p_pucStream += 4;
        InverseMemcpy((uint8*)&HRT_Para.h_fG1_width, p_pucStream, 4); p_pucStream += 4;
        InverseMemcpy((uint8*)&HRT_Para.h_fG1_thershold, p_pucStream, 4); p_pucStream += 4;
        InverseMemcpy((uint8*)&HRT_Para.h_fG2_start, p_pucStream, 4); p_pucStream += 4;
        InverseMemcpy((uint8*)&HRT_Para.h_fG2_width, p_pucStream, 4); p_pucStream += 4;
        InverseMemcpy((uint8*)&HRT_Para.h_fG2_thershold, p_pucStream, 4); p_pucStream += 4;
        InverseMemcpy((uint8*)&HRT_Para.h_uiMeasurement_mode, p_pucStream, 2); p_pucStream += 2;
     
    if(p_pRequest->h_g1_start > 9999 ||     
       p_pRequest->h_g1_width > 9999 || p_pRequest->h_g1_thershold > 100.0 ||     
       p_pRequest->h_g2_start > 9999 || p_pRequest->h_g2_width > 9999 ||     
       p_pRequest->h_g2_thershold > 100.0   )
    {
        return RCS_E03_PassedParameterTooLarge;
    }
    
    if(p_pRequest->h_g1_start < 0 ||     
       p_pRequest->h_g1_width < 0 || p_pRequest->h_g1_thershold < -100.0 ||     
       p_pRequest->h_g2_start <0  || p_pRequest->h_g2_width < 0 ||     
       p_pRequest->h_g2_thershold < -100.0   )
    {
        return RCS_E04_PassedParameterTooSmall;
    }
        
     c183_stat=1;
     strt_measurement(5);
     return RCS_E33_DelayedResponseInitiated;
             
    }
    else if (c183_stat && !m_ucHartReqInProgress)
    {
      c183_stat=0;             
    }

    
    return RCS_N00_Success ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C184
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C184_ReadUTGParameters(C184_ReadUTGParameters_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
       
    return RCS_N00_Success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C185
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C185_WriteUTTParameters(C185_WriteUTTParameters_Req *pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if(g_stDeviceProcessVariables.m_ucPasswordDisabled || !g_ucPasswordMatch)
    {
        return RCS_E16_AccessRestricted;
    }
    
    if(g_ucMeasurementInProgress || m_ucHartReqInProgress)
    {
        return RCS_E32_Busy;
    }
     if(p_ucCmdSize > C185_ReqSize)
    {
        return RCS_E05_TooFewDataBytesReceived;
    }
    if(c185_stat && m_ucHartReqInProgress)
    {
        return RCS_E34_DelayedResponseRunning;
    }
    else if(c181_stat || c180_stat || c183_stat )
    {
        return RCS_E36_DelayedResponseConflict;
    }
    else if(g_ucMeasurementInProgress || m_ucHartReqInProgress)
    {
        return RCS_E32_Busy;
    }
         
    if (!c185_stat && !m_ucHartReqInProgress)
    {
         
    InverseMemcpy((uint8*)&HRT_Para.channel, p_pucStream, 1); p_pucStream ++;
    ch = HRT_Para.channel;
    InverseMemcpy((uint8*)&pRequest->h_Ref_temp, p_pucStream, 4); p_pucStream += 4;
    InverseMemcpy((uint8*)&pRequest->h_material_velocity, p_pucStream, 4); p_pucStream += 4;
    InverseMemcpy((uint8*)&pRequest->h_Comp_factor, p_pucStream, 4); p_pucStream += 4;
    
     if(pRequest->h_Ref_temp > 140 || pRequest->h_material_velocity > 0xFFFFFFFF ||
       pRequest->h_Comp_factor > 100   )
    {
        return RCS_E03_PassedParameterTooLarge;
    }
    
    if(pRequest->h_Ref_temp < -40.0 || pRequest->h_material_velocity < 0 ||
       pRequest->h_Comp_factor < -100.0    )
    {
        return RCS_E04_PassedParameterTooSmall;
    }
    
    c185_stat =1 ;
    strt_measurement(4);
    return RCS_E33_DelayedResponseInitiated;
             
    }
    else if (c185_stat && !m_ucHartReqInProgress)
    {
      c185_stat=0;             
    }
        
    

    
    return RCS_N00_Success; // Request bytes: None
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C186
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C186_ReadUTTParameters(C186_ReadUTTParameters_Req *pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
   return RCS_N00_Success; // Request bytes: None
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// C188
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd188 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C188_ResetBatteryLife(C188_ResetBatteryLife_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    return RCS_N00_Success; // Request bytes: None
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// C195
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C195_AccessPassword(C195_AccessPassword_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if(p_ucCmdSize < C195_ReqSize)
    {
        return RCS_E05_TooFewDataBytesReceived;
    }
    
    if(p_ucCmdSize > C195_ReqSize)
    {
        return RCS_E02_InvalidSelection;
    }
    
    memcpy(p_pRequest->m_aucPassword, p_pucStream, sizeof(p_pRequest->m_aucPassword));
    
    return RCS_N00_Success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C196
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C196_WritePassword(C196_WritePassword_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if(g_stDeviceProcessVariables.m_ucPasswordDisabled || !g_ucPasswordMatch)
    {
        return RCS_E07_InWriteProtectMode;
    }
    
    if(p_ucCmdSize < C196_ReqSize)
    {
        return RCS_E05_TooFewDataBytesReceived;
    }
    
    if(p_ucCmdSize > C196_ReqSize)
    {
        return RCS_E02_InvalidSelection;
    }
    
    memcpy((uint8*)&p_pRequest->m_aucLevel1Password, p_pucStream, sizeof(p_pRequest->m_aucLevel1Password));
    
    return RCS_N00_Success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C197
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C197_DisablePassword(C197_DisablePassword_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if(g_stDeviceProcessVariables.m_ucPasswordDisabled && !g_ucPasswordMatch)
    {
        return RCS_E07_InWriteProtectMode;
    }
    
    if(p_ucCmdSize < C197_ReqSize)
    {
        return RCS_E05_TooFewDataBytesReceived;
    }
    
    if(*p_pucStream == 0 || *p_pucStream == 1)
    {
        p_pRequest->m_ucEnableOrDisable = *p_pucStream;
    }
    
    return RCS_N00_Success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C198
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C198_WriteAlarmMode(C198_WriteAlarmMode_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize) 
{
    if (g_stDeviceProcessVariables.m_ucPasswordDisabled || !g_ucPasswordMatch) 
    {
        return RCS_E07_InWriteProtectMode;
    }
    
    if(p_ucCmdSize < C198_ReqSize)
    {
        return RCS_E05_TooFewDataBytesReceived;
    }
    
    if(*p_pucStream == ALARM_MODE_NONE || *p_pucStream == ALARM_MODE_HIGH_AUTO
        || *p_pucStream == ALARM_MODE_LOW_AUTO || *p_pucStream == ALARM_MODE_HIGH_MANUAL
        || *p_pucStream == ALARM_MODE_LOW_MANUAL)
    {
        p_pRequest->m_ucAlarmMode = *p_pucStream;
        return RCS_N00_Success;
    }
    else
    {
        return RCS_E02_InvalidSelection;
    }
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C201
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C201_WriteMSSerialNumber(C201_WriteMSSerialNumber_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if (g_stDeviceProcessVariables.m_ucPasswordDisabled || !g_ucMasterPasswordMatch) 
    {
        return RCS_E07_InWriteProtectMode;
    }
    
    if(p_ucCmdSize < C201_ReqSize)
    {
        return RCS_E05_TooFewDataBytesReceived;
    }
    
    memcpy((uint8*)&p_pRequest->m_aucMSSerialNumber, p_pucStream, sizeof(p_pRequest->m_aucMSSerialNumber));
    
    return RCS_N00_Success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C202
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C202_ReadMSSerialNumber(C202_ReadMSSerialNumber_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if (g_stDeviceProcessVariables.m_ucPasswordDisabled || !g_ucPasswordMatch) 
    {
        return RCS_E16_AccessRestricted;
    }
    
    return RCS_N00_Success; // Request bytes: None
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C203
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C203_WriteCommissionDate(C203_WriteCommissionDate_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if(p_ucCmdSize < C204_ReqSize)
    {
        return RCS_E05_TooFewDataBytesReceived;
    }
        
    if (g_stDeviceProcessVariables.m_ucPasswordDisabled || !g_ucPasswordMatch) 
    {
        return RCS_E07_InWriteProtectMode;
    }
       
//    if(p_ucCmdSize > C204_ReqSize)
//    {
//        return RCS_E02_InvalidSelection;
//    }
    
    p_pRequest->m_ucMonth = *p_pucStream++;
    p_pRequest->m_ucDay = *p_pucStream++;
    p_pRequest->m_ucYear = *p_pucStream++;
    
    return RCS_N00_Success;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// C203
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C204_ReadCommissionDate(C204_ReadCommissionDate_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    if (g_stDeviceProcessVariables.m_ucPasswordDisabled || !g_ucPasswordMatch) 
    {
        return RCS_E16_AccessRestricted;
    }
    
    return RCS_N00_Success; // Request bytes: None
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// C205
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C205_WriteMeasurementTimeInterval(C205_WriteMeasurementTimeInterval_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize) 
{        
    if (p_ucCmdSize < C205_ReqSize)
    {
        return RCS_E05_TooFewDataBytesReceived;
    }
    
    unsigned int unTempMeasurementInterval = 0x00;
    InverseMemcpy((uint8*) & unTempMeasurementInterval, p_pucStream, sizeof (unTempMeasurementInterval));

    if (unTempMeasurementInterval < 0x01)
    {
        return RCM_E10_LowerRangeValueTooLow;
    }

    if (unTempMeasurementInterval > 0xA8) 
    {
        return RCM_E11_UpperRangeValueTooHigh;
    }
    p_pRequest->m_unMeasurementTimeInterval = unTempMeasurementInterval;

    return RCS_N00_Success; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C206
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C206_ReadMeasurementTimeInterval(C206_ReadMeasurementTimeInterval_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    return RCS_N00_Success; // Request bytes: None
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// C210
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Parse_C210_SetFactoryDefaults(C210_FactoryDefaults_Req *p_pRequest, uint8* p_pucStream, uint8* p_pucLen)
{
    if (g_stDeviceProcessVariables.m_ucPasswordDisabled || !g_ucPasswordMatch) 
    {
        return RCS_E07_InWriteProtectMode;
    }
    
    return RCS_N00_Success; // Request bytes: None
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// C778
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Parses a Cmd778 Request.
 * @param[in] *p_pRequest - Pointer to the specific command request data structure.
 * @param[in] *p_pucStream - Pointer to the actual data stream.
 * @param[in] p_ucCmdSize - Request data bytes
 */
uint8 Parse_C778_ReadBatteryLife(C778_ReadBatteryLife_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize)
{
    return RCS_N00_Success; // Request Data Bytes: None
}

