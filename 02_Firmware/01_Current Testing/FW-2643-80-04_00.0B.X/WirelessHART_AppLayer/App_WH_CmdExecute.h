#ifndef __CENTERO_APP_WH_CMD_EXECUTE_H_
#define __CENTERO_APP_WH_CMD_EXECUTE_H_


#include "../Global/typedef.h"
#include "App_WH_Tables.h"
#include "App_WH_Types.h"
#include "App_WH_User.h"


uint8 Execute_C001_ReadPrimaryVariable(C001_ReadPrimaryVariable_Req *p_pRequest, C001_ReadPrimaryVariable_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C002_ReadLoopCurrAndPercentRange(C002_ReadLoopCurrentAndPercentOfRange_Req *p_pRequest, C002_ReadLoopCurrentAndPercentOfRange_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C003_ReadDynamicVarsAndLoopCurrent(C003_ReadDynamicVariablesAndLoopCurrent_Req *p_pRequest, C003_ReadDynamicVariablesAndLoopCurrent_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C006_WritePollingAddress(C006_WritePollingAddress_Req *p_pRequest, C006_WritePollingAddress_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C007_ReadLoopConfiguration(C007_ReadLoopConfiguration_Req *p_pRequest, C007_ReadLoopConfiguration_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C008_ReadDynamicVarsClassif(C008_ReadDynamicVariableClassification_Req *p_pRequest, C008_ReadDynamicVariableClassification_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C009_ReadDeviceVarsWithStatus(C009_ReadDeviceVarsWithStatus_Req *p_pRequest, C009_ReadDeviceVarsWithStatus_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C014_ReadPrimaryVarTransducerInfo(C014_ReadDynamicVariableClassification_Req *p_pRequest, C014_ReadDynamicVariableClassification_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C015_ReadDeviceInfo(C015_ReadDeviceInfo_Req *p_pRequest, C015_ReadDeviceInfo_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C033_ReadDeviceVars(C033_ReadDeviceVars_Req *p_pRequest, C033_ReadDeviceVars_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C035_WritePVRangeValues(C035_WritePVRangeValues_Req *p_pRequest, C035_WritePVRangeValues_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C044_WritePVUnits(C044_WritePVUnits_Req *p_pRequest, C044_WritePVUnits_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C048_ReadAdditionalDeviceStatus(C048_ReadAdditionalDeviceStatus_Req *p_pRequest, C048_ReadAdditionalDeviceStatus_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C050_ReadDynamicVariableAssignments(C050_ReadDynamicVariableAssignments_Req *p_pRequest, C050_ReadDynamicVariableAssignments_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C051_WriteDynamicVariableAssignments(C051_WriteDynamicVariableAssignments_Req *p_pRequest, C051_WriteDynamicVariableAssignments_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C054_ReadDeviceVarsInfo(C054_ReadDeviceVarsInfo_Req *p_pRequest, C054_ReadDeviceVarsInfo_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C059_WriteNumberOfResponsePreambles(C059_WriteNumberOfResponsePreambles_Req *p_pRequest, C059_WriteNumberOfResponsePreambles_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C079_WriteDeviceVariable(C079_WriteDeviceVariable_Req *p_pRequest, C079_WriteDeviceVariable_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C103_WriteBurstPeriod(C103_WriteBurstPeriod_Req *p_pRequest, C103_WriteBurstPeriod_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C104_WriteBurstTrigger(C104_WriteBurstTrigger_Req *p_pRequest, C104_WriteBurstTrigger_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C105_ReadBurstModeConfiguration(C105_ReadBurstModeConfiguration_Req *p_pRequest, C105_ReadBurstModeConfiguration_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C107_WriteBurstDeviceVars(C107_WriteBurstDeviceVars_Req *p_pRequest, C107_WriteBurstDeviceVars_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C108_WriteBurstModeCommandNo(C108_WriteBurstModeCommandNo_Req *p_pRequest, C108_WriteBurstModeCommandNo_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C109_BurstModeCtrl(C109_BurstModeCtrl_Req *p_pRequest, C109_BurstModeCtrl_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C115_ReadEventNotificationSummary(C115_ReadEventNotificationSummary_Req *p_pRequest, C115_ReadEventNotificationSummary_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C116_WriteEventNotificationBitMask(C116_WriteEventNotificationBitMask_Req *p_pRequest, C116_WriteEventNotificationBitMask_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C117_WriteEventNotificationTiming(C117_WriteEventNotificationTiming_Req *p_pRequest, C117_WriteEventNotificationTiming_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C118_EventNotificationControl(C118_EventNotificationControl_Req *p_pRequest, C118_EventNotificationControl_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C119_AcknowledgeEventNotification(C119_AcknowledgeEventNotification_Req *p_pRequest, C119_AcknowledgeEventNotification_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C150_ReadAppFwVersion(C150_ReadAppFwVersion_Req *p_pRequest, C150_ReadAppFwVersion_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C151_RFEngineeringMode(C151_RFEngineeringMode_Req *p_pRequest, C151_RFEngineeringMode_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C152_EventsManagement(C152_EventsManagement_Req *p_pRequest, C152_EventsManagement_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C180_MakeMeasurement(C180_MakeMeasurement_Req *p_pRequest, C180_MakeMeasurement_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C181_WriteUTMParameters(C181_WriteUTMParameters_Req *p_pRequest, C181_WriteUTMParameters_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C182_ReadUTMParameters(C182_ReadUTMParameters_Req *p_pRequest, C182_ReadUTMParameters_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C183_WriteUTGParameters(C183_WriteUTGParameters_Req *pRequest, C183_WriteUTGParameters_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C184_ReadUTGParameters(C184_ReadUTGParameters_Req *pRequest, C184_ReadUTGParameters_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C185_WriteUTTParameters(C185_WriteUTTParameters_Req *pRequest, C185_WriteUTTParameters_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C186_ReadUTTParameters(C186_ReadUTTParameters_Req *pRequest, C186_ReadUTTParameters_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C188_ResetBatteryLife(C188_ResetBatteryLife_Req *p_pRequest, C188_ResetBatteryLife_Resp *p_pResponse, uint8 p_ucCmdType);
//uint8 Execute_C190_MakeMeasurement(C190_MakeMeasurement_Req *p_pRequest, C190_MakeMeasurement_Resp *p_pResponse, uint8 p_ucCmdType);
//uint8 Execute_C191_WriteUTMParameters(C191_WriteUTMParameters_Req *p_pRequest, C191_WriteUTMParameters_Resp *p_pResponse, uint8 p_ucCmdType);
//uint8 Execute_C192_ReadUTMParameters(C192_ReadUTMParameters_Req *p_pRequest, C192_ReadUTMParameters_Resp *p_pResponse, uint8 p_ucCmdType);
//uint8 Execute_C193_WriteUTTParameters(C193_WriteUTTParameters_Req *pRequest, C193_WriteUTTParameters_Resp *p_pResponse, uint8 p_ucCmdType);
//uint8 Execute_C194_ReadUTTParameters(C194_ReadUTTParameters_Req *pRequest, C194_ReadUTTParameters_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C195_AccessPassword(C195_AccessPassword_Req *p_pRequest, C195_AccessPassword_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C196_WritePassword(C196_WritePassword_Req *p_pRequest, C196_WritePassword_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C197_DisablePassword(C197_DisablePassword_Req *p_pRequest, C197_DisablePassword_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C198_WriteAlarmMode(C198_WriteAlarmMode_Req *p_pRequest, C198_WriteAlarmMode_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C201_WriteMSSerialNumber(C201_WriteMSSerialNumber_Req *p_pRequest, C201_WriteMSSerialNumber_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C202_ReadMSSerialNumber(C202_ReadMSSerialNumber_Req *p_pRequest, C202_ReadMSSerialNumber_Resp *p_pResponse, uint8 p_ucCmdType );
uint8 Execute_C203_WriteCommissionDate(C203_WriteCommissionDate_Req *p_pRequest, C203_WriteCommissionDate_Resp  *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C204_ReadCommissionDate(C204_ReadCommissionDate_Req *p_pRequest, C204_ReadCommissionDate_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C205_WriteMeasurementTimeInterval(C205_WriteMeasurementTimeInterval_Req *p_pRequest, C205_WriteMeasurementTimeInterval_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C206_ReadMeasurementTimeInterval(C206_ReadMeasurementTimeInterval_Req *p_pRequest, C206_ReadMeasurementTimeInterval_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C210_SetFactoryDefaults(C210_FactoryDefaults_Req *p_pRequest, C210_FactoryDefaults_Resp *p_pResponse, uint8 p_ucCmdType);
uint8 Execute_C778_ReadBatteryLife(C778_ReadBatteryLife_Req *p_pRequest, C778_ReadBatteryLife_Resp *p_pResponse, uint8 p_ucCmdType);
#endif // __CENTERO_APP_WH_CMD_EXECUTE_H_