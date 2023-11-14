/*
 * File:   main.c
 * Author: Lite
 *
 * Created on November 28, 2015, 2:00 PM
 */


// FSEC
#pragma config BWRP = OFF               // Boot Segment Write Protect (Boot segment may be written)
#pragma config BSS = OFF                // Boot segment Protect (No Protection (other than BWRP))
#pragma config BSEN = OFF               // Boot Segment Control bit (No Boot Segment)
#pragma config GWRP = OFF               // General Segment Write Protect (Writes to program memory are allowed)
#pragma config GSS = OFF                // General Segment Code Protect (Code protection is disabled)
#pragma config CWRP = OFF               // Configuration Segment Program Write Protection bit (Configuration Segment may be written)
#pragma config CSS = DIS                // Configuration Segment Code Protection Level bits (No Protection (other than CWRP))
#pragma config AIVTDIS = DISABLE        // Alternate Interrupt Vector Table Disable bit (Disable AIVT)

// FBSLIM
#pragma config BSLIM = 0x1FFF           // Boot Segment Code Flash Page Address Limit bits (Boot Segment Flash page address limit)

// FSIGN

// FOSCSEL
#pragma config FNOSC = PRI              // Oscillator Select (Fast RC Oscillator (FRC))
#pragma config PLLMODE = DISABLED       // Frequency Multiplier Select Bits (No PLL used; PLLEN bit is not available)
#pragma config IESO = ON                // Internal External Switchover (Start up device with FRC, then switch to user-selected oscillator source)

// FOSC
#pragma config POSCMOD = HS           // Primary Oscillator Select (Primary Oscillator disabled)
#pragma config OSCIOFCN = OFF           // OSCO Pin Configuration (OSCO/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config SOSCSEL = OFF             // SOSC Power Selection Configuration bits (SOSC is used in crystal (SOSCI/SOSCO) mode)
#pragma config PLLSS = PLL_PRI          // PLL Secondary Selection Configuration bit (PLL is fed by the Primary oscillator)
#pragma config IOL1WAY = OFF             // IOLOCK One-Way Set Enable (Once set the IOLOCK bit cannot be cleared)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Standard Watchdog Timer enabled (Windowed-mode is disabled))
#pragma config WDTWIN = PS25_0          // Watchdog Window Select bits (Watch Dog Timer Window Width is 25 percent)
#pragma config WDTCMX = WDTCLK          // WDT Clock Source Select bits (WDT clock source is determined by the WDTCLK Configuration bits)
#pragma config WDTCLK = LPRC            // WDT Clock Source Select bits (WDT uses LPRC)

// FPOR
#pragma config BOREN = ON               // Brown-out Reset Enable bits (Brown-out Reset Enable)
#pragma config LPCFG = OFF              // Low power regulator control (Disabled)

// FICD
#pragma config ICS = PGx1               // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC1/PGED1)
#pragma config JTAGEN = OFF              // JTAG Port Enable (JTAG port is enabled)
#pragma config BTSWP = OFF              // BOOTSWP Instruction Enable bit (BOOTSWP instruction is disabled)

// FDS
#pragma config DSWDTPS = DSWDTPS1F      // Deep Sleep Watchdog Timer Postscale Select bits (1:68,719,476,736 (25.7 days))
#pragma config DSWDTOSC = LPRC          // DSWDT Reference Clock Select bit (DSWDT uses Low Power RC Oscillator (LPRC))
#pragma config DSBOREN = ON             // Deep Sleep Zero-Power BOR Enable bit (Deep Sleep BOR enabled in Deep Sleep)
#pragma config DSWDTEN = ON             // Deep Sleep Watchdog Timer Enable bit (DSWDT enabled)

// FDEVOPT1
#pragma config ALTCMPI = DISABLE        // Alternate Comparator Input Enable bit (C1INC, C2INC, and C3INC are on their standard pin locations)
#pragma config TMPRPIN = OFF            // Tamper Pin Enable bit (TMPRN pin function is disabled)
#pragma config TMPRWIPE = OFF           // RAM Based Entryption Key Wipe Enable bit (Cryptographic Engine Key RAM is not erased onTMPR pin events)
#pragma config ALTVREF = ALTVREFDIS     // Alternate VREF location Enable (VREF is on a default pin (VREF+ on RA10 and VREF- on RA9))


#include "xc.h"
#include "hal.h"
#include "typedef.h"
#include "utils.h"
#include "objects/uap.h"
#include "objects/uapmo.h"
#include "objects/co.h"
#include "objects/bi.h"
//#include "feprom.h"
#include "nvm.h"
#include "sleep.h"
#include <string.h>
#include "application/app.h"
#include "api_handler/api_protocol.h"
#include "api_handler/provision.h"
#include "GlobalVariables.h"

static void vFastTaskContainer();
static void v125msec_task_container();
static void v250msec_task_container();
static void v1sec_task_container();


    /////////// DISPLAY AND KEYPAD /////////////////
volatile unsigned int lkey, ukey, dkey, rkey, ekey;
volatile unsigned char sticky;
int kcnt;  
unsigned char bt_btn; // Bluetooth / usb button


unsigned char value1, value2, value3;

volatile unsigned char keyfnd, oldkey, newkey, keycnt, kpos;
volatile unsigned char dat_rec;
char revli, revchr, passok;


#define _XTAL_FREQ 11059200UL
extern volatile unsigned char tx_flg;
//#include "DEE Emulation 16-bit.h"

 ///////////////// DISPLAY & KEYPAD ENDS


int main(void) 
{

    /* CPU clock */
    /* PIC24FJ256GA412/GB412 FAMILY -> The oscillator source (and operating mode) that is used
    at a device Power-on Reset event is selected using Configuration
    bit settings. The Oscillator Configuration bit
    settings are located in the Configuration registers in the
    program memory (refer to Section 33.1 ?Configuration
    Bits? for further details). The Primary Oscillator
    Configuration bits, POSCMOD<1:0> (FOSC<1:0>),
    and the Initial Oscillator Select Configuration bits,
    FNOSC<2:0> (FOSCSEL<2:0>), select the oscillator
    source that is used at a Power-on Reset. */
    INIT_DEBUG_LED_RB2();
    INIT_DEBUG_LED_RD5();
    DEBUG_LED_ON(); 
    
    DEBUG_LED_RD5_ON();
    //LED  --> MS
    TRISDbits.TRISD6 = 0; // Power LED
    LATDbits.LATD6=0;
    //*******************************
    /* HAL init */
    /* Disable interrupts */
    //GLOBAL_INTERRUPT_DISABLE();
    
    /* Timer 1 init: Type A Timer Asynchronous Counter Mode Using External Clock Input 32.768 kHz on SOSCI, SOSCO */
    timers_t1_init();

    /* Init GPIOs: 
    * WKU pin - output, GPIO
    * EXT_RTS pin - output, GPIO
    * RDY pin - input, GPIO, interrupt INT1
    * EXT_CTS pin - input, GPIO, interrupt INT2
    */
    asm volatile ("MOV #OSCCON, w1 \n"
    "MOV #0x46, w2 \n"
    "MOV #0x57, w3 \n"
    "MOV.b w2, [w1] \n"
    "MOV.b w3, [w1] \n"
    "BCLR OSCCON, #6");
    
    init_gpios();
   
    /* Init UART1:  8-bit data, No Parity, 1 Stop bit, Transmit and Receive 
    * Used for communication with the radio module
    */
    uart1_init(UART_BAUD_38400);
    
     /* Lock Registers */
    asm volatile ("MOV #OSCCON, w1 \n"
    "MOV #0x46, w2 \n"
    "MOV #0x57, w3 \n"
    "MOV.b w2, [w1] \n"
    "MOV.b w3, [w1] \n"
    "BSET OSCCON, #6");
    
    /***********************************************************************/
  IoInit(); // Added By MS

    /*********************************************************************/



    /* Put FSM in INIT state and reset the RX buffer index */
    API_PROTOCOL_RxFsmReset();
    /* Put FSM in INIT state and reset the RX buffer index */
    API_PROTOCOL_TxFsmReset();
    
    g_ucApiTxFsmState = MSG_NULL;

    appStruct.deviceStatus = 1;
    appStruct.deviceAddress = 2;
    appStruct.r_exposed = 3;
    appStruct.r_reference = 4;
    appStruct.r_Check = 5;
    appStruct.ratio = 6;
        
    UAPMO_Init();
    BI_Init();
    
    provision_init();
    
    NVM_DATA_MO mo;
    if (nvm_read_mo(&mo) != ERR_OK)
    {
        mo.m_unRevision = 0;
        mo.m_ucUapState = 0;
        mo.m_ucSignature = 0;
    }
    
    uint16 uapmoId = 0;
    uint8 aucVersion[5] = {'A', 'L', 'S', 'P', 'I'};
    UAPMO_Register(UAPID, aucVersion, 5, mo.m_unRevision, &uapmoId);

    uint16 coId1 = 0;
    uint16 coId2 = 0;
    IPV6_ADDR addr = {0xFE, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x7C, 0x7F, 0x00, 0x00, 0x01};
        
    BINARY_INPUT bi;
    BINARY_VALUE_STATUS bvalStat;
    bvalStat.ucValue = 1;
    
    CONTROL_MODE biControlMode;

    uint16 biId = 0;
    BI_Register(&bi, UAPID, &bvalStat, &biControlMode, &biId);
    
    //TODO read the endpoint from flash, if not available go to default
    NVM_DATA_CO nvmCo;
    //if (nvm_read_co(&nvmCo) != ERR_OK)
    {
        // could not read from flash
        nvmCo.m_ucConcentratorCount = 1;
        memcpy(nvmCo.m_stConcentratorData[0].m_stEndpoint.m_aRemoteAddr128, addr, 16);
        nvmCo.m_stConcentratorData[0].m_stEndpoint.m_nPubPeriod = 5;//3600;
        nvmCo.m_stConcentratorData[0].m_stEndpoint.m_ucIdealPhase = 0;
        nvmCo.m_stConcentratorData[0].m_stEndpoint.m_ucPubAutoRetransmit = 0;
        nvmCo.m_stConcentratorData[0].m_stEndpoint.m_ucStaleDataLimit = 0;
        nvmCo.m_stConcentratorData[0].m_stEndpoint.m_unRemoteObjID = OBJTYPE_DO;   // dispersion obj
        nvmCo.m_stConcentratorData[0].m_stEndpoint.m_unRemoteTLPort = 0xF0B2;
        nvmCo.m_stConcentratorData[0].m_stEndpoint.m_ucConfigured = 1;
        
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[0].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[0].m_unAttrID = BI_CUSTOM_ATTR1;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[0].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[0].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[1].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[1].m_unAttrID = BI_CUSTOM_ATTR2;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[1].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[1].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[2].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[2].m_unAttrID = BI_CUSTOM_ATTR3;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[2].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[2].m_unSize = BINARY_CUSTOM_ATTR_SIZE;
/*
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[3].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[3].m_unAttrID = BI_CUSTOM_ATTR4;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[3].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[3].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[4].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[4].m_unAttrID = BI_CUSTOM_ATTR5;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[4].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[4].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[5].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[5].m_unAttrID = BI_CUSTOM_ATTR6;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[5].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[5].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[6].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[6].m_unAttrID = BI_CUSTOM_ATTR7;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[6].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[6].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[7].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[7].m_unAttrID = BI_CUSTOM_ATTR8;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[7].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[7].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[8].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[8].m_unAttrID = BI_CUSTOM_ATTR9;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[8].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[8].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[9].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[9].m_unAttrID = BI_CUSTOM_ATTR10;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[9].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[9].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[10].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[10].m_unAttrID = BI_CUSTOM_ATTR11;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[10].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[10].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[11].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[11].m_unAttrID = BI_CUSTOM_ATTR12;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[11].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[11].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[12].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[12].m_unAttrID = BI_CUSTOM_ATTR13;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[12].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[12].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[13].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[13].m_unAttrID = BI_CUSTOM_ATTR14;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[13].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[13].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[14].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[14].m_unAttrID = BI_CUSTOM_ATTR15;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[14].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[14].m_unSize = BINARY_CUSTOM_ATTR_SIZE;

        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[15].m_unObjID = biId;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[15].m_unAttrID = BI_CUSTOM_ATTR16;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[15].m_unAttrIdx = 0;
        nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[15].m_unSize = BINARY_CUSTOM_ATTR_SIZE;
        
        nvmCo.m_stConcentratorData[0].m_ucPubItemsNo = 16;*/
        nvmCo.m_stConcentratorData[0].m_ucPubItemsNo = 3;
        nvmCo.m_stConcentratorData[0].m_ucRevision = 0;
        
        nvm_save_co(&nvmCo);
    }
    
    CONCENTRATOR co1;    
    CO_Init(&co1, 0, &nvmCo.m_stConcentratorData[0].m_stEndpoint, 16, nvmCo.m_stConcentratorData[0].m_ucPubItemsNo, 
            &nvmCo.m_stConcentratorData[0].m_aAttrDescriptor[0], &coId1);

   
    /* Enable interrupts */
    //TODO: GLOBAL_INTERRUPT_ENABLE();

    //provision_start();
    
    //provision_reset();
    
    while(1)
    {
        vFastTaskContainer();
        
        if (read_125msec_flag() == TRUE)
        {
            
            /* Execute 125 msec tasks */
            v125msec_task_container();
                       
            
            g_uc125msecCounter++;
            if (0 == (g_uc125msecCounter % 2))
			{
				g_uc250msec = TRUE;
			}
            
            if (g_uc125msecCounter >= 8)
			{
				g_ucOneSecond       = TRUE;
				g_uc125msecCounter  = 0;
			}
        }
        
        if(TRUE == g_uc250msec)
        {
            /* Execute 250 msec tasks */
            v250msec_task_container();
            g_uc250msec = FALSE;
        }
        
        if(TRUE == g_ucOneSecond)
        {
            /* Execute 1 sec tasks */
            v1sec_task_container();
            g_ucOneSecond = FALSE;
            /*  Added for measurement sensing */
            if(measure_flag==1){measure_secs++;}
            /*  Added for measurement sensing */
        }
        /************************************************************************/
        /* Added By MetalSamples
        // In this model ASPI variables are not stored in Flash , it is stored in SD card
        // The Following are the variables  stored in SD card
        // password (3333),Interval,Display Power Dn Time, Active PID, PID 1,2,3,4 and Unit ID
         ************************************************************************ */
        if(ekey>kavl){
             init_sd_card();
            display_init();
           
            mnu();
            wr_config();
            //set_probe_data();
            disp_normal();
            lkey = 0;
            }
        if(measure_flag==1){
         if(measure_secs>80){
             
             if(mcnt){
                 disp_values();
    } else{
        err_led = 1;
    set_pos(2,0,0);
    lcd_str("Measurement Error1"); 
    Er_stat = 1;
    }
                 
            Pwr_USB_Dis // added on 081517 in order to reduce the current after measurement
             measure_secs=0;
            measure_flag=0;
         }   
        } else {
        rtc_init();
        chk_measurement();
        }
        /*************************************************************************/
        
        /* TODO: enter sleep */
        
        /*Enter Sleep Mode if possible */
        
        // TO DISABLE SLEEP, comment the next line
        sleep_enter();
        
        DEBUG_LED_TOGGLE();
    }
    
    
    
    return 0;
}


static void v125msec_task_container()
{
//    DEBUG_LED_TOGGLE();
    //WKU_PIN_TOGGLE();
    API_PROTOCOL_SendMsgTask();
    CO_TaskPeriodic();
}

static void v250msec_task_container()
{
    //WKU_PIN_SET_LOW();
}

static void v1sec_task_container()
{
    UAP_OneSecondTasks();
    
    CO_TaskOneSecond();
    
    provision_timer();    
}

static void vFastTaskContainer()
{
    /* Process the received message if there is one */
    API_PROTOCOL_RecvMsgTask();
}