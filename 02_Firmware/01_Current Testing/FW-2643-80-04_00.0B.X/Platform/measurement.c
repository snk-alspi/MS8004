
#define _XTAL_FREQ 8000000UL
#include <xc.h>
#include <string.h>
#include "pic24f.h"
#include <libpic30.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "GlobalVariables.h"
#include "../Platform/gpio.h"
#include "../MS/MS_Process.h"
#include "../MS/MS_Data.h"
#include "../WirelessHART_App_Task/WirelessHART_App_Task.h"
#include "measurement.h"
#include "uart.h"
#include "gpio.h"
#include "../Global/SoftwareCrc.h"

extern HRTUT_parameters HRT_Para;
extern MBUT_parameters MB_Para;

//------------------------------ MODBUS ROUTINES----------------

/*
 Format
 * First value is the number of characters to send
 * Last 2 characters is CRC of characters to send
 */
// command1 for setting probe number.
//const char m_comm1[]={0x0A, 0x08, 0x01, 0x0F, 0x00, 0x04, 0x00, 0x04, 0x01, 0x00, 0xCF, 0x56};
//
//// command2 for firing
//const char m_comm2[]={0x08, 0x08, 0x01, 0x05, 0x00, 0x00, 0xFF, 0x00, 0x8C, 0x3A};
//
//// command3 for data ready
//const char m_comm3[]={0x08, 0x06, 0x01, 0x02, 0x00, 0x00, 0x00, 0x01, 0xB9, 0xCA};
//
//// command4 for Collecting Data - Read Probe #1 Thickness, Temp comp thickness and temp. in Floating Point Format
//const char m_comm4[]={0x08, 0x11, 0x01, 0x03, 0x00, 0xDE, 0x00, 0x06, 0xA5, 0xF2};
//
//// command5 for Collecting Data - Read Probe #1 Temperature Compensated Thickness in Floating Point Format
//const char m_comm5[]={0x08, 0x09, 0x01, 0x03, 0x00, 0xE0, 0x00, 0x02, 0xC5, 0xFD};
//
//// command6 for Collecting Data - Read Material Temperature in Floating Point Format
//const char m_comm6[]={0x08, 0x09, 0x01, 0x03, 0x00, 0xE2, 0x00, 0x02, 0x64, 0x3D};
//
//// command7 for Collecting Data - Read DSI Temperature in ASCII Format
//const char m_comm7[]={0x08, 0x0D, 0x01, 0x03, 0x00, 0x80, 0x00, 0x04, 0x45, 0xE1};



void chk_communication(void);
void store_mat_thick_raw(void);
void store_mat_thick_CT(void);
void store_mat_temp(void);
void store_DSI_T(void);
void power_up(void);
void power_down(void);
void load_tx_buf1(char * ru);

extern void mb_tx( void );

extern DEVICE_PROCESS_VARIABLES_PROVISION_T g_stDeviceProcessVariables;


void load_tx_buf(unsigned char a){
    unsigned char b;
    for (b = 0; b < 100; b++) {
    mb_rx_buff[b]=0; mb_tx_buff[b]=0;
    }
    switch (a){
        case MB_CMD_SET_PROBE:
            tx_count = 0x0A; rx_count = 8;
            mb_tx_buff[0]=0x01; mb_tx_buff[1]=0x0F; mb_tx_buff[2]=0x00; mb_tx_buff[3]=0x04; mb_tx_buff[4]=0x00; mb_tx_buff[5]=0x04; mb_tx_buff[6]=0x00; mb_tx_buff[7]=ch;
            crcr = SoftwareCrc16_Calculate((const uint8 *)mb_tx_buff,8); mb_tx_buff[8] = (char)(crcr & 0x00FF); mb_tx_buff[9] = (char)(crcr>>8);
            break;
        
        case MB_CMD_FIRE:
            tx_count = 0x08; rx_count = 0x08;
            mb_tx_buff[0]=0x01; mb_tx_buff[1]=0x05; mb_tx_buff[2]=0x00; mb_tx_buff[3]=0x00; mb_tx_buff[4]=0xFF; mb_tx_buff[5]=0x00; 
            crcr = SoftwareCrc16_Calculate((const uint8 *)mb_tx_buff,6); mb_tx_buff[6] = (char)(crcr & 0x00FF); mb_tx_buff[7] = (char)(crcr>>8);          
        break;
        
        case MB_CMD_DATA_RDY:  
            tx_count = 0x08; rx_count = 0x06;
            mb_tx_buff[0]=0x01; mb_tx_buff[1]=0x02; mb_tx_buff[2]=0x00; mb_tx_buff[3]=0x00; mb_tx_buff[4]=0x00; mb_tx_buff[5]=0x01;
            crcr = SoftwareCrc16_Calculate((const uint8 *)mb_tx_buff,6); mb_tx_buff[6] = (char)(crcr & 0x00FF); mb_tx_buff[7] = (char)(crcr>>8);        
        break;
        
        case MB_CMD_REQ_DATA1: //Read Probe #ch Thickness, Temp comp thickness and temp. in Floating Point Format
            tx_count = 0x08; rx_count = 0x11;
            mb_tx_buff[0]=0x01; mb_tx_buff[1]=0x03; mb_tx_buff[2]=(ch*8); mb_tx_buff[3]=0xDE; mb_tx_buff[4]=0x00; mb_tx_buff[5]=0x06;
            crcr = SoftwareCrc16_Calculate((const uint8 *)mb_tx_buff,6); mb_tx_buff[6] = (char)(crcr & 0x00FF); mb_tx_buff[7] = (char)(crcr>>8);          
        break;
        
        case MB_CMD_REQ_DATA2: //Read Probe #ch Temperature Compensated Thickness in Floating Point Format
            tx_count = 0x08; rx_count =0x09;
            mb_tx_buff[0]=0x01; mb_tx_buff[1]=0x03; mb_tx_buff[2]=(ch*8); mb_tx_buff[3]=0xE0; mb_tx_buff[4]=0x00; mb_tx_buff[5]=0x02;
            crcr = SoftwareCrc16_Calculate((const uint8 *)mb_tx_buff,6); mb_tx_buff[6] = (char)(crcr & 0x00FF); mb_tx_buff[7] = (char)(crcr>>8);           
        break;
        
        case MB_CMD_REQ_DATA3: //Read Material Temperature in Floating Point Format
            tx_count = 0x08; rx_count = 0x09;
            mb_tx_buff[0]=0x01; mb_tx_buff[1]=0x03; mb_tx_buff[2]=(ch*8); mb_tx_buff[3]=0xE2; mb_tx_buff[4]=0x00; mb_tx_buff[5]=0x06;
            crcr = SoftwareCrc16_Calculate((const uint8 *)mb_tx_buff,6); mb_tx_buff[6] = (char)(crcr & 0x00FF); mb_tx_buff[7] = (char)(crcr>>8);           
        break;
        
        case MB_CMD_REQ_DATA4: //Read DSI Temperature in ASCII Format
            tx_count = 0x08; rx_count = 0x0D;
            mb_tx_buff[0]=0x01; mb_tx_buff[1]=0x03; mb_tx_buff[2]=(ch*8); mb_tx_buff[3]=0x80; mb_tx_buff[4]=0x00; mb_tx_buff[5]=0x04;
            crcr = SoftwareCrc16_Calculate((const uint8 *)mb_tx_buff,6); mb_tx_buff[6] = (char)(crcr & 0x00FF); mb_tx_buff[7] = (char)(crcr>>8);           
        break;
        
    }
    tx_counter=0; 
    mb_comm_stat = MB_TX_START;
    mb_tx();
}


void load_tx_buf1(char * ru){
    unsigned char b;
    for (b = 0; b < 100; b++) {
    mb_rx_buff[b]=0; mb_tx_buff[b]=0;
    }
    for(b=0;b<tx_count;b++){
    mb_tx_buff[b] = ru[b];           
    }
    tx_counter=0;
    mb_comm_stat = MB_TX_START;
    mb_tx();
}

void Mes_Err(void){
    
    switch (ch) {
        case 0:
             mat_thick_raw_0 = 0;
             mat_thick_TC_0 = 0;
             mat_temp_0 = 0; 
             dsi_t_0 = 0;
             break;
             
        case 1:
             mat_thick_raw_1 = 0;
             mat_thick_TC_1 = 0;
             mat_temp_0 = 1; 
             dsi_t_0 = 1;
             break;
             
        case 2:
             mat_thick_raw_2 = 0;
             mat_thick_TC_2 = 0;
             mat_temp_2 = 0; 
             dsi_t_2 = 0;
             break;
             
        case 3:
             mat_thick_raw_3 = 0;
             mat_thick_TC_3 = 0;
             mat_temp_3 = 0; 
             dsi_t_3 = 0;
             break;
     
    }
     mes_stat = MES_STOPPED;
     g_ucMeasurementInProgress = 0;
     m_ucHartReqInProgress = 0;
     power_down();
}

void MPar_Err(void){
    mes_para_stat = MB_PARA_STOPPED;
     m_ucHartReqInProgress = 0;
     power_down();
}

void excute_mesurement(void){
    
    chk_communication(); // Modified on 03/07/23; to be verified. Timer #3 is not used 

    switch (mes_stat){
      case MES_START:
                g_ucMeasurementInProgress = 1;
                power_up();
                mes_stat = MB_MES_S2; 
                break;
                
        case MB_MES_S2:        
                load_tx_buf(1);
                timeout=0;
                mes_stat = MB_MES_SET_PROBE;
        break;  //MB_MES_START
        
        case MB_MES_SET_PROBE:
            if(mb_comm_stat == MB_RX_FINSHED){
                load_tx_buf(2);
                mes_stat = MB_MES_FIRE_COIL; remeasure=0;     
            } else if(mes_err_stat== MB_ERROR_NOREPLY){
                if(!remeasure){ mes_stat = MB_MES_S2; remeasure=1;} else {Mes_Err();}
            }
        break; //MB_MES_SET_PROBE
        
        case MB_MES_FIRE_COIL:
            if(mb_comm_stat == MB_RX_FINSHED){
                load_tx_buf(3);
                uc_Mes_finish_cnt=0;
                mes_stat = MB_MES_WAIT_TO_FINISH;  remeasure=0;   
            }else if(mes_err_stat== MB_ERROR_NOREPLY){
                if(!remeasure){ 
                    load_tx_buf(2);
                    remeasure=1;
                } else {Mes_Err();}
            }
        break; //MB_MES_FIRE_COIL
                
        case MB_MES_WAIT_TO_FINISH:
            // Check response 
            if(mb_comm_stat == MB_RX_FINSHED){
                if(mb_rx_buff[3]!=0x01){
                    load_tx_buf(3);
                    uc_Mes_finish_cnt++;
                    if(uc_Mes_finish_cnt>5){Mes_Err();}
                }else{
                   load_tx_buf(4);
                   mes_stat = MB_MES_READ_RESULT_3; remeasure=0;  
                }
            } else if(mes_err_stat== MB_ERROR_NOREPLY){
                if(!remeasure){ 
                    load_tx_buf(3);
                    remeasure=1;
                } else {Mes_Err();}
            }
        break; //MB_MES_WAIT_TO_FINISH
                
        case MB_MES_READ_RESULT_1:            
            if(mb_comm_stat == MB_RX_FINSHED){
                store_mat_thick_raw();
                load_tx_buf(5);
                mes_stat = MB_MES_READ_RESULT_2; remeasure=0;
            } else if(mes_err_stat== MB_ERROR_NOREPLY){
                if(!remeasure){ 
                    load_tx_buf(4);
                    remeasure=1;
                } else {Mes_Err();}
            }
        break; //MB_MES_READ_RESULT
        
        case MB_MES_READ_RESULT_2:            
            
            if(mb_comm_stat == MB_RX_FINSHED){
                store_mat_thick_CT();
                load_tx_buf(6);
                mes_stat = MB_MES_READ_RESULT_3; remeasure=0;
            } else if(mes_err_stat== MB_ERROR_NOREPLY){
                if(!remeasure){ 
                    load_tx_buf(5);
                    remeasure=1;
                } else {Mes_Err();}
            }
        break; //MB_MES_READ_RESULT
        
        case MB_MES_READ_RESULT_3:            
            
            if(mb_comm_stat == MB_RX_FINSHED){
                store_mat_thick_raw();
                store_mat_thick_CT();
                store_mat_temp();
                load_tx_buf(7);
                mes_stat = MB_MES_READ_RESULT_4; remeasure=0;
            } else if(mes_err_stat== MB_ERROR_NOREPLY){
                if(!remeasure){ 
                    load_tx_buf(6);
                    remeasure=1;
                } else {Mes_Err();}
            }
        break; //MB_MES_READ_RESULT
        
        case MB_MES_READ_RESULT_4:            
            
            if(mb_comm_stat == MB_RX_FINSHED){
                Pwr_MES_Dis();
                store_DSI_T(); //store_probe_temp();
                mes_stat = MB_MES_FINISH; 
            } else if(mes_err_stat== MB_ERROR_NOREPLY){
                if(!remeasure){
                    load_tx_buf(7);
                    remeasure=1;
                } else {Mes_Err();}
            }
        break; //MB_MES_READ_RESULT
        
                        
        case MB_MES_FINISH:
            // any thing left?
            if(!m_ucHartReqInProgress  && ch<3){
                ch++;
                mes_stat = MES_START; 
                remeasure = 0;
            } else {
                UpdateMetalLossValue();
                mes_stat = MES_STOPPED;
                g_ucMeasurementInProgress = 0;
                ch=0;
            }
        break; //MB_MES_FINISH  
        
    }
    
}


void chk_communication(void){
    
    if(mb_comm_stat != MB_RX_FINSHED){
        // Communication in progress
    switch(mb_comm_stat){
        
        case MB_TX_START:
            mes_err_stat=0;
         
        break;//MB_TX_START

        case MB_TX_FINISHED:
            mes_err_stat=0;
            timeout = 0;
            mb_comm_stat = MB_RX_START;
        break; //MB_TX_FINISHED:
        
        case MB_RX_START:
            if(mcnt>0){
                mb_comm_stat = MB_FRAME_OVER;
            }
            else if(timeout>2){
             mes_err_stat = MB_ERROR_NOREPLY;   
             }
        break;
                

        case MB_FRAME_OVER:
            if(mcnt> (rx_count-1)){
             mb_comm_stat = MB_RX_FINSHED;
            } else if(timeout>2){
             mes_err_stat = MB_ERROR_NOREPLY;   
             }            
        break;    //MB_FRAME_OVER     
    }
    
    }  // MB_Communication in ideal stage
}


void store_mat_thick_raw(void){ // Probe Thickness calculation
    //- you need to swap the word order of the result value before converting from IEEE 32bit Float
    switch (ch){
        case 0:
            BYTE3(mat_thick_raw_0) = mb_rx_buff[5];
            BYTE2(mat_thick_raw_0) = mb_rx_buff[6];
            BYTE1(mat_thick_raw_0) = mb_rx_buff[3];
            BYTE0(mat_thick_raw_0) = mb_rx_buff[4];
            mat_thick_raw_0 = mat_thick_raw_0 * 1000;
            break;
            
        case 1:
            BYTE3(mat_thick_raw_1) = mb_rx_buff[5];
            BYTE2(mat_thick_raw_1) = mb_rx_buff[6];
            BYTE1(mat_thick_raw_1) = mb_rx_buff[3];
            BYTE0(mat_thick_raw_1) = mb_rx_buff[4];
            mat_thick_raw_1 = mat_thick_raw_1 * 1000;
            break;
            
        case 2:
            BYTE3(mat_thick_raw_2) = mb_rx_buff[5];
            BYTE2(mat_thick_raw_2) = mb_rx_buff[6];
            BYTE1(mat_thick_raw_2) = mb_rx_buff[3];
            BYTE0(mat_thick_raw_2) = mb_rx_buff[4];
            mat_thick_raw_2 = mat_thick_raw_2 * 1000;
            break;    
            
        case 3:
            BYTE3(mat_thick_raw_3) = mb_rx_buff[5];
            BYTE2(mat_thick_raw_3) = mb_rx_buff[6];
            BYTE1(mat_thick_raw_3) = mb_rx_buff[3];
            BYTE0(mat_thick_raw_3) = mb_rx_buff[4];
            mat_thick_raw_3 = mat_thick_raw_3 * 1000;
            break;
            
    }
    
    //probe_thick, probe_thick_TC, probe_temperature
}

void store_mat_thick_CT(void){  // Temperature compensated Probe Thickness calculation
    //- you need to swap the word order of the result value before converting from IEEE 32bit Float
    switch (ch){
        case 0:
            BYTE3(mat_thick_TC_0) = mb_rx_buff[9];
            BYTE2(mat_thick_TC_0) = mb_rx_buff[10];
            BYTE1(mat_thick_TC_0) = mb_rx_buff[7];
            BYTE0(mat_thick_TC_0) = mb_rx_buff[8];
            mat_thick_TC_0 = mat_thick_TC_0 * 1000;
            break;
            
        case 1:
            BYTE3(mat_thick_TC_1) = mb_rx_buff[9];
            BYTE2(mat_thick_TC_1) = mb_rx_buff[10];
            BYTE1(mat_thick_TC_1) = mb_rx_buff[7];
            BYTE0(mat_thick_TC_1) = mb_rx_buff[8];
            mat_thick_TC_1 = mat_thick_TC_1 * 1000;
            break;
            
        case 2:
            BYTE3(mat_thick_TC_2) = mb_rx_buff[9];
            BYTE2(mat_thick_TC_2) = mb_rx_buff[10];
            BYTE1(mat_thick_TC_2) = mb_rx_buff[7];
            BYTE0(mat_thick_TC_2) = mb_rx_buff[8];
            mat_thick_TC_2 = mat_thick_TC_2 * 1000;
            break;
            
        case 3:
            BYTE3(mat_thick_TC_3) = mb_rx_buff[9];
            BYTE2(mat_thick_TC_3) = mb_rx_buff[10];
            BYTE1(mat_thick_TC_3) = mb_rx_buff[7];
            BYTE0(mat_thick_TC_3) = mb_rx_buff[8];
            mat_thick_TC_3 = mat_thick_TC_3 * 1000;
            break;    
    
    }
}

void store_mat_temp(void){  // Temperature
    //- you need to swap the word order of the result value before converting from IEEE 32bit Float
    switch (ch) {
        case 0: 
            BYTE3(mat_temp_0) = mb_rx_buff[13];
            BYTE2(mat_temp_0) = mb_rx_buff[14];
            BYTE1(mat_temp_0) = mb_rx_buff[11];
            BYTE0(mat_temp_0) = mb_rx_buff[12];
            break;
            
        case 1: 
            BYTE3(mat_temp_1) = mb_rx_buff[13];
            BYTE2(mat_temp_1) = mb_rx_buff[14];
            BYTE1(mat_temp_1) = mb_rx_buff[11];
            BYTE0(mat_temp_1) = mb_rx_buff[12];
            break;
        
        case 2: 
            BYTE3(mat_temp_2) = mb_rx_buff[13];
            BYTE2(mat_temp_2) = mb_rx_buff[14];
            BYTE1(mat_temp_2) = mb_rx_buff[11];
            BYTE0(mat_temp_2) = mb_rx_buff[12];
            break;
            
        case 3: 
            BYTE3(mat_temp_3) = mb_rx_buff[13];
            BYTE2(mat_temp_3) = mb_rx_buff[14];
            BYTE1(mat_temp_3) = mb_rx_buff[11];
            BYTE0(mat_temp_3) = mb_rx_buff[12];
            break;
    
    }
}

void store_DSI_T(void){  // Temperature
    char z[10];
    //- ASCII value received - convert to float
    
    switch (ch) {
        case 0: 
            z[0]=mb_rx_buff[4];     z[1]=mb_rx_buff[3];     z[2] = mb_rx_buff[6];   z[3] = mb_rx_buff[5];
            z[4]=mb_rx_buff[8];     z[5]=mb_rx_buff[7];     z[6] = mb_rx_buff[10];  z[7] = mb_rx_buff[9];
            dsi_t_0 = atof(z);
            break;
            
        case 1: 
            z[0]=mb_rx_buff[4];     z[1]=mb_rx_buff[3];     z[2] = mb_rx_buff[6];   z[3] = mb_rx_buff[5];
            z[4]=mb_rx_buff[8];     z[5]=mb_rx_buff[7];     z[6] = mb_rx_buff[10];  z[7] = mb_rx_buff[9];
            dsi_t_1 = atof(z);
            break;
            
        case 2: 
            z[0]=mb_rx_buff[4];     z[1]=mb_rx_buff[3];     z[2] = mb_rx_buff[6];   z[3] = mb_rx_buff[5];
            z[4]=mb_rx_buff[8];     z[5]=mb_rx_buff[7];     z[6] = mb_rx_buff[10];  z[7] = mb_rx_buff[9];
            dsi_t_2 = atof(z);
            break;
            
        case 3: 
            z[0]=mb_rx_buff[4];     z[1]=mb_rx_buff[3];     z[2] = mb_rx_buff[6];   z[3] = mb_rx_buff[5];
            z[4]=mb_rx_buff[8];     z[5]=mb_rx_buff[7];     z[6] = mb_rx_buff[10];  z[7] = mb_rx_buff[9];
            dsi_t_3 = atof(z);
            break;
    
    
    }

}

/*
 * ************************************************************
 * All Hart Reqeust Parameter execution starts here
 * m_ucHartReqInProgress --> HART Request monitoring
 * 1 - Read_MES - Read UT Setup parameters
 * 2 - Write_UTM - Write UT setup parameters
 * 3 - Write_UTG - Write UT Gate parameters
 * 4 - Write_UTT - Write UT Temperature Compensation parameters
 * **************************************************************
 
 */

void Read_Mes_Parameter(void){

    char yt[40];
    
    chk_communication();
    
    switch(mes_para_stat){
        
        case MB_PARA_START:
            m_ucHartReqInProgress = 2;
            power_up();
            mes_para_stat = MB_PARA_VAL_01;
            break;
        
        case MB_PARA_VAL_01:  // Starting from Material velocity till Temp. Compensation factor
            yt[0]=0x01; yt[1]=0x03; yt[2]=(ch*8); yt[3]=0x4C; yt[4]=0; yt[5]=0x22;
            crcr = SoftwareCrc16_Calculate((const uint8 *)yt,6); yt[6] = (char)(crcr & 0x00FF); yt[7] = (char)(crcr>>8);
            rx_count = 9; tx_count = 8;
            load_tx_buf1(yt);
            mes_para_stat = MB_PARA_VAL_02;  //mes_para_stat = MB_PARA_VAL_02; remeasure=0;
            
        break;
    
        case MB_PARA_VAL_02:  //
            if(mb_comm_stat == MB_RX_FINSHED){
                
                //for(r=0;r<16;r++){ Mat_velocity[0] = mb_rx_buff[r+6]; }
                //Save received Mat_velocity value
                MB_Para.mb_cMaterial_velocity[0] = mb_rx_buff[4];    MB_Para.mb_cMaterial_velocity[1] = mb_rx_buff[3];    MB_Para.mb_cMaterial_velocity[2] = mb_rx_buff[6];    MB_Para.mb_cMaterial_velocity[3] = mb_rx_buff[5];
                MB_Para.mb_cMaterial_velocity[4] = mb_rx_buff[8];    MB_Para.mb_cMaterial_velocity[5] = mb_rx_buff[7];    MB_Para.mb_cMaterial_velocity[6] = mb_rx_buff[10];   MB_Para.mb_cMaterial_velocity[7] = mb_rx_buff[9];
                MB_Para.mb_cMaterial_velocity[8] = mb_rx_buff[12];   MB_Para.mb_cMaterial_velocity[9] = mb_rx_buff[11];   MB_Para.mb_cMaterial_velocity[10] = mb_rx_buff[14];  MB_Para.mb_cMaterial_velocity[11] = mb_rx_buff[13];
                MB_Para.mb_cMaterial_velocity[12] = mb_rx_buff[16];  MB_Para.mb_cMaterial_velocity[13] = mb_rx_buff[15];  MB_Para.mb_cMaterial_velocity[14] = mb_rx_buff[18];  MB_Para.mb_cMaterial_velocity[15] = mb_rx_buff[17];
                
                //Save received Cal_0_Offset value
                BYTE0(MB_Para.mb_ulCal_0_offset) = mb_rx_buff[20];
                BYTE1(MB_Para.mb_ulCal_0_offset) = mb_rx_buff[19];
                BYTE2(MB_Para.mb_ulCal_0_offset) = mb_rx_buff[22];
                BYTE3(MB_Para.mb_ulCal_0_offset) = mb_rx_buff[21];
                
                //Save received Ref. Temperature and Comp. factor
                BYTE0(MB_Para.mb_fTempCompRefTemp) = mb_rx_buff[64];
                BYTE1(MB_Para.mb_fTempCompRefTemp) = mb_rx_buff[63];
                BYTE2(MB_Para.mb_fTempCompRefTemp) = mb_rx_buff[66];
                BYTE3(MB_Para.mb_fTempCompRefTemp) = mb_rx_buff[65];
                
                BYTE0(MB_Para.mb_fTempCompFactor) = mb_rx_buff[68];
                BYTE1(MB_Para.mb_fTempCompFactor) = mb_rx_buff[67];
                BYTE2(MB_Para.mb_fTempCompFactor) = mb_rx_buff[70];
                BYTE3(MB_Para.mb_fTempCompFactor) = mb_rx_buff[69];
                
                yt[0]=0x01; yt[1]=0x03; yt[2]=(ch*8); yt[3]=0xB9; yt[4]=0; yt[5]=0x15;
                crcr = SoftwareCrc16_Calculate((const uint8 *)yt,6); yt[6] = (char)(crcr & 0x00FF); yt[7] = (char)(crcr>>8);
                load_tx_buf1(yt);
                rx_count = 21; tx_count = 8;
                mes_para_stat = MB_PARA_VAL_03; remeasure=0;
            }else if(mes_err_stat== MB_ERROR_NOREPLY){
                if(!remeasure){
                    tx_counter=0;  
                    mb_comm_stat = MB_TX_START;
                    mb_tx();
                    remeasure=1;
                } else {MPar_Err();}
            }
        break;

        case MB_PARA_VAL_03:  //
            if(mb_comm_stat == MB_RX_FINSHED){
                
                //Save received Pulser_width value
                BYTE0(MB_Para.mb_uiPulsar_width) = mb_rx_buff[4];
                BYTE1(MB_Para.mb_uiPulsar_width) = mb_rx_buff[3];
                
                //Save received Gain value
                BYTE0(MB_Para.mb_uiGain) = mb_rx_buff[6];
                BYTE1(MB_Para.mb_uiGain) = mb_rx_buff[5];
                
                //Save received aScan value
                BYTE0(MB_Para.mb_ulA_scan_start) = mb_rx_buff[12];
                BYTE1(MB_Para.mb_ulA_scan_start) = mb_rx_buff[11];
                BYTE2(MB_Para.mb_ulA_scan_start) = mb_rx_buff[14];
                BYTE3(MB_Para.mb_ulA_scan_start) = mb_rx_buff[13];
                
                //Save received G1_Strt value
                BYTE0(MB_Para.mb_ulG1_start) = mb_rx_buff[16];
                BYTE1(MB_Para.mb_ulG1_start) = mb_rx_buff[15];
                BYTE2(MB_Para.mb_ulG1_start) = mb_rx_buff[18];
                BYTE3(MB_Para.mb_ulG1_start) = mb_rx_buff[17];
                
                BYTE0(MB_Para.mb_ulG1_width) = mb_rx_buff[20];
                BYTE1(MB_Para.mb_ulG1_width) = mb_rx_buff[19];
                BYTE2(MB_Para.mb_ulG1_width) = mb_rx_buff[22];
                BYTE3(MB_Para.mb_ulG1_width) = mb_rx_buff[21];
                
                //Save received G1_Thers value
                BYTE0(MB_Para.mb_uiG1_thershold) = mb_rx_buff[24];
                BYTE1(MB_Para.mb_uiG1_thershold) = mb_rx_buff[23];
                
                //Save received Measurement Mode value
                BYTE0(MB_Para.mb_uiMeasurement_mode) = mb_rx_buff[26];
                BYTE1(MB_Para.mb_uiMeasurement_mode) = mb_rx_buff[25];
                
                //Save received G2_Strt value
                BYTE0(MB_Para.mb_ulG2_start) = mb_rx_buff[34];
                BYTE1(MB_Para.mb_ulG2_start) = mb_rx_buff[33];
                BYTE2(MB_Para.mb_ulG2_start) = mb_rx_buff[36];
                BYTE3(MB_Para.mb_ulG2_start) = mb_rx_buff[35];
                
                //Save received G2_Width value
                BYTE0(MB_Para.mb_ulG2_width) = mb_rx_buff[38];
                BYTE1(MB_Para.mb_ulG2_width) = mb_rx_buff[37];
                BYTE2(MB_Para.mb_ulG2_width) = mb_rx_buff[40];
                BYTE3(MB_Para.mb_ulG2_width) = mb_rx_buff[39];
                
                //Save received G2_Thers value
                BYTE0(MB_Para.mb_uiG2_thershold) = mb_rx_buff[42];
                BYTE1(MB_Para.mb_uiG2_thershold) = mb_rx_buff[41];
                                
                
                 power_down();
                 m_ucHartReqInProgress = 0;
                 update_HRT();
                 mes_para_stat = MB_PARA_STOPPED;    
            }else if(mes_err_stat== MB_ERROR_NOREPLY){
                if(!remeasure){
                    tx_counter=0;
                    mb_comm_stat = MB_TX_START;
                    mb_tx();
                    remeasure=1;
                } else {MPar_Err();}
            }
        break;
    }
}

void Write_UTM_Parameter(void){
    
    unsigned char z;
    char yt[100];
    
    chk_communication();
    
     switch(mes_para_stat){
    
    case MB_PARA_START:
//            update_MB();
            power_up();
            m_ucHartReqInProgress = 1;
            mes_para_stat = MB_PARA_VAL_01;
            break;
    
    case MB_PARA_VAL_01:  //Cal_0_Offset
            update_MB();
            yt[0]=0x01; yt[1]=0x10; yt[2]=(ch*8); yt[3]=0x56; yt[4]=0x00; yt[5]=0x02;yt[6]=0x04;
            yt[8] = BYTE0(MB_Para.mb_ulCal_0_offset); yt[7] = BYTE1(MB_Para.mb_ulCal_0_offset); yt[10] = BYTE2(MB_Para.mb_ulCal_0_offset); yt[9] = BYTE3(MB_Para.mb_ulCal_0_offset);
            crcr = SoftwareCrc16_Calculate((const uint8 *)yt,11); yt[11] = (crcr & 0x00FF); yt[12] = (crcr>>8);
            rx_count = 8; tx_count = 13;
            load_tx_buf1(yt);
            mes_para_stat = MB_PARA_VAL_02; remeasure=0;
    break;
    
    
    case MB_PARA_VAL_02:  //Material Velocity
        if(mb_comm_stat == MB_RX_FINSHED){
            yt[0]=0x01; yt[1]=0x10; yt[2]=(ch*8); yt[3]=0x4C; yt[4]=0; yt[5]=0x08;yt[6]=0x10;
            for(z=0;z<16;z++){yt[z+7]= MB_Para.mb_cMaterial_velocity[z];}
            crcr = SoftwareCrc16_Calculate((const uint8 *)yt,23); yt[23] = (crcr & 0x00FF); yt[24] = (crcr>>8);
            rx_count = 8; tx_count = 25;
            load_tx_buf1(yt);
            mes_para_stat = MB_PARA_VAL_03; remeasure=0;
        }else if(mes_err_stat== MB_ERROR_NOREPLY){
            if(!remeasure){
                     tx_counter=0;
                    mb_comm_stat = MB_TX_START;
                    mb_tx();
                    remeasure=1;
            } else {MPar_Err();}
        }
    break;
    
    case MB_PARA_VAL_03:  //A-Scan
        if(mb_comm_stat == MB_RX_FINSHED){
            yt[0]=0x01; yt[1]=0x10; yt[2]=(ch*8); yt[3]=0xBD; yt[4]=0; yt[5]=0x02;yt[6]=0x04;
            yt[8] = BYTE0(MB_Para.mb_ulA_scan_start); yt[7] = BYTE1(MB_Para.mb_ulA_scan_start); yt[10] = BYTE2(MB_Para.mb_ulA_scan_start); yt[9] = BYTE3(MB_Para.mb_ulA_scan_start);
            crcr = SoftwareCrc16_Calculate((const uint8 *)yt,11); yt[11] = (crcr & 0x00FF); yt[12] = (crcr>>8);
            rx_count = 8; tx_count = 13;
            load_tx_buf1(yt);
            mes_para_stat = MB_PARA_VAL_04; remeasure=0;
        }else if(mes_err_stat== MB_ERROR_NOREPLY){
            if(!remeasure){
                tx_counter=0;
                mb_comm_stat = MB_TX_START;
                mb_tx();
                remeasure=1;
            } else {MPar_Err();}
        }
    break;
    
    case MB_PARA_VAL_04:  //Pulsar Width - Gain
        if(mb_comm_stat == MB_RX_FINSHED){
            yt[0]=0x01; yt[1]=0x10; yt[2]=(ch*8); yt[3]=0xB9; yt[4]=0; yt[5]=0x02; yt[6]=0x04;
            yt[8] = MB_Para.mb_uiPulsar_width; yt[7] = (MB_Para.mb_uiPulsar_width>>8); 
            yt[10] = MB_Para.mb_uiGain; yt[9] = (MB_Para.mb_uiGain>>8);
            crcr = SoftwareCrc16_Calculate((const uint8 *)yt,11); yt[11] = (crcr & 0x00FF); yt[12] = (crcr>>8);
            rx_count = 8; tx_count = 13;
            load_tx_buf1(yt);
            mes_para_stat = MB_PARA_VAL_05; remeasure=0;
        }else if(mes_err_stat== MB_ERROR_NOREPLY){
           if(!remeasure){
                tx_counter=0;
                mb_comm_stat = MB_TX_START;
                mb_tx();
                remeasure=1;
            } else {MPar_Err();}
        }
    break;
           
    case MB_PARA_VAL_05:  // Closr UTM parameter write
        if(mb_comm_stat == MB_RX_FINSHED){
            m_ucHartReqInProgress = 0;
            MB_Para.mb_ulCal_0_offset = 0;
        for(z=0;z<16;z++){MB_Para.mb_cMaterial_velocity[z]=0;}
        MB_Para.mb_ulA_scan_start=0;;
        MB_Para.mb_uiPulsar_width=0;;
        MB_Para.mb_uiGain=0;
          strt_measurement(3);
        }else if(mes_err_stat== MB_ERROR_NOREPLY){
            if(!remeasure){
                tx_counter=0;
                mb_comm_stat = MB_TX_START;
                mb_tx();
                remeasure=1;
            } else {Mes_Err();}
        }
    break;
     }
    
}

void Write_UTG_Parameter(void){
    
    char yt[100];
    
    chk_communication();
    
     switch(mes_para_stat){
    
    case MB_PARA_START:
            power_up();
            m_ucHartReqInProgress = 5;
            mes_para_stat = MB_PARA_VAL_01;
            break;
    
    case MB_PARA_VAL_01:  //Cal_0_Offset
            update_MB();
            yt[0]=0x01; yt[1]=0x10; yt[2]=(ch*8); yt[3]=0xBF; yt[4]=0; yt[5]=0x08;yt[6]=0x0C;
            yt[8] = BYTE0(MB_Para.mb_ulG1_start); yt[7] = BYTE1(MB_Para.mb_ulG1_start); yt[10] = BYTE2(MB_Para.mb_ulG1_start); yt[9] = BYTE3(MB_Para.mb_ulG1_start);
            yt[12] = BYTE0(MB_Para.mb_ulG1_width); yt[11] = BYTE1(MB_Para.mb_ulG1_width); yt[13] = BYTE2(MB_Para.mb_ulG1_width); yt[14] = BYTE3(MB_Para.mb_ulG1_width);
            yt[16] = MB_Para.mb_uiG1_thershold; yt[15] = (MB_Para.mb_uiG1_thershold>>8);
            yt[18] = MB_Para.mb_uiMeasurement_mode; yt[17] = (MB_Para.mb_uiMeasurement_mode>>8);
            crcr = SoftwareCrc16_Calculate((const uint8 *)yt,19); yt[19] = (crcr & 0x00FF); yt[20] = (crcr>>8);
            rx_count = 8; tx_count = 21;
            load_tx_buf1(yt);
            mes_para_stat = MB_PARA_VAL_02; remeasure=0;
        break;
    
    
    case MB_PARA_VAL_02:  //Material Velocity
         if(mb_comm_stat == MB_RX_FINSHED){
            yt[0]=0x01; yt[1]=0x10; yt[2]=(ch*8); yt[3]=0xC8; yt[4]=0; yt[5]=0x05; yt[6]=0x0A;
             yt[8] = BYTE0(MB_Para.mb_ulG2_start); yt[7] = BYTE1(MB_Para.mb_ulG2_start); yt[10] = BYTE2(MB_Para.mb_ulG2_start); yt[9] = BYTE3(MB_Para.mb_ulG2_start);
            yt[12] = BYTE0(MB_Para.mb_ulG2_width); yt[11] = BYTE1(MB_Para.mb_ulG2_width); yt[14] = BYTE2(MB_Para.mb_ulG2_width); yt[13] = BYTE3(MB_Para.mb_ulG2_width);
            yt[16] = MB_Para.mb_uiG2_thershold; yt[15] = (MB_Para.mb_uiG2_thershold>>8);
            crcr = SoftwareCrc16_Calculate((const uint8 *)yt,17); yt[17] = (crcr & 0x00FF); yt[18] = (crcr>>8);
            rx_count = 8; tx_count = 19;
            load_tx_buf1(yt);;
            mes_para_stat = MB_PARA_VAL_03; remeasure=0;
        }else if(mes_err_stat== MB_ERROR_NOREPLY){
            if(!remeasure){
                tx_counter=0;
                mb_comm_stat = MB_TX_START;
                mb_tx();
                remeasure=1;
            } else {MPar_Err();}
        }
     break;
     
     case MB_PARA_VAL_03:  // Measurement Mode
        if(mb_comm_stat == MB_RX_FINSHED){

            m_ucHartReqInProgress = 0;
        MB_Para.mb_ulG1_start=0;
        MB_Para.mb_ulG1_width=0;
        MB_Para.mb_uiG1_thershold=0;
        MB_Para.mb_ulG2_start=0;
        MB_Para.mb_ulG2_width=0;
        MB_Para.mb_uiG2_thershold=0;
        MB_Para.mb_uiMeasurement_mode=0;
          strt_measurement(3);
        }else if(mes_err_stat== MB_ERROR_NOREPLY){
            if(!remeasure){
                tx_counter=0;
                mb_comm_stat = MB_TX_START;
                mb_tx();
                remeasure=1;
            } else {Mes_Err();}
        }
    break;
               
     }
    
}

void Write_UTT_Parameter(void){
    
    unsigned char z;
    char yt[40];
    
    chk_communication();
    
    switch(mes_para_stat){
        case MB_PARA_START:
            update_TCP_MB();
            power_up();
            m_ucHartReqInProgress =4;
            mes_para_stat = MB_PARA_VAL_01;
            break;
            
        case MB_PARA_VAL_01:  //TC Ref. Temp & Comp. factor   Ref_temp, Comp_factor
            yt[0]=0x01; yt[1]=0x10; yt[2]=(ch*8); yt[3]=0x69; yt[4]=0x00; yt[5]=0x04; yt[6]=0x08;
            yt[8] = BYTE0(MB_Para.mb_fTempCompRefTemp); yt[7] = BYTE1(MB_Para.mb_fTempCompRefTemp); yt[10] = BYTE2(MB_Para.mb_fTempCompRefTemp); yt[9] = BYTE3(MB_Para.mb_fTempCompRefTemp);
            yt[12] = BYTE0(MB_Para.mb_fTempCompFactor); yt[11] = BYTE1(MB_Para.mb_fTempCompFactor); yt[14] = BYTE2(MB_Para.mb_fTempCompFactor); yt[13] = BYTE3(MB_Para.mb_fTempCompFactor);
            crcr = SoftwareCrc16_Calculate((const uint8 *)yt,15); yt[15] = (crcr & 0x00FF); yt[16] = (crcr>>8);
            rx_count = 8; tx_count = 17;
            load_tx_buf1(yt);
            mes_para_stat = MB_PARA_VAL_02; remeasure=0;
        break;
        
        case MB_PARA_VAL_02:  //Material Velocity
        if(mb_comm_stat == MB_RX_FINSHED){
            yt[0]=0x01; yt[1]=0x10; yt[2]=(ch*8); yt[3]=0x4C; yt[4]=0; yt[5]=0x08; yt[6]=0x10;
            for(z=0;z<16;z++){yt[z+5]= MB_Para.mb_cMaterial_velocity[z];}
            crcr = SoftwareCrc16_Calculate((const uint8 *)yt,23); yt[23] = (crcr & 0x00FF); yt[24] = (crcr>>8);
            rx_count = 8; tx_count = 25;
            load_tx_buf1(yt);
            mes_para_stat = MB_PARA_VAL_03; remeasure=0;
        }else if(mes_err_stat== MB_ERROR_NOREPLY){
            if(!remeasure){
                    rx_count = 8; tx_count = 17;
                    tx_counter=0;
                    mb_comm_stat = MB_TX_START;
                    mb_tx();
                    remeasure=1;
            } else {MPar_Err();}
        }
        break;
        
        case MB_PARA_VAL_03:  // Finish
        if(mb_comm_stat == MB_RX_FINSHED){
    
            power_down();
            m_ucHartReqInProgress = 0;
            mes_para_stat = MB_PARA_STOPPED;
        }else if(mes_err_stat== MB_ERROR_NOREPLY){
            if(!remeasure){
                rx_count = 8; tx_count = 25;
                    tx_counter=0;
                    mb_comm_stat = MB_TX_START;
                    mb_tx();
                    remeasure=1;
            } else {Mes_Err();}
        }
        break;
    }
  
}

void power_up(void){
     
    PMD1bits.U2MD = 0;

    GPIO_UART2_Enable();
    UART2_Init(UART_BAUD_38400);
    UART2_Enable_Tx(); // Interrupt Enable
    UART2_Enable_Rx(); // Interrupt Enable
       
}

void power_down(void){
  Pwr_MES_Dis(); 
  UART2_Disable();
  GPIO_UART2_Disable();   
}

unsigned char strt_measurement(char Rut_sel){
    if(!g_ucMeasurementInProgress && !m_ucHartReqInProgress) {
    TRISBbits.TRISB13 = 0;
    LATBbits.LATB13 = 0;   
    if(Rut_sel==1) {       
        mes_stat = MES_START; remeasure = 0; 
        excute_mesurement(); 
    }
    if(Rut_sel==2){
        mes_para_stat = MB_PARA_START; remeasure = 0;
        Write_UTM_Parameter();
    }
    if(Rut_sel==3){
        mes_para_stat = MB_PARA_START; remeasure = 0;
        Read_Mes_Parameter();
    }
    if(Rut_sel==4){
        mes_para_stat = MB_PARA_START; remeasure = 0;
        Write_UTT_Parameter();
    }
    if(Rut_sel==5){
        mes_para_stat = MB_PARA_START; remeasure = 0;
        Write_UTG_Parameter();
    }
    return 1;
    } else return 0;
}


void update_HRT(void){  // done while Reading
    
    float fTemp1, fMVinnsec;
    
    
    
    HRT_Para.h_fCal_0_offset = (float)((float)MB_Para.mb_ulCal_0_offset *(float)10.17252604);
    
    HRT_Para.h_fMaterial_velocity = atof(MB_Para.mb_cMaterial_velocity);
    /*
     * Material_Velocity in m/sec
     * Material Velocity in mm/sec = (Material_Velocity in m/sec *1000)
     * Material Velocity in mm/NonoSec = Material Velocity in mm/sec / 1000000000  
     */
    fMVinnsec = HRT_Para.h_fMaterial_velocity / 1000000; 
    
    fTemp1 = (float)MB_Para.mb_ulA_scan_start * (float)41.6666;
    HRT_Para.h_fA_scan_start = (float)((fTemp1 * fMVinnsec)/(float)2);
    
    HRT_Para.h_uiPulsar_width = (unsigned int)((float)MB_Para.mb_uiPulsar_width * (float)41.6666);
    
    HRT_Para.h_uiGain = (unsigned int)((float)MB_Para.mb_uiGain * (float)0.1);
    
    fTemp1 = (float)MB_Para.mb_ulG1_start * (float)41.6666;
    HRT_Para.h_fG1_start = (float)((fTemp1 * fMVinnsec)/(float)2);
    
    fTemp1 = (float)MB_Para.mb_ulG1_width * (float)41.6666;
    HRT_Para.h_fG1_width = (float)((fTemp1 * fMVinnsec)/(float)2);
    
    HRT_Para.h_fG1_thershold = MB_Para.mb_uiG1_thershold * 0.004;
    
    fTemp1 = (float)MB_Para.mb_ulG2_start * (float)41.6666;
    HRT_Para.h_fG2_start = (float)((fTemp1 * fMVinnsec)/(float)2);
    
    fTemp1 = (float)MB_Para.mb_ulG2_width * (float)41.6666;
    HRT_Para.h_fG2_width = (float)((fTemp1 * fMVinnsec)/(float)2);
    
    HRT_Para.h_fG2_thershold = MB_Para.mb_uiG2_thershold * 0.004;
    
    HRT_Para.h_uiMeasurement_mode = MB_Para.mb_uiMeasurement_mode;
    
}

void update_MB(void){  // Done while Writing
    char t1[17];
    unsigned char a;
    unsigned long b;
    float fTemp1, fMVinnsec;
    
    if((HRT_Para.h_fCal_0_offset/1000000)>4000){HRT_Para.h_fCal_0_offset = 4000000000;}
    MB_Para.mb_ulCal_0_offset = lroundf(HRT_Para.h_fCal_0_offset /(float)10.17252604);
    
    for(a=0;a<17;a++){t1[a]=0;}
    b = (unsigned long)HRT_Para.h_fMaterial_velocity ;
    sprintf(t1, "%lu",b);
    MB_Para.mb_cMaterial_velocity[0] = t1[1];   MB_Para.mb_cMaterial_velocity[1] = t1[0];   MB_Para.mb_cMaterial_velocity[2] = t1[3];   MB_Para.mb_cMaterial_velocity[3] = t1[2];
    MB_Para.mb_cMaterial_velocity[4] = t1[5];   MB_Para.mb_cMaterial_velocity[5] = t1[4];   MB_Para.mb_cMaterial_velocity[6] = t1[7];   MB_Para.mb_cMaterial_velocity[7] = t1[6];
    MB_Para.mb_cMaterial_velocity[8] = t1[9];   MB_Para.mb_cMaterial_velocity[9] = t1[8];   MB_Para.mb_cMaterial_velocity[10] = t1[11];  MB_Para.mb_cMaterial_velocity[11] = t1[10];
    MB_Para.mb_cMaterial_velocity[12] = t1[13]; MB_Para.mb_cMaterial_velocity[13] = t1[12]; MB_Para.mb_cMaterial_velocity[14] = t1[15];  MB_Para.mb_cMaterial_velocity[15] = t1[14];
    
    
    fMVinnsec = HRT_Para.h_fMaterial_velocity / 1000000;
    
    fTemp1 = (HRT_Para.h_fA_scan_start/(float)41.6666) * 2;
    MB_Para.mb_ulA_scan_start = lroundf(fTemp1/fMVinnsec);
    
    MB_Para.mb_uiPulsar_width = (unsigned int)(lroundf(HRT_Para.h_uiPulsar_width/(float)41.666));
    
    MB_Para.mb_uiGain = (unsigned int)(lroundf(HRT_Para.h_uiGain/(float)0.1));
    
    fTemp1 = (HRT_Para.h_fG1_start/(float)41.6666) * 2;
    MB_Para.mb_ulG1_start = lroundf(fTemp1/fMVinnsec);
    
    fTemp1 = (HRT_Para.h_fG1_width/(float)41.6666) * 2;
    MB_Para.mb_ulG1_width = lroundf(fTemp1/fMVinnsec);
    
    MB_Para.mb_uiG1_thershold = (int)(lroundf(HRT_Para.h_fG1_thershold/0.004));
    
    fTemp1 = (HRT_Para.h_fG2_start/(float)41.6666) * 2;
    MB_Para.mb_ulG2_start = lroundf(fTemp1/fMVinnsec);
    
    fTemp1 = (HRT_Para.h_fG2_width/(float)41.6666) * 2;
    MB_Para.mb_ulG2_width = lroundf(fTemp1/fMVinnsec);
    
    MB_Para.mb_uiG2_thershold = (int)(lroundf(HRT_Para.h_fG2_thershold/0.004));
    
    MB_Para.mb_uiMeasurement_mode = HRT_Para.h_uiMeasurement_mode;

    
}

void update_TCP_MB(void){  // Done while Writing
    char t1[17];
    unsigned char a;
    unsigned long b;
   
    for(a=0;a<17;a++){t1[a]=0;}
    b = (unsigned long)HRT_Para.h_fMaterial_velocity ;
    sprintf(t1, "%lu",b);
    MB_Para.mb_cMaterial_velocity[0] = t1[1];   MB_Para.mb_cMaterial_velocity[1] = t1[0];   MB_Para.mb_cMaterial_velocity[2] = t1[3];   MB_Para.mb_cMaterial_velocity[3] = t1[2];
    MB_Para.mb_cMaterial_velocity[4] = t1[5];   MB_Para.mb_cMaterial_velocity[5] = t1[4];   MB_Para.mb_cMaterial_velocity[6] = t1[7];   MB_Para.mb_cMaterial_velocity[7] = t1[6];
    MB_Para.mb_cMaterial_velocity[8] = t1[9];   MB_Para.mb_cMaterial_velocity[9] = t1[8];   MB_Para.mb_cMaterial_velocity[10] = t1[11];  MB_Para.mb_cMaterial_velocity[11] = t1[10];
    MB_Para.mb_cMaterial_velocity[12] = t1[13]; MB_Para.mb_cMaterial_velocity[13] = t1[12]; MB_Para.mb_cMaterial_velocity[14] = t1[15];  MB_Para.mb_cMaterial_velocity[15] = t1[14];
    
    MB_Para.mb_fTempCompFactor = HRT_Para.h_fTempCompFactor;
    MB_Para.mb_fTempCompRefTemp = HRT_Para.h_fTempCompRefTemp;
}

