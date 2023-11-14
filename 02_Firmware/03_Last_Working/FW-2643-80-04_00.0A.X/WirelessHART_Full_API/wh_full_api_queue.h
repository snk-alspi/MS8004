#ifndef __WH_FULL_API_QUEUE_H
#define __WH_FULL_API_QUEUE_H

#include "wh_full_api.h"

///@{
/** API Communication Receive Queue State */
typedef enum
{
    API_Q_RX_IDLE = 1, /*!< 1 */
    API_Q_RX_WAIT_WAKEUP = 2, /*!< 2 */
    API_Q_RX_READY = 3, /*!< 3 */
    API_Q_RX_IN_PROGRESS = 4, /*!< 4 */
    API_Q_RX_COMPLETE = 5, /*!< 5 */
} __attribute__((packed))  API_Q_RECEIVE_STATE_T;
///@}

extern uint8 g_aucApiRxBuf[API_MAX_BUFFER_SIZE]; ///< API Communication Receive buffer
extern uint8 g_ucApiRxIdx; ///< API Communication Receive Buffer Current index
extern uint8 g_ucApiRxState; ///< API Receive State

extern uint8 g_aucApiTxBuf[160 + API_CRC_SIZE]; ///< API Communication Transmit buffer
extern volatile uint16 g_ucApiTxIdx; ///< API Communication Transmit Buffer Current index
extern volatile uint16 g_ucApiTxLen; ///< API Communication Transmit length
extern uint8 g_aucTxQueue[API_MAX_TX_QUEUE_SIZE]; ///< API Communication Transmit Queue
extern uint16 g_unTxQueueSize; ///< API Communication Transmit Queue size.

extern volatile unsigned char * g_pucAgresiv; ///< WiredHART agressive retry when a response is not acknowledged
extern volatile unsigned char g_ucNumberOfAgresivRetries; ///< Number of wiredHART communication retries
extern volatile unsigned char g_ucAgresivTimeoutInProgress; ///< Flag indicating that agressive retries ar in progress
extern uint16 g_unAgresivTimeoutTmr;


void Api_Q_Init();
uint8 API_Q_GetRxMsg( unsigned char * p_pMsg, unsigned char p_ucMsgMaxSize);
uint8 Api_Q_AddTxMsg( const API_MSG_HEADER * p_pMsgHdr, const uint8 * p_pMsgPayload );
void Api_Q_RemTxMsg( uint8 * p_pMsg );
void Api_Q_CheckSWWatchdog();


#endif // __WH_FULL_API_QUEUE_H
