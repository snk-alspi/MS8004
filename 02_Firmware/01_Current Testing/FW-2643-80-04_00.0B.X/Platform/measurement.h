#ifndef _MEASUREMENT_H_
#define	_MEASUREMENT_H_

#include "xc.h"


#endif	/* MEASUREMENT_H */

// MB STATUS

void excute_mesurement(void);

#define MB_MES_START            0x00
#define MB_MES_INPROGRESS       0x01
#define MB_MES_ONHOLD           0x02
#define MB_MES_SET_PROBE        0x03
#define MB_MES_FIRE_COIL        0x04
#define MB_MES_WAIT_TO_FINISH   0x05
#define MB_MES_READ_RESULT_1    0x06
#define MB_MES_READ_RESULT_2    0x07
#define MB_MES_READ_RESULT_3    0x08
#define MB_MES_READ_RESULT_4    0x09
#define MB_MES_READ_RESULT_5    0x0A
#define MB_MES_FINISH           0x0B

    #define MES_START       0x0C
    #define MB_MES_S2       0x11
    #define MB_MES_S3       0x12
  #define MB_MES_S4       0x13
    #define MES_WAIT_START  0x0D
    #define MES_INPROGRESS  0x0E
    #define MES_FINISHED    0x0F 
    #define MES_STOPPED     0x10


// MB COMMUNICATION STATUS
#define MB_COM_IDLE             0x20

#define MB_TX_START             0x21
#define MB_TX_INPROGRESS        0x22
#define MB_TX_FINISHED          0x23

#define MB_RX_START             0x26
#define MB_RX_INPROGRESS        0x27
#define MB_FRAME_OVER           0x28
#define MB_RX_FINSHED           0x29

// Modbus error
#define MB_ERROR_NOREPLY        0x30
#define MB_ERROR_PARITY         0x31
#define MB_ERROR_CRC            0x32


// Measurement Parameters cases
#define MB_PARA_START           0x01
#define MB_PARA_VAL_01          0x02
#define MB_PARA_VAL_02          0x03
#define MB_PARA_VAL_03          0x04
#define MB_PARA_VAL_04          0x05
#define MB_PARA_VAL_05          0x06
#define MB_PARA_VAL_06          0x07
#define MB_PARA_VAL_07          0x08
#define MB_PARA_VAL_08          0x09
#define MB_PARA_VAL_09          0x0A
#define MB_PARA_VAL_10          0x0B
#define MB_PARA_VAL_11          0x0C
#define MB_PARA_VAL_12          0x0D
#define MB_PARA_VAL_13          0x0E



#define MB_PARA_FINISH          0xA1
#define MB_PARA_STOPPED         0xA2

#define MB_TCP_START           0x01
#define MB_TCP_VAL_01          0x02
#define MB_TCP_VAL_02          0x03
#define MB_TCP_VAL_03          0x04
#define MB_TCP_VAL_04          0x05
#define MB_TCP_VAL_05          0x06
#define MB_TCP_VAL_06          0x07
#define MB_TCP_VAL_07          0x08
#define MB_TCP_FINISH          0xA1
#define MB_TCP_STOPPED         0xA2



#define MB_CMD_SET_PROBE        0x01
#define MB_CMD_FIRE             0x02
#define MB_CMD_DATA_RDY         0x03
#define MB_CMD_REQ_DATA1        0x04
#define MB_CMD_REQ_DATA2        0x05
#define MB_CMD_REQ_DATA3        0x06
#define MB_CMD_REQ_DATA4        0x07



void Write_UTM_Parameter(void);
void Read_Mes_Parameter(void);
void Write_UTG_Parameter(void);
void Read_UTG_Parameter(void);
void Write_UTT_Parameter(void);
void Read_UTT_Parameter(void);
unsigned char strt_measurement(char Rut_sel);
void update_HRT(void);
void update_MB(void);
void update_TCP_MB(void);


