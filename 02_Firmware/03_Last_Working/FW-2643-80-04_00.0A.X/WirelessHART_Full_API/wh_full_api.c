#include <string.h>
#include "../Platform/timers.h"
#include "../Platform/gpio.h"
#include "../Platform/uart.h"
#include "../Lib/WirelessHART_Lib.h"
#include "../WirelessHART_Full_API/wh_full_api.h"
#include "../WirelessHART_Full_API/wh_full_api_queue.h"
#include "../Global/global.h"
#include "../Platform/nvm.h"
#include "../WiredHart/wired_hart_link_layer.h"
#include "../MS/MS_Data.h"
#include "../MS/MS_Process.h"
#include "../WirelessHART_AppLayer/App_WH_User.h"
#include "../WirelessHART_AppLayer/App_WH_CmdParse.h"
#include "../WirelessHART_AppLayer/App_WH_CmdExecute.h"
#include "../WirelessHART_AppLayer/App_WH_CmdCompose.h"



API_STATE g_ucAppProcessorStatus = API_STATE_INIT;
uint64 g_ullCrtASN;
volatile uint32 g_ulMonotonic;

volatile uint8 g_ucMsgID; ///< Variable used for storing the current message ID.

uint8 g_ucNetworkStatus;									///< Variable to store the actual network status of the device.
uint16 g_unAsnCorrection;									///< Variable used for ASN correction algorithm.
uint16 g_unASNInterogationTimer;							///< Variable to store the ASN interrogation timer increment.

uint16 g_unSTXTimestampMsec;								///< Variable used for storing the time stamp when an STX Frame was encountered.
uint16 g_unResponseTime;                                    ///< Get the absolute slot number from the Radio Module.
uint16 g_unDeltaTime;                                       ///< Variable to calculate the difference between response and request time - should be less than 10 milliseconds.
uint16 g_unDeltaTime2;                                      ///< Variable to calculate the difference between response time and the moment when STX-0xF1 is received.


volatile uint16 g_unWKUSignalSemaphoreCounter = 0;
volatile uint16 g_unWKUSignalEventTmr = 0;

/**
 * @brief Parses an incoming API message. Determines the class type of the message and calls specific a function for each type.
 */
void API_Rx_Handler( void )
{
    uint8 aucMsgBuf[API_MAX_BUFFER_SIZE];
    API_MSG_HEADER * pstMsgHdr = (API_MSG_HEADER *)aucMsgBuf;
    uint8 ucReturn = 1;
    
    uint8 ucPacketSize = API_Q_GetRxMsg( aucMsgBuf, sizeof(aucMsgBuf) );
    if( ucPacketSize == ( pstMsgHdr->m_ucMsgDataLen + sizeof(API_MSG_HEADER)) )
    {
        switch( pstMsgHdr->MsgHeader.m_eMsgClass )
        {
        case CLS_API_COMMANDS:
            ucReturn = API_OnRcvAPISpecific( (API_MSG_HEADER *)pstMsgHdr );
            break;
        case CLS_ACK:
            break;
        case CLS_NACK:
            break;
        case CLS_STACK_SPECIFIC:
            ucReturn = API_OnRcvStackSpecific( (API_MSG_HEADER *)pstMsgHdr );
            break;
        case CLS_USER_BOARD:
            ucReturn = API_OnRcvBoardCommand( (API_MSG_HEADER *)pstMsgHdr );
            break;
        default:
            API_SendResponse(CLS_NACK, NACK_UNSUPPORTED_FEATURE, ((API_MSG_HEADER *)pstMsgHdr)->m_ucMsgID, 0, NULL, 0);
            break;
        }
        
        if( !pstMsgHdr->MsgHeader.m_bIsRsp  )
        { // Accelerate a possible response that needs to be sent if not the response message will be sent later
            API_Tx_Handler(0);
        }
    }
    
    if( !ucReturn )
    {
        API_SendResponse(CLS_NACK, NACK_UNSUPPORTED_FEATURE, ((API_MSG_HEADER *)pstMsgHdr)->m_ucMsgID, 0, NULL, 0);
    }
}



/**
 * @brief Accelerates a message response from the Radio Module.
 *
 * When this function is called, it checks if there is any in progress API Communication.
 * If there is no communication, it will immediately get the next message from the API TX Queue and it will forward it to the Radio Module.
 */
void API_Tx_Handler( uint8 p_ucSource  )
{
    if( g_ucApiTxLen ) // TX on progress
    {
        return;
    }
    
    if ( !g_unTxQueueSize ) // nothing to TX
    {
        return;
    }
    
    if ( g_ucApiRxState != API_Q_RX_IDLE ) // avoid full-duplex ...
    {
        return;
    }

    if (
            (p_ucSource == 0)
            &&
            ((HART_CD_PIN_STATE()) || (g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse) || ((HART_RTS_PIN_STATE()) == 0)))
    {
        return;
    }
    //
    
    // get next API message to be sent
    uint8 * paucPacket = g_aucTxQueue;	// initially choose the first API message from queue
    uint8 * pQueueMsg = g_aucTxQueue;
    uint8 ucHartLoop = 0;

    // first search for Hart Loop generated packets
    if (!g_pucAgresiv)
    { // normal mode
        for( ;pQueueMsg < g_aucTxQueue + g_unTxQueueSize;  pQueueMsg += pQueueMsg[0] + 1 )
        {
            if( ((API_MSG_HEADER*)(pQueueMsg+1))->MsgHeader.m_bHartLoop )
            { // Hart Loop generated messages have highest priority
                paucPacket = pQueueMsg;

                // there is a HART Loop message for Radio Module
                g_ucNumberOfAgresivRetries = 5; // ... entering agressive mode
                g_pucAgresiv = paucPacket;      // ... storing the pointer to the packet that has to be transmitted.
                ucHartLoop = 1;
                break;
            }
         }
     }
    else
    { // agresiv mode ... retry mode ... first mesage did not worked
        paucPacket = (uint8 *) g_pucAgresiv;
        ucHartLoop = 1;
    }

    // second search for response packets
    if (ucHartLoop==0)
    {
        pQueueMsg = g_aucTxQueue;
        for( ;pQueueMsg < g_aucTxQueue + g_unTxQueueSize;  pQueueMsg += pQueueMsg[0] + 1 )
        {
            if( ((API_MSG_HEADER*)(pQueueMsg+1))->MsgHeader.m_bIsRsp )
            { // response type messages have priority; try to send them first
                paucPacket = pQueueMsg;
                break;
            }
        }
    }

    uint8 ucPkLen = paucPacket[0];
    
    // check if message size is correct; if too short, consider that the tx queue is corrupted and reset the queue
    if( ucPkLen < sizeof(API_MSG_HEADER) )
    {
        g_unTxQueueSize = 0;
        return;
    }
    
    // if the message is too long, discard it
    if( ucPkLen >= ( sizeof(g_aucApiTxBuf)-API_CRC_SIZE ) )
    {
        Api_Q_RemTxMsg( paucPacket );
        return;
    }

    if (p_ucSource == 0) {
        if (
                (((API_MSG_HEADER*) (paucPacket + 1))->MsgHeader.m_eMsgClass == CLS_STACK_SPECIFIC)
                &&
                ((HART_CD_PIN_STATE()) || (g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse) || ((HART_RTS_PIN_STATE()) == 0))

                ) {
            return;
        }
    }
    //
    
    // Read RTC
	uint16 unTimStart = READ_FREE_RUNNING_TIMER();
        
    // activate WKU signal
    //GPIO_Write(g_regWkuPort, g_unWkuPin, GPIO_ON);
    WKU_PIN_SET_HIGH();
        
    memcpy( g_aucApiTxBuf, paucPacket+1, ucPkLen );
    
    while(/*GPIO_State(g_regRdyPort, g_unRdyPin)*/ RDY_PIN_STATE() )
    { // need to wait a minimum of 2 ms for RDY activation
        if ((READ_FREE_RUNNING_TIMER() - unTimStart) > TMR_MS2TICS(2)) // 1 ms ~ 4ticks
        { // no activation of the RDY line during the last ~2 ms inactivate WKU signal
            //GPIO_Write(g_regWkuPort, g_unWkuPin, GPIO_OFF);
            WKU_PIN_SET_LOW();
            return;
        }
    }
    
    if( ((API_MSG_HEADER*)(paucPacket+1))->MsgHeader.m_bIsRsp )
    { // response, remove from queue
        Api_Q_RemTxMsg( paucPacket );
    }
    
    uint16 unCRC = LIB_CalculateCrc(g_aucApiTxBuf, (uint16)ucPkLen);
    g_aucApiTxBuf[ ucPkLen+0 ] = (unCRC >> 8);   // CRC -> HI byte
    g_aucApiTxBuf[ ucPkLen+1 ] = unCRC & 0x00FF; // CRC -> LO byte
    
    g_ucApiTxIdx = 0;
    g_ucApiTxLen = ucPkLen + API_CRC_SIZE;
    
    UART1_StartTx(STX);
    
    // inactivate WKU signal
    //GPIO_Write(g_regWkuPort, g_unWkuPin, GPIO_OFF);
    WKU_PIN_SET_LOW();
}

/**
 * @brief Inserts a new message in the API Message Queue.
 * @param[in] MsgClass - Message Class.
 * @param[in] MsgType - Message Type.
 * @param[in] bIsRsp - 0 = Request, 1 = Response.
 * @param[in] MsgId - Message ID. Used for identification.
 * @param[in] BuffSize - Message Data Length.
 * @param[in] *MsgDataBuff - Pointer to the message data.
 * @param[in] bHartLoop - 0 = Message sent from HARTLoop, 1 = Message Sent from API.
 */
API_ERROR_CODE_TYPES API_SendMessage( MSG_CLASS_TYPES MsgClass,
                                     uint8 MsgType,
                                     uint8 bIsRsp,
                                     uint8 MsgId,
                                     uint8 BuffSize,
                                     uint8 *MsgDataBuff,
                                     uint8 bHartLoop)
{
    uint8 anPacketBuffer[API_MAX_BUFFER_SIZE];
    API_MSG_HEADER *MessageFrame = (API_MSG_HEADER *)anPacketBuffer;
    
    if (BuffSize > API_MAX_BUFFER_SIZE)
    {
        return API_ERR_UNSUPPORTED_DATA_SIZE;
    }
    
    memset(anPacketBuffer, 0, API_MAX_BUFFER_SIZE);  // clear the anPacketBuffer(message frame) array
    
    if (!bIsRsp)
    {
        if ((g_ucMsgID == 0) || (g_ucMsgID == 128))
        {
            g_ucMsgID = 1;
        }
        MsgId = (uint8)(g_ucMsgID & 0x7F); // interval [1, 127] reserved for AP FULL API requests
        g_ucMsgID++;
    }
    
    MessageFrame->MsgHeader.m_eMsgClass = (uint8)MsgClass;
    MessageFrame->MsgHeader.m_bIsRsp = (uint8)bIsRsp;
    MessageFrame->MsgHeader.m_bHartLoop = (uint8)bHartLoop;
    MessageFrame->m_ucMsgType = (uint8)MsgType;
    MessageFrame->m_ucMsgID = (uint8)MsgId;
    MessageFrame->m_ucMsgDataLen = (uint8)BuffSize;
    
    if (MsgDataBuff != NULL)
    {
        memcpy((uint8 *)anPacketBuffer+sizeof(API_MSG_HEADER), (uint8 *)MsgDataBuff, MessageFrame->m_ucMsgDataLen);
    }
    
    return (API_ERROR_CODE_TYPES) Api_Q_AddTxMsg((const API_MSG_HEADER*)MessageFrame, ((uint8*)MessageFrame)+API_MSG_HEADER_SIZE);
}

/**
 * @brief Performs Absolute Slot Number Synchronization with the Radio Module.
 */
void API_Perform250MsecTasks( void )
{
    if( g_ullCrtASN )
    { // perform ASN maintenance only after the ASN is in sync with WISA radio
        g_ullCrtASN += 25;
        g_ulMonotonic += 25; // increment the monotonic every 250 milliseconds
    }
    
    if (g_ulMonotonic >= TICS_24H_ASN)
    {
        g_ulMonotonic = 0; // reset after 1 day
    }
    
    if (g_ucAppProcessorStatus >= API_STATE_NOTIFY_JOIN_OK )
    {
        g_unASNInterogationTimer++; // increment ASN interrogation timer (250 milliseconds units)
        
        if (g_unASNInterogationTimer >= API_PERIOD_ASN_REQ) // recurrence is 300 seconds = 5 minutes
        {
            // call your 300s function here
            // Send HART_GET_ASN (0x01) request
            API_SendRequest(CLS_STACK_SPECIFIC, HART_GET_ASN, 0, 0, NULL, 0);
            g_unASNInterogationTimer = 0;
        }
    }
}

/**
 * @brief Parses an incoming API Specific Messages and calls a specific function based on the message type.
 * @param[in] *p_pMsgFrame - Pointer to the message frame. Used for determining the message type.
 */
uint8 API_OnRcvAPISpecific( API_MSG_HEADER *p_pMsgFrame )
{
    switch (p_pMsgFrame->m_ucMsgType)
    {
    case API_CMD_HW_PLATFORM:
        break;
    case API_CMD_PROTOCOL_VERSION:
        break;
    case API_CMD_MAX_BUFFER_SIZE:
        break;
    case API_CMD_MAX_SPI_SPEED:
        API_SendResponse(CLS_ACK, ACK_DATA_OK, 0, 0, NULL, 0);
        break;
    case API_CMD_UPDATE_SPI_SPEED:
        break;
    case API_CMD_MAX_UART_SPEED:
        break;
    case API_CMD_UPDATE_UART_SPEED:
        break;
    case API_CMD_HEARTBEAT_FREQ:
        break;
    case API_CMD_HEARTBEAT:
        break;
    case API_CMD_ENERGY_MODEM_REQ:
    {
        uint8 *pRcvDataBuff = (uint8 *)(p_pMsgFrame + 1);
        uint8 ucRst = 0;

        Ram2Ram(pRcvDataBuff, (unsigned char *) g_stProv.m_stRadioCharge.charge.cvalue, 4); // read the charge ... accumulated charge.
        ucRst = pRcvDataBuff[4];
        if (!ucRst)
        { // was not after a reset battery command
            FLASH_UPDATE_NEEDED_WITH_STAT();
            CalculateLifeExpectancy();
        }
    }
      break;
    case API_RF_TEST_MODE:
        API_SendResponse(CLS_ACK, ACK_DATA_OK, 0, 0, NULL, 0); // just ACK ... not using for the moment WISA statistics on packets ... like in ISA100 implementation
        break;
    }
    return 1;
}

/**
 * @brief Parses an incoming Board Command Specific Messages and calls a specific function based on the message type.
 * @param[in] *p_pMsgFrame - Pointer to the message frame. Used for determining the message type.
 */
uint8 API_OnRcvBoardCommand( API_MSG_HEADER *p_pMsgFrame )
{
    uint8 ucRspCode;
    switch (p_pMsgFrame->m_ucMsgType)
    {
    case DAQ_FW_WIRELESS:
        // call the function that parses the HART request, executes the HART cmd and composes the HART response
        ucRspCode = App_RcvTunnelledCmd(p_pMsgFrame->m_ucMsgType, p_pMsgFrame->m_ucMsgID, (uint8 *)(p_pMsgFrame + 1));
        if (ucRspCode == RCS_N254_ResponseAck)
        {
            API_SendResponse(CLS_ACK, ACK_DATA_OK, p_pMsgFrame->m_ucMsgID, 0, NULL, 0);
        }
        else
        {
            if (ucRspCode == RCS_N255_ResponseNack)
            {
                API_SendResponse(CLS_NACK, NACK_API_COMMAND_ERROR, p_pMsgFrame->m_ucMsgID, 0, NULL, 0);
            }
        }
        break;
    case DAQ_FW_HART_MODEM:
        AppHART_Tx_Response((uint8 *)(p_pMsgFrame + 1), p_pMsgFrame->m_ucMsgDataLen);
        break;
        
    case DAQ_FW_WIRED: // we do not use this kind of a command
        API_SendResponse(CLS_NACK, NACK_UNSUPPORTED_FEATURE, p_pMsgFrame->m_ucMsgID, 0, NULL, 0);
        break;
        
    default:
        API_SendResponse(CLS_NACK, NACK_UNSUPPORTED_FEATURE, p_pMsgFrame->m_ucMsgID, 0, NULL, 0);
        break;
    }
    
    return 1;
}


/**
 * @brief Parses an incoming Stack Specific Messages and calls a specific function based on the message type.
 * @param[in] *p_pMsgFrame - Pointer to the message frame. Used for determining the message type.
 */
uint8 API_OnRcvStackSpecific( API_MSG_HEADER *p_pMsgFrame )
{
    uint8 *pRcvDataBuff = (uint8 *)(p_pMsgFrame + 1);   // skip the header
    uint8 aucResponseDataBuffer[16];                     // array for response data buffer
    uint16 unResponseNetworkStatus;                     // variable to store the value of join status code
    uint64 ullResponseASN;
    
    switch (p_pMsgFrame->m_ucMsgType)
    {
    case HART_GET_INITIAL_INFO:
        // if (g_ucAppProcessorStatus > API_INIT)
        // { // This is not the first HART_GET_INITIAL_INFO request, perform SW reset
        //     ; // reset here & nothing else matters
        // }
        aucResponseDataBuffer[0] = 3; // number of burst channels
        aucResponseDataBuffer[1] = 1; // number of event channels
        aucResponseDataBuffer[2] = (uint8)(PROV_unExpandedDeviceType >> 8);
        aucResponseDataBuffer[3] = (uint8)(PROV_unExpandedDeviceType);
        aucResponseDataBuffer[4] = (uint8)(PROV_unManufacturerIDCode >> 8);
        aucResponseDataBuffer[5] = (uint8)(PROV_unManufacturerIDCode);
        aucResponseDataBuffer[6] = (uint8)(PROV_umPrivateLabelDistributorCode >> 8);
        aucResponseDataBuffer[7] = (uint8)(PROV_umPrivateLabelDistributorCode);
        aucResponseDataBuffer[8] = (uint8)(g_stProv.m_sWiredHart.m_Device.m_PollingAddr.m_AddrBits);
        aucResponseDataBuffer[9] = (uint8)(PROV_ucHardwareRevisionLevel);
        aucResponseDataBuffer[10] = (uint8)(g_stProv.m_sWiredHart.m_Device.m_ucPreambleLength);
        aucResponseDataBuffer[11] = (uint8)(PROV_ucMaxNoOfDeviceVars);
        aucResponseDataBuffer[12] = (uint8)(PROV_ucDeviceProfile);
        aucResponseDataBuffer[13] = (uint8)(PROV_ucPowerSource);
        aucResponseDataBuffer[14] = (uint8)(g_stProv.m_sWiredHart.m_Device.m_ucLoopConfig);
        aucResponseDataBuffer[15] = (uint8)(PROV_ucDeviceRevisionLevel);
        
        // Send HART_GET_INITIAL_INFO response
        API_SendResponse(CLS_STACK_SPECIFIC, HART_GET_INITIAL_INFO, p_pMsgFrame->m_ucMsgID, 16, aucResponseDataBuffer, 0); // all bytes in network order
        
        g_ucAppProcessorStatus = API_STATE_GET_INITIAL_INFO_OK;
        break;
        
    case HART_NOTIFY_JOIN:
        
        // Inform the AP it has successfully joined the network - Join/Disjoin notification
        API_SendResponse(CLS_ACK, ACK_DATA_OK, p_pMsgFrame->m_ucMsgID, 0, NULL, 0);
        API_SendRequest(CLS_STACK_SPECIFIC, HART_GET_ASN, 0,0, NULL, 0); // send the first Get ASN request
        g_ucAppProcessorStatus = API_STATE_NOTIFY_JOIN_OK;
        break;
        
    case HART_SERVICE_REQUEST:
        App_GetSrvcRsp(pRcvDataBuff, p_pMsgFrame->m_ucMsgDataLen);
        break;
        
    case HART_FLUSH_BURST:
        App_FlushBurstMsg(pRcvDataBuff, p_pMsgFrame->m_ucMsgDataLen);
        API_SendResponse(CLS_ACK, ACK_DATA_OK, p_pMsgFrame->m_ucMsgID, 0, NULL, 0); // send ACK packet
        break;
        
    case HART_RADIO_RESET:
        API_SendResponse(CLS_ACK, ACK_DATA_OK, p_pMsgFrame->m_ucMsgID, 0, NULL, 0);
        // Cmd042
        
        ClrWdt();
        // Read RTC
        {
            uint16 unTimStart = READ_FREE_RUNNING_TIMER();
            while((READ_FREE_RUNNING_TIMER() - unTimStart) <= 2*TMR_MS2TICS(125)) // need to wait 250 ms for response
                ;

            //Software Reset
            asm("RESET");
        }
        break;
        
    case HART_GET_ASN:
        
        g_unResponseTime =  READ_FREE_RUNNING_TIMER();
        g_unDeltaTime = (g_unResponseTime - g_unSTXTimestampMsec) & 0xFFFF ;  // calculate the DeltaTime1
        
        // Store the value of ASN from the radio module's response
        ullResponseASN = ((long long)(pRcvDataBuff[0])) << 32;
        ullResponseASN |= ((long long)(pRcvDataBuff[1])) << 24;
        ullResponseASN |= ((long long)(pRcvDataBuff[2])) << 16;
        ullResponseASN |= ((long long)(pRcvDataBuff[3])) << 8;
        ullResponseASN |= ((long long)(pRcvDataBuff[4]));
        
        // Check if the radio module responded in under 10 msec
        if (g_unDeltaTime < 327 )
        { // The radio module responded in under 10 msec
            // Check if this is the first HART_GET_ASN response after reset/init
            if (g_ullCrtASN == 0)
            { // this is the first HART_GET_ASN response after reset/init
                // consider this poit as monotonic initialization
                g_ulMonotonic = 0;
                // ASN correction initialization
                g_unAsnCorrection = 0;
                // Init g_ullCrtASN with the ASN value from the first HART_GET_ASN response after reset/init
                g_ullCrtASN = ullResponseASN;
            }
            else
            { // This is the HART_GET_ASN response to the HART_GET_ASN (0x01) periodicall request (every 300sec -> API_PERIOD_ASN_REQ)
                // Calculate the time passed since the first response byte received on SPI
                if ( g_unSTXTimestampMsec < g_unResponseTime)
                {
                    g_unDeltaTime2 = g_unResponseTime - g_unSTXTimestampMsec; // calculate the DeltaTime2
                }
                else
                { // overflow
                    g_unDeltaTime2 = 0xFFFF - g_unSTXTimestampMsec + g_unResponseTime; // calculate the DeltaTime2
                }
                
                // Compare local ASN and ASN received from the radio module
                if (g_ullCrtASN >= ullResponseASN)
                {
                    // Needs correction
                    g_unAsnCorrection += g_unDeltaTime2;
                    
                    if (g_unAsnCorrection >= 327 )
                    { // 1 step correction in the past; not allowed to do correction in the future!
                        g_ullCrtASN--;
                        if (g_ulMonotonic > 0)
                        { // robustness; in case monotonic ==0 => protect against underflow!
                            g_ulMonotonic--;
                        }
                        g_unAsnCorrection -= 327;
                    }
                }
            }
        }
        else
        { // >= 10 msec request the ASN Absolute Slot Number from the WISA again
            g_unASNInterogationTimer = API_PERIOD_ASN_REQ-2; // postpone the query over 500 ms
        }
        
        break;
        //-------------------------------------------
        
    case HART_CLEAR_MSA:
        g_ucDeviceStatus[MASTER_WISA] &= ~FieldDeviceStatusesMask_MoreStatusAvailable; // clear MSA
        g_ucDeviceStatus[MASTER_SECONDARY] &= ~FieldDeviceStatusesMask_MoreStatusAvailable; // clear MSA
        g_ucDeviceStatus[MASTER_PRIMARY] &= ~FieldDeviceStatusesMask_MoreStatusAvailable; // clear MSA
        API_SendResponse(CLS_ACK, ACK_DATA_OK, p_pMsgFrame->m_ucMsgID, 0, NULL, 0); // send ACK packet
        break;
        
    case HART_UPDATE_BURST_PERIOD:
        App_UpdateBurstPeriod(pRcvDataBuff);
        API_SendResponse(CLS_ACK, ACK_DATA_OK, p_pMsgFrame->m_ucMsgID, 0, NULL, 0); // send ACK packet
        break;
        
    case HART_GET_JOIN_STATUS:
        
        unResponseNetworkStatus = ((pRcvDataBuff[0] << 8) | (pRcvDataBuff[1]));
        if (unResponseNetworkStatus & ADVERTISEMENT_HEARD)
        {
            g_ucNetworkStatus = NWK_STATUS_DISCOVERY | NWK_STATUS_RDY_TO_COMM;
        }
        if ((unResponseNetworkStatus & JOIN_REQUESTED ) || (unResponseNetworkStatus & JOIN_RETRYING ) || (unResponseNetworkStatus & AUTHENTICATED ) || (unResponseNetworkStatus & NETWORK_JOINED ))
        {
            g_ucNetworkStatus = NWK_STATUS_JOINING | NWK_STATUS_RDY_TO_COMM;
        }
        if (unResponseNetworkStatus & NEGOTIATING_NETWORK_PROPERTIES )
        {
            g_ucNetworkStatus = NWK_STATUS_OPERATIONAL | NWK_STATUS_RDY_TO_COMM;
        }
        break;
        
    case HART_GET_CFG_CHG_COUNTER:
        App_GetChangeCounter(pRcvDataBuff);
        break;
        
    case HART_ASYNC_OPERATION:
        App_AsyncOperation(pRcvDataBuff);
        API_SendResponse(CLS_ACK, ACK_DATA_OK, p_pMsgFrame->m_ucMsgID, 0, NULL, 0); // send ACK packet
        break;

    case HART_BLOCK_TRANSFER:
    default:
        API_SendResponse(CLS_NACK, NACK_UNSUPPORTED_FEATURE, p_pMsgFrame->m_ucMsgID, 0, NULL, 0);
        break;
    }
    return 1;
}

