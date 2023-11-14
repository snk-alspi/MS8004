#include "interrupt_callbacks.h"
#include "xc.h"
#include "../Global/typedef.h"
#include "../Platform/uart.h"
#include "../MS/MS_Process.h"
#include "../Lib/WirelessHART_Lib.h"
#include "../WiredHart/wired_hart_link_layer.h"
#include "../WirelessHART_Full_API/wh_full_api_queue.h"
#include "../Platform/gpio.h"
#include "../Platform/GlobalVariables.h"
#include "../WirelessHART_App_Task/WirelessHART_App_Task.h"
#include "measurement.h"


#define PARITY(b) ((((((b)^(((b)<<4)|((b)>>4))) + 0x41) | 0x7C ) +2 ) & 0x80)

void mb_tx( void );

void __attribute__((__interrupt__, auto_psv)) _U1TXInterrupt(void)
{
    if( U1STAbits.UTXBF == 0)
    {
        LIB_RadioAPI_UART_SendByte();
    }    
    
    IFS0bits.U1TXIF = 0;
}

void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void) // WISA UART
{
    unsigned short usIrqStatus = U1STA;
    
    if (usIrqStatus & (_U1STA_OERR_MASK | _U1STA_FERR_MASK | _U1STA_PERR_MASK))
    {
        if (usIrqStatus & _U1STA_OERR_MASK)
        {
             U1STAbits.OERR = 0;
        }
        else
        {
            (void)U1RXREG;
        }
        g_ucApiRxState = API_Q_RX_IDLE;
    }
    //RX
    else if (usIrqStatus & _U1STA_URXDA_MASK)
    {
       LIB_RadioAPI_UART_ReceiveByte((uint8)(U1RXREG & 0x00FF));
    }
    
    IFS0bits.U1RXIF = 0;
}

void __attribute__((interrupt, auto_psv)) _U2RXInterrupt (void) // ANALOG -> SENSOR UART
{

    IFS1bits.U2RXIF =0;
        mb_rx_buff[mcnt]= U2RXREG;    
        mcnt++;
        timeout = 0;

}

void __attribute__((interrupt, auto_psv)) _U2TXInterrupt (void) // ANALOG -> SENSOR UART
{
    mb_tx();
    IFS1bits.U2TXIF = 0;
}


void __attribute__((__interrupt__, auto_psv)) _U3TXInterrupt(void) // WIRED HART UART
{
    unsigned short usIrqStatus = U3STA;
    
    if (!(usIrqStatus & _U3STA_UTXBF_MASK))
    {
        Wired_HART_UART_TxByte();
    }
    
    IFS5bits.U3TXIF = 0;
}

void __attribute__((__interrupt__, auto_psv)) _U3RXInterrupt(void) // WIRED HART UART
{ 
    unsigned short usIrqStatus = U3STA;
    
    if (usIrqStatus & (_U3STA_OERR_MASK | _U3STA_FERR_MASK | _U3STA_PERR_MASK))
    {
        if (usIrqStatus & _U3STA_OERR_MASK)
        {
             U3STAbits.OERR = 0;
        }
        else
        {
            (void)U3RXREG;
        }
    }
    else if ((usIrqStatus & _U3STA_URXDA_MASK) )
    {
        Wired_HART_UART_RxByte((uint8) (U3RXREG & 0x00FF), usIrqStatus);
    }
    
    IFS5bits.U3RXIF = 0;
}

/* Timer1 ISR */
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
    ++g_ucUserTimerEventCnt;
    
    PR1 = g_un125MsLoadTmr;    
    IFS0bits.T1IF = 0; /* Reset Timer1 interrupt flag */
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt (void)  
{
    uint16 unIOCSTAT = IOCSTAT;
    
    LIB_OnGpioIrq();
    
    if (unIOCSTAT & (1 << 6))
    {
        IOCFG  = 0;
        if (EXT_CTS_PIN_STATE() == 0)
        {
            LIB_RadioAPI_PreparePacketRx();
        }
    }

    IOCFD = 0;
    
    IOCSTAT &= ~unIOCSTAT;
    
    IFS1bits.CNIF  =0;
}

void mb_tx( void )
{
	unsigned char tx_data_dummy;
	unsigned char tx_parity_dummy;
    unsigned int dummyz;

	// get hart transmitter engine status
	if ( mb_comm_stat != MB_TX_FINISHED)
	{
		// hart transmitter engine is not finished

		// read next byte & parity to transmit
		//hart_transmit_data( &tx_data_dummy, &tx_parity_dummy );
        
        tx_data_dummy = mb_tx_buff[tx_counter];
        tx_parity_dummy = ~PARITY(mb_tx_buff[tx_counter]);
        tx_counter++;
		
		// write data to UART TX buffer
        dummyz = tx_parity_dummy;
        dummyz = dummyz << 8;
        dummyz |= tx_data_dummy;
		U2TXREG  = dummyz;

		IEC1bits.U2TXIE = 1;		// 1: Enables the EUSART transmit interrupt
        
        if(tx_counter>(tx_count-1)){
        mes_tx_stat = MB_TX_FINISHED;
        mb_comm_stat = MB_TX_FINISHED;
        mcnt=0;
        
        }
        
	}
}


