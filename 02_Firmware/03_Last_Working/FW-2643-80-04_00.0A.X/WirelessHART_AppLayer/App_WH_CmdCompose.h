#ifndef __CENTERO_APP_WH_CMD_COMPOSE_H_
#define __CENTERO_APP_WH_CMD_COMPOSE_H_


#include "../Global/typedef.h"
#include "App_WH_Tables.h"
#include "App_WH_Types.h"
#include "App_WH_User.h"


uint8 Compose_C001_ReadPrimaryVariable(C001_ReadPrimaryVariable_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C002_ReadLoopCurrAndPercentRange(C002_ReadLoopCurrentAndPercentOfRange_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C003_ReadDynamicVarsAndLoopCurrent(C003_ReadDynamicVariablesAndLoopCurrent_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C006_WritePollingAddress(C006_WritePollingAddress_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C007_ReadLoopConfiguration(C007_ReadLoopConfiguration_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C008_ReadDynamicVarsClassif(C008_ReadDynamicVariableClassification_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C009_ReadDeviceVarsWithStatus(C009_ReadDeviceVarsWithStatus_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C014_ReadPrimaryVarTransducerInfo(C014_ReadDynamicVariableClassification_Resp  *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C015_ReadDeviceInfo(C015_ReadDeviceInfo_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C033_ReadDeviceVars(C033_ReadDeviceVars_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C035_WritePVRangeValues(C035_WritePVRangeValues_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C044_WritePVUnits(C044_WritePVUnits_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pLen);
uint8 Compose_C048_ReadAdditionalDeviceStatus(C048_ReadAdditionalDeviceStatus_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C050_ReadDynamicVariableAssignments(C050_ReadDynamicVariableAssignments_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C051_WriteDynamicVariableAssignments(C051_WriteDynamicVariableAssignments_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C054_ReadDeviceVarsInfo(C054_ReadDeviceVarsInfo_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C059_WriteNumberOfResponsePreambles(C059_WriteNumberOfResponsePreambles_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C079_WriteDeviceVariable(C079_WriteDeviceVariable_Resp *p_pResponse, uint8* p_pucStream, uint8 *p_pLen);
uint8 Compose_C103_WriteBurstPeriod(C103_WriteBurstPeriod_Resp *p_pResponse,uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C104_WriteBurstTrigger(C104_WriteBurstTrigger_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C105_ReadBurstModeConfiguration(C105_ReadBurstModeConfiguration_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C107_WriteBurstDeviceVars(C107_WriteBurstDeviceVars_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C108_WriteBurstModeCommandNo(C108_WriteBurstModeCommandNo_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C109_BurstModeCtrl(C109_BurstModeCtrl_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C115_ReadEventNotificationSummary(C115_ReadEventNotificationSummary_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C116_WriteEventNotificationBitMask(C116_WriteEventNotificationBitMask_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C117_WriteEventNotificationTiming(C117_WriteEventNotificationTiming_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C118_EventNotificationControl(C118_EventNotificationControl_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C119_AcknowledgeEventNotification(C119_AcknowledgeEventNotification_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C150_ReadAppFwVersion(C150_ReadAppFwVersion_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C151_RFEngineeringMode(C151_RFEngineeringMode_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C152_EventsManagement(C152_EventsManagement_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C180_MakeMeasurement(C180_MakeMeasurement_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C181_WriteUTMParameters(C181_WriteUTMParameters_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C182_ReadUTMParameters(C182_ReadUTMParameters_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C183_WriteUTGParameters(C183_WriteUTGParameters_Resp *pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C184_ReadUTGParameters(C184_ReadUTGParameters_Resp *pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C185_WriteUTTParameters(C185_WriteUTTParameters_Resp *pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C186_ReadUTTParameters(C186_ReadUTTParameters_Resp *pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C188_ResetBatteryLife(C188_ResetBatteryLife_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);
//uint8 Compose_C190_MakeMeasurement(C190_MakeMeasurement_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
//uint8 Compose_C191_WriteUTMParameters(C191_WriteUTMParameters_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
//uint8 Compose_C192_ReadUTMParameters(C192_ReadUTMParameters_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
//uint8 Compose_C193_WriteUTTParameters(C193_WriteUTTParameters_Resp *pRequest, uint8* p_pucStream, uint8* p_pucLen);
//uint8 Compose_C194_ReadUTTParameters(C194_ReadUTTParameters_Resp *pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C195_AccessPassword(C195_AccessPassword_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C196_WritePassword(C196_WritePassword_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C197_DisablePassword(C197_DisablePassword_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C198_WriteAlarmMode(C198_WriteAlarmMode_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C201_WriteMSSerialNumber(C201_WriteMSSerialNumber_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C202_ReadMSSerialNumber(C202_ReadMSSerialNumber_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C203_WriteCommissionDate(C203_WriteCommissionDate_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C204_ReadCommissionDate(C204_ReadCommissionDate_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C205_WriteMeasurementTimeInterval(C205_WriteMeasurementTimeInterval_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C206_ReadMeasurementTimeInterval(C206_ReadMeasurementTimeInterval_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C210_SetFactoryDefaults(C210_FactoryDefaults_Resp *p_pRequest, uint8* p_pucStream, uint8* p_pucLen);
uint8 Compose_C778_ReadBatteryLife(C778_ReadBatteryLife_Resp *p_pResponse, uint8* p_pucStream, uint8* p_pucLen);

#endif // __CENTERO_APP_WH_CMD_COMPOSE_H_


