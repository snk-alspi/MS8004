#include "xc.h"
#include "stdio.h"
#include <string.h>
#include "libpic30.h"
#include "MS_Process.h"
#include "MS_Data.h"
#include "../Platform/measurement.h"
#include "../Platform/nvm.h"
#include "../Global/provision.h"
#include "../WirelessHART_App_Task/WirelessHART_App_Task.h"
#include "../WirelessHART_Full_API/wh_full_api.h"
#include "../WirelessHART_Full_API/wh_full_api_queue.h"
#include "../Platform/gpio.h"
//#include "../Platform/flashFunctions.h"
#include "../Platform/GlobalVariables.h"
//#include "../Platform/DEE Emulation 16-bit.h"

unsigned int EnergyLeftDays = LIFE_EXPECTANCY_MAX_DAYS; /** Variable used for storing the remaining number of days of battery life */ // 5 years or 5*365 days

MSSystemCharge g_stMSSystemCharge; ///< Variable used for storing Application Processor's power consumption.
RadioCharge g_stRadioCharge; ///< Variable used for storing Radio Module's current session power consumption.
TotalRadioCharge g_stTotalRadioCharge; ///< Variable used for storing Radio Module's total power consumption.
DEVICE_PROCESS_VARIABLES_PROVISION_T g_stDeviceProcessVariables;

unsigned char g_ucPasswordMatch;
unsigned char g_ucMasterPasswordMatch;
static const char defaultCommissionDate[] =  {01,01,70};




void UpdateMetalLossValue(void)
{    
    //float fProbeValue = measurement(1); 
    
    g_stDataToPublish.fpValue = mat_thick_raw_0; //g_stProv.m_astDeviceVars[PV_IDX].m_ucUnit == UnitsCodes_MilsPerYear ? fProbeValue : fProbeValue*25.4;     
    
    g_stDataToPublish.ucStatus = 0;  /* Status of the process value */

    
    
//     g_stDataToPublish.fpValue = g_stProv.m_astDeviceVars[PV_IDX].m_ucUnit == UnitsCodes_MilsPerYear ? fProbeValue : fProbeValue*25.4;     
//    
//    g_stDataToPublish.ucStatus = 0;  /* Status of the process value */    
}

//END OF JUST FOR PROOF OF CONCEPT DEMO
///////////////////////////////////////////////////////////////

/**
 * @brief Resets battery life. Power consumption measurements from Application Processor and Radio Module are set to 0
 * and battery life is reset by calculating the life expectancy with the new values.
 */
uint8 ResetBatteryLife() {
    uint8 payload = 1;
    uint8 res;

    // Notify stack to reset the counters (send 1 byte with value 1)
    res = (uint8) API_SendRequest(CLS_API_COMMANDS, API_CMD_ENERGY_MODEM_REQ, 0, 1, &payload, 0);
    if (res) {
        return res;
    }
    // Reset values
    g_stProv.m_stMSSystemCharge.charge.lvalue = 0;
    g_stProv.m_stMSSystemCharge.uCcharge.lvalue = 0; //micro columb charge
    g_stProv.m_stMSSystemCharge.time.lvalue = 0;
    g_stProv.m_stRadioCharge.charge.lvalue = 0;
    g_stProv.m_stTotalRadioCharge.charge.lvalue = 0;

    // Update NVM
    FLASH_UPDATE_NEEDED_WITH_STAT();

    CalculateLifeExpectancy();
    return 0;
}

/**
 * @brief Adds the radio charge at power up. Then, the radio charge is set to 0.
 */
void ResetRadioCharge(void) {
    g_stProv.m_stMSSystemCharge.charge.lvalue += g_stProv.m_stRadioCharge.charge.lvalue / 2;
    g_stProv.m_stTotalRadioCharge.charge.lvalue += g_stProv.m_stRadioCharge.charge.lvalue;
    g_stProv.m_stRadioCharge.charge.lvalue = 0;
   
    // update data in NVM
    FLASH_UPDATE_NEEDED_WITH_STAT();
}

/**
 * @brief Calculates the battery life expectancy based on the power consumption tracking data.
 * 
 * BATTERY_CAPACITY (miliCoulombs) = capacity of the battery defined in MS_Process.h
 * TotalCharge (miliCoulombs) = total charge consumed by the device since battery plugged in.
 * ActiveTotalSeconds (seconds) = total up time in seconds since battery plugged in.
 *
 * BATTERY_CAPACITY (mC) .................... EstimatedLifeSeconds (s)
 * TotalCharge (mC) ......................... ActiveTotalSeconds (s)
 *
 * EstimatedLife(in days) = BATTERY_CAPACITY * ActiveTotalSeconds / TotalCharge / (24*3600)
 *
 */
void CalculateLifeExpectancy() {
    uint32 ulTotalCharge;
    uint32 ulActiveTotalSeconds;

    ulActiveTotalSeconds = g_stProv.m_stMSSystemCharge.time.lvalue;
    ulTotalCharge = g_stProv.m_stMSSystemCharge.charge.lvalue; // load MS System charge
    ulTotalCharge += g_stProv.m_stRadioCharge.charge.lvalue / 2; // add Radio charge => RADIO IS BASED OF 3.3V SOURCE => for 7.2V source it is about half of the charge

    if (ulTotalCharge && ulActiveTotalSeconds) // can estimate only if have a minimum activity
    {
        float fEstimatedLife;
        
        fEstimatedLife = ((float) BATTERY_CAPACITY)/(24*3600UL);
        fEstimatedLife *= (float) ulActiveTotalSeconds;
        fEstimatedLife /= (float) ulTotalCharge;

        if (fEstimatedLife > (float)LIFE_EXPECTANCY_MAX_DAYS)
        {
            fEstimatedLife = LIFE_EXPECTANCY_MAX_DAYS;
        }

        EnergyLeftDays = (unsigned int) (fEstimatedLife);
    }
}

/**
* @brief  This function checks if data stored in the NVM is valid or not. Validity is given by the NVM signature stored in the first page.
*/
Boolean PVSignatureAreaGood()
{
  uint8 aucRead[4] = {0};
  uint32 MEM_valid = 0;

  memcpy(aucRead, g_aucPVFlashCopy, 4);
  MEM_valid |= aucRead[3];
  MEM_valid <<= 8;
  MEM_valid |= aucRead[2];
  MEM_valid <<= 8;
  MEM_valid |= aucRead[1];
  MEM_valid <<= 8;
  MEM_valid |= aucRead[0];  

  return (Boolean)(MEM_valid == MEM_SIGNATURE);
}

void NVM_ReadPVData()
{
    
    NVMReadPage(NVM_DEVICE_PV_START_ADDRESS, g_aucPVFlashCopy);
    
    Boolean bArea1Signature = PVSignatureAreaGood();    

    if (bArea1Signature == 0x01)
    {
        memcpy(&g_stDeviceProcessVariables, g_aucPVFlashCopy, sizeof(g_stDeviceProcessVariables));
        return;
    }

    NVM_LoadPVDefaultData();
}

void NVM_LoadPVDefaultData()
{
    PV_SIGNATURE_T sGoodSignature;
    sGoodSignature.m_32 = MEM_SIGNATURE;
    
    memcpy(g_stDeviceProcessVariables.m_aucAccessPassword, ACCESS_PASSWORD_DEFAULT_VALUE, sizeof(g_stDeviceProcessVariables.m_aucAccessPassword));
    g_stDeviceProcessVariables.m_ucAlarmMode = ALARM_MODE_DEFAULT_VALUE;
    memcpy(g_stDeviceProcessVariables.m_aucMSSerialNumber, MS_SERIAL_NUMBER_DEFAULT_VALUE, sizeof(g_stDeviceProcessVariables.m_aucMSSerialNumber));
    memcpy(&(g_stDeviceProcessVariables.g_stCommissionDate), defaultCommissionDate, sizeof(defaultCommissionDate));
    g_stDeviceProcessVariables.m_unMeasurementTimeInterval = MEASUREMENT_TIME_INTERVAL_HOURS_DEFAULT_VALUE;
    g_stDeviceProcessVariables.m_ucSensorType = DEFAULT_SENSOR_TYPE;
    g_unMSTaskTmr = g_stDeviceProcessVariables.m_unMeasurementTimeInterval * SECONDS_IN_AN_HOUR;
    g_stDeviceProcessVariables.m_ucPasswordDisabled = 0; 
    g_stDeviceProcessVariables.signature = sGoodSignature;
    DEVICE_PROCESS_VARIABLES_FLASH_UPDATE_NEEDED();
}

uint8 SetSensorType(uint8 p_ucSensorTypeValue)
{
    //set probe type
    g_stDeviceProcessVariables.m_ucSensorType = p_ucSensorTypeValue;
    // update nvm
    DEVICE_PROCESS_VARIABLES_FLASH_UPDATE_NEEDED();
    
    return 0;
}

uint8 SetAccessPassword(const uint8* p_pPassword, uint8 p_ucLen)
{
    if(!memcmp(p_pPassword, &g_stDeviceProcessVariables.m_aucAccessPassword, sizeof(g_stDeviceProcessVariables.m_aucAccessPassword)))
    {
        g_ucPasswordMatch = 1;
    }
    
    if(!memcmp(p_pPassword, MASTER_PASSWORD, p_ucLen))
    {
        g_ucMasterPasswordMatch = 1;
        g_ucPasswordMatch = 1;
    }
    return 0;
}

uint8 UpdateAccessPassword(const uint8 *p_pPassword, uint8 p_ucLen)
{
    if(memcmp(g_stDeviceProcessVariables.m_aucAccessPassword, p_pPassword, sizeof(g_stDeviceProcessVariables.m_aucAccessPassword)))
    {
        memcpy((uint8*)&g_stDeviceProcessVariables.m_aucAccessPassword, p_pPassword, p_ucLen);
        g_ucPasswordMatch = 0;
        DEVICE_PROCESS_VARIABLES_FLASH_UPDATE_NEEDED();
    }
    return 0;
}

uint8 SetPasswordEnableStatus(uint8 p_ucStatus)
{
    // set password state
    g_stDeviceProcessVariables.m_ucPasswordDisabled = p_ucStatus;
    if(p_ucStatus)
    {
        g_ucPasswordMatch = 0;
    }
    //update nvm
    DEVICE_PROCESS_VARIABLES_FLASH_UPDATE_NEEDED();
    return 0;
}

uint8 SetAlarmMode(uint8 p_ucAlarmMode)
{
    // set alarm mode
    g_stDeviceProcessVariables.m_ucAlarmMode = p_ucAlarmMode;
    // update nvm
    DEVICE_PROCESS_VARIABLES_FLASH_UPDATE_NEEDED();
    return 0;
}

uint8 SetSerialNumber(const uint8* p_pSerialNumber, uint8 p_ucLen)
{
    // update Serial Number
    memcpy((uint8*)&g_stDeviceProcessVariables.m_aucMSSerialNumber, p_pSerialNumber, p_ucLen);
    //update nvm
    DEVICE_PROCESS_VARIABLES_FLASH_UPDATE_NEEDED();
    return 0;
}

uint8 SetCommissionDate(const uint8* p_pCommissionDate, uint8 p_ucLen)
{
    // update Commission Date
    memcpy((uint8*)&(g_stDeviceProcessVariables.g_stCommissionDate), p_pCommissionDate, p_ucLen);
    //update nvm
    DEVICE_PROCESS_VARIABLES_FLASH_UPDATE_NEEDED();
    return 0;
}

uint8 SetMeasurementTimeInterval(uint16 p_unMeasurementTimeInterval)
{
    // update measurement time interval
    g_stDeviceProcessVariables.m_unMeasurementTimeInterval = p_unMeasurementTimeInterval;
    //update MS Task timeout for probe interrogation
    g_unMSTaskTmr = g_stDeviceProcessVariables.m_unMeasurementTimeInterval * SECONDS_IN_AN_HOUR; // converting hours into seconds;
    //update nvm data
    DEVICE_PROCESS_VARIABLES_FLASH_UPDATE_NEEDED();
    return 0;
}

void MS_Initialize(void) {

    UART2_Init(UART_BAUD_38400);// Used for Measurement Board Communication
    UART2_Disable();
    GPIO_UART2_Disable();
    
    g_unMSTaskTmr = 3600; //Needs to be changed on release version to 12 Hours.
    // g_stDeviceProcessVariables.m_unMeasurementTimeInterval * SECONDS_IN_AN_HOUR; 
    mes_stat = MES_STOPPED;
    
 }

void MS_PeriodicTasks_1sec(void)

{
   
     if (g_ucMeasurementInProgress)
    {
         timeout++;
        excute_mesurement();
    }else if(m_ucHartReqInProgress )
    {
        timeout++;
        switch(m_ucHartReqInProgress){
         
            case 1 :
                Write_UTM_Parameter();
                break;
            case 2:
                Read_Mes_Parameter();
                break;
            case 4:
                Write_UTT_Parameter();
                break;
            case 5:
                Write_UTG_Parameter();
                break;    
            default:
                break;
                
                
        }
    }
    if(g_unMSTaskTmr > 0)
    {
        g_unMSTaskTmr--;
    }
    else if (mes_stat == MES_STOPPED && !m_ucHartReqInProgress)
    {
        g_unMSTaskTmr = 3600;; //Need to be changed on final version
//        g_stDeviceProcessVariables.m_unMeasurementTimeInterval * SECONDS_IN_AN_HOUR;
         
         TRISBbits.TRISB13 = 0;
         LATBbits.LATB13 = 0;
         mes_stat = MES_START; remeasure = 0; ch=0;
         excute_mesurement();
    }
}




