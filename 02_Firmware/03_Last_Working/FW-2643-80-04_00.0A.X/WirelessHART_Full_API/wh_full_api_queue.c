#include <string.h>
#include "../Global/typedef.h"
#include "../Lib/WirelessHART_Lib.h"
#include "wh_full_api_queue.h"
#include "wh_full_api.h"
#include "../Platform/gpio.h"
#include "../Platform/timers.h"


uint8 g_aucApiRxBuf[API_MAX_BUFFER_SIZE];
uint8 g_ucApiRxIdx;
uint8 g_ucApiRxState;

//#pragma data_alignment = 4
uint8 g_aucApiTxBuf[160 + API_CRC_SIZE];
volatile uint16 g_ucApiTxIdx;
volatile uint16 g_ucApiTxLen;
uint8 g_aucTxQueue[API_MAX_TX_QUEUE_SIZE];
uint16 g_unTxQueueSize;

volatile unsigned char * g_pucAgresiv = NULL;
volatile unsigned char g_ucNumberOfAgresivRetries = 0;
volatile unsigned char g_ucAgresivTimeoutInProgress = 0;

uint16 g_unAgresivTimeoutTmr;

static uint16 g_unAPIWatchdog = 0;      ///< Software watchdog; resets the AP if no valid messager reveiced from WISA radio within a specified time
/**
 * @brief Initializes API Communication Queue.
 */
void Api_Q_Init()
{
    // Rx
    g_ucApiRxState = API_Q_RX_IDLE;
    // Tx
    g_ucApiTxIdx    = 0;
    g_ucApiTxLen    = 0;
    g_unTxQueueSize = 0;
    // Timeouts
    g_unAPIWatchdog = 0;
    g_pucAgresiv = NULL;
    g_ucNumberOfAgresivRetries = 0;
    g_ucAgresivTimeoutInProgress = 0;
}



/**
 * @brief Adds a message which need to be send from Application Processor to the Radio Module in the API TX Queue.
 * @param[in] *p_pMsgHdr - pointer to the current message header.
 * @param[in] *p_pMsgPayload - pointer to the current message payload.
 */
uint8 Api_Q_AddTxMsg( const API_MSG_HEADER * p_pMsgHdr, const uint8 * p_pMsgPayload )
{
uint8 ucReturnCode = API_QUEUE_ADD_FAIL;
uint8 ucMsgSize = sizeof(API_MSG_HEADER) + p_pMsgHdr->m_ucMsgDataLen;

    if( (g_unTxQueueSize + ucMsgSize + 1) < sizeof(g_aucTxQueue) )
    { // there is space in the queue for this message
        uint8 * pTxQueue = g_aucTxQueue + g_unTxQueueSize;

        // Len
        *pTxQueue = ucMsgSize;
        pTxQueue++;
        // Hdr
        memcpy( pTxQueue, p_pMsgHdr, sizeof(API_MSG_HEADER) );
        pTxQueue += sizeof(API_MSG_HEADER);
        // Data
        memcpy( pTxQueue, p_pMsgPayload, p_pMsgHdr->m_ucMsgDataLen );

        g_unTxQueueSize += ucMsgSize+1;
        ucReturnCode = API_QUEUE_ADD_SUCCESS;
    }

    return ucReturnCode;
}


/**
 * @brief Removes a message from the API TX Queue.
 * @param[in] *p_pMsg - pointer to the desired message.
 */
void Api_Q_RemTxMsg( uint8 * p_pMsg )
{
uint16 unPkLen = 1 + p_pMsg[0];

    if( p_pMsg+unPkLen <= g_aucTxQueue+g_unTxQueueSize )
    {
        g_unTxQueueSize -= unPkLen;
        memmove( p_pMsg,
                 p_pMsg+unPkLen,
                 g_aucTxQueue+g_unTxQueueSize-p_pMsg );  // remove from queue
    }
    else
    { // corrupted queue
        g_unTxQueueSize = 0;
    }
}



/**
 * @brief Fetches a response message from the API Communication RX Queue.
 * @param[in] *p_pMsg - pointer to the received message.
 * @param[in] p_ucMsgMaxSize - maximum message size.
 */
uint8 API_Q_GetRxMsg( unsigned char * p_pMsg, unsigned char p_ucMsgMaxSize)
{
    uint8 ucResultSize = 0;
    if( API_Q_RX_COMPLETE == g_ucApiRxState )
    {
        uint8 ucPacketSize = g_ucApiRxIdx;
        if( ucPacketSize > 2 ) // valid size
        {            
            if( 0 == LIB_CalculateCrc( g_aucApiRxBuf, ucPacketSize ) )
            {
                ucPacketSize -= 2; // CRC[2]
                if (ucPacketSize < p_ucMsgMaxSize)
                {
                  unsigned char * pucPacket = NULL;

                    if (g_pucAgresiv)
                    {
                        pucPacket = (unsigned char *) g_pucAgresiv;
                    }
                    else
                    {
                        pucPacket = g_aucTxQueue;
                    }
                    if(    ((API_MSG_HEADER*)g_aucApiRxBuf)->MsgHeader.m_bIsRsp
                       && ((API_MSG_HEADER*)g_aucApiRxBuf)->m_ucMsgID == ((API_MSG_HEADER*)(pucPacket+1))->m_ucMsgID
                         && !((API_MSG_HEADER*)(pucPacket+1))->MsgHeader.m_bIsRsp   //it is waited response                           
                           )
                    {
                        // if it is response & it is waited response => remove the request from queue,
                        // but save the MsgType before deleting it (will be needed for certain ACKs)
                        // Save the MsgType in the MsgID field (MsgId won't be needed anymore)
                        if( CLS_ACK == ((API_MSG_HEADER*)g_aucApiRxBuf)->MsgHeader.m_eMsgClass )
                        {
                            ((API_MSG_HEADER*)g_aucApiRxBuf)->m_ucMsgID = ((API_MSG_HEADER*)(pucPacket+1))->m_ucMsgType;
                        }

                        // now remove the request message from the transmission queue
                        Api_Q_RemTxMsg( pucPacket );
                        
                        // if in aggressive mode, when a message is deleted from the queue, the agressive mechanism will be disabled by clearing its flags
                        // a response arrived so agressive mechanism is not needed anymore
                        
                        if (g_pucAgresiv)
                        {
                            g_pucAgresiv = NULL;
                            g_ucNumberOfAgresivRetries = 0;
                            g_ucAgresivTimeoutInProgress = 0;
                        }
                    }

                    memcpy(p_pMsg, g_aucApiRxBuf, ucPacketSize);
                    ucResultSize = ucPacketSize;
                }

                // reset API watchdog
                g_unAPIWatchdog = 0;
            }
        }

        //TO DO - for a UART Speeding Up if CTS is active -> activate RTS and don't go to sleep
        g_ucApiRxIdx = 0x00;
        g_ucApiRxState = API_Q_RX_IDLE;
    }
    else if (API_Q_RX_READY == g_ucApiRxState) 
    {
        LIB_RadioAPI_CkRtsTimeout();
    }
    return ucResultSize;
}

/**
 * @brief Software watchdog implementation for the API Communication Queue.
 */
void Api_Q_CheckSWWatchdog()
{
    if( (++g_unAPIWatchdog) > FULL_API_COMM_WDT_TIMEOUT_SEC )
    {
        while( 1 )
            ;
    }
}

