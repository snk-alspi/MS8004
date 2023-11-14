#ifndef __WIRED_HART_H_
#define __WIRED_HART_H_

#include "../Global/typedef.h"

/*************************************************************
2018 April 25:    
        HART TEST Specfications [Protocol R7.6] – Document Test01 – 
            “Slave Token Passing Data Link Layer Test Specification”.
        page 86 => chapter: 7.37 DLL037 Burst Mode Control (Command 109)

        "If the device supports more than one Data Link Layer, the operations of the token-passing Data Link Layer
        are verified. TDMA devices with maintenance ports are not required to support burst mode on the non-
        TDMA data link layer."
        => 
        On this WirelessHART field device "Token Passing Data Link Layer" will be implemented only 
            for slave device type because it will always be an HART SLAVE (without burst)
 ************************************************************/


/*************************************************************
Timings :
    bit time => 1 / 1200                = 0.000 833 333 s
    character time => 11 * (1/1200)     = 0.009 166 666 s
    gap error timeout ... must be less  < 0.009 166 666 s
    slave timeout => 28 * 11 * (1/1200) = 0.256 666 666 s

         // PIC SPECIFIC
 *      TIMER4 tick 1/31000   = 0.000 032 258 064 516 s
 *      Character in TIMER4 ticks =>  284.166 => 284 => 0.009 161 290 322 s
 *      Slave timeout in TIMER4 ticks => 7,956.6666 ticks => 7956 => 0.256 645 161 290 s
 * 
 *************************************************************/
// HART SLAVE TIMEOUTS
#define WIRED_HART_CHARACTER_TIME     284     // in TIMER4 ticks                             
#define WIRED_HART_STO               7956    // in TIMER4 ticks
#define WIRED_HART_GAP_TIMEOUT       284   // in TIMER4 ticks
     

/*************************************************************
Fields : 
    Delimiter       = 1
    Address         = 1 - 5
    Expansion Byte  = 0 - 3
    Command         = 1
    Byte Count      = 1
    Data            = 0 - 255
    Check           = 1
-----------------------------
    Max Len = 1+5+3+1+1+255+1 = 267
*************************************************************/
#define HART_BUFFER_MAX_LEN   268 

#define HART_FRAME_TYPE_MASK  (0x07)
#define HART_FRAME_STX        (0x02)
#define HART_FRAME_ACK        (0x06)
#define HART_FRAME_BACK       (0x01)

#define HART_DELIMITER_ADDRESS_MASK         0x80
#define HART_DELIMITER_EXPBYTES_MASK        0x60
#define HART_DELIMITER_PHY_TYPE_MASK        0x18

#define HART_DELIMITER_UNIQUE_ADDRESS       0x80        // 5 byte  Address
#define HART_DELIMITER_POOLING_ADDRESS      0x00        // 1 bytes Address

#define HART_FIELD_UNIQUE_ADDRESS_LEN       5           // 5 byte  Address
#define HART_FIELD_POOLING_ADDRESS_LEN      1           // 1 bytes Address
        
#define HART_FIELD_POOLING_ADDRESS_BITS     0x3F        // 

#define HART_FIELD_EXPDEVTYPE_MSBITS        0x3F        // 
#define HART_FIELD_EXPDEVTYPE_LSBITS        0xFF        // 


void Wired_HART_Init (void);
void Wired_HART_Task (void);

void Wired_HART_CarrierDetect (void);
uint8 Wired_HART_UART_RxByte (uint8 p_ucRxByte, uint8 p_ucStatus);

uint8 Wired_HART_StartTxBuffer (const uint8* p_pucTxBuffer);
void Wired_HART_UART_TxByte (void);


typedef enum 
{
    SET_UNIQUE_ADDRESS,         // Slave
    SET_POOLING_ADDRESS,        // Slave 
    SET_PREAMBLE,               // Slave
    SET_CAPTURE_ADDRESS,        // Slave - optional
    DISABLE_CAPTURE_ADDRESS,    // Slave - optional
    ENABLE_BURST_MODE,          // Slave - optional - only for burst slaves
    DISABLE_BURST_MODE          // Slave - optional - only for burst slaves
} __attribute__((packed)) T_WIRED_HART_SAP;


typedef enum 
{
    WIRED_HART_RX_WAIT = 0, 
    WIRED_HART_RX_PREAMBLE, 
    WIRED_HART_RX_DELIMITER, 
    WIRED_HART_RX_UNIQUE_ADDRESS, 
    WIRED_HART_RX_POLLING_ADDRESS, 
    WIRED_HART_RX_EXPBYTES,
    WIRED_HART_RX_COMMAND, 
    WIRED_HART_RX_COUNT, 
    WIRED_HART_RX_DATA, 
    WIRED_HART_RX_CHECK    
} __attribute__((packed)) T_WIRED_HART_RXSTATE;


typedef enum 
{
    WIRED_HART_RX_ERR_VERTICAL_PARITY          = 0x40,
    WIRED_HART_RX_ERR_OVERRUN                  = 0x20,
    WIRED_HART_RX_ERR_FRAMING                  = 0x10,
    WIRED_HART_RX_ERR_LONGITUDINAL_PARITY      = 0x08,
    WIRED_HART_RX_ERR_COMMUNICATION_FAILURE    = 0x04,
    WIRED_HART_RX_ERR_BUFFER_OVERFLOW          = 0x02
} __attribute__((packed)) T_WIRED_HART_RX_ERR;


typedef enum 
{
    WIRED_HART_TX_IDLE = 0,
    WIRED_HART_TX_PREAMBLE,
    WIRED_HART_TX_BUFFER,
    WIRED_HART_TX_STOP
} __attribute__((packed)) T_WIRED_HART_TXSTATE;


typedef struct 
{
    uint8 m_AddrBits:6;
    uint8 m_BurstMode:1;
    uint8 m_MasterAddress:1;
}  __attribute__((packed)) T_FIELD_ADDRESS_POLLING;


typedef struct 
{
    union {
        uint8 m_AddrBytes[5];
        struct {
            uint8 m_ExpDevTypeMSBits:6;
            uint8 m_BurstMode:1;    
            uint8 m_MasterAddress:1;
            uint8 m_ExpDevTypeLSBits;
            uint8 m_UniqueDevID[3];
        };
    };
} __attribute__((packed)) T_FIELD_ADDRESS_UNIQUE;
    

typedef struct 
{
    uint8 m_ucPreambleLength;
    uint8 m_ucLoopConfig;
    T_FIELD_ADDRESS_POLLING m_PollingAddr;
    T_FIELD_ADDRESS_UNIQUE m_UniqueAddr;
} __attribute__((packed)) T_WIRED_HART_DEVICE;


typedef struct 
{    
    uint16 m_OwnTokenTime; // in ALCK ticks
    uint8 m_OwnTokenFlag;
} __attribute__((packed)) T_WIRED_HART_TOKEN; 


typedef struct 
{    
    uint16 m_unTime; // in ALCK ticks
    uint8 m_ucResponse;
} __attribute__((packed)) T_WIRED_HART_NEED_RESPONSE; 

typedef struct 
{
    uint16 m_unRxTimeStamp; // in ALCK ticks
    uint16 m_unTxTimeStamp; // in ALCK ticks 
    uint8 m_RxMsgReceived;
    uint8 m_RxMsgErrors;
    uint8 m_unWaitTxEnd;
    uint8 m_RxDelimiter;
    uint8 m_RxMasterBurst;
    uint8 m_ucInit;
    T_WIRED_HART_DEVICE  m_Device;
    T_WIRED_HART_TOKEN   m_Token;
    T_WIRED_HART_NEED_RESPONSE m_sNeedHART;
    T_WIRED_HART_RXSTATE m_eRxState;
    T_WIRED_HART_TXSTATE m_eTxState;
} __attribute__((packed)) T_WIRED_HART_GLOBALS;


//extern volatile T_WIRED_HART_GLOBALS g_sWiredHart;

void AppHART_Tx_Response ( uint8 * p_pucData, uint16 p_unLen );

void AppHART_Cmd_0_Request ( void );
void AppHART_Cmd_0_Response ( uint8 * p_pucData, uint16 p_unLen );

uint8 Wired_HART_WritePollingAddress (uint8 p_ucAddress, uint8 p_ucLoopCurrentMode);
uint8 Wired_HART_WriteNumberOfResponsePreambles(uint8 p_ucNPreambles);

#endif /* __WIRED_HART_H_ */
