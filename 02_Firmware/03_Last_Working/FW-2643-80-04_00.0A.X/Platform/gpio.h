/* 
 * File:   gpio.h
 * Author: Cristian
 *
 * Created on June 22, 2021, 1:02 PM
 */

#ifndef _GPIO_H_
#define	_GPIO_H_

#ifdef	__cplusplus
extern "C" {
#endif
    
    //----------------- RESET PIN DEFINITION --------------------------// 
    
    /* RESET pin - output, GPIO */
    /* This signal is active low and is used by the application CPU to reset the radio modem CPU.
    */
    /* RESET pin - RE0 */
#define INIT_RST_PIN() { ANSEbits.ANSE0      = 0; /* Set pin as digital */ \
                         LATEbits.LATE0      = 1; /* Set output level high*/ \
                         TRISEbits.TRISE0    = 0; /* Set pin direction output */ \
                       }
#define RST_PIN_SET_HIGH()  {LATEbits.LATE0 = 1;}
#define RST_PIN_SET_LOW()   {LATEbits.LATE0 = 0;}
    
    /////////////////////////////////////////////////////////////////////////////////////////////
    
    //----------------- API DEFINITIONS --------------------------//

    /* WKU pin - output, GPIO */
    /* This signal is active high and is used by the application CPU to wake up the radio modem CPU from hibernation
     *  and to signal the intention to communicate with the modem. Keeping this line active will block the modem from
     *  entering the low power mode (sleep).
     */
    /* WKU pin - RG9 */
#define INIT_WKU_PIN() { ANSGbits.ANSG9      = 0; /* Set pin as digital */ \
                         TRISGbits.TRISG9    = 0; /* Set pin direction output */ \
                       }
#define WKU_PIN_SET_HIGH()  {LATGbits.LATG9 = 1;}
#define WKU_PIN_SET_LOW()   {LATGbits.LATG9 = 0;}

    /* EXT_RTS pin - output, GPIO */
    /* This signal is active low and is used by the application CPU to indicate a ready-to-receive 
     * state. It will Implicitly confirm the acquisition of ExtCTS low signal to the modem.
     */
    /* EXT_RTS pin - RG7 */
#define INIT_EXT_RTS_PIN() { ANSGbits.ANSG7     = 0; /* Set pin as digital */ \
                            TRISGbits.TRISG7    = 0; /* Set pin direction output */ \
                            }
#define EXT_RTS_PIN_SET_HIGH()  {LATGbits.LATG7 = 1;}
#define EXT_RTS_PIN_SET_LOW()   {LATGbits.LATG7 = 0;}
#define EXT_RTS_PIN_STATE()     (LATGbits.LATG7)

    /* RDY pin - input, GPIO, interrupt */
    /* This signal is active low and is used by the modem to indicate a ready-to-receive state. It 
     * will be generated as a response to the application CPU WKU signal.
     */
    /* RDY pin - RG6 */
#define INIT_RDY_PIN() { ANSGbits.ANSG6      = 0; /* Set pin as digital */ \
                         TRISGbits.TRISG6    = 1; /* Set pin direction input */ \
                       }
#define RDY_PIN_STATE()     (PORTGbits.RG6)


#define RDY_PIN_IF(x)   { IFS1bits.INT1IF = x; } /* Set/clear INT1 interrupt flag */

    /* EXT_CTS pin - input, GPIO, interrupt */
    /* This signal is active low and is used by the modem to indicate a 
     * want-to-send state. The signal will return to high state after a high to low transition of the 
     * ExtRTS signal.
     */
    /* EXT_CTS pin - RG8 */
#define INIT_EXT_CTS_PIN() {\
                                ANSGbits.ANSG8 = 0; /* Set pin as digital */ \
                                TRISGbits.TRISG8    = 1; /* Set pin direction input */ \
                                IOCPUGbits.IOCPUG8 = 1; /* Enable pull-up */ \
                                IOCNGbits.IOCNG8 = 1; /* Interrupt enabled on high-to-low-transition */ \
                            }

#define EXT_CTS_PIN_STATE()     (PORTGbits.RG8)

    /* Init GPIOs: 
     * WKU pin - output, GPIO
     * EXT_RTS pin - output, GPIO
     * RDY pin - input, GPIO, interrupt INT1
     * EXT_CTS pin - input, GPIO, interrupt INT2
     */

    /*HART_RTS pin - RB6*/
#define INIT_HART_RTS_PIN() {   ANSDbits.ANSD10 = 0; /* Set pin as digital */ \
                                TRISDbits.TRISD10 = 0; /* Set pin direction output */ \
                                }

#define HART_RTS_PIN_SET_HIGH()  {LATDbits.LATD10 = 1;}
#define HART_RTS_PIN_SET_LOW()   {LATDbits.LATD10 = 0;}
#define HART_RTS_PIN_STATE()     (LATDbits.LATD10)


#define INIT_HART_CD_PIN() {   ANSDbits.ANSD11 = 0; /* Set pin as digital */ \
                                TRISDbits.TRISD11 = 1; /* Set pin direction input */ \
                                IOCPDbits.IOCPD11 = 1; /* Interrupt enabled on low-to-high-transition */ \
                                }

#define HART_CD_PIN_STATE()     (PORTDbits.RD11)
//#endif
    
    /////////////////////////////////////////////////////////////////////////////////////////////

      

    void GPIO_Init(void);
    void GPIO_UART1_Init(void);
    void GPIO_UART2_Init(void);
    void GPIO_UART3_Init(void);
    
    void GPIO_UART2_Enable();
    void GPIO_UART2_Disable();


#ifdef	__cplusplus
}
#endif

#endif	/* GPIO_H */

