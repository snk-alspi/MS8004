/* 
 * File:   nvm.h
 * Author: Cristian
 *
 * Created on June 30, 2021, 12:28 PM
 */

#ifndef _NVM_H_
#define	_NVM_H_

#include "xc.h"
#include "../Global/typedef.h"


#ifdef	__cplusplus
extern "C" {
#endif

#define NVM_START_ADDRESS  0x1F000
#define MEM_SIGNATURE   0xAA55A5A5UL
#define NVM_DEVICE_PV_START_ADDRESS 0x20000
    
//#define NVM_START_ADDRESS  0x12000
//#define MEM_SIGNATURE   0xAA55A5A5UL
//#define NVM_DEVICE_PV_START_ADDRESS 0x13000

//#define FLASH_ROWSIZE    64       //in number of instructions
//#define FLASH_ROWS_PER_PAGE 8
//#define FLASH_PAGEINSTR (FLASH_ROWSIZE * FLASH_ROWS_PER_PAGE)  //8 rows of 64 instructions
//#define FLASH_PAGESIZE (FLASH_PAGEINSTR*2)     //each instruction appears to occupy 2 program memory addresses
//#define FLASH_PAGEBYTES (FLASH_PAGEINSTR*3)  //total bytes is FLASH_PAGEINSTR *3 = 1536
//#define FLASH_ROWBYTES  (FLASH_ROWSIZE*3)    //bytes in one row
//
//#define ERASE_PAGE      0x4003
//#define PROGRAM_ROW     0x4002
//#define PROGRAM_DWORD   0x4001
//
//#define DWORD_LEN       0x06

    /* Low word of a 32 bit integer */
#define LOW_W(_x)	(uint16)((uint32)_x & 0x0000FFFF)

    /* High word of a 32 bit integer */
#define HIGH_W(_x)	(uint16)(((uint32)_x & 0xFFFF0000) >> 16)
    
    /* Low byte of a 16 bit integer */
#define LOW(_x)	(uint8)((uint16)_x & 0x00FF)

/* High byte of a 16 bit integer */
#define HIGH(_x)	(uint8)(((uint16)_x & 0xFF00) >> 8)
    
#define FLASH_UPDATE_NEEDED() NVM_WritePersistentData(NVM_START_ADDRESS)
#define FLASH_UPDATE_NEEDED_WITH_STAT() NVM_WritePersistentDataWithStatistics(NVM_START_ADDRESS)
#define DEVICE_PROCESS_VARIABLES_FLASH_UPDATE_NEEDED()NVM_WriteProcessPersistentData(NVM_DEVICE_PV_START_ADDRESS)


 extern unsigned char g_aucFlashCopy[1536];
 extern unsigned char g_aucPVFlashCopy[1536];
 extern volatile unsigned char g_ucFlashUpdatePending;

    void NVM_WritePersistentData(uint32 p_ulAddress);
    void NVM_WritePersistentDataWithStatistics(uint32 p_ulAddress);
    
    void NVM_WriteProcessPersistentData(uint32 p_ulAddress);
    void NVMErasePage(uint32_t p_ulAddress);
    uint32_t NVMReadWord(uint32_t p_ulAddress);
    void NVMReadRow(uint32_t p_ulAddress,uint8_t *p_paucBuffer);
    void NVMWriteRow(uint32_t p_ulAddress,uint8_t *p_paucBuffer);
    void NVMWritePage(uint32_t p_ulAddress,uint8_t *p_paucBuffer);
    void NVMReadPage(uint32_t p_ulAddress, uint8_t *p_paucBuffer);


#ifdef	__cplusplus
}
#endif

#endif	/* _NVM_H_ */
