
/*
* METAL SAMPLES COMPANY, 152 Metal Samples Rd, Munford, Alabama, USA
* 
* Project Name     : MS8000 UT
* Version          : FW-2463-02.00
* Created Date     : 04 April 2023
* Modified date    : 05 May 2023
* Prod, Rel date   : ???????
* Author           : METAL SAMPLES COMAPNY
* 
* Host Processor   : PIC24FJ128GL406
* IDE              : MPLABX V6.00
* Compiler         : XC16 V2.0
* Pack             : 1.4.74
*  
* HB Part No.      : ET2463 REV 0
* MB Part No.      : ET2644 REV 0
* 
* 
 * Revision History
* 
* V01.0A to V01.0Y  FW Development Version
* 
 * 
 * Version Number changed as the main processor changed
 * FW-2643-80-02_00.0B
 * Date 04May23
 * 
 * Converted processor to PIC24FJ256GA406
 * Configuration bits modified.
 * Frequency set in "pic24.h" file
 * UART checked.
 * NVM Address replaced to old address.
 * Pin settings verified
 * Peripheral settings compared and corrected
 * Pin change interrupts corrected
 * Analog pins disabled.
 *UT Setup cable Analog voltage used for cable presence.
 * 
 * 
 * FW-2643-80-02_00.0E
 * Date 05October23
 * This version is modified from V00.0D from Bala
 * Commands 203 and 204 interchanged to comply with the document.
 * Error No.2 removed from current Command 203 as it is not applicable.
 * 
 */



// PIC24FJ256GA406 Configuration Bit Settings

// 'C' source line config statements

// FSEC
#pragma config BWRP = OFF               // Boot Segment Write Protect (Boot segment may be written)
#pragma config BSS = DISABLED           // Boot segment Protect (No Protection (other than BWRP))
#pragma config BSEN = OFF               // Boot Segment Control bit (No Boot Segment)
#pragma config GWRP = OFF               // General Segment Write Protect (Writes to program memory are allowed)
#pragma config GSS = DISABLED           // General Segment Code Protect (Code protection is disabled)
#pragma config CWRP = OFF               // Configuration Segment Program Write Protection bit (Configuration Segment may be written)
#pragma config CSS = DISABLED           // Configuration Segment Code Protection Level bits (No Protection (other than CWRP))
#pragma config AIVTDIS = DISABLE        // Alternate Interrupt Vector Table Disable bit (Disable AIVT)

// FBSLIM
#pragma config BSLIM = 0x1FFF           // Boot Segment Code Flash Page Address Limit bits (Enter Hexadecimal value)

// FSIGN

// FOSCSEL
#pragma config FNOSC = FRCDIV           // Oscillator Select (Fast RC Oscillator with divide-by-n (FRCDIV))
#pragma config PLLMODE = DISABLED       // Frequency Multiplier Select Bits (No PLL used; PLLEN bit is not available)
#pragma config IESO = OFF               // Internal External Switchover (Start up with user-selected oscillator source)

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary Oscillator disabled)
#pragma config OSCIOFCN = OFF           // OSCO Pin Configuration (OSCO/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config SOSCSEL = ON             // SOSC Power Selection Configuration bits (SOSC is used in crystal (SOSCI/SOSCO) mode)
#pragma config PLLSS = PLL_PRI          // PLL Secondary Selection Configuration bit (PLL is fed by the Primary oscillator)
#pragma config IOL1WAY = OFF            // IOLOCK One-Way Set Enable (The IOLOCK bit can be set and cleared using the unlock sequence)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog Timer is enabled)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Standard Watchdog Timer enabled (Windowed-mode is disabled))
#pragma config WDTWIN = PS25_0          // Watchdog Window Select bits (Watch Dog Timer Window Width is 25 percent)
#pragma config WDTCMX = WDTCLK          // WDT Clock Source Select bits (WDT clock source is determined by the WDTCLK Configuration bits)
#pragma config WDTCLK = LPRC            // WDT Clock Source Select bits (WDT uses LPRC)

// FPOR
#pragma config BOREN = OFF              // Brown-out Reset Enable bits (Brown-out Reset Disabled)
#pragma config LPCFG = OFF              // Low power regulator control (Disabled)

// FICD
#pragma config ICS = PGx1               // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC1/PGED1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)
#pragma config BTSWP = OFF              // BOOTSWP Instruction Enable bit (BOOTSWP instruction is disabled)

// FDS
#pragma config DSWDTPS = DSWDTPS1F      // Deep Sleep Watchdog Timer Postscale Select bits (1:68,719,476,736 (25.7 days))
#pragma config DSWDTOSC = LPRC          // DSWDT Reference Clock Select bit (DSWDT uses Low Power RC Oscillator (LPRC))
#pragma config DSBOREN = ON             // Deep Sleep Zero-Power BOR Enable bit (Deep Sleep BOR enabled in Deep Sleep)
#pragma config DSWDTEN = ON             // Deep Sleep Watchdog Timer Enable bit (DSWDT enabled)
//#pragma config DSSWEN = ON              // Deep Sleep Software Control Select Bit (Deep Sleep enabled and controlled by the DSEN bit)

// FDEVOPT1
#pragma config ALTCMPI = DISABLE        // Alternate Comparator Input Enable bit (C1INC, C2INC, and C3INC are on their standard pin locations)
#pragma config TMPRPIN = OFF            // Tamper Pin Enable bit (TMPRN pin function is disabled)
#pragma config TMPRWIPE = OFF           // RAM Based Entryption Key Wipe Enable bit (Cryptographic Engine Key RAM is not erased onTMPR pin events)
#pragma config ALTVREF = ALTVREFDIS     // Alternate VREF location Enable (VREF is on a default pin (VREF+ on RA10 and VREF- on RA9))

// FBTSEQ
#pragma config BSEQ = 0xFFF             // Relative value defining which partition will be active after devie Reset; the partition containing a lower boot number will be active. (Enter Hexadecimal value)
#pragma config IBSEQ = 0xFFF            // The one's complement of BSEQ; must be calculated by the user and written during device programming. (Enter Hexadecimal value)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


#include <string.h>
#include "Platform/timers.h"
#include "Platform/watchdog.h"
#include "Platform/measurement.h"
#include "Platform/GlobalVariables.h"
#include "MS/MS_Process.h"
#include "Lib/WirelessHART_Lib.h"
#include "WiredHart/wired_hart_app_layer.h"
#include "WiredHart/wired_hart_link_layer.h"
#include "Platform/nvm.h"
#include "MS/MS_Data.h"
#include "MS/MS_Process.h"
#include "Global/provision.h"
#include "WirelessHART_App_Task/WirelessHART_App_Task.h"
#include "WirelessHART_AppLayer/App_WH_User.h"

#include "Platform/measurement.h"  // to be removed. Added on 15Mar23 SNK

static uint8 s_uc250MsTmrClk = 0;
static uint8 s_uc1SecTmrClk = 0;
//static uint8 s_uc10SecTmrClk = 0;
//static uint8 s_uc100SecTmrClk = 0;
#define MS_CanSleep() (mes_stat == MES_STOPPED  && mes_para_stat == MB_PARA_STOPPED)

unsigned int ac, vj;
float vol1, vol;

void read_adc(void){
    
    vj=vol1=vol=0;
    
      for(ac=0;ac<100;ac++){

        while(!AD1CON1bits.DONE);

        vj = ADC1BUF0;             
        vol1 = ((float) vj);
        vol = (vol+vol1)/2;
        AD1CON1bits.SAMP = 1;
        } 
}
static void app_init()
{
    
    //__delay32(1000UL);
    
//    /*
//     * Remove this comment for 4MHz FRC
//    CLKDIVbits.RCDIV = 0;  // 000 Fast RC Oscillator (FRC)
//    CLKDIVbits.CPDIV = 1;  // 01 Divide by 2
//    */
    
    
    RCONbits.RETEN=0; // added on 03/03/2023
    GPIO_Init();
    
    
    // measurement board setup mode checking
        TRISBbits.TRISB13 =0;  // Power up Measurement board
        LATBbits.LATB13 = 0;
         
        ANSBbits.ANSB5 = 1;
        TRISBbits.TRISB5 =1;
        
        AD1CON1 = 0x8070;
        AD1CON2 = 0;
        AD1CON3 = 0x1F05;
        AD1CHS = 0x0005;
        AD1CSSL = 0;
        AD1CON1bits.SAMP =1;
        
        read_adc();
        /*
         Max resolution is 4096 count
         * For checking low 90% 4096 is 3690count
         
         
         */
        if(vol< 920){
        while(vol< 920){ClrWdt(); read_adc();}  //while(!PORTBbits.RB5);
    }    
    
        ANSBbits.ANSB5 = 1;     // Disable analog
        PMD1bits.ADC1MD = 1;    // Disable ADC function
//    if(!PORTBbits.RB5){
//        while(!PORTBbits.RB5){ClrWdt();}  //while(!PORTBbits.RB5);
//    }
    LATBbits.LATB13 = 1;
    LATBbits.LATB4 = 0;  // Pulled down for power saving
    
    NVM_ReadAllData();
    NVM_ReadPVData();    
    
    Timer1_Init();
    Timer4_Init();
    ResetRadioCharge();
    PowerConsumptionTracking_DataInit();
    CalculateLifeExpectancy();  
    
    RST_PIN_SET_LOW();
    
    LIB_RadioAPI_Init_HW_Interface();
    App_UserInit();
    Wired_HART_Init();
    MS_Initialize();
    
    strt_measurement(3);
            
    RST_PIN_SET_HIGH();
       
    API_Read_Cmd_0(); //Read Command 0 - Read Unique Identifier    
}

int main(void) {
    
    uint8 uc125MsTics;
    uint8 ucLast125MsTic = 0;
    
    app_init();
    
    
    while (1) 
    {
        if (MS_CanSleep() && (ucLast125MsTic == g_ucUserTimerEventCnt))
        {
            uint16 unSleepPeriod = LIB_GoToLowPower();
            if (unSleepPeriod)
            {
                PowerConsumptionTracking_AddSleep( unSleepPeriod );
            }
        }
        
        ClrWdt();
        LIB_MainTask();
        Wired_HART_Task();
        
        uc125MsTics = g_ucUserTimerEventCnt;
        if (ucLast125MsTic != uc125MsTics)
        {
            uint8 ucElapsed125MsTics = uc125MsTics - ucLast125MsTic;
            
            s_uc250MsTmrClk += ucElapsed125MsTics;
            
            ucLast125MsTic = uc125MsTics;
            
            // run at every 125 ms
            LIB_PeriodicTask_125msec();
 //           MS_PeriodicTasks_125msec(); Remvoed on 03/07/23 as per version 00-0j
            
             
            if (s_uc250MsTmrClk < 2)
                continue;
            s_uc250MsTmrClk -= 2;
            
            // run at every 250 ms
            LIB_PeriodicTask_250msec();
              
            if (++s_uc1SecTmrClk < 4)
                continue;
            s_uc1SecTmrClk -= 4;
            // run at every 1 sec
            LIB_PeriodicTask_1sec();
            MS_PeriodicTasks_1sec();
            PowerConsumptionTrackingTask_1sec();
             
             
        } 
    }
    return 0;
}

