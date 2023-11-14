/*************************************************************************
 * File:
 * Description:
 *************************************************************************/
#ifndef __CENTERO_APP_USER_H_
#define __CENTERO_APP_USER_H_

#include "../Global/typedef.h"
#include "../WirelessHART_Full_API/wh_full_api.h"
#include "../Global/global.h"
#include "../Global/provision.h"
#include "App_WH_Tables.h"
#include "App_WH_Types.h"



#define BURST_TASK_PERIOD_MS	        125
#define EVENT_TASK_PERIOD_MS	        125

#define MAX_APDU_SIZE		            92

#define COMMAND_FOUND                   0
#define COMMAND_NOT_FOUND               1
#define DEV_SPECIFIC_STATUS_1_SIZE      6
#define DEV_SPECIFIC_STATUS_2_SIZE      11

// Definitions for Burst Service Request Task
#define BURST_SRVC_REQ_OK               0
#define BURST_SRVC_REQ_ERR              1


typedef enum
{
    SRVC_REQ_NONE = 0,
    SRVC_REQ_C799 = 1,                                                              // C799 Request Timetable
    SRVC_REQ_C801 = 2                                                               // C801 Delete Timetable
} SRVC_REQ_TYPE_T;

extern Boolean g_flagDynamicRFBWService;

typedef struct
{
    uint8           m_aucRequest[MAX_APDU_SIZE];
    uint8           m_aucResponse[MAX_APDU_SIZE];
    uint32          m_ulSrvcRequestPeriod;      // ... unit = time as spec in WH std = 1/32 of a miliSecond
    uint32          m_ulSrvcInServicePeriod;    // ... unit = miliSeconds
    uint32          m_ulSrvcInServiceMaxPeriod; // ... unit = miliSeconds
    uint16          m_unSrvcCommandNumber;      // 103, 109, 117, 118
    uint8           m_ucSrvcModeControlCode;
    uint8           m_flagSrvcInServiceSyncro;  // align flag in order to concatenate multiples of one of another
    uint8           m_ucResponseDataLen;
    /*SRVC_REQ_TYPE_T*/ uint8 m_ucSrvcReqStatus;
    uint8           m_ucSrvcDenied;
} RF_BANDWIDTH_SERVICE_T;

extern RF_BANDWIDTH_SERVICE_T g_stRFBWService[MAX_BW_SERVICES_NO];                                // Burst configs stored in RAM

extern volatile uint8   g_ucIsEventAck;

#define MASTER_PRIMARY      0
#define MASTER_SECONDARY    1
#define MASTER_WISA         2
#define MASTER_NO           3

extern uint8 g_ucDeviceStatus[MASTER_NO];
extern Boolean g_bNeedSendRsp;                                                      // Flag indicating whether a command response needs to be sent (via full API to the VN radio)
                                                                                    // in the case of a srvc req

typedef uint8 (* ExecuteFunction)(void*, void*, uint8);                             // request, response, command type (wired or wireless)
typedef uint8 (* ParseFunction)(void*, uint8*, uint8);                              // command, input stream, command lenght
typedef uint8 (* ComposeFunction)(void*, uint8*, uint8*);                           // command, input stream, size

typedef struct
{
    uint16 cmdId;
    uint8 cmdSize;
    ParseFunction cmdParse;
    ExecuteFunction cmdExec;
    ComposeFunction cmdCompose;
} CmdEntry;

typedef union
{
  C048_ReadAdditionalDeviceStatus_Req data;
  uint8 byte[25];
} Cmd48Data;

extern const CmdEntry g_astCmdTable[];

extern Cmd48Data g_stCmd48Data;


void App_UserInit();                                                                    // Init the g_aucRequest[] and g_aucResponse[], used in the case of SW reset
void App_BurstInit();                                                                   // Initializations for burst task (burst periods)
void APP_InitialRFBWServiceTask();                                                      // Process the burst config from NVM and send the service requests for the active bursts
void APP_DynamicRFBWServiceTask();

void App_EventTask();                                                                   // Scan Events and if found one add it to Evt Queue
void App_BurstTask();                                                                   // Burst task
void App_BurstOneSecTask();                                                             // Burst 1 second task


uint8 App_GetSrvcReq(uint16 p_unCmdId, uint8 p_ucBurstMessage, uint32 p_ulBurstPeriod, uint8 p_ucSource);
uint8 App_GetSrvcRsp( uint8* p_ucRcvDataBuff, uint8 p_ucMsgSize);
void  App_DelSrvcReq(uint16 p_unCmdId, uint8 p_ucBurstMessage);

uint8 App_RcvTunnelledCmd (uint8 p_ucCmdType, uint8 p_ucMsgId, uint8 * p_pucCmdData);
void App_FlushBurstMsg( uint8* p_ucRcvDataBuff, uint8 p_ucMsgSize);
void App_UpdateBurstPeriod (uint8* p_ucRcvDataBuff);
void App_AsyncOperation (uint8* p_ucRcvDataBuff);
void App_GetChangeCounter (uint8* p_ucRcvDataBuff);
void App_OnBlockTransfer (uint8* p_ucRcvDataBuff, uint8 p_ucMsgID, uint8 p_ucMsgSize);
uint8 App_WriteAdditionalDeviceStatus(void);

DEVICE_VARIABLE_T* App_TriggerSorceVal(uint8 p_ucCode);                                 // Return the trigger sorce value
NV_DEVICE_VARIABLE_T* App_GetDeviceVar(uint8 p_ucCode);                                 // Returns a pointer to a Device Variable's properties (structure saved in NVM memory)
DEVICE_VARIABLE_T* App_GetDeviceVarValue(uint8 p_ucCode);                               // Returns a pointer to a Device Variable's current value, based on the code

float ConvertTemperatureVariable(float value, uint8 oldUnit, uint8 newUnit);
float ConvertToNewUnit(uint8 p_ucCode, float value, uint8 oldUnit, uint8 newUnit);

uint8 App_CommandSupported(uint16 p_unCommandNo, uint16 *p_unTableIndex);               // Checks if a command is supported by the AP
uint8 App_ParseCommand(uint8 p_ucCmdType, uint8 *p_pucInBuffer, void *p_pParsedCmd, uint16 *p_punCmdId, uint8 *p_pIndex);
uint8 App_ComposeCommand(uint8 p_ucCmdType, void *p_pucInBuffer, uint8 *p_pComposedCmd, uint16 p_unCmdId, uint8 p_ucRespCode, uint8 p_ucIndex, uint8 *p_ucDataLen);


#define SET_CONFIGURATION_CHANGED() { \
                                        g_ucDeviceStatus[MASTER_WISA] |= FieldDeviceStatusesMask_ConfigurationChanged; \
                                        g_ucDeviceStatus[MASTER_SECONDARY] |= FieldDeviceStatusesMask_ConfigurationChanged; \
                                        g_ucDeviceStatus[MASTER_PRIMARY] |= FieldDeviceStatusesMask_ConfigurationChanged; \
                                     }


#endif // __CENTERO_APP_USER_H_
