/*************************************************************************
 * File:
 * Description:
 *************************************************************************/

#include <string.h>
#include "../WirelessHART_Full_API/wh_full_api.h"
#include "../WirelessHART_Full_API/wh_full_api_queue.h"
#include "../WiredHart/wired_hart_link_layer.h"
#include "../WiredHart/wired_hart_app_layer.h"
#include "../Platform/nvm.h"
#include "../Platform/gpio.h"
#include "../Platform/timers.h"
#include "../Platform/uart.h"
#include "../MS/MS_Data.h"
#include "App_WH_Tables.h"
#include "App_WH_Types.h"
#include "App_WH_User.h"
#include "App_WH_CmdParse.h"
#include "App_WH_CmdExecute.h"
#include "App_WH_CmdCompose.h"
#include "../Platform/GlobalVariables.h"



//#pragma data_alignment = 2
static uint8 g_aucRequest[100]; ///< Request buffer.
//#pragma data_alignment = 2
static uint8 g_aucResponse[100]; ///< Response buffer.

Boolean g_flagInitialRFBWService; ///< Initial RF bandwidth service execution flag.
Boolean g_flagDynamicRFBWService; ///< Dynamic RF bandwidth service execution flag.
RF_BANDWIDTH_SERVICE_T g_stRFBWService[MAX_BW_SERVICES_NO];    ///< RF Bandwidth configuration stored in RAM


EVENT_PENDING_T  g_stEventQueue[MAX_EVENTS_PENDING];
EVENT_PENDING_T* g_pEventQueueEnd;

uint16 g_unMirrorCCCounter = 0;

volatile uint8   g_ucIsEventAck = 0;

Boolean g_bNeedSendRsp; ///< Flag indicating whether a command response needs to be sent (via full API to the WISA radio) in the case of a service request

uint8 g_ucDeviceStatus[MASTER_NO] = { 0, 0, 0}; ///< Device status


Cmd48Data g_stCmd48Data;  ///< Additional Device Status structure
Cmd48Data g_stEventsScanLatched; //

const CmdEntry g_astCmdTable[] =
{
    {CMDID_C001_ReadPV,                           C001_ReqSize, (ParseFunction)&Parse_C001_ReadPrimaryVariable,             (ExecuteFunction)&Execute_C001_ReadPrimaryVariable,             (ComposeFunction)&Compose_C001_ReadPrimaryVariable              },
    {CMDID_C002_ReadLoopCurrAndPercentRange,      C002_ReqSize, (ParseFunction)&Parse_C002_ReadLoopCurrAndPercentRange,     (ExecuteFunction)&Execute_C002_ReadLoopCurrAndPercentRange,     (ComposeFunction)&Compose_C002_ReadLoopCurrAndPercentRange      },
    {CMDID_C003_ReadDynamicVarsAndLoopCurrent,    C003_ReqSize, (ParseFunction)&Parse_C003_ReadDynamicVarsAndLoopCurrent,   (ExecuteFunction)&Execute_C003_ReadDynamicVarsAndLoopCurrent,   (ComposeFunction)&Compose_C003_ReadDynamicVarsAndLoopCurrent    },
    {CMDID_C006_WritePollingAddress,              C006_ReqSize, (ParseFunction)&Parse_C006_WritePollingAddress,             (ExecuteFunction)&Execute_C006_WritePollingAddress,             (ComposeFunction)&Compose_C006_WritePollingAddress              },
    {CMDID_C007_ReadLoopConfiguration,            C007_ReqSize, (ParseFunction)&Parse_C007_ReadLoopConfiguration,           (ExecuteFunction)&Execute_C007_ReadLoopConfiguration,           (ComposeFunction)&Compose_C007_ReadLoopConfiguration            },
    {CMDID_C008_ReadDynamicVarsClassif,           C008_ReqSize, (ParseFunction)&Parse_C008_ReadDynamicVarsClassif,          (ExecuteFunction)&Execute_C008_ReadDynamicVarsClassif,          (ComposeFunction)&Compose_C008_ReadDynamicVarsClassif           },
    {CMDID_C009_ReadDeviceVarsWithStatus,         C009_ReqSize, (ParseFunction)&Parse_C009_ReadDeviceVarsWithStatus,        (ExecuteFunction)&Execute_C009_ReadDeviceVarsWithStatus,        (ComposeFunction)&Compose_C009_ReadDeviceVarsWithStatus         },
    {CMDID_C014_ReadPrimaryVarTransducerInfo,     C014_ReqSize, (ParseFunction)&Parse_C014_ReadPrimaryVarTransducerInfo,    (ExecuteFunction)&Execute_C014_ReadPrimaryVarTransducerInfo,    (ComposeFunction)&Compose_C014_ReadPrimaryVarTransducerInfo     },
    {CMDID_C015_ReadDeviceInfo,                   C015_ReqSize, (ParseFunction)&Parse_C015_ReadDeviceInfo,                  (ExecuteFunction)&Execute_C015_ReadDeviceInfo,                  (ComposeFunction)&Compose_C015_ReadDeviceInfo                   },
    {CMDID_C033_ReadDeviceVars,                   C033_ReqSize, (ParseFunction)&Parse_C033_ReadDeviceVars,                  (ExecuteFunction)&Execute_C033_ReadDeviceVars,                  (ComposeFunction)&Compose_C033_ReadDeviceVars                   },
    {CMDID_C035_WritePVRangeValues,               C035_ReqSize, (ParseFunction)&Parse_C035_WritePVRangeValues,              (ExecuteFunction)&Execute_C035_WritePVRangeValues,              (ComposeFunction)&Compose_C035_WritePVRangeValues               },
    {CMDID_C044_WritePVUnits,                     C044_ReqSize, (ParseFunction)&Parse_C044_WritePVUnits,                    (ExecuteFunction)&Execute_C044_WritePVUnits,                    (ComposeFunction)&Compose_C044_WritePVUnits                     },
    {CMDID_C048_ReadAdditionalDeviceStatus,       C048_ReqSize, (ParseFunction)&Parse_C048_ReadAdditionalDeviceStatus,      (ExecuteFunction)&Execute_C048_ReadAdditionalDeviceStatus,      (ComposeFunction)&Compose_C048_ReadAdditionalDeviceStatus       },
    {CMDID_C050_ReadDynamicVariableAssignments,   C050_ReqSize, (ParseFunction)&Parse_C050_ReadDynamicVariableAssignments,  (ExecuteFunction)&Execute_C050_ReadDynamicVariableAssignments,  (ComposeFunction)&Compose_C050_ReadDynamicVariableAssignments   },
    {CMDID_C051_WriteDynamicVariableAssignments,  C051_ReqSize, (ParseFunction)&Parse_C051_WriteDynamicVariableAssignments, (ExecuteFunction)&Execute_C051_WriteDynamicVariableAssignments, (ComposeFunction)&Compose_C051_WriteDynamicVariableAssignments  },
    {CMDID_C054_ReadDeviceVariableInformation,    C054_ReqSize, (ParseFunction)&Parse_C054_ReadDeviceVarsInfo,              (ExecuteFunction)&Execute_C054_ReadDeviceVarsInfo,              (ComposeFunction)&Compose_C054_ReadDeviceVarsInfo               },
    {CMDID_C059_WriteNumberOfResponsePreambles,   C059_ReqSize, (ParseFunction)&Parse_C059_WriteNumberOfResponsePreambles,  (ExecuteFunction)&Execute_C059_WriteNumberOfResponsePreambles,  (ComposeFunction)&Compose_C059_WriteNumberOfResponsePreambles   },
    {CMDID_C079_WriteDeviceVariable_Req,          C079_ReqSize, (ParseFunction)&Parse_C079_WriteDeviceVariable,             (ExecuteFunction)&Execute_C079_WriteDeviceVariable,             (ComposeFunction)&Compose_C079_WriteDeviceVariable              },
    {CMDID_C103_WriteBurstPeriod,                 C103_ReqSize, (ParseFunction)&Parse_C103_WriteBurstPeriod,                (ExecuteFunction)&Execute_C103_WriteBurstPeriod,                (ComposeFunction)&Compose_C103_WriteBurstPeriod                 },
    {CMDID_C104_WriteBurstTrigger,                C104_ReqSize, (ParseFunction)&Parse_C104_WriteBurstTrigger,               (ExecuteFunction)&Execute_C104_WriteBurstTrigger,               (ComposeFunction)&Compose_C104_WriteBurstTrigger                },
    {CMDID_C105_ReadBurstModeConfiguration,       C105_ReqSize, (ParseFunction)&Parse_C105_ReadBurstModeConfiguration,      (ExecuteFunction)&Execute_C105_ReadBurstModeConfiguration,      (ComposeFunction)&Compose_C105_ReadBurstModeConfiguration       },
    {CMDID_C107_WriteBurstDeviceVars,             C107_ReqSize, (ParseFunction)&Parse_C107_WriteBurstDeviceVars,            (ExecuteFunction)&Execute_C107_WriteBurstDeviceVars,            (ComposeFunction)&Compose_C107_WriteBurstDeviceVars             },
    {CMDID_C108_WriteBurstModeCommandNo,          C108_ReqSize, (ParseFunction)&Parse_C108_WriteBurstModeCommandNo,         (ExecuteFunction)&Execute_C108_WriteBurstModeCommandNo,         (ComposeFunction)&Compose_C108_WriteBurstModeCommandNo          },
    {CMDID_C109_BurstModeCtrl,                    C109_ReqSize, (ParseFunction)&Parse_C109_BurstModeCtrl,                   (ExecuteFunction)&Execute_C109_BurstModeCtrl,                   (ComposeFunction)&Compose_C109_BurstModeCtrl                    },
    {CMDID_C115_ReadEventNotificationSummary,     C115_ReqSize, (ParseFunction)&Parse_C115_ReadEventNotificationSummary,    (ExecuteFunction)&Execute_C115_ReadEventNotificationSummary,    (ComposeFunction)&Compose_C115_ReadEventNotificationSummary     },
    {CMDID_C116_WriteEventNotificationBitMask,    C116_ReqSize, (ParseFunction)&Parse_C116_WriteEventNotificationBitMask,   (ExecuteFunction)&Execute_C116_WriteEventNotificationBitMask,   (ComposeFunction)&Compose_C116_WriteEventNotificationBitMask    },
    {CMDID_C117_WriteEventNotificationTiming,     C117_ReqSize, (ParseFunction)&Parse_C117_WriteEventNotificationTiming,    (ExecuteFunction)&Execute_C117_WriteEventNotificationTiming,    (ComposeFunction)&Compose_C117_WriteEventNotificationTiming     },
    {CMDID_C118_EventNotificationControl,         C118_ReqSize, (ParseFunction)&Parse_C118_EventNotificationControl,        (ExecuteFunction)&Execute_C118_EventNotificationControl,        (ComposeFunction)&Compose_C118_EventNotificationControl         },
    {CMDID_C119_AcknowledgeEventNotification,     C119_ReqSize, (ParseFunction)&Parse_C119_AcknowledgeEventNotification,    (ExecuteFunction)&Execute_C119_AcknowledgeEventNotification,    (ComposeFunction)&Compose_C119_AcknowledgeEventNotification     },
    {CMDID_C150_ReadAppFwVersion,                 C150_ReqSize, (ParseFunction)&Parse_C150_ReadAppFwVersion,                (ExecuteFunction)&Execute_C150_ReadAppFwVersion,                (ComposeFunction)&Compose_C150_ReadAppFwVersion                 },
    {CMDID_C151_RFEngineeringMode,                C151_ReqSize, (ParseFunction)&Parse_C151_RFEngineeringMode,               (ExecuteFunction)&Execute_C151_RFEngineeringMode,               (ComposeFunction)&Compose_C151_RFEngineeringMode                },
    {CMDID_C152_EventsManagement,                 C152_ReqSize, (ParseFunction)&Parse_C152_EventsManagement,                (ExecuteFunction)&Execute_C152_EventsManagement,                (ComposeFunction)&Compose_C152_EventsManagement                 },
    {CMDID_C180_MakeMeasurement,                  C180_ReqSize, (ParseFunction)&Parse_C180_MakeMeasurement,                 (ExecuteFunction)&Execute_C180_MakeMeasurement,                 (ComposeFunction)&Compose_C180_MakeMeasurement                  },
    {CMDID_C181_WriteUTMParameters,               C181_ReqSize, (ParseFunction)&Parse_C181_WriteUTMParameters,              (ExecuteFunction)&Execute_C181_WriteUTMParameters,              (ComposeFunction)&Compose_C181_WriteUTMParameters               },
    {CMDID_C182_ReadUTMParameters,                C182_ReqSize, (ParseFunction)&Parse_C182_ReadUTMParameters,               (ExecuteFunction)&Execute_C182_ReadUTMParameters,               (ComposeFunction)&Compose_C182_ReadUTMParameters                },
    {CMDID_C183_WriteUTGParameters,               C183_ReqSize, (ParseFunction)&Parse_C183_WriteUTGParameters,              (ExecuteFunction)&Execute_C183_WriteUTGParameters,              (ComposeFunction)&Compose_C183_WriteUTGParameters               },
    {CMDID_C184_ReadUTGParameters,                C184_ReqSize, (ParseFunction)&Parse_C184_ReadUTGParameters,               (ExecuteFunction)&Execute_C184_ReadUTGParameters,               (ComposeFunction)&Compose_C184_ReadUTGParameters                },
    {CMDID_C185_WriteUTTParameters,               C185_ReqSize, (ParseFunction)&Parse_C185_WriteUTTParameters,              (ExecuteFunction)&Execute_C185_WriteUTTParameters,              (ComposeFunction)&Compose_C185_WriteUTTParameters               },
    {CMDID_C186_ReadUTTParameters,                C186_ReqSize, (ParseFunction)&Parse_C186_ReadUTTParameters,               (ExecuteFunction)&Execute_C186_ReadUTTParameters,               (ComposeFunction)&Compose_C186_ReadUTTParameters                },
    {CMDID_C188_ResetBatteryLife,                 C188_ReqSize, (ParseFunction)&Parse_C188_ResetBatteryLife,                (ExecuteFunction)&Execute_C188_ResetBatteryLife,                (ComposeFunction)&Compose_C188_ResetBatteryLife                 },
//    {CMDID_C190_MakeMeasurement,                  C190_ReqSize, (ParseFunction)&Parse_C190_MakeMeasurement,                 (ExecuteFunction)&Execute_C190_MakeMeasurement,                 (ComposeFunction)&Compose_C190_MakeMeasurement                  },
//    {CMDID_C191_WriteUTMParameters,               C191_ReqSize, (ParseFunction)&Parse_C191_WriteUTMParameters,              (ExecuteFunction)&Execute_C191_WriteUTMParameters,              (ComposeFunction)&Compose_C191_WriteUTMParameters               },
//    {CMDID_C192_ReadUTMParameters,                C192_ReqSize, (ParseFunction)&Parse_C192_ReadUTMParameters,               (ExecuteFunction)&Execute_C192_ReadUTMParameters,               (ComposeFunction)&Compose_C192_ReadUTMParameters                },
//    {CMDID_C193_WriteUTTParameters,               C193_ReqSize, (ParseFunction)&Parse_C193_WriteUTTParameters,              (ExecuteFunction)&Execute_C193_WriteUTTParameters,              (ComposeFunction)&Compose_C193_WriteUTTParameters               },
//    {CMDID_C194_ReadUTTParameters,                C194_ReqSize, (ParseFunction)&Parse_C194_ReadUTTParameters,               (ExecuteFunction)&Execute_C194_ReadUTTParameters,               (ComposeFunction)&Compose_C194_ReadUTTParameters                },
    {CMDID_C195_AccessPassword,                   C195_ReqSize, (ParseFunction)&Parse_C195_AccessPassword,                  (ExecuteFunction)&Execute_C195_AccessPassword,                  (ComposeFunction)&Compose_C195_AccessPassword                   },
    {CMDID_C196_WritePassword,                    C196_ReqSize, (ParseFunction)&Parse_C196_WritePassword,                   (ExecuteFunction)&Execute_C196_WritePassword,                   (ComposeFunction)&Compose_C196_WritePassword                    },
    {CMDID_C197_DisablePassword,                  C197_ReqSize, (ParseFunction)&Parse_C197_DisablePassword,                 (ExecuteFunction)&Execute_C197_DisablePassword,                 (ComposeFunction)&Compose_C197_DisablePassword                  },
    {CMDID_C198_WriteAlarmMode,                   C198_ReqSize, (ParseFunction)&Parse_C198_WriteAlarmMode,                  (ExecuteFunction)&Execute_C198_WriteAlarmMode,                  (ComposeFunction)&Compose_C198_WriteAlarmMode                   },
    {CMDID_C201_WriteMSSerialNumber,              C201_ReqSize, (ParseFunction)&Parse_C201_WriteMSSerialNumber,             (ExecuteFunction)&Execute_C201_WriteMSSerialNumber,             (ComposeFunction)&Compose_C201_WriteMSSerialNumber              },
    {CMDID_C202_ReadMSSerialNumber,               C202_ReqSize, (ParseFunction)&Parse_C202_ReadMSSerialNumber,              (ExecuteFunction)&Execute_C202_ReadMSSerialNumber,              (ComposeFunction)&Compose_C202_ReadMSSerialNumber               },
    {CMDID_C203_WriteCommissionDate,              C203_ReqSize, (ParseFunction)&Parse_C203_WriteCommissionDate,             (ExecuteFunction)&Execute_C203_WriteCommissionDate,             (ComposeFunction)&Compose_C203_WriteCommissionDate              },
    {CMDID_C204_ReadCommissionDate,               C203_ReqSize, (ParseFunction)&Parse_C204_ReadCommissionDate,              (ExecuteFunction)&Execute_C204_ReadCommissionDate,              (ComposeFunction)&Compose_C204_ReadCommissionDate               },
    {CMDID_C205_WriteMeasurementTimeInterval,     C205_ReqSize, (ParseFunction)&Parse_C205_WriteMeasurementTimeInterval,    (ExecuteFunction)&Execute_C205_WriteMeasurementTimeInterval,    (ComposeFunction)&Compose_C205_WriteMeasurementTimeInterval     },
    {CMDID_C206_ReadMeasurementTimeInterval,      C206_ReqSize, (ParseFunction)&Parse_C206_ReadMeasurementTimeInterval,     (ExecuteFunction)&Execute_C206_ReadMeasurementTimeInterval,     (ComposeFunction)&Compose_C206_ReadMeasurementTimeInterval      },
    {CMDID_C210_SetFactoryDefaults,               C210_ReqSize, (ParseFunction)&Parse_C210_SetFactoryDefaults,              (ExecuteFunction)&Execute_C210_SetFactoryDefaults,              (ComposeFunction)&Compose_C210_SetFactoryDefaults               },
    {CMDID_C778_ReadBatteryLife,                  C778_ReqSize, (ParseFunction)&Parse_C778_ReadBatteryLife,                 (ExecuteFunction)&Execute_C778_ReadBatteryLife,                 (ComposeFunction)&Compose_C778_ReadBatteryLife                  }
};

uint8 App_ComposeBurstRsp(uint16 p_ucCommandNo, void *p_aucRequest, void *p_aucResponse, uint8 *p_ucApiDataSize);
void App_PrepareAndSendBurst(uint8 p_ucBurstNo);
void App_PrepareAndSendEvents(uint8 p_ucBurstNo, uint8 p_ucType);
uint8 App_ComposeEventRsp(void *p_aucRequest, void *p_aucResponse, uint8 *p_ucApiDataSize);


/**
* @brief Initializes data structures. This function is used in the case of a SW reset.
*/
void App_UserInit()
{
    memset((uint8 *)g_aucRequest, 0x00, sizeof(g_aucRequest));                          // init the g_aucRequest buffer
    memset((uint8 *)g_aucResponse, 0x00, sizeof(g_aucResponse));                        // init the g_aucResponse buffer
    g_flagInitialRFBWService = FALSE;                                                   // Init the Burst Service Task flag, the task will execute
    g_bNeedSendRsp = TRUE;                                                              // Init the flag, a response to a command that causes a srvc req will be sent
    memset(&g_stCmd48Data, 0, sizeof(g_stCmd48Data));
    memset( &g_stEventsScanLatched, 0, sizeof(g_stEventsScanLatched));
    memset(g_stEventQueue, 0, sizeof(g_stEventQueue));
    g_pEventQueueEnd = g_stEventQueue;
    App_BurstInit();
}



/**
 * @brief Initializes Burst Period and Burst Enable Properties.
 */
void App_BurstInit()
{
    uint8 ucBurstNo;
    for (ucBurstNo = 0; ucBurstNo < MAX_BW_SERVICES_NO; ucBurstNo++)
    {
        g_stRFBWService[ucBurstNo].m_ucSrvcModeControlCode = Off;                       // Burst isn't activated yet
        g_stRFBWService[ucBurstNo].m_ucSrvcReqStatus = SRVC_REQ_NONE;                   // Service request isn't sent yet
        g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod = 0;                         // Not g_stBurstNVM[ucBurstNo].m_ulUpdateTime - so if trigger valid the device will send the burst soon
        g_stRFBWService[ucBurstNo].m_flagSrvcInServiceSyncro = 0;
        g_stRFBWService[ucBurstNo].m_ucSrvcDenied = 0;
        if (ucBurstNo<MAX_BURST_MSG_NO)
        { // is burst
            g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod = g_stProv.m_stBurstNVM[ucBurstNo].m_ulMaxUpdateTime;
        }
        else if ( (ucBurstNo>=MAX_BURST_MSG_NO) && (ucBurstNo<MAX_BW_SERVICES_NO) )
        { // is event
            g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod = g_stProv.m_stEventNVM.m_ulMaxUpdateTime;
        }
    }
}



/**
* @brief Processes the burst config (loaded from NVM on device initialization) and sends the RF Bandwidth Service requests for the active bursts or events.
*/
void APP_InitialRFBWServiceTask()
{ //
    // If task already executed with success, exit
    if(g_flagInitialRFBWService == TRUE)
        return;

    // If device not joined exit
    if(g_ucAppProcessorStatus < API_STATE_NOTIFY_JOIN_OK)
        return;

uint8 ucBurstNo;
uint8 ucGetSrvcReqStatus;

    ucGetSrvcReqStatus = BURST_SRVC_REQ_OK;
    // At this point, the burst config has either been loaded from NVM or initialised with defaults in case of a NVM error
    // Parse burst table, look for enabled bursts
    for (ucBurstNo = 0; ucBurstNo < MAX_BW_SERVICES_NO; ucBurstNo++)
    {
        if (g_stRFBWService[ucBurstNo].m_ucSrvcModeControlCode == Off)
        {
            if (ucBurstNo<MAX_BURST_MSG_NO)
            { // burst
                if (g_stProv.m_stBurstNVM[ucBurstNo].m_ucBurstModeControlCode == EnableOnTDMADataLinkLayer)
                {
                    if (g_stRFBWService[ucBurstNo].m_flagSrvcInServiceSyncro)
                    { // An App_GetSrvcRsp was already reaceived but not syncronized.
                        continue;
                    }
                    // Burst was enabled in NVM stored config and in RAM config appears as - a reset occured
                    // OR a service request was sent and the response did not arrive before the device reset
                    // Send a service request
                    uint32 ulBurstPeriod;
                    // g_stBurstNVM[i].m_ulUpdateTime was divided by 32 in CMD103! so multiply the value by 32 when requiring a service
                    ulBurstPeriod = (uint32)(g_stProv.m_stBurstNVM[ucBurstNo].m_ulUpdateTime * 32);
                    if(App_GetSrvcReq(CMDID_C109_BurstModeCtrl, ucBurstNo, ulBurstPeriod, 0) == API_SUCCESS)
                    {
                        g_bNeedSendRsp = FALSE; // Reset flag - prevent sending a command response to the VN radio when the srvc response arrives
                                                // This srvc request is sent automatically, the AP must not send a cmd 109 response via API interface
                                                // to the radio
                        g_stRFBWService[ucBurstNo].m_ucSrvcReqStatus = SRVC_REQ_C799;
                        return;
                    }
                    else
                    {
                        g_stRFBWService[ucBurstNo].m_ucSrvcReqStatus = SRVC_REQ_NONE; // Set SrvReq false
                        // Note that an error occured
                        ucGetSrvcReqStatus = BURST_SRVC_REQ_ERR;
                    }
                }
            }
            else// if ( (ucBurstNo>=MAX_BURST_MSG_NO) && (ucBurstNo<MAX_BW_SERVICES_NO) )
            { // event
                if (g_stProv.m_stEventNVM.m_ucEventControlCode == EnableOnTDMADataLinkLayer)
                {
                    if (g_stRFBWService[ucBurstNo].m_flagSrvcInServiceSyncro)
                    { // An App_GetSrvcRsp was already reaceived but not syncronized.
                        continue;
                    }
                    // Burst was enabled in NVM stored config and in RAM config appears as - a reset occured
                    // OR a service request was sent and the response did not arrive before the device reset
                    // Send a service request
                    uint32 ulBurstPeriod;
                    // g_stBurstNVM[i].m_ulUpdateTime was divided by 32 in CMD103! so multiply the value by 32 when requiring a service
                    ulBurstPeriod = (uint32)(g_stProv.m_stEventNVM.m_ulRetryTime * 32);
                    if(App_GetSrvcReq(CMDID_C118_EventNotificationControl, ucBurstNo, ulBurstPeriod, 0) == API_SUCCESS)
                    {
                        g_bNeedSendRsp = FALSE; // Reset flag - prevent sending a command response to the VN radio when the srvc response arrives
                                                // This srvc request is sent automatically, the AP must not send a cmd 109 response via API interface
                                                // to the radio
                        g_stRFBWService[ucBurstNo].m_ucSrvcReqStatus = SRVC_REQ_C799;
                        return;
                    }
                    else
                    {
                        g_stRFBWService[ucBurstNo].m_ucSrvcReqStatus = SRVC_REQ_NONE; // Set SrvReq false
                        // Note that an error occured
                        ucGetSrvcReqStatus = BURST_SRVC_REQ_ERR;
                    }
                }
            }
        }
    }

    if (ucGetSrvcReqStatus == BURST_SRVC_REQ_OK)
    {
        g_flagInitialRFBWService = TRUE;
    }
    // Else, the task will execute again and request the service for the burst(s) that failed
}

/**
* @brief Calculates smallest common divisor of two numbers.
* @param[in] a - first number
* @param[in] b - second number
*/
uint32 cmmdc(uint32 a, uint32 b)
{
    while( a != b )
    {
        if(a>b)
            a=a-b;
        else
            b=b-a;
    }
    return a;
}

/**
* @brief Returns the updated period for the active burst or event message.
* param[in] p_ucChannel - Burst or event index
*/
uint32 getChannelPeriod(uint8 p_ucChannel)
{
    switch (p_ucChannel)
    {
    case 0:
        return  g_stProv.m_stBurstNVM[0].m_ulUpdateTime;
    case 1:
        return  g_stProv.m_stBurstNVM[1].m_ulUpdateTime;
    case 2:
        return  g_stProv.m_stBurstNVM[2].m_ulUpdateTime;
    case 3:
        return  g_stProv.m_stEventNVM.m_ulRetryTime;
    }
    return 0xFFFFFFFF;
}

/**
* @brief Returns the longest message index (in terms of data bytes length) of the RF Bandwidth array.
* @param[in] p_ucPreparedMask - bit mask of active events and bursts.
*/
uint8 getPreparedServiceMessage(uint8 p_ucPreparedMask)
{
volatile uint8 ucReturnedIndex = 0xFF;
volatile uint8 ucMaxLen = 0;
    
uint8 ucIndex;
    for (ucIndex=0; ucIndex < MAX_BW_SERVICES_NO; ucIndex++)
    {
        if (p_ucPreparedMask & (1 << ucIndex))
        {
            if ( g_stRFBWService[ucIndex].m_ucResponseDataLen > ucMaxLen)
            {
                ucMaxLen = g_stRFBWService[ucIndex].m_ucResponseDataLen;
                ucReturnedIndex = ucIndex;
            }
        }
    }

    return ucReturnedIndex;
}


void App_BurstOneSecTask()
{
    if (g_ucAppProcessorStatus < API_STATE_NOTIFY_JOIN_OK)
        return;
  
    uint8 i;
    for (i = 0; i < MAX_BW_SERVICES_NO; i++)
    { // need to syncro some one ?
        if (g_stRFBWService[i].m_flagSrvcInServiceSyncro)
        { // Yes : I need to syncro the channel
            uint8 ucSyncroChannel = i;
            
            uint32 ulCMMDC = 0;
            uint32 ulChannel = 0xFF;
            
            for (i = 0; i < MAX_BW_SERVICES_NO; i++)
            {
                if (i == ucSyncroChannel)
                    continue;

                if (g_stRFBWService[i].m_ucSrvcModeControlCode == EnableOnTDMADataLinkLayer)
                { // a possible candidate to sync with ...
                    uint32 ulDivComun = cmmdc(getChannelPeriod(ucSyncroChannel), getChannelPeriod(i));
                    if (ulDivComun > ulCMMDC)
                    {
                        ulCMMDC = ulDivComun;
                        ulChannel = i;
                    }
                }
            }

            if (ulChannel < MAX_BW_SERVICES_NO)
            { // syncronize ucSyncroChannel to ulChannel
                if ( getChannelPeriod(ucSyncroChannel) >= getChannelPeriod(ulChannel) )
                { // I am bigger or equal => syncro with his first slot
                    g_stRFBWService[ucSyncroChannel].m_ulSrvcInServicePeriod = g_stRFBWService[ulChannel].m_ulSrvcInServicePeriod;
                }
                else
                { // I am smaller
                    uint32 ulTarget = getChannelPeriod(ulChannel);
                    do
                    { //  succesive substractions
                        ulTarget -= getChannelPeriod(ucSyncroChannel);
                    }
                    while  ( ulTarget > ulCMMDC );
                    g_stRFBWService[ucSyncroChannel].m_ulSrvcInServicePeriod = ulTarget;
                }
            }
            else
            {
                // maybe I am the first ... or cannot sync with ... no need to syncro with somebody ... just start at aligned secomd
                g_stRFBWService[ucSyncroChannel].m_ulSrvcInServicePeriod = getChannelPeriod(ucSyncroChannel);
            }
            g_stRFBWService[ucSyncroChannel].m_ucSrvcModeControlCode = EnableOnTDMADataLinkLayer;
            g_stRFBWService[ucSyncroChannel].m_flagSrvcInServiceSyncro = 0;
            
            break; // exit from this for ... I have the channel ... the first channel
        }
    }

}

/**
* @brief Verifies if any of the RF Bandwidth channels needs synchronization. 
*
* Parses burst and event configuration. Based on the trigger source configuration, specific behavior is adopted.
* Burst message payload is sent to the radio module.
*/
void App_BurstTask()
{
static uint8 ucEventMaxUpdateDoOnce = 2;

    if (g_ucAppProcessorStatus < API_STATE_NOTIFY_JOIN_OK)
        return;

    // Parse burst table
    uint8 ucBurstNo;
    for (ucBurstNo = 0; ucBurstNo < MAX_BURST_MSG_NO; ucBurstNo++)
    {
        // If BURST ENABLED on EnableOnTDMADataLinkLayer
        if (g_stRFBWService[ucBurstNo].m_ucSrvcModeControlCode == EnableOnTDMADataLinkLayer)
        {
            // Decrement burst period
            if (g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod >= BURST_TASK_PERIOD_MS)
            {
                g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod -= BURST_TASK_PERIOD_MS;
            }
            else
            {
                g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod = 0;
            }

            // Decrement max burst period
            if (g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod >= BURST_TASK_PERIOD_MS)
            {
                g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod -= BURST_TASK_PERIOD_MS;
            }
            else
            {
                g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod = 0;
            }

            // If trigger mode is "Continuous" and burst period expired
            if (g_stProv.m_stBurstNVM[ucBurstNo].m_ucBurstTriggerModeCode == BurstMessageTriggerMode_Continuous)
            {
                if (g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod == 0)
                {
                    // Active burst, prepare an send burst
                    App_PrepareAndSendBurst(ucBurstNo);
                    // Reinit burst period
                    g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod = g_stProv.m_stBurstNVM[ucBurstNo].m_ulUpdateTime;
                }
            }
            else //if (g_stProv.m_stBurstNVM[ucBurstNo].m_ucBurstTriggerModeCode != BurstMessageTriggerMode_Continuous)
            {
                DEVICE_VARIABLE_T* pstTriggerValue;
                
                // Retrieve trigger sorce value - first value in current burst device variable codes table (in our case Publish Mode)
                if ((pstTriggerValue = App_TriggerSorceVal(g_stProv.m_stBurstNVM[ucBurstNo].m_aucDeviceVariableCodes[0])) != NULL)
                {
                    fp32 fTempFloat;
                    
                    switch (g_stProv.m_stBurstNVM[ucBurstNo].m_ucBurstTriggerModeCode)
                    {
                    case BurstMessageTriggerMode_Rising:
                        if (pstTriggerValue->m_stValue.m_fValue > g_stProv.m_stBurstNVM[ucBurstNo].m_fTriggerValue)
                        {
                            if (g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod  == 0)
                            {
                                // Active burst, prepare an send burst
                                App_PrepareAndSendBurst(ucBurstNo);
                                // Reinit burst period
                                g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod  = g_stProv.m_stBurstNVM[ucBurstNo].m_ulUpdateTime;
                                // Reinit max. burst period
                                g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod = g_stProv.m_stBurstNVM[ucBurstNo].m_ulMaxUpdateTime;
                            }
                        }
                        else
                        {
                            // Set the period to 0 so the next time the trigger is valid, the burst will be sent without delay
                            g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod = 0;
                            // Check if max burst period expired
                            if (g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod == 0)
                            {
                                // Active burst, prepare an send burst
                                App_PrepareAndSendBurst(ucBurstNo);
                                // Reinit burst period
                                g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod = g_stProv.m_stBurstNVM[ucBurstNo].m_ulMaxUpdateTime;
                            }
                        }
                        break;

                    case BurstMessageTriggerMode_Falling:
                        if (pstTriggerValue->m_stValue.m_fValue < g_stProv.m_stBurstNVM[ucBurstNo].m_fTriggerValue)
                        {
                            if (g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod  == 0)
                            {
                                // Active burst, prepare an send burst
                                App_PrepareAndSendBurst(ucBurstNo);
                                // Reinit burst period
                                g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod  = g_stProv.m_stBurstNVM[ucBurstNo].m_ulUpdateTime;
                                // Reinit max. burst period
                                g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod = g_stProv.m_stBurstNVM[ucBurstNo].m_ulMaxUpdateTime;
                            }
                        }
                        else
                        {
                            // Set the period to 0 so the next time the trigger is valid, the burst will be sent withouth delay
                            g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod = 0;
                            // Check if max. burst period expired
                            if (g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod == 0)
                            {
                                // Active burst, prepare an send burst
                                App_PrepareAndSendBurst(ucBurstNo);
                                // Reinit burst period
                                g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod = g_stProv.m_stBurstNVM[ucBurstNo].m_ulMaxUpdateTime;
                            }
                        }
                        break;

                    case BurstMessageTriggerMode_OnChange:
                        if (pstTriggerValue->m_stLastValue.m_fValue != pstTriggerValue->m_stValue.m_fValue)
                        { // value != last value or previous value != last value
                            // Active burst, prepare an send burst
                            App_PrepareAndSendBurst(ucBurstNo);
                            // Reinit burst period
                            g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod = g_stProv.m_stBurstNVM[ucBurstNo].m_ulUpdateTime;
                            // Reinit max. burst period
                            g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod = g_stProv.m_stBurstNVM[ucBurstNo].m_ulMaxUpdateTime;
                        }
                        else
                        {
                            if (g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod == 0)
                            {
                                // Active burst, prepare an send burst
                                App_PrepareAndSendBurst(ucBurstNo);
                                // Reinit burst period
                                g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod = g_stProv.m_stBurstNVM[ucBurstNo].m_ulMaxUpdateTime;
                            }
                        }
                        break;

                    case BurstMessageTriggerMode_Window:
                        if (pstTriggerValue->m_stValue.m_fValue > pstTriggerValue->m_stLastValue.m_fValue)
                        {
                            fTempFloat = pstTriggerValue->m_stValue.m_fValue - pstTriggerValue->m_stLastValue.m_fValue;
                        }
                        else
                        {
                            fTempFloat = pstTriggerValue->m_stLastValue.m_fValue - pstTriggerValue->m_stValue.m_fValue;
                        }
                        if ((fTempFloat > g_stProv.m_stBurstNVM[ucBurstNo].m_fTriggerValue))
                        { // I am out of the window
                            // Active burst, prepare an send burst
                            App_PrepareAndSendBurst(ucBurstNo);
                            // Reinit burst period
                            g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod = g_stProv.m_stBurstNVM[ucBurstNo].m_ulUpdateTime;
                            // Reinit max. burst period
                            g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod = g_stProv.m_stBurstNVM[ucBurstNo].m_ulMaxUpdateTime;
                        }
                        else
                        { // I am in window
                            if (g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod == 0)
                            { // but max update time expired
                                // Active burst, prepare an send burst
                                App_PrepareAndSendBurst(ucBurstNo);
                                // Reinit burst period
                                g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod = g_stProv.m_stBurstNVM[ucBurstNo].m_ulMaxUpdateTime;
                            }
                        }
                        break;
                    } // end switch
                    pstTriggerValue->m_stLastValue.m_fValue = pstTriggerValue->m_stValue.m_fValue;
                }
            }
        } // end if BURST ENABLED on EnableOnTDMADataLinkLayer
    } // end for

    // Events
    for (ucBurstNo = MAX_BURST_MSG_NO; ucBurstNo < MAX_BW_SERVICES_NO; ucBurstNo++)
    {
        // If Event ENABLED on EnableOnTDMADataLinkLayer
        if (g_stRFBWService[ucBurstNo].m_ucSrvcModeControlCode == EnableOnTDMADataLinkLayer)
        {
            if (EVENT_QUEUE_NOT_EMPTY)
            { // Events queue not empty ...

                // init max update time ... to send only once after there will be no events ... queue empty
                g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod = g_stProv.m_stEventNVM.m_ulMaxUpdateTime;
                ucEventMaxUpdateDoOnce = 2;

                // Decrement retry period
                if (g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod >= BURST_TASK_PERIOD_MS)
                {
                    g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod -= BURST_TASK_PERIOD_MS;
                }
                else
                {
                    g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod = 0;
                }

                if ( g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod == 0 )
                { // time to process it
                    if ( (g_stEventQueue[0].m_ucAckReceived == 0) && (g_stEventQueue[0].m_ucCCRequested == 0xAA))
                    { // I have unacknowledged event ready to be published
                        App_PrepareAndSendEvents(ucBurstNo, 0);
                    }

                    // reinit retry period until receive ack and delete from queue
                    g_stRFBWService[ucBurstNo].m_ulSrvcInServicePeriod = g_stProv.m_stEventNVM.m_ulRetryTime;
                }
            }
            else
            { // Events queue empty
                if (ucEventMaxUpdateDoOnce)
                {
                    if (ucEventMaxUpdateDoOnce >= 2)
                    { // not requested
                        API_SendRequest(CLS_STACK_SPECIFIC, HART_GET_CFG_CHG_COUNTER, 0, 0, NULL, 0);
                        ucEventMaxUpdateDoOnce = 1;
                    }

                    // Decrement max update period
                    if (g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod >= BURST_TASK_PERIOD_MS)
                    {
                        g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod -= BURST_TASK_PERIOD_MS;
                    }
                    else
                    {
                        g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod = 0;
                    }

                    // send only once ... no repeat ... no reinit
                    // Max update period expired
                    if (g_stRFBWService[ucBurstNo].m_ulSrvcInServiceMaxPeriod == 0)
                    { //
                        ucEventMaxUpdateDoOnce = 0;
                        App_PrepareAndSendEvents(ucBurstNo, 1);
                    }
                }
            }
        }
    }

    // Send burst concatenated to API;
    {
        uint8 ucPreparedMask = 0x0F; // B3 = Event, B2 = Burst 2, B1 = Burst 1, B0 = Burst 0 .. to be checked
        uint8 ucIndex;
        uint8 ucDataLen = 0;
        uint8 ucData[MAX_APDU_SIZE];

        do
        {
            ucIndex = getPreparedServiceMessage(ucPreparedMask);
            if ( ucIndex < MAX_BW_SERVICES_NO )
            {
                // valid message data
                if ( ( ucDataLen + g_stRFBWService[ucIndex].m_ucResponseDataLen ) <= MAX_APDU_SIZE )
                { // buffer is still capable of receiving data
                    if (ucDataLen==0)
                    { // it's the first message. Add it + Device Status
                        memcpy((void*) ucData, g_stRFBWService[ucIndex].m_aucRequest, g_stRFBWService[ucIndex].m_ucResponseDataLen);
                        ucDataLen = g_stRFBWService[ucIndex].m_ucResponseDataLen;
                    }
                    else
                    { // it's at least the second one and can be placed in buffer. Not adding Device Status
                        memcpy( (void*) &ucData[ucDataLen], g_stRFBWService[ucIndex].m_aucRequest+1, g_stRFBWService[ucIndex].m_ucResponseDataLen-1);
                        ucDataLen += g_stRFBWService[ucIndex].m_ucResponseDataLen-1;
                    }
                    ucPreparedMask &= ~(1<<ucIndex);
                }
                else
                { // No more space in the buffer ... Feed the API queue with what is so far
                    if (ucDataLen)
                    {
                        API_SendMessage(CLS_USER_BOARD,
                                        DAQ_BURST_PIPE_0,           // use only this channel
                                        API_MSGTYPE_REQUEST,		// Is not response
                                        0,
                                        ucDataLen,
                                        (uint8*) ucData,
                                        0);
                        g_ucAppProcessorStatus = API_STATE_PUBLISH_OK;
                        ucDataLen = 0;
                    }
                }
            }
        }
        while ( ucIndex < MAX_BURST_MSG_NO );

        if (ucDataLen)
        {
            API_SendMessage(CLS_USER_BOARD,
                            DAQ_BURST_PIPE_0,           // use only this channel
                            API_MSGTYPE_REQUEST,		// Is not response
                            0,
                            ucDataLen,
                            (uint8*) ucData,
                            0);
            g_ucAppProcessorStatus = API_STATE_PUBLISH_OK;
            ucDataLen = 0;
        }

        // delete all ... should be in API queue already
        for (ucIndex=0; ucIndex < MAX_BW_SERVICES_NO; ucIndex++)
        {
            g_stRFBWService[ucIndex].m_ucResponseDataLen = 0;
        }
    }

}



/**
* @brief Scans for any change in the event configration.
*/
void App_EventTask()
{
    static uint32 sulTimeToScanEvent = 0;

    if ( g_stProv.m_stEventNVM.m_ucEventControlCode == EventNotificationControlCodes_EnableEventNotificationOnTDMADataLinkLayer )
    {
        if( EVENT_QUEUE_NOT_EMPTY )
        {
            EVENT_PENDING_T* pEvent = g_stEventQueue;
            if( pEvent->m_ucAckReceived )
            { // acknowlegde received
                // compact the event queue(memmove has no effect if there is only one event on queue)
                memmove(pEvent, pEvent+1, (g_pEventQueueEnd - g_stEventQueue - 1) * sizeof(EVENT_PENDING_T));
                g_pEventQueueEnd--;
            }
        }
        
        // Decrement scan period
        if (sulTimeToScanEvent > EVENT_TASK_PERIOD_MS)
        {
            sulTimeToScanEvent -= EVENT_TASK_PERIOD_MS;
        }
        else
        {
           // time to scan anc check events ... new scan needed
            if ( memcmp(&g_stEventsScanLatched, &g_stCmd48Data, sizeof(g_stEventsScanLatched) ) )
            { // Current scan is different than Latched scan => something changed ... cleared or set
                Cmd48Data stEventsScanCurrent;

                memcpy( &stEventsScanCurrent, &g_stCmd48Data, sizeof(stEventsScanCurrent)); // get current scan 
                
                uint8 i;
                for (i=0; i<25; i++)
                {
                    if ( (~g_stEventsScanLatched.byte[i]) & stEventsScanCurrent.byte[i] )
                    { // A bit was set ... an event was rised
                        if( ! EVENT_QUEUE_IS_FULL )
                        { // I have space for this event ... other ways discard it
                            g_pEventQueueEnd->m_ucEventNo = 0; // we support only 1 Event channel => channel id = 0
                            g_pEventQueueEnd->m_ucCCRequested = 0x55;
                            g_pEventQueueEnd->m_ucCCCounter = 0;
                            memcpy(g_pEventQueueEnd->m_aucEvents, &stEventsScanCurrent, 25);

                            g_pEventQueueEnd->m_ulTimestamp = (g_ulMonotonic%((uint32)24*60*60*100))*10*32;
                            // H . min . sec . slots .. miliSec .. 1/32 miliSec

                            // reset the ACK flag
                            g_pEventQueueEnd->m_ucAckReceived = 0;

                            // save event status
                            g_pEventQueueEnd->m_ucEventStatus = EventStatusCodesMask_DeviceStatusEventPending;

                            // set
                            // g_pEventQueueEnd->m_ucEventStatus
                                    //... |= EventStatusCodesMask_ConfigurationChangedEventPending; // Config Change triggered
                                    //... |= EventStatusCodesMask_MoreStatusAvailableEventPending; // MSA triggered
                                    //... |= EventStatusCodesMask_DeviceStatusEventPending; // cmd 48 triggered


                            // update queue size
                            g_pEventQueueEnd ++;

                            // Send API Change Counter request
                            API_SendRequest(CLS_STACK_SPECIFIC, HART_GET_CFG_CHG_COUNTER, 0, 0, NULL, 0);
                        }
                        break;
                    }
                }

                // save the current scan into latched
                memcpy( &g_stEventsScanLatched, &stEventsScanCurrent, sizeof(stEventsScanCurrent));


                // All events in cmd48 are already into queue or latched.
                // clear cmd 48 contents in order to catch an second event raise
                // or only device specific bytes ???
                // maybe some events are
                // - time cleared
                // - hardware condition cleared
                // - acknowledge / commands cleared

                memset( &g_stCmd48Data, 0, sizeof(g_stCmd48Data)); // in this moment we will work like this: clear after latched ... not need to be persistent.
            }
            else
            { // Current scan is the same with latched one ... nothing to do
                ;
            }

            sulTimeToScanEvent = g_stProv.m_stEventNVM.m_ulDebounceInterval; // reinit Scan to Latch period
        }
    }
    else
    { // EventNotificationControlCodes_Off
        if ( EVENT_QUEUE_NOT_EMPTY )
        { // clear event queue
            memset(g_stEventQueue, 0, sizeof(g_stEventQueue));
            g_pEventQueueEnd = g_stEventQueue;
        }
    }
}


/**
 *  @brief Processes the service request(s)
 */

void APP_DynamicRFBWServiceTask()
{
    if (g_flagInitialRFBWService == TRUE)
    {
        if(g_flagDynamicRFBWService == TRUE)
        {
            uint8 ucBurstNo;
            for (ucBurstNo = 0; ucBurstNo < MAX_BW_SERVICES_NO; ucBurstNo++)
            { // Parse burst table, look for pending service request
                if (g_stRFBWService[ucBurstNo].m_ucSrvcReqStatus == SRVC_REQ_C799)
                {
                    uint32 ulBurstPeriod = g_stRFBWService[ucBurstNo].m_ulSrvcRequestPeriod;
                    uint16 unCommanNumber = g_stRFBWService[ucBurstNo].m_unSrvcCommandNumber;

                    // Send a service request to WISA over API
                    if(App_GetSrvcReq(unCommanNumber, ucBurstNo, ulBurstPeriod, 0) == API_SUCCESS)
                    {
                        g_bNeedSendRsp = FALSE; // Reset flag - prevent sending a command response to the VN radio when the srvc response arrives
                                                // This srvc request is sent automatically, the AP must not send a cmd 109 response via API interface
                                                // to the radio
                        return;
                    }
                }
            }
        }
    }
}



/**
 *  @brief Sends a service request to the radio module. This function returns an API_ERROR_CODE_TYPES code.
 *  @param[in] p_unCmdId
 *  @param[in] p_ucBurstMessage
 *  @param[in] p_ulBurstPeriod
 *  @param[in] p_ucSource
 *
 */

uint8 App_GetSrvcReq(uint16 p_unCmdId, uint8 p_ucBurstMessage, uint32 p_ulBurstPeriod, uint8 p_ucSource)
{
    if (p_ucSource == 0)
    { // from WISA
      if ( (HART_CD_PIN_STATE()) || (g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse) || ((HART_RTS_PIN_STATE()) == 0) )        
        { // Hart in Progress do not permit SrvReq from WISA while HART in progress
            return API_ERR_API_FAILED;
        }
    }
    //else => from AP Hart

    #define HART_SERVICE_REQUEST_SIZE 7
    uint8 aucOutBuf[7]; // array to store the command Id(103,109...), burst message and burst period
    aucOutBuf[0] = p_unCmdId >> 8;
    aucOutBuf[1] = p_unCmdId;
    aucOutBuf[2] = p_ucBurstMessage;
    aucOutBuf[3] = p_ulBurstPeriod >> 24;
    aucOutBuf[4] = p_ulBurstPeriod >> 16;
    aucOutBuf[5] = p_ulBurstPeriod >> 8;
    aucOutBuf[6] = p_ulBurstPeriod;

    if(API_SUCCESS == API_SendRequest(CLS_STACK_SPECIFIC, HART_SERVICE_REQUEST, 0, HART_SERVICE_REQUEST_SIZE, aucOutBuf, p_ucSource))
    {
        return API_SUCCESS; // NO_ERR=0
    }
    else
    {
        return API_ERR_API_FAILED; // respond with not ackowloedge-NACK, ER_API_FAILED=7
    }
}



/**
 *  @brief Parses the service request response from the radio module. This function returns an API_ERROR_CODE_TYPES code.
 *  @param[in] *p_ucRcvDataBuff - pointer to the received data payload.
 *  @param[in] p_ucMsgSize - received payload size.
 */

uint8 App_GetSrvcRsp( uint8* p_ucRcvDataBuff, uint8 p_ucMsgSize)
{
uint16  unCommandId;
uint8   ucReturnCode;
uint8   ucBurst;
uint8   aucApiCommandBuff[32];
uint8   *aucApiCommand = (uint8 *)aucApiCommandBuff;
uint8   ucRspSize = 0;
uint32  ulUpdatePeriod = 0;
uint32  ulMaxUpdatePeriod;
uint32  ulPeriod;

    // Extract Command Number from the received WirelessHART message
    InverseMemcpy((uint8 *)&unCommandId, (uint8 *)p_ucRcvDataBuff, 2);
    p_ucRcvDataBuff += 2;

    if (
         (unCommandId != CMDID_C109_BurstModeCtrl) &&
         (unCommandId != CMDID_C103_WriteBurstPeriod) &&
         (unCommandId != CMDID_C117_WriteEventNotificationTiming) &&
         (unCommandId != CMDID_C118_EventNotificationControl)
        )
    { // Command error
        return RCS_N255_ResponseNack; // this error code is read by the API_OnRcvStackSpecific() function, which sends ACK/NACK packet over full api
    }

    // Extract return code from the received WirelessHART message
    ucReturnCode = *p_ucRcvDataBuff;
    p_ucRcvDataBuff++;

    // Extract burst channel number from the received WirelessHART message
    ucBurst = *p_ucRcvDataBuff;
    p_ucRcvDataBuff++;

    if (ucBurst >= MAX_BW_SERVICES_NO)
        return RCS_N255_ResponseNack; // this is error code is read by the API_OnRcvStackSpecific() function, which sends ACK/NACK packet over full api

    // Extract Update Period from the received WirelessHART message
    if ( p_ucMsgSize >= 8 )
    {
        InverseMemcpy((uint8 *)&ulUpdatePeriod, (uint8 *)p_ucRcvDataBuff, 4); // check ulUpdatePeriod == 0 ???
    }

    // ... here Received API WirelessHART message parsed with succes

    if ((ucReturnCode == RCS_E33_DelayedResponseInitiated) || (ucReturnCode == RCS_E34_DelayedResponseRunning))
    {
        if ( !g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse )
        { // Request was generated automatically or by Gateway
            if (g_bNeedSendRsp == TRUE)
            { // generated by the gateway ... compose API message classs CLS_USER_BOARD, type DAQ_FW_WIRELESS to be send by WISA as response back to gateway if needed
                *(aucApiCommand++) = g_ucDeviceStatus[MASTER_WISA];
                InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&unCommandId, 2);
                aucApiCommand +=2;
                ucRspSize = 5;
                *(aucApiCommand++) = 1;             // data size on api message
                *aucApiCommand = ucReturnCode;      // only one byte of data
            }
        }
        else
        { // Request was generated due to a wired HART command on the HART loop prepare a response to be send on the HART loop
            if (unCommandId == CMDID_C109_BurstModeCtrl)
            {
                // command
                *(aucApiCommand++) = 0xFF & unCommandId;
                // Byte Count
                *(aucApiCommand++) = 4;
                // Return Code = Delay Response Mech.
                *(aucApiCommand++) = ucReturnCode;
                // Device status
                *(aucApiCommand++) = ((HART_DELIMITER_ADDRESS_MASK & g_stProv.m_sWiredHart.m_RxMasterBurst) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]);
                // Cmd Data
                *(aucApiCommand++) = g_stRFBWService[ucBurst].m_ucSrvcModeControlCode;
                *(aucApiCommand++) = ucBurst;
                // rsp size
                ucRspSize = 6;
            }
            if (unCommandId == CMDID_C103_WriteBurstPeriod)
            {
                // command
                *(aucApiCommand++) = 0xFF & unCommandId;
                // Byte Count
                *(aucApiCommand++) = 11;
                // Return Code = Delay Response Mech.
                *(aucApiCommand++) = ucReturnCode;
                // Device status
                *(aucApiCommand++) = ((HART_DELIMITER_ADDRESS_MASK & g_stProv.m_sWiredHart.m_RxMasterBurst) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]);
                // Cmd Data
                *(aucApiCommand++) = ucBurst;
                ulPeriod = g_stProv.m_stBurstNVM[ucBurst].m_ulUpdateTime * 32;
                InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulPeriod , 4);
                aucApiCommand += 4;
                ulPeriod = g_stProv.m_stBurstNVM[ucBurst].m_ulMaxUpdateTime * 32;
                InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulPeriod, 4);
                aucApiCommand += 4;
                // rsp size
                ucRspSize = 13;
            }

            if ((unCommandId == CMDID_C117_WriteEventNotificationTiming))
            {
                // command
                *(aucApiCommand++) = 0xFF & unCommandId;
                // Byte Count
                *(aucApiCommand++) = 15;
                // Return Code = Delay Response Mech.
                *(aucApiCommand++) = ucReturnCode;
                // Device status
                *(aucApiCommand++) = ((HART_DELIMITER_ADDRESS_MASK & g_stProv.m_sWiredHart.m_RxMasterBurst) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]);
                // Cmd Data
                *(aucApiCommand++) = (ucBurst - MAX_BURST_MSG_NO); // channel event_no should be zero when only one event is supported
                ulPeriod = g_stProv.m_stEventNVM.m_ulRetryTime * 32;
                InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulPeriod, 4);
                aucApiCommand += 4;
                ulPeriod = g_stProv.m_stEventNVM.m_ulMaxUpdateTime * 32;
                InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulPeriod, 4);
                aucApiCommand += 4;
                ulPeriod = g_stProv.m_stEventNVM.m_ulDebounceInterval * 32;
                InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulPeriod, 4);
                aucApiCommand += 4;
                // rsp size
                ucRspSize = 17;
            }

            if ((unCommandId == CMDID_C118_EventNotificationControl))
            {
                // command
                *(aucApiCommand++) = 0xFF & unCommandId;
                // Byte Count
                *(aucApiCommand++) = 4;
                // Return Code = Delay Response Mech.
                *(aucApiCommand++) = ucReturnCode;
                // Device status
                *(aucApiCommand++) = ((HART_DELIMITER_ADDRESS_MASK & g_stProv.m_sWiredHart.m_RxMasterBurst) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]);
                // Cmd Data
                *(aucApiCommand++) = (ucBurst - MAX_BURST_MSG_NO); // channel event_no should be zero when only one event is supported
                *(aucApiCommand++) = g_stRFBWService[ucBurst].m_ucSrvcModeControlCode;
                // rsp size
                ucRspSize = 6;
            }
        }
        g_flagDynamicRFBWService = TRUE;
    }
    else if  ( (ucReturnCode == RCS_N00_Success) || (ucReturnCode == RCM_W14_CommBandwidthLessThanRequested))
    { // if I am here, then the service request was approved by the NetworkManager; therefore, the burst message must be activated
        g_stRFBWService[ucBurst].m_ucSrvcReqStatus = SRVC_REQ_NONE; // Service request finished
        g_flagDynamicRFBWService = FALSE;
        g_stRFBWService[ucBurst].m_ucSrvcModeControlCode = Off;
        g_stRFBWService[ucBurst].m_flagSrvcInServiceSyncro = 1;
        // Cmd 109
        if (unCommandId == CMDID_C109_BurstModeCtrl)
        {
            if (ulUpdatePeriod > 0)
            {
                g_stProv.m_stBurstNVM[ucBurst].m_ulUpdateTime = (uint32)(ulUpdatePeriod / ((uint32)32)); // the burst time format is msec
            }
            g_stProv.m_stBurstNVM[ucBurst].m_ucBurstModeControlCode = EnableOnTDMADataLinkLayer;

            if ( !g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse )
            { // Request was generated automatically or by Gateway
                if (g_bNeedSendRsp == TRUE)
                { // if a command response needs to be sent then command was issued by the the Gateway and Configuration Changed
                    SET_CONFIGURATION_CHANGED();

                    // Compose response for command 109 wireless
                    *(aucApiCommand++) = g_ucDeviceStatus[MASTER_WISA];
                    InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&unCommandId, 2);
                    aucApiCommand +=2;
                    ucRspSize = 7;                                  // wireless : 1 [DevStat] + 2 [cmdID] + 1 [size] + 1[RC] + 2[payload]
                    *aucApiCommand++  = 3;                          // 3 bytes payload
                    *aucApiCommand++  = ucReturnCode;               // Rsp code = 0
                    *aucApiCommand++  = EnableOnTDMADataLinkLayer;  // BurstControlMode
                    *aucApiCommand    = ucBurst;                    // Burst Channel
                }
            }
            else
            { // Request was generated due to a wired HART command on the HART loop prepare a response to be send on the HART loop
                // command
                *(aucApiCommand++) = 0xFF & unCommandId;
                // Byte Count
                *(aucApiCommand++) = 4;
                // Return Code = Delay Response Mech.
                *(aucApiCommand++) = ucReturnCode;
                // Device status
                *(aucApiCommand++) = ((HART_DELIMITER_ADDRESS_MASK & g_stProv.m_sWiredHart.m_RxMasterBurst) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]);
                // Cmd Data
                *(aucApiCommand++) = EnableOnTDMADataLinkLayer;
                *(aucApiCommand++) = ucBurst;
                // rsp size
                ucRspSize = 6;
            }
        }

        // Cmd 103
        if (unCommandId == CMDID_C103_WriteBurstPeriod)
        {
            if (ulUpdatePeriod > 0)
            {
                g_stProv.m_stBurstNVM[ucBurst].m_ulUpdateTime = (uint32)(ulUpdatePeriod / ((uint32)32)); // the burst time format is msec
            }
            ulMaxUpdatePeriod = g_stProv.m_stBurstNVM[ucBurst].m_ulMaxUpdateTime * 32;

            if ( !g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse )
            { // Request was generated automatically or by Gateway
                if (g_bNeedSendRsp == TRUE)
                { // if a command response needs to be sent then command was issued by the the Gateway and Configuration Changed
                    SET_CONFIGURATION_CHANGED();

                    // Compose response for command 103 wireless
                    *(aucApiCommand++) = g_ucDeviceStatus[MASTER_WISA];
                    InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&unCommandId, 2);
                    aucApiCommand +=2;
                    *(aucApiCommand++) = 10;           // Apicommand length = 9 bytes payload + 1 byte RC
                    *(aucApiCommand++) = ucReturnCode;
                    *(aucApiCommand++) = ucBurst;
                    InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulUpdatePeriod, 4);
                    aucApiCommand += 4;
                    InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulMaxUpdatePeriod, 4);
                    ucRspSize = 14;                     // wireless : 1 [DevStat] + 2 [cmdID] + 1 [size] + 1[RC] + 9[payload]
                }
            }
            else
            { // Request was generated due to a wired HART command on the HART loop prepare a response to be send on the HART loop
                // command
                *(aucApiCommand++) = 0xFF & unCommandId;
                // Byte Count
                *(aucApiCommand++) = 11;
                // Return Code = Delay Response Mech.
                *(aucApiCommand++) = ucReturnCode;
                // Device status
                *(aucApiCommand++) = ((HART_DELIMITER_ADDRESS_MASK & g_stProv.m_sWiredHart.m_RxMasterBurst) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]);
                // Cmd Data
                *(aucApiCommand++) = ucBurst;
                InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulUpdatePeriod, 4);
                aucApiCommand += 4;
                InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&g_stProv.m_stBurstNVM[ucBurst].m_ulMaxUpdateTime, 4);
                aucApiCommand += 4;
                // rsp size
                ucRspSize = 13;
            }
        }

        if ((unCommandId == CMDID_C118_EventNotificationControl))
        {
            g_stProv.m_stEventNVM.m_ucEventControlCode = EnableOnTDMADataLinkLayer;
            if (ulUpdatePeriod > 0)
            {
                g_stProv.m_stEventNVM.m_ulRetryTime = (uint32)(ulUpdatePeriod / ((uint32)32)); // the burst time format is msec
            }

            if ( !g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse )
            { // Request was generated automatically or by Gateway
                if (g_bNeedSendRsp == TRUE)
                { // if a command response needs to be sent then command was issued by the the Gateway and Configuration Changed
                    SET_CONFIGURATION_CHANGED();

                    // Compose response for command 118 wireless
                    *(aucApiCommand++) = g_ucDeviceStatus[MASTER_WISA];
                    InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&unCommandId, 2);
                    aucApiCommand +=2;
                    ucRspSize = 7;                                      // wireless : 1 [DevStat] + 2 [cmdID] + 1 [size] + 1[RC] + 2[payload]
                    *aucApiCommand++  = 3;                              // 3 bytes payload
                    *aucApiCommand++  = ucReturnCode;                   // Rsp code = 0
                    *aucApiCommand++  = EnableOnTDMADataLinkLayer;      // BurstControlMode
                    *aucApiCommand    = (ucBurst - MAX_BURST_MSG_NO);   // Event No
                }
            }
            else
            { // Request was generated due to a wired HART command on the HART loop prepare a response to be send on the HART loop
                // command
                *(aucApiCommand++) = 0xFF & unCommandId;
                // Byte Count
                *(aucApiCommand++) = 4;
                // Return Code = Delay Response Mech.
                *(aucApiCommand++) = ucReturnCode;
                // Device status
                *(aucApiCommand++) = ((HART_DELIMITER_ADDRESS_MASK & g_stProv.m_sWiredHart.m_RxMasterBurst) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]);
                // Cmd Data
                *(aucApiCommand++) = (ucBurst - MAX_BURST_MSG_NO);
                *(aucApiCommand++) = EnableOnTDMADataLinkLayer;
                // rsp size
                ucRspSize = 6;
            }
        }

        if ((unCommandId == CMDID_C117_WriteEventNotificationTiming))
        {
            if (ulUpdatePeriod > 0)
            {
                g_stProv.m_stEventNVM.m_ulRetryTime = (uint32)(ulUpdatePeriod / ((uint32)32)); // the burst time format is msec
            }

            if ( !g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse )
            { // Request was generated automatically or by Gateway
                if (g_bNeedSendRsp == TRUE)
                { // if a command response needs to be sent then command was issued by the the Gateway and Configuration Changed
                    SET_CONFIGURATION_CHANGED();

                    // Compose response for command 117 wireless
                    *(aucApiCommand++) = g_ucDeviceStatus[MASTER_WISA];
                    InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&unCommandId, 2);
                    aucApiCommand +=2;
                    *(aucApiCommand++) = 14;           // Apicommand length = 13 bytes payload + 1 byte RC
                    *(aucApiCommand++) = ucReturnCode;
                    *(aucApiCommand++) = (ucBurst - MAX_BURST_MSG_NO);
                    ulPeriod = g_stProv.m_stEventNVM.m_ulRetryTime * 32;
                    InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulPeriod, 4);
                    aucApiCommand += 4;
                    ulPeriod = g_stProv.m_stEventNVM.m_ulMaxUpdateTime * 32;
                    InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulPeriod, 4);
                    aucApiCommand += 4;
                    ulPeriod = g_stProv.m_stEventNVM.m_ulDebounceInterval * 32;
                    InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulPeriod, 4);
                    ucRspSize = 18;                     // wireless : 1 [DevStat] + 2 [cmdID] + 1 [size] + 1[RC] + 9[payload]
                }
            }
            else
            { // Request was generated due to a wired HART command on the HART loop prepare a response to be send on the HART loop
                // command
                *(aucApiCommand++) = 0xFF & unCommandId;
                // Byte Count
                *(aucApiCommand++) = 15;
                // Return Code = Delay Response Mech.
                *(aucApiCommand++) = ucReturnCode;
                // Device status
                *(aucApiCommand++) = ((HART_DELIMITER_ADDRESS_MASK & g_stProv.m_sWiredHart.m_RxMasterBurst) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]);
                // Cmd Data
                *(aucApiCommand++) = (ucBurst - MAX_BURST_MSG_NO);
                ulPeriod = g_stProv.m_stEventNVM.m_ulRetryTime * 32;
                InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulPeriod, 4);
                aucApiCommand += 4;
                ulPeriod = g_stProv.m_stEventNVM.m_ulMaxUpdateTime * 32;
                InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulPeriod, 4);
                aucApiCommand += 4;
                ulPeriod = g_stProv.m_stEventNVM.m_ulDebounceInterval * 32;
                InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&ulPeriod, 4);
                aucApiCommand += 4;
                // rsp size
                ucRspSize = 17;
            }
        }

        // new values, so save data into nvm
        if (ucBurst<MAX_BURST_MSG_NO)
        { // is burst
            FLASH_UPDATE_NEEDED();
        }
        if ( (ucBurst>=MAX_BURST_MSG_NO) && (ucBurst<MAX_BW_SERVICES_NO) )
        { // is event
            FLASH_UPDATE_NEEDED();
        }
    }
    else if (ucReturnCode == RCM_E65_ServiceRequestDenied)
    {
    	g_stRFBWService[ucBurst].m_ucSrvcReqStatus = SRVC_REQ_NONE; // Service request finished
        g_flagDynamicRFBWService = FALSE; // stop asking for BW
        g_stRFBWService[ucBurst].m_ucSrvcModeControlCode = Off;
        g_stRFBWService[ucBurst].m_flagSrvcInServiceSyncro = 0; // stop
        g_stRFBWService[ucBurst].m_ucSrvcDenied = 0x41;

        if ( !g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse )
        { // Request was generated automatically or by Gateway
            if (g_bNeedSendRsp == TRUE)
            { // generated by the gateway ... compose API message classs CLS_USER_BOARD, type DAQ_FW_WIRELESS to be send by WISA as response back to gateway if needed
                *(aucApiCommand++) = g_ucDeviceStatus[MASTER_WISA];
                InverseMemcpy((uint8 *)aucApiCommand, (uint8 *)&unCommandId, 2);
                aucApiCommand +=2;
                ucRspSize = 5;
                *(aucApiCommand++) = 1;             // data size on api message
                *aucApiCommand = RCS_E35_DelayedResponseDead;      // only one byte of data
            }
        }
        else
        {
            *(aucApiCommand++) = 0xFF & unCommandId;
            // Byte Count
            *(aucApiCommand++) = 2;
            // Return Code = Delay Response Mech.
            *(aucApiCommand++) = RCS_E35_DelayedResponseDead;
            // Device status
            *(aucApiCommand++) = ((HART_DELIMITER_ADDRESS_MASK & g_stProv.m_sWiredHart.m_RxMasterBurst) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]);
            ucRspSize = 4;
        }

        // new values, so save data into nvm
        if (ucBurst<MAX_BURST_MSG_NO)
        { // is burst
            g_stProv.m_stBurstNVM[ucBurst].m_ucBurstModeControlCode = Off; // disable
            
           FLASH_UPDATE_NEEDED();
        }
        if ( (ucBurst>=MAX_BURST_MSG_NO) && (ucBurst<MAX_BW_SERVICES_NO) )
        { // is event
            g_stProv.m_stEventNVM.m_ucEventControlCode = Off; // disable
            FLASH_UPDATE_NEEDED();
        }
    }
    else
    { // Command error
        return RCS_N255_ResponseNack;
    }

    if (!g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse)
    { // Request was generated automatically or by Gateway
        if (g_bNeedSendRsp == TRUE)
        { // if a response needs to be sent, the srvc req was not sent automatically - it was caused by a command received by the AP (109, 103)
            API_SendRequest(CLS_USER_BOARD, DAQ_FW_WIRELESS, 0, ucRspSize, (uint8 *)aucApiCommandBuff, 0);
        }
    }
    else
    { // Request was generated due to a wired HART command on the HART loop TX a response on the HART loop
        AppHART_Tx_Response(aucApiCommandBuff, ucRspSize);
        g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse = 0;
    }

    g_ucDeviceStatus[MASTER_WISA] &= (~FieldDeviceStatusesMask_ConfigurationChanged); // CC is reset for MASTER_WISA
    return RCS_N254_ResponseAck;
}



/**
 *  @brief Sends a delete service request to the radio module.
 *  @param[in] p_unCmdId
 *  @param[in] p_ucBurstMessage
 *
 */
void App_DelSrvcReq(uint16 p_unCmdId, uint8 p_ucBurstMessage)
{
uint8 auc_OutBuf[3];
    auc_OutBuf[0] = p_unCmdId >> 8;
    auc_OutBuf[1] = p_unCmdId;
    auc_OutBuf[2] = p_ucBurstMessage;
    API_SendRequest(CLS_STACK_SPECIFIC, HART_SERVICE_DELETE, 0, 3, auc_OutBuf, 0);
}




/**
 * @brief Parses the HART request, executes the HART command and composes the HART response.
 * @param[in] p_ucCmdType - Wireless or Wired message type for the class USER_BOARD: DAQ_FW_WIRELESS
 * @param[in] p_ucMsgId - Message ID (size = 1 byte) - represent the message ID from the API message - used for correspondence between request and response
 * @param[in] *p_pucCmdData - pointer to Full API payload (Status byte + APDU [Command ID(2 bytes) + Byte Count(1 byte length) + Command data bytes- which is the command payload )
*/
uint8 App_RcvTunnelledCmd(uint8 p_ucCmdType, uint8 p_ucMsgId, uint8 * p_pucCmdData)
{
uint16 unCmdId; // HART format cmd ID
uint8 ucReturnCode;
uint8 ucTableIndex;
uint8 ucApiDataSize;

    ExecuteFunction cmdExecute = NULL;

    // parse the stream within "p_pucCmdData" and copy the payload into "g_aucRequest" which is API message payload
    ucReturnCode = App_ParseCommand(p_ucCmdType, p_pucCmdData, g_aucRequest, &unCmdId, &ucTableIndex);

    if (unCmdId==119)
    {
        g_ucIsEventAck = 1;
    }

    if ((ucReturnCode == RCS_E05_TooFewDataBytesReceived) || (ucReturnCode == RCS_E64_CommandNotImplemented))
    {
        ; // do nothing
    }
    else
    {
        if (ucReturnCode == RCS_N00_Success)
        {
            cmdExecute = g_astCmdTable[ucTableIndex].cmdExec;
            // g_aucResponse is the Whart response command payload
            ucReturnCode = (*cmdExecute)((void*)g_aucRequest, (void*)g_aucResponse, p_ucCmdType); // execute the command
        }
        else if ( ( ucReturnCode == RCM_W30_CommandResponseTruncated ) && (ucTableIndex == 6) ) // command 9 with more bytes ... test UAL011a
        {
            cmdExecute = g_astCmdTable[ucTableIndex].cmdExec;
            // g_aucResponse is the Whart response command payload
            ucReturnCode = (*cmdExecute)((void*)g_aucRequest, (void*)g_aucResponse, p_ucCmdType); // execute the command
        }
    }

    if ((ucReturnCode == RCM_E65_ServiceRequestDenied) && (unCmdId==109) )
    { // Send response via API.
        uint8 aucBuffer[8];
        aucBuffer[0] = g_ucDeviceStatus[MASTER_WISA];
        aucBuffer[1] = 0x00;
        aucBuffer[2] = 109;
        aucBuffer[3] = 1;
        aucBuffer[4] = RCM_E09_InsufficientBandwidth;

        API_SendResponse(CLS_USER_BOARD , p_ucCmdType, p_ucMsgId, 5, aucBuffer, 0);
        return ucReturnCode;
    }

    if ((ucReturnCode != RCS_N254_ResponseAck) && (ucReturnCode != RCS_N255_ResponseNack))
    {
        // g_aucRequest contains the FULL API payload field [cmdID + Data Len + RespCode + Whart payload]
        // g_aucRequest contains the API message payload - wireless packet for a response
        // g_aucResponse is the Whart response command payload
        App_ComposeCommand(p_ucCmdType, (void*)g_aucResponse, g_aucRequest, unCmdId, ucReturnCode, ucTableIndex, &ucApiDataSize); // we do not care the return value of this function

        // Send response command via full api
        if (API_SendResponse(CLS_USER_BOARD , p_ucCmdType, p_ucMsgId, ucApiDataSize, g_aucRequest, 0) != API_SUCCESS)
        {
            ucReturnCode = RCS_N255_ResponseNack; // this error code is read by the API_OnRcvBoardCommand() function, which sends NACK packet over full api
        }
    }

    return ucReturnCode; // this error code is read by the API_OnRcvBoardCommand() function, which sends ACK/NACK packet over full api
}



/**
 * @brief This function disables the configured burst message.
 * @param[in] *p_ucRcvDataBuff - pointer to the received data payload
 * @param[in] p_ucMsgSize - pointer to the received data payload
 */
void App_FlushBurstMsg( uint8* p_ucRcvDataBuff, uint8 p_ucMsgSize)
{
uint8 ucBurst = *p_ucRcvDataBuff;

    if (ucBurst >= MAX_BW_SERVICES_NO)
        return;

    g_stRFBWService[ucBurst].m_ucSrvcReqStatus = SRVC_REQ_NONE;
    g_stRFBWService[ucBurst].m_ucSrvcModeControlCode = Off;
    g_flagDynamicRFBWService = FALSE;

    if (ucBurst < MAX_BURST_MSG_NO)
    { // burst
        g_stProv.m_stBurstNVM[ucBurst].m_ucBurstModeControlCode = Off;
        FLASH_UPDATE_NEEDED();
    }
    else if ( (ucBurst >= MAX_BURST_MSG_NO) && (ucBurst < MAX_BW_SERVICES_NO) )
    { // event
        g_stProv.m_stEventNVM.m_ucEventControlCode = Off;
        FLASH_UPDATE_NEEDED();
    }
}


/**
 * @brief Parses the incoming data payload and updates the burst message period.
 * @param[in] *p_ucRcvDataBuff - pointer to Full message payload.
*/
void App_UpdateBurstPeriod (uint8* p_ucRcvDataBuff)
{
uint8 ucBurst = *p_ucRcvDataBuff++;
uint32  ulUpdatePeriod;

    if (ucBurst >= MAX_BW_SERVICES_NO)
        return;

    InverseMemcpy((uint8 *)&ulUpdatePeriod, (uint8 *)p_ucRcvDataBuff, 4);
    ulUpdatePeriod /= 32; // switch to miliSeconds

    if (ulUpdatePeriod == 0)
        return;

    if (ucBurst < MAX_BURST_MSG_NO)
    { // burst
        g_stProv.m_stBurstNVM[ucBurst].m_ulUpdateTime = ulUpdatePeriod;                                  // the burst time format is msec
        //NVM_SaveData(ucBurst + 4, (uint8 *)&g_stBurstNVM[ucBurst], sizeof(NV_BURST_T));      // new values, so save data into nvm
        FLASH_UPDATE_NEEDED();
    }
    else if ( (ucBurst >= MAX_BURST_MSG_NO) && (ucBurst < MAX_BW_SERVICES_NO) )
    { // event
        g_stProv.m_stEventNVM.m_ulRetryTime  = ulUpdatePeriod;
        //NVM_SaveData(OFFSET7, (uint8 *)&g_stEventNVM, sizeof(NV_EVENT_T));
        FLASH_UPDATE_NEEDED();
    }

    SET_CONFIGURATION_CHANGED();
    g_stRFBWService[ucBurst].m_ucSrvcModeControlCode = Off;
    g_stRFBWService[ucBurst].m_ulSrvcInServicePeriod = ulUpdatePeriod;
    g_stRFBWService[ucBurst].m_flagSrvcInServiceSyncro = 1;
    g_stRFBWService[ucBurst].m_ucSrvcDenied = 0;
}

/**
 * @brief This function is called when Radio Module sends a message indicating that Long Tag has been changed.
 * Application Processor will raise an event which will indicate the change of the Long Tag.
 * @param[in] *p_ucRcvDataBuff - pointer to Full message payload.
*/
void App_AsyncOperation (uint8* p_ucRcvDataBuff)
{
    if ( ( p_ucRcvDataBuff[0] == 0 ) &&  ( p_ucRcvDataBuff[1] == 0 ) )
    { // Tag was changed on WISA ... raise an event
        g_stCmd48Data.byte[2] |= 0x01;
        // seteaza si MSA ...
        g_ucDeviceStatus[MASTER_PRIMARY] |= FieldDeviceStatusesMask_MoreStatusAvailable;
        g_ucDeviceStatus[MASTER_SECONDARY] |= FieldDeviceStatusesMask_MoreStatusAvailable;
        g_ucDeviceStatus[MASTER_WISA] |= FieldDeviceStatusesMask_MoreStatusAvailable;
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// Name:
// Description:
// Param:
// Return:
// Access level:
////////////////////////////////////////////////////////////////////////////////////////////////////
void App_GetChangeCounter (uint8* p_ucRcvDataBuff)
{
    // response on get change counter request ... used only in events queue for now
    // ... update local cccounter eventually

    EVENT_PENDING_T* pEvent = g_stEventQueue;

    // latest CC counter
    g_unMirrorCCCounter = ((0xFF & p_ucRcvDataBuff[0]) << 8) | (0xFF & p_ucRcvDataBuff[1]);

    for ( ; pEvent < g_pEventQueueEnd; pEvent++ )
    {
        if ( (pEvent->m_ucCCRequested == 0x55) && (pEvent->m_ucAckReceived == 0) )
        {
            pEvent->m_ucCCCounter = ((0xFF & p_ucRcvDataBuff[0]) << 8) | (0xFF & p_ucRcvDataBuff[1]);
            pEvent->m_ucCCRequested = 0xAA; // valid CC received ... event can be sent
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Name:
// Description:
// Param:
// Return:
// Access level:
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 App_WriteAdditionalDeviceStatus(void)
{
uint8 aucOutBuf[26];

    aucOutBuf[0] = g_ucDeviceStatus[MASTER_WISA];
    aucOutBuf[1] = 0x80;
    memset(aucOutBuf+2, 0x00, 24);
    API_SendRequest(CLS_STACK_SPECIFIC, HART_WRITE_ADDITIONAL_DEVICE_STATUS, 0, 26, aucOutBuf, 0);
    return 1;
}



/**
* @brief Returns the burst trigger source value. Output is a pointer to the structure that contains the device variable value.
* @param[in] p_ucCode - the Device Variable code of the trigger source
*/

DEVICE_VARIABLE_T* App_TriggerSorceVal(uint8 p_ucCode)
{
DEVICE_VARIABLE_T*    pstDevVarValue;

    if ((App_GetDeviceVar(p_ucCode) != NULL) && ((pstDevVarValue =  App_GetDeviceVarValue(p_ucCode)) != NULL))
    {
        return pstDevVarValue;
    }

    return NULL;
}



/**
* @brief Returns a pointer to a Device Variable's properties. Output is a pointer to the structure that contains the device variable information.
* @param[in] p_ucCode - the device variable code
*/

NV_DEVICE_VARIABLE_T * App_GetDeviceVar(uint8 p_ucCode)
{
    if(p_ucCode < CST_DEV_VARS_NO)
    {
        return (g_stProv.m_astDeviceVars + p_ucCode);
    }
    else
    { // HART Standardized var
        if ((p_ucCode >= BatteryLife ) && (p_ucCode <= QuaternaryVariable ))
        {
            if (p_ucCode <= LoopCurrent)
            {
                if (p_ucCode==BatteryLife)
                {
                    return (g_stProv.m_astDeviceVars + 1/*DEVICE_VARIABLE_3*/);
                }
                return (g_stProv.m_astDeviceVars + (CST_DEV_VARS_NO + p_ucCode - BatteryLife));
            }
            else
            {
                // PV, TV, SV or QV
                p_ucCode -= PrimaryVariable;
                switch (g_stProv.m_aucHartDynVarsAssignCodes[p_ucCode])
                {
                case DEVICE_VARIABLE_0:
                case DEVICE_VARIABLE_1:
                case DEVICE_VARIABLE_2:
                case DEVICE_VARIABLE_3:
                case DEVICE_VARIABLE_4:
                case DEVICE_VARIABLE_5:
                    return (g_stProv.m_astDeviceVars + g_stProv.m_aucHartDynVarsAssignCodes[p_ucCode]);
                case BatteryLife:
                    return (g_stProv.m_astDeviceVars + DEVICE_VARIABLE_4);
                default:
                    return NULL;
                }
            }
        }
    }
    // Invalid code
    return NULL;
}

/**
* @brief Returns a pointer to a Device Variable's current value, based on the code. Output is a pointer to the structure that contains the device variable value.
* @param[in] p_ucCode - the device variable code
*/
DEVICE_VARIABLE_T * App_GetDeviceVarValue(uint8 p_ucCode)
{
uint8 ucCodeIndex = 0;

    if (p_ucCode >= CST_DEV_VARS_NO)
    { // NOT Device Variable Code
        if ((p_ucCode >= BatteryLife ) && (p_ucCode <= QuaternaryVariable ))
        { // HART Standardized variables
            if (p_ucCode <= LoopCurrent)
            { // BatteryLife, PercentRange, LoopCurrent
                if (p_ucCode==BatteryLife)
                {
                    return (g_stProv.m_astDeviceVarsValues + DEVICE_VARIABLE_4);
                }
                return (g_stProv.m_astDeviceVarsValues + (CST_DEV_VARS_NO + p_ucCode - BatteryLife));
            }
            else
            { // PV, SV, TV or QV
                ucCodeIndex = p_ucCode - PrimaryVariable;
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
                    return NULL;
                }
            }
        }
        else
        { // p_ucCode ... between CST_DEV_VARS_NO and BatteryLife ... or ... > QuaternaryVariable => illegal values
            return NULL;
        }
    }
    else
    { // Device Variable Code
        ucCodeIndex = p_ucCode;
    }

    // from here ucCodeIndex should be < CST_DEV_VARS_NO ... i.e. idx of device variable.

    if (g_stProv.m_astDeviceVarsValues[ucCodeIndex].m_ucNormalOperation == WriteDeviceVariableCodes_FixedValue)
    {
        return (g_stProv.m_astDeviceVarsValues + ucCodeIndex);
    }
    g_stProv.m_astDeviceVarsValues[ucCodeIndex].m_stLastValue = g_stProv.m_astDeviceVarsValues[ucCodeIndex].m_stValue;
    /*
    
     * UPDATED DEVICE VARIABLE ON 8MAR23
    Device Variable 0 --> Thickness in inches Raw
    Device Variable 1 --> Thickness in inches Temp. Compensated
    Device Variable 2 --> Material Temperature
    Device Variable 3 --> % Battery Remaining
    Device Variable 4 --> DSI Temperature
    Device Variable 5 --> Remaining Battery Life in days
    */
    switch (ucCodeIndex)
    {
        case DEVICE_VARIABLE_0:
            g_stProv.m_astDeviceVarsValues[DEVICE_VARIABLE_0].m_stValue.m_fValue = mat_thick_raw_0;  //(g_stDataToPublish.fpValue);
            break;
        case DEVICE_VARIABLE_1:
            g_stProv.m_astDeviceVarsValues[DEVICE_VARIABLE_1].m_stValue.m_fValue = mat_thick_raw_1; //g_stDeviceProcessVariables.m_unMeasurementTimeInterval;
            break;
        case DEVICE_VARIABLE_2:  // should be converted to %
            g_stProv.m_astDeviceVarsValues[DEVICE_VARIABLE_2].m_stValue.m_fValue = mat_thick_raw_2; 
            break;
        case DEVICE_VARIABLE_3:
             g_stProv.m_astDeviceVarsValues[DEVICE_VARIABLE_3].m_stValue.m_fValue = mat_thick_raw_3;  
            break;
        case DEVICE_VARIABLE_4:
            g_stProv.m_astDeviceVarsValues[DEVICE_VARIABLE_4].m_stValue.m_fValue = dsi_t_0; //EnergyLeftDays;
            break;
        case DEVICE_VARIABLE_5:
            g_stProv.m_astDeviceVarsValues[DEVICE_VARIABLE_5].m_stValue.m_fValue = EnergyLeftDays;  //g_stDeviceProcessVariables.m_ucAlarmMode;
            break;
        default:
            return NULL;
    }
    return (g_stProv.m_astDeviceVarsValues + ucCodeIndex);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// Name:
// Description:
// Param:
// Return:
// Access level:
////////////////////////////////////////////////////////////////////////////////////////////////////
float ConvertThicknessVariable(float value, uint8 oldUnit, uint8 newUnit) {
    // Value is read in mils per year, needs to be converted to whatever is set now
    if (oldUnit == newUnit) {
        return value;
    }
    // Make sure we have millimeters as default to simplify the conversion
    switch (oldUnit) {
        case UnitsCodes_Millimeters:
            //No need to convert mpy to mpy
            break;
        case UnitsCodes_Inches:
            value = value * 25.4f;
            break;
        default:
            return 0x7fc00000f; //invalid unit, not implemented
    }

    // Convert to the desired unit
    switch (newUnit) {
        case UnitsCodes_Millimeters:
            //No need to convert mpy to mpy
            break;
        case UnitsCodes_Inches:
            value = value / 25.4f;
            break;
        default:
            return 0x7fc00000f; //invalid unit, not implemented
    }

    return value;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// Name:
// Description:
// Param:
// Return:
// Access level:
////////////////////////////////////////////////////////////////////////////////////////////////////
float ConvertToNewUnit(uint8 p_ucCode, float value, uint8 oldUnit, uint8 newUnit)
{
    if (p_ucCode == DEVICE_VARIABLE_0)
    {
        return ConvertThicknessVariable(value, oldUnit, newUnit);
    }

    return value;
}



/**
* @brief Checks if a command is supported by the Application Processor. This function returns a Service Feedback Code.
* @param[in] p_unCommandNo   - Command number
* @param[in] p_unTableIndex  - pointer to the index of the command in the Application Processor command table (if found)
*/
uint8 App_CommandSupported(uint16 p_unCommandNo, uint16 *p_unTableIndex)
{
uint16 unIndex;

    // Search for the Command ID in the AP command table
    for (unIndex = 0; unIndex < (sizeof(g_astCmdTable)/sizeof(g_astCmdTable[0])); unIndex++)
    {
        if (p_unCommandNo == g_astCmdTable[unIndex].cmdId)
        { // Command ID found in table, check if the execute function is not NULL
            if (g_astCmdTable[unIndex].cmdExec != NULL)
            { // Command execute function not NULL, update p_ucTableIndex and return success
                *p_unTableIndex = unIndex;
                return COMMAND_FOUND;
            }
            // Command execute function is NULL
            break;
        }
    }
    // Command not found in command table or execute function is NULL
    return COMMAND_NOT_FOUND; // Return error
}



/**
* @brief Fetches WHART command no 1. Returns a pointer to the next data to be parsed.
* @param[in]  p_ucCmdType - Wireless or Wired message type for the class USER_BOARD: DAQ_FW_WIRELESS
* @param[in] *p_pucInBuffer - pointer to FULL API message payload - WIRELESS/WIRED PACKET for a Request: Status byte + command ID, Byte Count , Command Data Bytes
* @param[in] *p_pParsedCmd - pointer to the specific parse function
* @param[in] *p_punCmdId - Command ID(2 bytes long)
* @param[in] *p_pIndex - Retain the index from the table -  const CmdEntry g_astCmdTable[]
*/
uint8 App_ParseCommand(uint8 p_ucCmdType, uint8 *p_pucInBuffer, void *p_pParsedCmd, uint16 *p_punCmdId, uint8 *p_pIndex)
{
uint8 ucCmdSize = 0;
uint8 ucTableIndex;
uint8 ucReturnCode;
ParseFunction pfParseCmd = NULL;

    p_pucInBuffer ++; // device status received from API
    if (p_ucCmdType == DAQ_FW_WIRELESS)
    {
        // wireless packet, so cmd occupies 2 bytes
        InverseMemcpy((uint8*)p_punCmdId, p_pucInBuffer, 2); // MSB is sent first via API, but MSP430 is little-endian
        p_pucInBuffer += 2;
        // extract cmd size
        memcpy(&ucCmdSize, p_pucInBuffer, 1);
        p_pucInBuffer += 1;
    }
    //else
    //{ // if we reach here, then there is an error
    //    return 0;
    //}

    // check if the command is supported by the AP
    for (ucTableIndex = 0; ucTableIndex < (sizeof(g_astCmdTable)/sizeof(g_astCmdTable[0])); ucTableIndex++)
    {
        if ((*p_punCmdId) == g_astCmdTable[ucTableIndex].cmdId)
        {
            pfParseCmd = g_astCmdTable[ucTableIndex].cmdParse;
        break;
        }
    }

    if (pfParseCmd == NULL)
    {
        ucReturnCode = RCS_E64_CommandNotImplemented;
    }
    else
    {
        if (ucCmdSize < g_astCmdTable[ucTableIndex].cmdSize)
        {
            ucReturnCode = RCS_E05_TooFewDataBytesReceived;
        }
        else
        {
            ucReturnCode = (*pfParseCmd)(p_pParsedCmd, p_pucInBuffer, ucCmdSize);
        }
    }

    *p_pIndex = ucTableIndex;

    return ucReturnCode;
}



/**
 * @brief Verifies if an Application Processor Command returns an error.
 * @param[in] p_ucRc - Specific Command Response.
 */
uint8 IsResponseCodeError(uint8 p_ucRc)
{
  return ((1 <= p_ucRc && p_ucRc <= 7) || (9 <= p_ucRc && p_ucRc <= 13) || (15 <= p_ucRc && p_ucRc <= 23)
         || (p_ucRc == 28) || (p_ucRc == 29) || (32 <= p_ucRc && p_ucRc <= 95));
}




/**
* @brief Composes the WHART command. Output is a pointer to the next data to be parsed.
* @param[in] p_ucCmdType - Wireless(0x00) or Wired(0x04) message type for the class USER_BOARD: DAQ_FW_WIRELESS
* @param[in] *p_pucInBuffer - pointer to API message payload- WIRELESS/WIRED PACKET for a Response: Command Data Bytes
* @param[in] *p_pComposedCmd- contains the FULL API payload field [cmdID + Data Len + RespCode + Whart payload]
* @param[in] p_unCmdId - Command ID(2 bytes long)
* @param[in] p_ucRespCode - CommandResponseCodes (for example RCS_N00_Success which is 0x00)
* @param[in] p_ucIndex - Retain the index from the table -  const CmdEntry g_astCmdTable[]
* @param[in] *p_ucApiDataSize - pointer to ApiDataSize
*/
uint8 App_ComposeCommand(uint8 p_ucCmdType, void *p_pucInBuffer, uint8 *p_pComposedCmd, uint16 p_unCmdId, uint8 p_ucRespCode, uint8 p_ucIndex, uint8 *p_ucApiDataSize)
{
uint8 ucReturnCode;
uint8 ucDataLength;
uint8 *pDataLen; // temp pointer to store the address for the Data Length
ComposeFunction pfComposeCmd = NULL;

    *p_ucApiDataSize = 0;

    if (p_ucCmdType == DAQ_FW_WIRELESS)
    {
        *(p_pComposedCmd++) = g_ucDeviceStatus[MASTER_WISA];
        // wireless packet, so cmd occupies 2 bytes
        InverseMemcpy((uint8*)p_pComposedCmd, (uint8*)&p_unCmdId, 2); // MSB is sent first via API, but MSP430 is little-endian
        p_pComposedCmd += 2;
        *p_ucApiDataSize = 3; // 1 byte device status + 2 bytes command ID
        g_ucDeviceStatus[MASTER_WISA] &= (~FieldDeviceStatusesMask_ConfigurationChanged); // clear CC flag
    }

    pDataLen = p_pComposedCmd; //retain the position of dataLength byte inside the array for later use
    // compose response header as for an error RC - no RespData will be added
    *(p_pComposedCmd++) = 1; // in case of an error RC, len = RespCode [+ DevStatusByte + [ExtendedCommNumber]]
    *(p_pComposedCmd++) = p_ucRespCode;

    if (IsResponseCodeError(p_ucRespCode))
    {
        *p_ucApiDataSize += 2;
        return 1;
    }

    pfComposeCmd = g_astCmdTable[p_ucIndex].cmdCompose;

    if (pfComposeCmd == NULL)
    {
        ucReturnCode = RCS_E64_CommandNotImplemented;
    }
    else
    {
        ucReturnCode = (*pfComposeCmd)(p_pucInBuffer, p_pComposedCmd, &ucDataLength);
    }

    *pDataLen = ucDataLength + 1;
    *p_ucApiDataSize += *pDataLen + 1;

    return ucReturnCode;
}



/**
* @ brief Composes the WHART Burst Response.
* @param[in] p_ucCommandNo - Burst Command Number.
* @param[in] *p_aucRequest - pointer to Command Request Data
* @param[in] *p_aucResponse- pointer to Command Response Data
* @param[in] *p_ucApiDataSize - pointer to ApiDataSize
*/
uint8 App_ComposeBurstRsp(uint16 p_ucCommandNo, void *p_aucRequest, void *p_aucResponse, uint8 *p_ucApiDataSize)
{
uint16 unTableIndex;
uint8 ucReturnCode;
ExecuteFunction cmdExecute = NULL; // Pointer to execute function

    // Check if the command is supported by the AP
    if (App_CommandSupported(p_ucCommandNo, &unTableIndex) == COMMAND_NOT_FOUND)
    {
        return RCS_E02_InvalidSelection;
    }

    cmdExecute = g_astCmdTable[unTableIndex].cmdExec;

    ucReturnCode = (*cmdExecute)((void *)p_aucRequest, (void *)p_aucResponse, DAQ_FW_WIRELESS);

    if ((ucReturnCode != RCS_N254_ResponseAck) && (ucReturnCode != RCS_N255_ResponseNack))
    {
        // p_aucRequest contains the FULL API payload field [cmdID + Data Len + RespCode + Whart payload]
        App_ComposeCommand(DAQ_FW_WIRELESS, (void *)p_aucResponse, (uint8 *)p_aucRequest, p_ucCommandNo, ucReturnCode, unTableIndex, p_ucApiDataSize);
    }

    return RCS_N00_Success;
}


/**
* @brief Prepares and sends Burst Message data payload accordingly to current Burst Command Number.
* @param[in] p_ucBurstNo - burst Message Number
*/
void App_PrepareAndSendBurst(uint8 p_ucBurstNo)
{ // Only Burst messages ... Event will used other dedicated function
//uint8 ucApiResponseSize;
uint8 ucIdx;

    if (p_ucBurstNo >= MAX_BURST_MSG_NO)
        return;

    switch(g_stProv.m_stBurstNVM[p_ucBurstNo].m_unCmdNo)
    {
    case 1: // Request Data Bytes: None
        break;

    case 2: // Request Data Bytes: None
        break;

    case 3: // Request Data Bytes: None
        break;

    case 9:
        for (ucIdx = 0; ucIdx < C009_MAX_REQ_SIZE; ucIdx++)
        {
            if (g_stProv.m_stBurstNVM[p_ucBurstNo].m_aucDeviceVariableCodes[ucIdx] != NotUsed)
            {
                // Add to the Command 9 req
                ((C009_ReadDeviceVarsWithStatus_Req *) g_stRFBWService[p_ucBurstNo].m_aucRequest)->m_aucDeviceVarCode[ucIdx] = g_stProv.m_stBurstNVM[p_ucBurstNo].m_aucDeviceVariableCodes[ucIdx];
            }
            else
            {
                break;
            }
        }
        ((C009_ReadDeviceVarsWithStatus_Req *)g_stRFBWService[p_ucBurstNo].m_aucRequest)->m_ucNoDeviceVariables = ucIdx;
        break;

    case 33:
        for (ucIdx = 0; ucIdx < C033_MAX_REQ_SIZE; ucIdx++)
        {
            if (g_stProv.m_stBurstNVM[p_ucBurstNo].m_aucDeviceVariableCodes[ucIdx] != NotUsed)
            {
                // Add to the Command 9 req
                ((C033_ReadDeviceVars_Req *)g_stRFBWService[p_ucBurstNo].m_aucRequest)->m_aucDeviceVarCode[ucIdx] = g_stProv.m_stBurstNVM[p_ucBurstNo].m_aucDeviceVariableCodes[ucIdx];
            }
            else
            {
                break;
            }
        }
        ((C033_ReadDeviceVars_Req *)g_stRFBWService[p_ucBurstNo].m_aucRequest)->m_ucNoDeviceVariables = ucIdx;
        break;

    case 48:
        // we are using cmd req 0 bytes to compose all 25 bytes
        ((C048_ReadAdditionalDeviceStatus_Req *)g_stRFBWService[p_ucBurstNo].m_aucRequest)->commandSize = 0;
        break;

    default:
        return; // Burst command error
    }

    g_stRFBWService[p_ucBurstNo].m_ucResponseDataLen = 0;
    //
    if ( App_ComposeBurstRsp(g_stProv.m_stBurstNVM[p_ucBurstNo].m_unCmdNo, (void *)g_stRFBWService[p_ucBurstNo].m_aucRequest, (void *)g_stRFBWService[p_ucBurstNo].m_aucResponse, &g_stRFBWService[p_ucBurstNo].m_ucResponseDataLen) == 0)
    {
        g_ucAppProcessorStatus = API_STATE_PUBLISH_OK;
    }

}


/**
* @brief Prepares Event Message data payload.
* @param[in] *p_aucRequest - pointer to the request payload
* @param[in] *p_aucResponse - pointer to the response payload
* @param[in] *p_ucApiDataSize - pointer to the API Data Size
*/
uint8 App_ComposeEventRsp(void *p_aucRequest, void *p_aucResponse, uint8 *p_ucApiDataSize)
{
uint16 unTableIndex;
uint8 ucReturnCode = RCS_N00_Success;

    // Check if the command is supported by the AP
    if (App_CommandSupported(119, &unTableIndex) == COMMAND_NOT_FOUND)
    {
        return RCS_E02_InvalidSelection;
    }

    App_ComposeCommand(DAQ_FW_WIRELESS, (void *) p_aucRequest, (uint8 *) p_aucResponse, 119, ucReturnCode, unTableIndex, p_ucApiDataSize);
    return RCS_N00_Success;
}

/**
* @brief Sends Event Message data payload.
* @param[in] p_ucBurstNo - index of the RF Bandwidth array
* @param[in] p_ucType - 0 = retry from queue, 1 = not an event
*/
void App_PrepareAndSendEvents(uint8 p_ucBurstNo, uint8 p_ucType)
{ // Only Events
    if (p_ucBurstNo != MAX_BURST_MSG_NO)
        return;

    C119_AcknowledgeEventNotification_Req * pReq = (C119_AcknowledgeEventNotification_Req *) g_stRFBWService[p_ucBurstNo].m_aucResponse;

    if ( p_ucType == 0 )
    { // retry period from queue
        pReq->eventNo = g_stEventQueue[0].m_ucEventNo;
        pReq->firstUnacknowledged.u32 = g_stEventQueue[0].m_ulTimestamp;
        pReq->configurationChangedCounter = g_stEventQueue[0].m_ucCCCounter;
        pReq->deviceStatus = g_stEventQueue[0].m_ucDeviceStatus;
        memcpy( pReq->command48Data, g_stEventQueue[0].m_aucEvents, 25 );
    }
    else
    { // max update once ... not an event
        pReq->eventNo = 0x00;
        pReq->firstUnacknowledged.u32 = 0xFFFFFFFF;
        pReq->configurationChangedCounter = g_unMirrorCCCounter;
        pReq->deviceStatus = g_ucDeviceStatus[MASTER_WISA];
        memcpy( pReq->command48Data, &g_stCmd48Data.byte[0], 25 );
    }

    g_stRFBWService[p_ucBurstNo].m_ucResponseDataLen = 0;

    if ( App_ComposeEventRsp((void *)g_stRFBWService[p_ucBurstNo].m_aucResponse, (void *)g_stRFBWService[p_ucBurstNo].m_aucRequest, &g_stRFBWService[p_ucBurstNo].m_ucResponseDataLen) == 0)
    {
        g_ucAppProcessorStatus = API_STATE_PUBLISH_OK;
    }

}


