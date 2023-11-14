#include "MS_Data.h"
#include "MS_Process.h"
#include "../Global/provision.h"
#include "../Platform/timers.h" 
#include "../WirelessHART_Full_API/wh_full_api.h"

//power consumption tracking registers
uint16 g_unSleepTicksCounter;
uint32 g_unActiveTicksCounter;

static uint16 s_unStartTicksCounter;
static uint8  s_ucConsumptionUpdateInterval;
static uint8  s_ucModemReadInterval;

PROCESS_CTRL_ANALOG_VAL_AND_STAT g_stDataToPublish      = {.fpValue = 0, .ucStatus = 0};

unsigned long g_unMSTaskTmr;

/**
 * @brief initializes the variables used for power consumption measurement.
 */
void PowerConsumptionTracking_DataInit(void)
{
    
  g_unSleepTicksCounter=0;
  g_unActiveTicksCounter=0;
  
  s_unStartTicksCounter = READ_FREE_RUNNING_TIMER();
  
  s_ucConsumptionUpdateInterval = 0;
  s_ucModemReadInterval = 0;
}

void PowerConsumptionTracking_AddSleep(uint16 p_ucTmrTics)
{
    g_unSleepTicksCounter += p_ucTmrTics;
}
void PowerConsumptionTrackingTask_AddExtraConsumption(uint16 p_unExtraCharge)
{
    g_stProv.m_stMSSystemCharge.charge.lvalue += p_unExtraCharge;
}

void PowerConsumptionTrackingTask_1sec(void)
{
    uint16 unTmr = READ_FREE_RUNNING_TIMER();
    
    g_unActiveTicksCounter += (uint16)(unTmr - s_unStartTicksCounter);
    s_unStartTicksCounter = unTmr;
    
    g_unActiveTicksCounter -= g_unSleepTicksCounter;
    g_unSleepTicksCounter = 0;
        
    if (++s_ucConsumptionUpdateInterval >= 100) // at every 100 seconds
    {
        s_ucConsumptionUpdateInterval = 0;
        g_stProv.m_stMSSystemCharge.time.lvalue += 100; // update charge interval
        g_stProv.m_stMSSystemCharge.charge.lvalue += SLEEP_CHARGE_100_SECONDS; // add previously measured device sleep charge/100 seconds        
        if (g_unActiveTicksCounter >= 31000)
        {
            unTmr = (g_unActiveTicksCounter/31000); // active in seconds
            g_stProv.m_stMSSystemCharge.charge.lvalue += unTmr * ACTIVE_CHARGE;
            g_unActiveTicksCounter -= (uint32)unTmr*31000;
        }
        
        if (++s_ucModemReadInterval >= 36) // read modem energy at 36 x 100 = 3600 seconds (every hour)
        {
            s_ucModemReadInterval = 0;
            API_SendRequest(CLS_API_COMMANDS, API_CMD_ENERGY_MODEM_REQ, 0, 0, NULL, 0);
        }
    }
}