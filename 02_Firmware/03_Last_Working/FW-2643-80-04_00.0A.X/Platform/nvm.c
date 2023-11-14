#include "nvm.h"
#include "xc.h"
#include "../Global/provision.h"
#include <string.h>

unsigned char g_aucFlashCopy[1536];
unsigned char g_aucPVFlashCopy[1536];

void NVMErasePage(uint32_t p_ulAddress) {
    // Erases 512 instructions at aligned address u32Addr
    // Erase page is 8 rows, or 64 * 8 = 512 instructions, or 64 * 8 * 2 = 1024 addresses, or 64 * 8 * 3 = 1536 bytes
    p_ulAddress &= 0xFFFFFC00; // Mask to page boundary
    NVMCONbits.NVMOP = 0b0011; // 0b0011 => Memory page erase operation, ERASE = 1 (program or executive memory)
    NVMCONbits.WREN = 1;
    NVMADR = (uint16_t) p_ulAddress;
    NVMADRH = (uint16_t) (p_ulAddress >> 16);
    __builtin_disi(5);
    __builtin_write_NVM();
    while (NVMCONbits.WR == 1) {
        Nop();
    }
    NVMCONbits.WREN = 0;
}

uint32_t NVMReadWord(uint32_t p_ulAddress) {
    // Returns single 24-bit instruction at address u32Addr, padding it to 32 bits.
    uint32_t ulRead;

    TBLPAG = (uint16_t) (p_ulAddress >> 16);

    ulRead = __builtin_tblrdl((uint16_t) p_ulAddress);
    ulRead |= ((uint32_t) __builtin_tblrdh((uint16_t) p_ulAddress)) << 16;

    return ulRead;
}

void NVMReadRow(uint32_t p_ulAddress, uint8_t *p_paucBuffer) {
    // Reads 64 24-bit instructions starting at at aligned address p_ulAddress
    // One row is 128 addresses.
    int i;

    p_ulAddress &= 0xFFFFFF80; // 64 instruction words, or 128 addresses on PIC24
    TBLPAG = (uint16_t) (p_ulAddress >> 16);
    for (i = 0; i < 64; i++) {
        uint16 unWordH = 0, unWordL = 0;
        unWordL = __builtin_tblrdl((uint16_t) p_ulAddress);
        unWordH = __builtin_tblrdh((uint16_t) p_ulAddress);

        *p_paucBuffer++ = LOW(unWordH);
        *p_paucBuffer++ = HIGH(unWordL);
        *p_paucBuffer++ = LOW(unWordL);
        p_ulAddress += 2;
    }
}

void NVMWriteRow(uint32_t p_ulAddress, uint8_t *p_paucBuffer) {
    // Writes 64 24-bit instructions starting at at aligned address p_ulAddress
    // p_paucBuffer is a pointer to write to flash
    int i;
    uint16 temp = 0;
    uint16 temp1 = 0;

    p_ulAddress &= 0xFFFFFF80; // 64 instruction words
    NVMCONbits.NVMOP = 0b0010; // 0b0010 => Memory row program operation, ERASE = 0
    NVMCONbits.WREN = 1;
    TBLPAG = 0xFA; // DS30010073D-page 26, high address of flash write latches (0xFA0000 - 0xFA007E)

    NVMADR = (uint16_t) p_ulAddress;
    NVMADRH = (uint16_t) (p_ulAddress >> 16);
    // Fill in the memory mapped row
    for (i = 0; i < 64; i++) {
        temp = (uint16) (*p_paucBuffer) & 0x00FF;
        p_paucBuffer++;
        temp1 = (uint16) (*p_paucBuffer) << 8;
        p_paucBuffer++;
        temp1 += ((uint16) (*p_paucBuffer) & 0x00FF);
        p_paucBuffer++;

        __builtin_tblwtl(i * 2, temp1);
        __builtin_tblwth(i * 2, temp);
    }
    __builtin_disi(5);
    __builtin_write_NVM();
    while (NVMCONbits.WR == 1) {
        Nop();
    }
    NVMCONbits.WREN = 0;
}

void NVMWritePage(uint32_t p_ulAddress, uint8_t* p_paucBuffer) {
    NVMErasePage(p_ulAddress);
    unsigned char counter = 0;
    do {
        NVMWriteRow(p_ulAddress, p_paucBuffer + counter * 128);
        p_ulAddress += 128;
        counter++;
    } while (counter < 8);
}

void NVMReadPage(uint32_t p_ulAddress, uint8_t* p_paucBuffer) {
    unsigned char counter = 0;
    do {
        NVMReadRow(p_ulAddress, p_paucBuffer + counter*128);
        p_ulAddress += 128;
        counter++;
    } while (counter < 8);
}

void NVM_WritePersistentData(uint32 p_ulAddress) {
    if(memcmp(g_aucFlashCopy, (uint8*)&g_stProv, (unsigned int)(&((PROVISION_T*)0)->m_stRadioCharge))) // ignore statistics area
    {
        NVM_WritePersistentDataWithStatistics(p_ulAddress);
    }
}

void NVM_WritePersistentDataWithStatistics(uint32 p_ulAddress) {
    int current_cpu_ipl;

    SET_AND_SAVE_CPU_IPL(current_cpu_ipl, 7);
    if(memcmp(g_aucFlashCopy, (uint8*)&g_stProv, sizeof(g_stProv)))
    {
       memcpy(g_aucFlashCopy, (uint8*)&g_stProv, sizeof (g_stProv));    
       NVMWritePage(p_ulAddress, g_aucFlashCopy);
    }

    RESTORE_CPU_IPL(current_cpu_ipl);
}


void NVM_WriteProcessPersistentData(uint32 p_ulAddress) {
    int current_cpu_ipl;

    SET_AND_SAVE_CPU_IPL(current_cpu_ipl, 7);
    if(memcmp(g_aucPVFlashCopy, (uint8*)&g_stDeviceProcessVariables, sizeof(g_stDeviceProcessVariables)))
    {
       memcpy(g_aucPVFlashCopy, (uint8*)&g_stDeviceProcessVariables, sizeof (g_stDeviceProcessVariables));    
       NVMWritePage(p_ulAddress, g_aucPVFlashCopy);
    }

    RESTORE_CPU_IPL(current_cpu_ipl);
}
