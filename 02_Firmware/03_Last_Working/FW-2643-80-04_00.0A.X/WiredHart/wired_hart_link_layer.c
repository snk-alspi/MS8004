#include <string.h>


#include "../Platform/uart.h"
#include "../Platform/timers.h"
#include "../Platform/gpio.h"
#include "../Platform/nvm.h"
#include "../WirelessHART_Full_API/wh_full_api.h"
#include "wired_hart_link_layer.h"
#include "wired_hart_app_layer.h"
#include "../WirelessHART_AppLayer/App_WH_User.h"
#include "../WirelessHART_AppLayer/App_WH_CmdParse.h"
#include "../WirelessHART_AppLayer/App_WH_CmdExecute.h"
#include "../WirelessHART_AppLayer/App_WH_CmdCompose.h"


/*******************************************************************************
Loop HART physical medium is half duplex => tx and rx are never simultaneous;
     Rx has priority => once in rx, it will remain there
     Tx will be delayed until getting the token
******************************************************************************/

//volatile T_WIRED_HART_GLOBALS g_sWiredHart;

/* Wired HART receive control variables */
volatile uint8   g_aucHartRxBuf[HART_BUFFER_MAX_LEN]; ///< WiredHART RX data buffer.
volatile uint16  g_unHartRxIdx; ///< RX data buffer index.
volatile uint16  g_unNextFieldIdx; ///< RX data buffer next field index.
volatile uint16  g_unLastByteRTCTicks; ///< Last received byte time stamp used for checking transmission gap error.

/* Wired HART transmit control variables */
volatile uint8   g_aucHartTxBuf[HART_BUFFER_MAX_LEN]; ///< WiredHART TX data buffer.
const uint8 * g_pucTxBuffer = NULL; ///< Pointer to TX data buffer.
volatile uint16  g_ucHartTxIdx; ///< TX data buffer index.
volatile uint16  g_ucHartTxLen; ///< TX data length.

void set_HART_in_RX_Wait( void );

/**
 * @brief Initializes the wiredHART Interface.
 */
void Wired_HART_Init (void)
{
    // Token
    g_stProv.m_sWiredHart.m_Token.m_OwnTokenFlag = FALSE;
    // Rx State
    g_stProv.m_sWiredHart.m_RxMsgReceived = 0;                   // already set by memset above only for easy read
    g_stProv.m_sWiredHart.m_RxMsgErrors = 0;                     // already set by memset above only for easy read
    g_stProv.m_sWiredHart.m_RxDelimiter = 0;                     // already set by memset above only for easy read
    g_stProv.m_sWiredHart.m_RxMasterBurst = 0;                   // already set by memset above only for easy read
    g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_WAIT;       // already set by memset above only for easy read
    // Tx State
    g_ucHartTxLen = 0;                                  // already set by memset above only for easy read
    g_stProv.m_sWiredHart.m_eTxState = WIRED_HART_TX_IDLE;       // already set by memset above only for easy read

    g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse = 0;          // already set by memset above only for easy read

    // HART UART
    UART3_Enable_TxInterrupt();
    UART3_Init(UART_BAUD_1200);
    UART3_Enable_RxInterrupt();
    
}


/**
 * @brief Runs the wiredHART Task.
 */
void Wired_HART_Task (void)
{
    // ... monitor Carrier Detect; ... is this necessary

    // received messages
    if (g_stProv.m_sWiredHart.m_RxMsgReceived)
    {
        // Process rx msg
        if ( AppHART_Rx_Indicate( (const uint8 *) g_aucHartRxBuf, g_unHartRxIdx) )
        {
            g_stProv.m_sWiredHart.m_Token.m_OwnTokenFlag = FALSE; // rx message will not be processed and not be a response
            g_stProv.m_sWiredHart.m_eTxState = WIRED_HART_TX_IDLE;
            set_HART_in_RX_Wait();
        }
        else
        {
            g_stProv.m_sWiredHart.m_RxMsgReceived = 0;
            g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_WAIT;
        }
    }

    // manage token
    if ((g_stProv.m_sWiredHart.m_eTxState < WIRED_HART_TX_PREAMBLE) && (g_stProv.m_sWiredHart.m_Token.m_OwnTokenFlag))
    {
        if ( (READ_FREE_RUNNING_TIMER() - g_stProv.m_sWiredHart.m_Token.m_OwnTokenTime) > (WIRED_HART_STO) )
        {
            // switch from tx to rx
          HART_RTS_PIN_SET_HIGH();
        
          // release token
          g_stProv.m_sWiredHart.m_Token.m_OwnTokenFlag = FALSE;
          //
          g_stProv.m_sWiredHart.m_eTxState = WIRED_HART_TX_IDLE;
        }
    }

    // tx last byte
    if (g_stProv.m_sWiredHart.m_unWaitTxEnd)
    { // wait for last byte to be sent
        if ( (READ_FREE_RUNNING_TIMER() - g_stProv.m_sWiredHart.m_unTxTimeStamp) > (WIRED_HART_CHARACTER_TIME) )
        {
            // switch from tx to rx
            HART_RTS_PIN_SET_HIGH(); 
            // release token
            g_stProv.m_sWiredHart.m_Token.m_OwnTokenFlag = FALSE;
            //
            g_stProv.m_sWiredHart.m_eTxState = WIRED_HART_TX_IDLE;
            //
            g_stProv.m_sWiredHart.m_unWaitTxEnd = 0;
            // tx done
            // tx confirm
        }
    }

    if (g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse)
    {
        if ( (READ_FREE_RUNNING_TIMER() - g_stProv.m_sWiredHart.m_sNeedHART.m_unTime) > WIRED_HART_STO )
        {
            g_stProv.m_sWiredHart.m_sNeedHART.m_ucResponse = 0;
        }
    }
}


/**
 * @brief Checks if there is any gap error in the wiredHART Transmission.
 */
uint8 checkGapError( uint16 p_unRTCTime )
{ 
uint8 ucGapError = 0;
    if ( ( p_unRTCTime - g_unLastByteRTCTicks ) > (WIRED_HART_GAP_TIMEOUT + WIRED_HART_CHARACTER_TIME) )
    { // gap error
        ucGapError = 1; // detected a gap error
    }
    g_unLastByteRTCTicks = p_unRTCTime;
    return ucGapError;
}


/**
 * @brief Checks the received message's index. Decides if there is any space to accommodate another message in the RX queue.
 */
uint8 checkRxIdx( void )
{
    if ( g_unHartRxIdx < HART_BUFFER_MAX_LEN )
    { // idx is ok
        return 0;
    }
    else
    { // idx is not ok abort message not enough buffer
        return 1;
    }
}

/**
 * @brief Checks for hardware interface errors in the received message.
 * @param[in] p_ucStatus - Bit mask of the USART Interrupt Service Register.
 */
uint8 checkRxErrors(uint8 p_ucStatus) {
    uint8 ucReturn = 0;

    if (p_ucStatus & U3STAbits.PERR) {
        ucReturn |= WIRED_HART_RX_ERR_VERTICAL_PARITY;
    }
    if (p_ucStatus & U3STAbits.OERR) {
        ucReturn |= WIRED_HART_RX_ERR_OVERRUN;
    }
    if (p_ucStatus & U3STAbits.FERR) {
        ucReturn |= WIRED_HART_RX_ERR_FRAMING;
    }
    return ucReturn;
}



/**
 * @brief Sets wiredHART state to RX wait. It is also used when a message needs to be aborted.
 */
void set_HART_in_RX_Wait( void )
{
    g_stProv.m_sWiredHart.m_RxMsgReceived = 0;
    g_stProv.m_sWiredHart.m_RxMsgErrors = 0;
    g_stProv.m_sWiredHart.m_RxDelimiter = 0;
    g_stProv.m_sWiredHart.m_RxMasterBurst = 0;
    g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_WAIT;
}


/**
 * @brief Called from WiredHART UART interrupt.
 * @param[in] p_ucRxByte - incoming data byte
 * @param[in] p_ucStatus - USART ISR bit mask
 * @retval 1 - last packet byte received, 0 - intermediate byte received
 */
uint8 Wired_HART_UART_RxByte( uint8 p_ucRxByte, uint8 p_ucStatus )
{
static volatile uint8 g_ucRxCheckByte = 0;
volatile unsigned char ucStatus = p_ucStatus; // Read Error Status first
volatile unsigned char ucRxChar = p_ucRxByte; // Read byte (this also clear Error Status)

    if( !g_stProv.m_sWiredHart.m_RxMsgReceived ) // not pending RX message ...
    {
        uint16 unRTCTime = READ_FREE_RUNNING_TIMER(); // get time stamp for this byte in g_unRTCTime
        switch( g_stProv.m_sWiredHart.m_eRxState )
        {
        case WIRED_HART_RX_WAIT:
            if ( (!checkRxErrors(ucStatus)) && (ucRxChar==0xFF) )
            {
                g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_PREAMBLE;
                g_unLastByteRTCTicks = unRTCTime; // start dealing with gap timeout
            }
            break;

        case WIRED_HART_RX_PREAMBLE:
            if ( (!checkGapError(unRTCTime)) && (!checkRxErrors(ucStatus)) && (ucRxChar==0xFF) )
            {
                g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_DELIMITER;
            }
            else
            { // gap error, uart error or framing in Preamble field => framing the message must be aborted, no reply shall be generated
                set_HART_in_RX_Wait();
            }
            break;

        case WIRED_HART_RX_DELIMITER:
            if ( (!checkGapError(unRTCTime)) && (!checkRxErrors(ucStatus)) )
            {
                if ( ucRxChar!=0xFF )
                {
                    if (
                            (ucRxChar & 0x60)
                          ||
                            (((ucRxChar & HART_FRAME_TYPE_MASK) != HART_FRAME_STX) &&
                            ((ucRxChar & HART_FRAME_TYPE_MASK) != HART_FRAME_ACK) &&
                            ((ucRxChar & HART_FRAME_TYPE_MASK) != HART_FRAME_BACK))
                        )
                    { // not a valid Delimiter field => framing the message must be aborted, no reply shall be generated
                        set_HART_in_RX_Wait();
                    }
                    else
                    { // valid Delimiter
                        g_stProv.m_sWiredHart.m_RxMsgErrors = 0;
                        g_stProv.m_sWiredHart.m_RxDelimiter = 0;
                        g_stProv.m_sWiredHart.m_RxMasterBurst = 0;
                        g_unHartRxIdx = 0; // start message
                        g_aucHartRxBuf[g_unHartRxIdx++] = ucRxChar; // Delimiter Field
                        g_ucRxCheckByte = ucRxChar;
                        if (ucRxChar & HART_DELIMITER_ADDRESS_MASK) // check address type
                        {
                            g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_UNIQUE_ADDRESS;
                            g_unNextFieldIdx = g_unHartRxIdx + HART_FIELD_UNIQUE_ADDRESS_LEN; // next field idx is Exp or Cmd
                        }
                        else
                        {
                            g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_POLLING_ADDRESS;
                            g_unNextFieldIdx = g_unHartRxIdx + HART_FIELD_POOLING_ADDRESS_LEN; // next field idx is Exp or Cmd
                        }
                    }
                }
                // else ... not an error ... keep listening as many 0xFF will be
            }
            else
            { // gap error, uart error in Delimiter field => framing the message must be aborted, no reply shall be generated
                set_HART_in_RX_Wait();
            }
            break;

        case WIRED_HART_RX_UNIQUE_ADDRESS:
            if ( (!checkGapError(unRTCTime)) && (!checkRxErrors(ucStatus)) )
            {
                g_aucHartRxBuf[g_unHartRxIdx++] = ucRxChar;
                g_ucRxCheckByte ^= ucRxChar;
                if (g_unHartRxIdx == g_unNextFieldIdx)
                {
                    if (g_aucHartRxBuf[0] & HART_DELIMITER_EXPBYTES_MASK) // check Exp Bytes
                    {
                        g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_EXPBYTES;
                        g_unNextFieldIdx = g_unHartRxIdx + ((g_aucHartRxBuf[0] & HART_DELIMITER_EXPBYTES_MASK) >> 5); // next field idx is Cmd
                    }
                    else
                    {
                        g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_COMMAND;
                    }
                }
            }
            else
            { // gap error, uart error in Unique Address field => the message should be treated as if addressed to other device. No reply shall be generated
                set_HART_in_RX_Wait();
            }
            break;

        case WIRED_HART_RX_POLLING_ADDRESS:
            if ( (!checkGapError(unRTCTime)) && (!checkRxErrors(ucStatus)) )
            {
                g_aucHartRxBuf[g_unHartRxIdx++] = ucRxChar;
                g_ucRxCheckByte ^= ucRxChar;
                if (g_unHartRxIdx == g_unNextFieldIdx)
                {
                    if (g_aucHartRxBuf[0] & HART_DELIMITER_EXPBYTES_MASK) // check Exp Bytes
                    {
                        g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_EXPBYTES;
                        g_unNextFieldIdx = g_unHartRxIdx + ((g_aucHartRxBuf[0] & HART_DELIMITER_EXPBYTES_MASK) >> 5); // next field idx is Cmd
                    }
                    else
                    {
                        g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_COMMAND;
                    }
                }
            }
            else
            { // gap error, uart error in Polling Address field => the message should be treated as if addressed to other device. No reply shall be generated
                set_HART_in_RX_Wait();
            }
            break;

        case WIRED_HART_RX_EXPBYTES:
            if ( (!checkGapError(unRTCTime)) && (!checkRxErrors(ucStatus)) )
            {
                g_aucHartRxBuf[g_unHartRxIdx++] = ucRxChar;
                g_ucRxCheckByte ^= ucRxChar;
                if (g_unHartRxIdx==g_unNextFieldIdx)
                {
                    g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_COMMAND;
                }
            }
            else
            { // gap error, uart error in Expansion Bytes fields => field device must not answer
                set_HART_in_RX_Wait();
            }
            break;

        case WIRED_HART_RX_COMMAND:
            if ( !checkGapError(unRTCTime) )
            {
                // uart error in Command Field => device must reply with error code and no data
                g_stProv.m_sWiredHart.m_RxMsgErrors |= checkRxErrors(ucStatus);
                g_aucHartRxBuf[g_unHartRxIdx++] = ucRxChar;
                g_ucRxCheckByte ^= ucRxChar;
                g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_COUNT;
            }
            else
            { // gap error in Command Field => fatal error abort the message ... spec 081 ch. 5.2
                set_HART_in_RX_Wait();
            }
            break;

        case WIRED_HART_RX_COUNT:
            if ( (!checkGapError(unRTCTime)) && (!checkRxErrors(ucStatus)) )
            {
                g_aucHartRxBuf[g_unHartRxIdx++] = ucRxChar;
                g_ucRxCheckByte ^= ucRxChar;
                if (ucRxChar)
                {
                    g_unNextFieldIdx = g_unHartRxIdx + ucRxChar; // next field idx is Check Byte
                    g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_DATA;
                }
                else
                {
                    g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_CHECK;
                }
            }
            else
            { // gap error, uart error in Byte Count field => framing the message must be aborted, no reply shall be generated
                set_HART_in_RX_Wait();
            }
            break;

        case WIRED_HART_RX_DATA:
            if ( !checkGapError(unRTCTime) )
            {
                // uart error in Data Field => the response shall indicate the appropriate error in the communication error status, no data shall be returned
                g_stProv.m_sWiredHart.m_RxMsgErrors |= checkRxErrors(ucStatus);
                if ( checkRxIdx() )
                { // buffer error in Data Field => the response shall indicate the appropriate error in the communication error status, no data shall be returned
                    g_stProv.m_sWiredHart.m_RxMsgErrors |= WIRED_HART_RX_ERR_BUFFER_OVERFLOW;
                }
                g_aucHartRxBuf[g_unHartRxIdx++] = ucRxChar;
                g_ucRxCheckByte ^= ucRxChar;
                if (g_unHartRxIdx == g_unNextFieldIdx)
                {
                    g_stProv.m_sWiredHart.m_eRxState = WIRED_HART_RX_CHECK;
                }
            }
            else
            { // gap error in Command Field => fatal error abort the message ... spec 081 ch. 5.2
                set_HART_in_RX_Wait();
            }
            break;

        case WIRED_HART_RX_CHECK:
            if ( !checkGapError(unRTCTime) )
            {
                if ( checkRxIdx() )
                { // buffer error in Check Byte Field => the response shall indicate the appropriate error in the communication error status, no data shall be returned
                    g_stProv.m_sWiredHart.m_RxMsgErrors |= WIRED_HART_RX_ERR_BUFFER_OVERFLOW;
                }
                // uart error in Check Byte Field => the response shall indicate the appropriate error in the communication error status, no data shall be returned
                g_stProv.m_sWiredHart.m_RxMsgErrors |= checkRxErrors(ucStatus);
                g_aucHartRxBuf[g_unHartRxIdx] = ucRxChar;
                g_ucRxCheckByte ^= ucRxChar;
                if ( g_ucRxCheckByte )
                { // error Check Byte does not verify => the response shall indicate the appropriate error in the communication error status, no data shall be returned
                    g_stProv.m_sWiredHart.m_RxMsgErrors |= WIRED_HART_RX_ERR_LONGITUDINAL_PARITY;
                }
                else
                {
                    ; // correct CheckByte
                }
                // get token
                g_stProv.m_sWiredHart.m_Token.m_OwnTokenTime = unRTCTime;
                g_stProv.m_sWiredHart.m_Token.m_OwnTokenFlag = TRUE;
                //
                g_stProv.m_sWiredHart.m_RxDelimiter = g_aucHartRxBuf[0];
                g_stProv.m_sWiredHart.m_RxMasterBurst = g_aucHartRxBuf[1];
                // message received
                g_stProv.m_sWiredHart.m_RxMsgReceived = 1;
                g_stProv.m_sWiredHart.m_unRxTimeStamp = unRTCTime;
                //
                return 1; // to exit from Low Power
            }
            else
            { // gap error in Command Field => fatal error abort the message ... spec 081 ch. 5.2
                set_HART_in_RX_Wait();
            }
        }
    }
    return 0;
}

/**
 * @brief Starts WiredHART data sending.
 * @param[in] *p_pucTxBuffer - pointer to the transmit data buffer.
 */
uint8 Wired_HART_StartTxBuffer (const uint8* p_pucTxBuffer)
{
uint8 ucTxStarted = 0;
//volatile uint16 unDelay = 0x3FFF; // do not modify this unless you are very sure what you are doing.
                                  // without this delay TX Hart will not work. Alternative
                                  // you could implement this almost one character (at 1200bps) delay in other manner

    if ( g_stProv.m_sWiredHart.m_Token.m_OwnTokenFlag == TRUE )
    {
        
        if ( (READ_FREE_RUNNING_TIMER() - g_stProv.m_sWiredHart.m_Token.m_OwnTokenTime) < WIRED_HART_STO )
        {
            // set HART modem in Tx mode
            HART_RTS_PIN_SET_LOW();
          // This delay 0x3FFF is almost a character time at 1200 bps
//            while (unDelay--)       // delay in order to wait Carrier start time.
//                ;                   // Time from RTS falling edge to first preamble char to be sent.
            //
            LOOP_WAIT_MSECONDS(10);
            
            g_stProv.m_sWiredHart.m_eTxState = WIRED_HART_TX_PREAMBLE;
            g_pucTxBuffer = p_pucTxBuffer;
            g_ucHartTxIdx = g_stProv.m_sWiredHart.m_Device.m_ucPreambleLength - 1;
            g_stProv.m_sWiredHart.m_unWaitTxEnd = 0;
            // enable TX
            UART3_StartTx(0xFF);            

            ucTxStarted = 1;
        }
    }
    return ucTxStarted;
}


/**
 * @brief Called from WiredHART USART interrupt. Sends a byte of data.
 */
void Wired_HART_UART_TxByte(void)
{
static uint8 g_ucTxCheckByte = 0;

    switch (g_stProv.m_sWiredHart.m_eTxState)
    {
        case WIRED_HART_TX_PREAMBLE:
          U3TXREG = 0xFF; // send preamble byte

            
            g_ucHartTxIdx--;
            if( ! g_ucHartTxIdx )
            { // all preambles done move to buffer
                uint8 ucByteCountIdx = 0; // need = 0.
                // init tx check byte
                g_ucTxCheckByte = 0;
                // compute buffer len
                if (g_pucTxBuffer[0] & HART_DELIMITER_EXPBYTES_MASK)
                { // add Exp Bytes if exists
                    ucByteCountIdx += ((g_pucTxBuffer[0] & HART_DELIMITER_EXPBYTES_MASK) >> 5);
                }
                if (g_pucTxBuffer[0] & HART_DELIMITER_ADDRESS_MASK)
                {  // add 1=Delimiter, 5=UniqueAddress, 1=Command , 1=ByteCount = 8
                    g_ucHartTxLen = 8 + ucByteCountIdx + g_pucTxBuffer[7 + ucByteCountIdx];
                }
                else
                { // add 1=Delimiter, 1=PollingAddress, 1=Command , 1=ByteCount = 4
                    g_ucHartTxLen = 4 + ucByteCountIdx + g_pucTxBuffer[3 + ucByteCountIdx];
                }
                g_stProv.m_sWiredHart.m_eTxState = WIRED_HART_TX_BUFFER;
            }
            break;

        case WIRED_HART_TX_BUFFER:

            if( g_ucHartTxIdx < g_ucHartTxLen )
            {
                g_ucTxCheckByte ^= g_pucTxBuffer[g_ucHartTxIdx];
                U3TXREG = g_pucTxBuffer[g_ucHartTxIdx++];                
            }
            else
            {
                U3TXREG = g_ucTxCheckByte;                
                g_stProv.m_sWiredHart.m_eTxState = WIRED_HART_TX_STOP;
            }
            break;

        case WIRED_HART_TX_STOP:

            // disable TX interrupt
          UART3_Disable_TxInterrupt();

            
            // message done
            g_ucHartTxLen = 0;
            // get
            g_stProv.m_sWiredHart.m_unTxTimeStamp = READ_FREE_RUNNING_TIMER();
            //
            g_stProv.m_sWiredHart.m_unWaitTxEnd = 1;
            break;
            
        default:
            break;
    }
}

/**
 * @brief Generates a WiredHART Response due to a request.
 * @param[in] *p_pucData - pointer to the response buffer to be sent.
 * @param[in] p_unLen - response data size.
 */
void AppHART_Tx_Response ( uint8 * p_pucData, uint16 p_unLen )
{
uint8 * pucIdx = (uint8 *) g_aucHartTxBuf;

    // Delimiter
    g_aucHartTxBuf[0] = ( g_stProv.m_sWiredHart.m_RxDelimiter & HART_DELIMITER_ADDRESS_MASK ) | HART_FRAME_ACK; // keep only the address bit
    pucIdx +=1;

    // Address
    if ( HART_DELIMITER_ADDRESS_MASK & g_stProv.m_sWiredHart.m_RxDelimiter )
    { // HART_DELIMITER_UNIQUE_ADDRESS
        g_aucHartTxBuf[1] = (0x80 & g_stProv.m_sWiredHart.m_RxMasterBurst) |
                            (0x3F & g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_AddrBytes[0]);
        g_aucHartTxBuf[2] = g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_AddrBytes[1];
        g_aucHartTxBuf[3] = g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_AddrBytes[2];
        g_aucHartTxBuf[4] = g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_AddrBytes[3];
        g_aucHartTxBuf[5] = g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_AddrBytes[4];
        pucIdx += 5;
    }
    else
    { // HART_DELIMITER_POOLING_ADDRESS
        g_aucHartTxBuf[1] = (0x80 & g_stProv.m_sWiredHart.m_RxMasterBurst) |
                            (0x3F & g_stProv.m_sWiredHart.m_Device.m_PollingAddr.m_AddrBits);
        pucIdx +=1;
    }

    // update field device status cold start based on m_RxMasterBurst
    if (g_stProv.m_sWiredHart.m_RxMasterBurst & HART_DELIMITER_ADDRESS_MASK)
    {
        p_pucData[3] |= g_ucDeviceStatus[MASTER_PRIMARY];
        if ( ! (g_ucDeviceStatus[MASTER_PRIMARY] & FieldDeviceStatusesMask_ColdStart) )
        {
            p_pucData[3] &= (~FieldDeviceStatusesMask_ColdStart);
        }
        g_ucDeviceStatus[MASTER_PRIMARY] &= (~FieldDeviceStatusesMask_ColdStart);
        g_ucDeviceStatus[MASTER_PRIMARY] &= (~FieldDeviceStatusesMask_ConfigurationChanged); // [???]-[0DD] // ... change this ... comenteaza asta pentru DDFile issue
    }
    else
    {
        p_pucData[3] |= g_ucDeviceStatus[MASTER_SECONDARY];
        g_ucDeviceStatus[MASTER_SECONDARY] &= (~FieldDeviceStatusesMask_ColdStart);
        g_ucDeviceStatus[MASTER_SECONDARY] &= (~FieldDeviceStatusesMask_ConfigurationChanged); // [???]-[0DD] // ... change this ... comenteaza asta pentru DDFile issue
    }

    // Payload
    memcpy(pucIdx, p_pucData, p_unLen);

    // Start TX
    Wired_HART_StartTxBuffer( (const uint8 *) g_aucHartTxBuf);
}

/**
 * @brief Writes the polling address and the loop current mode to the field device.
 * @param[in] p_ucAddress - polling address of the device.
 * @param[in] p_ucLoopCurrentMode - Loop Current Mode.
 */
uint8 Wired_HART_WritePollingAddress (uint8 p_ucAddress, uint8 p_ucLoopCurrentMode)
{
    g_stProv.m_sWiredHart.m_Device.m_PollingAddr.m_AddrBits = (HART_FIELD_POOLING_ADDRESS_BITS & p_ucAddress);
    g_stProv.m_sWiredHart.m_Device.m_ucLoopConfig = 0;

    FLASH_UPDATE_NEEDED();

    return RCS_N00_Success;
}

/**
 * @brief Sets the number of the WiredHART communication response preambles.
 * @param[in] p_ucNPreambles - number of preambles to be set.
 */
uint8 Wired_HART_WriteNumberOfResponsePreambles(uint8 p_ucNPreambles)
{

    if  ( p_ucNPreambles < 5 )
        return RCS_E04_PassedParameterTooSmall;

    if  ( p_ucNPreambles > 20 )
        return RCS_E03_PassedParameterTooLarge;

    g_stProv.m_sWiredHart.m_Device.m_ucPreambleLength = p_ucNPreambles;

    FLASH_UPDATE_NEEDED();

    return RCS_N00_Success;
}

void AppHART_Cmd_0_Response ( uint8 * p_pucData, uint16 p_unLen )
{
    // minimal check is command 0
    if ((p_pucData[0] != 0) || (p_pucData[4] != 0xFE))
        return;

    // keep the device status returned from WISA (cold bit start, config, etc.)
    g_ucDeviceStatus[MASTER_PRIMARY] = p_pucData[3] | FieldDeviceStatusesMask_ColdStart;
    g_ucDeviceStatus[MASTER_SECONDARY] = p_pucData[3] | FieldDeviceStatusesMask_ColdStart;
    g_ucDeviceStatus[MASTER_WISA] = p_pucData[3];
    g_ucDeviceStatus[MASTER_WISA] &= ~(FieldDeviceStatusesMask_ColdStart | FieldDeviceStatusesMask_MoreStatusAvailable);

    // g_ucDeviceStatus[MASTER_SECONDARY] |= FieldDeviceStatusesMask_ConfigurationChanged;  // [???]-[0DD] // ... change this ... decomenteaza asta pentru DDFile issue

    g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_AddrBytes[0] = p_pucData[5];
    g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_AddrBytes[1] = p_pucData[6];
    g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_AddrBytes[2] = p_pucData[13];
    g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_AddrBytes[3] = p_pucData[14];
    g_stProv.m_sWiredHart.m_Device.m_UniqueAddr.m_AddrBytes[4] = p_pucData[15];
    g_stProv.m_sWiredHart.m_ucInit = 1;
}


void AppHART_Cmd_0_Request ( void )
{
uint8 ucCmdBuffer[] = {0x01, 0x00, 0x00};

    // Send Cmd 0 request on API, in order to get Unique Address.
    API_SendRequest(CLS_USER_BOARD, DAQ_FW_HART_MODEM, 0, 3, (uint8 *) ucCmdBuffer, 0 );
}
