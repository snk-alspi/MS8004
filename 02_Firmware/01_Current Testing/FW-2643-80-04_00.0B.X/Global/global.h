
#ifndef __CENTERO_GLOBAL_H_
#define __CENTERO_GLOBAL_H_

#include "typedef.h"

/**A macro that returns the maximum of a and b */
#define MAX_VALUE(x, y) ((x > y) ? (x) : (y))

/**A macro that returns the minimum of a and b */
#define MIN_VALUE(x, y) ((x < y) ? (x) : (y))


void InverseMemcpy (uint8* p_pDst, uint8* p_pSrc, uint8 p_ucSize);
void Ram2Ram(unsigned char *src,unsigned char *dest,unsigned char count);
void Ram2RamR(unsigned char *src,unsigned char *dest,unsigned char count);
void Rom2Ram(const unsigned char *src,unsigned char *dest,unsigned char count);


#endif // __CENTERO_GLOBAL_H_
