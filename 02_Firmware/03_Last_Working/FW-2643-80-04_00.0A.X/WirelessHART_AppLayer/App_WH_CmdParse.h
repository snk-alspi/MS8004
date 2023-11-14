#ifndef __CENTERO_APP_WH_CMD_PARSE_H_
#define __CENTERO_APP_WH_CMD_PARSE_H_


#include "../Global/typedef.h"
#include "App_WH_Tables.h"
#include "App_WH_Types.h"
#include "App_WH_User.h"


uint8 Parse_C001_ReadPrimaryVariable(C001_ReadPrimaryVariable_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C002_ReadLoopCurrAndPercentRange(C002_ReadLoopCurrentAndPercentOfRange_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C003_ReadDynamicVarsAndLoopCurrent(C003_ReadDynamicVariablesAndLoopCurrent_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C006_WritePollingAddress(C006_WritePollingAddress_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C007_ReadLoopConfiguration(C007_ReadLoopConfiguration_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C008_ReadDynamicVarsClassif(void* p_pCommand, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C009_ReadDeviceVarsWithStatus(C009_ReadDeviceVarsWithStatus_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C014_ReadPrimaryVarTransducerInfo(C014_ReadDynamicVariableClassification_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C015_ReadDeviceInfo(C015_ReadDeviceInfo_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C033_ReadDeviceVars(C033_ReadDeviceVars_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C035_WritePVRangeValues(C035_WritePVRangeValues_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C044_WritePVUnits(C044_WritePVUnits_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C048_ReadAdditionalDeviceStatus(C048_ReadAdditionalDeviceStatus_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C050_ReadDynamicVariableAssignments(C050_ReadDynamicVariableAssignments_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C051_WriteDynamicVariableAssignments(C051_WriteDynamicVariableAssignments_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C054_ReadDeviceVarsInfo(C054_ReadDeviceVarsInfo_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C059_WriteNumberOfResponsePreambles(C059_WriteNumberOfResponsePreambles_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C079_WriteDeviceVariable(C079_WriteDeviceVariable_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C103_WriteBurstPeriod(C103_WriteBurstPeriod_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C104_WriteBurstTrigger(C104_WriteBurstTrigger_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C105_ReadBurstModeConfiguration(C105_ReadBurstModeConfiguration_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C107_WriteBurstDeviceVars(C107_WriteBurstDeviceVars_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C108_WriteBurstModeCommandNo(C108_WriteBurstModeCommandNo_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C109_BurstModeCtrl(C109_BurstModeCtrl_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C115_ReadEventNotificationSummary(C115_ReadEventNotificationSummary_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C116_WriteEventNotificationBitMask(C116_WriteEventNotificationBitMask_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C117_WriteEventNotificationTiming(C117_WriteEventNotificationTiming_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C118_EventNotificationControl(C118_EventNotificationControl_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C119_AcknowledgeEventNotification(C119_AcknowledgeEventNotification_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C150_ReadAppFwVersion(C150_ReadAppFwVersion_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C151_RFEngineeringMode(C151_RFEngineeringMode_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C152_EventsManagement(C152_EventsManagement_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C180_MakeMeasurement(C180_MakeMeasurement_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C181_WriteUTMParameters(C181_WriteUTMParameters_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C182_ReadUTMParameters(C182_ReadUTMParameters_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C183_WriteUTGParameters(C183_WriteUTGParameters_Req *pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C184_ReadUTGParameters(C184_ReadUTGParameters_Req *pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C185_WriteUTTParameters(C185_WriteUTTParameters_Req *pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C186_ReadUTTParameters(C186_ReadUTTParameters_Req *pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C188_ResetBatteryLife(C188_ResetBatteryLife_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
//uint8 Parse_C190_MakeMeasurement(C190_MakeMeasurement_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
//uint8 Parse_C191_WriteUTMParameters(C191_WriteUTMParameters_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
//uint8 Parse_C192_ReadUTMParameters(C192_ReadUTMParameters_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
//uint8 Parse_C193_WriteUTTParameters(C193_WriteUTTParameters_Req *pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
//uint8 Parse_C194_ReadUTTParameters(C194_ReadUTTParameters_Req *pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C195_AccessPassword(C195_AccessPassword_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C196_WritePassword(C196_WritePassword_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C197_DisablePassword(C197_DisablePassword_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C198_WriteAlarmMode(C198_WriteAlarmMode_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C201_WriteMSSerialNumber(C201_WriteMSSerialNumber_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C202_ReadMSSerialNumber(C202_ReadMSSerialNumber_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C203_WriteCommissionDate(C203_WriteCommissionDate_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C204_ReadCommissionDate(C204_ReadCommissionDate_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C205_WriteMeasurementTimeInterval(C205_WriteMeasurementTimeInterval_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C206_ReadMeasurementTimeInterval(C206_ReadMeasurementTimeInterval_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);
uint8 Parse_C210_SetFactoryDefaults(C210_FactoryDefaults_Req *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Parse_C778_ReadBatteryLife(C778_ReadBatteryLife_Req *p_pRequest, uint8* p_pucStream, uint8 p_ucCmdSize);

#endif // __CENTERO_APP_WH_CMD_PARSE_H_

