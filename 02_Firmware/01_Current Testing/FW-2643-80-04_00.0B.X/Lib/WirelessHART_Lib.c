#include <string.h>
#include "../Platform/timers.h"
#include "../Platform/gpio.h"
#include "../Platform/uart.h"
#include "../WirelessHART_Full_API/wh_full_api.h"
#include "../WiredHart/wired_hart_link_layer.h"
#include "../WiredHart/wired_hart_app_layer.h"
#include "../WirelessHART_Full_API/wh_full_api_queue.h"
#include "../WirelessHART_AppLayer/App_WH_User.h"
#include "../WirelessHART_AppLayer/App_WH_CmdParse.h"
#include "../WirelessHART_AppLayer/App_WH_CmdExecute.h"
#include "../WirelessHART_AppLayer/App_WH_CmdCompose.h"
#include "../Platform/gpio.h"
#include "../Platform/nvm.h"


extern uint8 g_ucNetworkStatus;

volatile uint16 g_unDelayedLowPower;

static uint16 s_unRtsStartTime = 0; ///< Ready To Send start time

/**
 * @brief Table used for CRC computation
 */
static const uint16 CCITTCrcTable[256]={
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};


/**
 * @brief Clculates CRC. Takes two arguments and returns an unsigned integer value
 * @param[in] Buffer
 * @param[in] BufferSize
 */
uint16 LIB_CalculateCrc( const uint8 *Buffer, uint16 BufferSize )
{
uint16 CrcResult;

    for (CrcResult=0xFFFF; BufferSize; BufferSize-- )
    {
        CrcResult = (CrcResult << 8) ^ CCITTCrcTable[CrcResult>>8 ^ *Buffer] ;
        Buffer++;
    }

    return CrcResult;
}


/**
 * @brief Executes the 125 milliseconds periodic task.
 *
 * This function executes the Burst Task. It is responsible for checking if there is any packet
 * in the API TX Queue. If so, the message will be sent to the radio module.
 */
void LIB_PeriodicTask_125msec()
{
    App_EventTask();
    App_BurstTask();
}

/**
 * @brief Executes the 250 milliseconds periodic task.
 *
 * This function executes the Burst Task. It is responsible for checking if there is any packet
 * in the API TX Queue. If so, the message will be sent to the radio module.
 * It is also responsible for checking the API RX Queue state and setting the Ready to Send signal
 * according to RTSCounter and API RX state.
 */
void LIB_PeriodicTask_250msec()
{
    // ASN and monotonic processing
    API_Perform250MsecTasks();
    // Process the burst config (loaded from NVM on device initialization) and send the service requests for the active bursts
    APP_InitialRFBWServiceTask();
    // Save the Device Variables PV, SV, TV information to NV memory (if necesarry).
}


/**
 * @brief Executes the 1 second periodic task.
 *
 * This function executes the Dynamic RF Bandwidth Task.
 * It is also responsible for checking the network join status from the radio module.
 */
void LIB_PeriodicTask_1sec()
{
    App_BurstOneSecTask();
    
    Api_Q_CheckSWWatchdog();
    
    // in case there is a packet in the TX queue, send it
    API_Tx_Handler(0);
    
    APP_DynamicRFBWServiceTask();

    // verify if the radio has joined the network - if no, request the join status from the radio
    if ((g_ucNetworkStatus & NWK_STATUS_OPERATIONAL) != NWK_STATUS_OPERATIONAL)
    {
        API_SendRequest(CLS_STACK_SPECIFIC, HART_GET_JOIN_STATUS, 0, 0, NULL, 0);
    }    
 }

/**
 * @brief Executes the main periodic tasks.
 *
 * This function executes the 25 and 250 milliseconds tasks, as well as the 1 second task.
 * It also checks for timeouts on the API Communication Interface.
 */
void LIB_MainTask()
{
    if ( (g_pucAgresiv) && (g_ucAgresivTimeoutInProgress) )
    {
        if (g_ucNumberOfAgresivRetries > 0)
        {
            while( (uint16)(READ_FREE_RUNNING_TIMER() - g_unAgresivTimeoutTmr) < TMR_MS2TICS(3) )
                  ;
            
            g_ucNumberOfAgresivRetries--;
            g_ucAgresivTimeoutInProgress = 0;
            API_Tx_Handler(1);
        }
        else
        { // no more retries left
            Api_Q_RemTxMsg( (uint8 *) g_pucAgresiv );  // remode from queue
            g_pucAgresiv = NULL; // stop agresiv mode
            g_ucNumberOfAgresivRetries = 0; // degeaba ... oricum era
            g_ucAgresivTimeoutInProgress = 0;
        }
    }

    API_Rx_Handler();// ASYNC Task
}
/**
* @brief Go to low power mode
*/
uint16 LIB_GoToLowPower ( void )
{
    // Wait for API Tx to complete // g_unWaitForTxCompleteTmr
    if( !API_Tx_IsEmpty() )
        return 0;
    
    if (g_unDelayedLowPower != 0)
    {
        g_unDelayedLowPower --;
        return 0;
    }

    // Wait for API Rx to complete
    if ( API_Q_RX_IDLE != *(volatile unsigned char*)&g_ucApiRxState )
        return 0;
    
    // If aggressive mechanism is enabled do not allow
    if(g_pucAgresiv)
      return 0;

    
    if (WIRED_HART_TX_IDLE != g_stProv.m_sWiredHart.m_eTxState)
      return 0;
    
    // HART Carrier present
    if(HART_CD_PIN_STATE())  
      return 0;
       
    uint16 unStartSleepTimestamp = READ_FREE_RUNNING_TIMER();

//    LATDbits.LATD5 = 1;
    if (g_unDelayedLowPower == 0)
    {
        Sleep();
    }
    //LATDbits.LATD5 = 0;
    
    return (READ_FREE_RUNNING_TIMER() - unStartSleepTimestamp);
}

void LIB_OnGpioIrq()
{
    g_unDelayedLowPower = 2;
}
/**
 * @brief Initializes the hardware interface for the API communication between Application Processor and radio module.
 * @param[in] *p_pregWKUPort - pointer to WKU pin port assignment
 * @param[in] p_unWKUPin - pointer to WKU pin assignment
 * @param[in] *p_pregRDYPort - pointer to RDY pin port assignment
 * @param[in] p_unRDYPin - pointer to RDY pin assignment
 * @param[in] *p_pregCTSPort - pointer to CTS pin port assignment
 * @param[in] p_unCTSPin - pointer to CTS pin assignment
 * @param[in] *p_pregRTSPort - pointer to RTS pin port assignment
 * @param[in] p_unRTSPin - pointer to RTS pin assignment
 */
LIB_RET_CODE LIB_RadioAPI_Init_HW_Interface()
{
    LIB_RET_CODE rc = RC_SUCCESS;

    /* initialize interface */
    UART1_Init(UART_BAUD_38400);
    UART1_Enable_RxInterrupt();

    /* Initialize global variables */
    Api_Q_Init();

    return rc;
}

/**
 * @brief This function checks if there is no message in the API RX Queue. If so, Application Processor will signal to the Radio Module
 * that it is ready to receive a new message.
 */
void LIB_RadioAPI_PreparePacketRx(void)
{
    if( API_Q_RX_COMPLETE > g_ucApiRxState ) // no API RX message on processing(eg. cover also the case when an incomplete message was received)
    {    
        g_ucApiRxState = API_Q_RX_READY;        
        s_unRtsStartTime = READ_FREE_RUNNING_TIMER();        
        EXT_RTS_PIN_SET_LOW();// signal to VN310 radio that we are ready to receive the full API message activate RTS signal
    }
}

/**
 * @brief This function checks no message received after RTS signal.
 */
void LIB_RadioAPI_CkRtsTimeout(void)
{
    if(!EXT_RTS_PIN_STATE() && (READ_FREE_RUNNING_TIMER() - s_unRtsStartTime) > TMR_MS2TICS(100))
    {
        EXT_RTS_PIN_SET_HIGH();
        g_ucApiRxState = API_Q_RX_IDLE;
    }
}


/**
 * @brief This function is called whenever a byte of data was received on the API interface.
 */
void LIB_RadioAPI_UART_ReceiveByte( uint8 p_ucRxByte )
{
  static unsigned char s_ucPrevRxChar;
  static uint8 s_ucApiRxCounter;
  

  if( p_ucRxByte == STX )
  {
    g_unSTXTimestampMsec = READ_FREE_RUNNING_TIMER();

    if( API_Q_RX_COMPLETE > g_ucApiRxState)
    {
      g_ucApiRxIdx = 0;
      s_ucApiRxCounter = 0;
      g_ucApiRxState = API_Q_RX_IN_PROGRESS;
    }
  }
  else if( API_Q_RX_IN_PROGRESS == g_ucApiRxState )
  {
    s_ucApiRxCounter++;
    if( g_ucApiRxIdx >= sizeof(g_aucApiRxBuf) )
    {
      g_ucApiRxState = API_Q_RX_IDLE;
    }
    else if( p_ucRxByte != CHX )
    {
      g_aucApiRxBuf[g_ucApiRxIdx++] = ( s_ucPrevRxChar != CHX ? p_ucRxByte : ~p_ucRxByte );

      if( (g_ucApiRxIdx == ((unsigned int)(g_aucApiRxBuf[3]) + 6) ) ) // end of packet
      {
        g_ucApiRxState = API_Q_RX_COMPLETE;
        if (s_ucApiRxCounter >= 8)
        {
            // unElapsedTime = g_ucApiRxIdx * 260.4167 = g_ucApiRxIdx * 260.5
            uint16 unElapsedTime = s_ucApiRxCounter;
            
            unElapsedTime *= 260;
            unElapsedTime += s_ucApiRxCounter/2;
            
            LPTM_Adjust(unElapsedTime, READ_FREE_RUNNING_TIMER() - g_unSTXTimestampMsec);
        }
      }
    }

    /*inactivate RTS signal*/
    //GPIO_Write(g_regRtsPort, g_unRtsPin, GPIO_ON);
    EXT_RTS_PIN_SET_HIGH();
  }

  s_ucPrevRxChar = p_ucRxByte;
}

/**
 * @brief This function sends a byte of data on the API interface.
 */
void LIB_RadioAPI_UART_SendByte(void)
{
    uint8 ucTxChar;
    uint16 g_ucTxBufIdxTmp = g_ucApiTxIdx;

    if( g_ucTxBufIdxTmp < g_ucApiTxLen )
    {
        ucTxChar = g_aucApiTxBuf[g_ucTxBufIdxTmp];

        if ( ucTxChar == STX || ucTxChar == CHX )
        {
            g_aucApiTxBuf[g_ucTxBufIdxTmp] = ~ucTxChar;
            ucTxChar = CHX;
        }
        else
        {
            g_ucApiTxIdx++;
        }

        U1TXREG = ucTxChar;
    }
    else
    {
        g_ucApiTxLen = 0;
        
        if (((API_MSG_HEADER*)g_aucApiTxBuf)->MsgHeader.m_bIsRsp == 0)
        {
            g_unDelayedLowPower = 2;
        }

        /*disable TX interrupt*/
        UART1_Disable_TxInterrupt();
        
        if (g_pucAgresiv)
        {
            g_ucAgresivTimeoutInProgress = 1;
            g_unAgresivTimeoutTmr = READ_FREE_RUNNING_TIMER();
        }
        
        // If in API_TX
        // having a message on the hart loop... start a 1ms timeout ... look for RTS/CTS from WISA
        // if no response, call api_tx(1) ... 5 times.
    }
}

void API_Read_Cmd_0_RxHandler( void )
{
uint8 aucMsgBuf[API_MAX_BUFFER_SIZE];
API_MSG_HEADER * pstMsgHdr = (API_MSG_HEADER *)aucMsgBuf;

    uint8 ucPacketSize = API_Q_GetRxMsg( aucMsgBuf, sizeof(aucMsgBuf) );

    if( ucPacketSize == ( pstMsgHdr->m_ucMsgDataLen + sizeof(API_MSG_HEADER)) )
    {
        if ( pstMsgHdr->MsgHeader.m_eMsgClass == CLS_USER_BOARD )
        {
            if ( pstMsgHdr->m_ucMsgType == DAQ_FW_HART_MODEM  )
            {
                AppHART_Cmd_0_Response((uint8 *)(pstMsgHdr + 1), pstMsgHdr->m_ucMsgDataLen);
            }
        }
    }
}

/**
 * @brief This function reads the Command 0 Response from the Radio Module.
 */
void API_Read_Cmd_0(void)
{
    uint16 unStartTime = READ_FREE_RUNNING_TIMER();
    uint16 unCounter = 0;
    
    //Read Command 0 - Read Unique Identifier
    while (!*(volatile uint8_t*)&g_stProv.m_sWiredHart.m_ucInit) 
    {
        ClrWdt();
        API_Read_Cmd_0_RxHandler();

        uint16 unNow = READ_FREE_RUNNING_TIMER();
        if( (uint16)(unNow - unStartTime) >= 4*TMR_MS2TICS(125) ) // every 500 ms
        {
            unStartTime = unNow;
            if ( !g_unTxQueueSize ) // nothing to TX
            {
                AppHART_Cmd_0_Request();
            }
            API_Tx_Handler(0);
            
            if ((++unCounter) & 1) // every second
            {
                Api_Q_CheckSWWatchdog();
            }
        }
    }
}

