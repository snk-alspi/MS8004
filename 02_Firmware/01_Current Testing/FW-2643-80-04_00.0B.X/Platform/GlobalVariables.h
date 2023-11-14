#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#define T3_on IEC0bits.T3IE = 1;
#define T3_off IEC0bits.T3IE = 0;

 typedef struct {
    unsigned char channel;
    unsigned long mb_ulCal_0_offset;
    char mb_cMaterial_velocity[17];
    unsigned long mb_ulA_scan_start;
    unsigned int mb_uiPulsar_width;
    unsigned int mb_uiGain;
    unsigned long mb_ulG1_start;
    unsigned long mb_ulG1_width;
    int mb_uiG1_thershold;
    unsigned long mb_ulG2_start;
    unsigned long mb_ulG2_width;
    int mb_uiG2_thershold;
    unsigned int mb_uiMeasurement_mode;
    float mb_fTempCompRefTemp;
    float mb_fTempCompFactor;
} MBUT_parameters;

typedef struct {
    unsigned char channel;
    float h_fCal_0_offset;
    float h_fMaterial_velocity;
    float h_fA_scan_start;
    unsigned int h_uiPulsar_width;
    unsigned int h_uiGain;
    float h_fG1_start;
    float h_fG1_width;
    float h_fG1_thershold;
    float h_fG2_start;
    float h_fG2_width;
    float h_fG2_thershold;
    unsigned int h_uiMeasurement_mode;
    float h_fTempCompRefTemp;
    float h_fTempCompFactor;
} HRTUT_parameters;



#define  BYTE0(expression) ((unsigned char *) &(expression))[0]
#define  BYTE1(expression) ((unsigned char *) &(expression))[1]
#define  BYTE2(expression) ((unsigned char *) &(expression))[2]
#define  BYTE3(expression) ((unsigned char *) &(expression))[3]

//--------------------------------------------------------


extern volatile unsigned char mcnt;
extern unsigned char m_ucHartReqInProgress;
extern volatile unsigned char g_ucMeasurementInProgress;
extern unsigned char c180_stat, c181_stat, c183_stat,c185_stat, c190_stat, c191_stat, c193_stat;
extern unsigned char uc_Mes_finish_cnt;
// SETTINGS //



extern unsigned char mb_tx_buff[100], mb_rx_buff[100];
extern unsigned char tx_count, tx_counter, timeout, remeasure, rx_count;
extern volatile unsigned char mes_stat, mes_tx_stat, mes_rx_stat, mes_err_stat;
extern volatile unsigned char mb_comm_stat, mb_stat;
extern float mat_thick_raw_0, mat_thick_TC_0, mat_temp_0, dsi_t_0;
extern float mat_thick_raw_1, mat_thick_TC_1, mat_temp_1, dsi_t_1;
extern float mat_thick_raw_2, mat_thick_TC_2, mat_temp_2, dsi_t_2;
extern float mat_thick_raw_3, mat_thick_TC_3, mat_temp_3, dsi_t_3;
extern char dat[75];
extern volatile unsigned char mes_para_stat, mes_tcp_stat;
extern unsigned char ch, last_ch;


extern unsigned int crcr;
//extern unsigned char Mes_Mode;
//extern int  G1_Thers, G2_Thers;
//extern unsigned int Pulser_width, Gain;
//extern unsigned long Cal_0_Offset, aScan, G1_Strt, G1_Width, G2_Strt, G2_Width;
//extern unsigned char MesMode;
//extern char Mat_velocity[18];
//extern float Ref_temp, Comp_factor;



#define Pwr_MES_Enb() LATBbits.LATB13=0     // POWER UP USB & ANALOG BOARDS
#define Pwr_MES_Dis() LATBbits.LATB13=1     // POWER DOWN USB & ANALOG BOARDS // to be changed

#define UNLOCK_IO_REGISTERS() __builtin_write_OSCCONL(OSCCON & 0xbf)  //__builtin_write_OSCCONL(OSCCON & 0xA8)
#define LOCK_IO_REGISTERS()   __builtin_write_OSCCONL(OSCCON | 0x40)

#endif /*GLOBALVARIABLES_H*/