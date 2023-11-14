

#include "GlobalVariables.h"
//#include "GlobalStructure.h"

HRTUT_parameters HRT_Para;
MBUT_parameters MB_Para;

volatile unsigned char mcnt;
unsigned char m_ucHartReqInProgress;
volatile unsigned char g_ucMeasurementInProgress;
unsigned char c180_stat, c181_stat, c183_stat,c185_stat, c190_stat, c191_stat, c193_stat;
unsigned char uc_Mes_finish_cnt;
//  SETTINGS ////////



unsigned char mb_tx_buff[100], mb_rx_buff[100];
unsigned char tx_count, tx_counter, timeout, remeasure, rx_count;
volatile unsigned char mes_stat, mes_tx_stat, mes_rx_stat, mes_err_stat;
volatile unsigned char mb_comm_stat, mb_stat;
float mat_thick_raw_0, mat_thick_TC_0, mat_temp_0, dsi_t_0;
float mat_thick_raw_1, mat_thick_TC_1, mat_temp_1, dsi_t_1;
float mat_thick_raw_2, mat_thick_TC_2, mat_temp_2, dsi_t_2;
float mat_thick_raw_3, mat_thick_TC_3, mat_temp_3, dsi_t_3;
char dat[75];
volatile unsigned char mes_para_stat, mes_tcp_stat;
unsigned char mes_buff_1[100], mes_buff_2[100];
unsigned char ch, last_ch;

unsigned int crcr;
//unsigned char Mes_Mode;
//int  G1_Thers, G2_Thers;
//unsigned int Pulser_width, Gain;  
//unsigned long Cal_0_Offset, aScan, G1_Strt, G1_Width, G2_Strt, G2_Width;
//unsigned char MesMode;
//char Mat_velocity[18];
//float Ref_temp, Comp_factor;









