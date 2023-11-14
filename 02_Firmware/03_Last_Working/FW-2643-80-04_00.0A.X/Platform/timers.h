/* 
 * File:   timers.h
 * Author: Cristian
 *
 * Created on June 22, 2021, 2:39 PM
 */

#ifndef TIMERS_H
#define	TIMERS_H

#include "xc.h"
#include "../Global/typedef.h"

#ifdef	__cplusplus
extern "C" {
#endif   
    
    extern volatile uint8 g_ucUserTimerEventCnt;
    extern volatile uint16 g_un125MsLoadTmr;

    enum STATE_TIMEOUT_IDX {
        API_READY_ACTIVATION_TMO_IDX = 0,
        API_AGRESIV_TMO_IDX = 1,
        LIB_RESERVED2_TMO_IDX = 2,
        LIB_RESERVED3_TMO_IDX = 3,
        LIB_RESERVED4_TMO_IDX = 4,
        USER_RESERVED1_TMO_IDX = 5,
        USER_RESERVED2_TMO_IDX = 6,
        MAX_MANAGED_TIMEOUTS_NO
    };    
    
    void Timer1_Init(void);
    void LPTM_Adjust(uint16 p_unElapsedUsec, uint16 p_unCrtTics);
    void Timer4_Init(void);
    uint16 TMR_GP_GetTmrValue(void);
    // RECEIVED FROM ALPSI - LEFT AS RECEIVED
    void InitTimer3(void);
    void Timer4_Start();
    void Timer4_Stop();

    void TMR_WaitTics(uint16 p_unTmrTics);
    
//    #define TMR_MS2TICS(ms) ((ms)*(31*5)/4) // 31.25 KHz tmr
    #define TMR_MS2TICS(ms) ((ms)*(31)) // 31 KHz tmr
    
    #define READ_FREE_RUNNING_TIMER()  TMR_GP_GetTmrValue()
    #define LOOP_WAIT_MSECONDS(ms)  TMR_WaitTics(TMR_MS2TICS(ms)) 
    



#ifdef	__cplusplus
}
#endif

#endif	/* TIMERS_H */

