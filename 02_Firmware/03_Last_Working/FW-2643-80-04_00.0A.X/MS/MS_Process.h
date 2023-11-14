#ifndef _MS_PROCESS_H_
#define _MS_PROCESS_H_

#include "../Platform/interrupt_callbacks.h"
#include "../Global/typedef.h"

/**************************************************************************************************
                                            CONSTANTS
**************************************************************************************************/
// In Sleep = Full Instrument = 130 uA at 7.2 V, 115 uA application board + hart modem
#define SLEEP_CHARGE_100_SECONDS        ((unsigned long)12)             // sleep current = 0.115 mA => for every 100 seconds 12mA
#define ACTIVE_CHARGE                   ((unsigned long) 2)             // Controller when active executing code draws about 2 mA 
#define MEASUREMENT_CHARGE              ((unsigned long)90)             // Controller when active executing code draws about extra 90 mA x  according with datasheet => 90mA

// we are using 10% derated values
#define BATTERY_CAPACITY                 ((unsigned long) 61560000)     // mColoumbs = 19A * 3600sec * 1000 (mili)
                                                                        // 68,400,000 * 0.9 = 61,560,000 = derated with 10%

#define LIFE_EXPECTANCY_MAX_DAYS        (7*365)                         // 7 years worth of days - (365*7) 


#define ACCESS_PASSWORD_DEFAULT_VALUE                   "managers"
#define MASTER_PASSWORD                                 "425-3555"
#define ALARM_MODE_DEFAULT_VALUE                        0
#define MS_SERIAL_NUMBER_DEFAULT_VALUE                  "##############"
#define MEASUREMENT_TIME_INTERVAL_HOURS_DEFAULT_VALUE   12
#define DEFAULT_SENSOR_TYPE                              1
#define SECONDS_IN_AN_HOUR                              3600UL

extern volatile unsigned char g_ucMeasurementInProgress;

void MS_PeriodicTasks_125msec(void);
void MS_PeriodicTasks_1sec(void);

void CalculateLifeExpectancy();
uint8 ResetBatteryLife();
void ResetRadioCharge(void);
void MS_Initialize(void);
Boolean PVSignatureAreaGood();
void NVM_ReadPVData(void);
void NVM_LoadPVDefaultData(void);
uint8 SetProbeType(uint8 p_ucProbeTypeValue);
uint8 SetAccessPassword(const uint8* p_pPassword, uint8 p_ucLen);
uint8 UpdateAccessPassword(const uint8 *p_pPassword, uint8 p_ucLen);
uint8 SetPasswordEnableStatus(uint8 p_ucStatus);
uint8 SetAlarmMode(uint8 p_ucAlarmMode);
uint8 SetSerialNumber(const uint8* p_pSerialNumber, uint8 p_ucLen);
uint8 SetCommissionDate(const uint8* p_pCommissionDate, uint8 p_ucLen);
uint8 SetMeasurementTimeInterval(uint16 p_unMeasurementTimeInterval);

void UpdateMetalLossValue(void);



#endif /* _MS_PROCESS_H_ */
