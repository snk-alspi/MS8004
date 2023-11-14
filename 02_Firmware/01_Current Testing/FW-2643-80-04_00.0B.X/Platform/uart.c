#include "uart.h"

 void UART1_Init(baudRate_t p_unBRG) {
    /* U1MODE register*/

    U1MODEbits.UARTEN = 1; /* UARTEN: UARTx Enable bit; 1 = UARTx is enabled; all UARTx pins are controlled by UARTx as defined by UEN[1:0]; */
    U1MODEbits.USIDL = 1; /* USIDL: UARTx Stop in Idle Mode bit; 0 = Continues module operation in Idle mode */
    U1MODEbits.IREN = 0; /* IREN: IrDA® Encoder and Decoder Enable bit; 0 = IrDA encoder and decoder are disabled  */
    U1MODEbits.RTSMD = 1; /* RTSMD: Mode Selection for UxRTS Pin bit; 1 = UxRTS pin is in Simplex mode */
    U1MODEbits.UEN = 0; /* UEN[1:0]: UARTx Enable bits; 00 = UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/UxBCLK pins are controlled by port latches */
    U1MODEbits.WAKE = 0; /* WAKE: Wake-up on Start Bit Detect During Sleep Mode Enable bit; 0 = No wake-up is enabled */
    U1MODEbits.LPBACK = 0; /* LPBACK: UARTx Loopback Mode Select bit; 0 = Loopback mode is disabled */
    U1MODEbits.ABAUD = 0; /* ABAUD: Auto-Baud Enable bit; 0 = Baud rate measurement is disabled or completed */
    U1MODEbits.URXINV = 0; /* URXINV: UARTx Receive Polarity Inversion bit; 0 = UxRX Idle state is ?1? */
    U1MODEbits.BRGH = 1; /* BRGH: High Baud Rate Enable bit; 1 = High-Speed mode (4 BRG clock cycles per bit) */
    U1MODEbits.PDSEL = 0; /*PDSEL[1:0]: Parity and Data Selection bits; 00 = 8-bit data, no parity */
    U1MODEbits.STSEL = 0; /* STSEL: Stop Bit Selection bit; 0 = One Stop bit*/

    U1BRG = p_unBRG;

    /*U1STAL register*/

    /*UTXISEL[1:0]: UARTx Transmission Interrupt Mode Selection bits; 10 = Interrupt when a character is transferred to the Transmit Shift Register (TSR), and as a result, the
                    transmit buffer becomes empty */
    U1STAbits.UTXISEL1 = 1;
    U1STAbits.UTXISEL0 = 0;

    U1STAbits.UTXINV = 0; /* UTXINV: UARTx IrDA® Encoder Transmit Polarity Inversion bit; For IREN = 0: 0 = UxTX Idle state is ?1? */
    U1STAbits.UTXBRK = 0; /* UTXBRK: UARTx Transmit Break bit; 0 = Sync Break transmission is disabled or completed */
    /* URXISEL[1:0]: UARTx Receive Interrupt Mode Selection bits; 0x = Interrupt is set when any character is received and transferred from the RSR to the receive buffer;
                    receive buffer has one or more characters */
    U1STAbits.URXISEL1 = 0;
    U1STAbits.URXISEL0 = 0;
    U1STAbits.ADDEN = 0; /*ADDEN: Address Character Detect bit (bit 8 of received data = 1); 0 = Address Detect mode is disabled  */

    U1SCCON = 0; /* Smart card disabled */
    U1SCINT = 0; /* Smart card interrupt disabled */

    /* Reset RX TX interrupt flag */
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;

    /* U1RX interrupt priority */
    /* U1RXIP[2:0]: UART1 Receiver Interrupt Priority bits*/
    IPC2bits.U1RXIP2 = 0;
    IPC2bits.U1RXIP1 = 0;
    IPC2bits.U1RXIP0 = 1;

    /* U1TX interrupt priority*/
    /* U1TXIP[2:0]: UART1 Transmitter Interrupt Priority bits*/
    IPC3bits.U1TXIP2 = 0;
    IPC3bits.U1TXIP1 = 0;
    IPC3bits.U1TXIP0 = 1;

    U1STAbits.UTXEN = 1; /* UTXEN: UARTx Transmit Enable bit; 1 = Transmit is enabled, UxTX pin is controlled by UARTx */
    U1STAbits.URXEN = 1; /* URXEN: UARTx Receive Enable bit; 0 = Receive is disabled, UxRX pin is controlled by the port */
}

void UART1_Enable_TxInterrupt(void) {
    _U1TXIE = 1;
}

void UART1_Disable_TxInterrupt(void) {
    _U1TXIE = 0;
}

void UART1_Enable_RxInterrupt(void) {
    _U1RXIE = 1;
}

void UART1_Disable_RxInterrupt(void) {
    _U1RXIE = 0;
}

void UART1_StartTx(uint8_t p_ucByte) {
    U1TXREG = p_ucByte;
    UART1_Enable_TxInterrupt();
}

int UART1_TxIsEmpty() {
    return U1STAbits.TRMT;
}

/*          UART2 INIT              */
// UART2 is used for measurement in MODBUS
/*
 * Baud rate 38400
 * 8 bit data
 * Even Parity
 * 1 stop bit
 */

void UART2_Init(baudRate_t p_ulBaudRate) {

    /* U3MODE register*/
    U2MODEbits.UARTEN = 1; /* UARTEN: UARTx Enable bit; 1 = UARTx is enabled; all UARTx pins are controlled by UARTx as defined by UEN[1:0]; */
    U2MODEbits.USIDL = 0; /* USIDL: UARTx Stop in Idle Mode bit; 0 = Continues module operation in Idle mode */
    U2MODEbits.IREN = 0; /* IREN: IrDA® Encoder and Decoder Enable bit; 0 = IrDA encoder and decoder are disabled  */
    U2MODEbits.RTSMD = 1; /* RTSMD: Mode Selection for UxRTS Pin bit; 1 = UxRTS pin is in Simplex mode */
    U2MODEbits.UEN = 0; /* UEN[1:0]: UARTx Enable bits; 00 = UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/UxBCLK pins are controlled by port latches */
    U2MODEbits.WAKE = 0; /* WAKE: Wake-up on Start Bit Detect During Sleep Mode Enable bit; 0 = No wake-up is enabled */
    U2MODEbits.LPBACK = 0; /* LPBACK: UARTx Loopback Mode Select bit; 0 = Loopback mode is disabled */
    U2MODEbits.ABAUD = 0; /* ABAUD: Auto-Baud Enable bit; 0 = Baud rate measurement is disabled or completed */
    U2MODEbits.URXINV = 0; /* URXINV: UARTx Receive Polarity Inversion bit; 0 = UxRX Idle state is ?1? */
    U2MODEbits.BRGH = 1; /* BRGH: High Baud Rate Enable bit; 1 = High-Speed mode (4 BRG clock cycles per bit) */
    U2MODEbits.PDSEL = 1; /*PDSEL[0:1]: Parity and Data Selection bits; 01 = 8-bit data, even parity */
    U2MODEbits.STSEL = 0; /* STSEL: Stop Bit Selection bit; 0 = One Stop bit*/

    /* BAUD RATE CALCULATION*/

    /*
     * Baud Rate = FCY/4 ? (UxBRG + 1)
     * UxBRG = (FCY/4 ? Baud Rate) ? 1
     * FCY = XTAL_FREQ/2 = 11059200 / 2 = 5,529,600
     * U1BRG = (5529600 / 4* 115200) - 1 = 11     
     */
    U2BRG = p_ulBaudRate; //25; // for 8MHz FRC    // old value 11 ;

    /*U2STAL register*/

    /*UTXISEL[1:0]: UARTx Transmission Interrupt Mode Selection bits; 10 = Interrupt when a character is transferred to the Transmit Shift Register (TSR), and as a result, the
                    transmit buffer becomes empty */
    U2STAbits.UTXISEL1 = 1;
    U2STAbits.UTXISEL0 = 0;
    U2STAbits.UTXINV = 0; /* UTXINV: UARTx IrDA® Encoder Transmit Polarity Inversion bit; For IREN = 0: 0 = UxTX Idle state is ?1? */
    U2STAbits.UTXBRK = 0; /* UTXBRK: UARTx Transmit Break bit; 0 = Sync Break transmission is disabled or completed */
    /* URXISEL[1:0]: UARTx Receive Interrupt Mode Selection bits; 0x = Interrupt is set when any character is received and transferred from the RSR to the receive buffer;
                    receive buffer has one or more characters */
    U2STAbits.URXISEL1 = 0;
    U2STAbits.URXISEL0 = 0;
    U2STAbits.ADDEN = 0; /*ADDEN: Address Character Detect bit (bit 8 of received data = 1); 0 = Address Detect mode is disabled  */

    /* U3RX interrupt priority */
    IPC7bits.U2RXIP = 1;

    /* U3TX interrupt priority */
    IPC7bits.U2TXIP = 1;

//    /* Reset RX TX interrupt flag */
    IFS1bits.U2RXIF = 0; 
    IFS1bits.U2TXIF = 0;
    
    
    
    
    


}

void UART2_Enable_Tx()
{
    IFS1bits.U2TXIF = 0;
    
    U2STAbits.UTXEN = 1; /* UTXEN: UARTx Transmit Enable bit; 1 = Transmit is enabled, UxTX pin is controlled by UARTx */    
    IEC1bits.U2TXIE = 1;
}

void UART2_Enable_Rx()
{
//    (void)U2RXREG;    
//    U2STA &= ~(_U2STA_OERR_MASK | _U2STA_FERR_MASK | _U2STA_PERR_MASK);
    
    IFS1bits.U2RXIF = 0;
    
    U2STAbits.URXEN = 1; /* URXEN: UARTx Receive Enable bit; 0 = Receive is disabled, UxRX pin is controlled by the port */        
    IEC1bits.U2RXIE = 1;    
}

void UART2_Disable()
{
    IEC1bits.U2RXIE = 0;
    IEC1bits.U2TXIE = 0;
    U2STAbits.URXEN = 0; /* URXEN: UARTx Receive Enable bit; 0 = Receive is disabled, UxRX pin is controlled by the port */        
    U2STAbits.UTXEN = 0; /* UTXEN: UARTx Transmit Enable bit; 1 = Transmit is enabled, UxTX pin is controlled by UARTx */    

    PMD1bits.U2MD = 1;
}



void UART2_StartTx(uint8_t p_ucByte) {
    U2TXREG = p_ucByte;
    UART2_Enable_Tx();
}


void UART3_Init(baudRate_t p_unBRG) {

    /* U3MODE register*/
    U3MODEbits.UARTEN = 1; /* UARTEN: UARTx Enable bit; 1 = UARTx is enabled; all UARTx pins are controlled by UARTx as defined by UEN[1:0]; */
    U3MODEbits.USIDL = 1; /* USIDL: UARTx Stop in Idle Mode bit; 0 = Continues module operation in Idle mode */
    U3MODEbits.IREN = 0; /* IREN: IrDA® Encoder and Decoder Enable bit; 0 = IrDA encoder and decoder are disabled  */
    U3MODEbits.RTSMD = 1; /* RTSMD: Mode Selection for UxRTS Pin bit; 1 = UxRTS pin is in Simplex mode */
    U3MODEbits.UEN = 0; /* UEN[1:0]: UARTx Enable bits; 00 = UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/UxBCLK pins are controlled by port latches */
    U3MODEbits.WAKE = 0; /* WAKE: Wake-up on Start Bit Detect During Sleep Mode Enable bit; 0 = No wake-up is enabled */
    U3MODEbits.LPBACK = 0; /* LPBACK: UARTx Loopback Mode Select bit; 0 = Loopback mode is disabled */
    U3MODEbits.ABAUD = 0; /* ABAUD: Auto-Baud Enable bit; 0 = Baud rate measurement is disabled or completed */
    U3MODEbits.URXINV = 0; /* URXINV: UARTx Receive Polarity Inversion bit; 0 = UxRX Idle state is ?1? */
    U3MODEbits.BRGH = 1; /* BRGH: High Baud Rate Enable bit; 1 = High-Speed mode (4 BRG clock cycles per bit) */
    U3MODEbits.PDSEL = 2; /*PDSEL[1:0]: Parity and Data Selection bits; 10 = 8-bit data, odd parity */
    U3MODEbits.STSEL = 0; /* STSEL: Stop Bit Selection bit; 0 = One Stop bit*/

    U3BRG = 416; //p_unBRG;

    /*U3STAL register*/

    /*UTXISEL[1:0]: UARTx Transmission Interrupt Mode Selection bits; 10 = Interrupt when a character is transferred to the Transmit Shift Register (TSR), and as a result, the
                    transmit buffer becomes empty */
    U3STAbits.UTXISEL1 = 1;
    U3STAbits.UTXISEL0 = 0;
    U3STAbits.UTXINV = 0; /* UTXINV: UARTx IrDA® Encoder Transmit Polarity Inversion bit; For IREN = 0: 0 = UxTX Idle state is ?1? */
    U3STAbits.UTXBRK = 0; /* UTXBRK: UARTx Transmit Break bit; 0 = Sync Break transmission is disabled or completed */
    /* URXISEL[1:0]: UARTx Receive Interrupt Mode Selection bits; 0x = Interrupt is set when any character is received and transferred from the RSR to the receive buffer;
                    receive buffer has one or more characters */
    U3STAbits.URXISEL1 = 0;
    U3STAbits.URXISEL0 = 0;
    U3STAbits.ADDEN = 0; /*ADDEN: Address Character Detect bit (bit 8 of received data = 1); 0 = Address Detect mode is disabled  */

    /* Reset RX TX interrupt flag */
    IFS5bits.U3RXIF = 0;
    IFS5bits.U3TXIF = 0;

    /* U3RX interrupt priority */
    /* U3RXIP[2:0]: UART3 Receiver Interrupt Priority bits*/
    IPC20bits.U3RXIP2 = 0;
    IPC20bits.U3RXIP1 = 0;
    IPC20bits.U3RXIP0 = 1;

    /* U3TX interrupt priority */
    /* U3TXIP[2:0]: UART3 Transmitter Interrupt Priority bits*/
    IPC20bits.U3TXIP2 = 0;
    IPC20bits.U3TXIP1 = 0;
    IPC20bits.U3TXIP0 = 1;

    U3STAbits.UTXEN = 1; /* UTXEN: UARTx Transmit Enable bit; 1 = Transmit is enabled, UxTX pin is controlled by UARTx */
    U3STAbits.URXEN = 1; /* URXEN: UARTx Receive Enable bit; 0 = Receive is disabled, UxRX pin is controlled by the port */
}

void UART3_Enable_TxInterrupt(void) {
    _U3TXIE = 1;
}

void UART3_Disable_TxInterrupt(void) {
    _U3TXIE = 0;
}

void UART3_Enable_RxInterrupt(void) {
    _U3RXIE = 1;
}

void UART3_Disable_RxInterrupt(void) {
    _U3RXIE = 0;
}

void UART3_StartTx(uint8_t p_ucByte) {
    U3TXREG = p_ucByte;
//    U3STAbits.UTXEN = 1;
    UART3_Enable_TxInterrupt();
}
