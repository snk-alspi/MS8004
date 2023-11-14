#include "timers.h"

uint8 volatile g_ucUserTimerEventCnt = 0; ///< Variable used for initializing library tasking. Incremented every 25 milliseconds.
volatile uint16 g_un125MsLoadTmr;

void Timer1_Init()
{
    T1CON           = 0x00; /* Stop the timer and reset the control register */
    TMR1            = 0x00; /* Clear the timer register */
    T1CONbits.TSIDL = 0; /* TSIDL: Timer1 Stop in Idle Mode bit 0 = Continues module operation in Idle mode */
    T1CONbits.TCKPS = 0; /* TCKPS<1:0>: Timer1 Input Clock Prescale Select bits 11 = 1:256 10 = 1:64 01 = 1:8 00 = 1:1 */
    T1CONbits.TSYNC = 0; /* TSYNC: Timer1 External Clock Input Synchronization Select bit When TCS = 1: 0 = Does not synchronize external clock input */
    T1CONbits.TCS   = 1; /* TCS: Timer1 Clock Source Select bit 1 = Extended clock selected by the TECS<1:0> bits */
    T1CONbits.TECS = 2;  /* Timer1 Extended Clock Source Select bits (selected when TCS = 1) LPRC Oscillator */
    
    g_un125MsLoadTmr = TMR_MS2TICS(125)-1;
    
    PR1 = g_un125MsLoadTmr;
    
    /* Setup interrupt */
    IPC0bits.T1IP = 0x01; /* Setup Timer1 interrupt priority level 1 */
    IFS0bits.T1IF = 0;    /* Clear Timer1 interrupt status flag */
    IEC0bits.T1IE = 1;    /* Enable Timer1 interrupts */
    
    /* Enable Timer1 */
    T1CONbits.TON = 1;    
}

void Timer4_Init(void) 
{
    // configure Timer 4 as a free running timer at LPRC source clock
    T4CON           = 0x00; /* Stop the timer and reset the control register */
    TMR4            = 0x00; /* Clear the timer register */
    T4CONbits.TSIDL = 0; /* TSIDL: Timer1 Stop in Idle Mode bit 0 = Continues module operation in Idle mode */
    T4CONbits.TECS = 2; /* TECS[1:0]: Timerx Extended Clock Source Select bits (selected when TCS = 1); 10 = LPRC Oscillator */
    T4CONbits.TCKPS = 0; /* TCKPS<1:0>: Timer1 Input Clock Prescale Select bits 11 = 1:256 10 = 1:64 01 = 1:8 00 = 1:1;  */
    T4CONbits.TCS   = 1; /* TCS: Timer1 Clock Source Select bit 1 = Extended clock selected by the TECS<1:0> bits */    
    
    T4CONbits.TON = 1; // enable     
}

/**
 * @brief Returns the value of the free running timer counter at a desired moment.
 */
uint16 TMR_GP_GetTmrValue( void )
{
    uint16 unTmrReg;
    do 
    { 
        unTmrReg = TMR4;
    } while (((uint16)(TMR4 - unTmrReg)) > 1);
    
    return unTmrReg;
}

//
void LPTM_Adjust(uint16 p_unElapsedUsec, uint16 p_unCrtTics)
{
    uint32 nTmp;
    uint16 nNew125MsLoadTmr;
    
    // p_unElapsedUsec -> p_unCrtTics
    // 125000          -> 125ms tics timer
    
    nTmp = 125000 * (uint32)p_unCrtTics;
    nNew125MsLoadTmr =  (uint16)(nTmp / p_unElapsedUsec) - 1;    
    
    if (nNew125MsLoadTmr != g_un125MsLoadTmr)
    {
        if (nNew125MsLoadTmr < g_un125MsLoadTmr)
        {
            g_un125MsLoadTmr--;
        }
        else
        {
            g_un125MsLoadTmr++;
        }
    }
}

void TMR_WaitTics(uint16 p_unTmrTics)
{
    uint16 unTime = TMR_GP_GetTmrValue();
    
    while((uint16)(TMR_GP_GetTmrValue() - unTime) <= p_unTmrTics)
        ;
}