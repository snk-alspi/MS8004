#ifndef _MS_DATA_H_
#define _MS_DATA_H_

#include "../Global/typedef.h"
#include "../Lib/WirelessHART_Lib.h"

void PowerConsumptionTracking_DataInit(void);
void PowerConsumptionTracking_AddSleep(uint16 p_ucTmrTics);
void PowerConsumptionTrackingTask_AddExtraConsumption(uint16 p_unExtraCharge);
void PowerConsumptionTrackingTask_1sec(void);


typedef struct{
  union{
  	unsigned long lvalue;
  	unsigned char cvalue[4];
  }charge; //accumlated charge value in mC

  union{
  	unsigned long lvalue;
  	unsigned char cvalue[4];
  }uCcharge; //accumlated charge value in uC

  union{
  	unsigned long lvalue;
  	unsigned char cvalue[4];
  }time; //accumlated Time in seconds from the beginning

}MSSystemCharge; ///< Application Processor total charge.

extern MSSystemCharge g_stMSSystemCharge;

typedef struct{
  union{
  	unsigned long lvalue;
  	unsigned char cvalue[4];
  }charge; //accumlated charge value in mC
}RadioCharge; //Radio charge

extern RadioCharge g_stRadioCharge; ///< Radio Module current session charge.

typedef struct{
  union{
  	unsigned long lvalue;
  	unsigned char cvalue[4];
  }charge; //accumlated charge value in mC
}TotalRadioCharge; ///< Total Radio Module charge

extern TotalRadioCharge g_stTotalRadioCharge;

typedef struct {
    uint8 m_ucMonth;
    uint8 m_ucDay;    
    uint8 m_ucYear;
}COMMISSION_DATE_T;

/**
 * Non-volatile storage data signature
 */
typedef union
{
    uint8  m_Bytes[4];
    uint16 m_16[2];
    uint32 m_32;
} PV_SIGNATURE_T;

typedef struct {
    PV_SIGNATURE_T signature;
    unsigned char m_ucSensorType;
    unsigned char m_aucAccessPassword[8];
    unsigned char m_ucPasswordDisabled;
    unsigned char m_ucAlarmMode;
    unsigned char m_aucMSSerialNumber[14];
    COMMISSION_DATE_T g_stCommissionDate;
    unsigned int m_unMeasurementTimeInterval;    
}DEVICE_PROCESS_VARIABLES_PROVISION_T;

extern DEVICE_PROCESS_VARIABLES_PROVISION_T g_stDeviceProcessVariables;

//typedef enum {
//    PROBE_TYPE_WIRE = 0x01,
//    PROBE_TYPE_TUBE,
//    PROBE_TYPE_CYLINDER,
//    PROBE_TYPE_SMALL_FLUSH,
//    PROBE_TYPE_LARE_FLUSH,
//    PROBE_TYPE_SPIRAL,
//    PROBE_TYPE_STRIP,
//    PROBE_TYPE_MCE
//} __attribute__((packed)) PROBE_TYPES_T;

typedef enum {
    ALARM_MODE_NONE = 0x00,
    ALARM_MODE_HIGH_AUTO,
    ALARM_MODE_LOW_AUTO,
    ALARM_MODE_HIGH_MANUAL = 0x05,
    ALARM_MODE_LOW_MANUAL = 0x0A
} __attribute__((packed)) ALARM_MODES_T;

extern unsigned char g_ucPasswordMatch;
extern unsigned char g_ucMasterPasswordMatch;

//power consumption tracking registers

extern volatile unsigned int g_unActive;
extern volatile unsigned char SleepCntrSec; // counts the sleep time

extern unsigned int EnergyLeftDays;

extern PROCESS_CTRL_ANALOG_VAL_AND_STAT g_stDataToPublish;

extern unsigned long g_unMSTaskTmr;
#endif /* MS_DATA_H_ */
