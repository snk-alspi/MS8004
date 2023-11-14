#ifndef __WH_FULL_API_H
#define __WH_FULL_API_H


#include "../Global/typedef.h"
#include "../Platform/gpio.h"
#include "../Platform/uart.h"


#define API_VERSION             0x0001
#define API_MAX_BUFFER_SIZE     128

#define STX 0xF1
#define CHX 0xF2

#define API_CRC_SIZE          	        2
#define API_MAX_TX_QUEUE_SIZE           (5 * 160)

#define API_QUEUE_ADD_SUCCESS   		0
#define API_QUEUE_ADD_FAIL     	 		1

#define API_PERIOD_ASN_REQ              1200

#define TICS_24H_ASN                    (uint32)(24UL * 3600UL * 100UL) // 100*10 msec in 1 sec; 3600 sec in 1h; 24h in 1 day

#define FULL_API_COMM_WDT_TIMEOUT_SEC   600	// 600 seconds -> 10 minutes
#define API_RESPONSE_WAIT_TIMEOUT_MS    300 // by design WISA have an API TX opportunity at each 100 ms at this moment

// Status
#define ADVERTISEMENT_HEARD             0x0008
#define JOIN_REQUESTED                  0x0010
#define JOIN_RETRYING                   0x0020
#define AUTHENTICATED                   0x0080
#define NETWORK_JOINED                  0x0100
#define NEGOTIATING_NETWORK_PROPERTIES  0x0200

// Status bits for Device Status (Return Status message parameter)
#define DEV_STATUS_CEC_BUSY             0x01 // Bit 0
#define DEV_STATUS_HW_ERROR             0x02 // Bit 1
#define DEV_STATUS_NV_MEM_AT_DEFAULT    0x04 // Bit 2
#define DEV_STATUS_LOCK                 0x40 // Bit 5
#define DEV_STATUS_INIT_COMPLETE        0x80 // Bit 7

// Status bits for Network Status (Return Status message parameter
#define NWK_STATUS_RDY_TO_COMM          0x01 // Bit 0
#define NWK_STATUS_DISCOVERY            0x40 // Connection Status bits (Bits 6 & 7)
#define NWK_STATUS_JOINING              0x80 // Connection Status bits (Bits 6 & 7)
#define NWK_STATUS_OPERATIONAL          0xC0 // Connection Status bits (Bits 6 & 7)


typedef enum {
    API_UART_SPEED_9600   = 9600,
    API_UART_SPEED_19200  = 19200,
    API_UART_SPEED_38400  = 38400,
    API_UART_SPEED_115200 = 115200
} __attribute__((packed)) API_UART_SPEED_TYPES;

#define API_MIN_UART_SPEED          (API_UART_SPEED_9600)
#define API_MAX_UART_SPEED          (API_UART_SPEED_115200)
#define API_UART_SPEED              (API_UART_SPEED_38400) // WARNING ! Move this default into project compiler options

typedef enum {
    API_STATE_INIT                = 0,
    API_STATE_GET_INITIAL_INFO_OK = 1,
    API_STATE_NOTIFY_JOIN_OK      = 2,
    API_STATE_PUBLISH_OK          = 3
} __attribute__((packed)) API_STATE;

typedef enum {
    // Success
    API_SUCCESS = 0,
    // Errors
    API_ERR_API_NOT_INITIALIZED,
    API_ERR_API_FAILED,
    API_ERR_LINK_UNAVAILABLE,
    API_ERR_UNSUPPORTED_DATA_SIZE,
    API_ERR_UNKNOWN_DATA_TYPE,
    API_ERR_QUEUE_FULL,
    API_ERR_QUEUE_ADD_FAIL,
    API_ERR_PARAM
} __attribute__((packed))  API_ERROR_CODE_TYPES;

// MESSAGE CLASS API
typedef enum {
    CLS_DATA_PASSTHROUGH    = 1,
    CLS_POWER_MANAGEMENT    = 2,
    CLS_STACK_SPECIFIC      = 3,
    CLS_API_COMMANDS        = 4,
    CLS_ACK                 = 5,
    CLS_NACK                = 6,
    CLS_USER_BOARD          = 7
} __attribute__((packed)) MSG_CLASS_TYPES;


// 1 = DATA_PASSTHROUGH
typedef enum {
    DPST_WRITE_DATA_REQUEST = 0x01,
    DPST_READ_DATA_REQUEST,
    DPST_READ_DATA_RESPONSE,
    DPST_RADIO_DATA = 0xAA,
    DPST_APPLICATION_DATA = 0x55,
    DPST_FIRMWARE_START_UPGRADE = 0x3C,
    DPST_FIRMWARE_PAGE = 0xC3,
    DPST_FIRMWARE_END_UPGRADE = 0xCC,
    DPST_FIRMWARE_CANCEL = 0x99
} __attribute__((packed))  DATA_PASSTHROUGH_TYPES;

// 2 = POWER_MANAGEMENT
typedef enum {
    PWR_STATUS_LINE = 0,    // line powered
    PWR_STATUS_1    = 1,    // battery powered, greater than 75% remaining capacity
    PWR_STATUS_2    = 2,    // battery powered, between 25% and 75% remaining capacity
    PWR_STATUS_3    = 3     // battery powered, less than 25% remaining capacity
} __attribute__((packed))  POWER_STATUS_TYPES;

// 3 = STACK_SPECIFIC
typedef enum {
    HART_GET_JOIN_STATUS        = 0,
    HART_GET_ASN                = 1,
    HART_POWER_LEVEL            = 2,                                        // to be defined
    HART_SERVICE_REQUEST        = 3,            // WHART Service
    HART_SERVICE_DELETE         = 4,            // WHART Service
    HART_NOTIFY_JOIN            = 5,            // WHART Notifications      // to be defined
    HART_GET_INITIAL_INFO       = 6,            // WHART Notifications
    HART_RADIO_RESET            = 7,            // WHART Notifications
    HART_WRITE_ADDITIONAL_DEVICE_STATUS = 8,    // WHART Notifications
    HART_CLEAR_MSA              = 9,            // WHART Notifications
    HART_UPDATE_BURST_PERIOD    = 10,           // WHART Notifications
    HART_GET_CFG_CHG_COUNTER    = 11,           // WHART Notifications 
    HART_ASYNC_OPERATION        = 12,           // WHART Notifications // 0x00 0x00 => Long tag has been modified
    HART_FLUSH_BURST            = 13,            // WHART Notifications
    HART_BLOCK_TRANSFER         = 14
} __attribute__((packed))  STACK_SPECIFIC_TYPES;

// 4 = API_COMMANDS
typedef enum {
    API_CMD_HW_PLATFORM       = 0,
    API_CMD_PROTOCOL_VERSION  = 1,
    API_CMD_MAX_BUFFER_SIZE   = 2,
    API_CMD_MAX_SPI_SPEED     = 3,
    API_CMD_UPDATE_SPI_SPEED  = 4,
    API_CMD_MAX_UART_SPEED    = 5,
    API_CMD_UPDATE_UART_SPEED = 6,
    API_CMD_HEARTBEAT_FREQ    = 7,
    API_CMD_HEARTBEAT         = 8,
    API_CMD_ENERGY_LEFT       = 9,
    API_CMD_ENERGY_MODEM_REQ  = 10,
    API_RF_TEST_MODE          = 11,
    API_SET_COUNTRY_CODE      = 12,
    API_RF_RX_MODE            = 13
} __attribute__((packed)) API_COMMAND_TYPES;

// 5 = API_ACK
typedef enum {
    ACK_DATA_OK                 = 1,
    ACK_MESSAGE_SENT_VIA_RF     = 2,
    ACK_API_CHANGE_ACCEPTED     = 3,
    ACK_FIRMWARE                = 4
} __attribute__((packed))  ACK_MESSAGES_TYPES;


// 6 = API_NACK
typedef enum {
    NACK_CRC_FAIL             = 1,
    NACK_DATA_OVERRUN         = 2,
    NACK_PACKET_INCOMPLETE    = 3,
    NACK_PARITY_ERROR         = 4,
    NACK_API_NOT_INITIALIZED  = 5,
    NACK_API_COMMAND_ERROR    = 6,
    NACK_API_BUSY             = 7,
    NACK_API_ERROR            = 8,
    NACK_STACK_ERROR          = 9,
    NACK_UNSUPPORTED_FEATURE  = 10,
    NACK_FIRMWARE_UPDATE_ERR  = 11
} __attribute__((packed))  NACK_MESSAGE_TYPES;


// 7 = USER_BOARD_COMMANDS
typedef enum {
    DAQ_FW_WIRELESS     = 0,
    DAQ_BURST_PIPE_0    = 1,
    DAQ_BURST_PIPE_1    = 2,
    DAQ_BURST_PIPE_2    = 3,
    DAQ_FW_WIRED        = 4,
    DAQ_FW_HART_MODEM   = 5
} __attribute__((packed))  USER_BOARD_TYPES;


#define API_MSG_HEADER_SIZE 4

typedef struct {

    struct {
        uint8 m_bReserved : 2,
            m_bHartLoop : 1,
            m_bIsRsp : 1,
            m_eMsgClass : 4;
    } MsgHeader;
    uint8 m_ucMsgType;
    uint8 m_ucMsgID;
    uint8 m_ucMsgDataLen;
} __attribute__((packed)) API_MSG_HEADER;



extern API_STATE g_ucAppProcessorStatus;	///< Variable used for determining Application Processor Status.
extern uint64 g_ullCrtASN;					///< Global variable used for obtaining a 24 hours period.
extern volatile uint32 g_ulMonotonic;		///< 4 bytes monotonic variable.

extern volatile uint8 g_ucMsgID;

extern uint8 g_unInterfaceType;

extern uint16 g_unSTXTimestampMsec;         ///< Millisecond time stamp of the last received STX.
extern uint16 g_unASNInterogationTimer;     ///< Units of 250 milliseconds.


void API_Perform250MsecTasks( void );
void API_Rx_Handler( void );
void API_Tx_Handler( uint8 p_ucSource );

#define API_Tx_IsEmpty() UART1_TxIsEmpty()

API_ERROR_CODE_TYPES API_SendMessage( MSG_CLASS_TYPES MsgClass,
				     uint8 MsgType,
				     uint8 RequestResponse,
				     uint8 MsgId,
				     uint8 BuffSize,
				     uint8 *MsgBuff,
				     uint8 HartLoop);
#define API_MSGTYPE_REQUEST       0
#define API_MSGTYPE_RESPONSE      1
#define API_SendRequest(MsgClass, MsgType, MsgId, DataSize, Data, HartLoop)     API_SendMessage(MsgClass, MsgType, API_MSGTYPE_REQUEST, MsgId, DataSize, Data, HartLoop)
#define API_SendResponse(MsgClass, MsgType, MsgId, DataSize, Data, HartLoop)    API_SendMessage(MsgClass, MsgType, API_MSGTYPE_RESPONSE, MsgId, DataSize, Data, HartLoop)

uint8 API_OnRcvAPISpecific( API_MSG_HEADER *p_pMsgFrame );
uint8 API_OnRcvBoardCommand( API_MSG_HEADER *p_pMsgFrame );
uint8 API_OnRcvStackSpecific( API_MSG_HEADER *p_pMsgFrame );

void API_ActivateWKUSignal(uint8 p_unEventTmr);
void API_InactivateWKUSignal(void);

extern volatile uint16 g_unWKUSignalEventTmr;

#endif // __WH_FULL_API_H
