/* 
 * File:   uart.h
 * Author: Cristian
 *
 * Created on June 22, 2021, 2:39 PM
 */

#ifndef _UART_H_
#define	_UART_H_

#include "pic24f.h"
#include "xc.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
    /* BAUD RATE CALCULATION*/
    /*
     * Baud Rate = FCY/4 / (UxBRG + 1)
     * U1BRG = (FCY/ 4 / baudrate) - 1 
     */
    #define UART_BRG(baudrate) ((((FPER_CLK/2)/4)-(baudrate/2))/(baudrate))
    
/* Definitions for baud rate */
    typedef enum baudRate {
        UART_BAUD_1200 = UART_BRG(1200),
        UART_BAUD_2400 = UART_BRG(2400),
        UART_BAUD_4800 = UART_BRG(4800),
        UART_BAUD_9600 = UART_BRG(9600),
        UART_BAUD_19200 = UART_BRG(19200),
        UART_BAUD_38400 = UART_BRG(38400),
        UART_BAUD_57600 = UART_BRG(57600),
        UART_BAUD_115200 = UART_BRG(115200)
    }  baudRate_t;
    
    void UART1_Init(baudRate_t p_unBRG);
    void UART1_Enable_TxInterrupt(void);
    void UART1_Disable_TxInterrupt(void);
    void UART1_Enable_RxInterrupt(void);
    void UART1_Disable_RxInterrupt(void);
    void UART1_StartTx(uint8_t p_ucByte);
    int UART1_TxIsEmpty();
    
    void UART2_Init(baudRate_t p_unBRG);
    
    void UART2_Enable_TxInterrupt(void);
    void UART2_Disable_TxInterrupt(void);
    void UART2_Enable_RxInterrupt(void);
    void UART2_Disable_RxInterrupt(void);
    void UART2_StartTx(uint8_t p_ucByte);
    
    void UART2_Enable_Tx(void);
    void UART2_Enable_Rx(void);
    void UART2_Disable(void);
    
    void UART3_Init(baudRate_t p_unBRG);
    void UART3_Enable_TxInterrupt(void);
    void UART3_Disable_TxInterrupt(void);
    void UART3_Enable_RxInterrupt(void);
    void UART3_Disable_RxInterrupt(void);
    void UART3_StartTx(uint8_t p_ucByte);


#ifdef	__cplusplus
}
#endif

#endif	/* _UART_H_ */

