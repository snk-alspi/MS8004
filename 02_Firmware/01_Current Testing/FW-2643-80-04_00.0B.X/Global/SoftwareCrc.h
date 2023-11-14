/*************************************************************************
 * File: SoftwareCrc.h
 * Description: Calculate a software CRC (Modbus CRC-16)
 *************************************************************************/

#ifndef __CENTERO_SOFTWARECRC16_H_
#define __CENTERO_SOFTWARECRC16_H_
 
#include "typedef.h"

uint16 SoftwareCrc16_Calculate(const uint8 *Buffer, uint16 BufferSize);

#endif // __CENTERO_SOFTWARECRC16_H_
