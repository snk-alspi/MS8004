#include <string.h>
#include "../Global/typedef.h"
#include "../Platform/timers.h"
#include "wired_hart_link_layer.h"
#include "wired_hart_app_layer.h"
#include "../WirelessHART_Full_API/wh_full_api.h"
#include "../WirelessHART_AppLayer/App_WH_User.h"
#include "../WirelessHART_AppLayer/App_WH_CmdParse.h"
#include "../WirelessHART_AppLayer/App_WH_CmdExecute.h"
#include "../WirelessHART_AppLayer/App_WH_CmdCompose.h"


static uint8 g_aucHARTRequest[256];
static uint8 g_aucHARTResponse[256];

/**
 * @brief Processes an incoming wiredHART message.
 * @param[in] *p_pucMsgBuf - pointer to the message buffer.
 * @param[in] p_unSize
 */
uint8 AppHART_Rx_Indicate ( const uint8 * p_pucMsgBuf, uint8 p_unSize )
{
uint8 * pucDelimiter = (uint8*) p_pucMsgBuf;
uint8 * pucAddress = NULL;
uint8 * pucCommand = NULL;
uint8 * pucByteCount = NULL;
uint8 * pucData = NULL;
uint8 ucMsgSize;
uint8 ucRespCode;
uint16 unCmdId;

    //
    if ( (*pucDelimiter & HART_FRAME_TYPE_MASK) != HART_FRAME_STX )
        return 1; //error

    //
    if  ( ! (
            (
                (p_pucMsgBuf[0] & HART_DELIMITER_ADDRESS_MASK)
                    &&
                (
                    ((p_pucMsgBuf[1] & HART_FIELD_EXPDEVTYPE_MSBITS) == g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_ExpDevTypeMSBits)
                        &&
                    ((p_pucMsgBuf[2] & HART_FIELD_EXPDEVTYPE_LSBITS) == g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_ExpDevTypeLSBits)
                        &&
                    (p_pucMsgBuf[3] == g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_UniqueDevID[0])
                        &&
                    (p_pucMsgBuf[4] == g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_UniqueDevID[1])
                        &&
                    (p_pucMsgBuf[5] == g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_UniqueDevID[2])
                )
            )
            ||
            (
                (!(p_pucMsgBuf[0] & HART_DELIMITER_ADDRESS_MASK))
                    &&
                (p_pucMsgBuf[1] & HART_FIELD_POOLING_ADDRESS_BITS) == g_stProv.m_sWiredHart.m_Device.m_PollingAddr.m_AddrBits
            )
            ||
            (
                (p_pucMsgBuf[0] & HART_DELIMITER_ADDRESS_MASK)
                    &&
                (
                    ((p_pucMsgBuf[1] & HART_FIELD_EXPDEVTYPE_MSBITS) == 0x00)
                        &&
                    ((p_pucMsgBuf[2] & HART_FIELD_EXPDEVTYPE_LSBITS) == 0x00)
                        &&
                    (p_pucMsgBuf[3] == 0x00)
                        &&
                    (p_pucMsgBuf[4] == 0x00)
                        &&
                    (p_pucMsgBuf[5] == 0x00)
                        &&
                    (p_pucMsgBuf[6] == 11) // command 11 on broadcast address
                )
            )
        )   )
        return 1; // error not for me ... discard message

    // go further start transaction initiated by master

    // Address field
    pucAddress = (uint8*) p_pucMsgBuf + 1;

    // Command field
    pucCommand = pucAddress;
    if (*pucDelimiter & HART_DELIMITER_ADDRESS_MASK)
    { // long address
        pucCommand +=5;
    }
    else
    { // short address
        pucCommand += 1;
    }
    //
    if (*pucDelimiter & HART_DELIMITER_EXPBYTES_MASK )
    { // has expansion bytes
        pucCommand += ((*pucDelimiter & HART_DELIMITER_EXPBYTES_MASK) >> 5);
    }

    // ByteCount field
    pucByteCount = pucCommand + 1;

    // Data field start
    pucData = pucByteCount + 1;

    // Cmd + ByteCount + DataLen
    ucMsgSize = 1 + 1 + *pucByteCount;

    // Extended Command check
    if (*pucCommand == 0x1F)
    { // Extended Command
        unCmdId = (uint16) 0 | ( (pucData[0] << 8) | pucData[1] );
    }
    else
    { // Single byte command
        unCmdId = (uint16) *pucCommand;
    }

    if (  (!((*pucDelimiter) & HART_DELIMITER_ADDRESS_MASK) )  && ((*pucCommand) > 0) )
        return 1; // on short address response only to command 0

    // check RX errors and respond as requested by standard
    if (g_stProv.m_sWiredHart.m_RxMsgErrors)
    { // we have Communication Error ... see spec 085 ch 8.1.3 Error Responses
        uint8 aucRespBuff[4];
        aucRespBuff[0] = *pucCommand;                           // Cmd
        aucRespBuff[1] = 2;                                     // ByteCount
        aucRespBuff[2] = 0x80 | g_stProv.m_sWiredHart.m_RxMsgErrors;     // Comm Err
        aucRespBuff[3] = ((HART_DELIMITER_ADDRESS_MASK & (*pucAddress)) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]); // Field Dev Status
        AppHART_Tx_Response(aucRespBuff, 4);
    }
    else
    {
        if ( AppHART_IsLocalCmd(unCmdId) == 0 )
        { // for me => process it and response
            ucRespCode = AppHART_ProcessLocalCmd(pucCommand);


            if ( ! (
                     (ucRespCode == RCS_N00_Success) ||
                     (ucRespCode == RCS_N254_ResponseAck) ||
                     ((ucRespCode == RCM_W14_StatusBytesMismatch) && (unCmdId == CMDID_C048_ReadAdditionalDeviceStatus)) ||
                     ((ucRespCode == RCM_W08_UpdateTimesAdjusted) && (unCmdId == CMDID_C103_WriteBurstPeriod))
                   )
                )
            { // we Response Code with Error ... see spec 085 ch 8.1.3 Error Responses
                uint8 aucRespBuff[6];

                if (*pucCommand != 0x1F)
                { // single byte command
                    aucRespBuff[0] = *pucCommand;                           // Cmd
                    aucRespBuff[1] = 2;                                     // ByteCount
                    aucRespBuff[2] = 0x7F & ucRespCode;                     // Response Code
                    aucRespBuff[3] = ((HART_DELIMITER_ADDRESS_MASK & (*pucAddress)) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]); // Field Dev Status
                    AppHART_Tx_Response(aucRespBuff, 4);
                }
                else
                { // extended command
                    aucRespBuff[0] = *pucCommand;                           // Cmd
                    aucRespBuff[1] = 4;                                     // ByteCount
                    aucRespBuff[2] = 0x7F & ucRespCode;                     // Response Code
                    aucRespBuff[3] = ((HART_DELIMITER_ADDRESS_MASK & (*pucAddress)) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]); // Field Dev Status
                    aucRespBuff[4] = 0xFF & (unCmdId >> 8);
                    aucRespBuff[5] = 0xFF & unCmdId;
                    AppHART_Tx_Response(aucRespBuff, 6);
                }
            }
            // else already responded
        }
        else
        { // forward
            if ( ucMsgSize < API_MAX_BUFFER_SIZE )
            {
                uint8 aucBuffer[API_MAX_BUFFER_SIZE];
                aucBuffer[0] = ( ((*pucAddress) & 0x80) ? 0x01 : 0x00);
                memcpy( (void *) (aucBuffer + 1) , (const void *) pucCommand, ucMsgSize );
                // one byte at beginning: 0 = secondary master; 1 = primary master;
                if ( API_SUCCESS != API_SendRequest(CLS_USER_BOARD, DAQ_FW_HART_MODEM, 0, ucMsgSize + 1, aucBuffer, 1 ))
                {
                    ;
                }
                API_Tx_Handler(1); // Accelerate response
            }
        }
    }
    return 0;
}


/**
 * @brief Verifies if a command received from wiredHART medium is locally implemented or needs to be forwarded to the radio module.
 * @param[in] p_unCmdId - received command ID.
 */
uint8 AppHART_IsLocalCmd(uint16 p_unCmdId)
{
    switch (p_unCmdId)
    {
    case CMDID_C001_ReadPV:
    case CMDID_C002_ReadLoopCurrAndPercentRange:
    case CMDID_C003_ReadDynamicVarsAndLoopCurrent:
    case CMDID_C006_WritePollingAddress:
    case CMDID_C007_ReadLoopConfiguration:
    case CMDID_C008_ReadDynamicVarsClassif:
    case CMDID_C009_ReadDeviceVarsWithStatus:
    case CMDID_C014_ReadPrimaryVarTransducerInfo:
    case CMDID_C015_ReadDeviceInfo:
    case CMDID_C033_ReadDeviceVars:
    case CMDID_C035_WritePVRangeValues:
    case CMDID_C044_WritePVUnits:
    case CMDID_C048_ReadAdditionalDeviceStatus:
    case CMDID_C050_ReadDynamicVariableAssignments:
    case CMDID_C051_WriteDynamicVariableAssignments:
    case CMDID_C054_ReadDeviceVariableInformation:
    case CMDID_C059_WriteNumberOfResponsePreambles:
    case CMDID_C079_WriteDeviceVariable_Req:
    case CMDID_C103_WriteBurstPeriod:
    case CMDID_C104_WriteBurstTrigger:
    case CMDID_C105_ReadBurstModeConfiguration:
    case CMDID_C107_WriteBurstDeviceVars:
    case CMDID_C108_WriteBurstModeCommandNo:
    case CMDID_C109_BurstModeCtrl:
    case CMDID_C115_ReadEventNotificationSummary:
    case CMDID_C116_WriteEventNotificationBitMask:
    case CMDID_C117_WriteEventNotificationTiming:
    case CMDID_C118_EventNotificationControl:
    case CMDID_C119_AcknowledgeEventNotification:
    case CMDID_C778_ReadBatteryLife:
        return 0;
    }

    if ( ( 150 <= p_unCmdId ) && (p_unCmdId <= 253 ) )
        return 0;

    return 1;
}


/**
 * @brief Parses a local command.
 * @param[in] *p_pucInBuffer - pointer to the incoming data buffer.
 * @param[in] *p_pParsedCmd - pointer to the parsed data buffer.
 * @param[in] *p_punCmdId - Command ID
 * @param[in] *p_pIndex - Command's specific table index.
 */
uint8 AppHART_ParseLocalCmd(uint8 *p_pucInBuffer, void *p_pParsedCmd, uint16 *p_punCmdId, uint16 *p_pIndex)
{
uint8 ucCmdSize;
uint16 unTableIndex;
uint8 ucReturnCode;
ParseFunction pfParseCmd = NULL;

    // extract the extended cmd flag
    *p_punCmdId = 0;
    memcpy((uint8*)p_punCmdId, p_pucInBuffer, 1);
    p_pucInBuffer++;

    // extract cmd size
    memcpy(&ucCmdSize, p_pucInBuffer, 1);
    p_pucInBuffer++;

    // Extended Command check
    if ( *p_punCmdId==0x1F )
    { // we have local extended command ... 778
        *p_punCmdId = (uint16) 0 | ( p_pucInBuffer[0] << 8) | p_pucInBuffer[1];
        ucCmdSize -= 2;
    }

    if ( COMMAND_FOUND == App_CommandSupported(*p_punCmdId, &unTableIndex) )
    {
        pfParseCmd = g_astCmdTable[unTableIndex].cmdParse;
    }

    if (pfParseCmd == NULL)
    {
        ucReturnCode = RCS_E64_CommandNotImplemented;
    }
    else
    {
        if (ucCmdSize < g_astCmdTable[unTableIndex].cmdSize)
        {
            ucReturnCode = RCS_E05_TooFewDataBytesReceived;
        }
        else
        {
            ucReturnCode = (*pfParseCmd)(p_pParsedCmd, p_pucInBuffer, ucCmdSize);
        }
    }

    *p_pIndex = unTableIndex;
    return ucReturnCode;
}


// wired, 8-bitCmdId : CmdID + ByteCnt + RespCode + FieldDevStatusByte + ResponseData
uint8 AppHART_ComposeLocalCmd(void *p_pucInBuffer, uint8 *p_pComposedCmd, uint16 p_unCmdId, uint8 p_ucRespCode, uint16 p_unIndex, uint16 *p_unSize)
{
uint8 ucReturnCode;
uint8 ucDataLength;
uint8 * pByteCount;
ComposeFunction pfComposeCmd = NULL;

// extended command 778 ...
    if ( p_unCmdId > 255 )
    { // Extended command

        //
        *p_pComposedCmd = 0x1F;
        p_pComposedCmd++;

        // ByteCount
        pByteCount = p_pComposedCmd; // will be set later
        p_pComposedCmd++;

        // Response Code
        *p_pComposedCmd = p_ucRespCode;
        p_pComposedCmd++;

        // Field Device Status
        *p_pComposedCmd = ((HART_DELIMITER_ADDRESS_MASK & g_stProv.m_sWiredHart.m_RxMasterBurst) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]);
        p_pComposedCmd++;

        *p_pComposedCmd = 0xFF & (p_unCmdId >> 8);
        p_pComposedCmd++;

        *p_pComposedCmd = 0xFF & p_unCmdId;
        p_pComposedCmd++;
    }
    else
    { // Single byte command

        // Cmd field
        memcpy((uint8*)p_pComposedCmd, (uint8*)&p_unCmdId, 1);
        p_pComposedCmd++;

        // ByteCount
        pByteCount = p_pComposedCmd; // will be set later
        p_pComposedCmd++;

        // Response Code
        *p_pComposedCmd = p_ucRespCode;
        p_pComposedCmd++;

        // Field Device Status
        *p_pComposedCmd = ((HART_DELIMITER_ADDRESS_MASK & g_stProv.m_sWiredHart.m_RxMasterBurst) ? g_ucDeviceStatus[MASTER_PRIMARY] : g_ucDeviceStatus[MASTER_SECONDARY]);
        p_pComposedCmd++;
    }

    // Add Data
    pfComposeCmd = g_astCmdTable[p_unIndex].cmdCompose;
    if (pfComposeCmd == NULL)
    {
        ucReturnCode = RCS_E64_CommandNotImplemented;
    }
    else
    {
        ucReturnCode = (*pfComposeCmd)(p_pucInBuffer, p_pComposedCmd, &ucDataLength);
    }


    if ( p_unCmdId > 255 )
    { // Extended command
        *pByteCount = 1+1+2+ucDataLength;
    }
    else
    { // Single byte command
        // Set Byte Count = RC+FDS+RespLen
        *pByteCount = 1+1+ucDataLength;
    }

    // Add Cmd+BC
    *p_unSize = 2 + *pByteCount;

    return ucReturnCode;
}


/**
 * @brief Processes a local command.
 * @param[in] *p_pucCmdData - pointer to the command's data buffer.
 */
uint8 AppHART_ProcessLocalCmd(uint8 * p_pucCmdData)
{
uint16 unCmdId;
uint8 ucReturnCode;
uint16 unTableIndex;
uint16 unMsgLen;
ExecuteFunction cmdExecute = NULL;

    ucReturnCode = AppHART_ParseLocalCmd(p_pucCmdData, g_aucHARTRequest, &unCmdId, &unTableIndex);

    if ((ucReturnCode == RCS_E05_TooFewDataBytesReceived) || (ucReturnCode == RCS_E64_CommandNotImplemented))
        return ucReturnCode;

    if (
        (ucReturnCode == RCS_N00_Success)
            ||
        ( ( ucReturnCode == RCM_W30_CommandResponseTruncated ) && (unCmdId == CMDID_C009_ReadDeviceVarsWithStatus) ) // command 9 with more bytes ... test UAL011a
       )
    {
        g_stProv.m_sWiredHart.m_sNeedHART.m_unTime = READ_FREE_RUNNING_TIMER();
        g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse = 1;

        cmdExecute = g_astCmdTable[unTableIndex].cmdExec;
        ucReturnCode = (*cmdExecute)((void*)g_aucHARTRequest, (void*)g_aucHARTResponse, 0xA5); // 0xA5 = corner case ... signature for here enabled commands

        if (
            (RCS_N00_Success == ucReturnCode)
                ||
            ( ( ucReturnCode == RCM_W14_StatusBytesMismatch ) && (unCmdId == CMDID_C048_ReadAdditionalDeviceStatus) ) // command 48 with more bytes ... test UAL048b
                ||
            ( ( ucReturnCode == RCM_W08_UpdateTimesAdjusted ) && (unCmdId == CMDID_C103_WriteBurstPeriod) ) //
           )
        {
            AppHART_ComposeLocalCmd((void*)g_aucHARTResponse, g_aucHARTRequest, unCmdId, ucReturnCode, unTableIndex, &unMsgLen);

            LOOP_WAIT_MSECONDS(25);
            // g_aucHARTRequest contains the payload to be transmitted over UART HART MODEM link
//            {
//                volatile uint16 unDelay = 0x7FFF; // HCF test related
//                while (unDelay--)
//                    ;
//            }


            // prepare TX framing and start TX response to HART MODEM using UART
            AppHART_Tx_Response(g_aucHARTRequest, unMsgLen);
        }
    }
    return ucReturnCode;
}
