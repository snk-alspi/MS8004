#include <builtins.h>
#include "xc.h"
#include "uart.h"
#include "gpio.h"
#include "GlobalVariables.h"

/*
 https://ww1.microchip.com/downloads/en/DeviceDoc/01146B_chapter%202.pdf
 PIC® Microcontroller Low Power Tips ?n Tricks
 TIP #3 Configuring Port Pins
Unused Port Pins
If a port pin is unused, it may be left unconnected but configured as an output pin
driving to either state (high or low), or it may be configured as an input with an external
resistor (about 10 k?) pulling it to Vdd or Vss.
If configured as an input, only the pin input leakage current will be drawn through the
pin (the same current would flow if the pin was connected directly to Vdd or Vss). Both
options allow the pin to be used later for either input or output without significant hardware
modifications.
Digital Inputs
If such a pin can be configured as an analog input, the digital buffer is turned off,
reducing both the pin current as well as the total controller current.
Analog Inputs
Analog inputs have a very high-impedance so they consume very little current. They
will consume less current than a digital input if the applied voltage would normally be
centered between Vdd and Vss. Sometimes it is appropriate and possible to configure digital
inputs as analog inputs when the digital input must go to a low power state.
 */



void GPIO_Init(void)
{
    // Unlock Registers
    UNLOCK_IO_REGISTERS();
    
    // IO setting for unsued pins
    ANSB = 0;
    TRISBbits.TRISB6 = 0; LATBbits.LATB6 = 0;  // Pin 17
    TRISBbits.TRISB7 = 0; LATBbits.LATB7 = 0;  // Pin 18
    TRISBbits.TRISB10 = 0; LATBbits.LATB10 = 0;  // Pin 23
    TRISBbits.TRISB11 = 0; LATBbits.LATB11 = 0;  // Pin 24
    TRISBbits.TRISB12 = 0; LATBbits.LATB12 = 0;  // Pin 27
    TRISBbits.TRISB14 = 0; LATBbits.LATB14 = 0;  // Pin 29
    TRISBbits.TRISB15 = 0; LATBbits.LATB15 = 0;  // Pin 30
    
    TRISDbits.TRISD0 = 0; LATDbits.LATD0 = 0;  // Pin 46
    TRISDbits.TRISD1 = 0; LATDbits.LATD1 = 0;  // Pin 49
    TRISDbits.TRISD2 = 0; LATDbits.LATD2 = 0;  // Pin 50
    TRISDbits.TRISD3 = 0; LATDbits.LATD3 = 0;  // Pin 51
    TRISDbits.TRISD4 = 0; LATDbits.LATD4 = 0;  // Pin 52
    TRISDbits.TRISD5 = 0; LATDbits.LATD5 = 0;  // Pin 53
    TRISDbits.TRISD6 = 0; LATDbits.LATD6 = 0;  // Pin 54
    TRISDbits.TRISD7 = 0; LATDbits.LATD7 = 0;  // Pin 55
    
    TRISEbits.TRISE2 = 0; LATEbits.LATE2 = 0;  // Pin 62
    TRISEbits.TRISE3 = 0; LATEbits.LATE3 = 0;  // Pin 63
    TRISEbits.TRISE4 = 0; LATEbits.LATE4 = 0;  // Pin 64
    TRISEbits.TRISE5 = 0; LATEbits.LATE5 = 0;  // Pin 1
    TRISEbits.TRISE6 = 0; LATEbits.LATE6 = 0;  // Pin 2
    TRISEbits.TRISE7 = 0; LATEbits.LATE7 = 0;  // Pin 3
    
    TRISFbits.TRISF0 = 0; LATFbits.LATF0 = 0;  // Pin 58
    TRISFbits.TRISF1 = 0; LATFbits.LATF1 = 0;  // Pin 59
    TRISFbits.TRISF2 = 0; LATFbits.LATF2 = 0;  // Pin 33
    TRISFbits.TRISF3 = 0; LATFbits.LATF3 = 0;  // Pin 34
    TRISFbits.TRISF6 = 0; LATFbits.LATF6 = 0;  // Pin 35
    
    TRISGbits.TRISG2 = 0; LATGbits.LATG2 = 0;  // Pin 35
    TRISGbits.TRISG3 = 0; LATGbits.LATG3 = 0;  // Pin 35
    
    INIT_RST_PIN();
    
    GPIO_UART1_Init();
    GPIO_UART2_Init();
    GPIO_UART3_Init();
    
    // HART modem GPIO init
    INIT_HART_RTS_PIN();
    INIT_HART_CD_PIN();    
    HART_RTS_PIN_SET_HIGH(); 
    
    // Host API GPIO init            
    PADCONbits.IOCON = 1; /* Enable IOC functionality */ 
    
    INIT_WKU_PIN();    
    INIT_RDY_PIN();
    INIT_EXT_CTS_PIN();
    INIT_EXT_RTS_PIN();
    
    WKU_PIN_SET_LOW();  /* WKU is active high */
    EXT_RTS_PIN_SET_HIGH(); /* ExtRTS is active low */
    
    IOCFD = 0;
    IOCFG = 0;
    
    IEC1bits.CNIE = 1; /* Input Change Notification Interrupt Enable bit - Interrupt request is enabled */
        
   TRISBbits.TRISB0 = 0 ;
   LATBbits.LATB0 =0 ; // 0 --> low Power
   
   TRISBbits.TRISB1 =0;
   LATBbits.LATB1 =0;  //0 --> Low Power
    
   ANSBbits.ANSB2 = 0;
   LATBbits.LATB2 = 0; // 0 --> Low Power
   TRISBbits.TRISB2 = 0; // Output
    
   ANSBbits.ANSB3 = 0;
   LATBbits.LATB3 = 0; // 0 --> Low Power
   TRISBbits.TRISB3 = 0; // Output
   
    ANSBbits.ANSB4 = 0;
    LATBbits.LATB4 = 1; // 1 --> Disable
   TRISBbits.TRISB4 = 0; // Output
   
   ANSBbits.ANSB13 = 0; // Digital
   LATBbits.LATB13 = 1; // 1 --> Disable
   TRISBbits.TRISB13 = 0; // Output
    
    ANSC  &= ~((1 << 12) | (1 << 15));
    LATC  &= ~((1 << 12) | (1 << 15));
    TRISC &= ~((1 << 12) | (1 << 15));
        
    PMD1bits.T2MD = 1;
    PMD1bits.T3MD = 1;
    PMD1bits.T5MD = 1;
//    PMD1bits.ADC1MD =1;
    PMD1bits.SPI1MD =1;
    PMD1bits.SPI2MD =1;
    PMD1bits.I2C1MD =1;
    
 //   PMD2 = 0xFFFF;
    PMD3bits.CMPMD =1;
 //   PMD3bits.PMPMD =1;
    PMD3bits.CRCMD =1;
    PMD3bits.DAC1MD =1;
    PMD3bits.I2C3MD =1;
    PMD3bits.I2C2MD =1;
//    
    PMD4 = 0xFFFF;
    PMD5 = 0xFFFF;
    PMD6 = 0xFFFF;
    PMD7 = 0xFFFF;
        
    // Lock Registers
    LOCK_IO_REGISTERS();
}

void GPIO_UART1_Init(void) {
    
    // Assign U1RX to pin RP10
    RPINR18bits.U1RXR = 10;
    // Assign U1TX to pin RP17
    RPOR8bits.RP17R = 3;    
    
    // Assign U1TX To Pin RP10
    ANSFbits.ANSF5 = 0; /* Set pin as digital */
    TRISFbits.TRISF5 = 0; /* Set pin direction output */    

    // Assign U1RX To Pin RP17
    ANSFbits.ANSF4 = 0; /* Set pin as digital */
    LATFbits.LATF4 = 1;
    TRISFbits.TRISF4 = 1; /* Set pin direction input */
}

void GPIO_UART2_Init(void) 
{
    ANSBbits.ANSB8 = 1; // disable digital
    ANSBbits.ANSB9 = 1; // disable digital
    
    TRISBbits.TRISB9 = 0; // rx pin 22. set as input
    TRISBbits.TRISB8 = 1; // tx pin 21  set as input
    
}

void GPIO_UART2_Enable(void)
{
      
    TRISBbits.TRISB8 = 1; // tx pin 21    // set as output
    TRISBbits.TRISB9 = 0;
    
    
    ANSBbits.ANSB8 = 0; // set pin as digital
    ANSBbits.ANSB9 = 0; // set pin as digital
    
    UNLOCK_IO_REGISTERS();
        
    /* Attach UART2 module to I/O pads */
    RPOR4bits.RP9R = 5; // U2 TX	/* U2TX --> RP8 */
    RPINR19bits.U2RXR = 8; // U2 RX        /* U2RX <-- RP9 */
    
    LOCK_IO_REGISTERS();
}

void GPIO_UART2_Disable(void)
{
    UNLOCK_IO_REGISTERS();
    
    RPOR4bits.RP8R = 0; // U2 TX	/* U2TX --> RP8 */
    RPINR19bits.U2RXR = 0; // U2 RX        /* U2RX <-- RP9 */

    LOCK_IO_REGISTERS();
    
//    TRISBbits.TRISB9 = 1; // tx pin 21  set as input
     TRISBbits.TRISB9 = 0; // tx pin 21  set as output for testing 050523
    
     ANSBbits.ANSB8 = 1; // disable digital // Disabled for Testing
    ANSBbits.ANSB9 = 1; // disable digital
    
}

void GPIO_UART3_Init(void) {
    
    // Assign U3RX to pin RP4
    RPINR17bits.U3RXR = 4;
    // Assign U3TX to pin RP2
    RPOR1bits.RP2R = 19; //22;    
    
    // Assign U3RX To Pin RP4
    ANSDbits.ANSD9 = 0; /* Set pin as digital */
    TRISDbits.TRISD9 = 1; /* Set pin direction input */    

    // Assign U3TX To Pin RP3
    ANSDbits.ANSD8 = 0; /* Set pin as digital */
    LATDbits.LATD8 = 1;
    TRISDbits.TRISD8 = 0; /* Set pin direction output */
}
