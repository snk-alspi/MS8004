#ifndef __WIRELESS_HART_LIB_H_
#define __WIRELESS_HART_LIB_H_

#include "../Global/typedef.h"
#include "../Platform/gpio.h"

#define API_STATUS_READY_FOR_NEW_PACKET       0 ///< API Communication Status Code for receiving a new packet
#define API_STATUS_MESSAGE_WAIT_PROCESSING    1 ///< API Communication Status Code for processing a packet
#define API_STATUS_COMMUNICATION_TIMEOUT      2 ///< API Communication Timeout Code
#define API_STATUS_HW_INTERFACE_BLOCKED       3 ///< API Communication Interface Blocked Code

///@{
/** WirelessHART Lib return codes */
typedef enum
{
    RC_SUCCESS           	= 0, /*!< 0 */
    RC_ALLOWED		        = 1, /*!< 1 */
    RC_INVALID_ARG       	= 2, /*!< 2 */
    RC_FAILED            	= 3, /*!< 3 */
    RC_INSUFF_RESOURCES  	= 4, /*!< 4 */
    RC_DUPLICATE_OID     	= 5, /*!< 5 */
    RC_INVALID_RANGE     	= 6, /*!< 6 */
    RC_NOT_IMPLEMENTED   	= 7, /*!< 7 */
    RC_NOT_ALLOWED	     	= 8, /*!< 8 */
    RC_CONFIG_MISMATCH 	    = 9, /*!< 9 */
    RC_INSUFFICIENT_DATA	= 10, /*!< 10 */
    RC_TIMEOUT		        = 11, /*!< 11 */
    RC_INVALID_UAP_ID	    = 12, /*!< 12 */
    RC_INVALID_OBJ_ID	    = 13, /*!< 13 */
    RC_SUBSCR_NO_EXCEEDED   = 14, /*!< 14 */
    RC_INVALID_SIZE	        = 15, /*!< 15 */
    RC_OP_ACCEPTED          = 16, /*!< 16 */
    RC_INVALID_DATA         = 17, /*!< 17 */
    RC_NOT_CONFIGURED       = 18 /*!< 18 */
} __attribute__((packed)) LIB_RET_CODE;
///@}

///@{
/** Task periodicity enumeration  */
typedef enum
{
    LIB_TASK_PERIODICITY_10MS   = 0, /*!< 10 milliseconds period */
    LIB_TASK_PERIODICITY_25MS   = 1, /*!< 25 milliseconds period */
    LIB_TASK_PERIODICITY_100MS  = 2  /*!< 100 milliseconds period */
} __attribute__((packed)) LIB_TASK_PERIODICITY;
///@}

typedef void (* GENERIC_CBACK_FCT)( void );
    
uint16 LIB_CalculateCrc( const uint8 *Buffer, uint16 BufferSize );
void LIB_MainTask();

void LIB_PeriodicTask_125msec();
void LIB_PeriodicTask_250msec();
void LIB_PeriodicTask_1sec();

LIB_RET_CODE LIB_TaskingInit( LIB_TASK_PERIODICITY p_ucLibTaskPeriod, uint8* p_pucUserTimerEventCnt );
void LIB_RadioAPI_UART_ReceiveByte( uint8 p_ucRxByte );
void LIB_RadioAPI_UART_SendByte(void);
void LIB_RadioAPI_PreparePacketRx(void);
void LIB_RadioAPI_CkRtsTimeout(void);
LIB_RET_CODE LIB_RadioAPI_Init_HW_Interface(void);

uint16 LIB_GoToLowPower ( void );
void LIB_OnGpioIrq ();


void API_Read_Cmd_0(void);

typedef struct
{
  fp32    fpUpperRange;           /**< Engineering units at 100 % (the upper range of the associated object parameter) */
  fp32    fpLowerRange;           /**< Engineering units at   0 % (the lower range of the associated object parameter) */
  uint16  unUnitsIndex;           /**<Indicates the units index, as defined in Foundation Fieldbus, of the engineering unit
                                       of the process value (See Foundation Fieldbus Technical Device 16 (document TN-016,
                                       rev 3.0 dated Nov 30, 2005) for definition of the unit indices used by the standard.) */
  uint8   ucDecPointLoc;          /**< Decimal point location, representing the number of places to the right of the decimal
                                       point, i.e., the significance of fractional part of the associated value). Useful as it
                                       indicates the useful number of digits for the display of such as the HOST system or
                                       configuration tool. */

} PROCESS_CONTROL_SCALE;

typedef struct
{
  fp32  fpValue;                    /**<The measurement variable in engineering units of the sensor. Engineering units
                                         are specified by PROCESS_CONTROL_SCALE::unUnitsIndex.*/
  uint8 ucStatus;                   /**<The status of the analog process value.
                                         There are four states of quality of the data, an enumerated set of sixteen
                                         sub-status values for each quality, and four states of the limits placed on the data. Limit information is generated for all status attributes of all parameters having status.
                                         The status is represented as an ISA100 Status Bit String.
                                         The bit string construction details are detailed in [reference Table 299, of ISA-100.11a-2011 specification].
                                         The status is decided based on the value of PROCESS_CONTROL_MODE::ucActual, internal device
                                         status, such as sensor failure and alarm (if any enabled) limit values.*/
} PROCESS_CTRL_ANALOG_VAL_AND_STAT;

#endif // __WIRELESS_HART_LIB_H_
