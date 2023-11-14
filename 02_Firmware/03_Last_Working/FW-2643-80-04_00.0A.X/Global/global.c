/*************************************************************************
 * File: 
 * Description: 
 *************************************************************************/

#include "typedef.h"

/**
 * Inverse memcpy function
 * @param[in] p_pDst
 * @param[in] p_pSrc
 * @param[in] p_ucSize
 */
void InverseMemcpy (uint8* p_pDst, uint8* p_pSrc, uint8 p_ucSize)
{
  uint8 *d = p_pDst;
  uint8 *s = p_pSrc;
  s += p_ucSize - 1;
  while(p_ucSize--)
  {
    *(d++) = *(s--);
  }
}

/**
 * Transfers the data from ROM to RAM
 * @param[in] *src - pointer to the variable where the data will be copied
 * @param[in] *dest - pointer to the variable from where the data will be copied
 * @param[in] count - number of bytes to be copied
 */
void Rom2Ram(const unsigned char *src,unsigned char *dest,unsigned char count)
{
  unsigned char i;
  for(i=0;(i < count);i++){
	*(dest+i) = *(src+i);
   }
}

/**
 * Transfers the data from RAM to RAM
 * @param[in] *src - pointer to the variable where the data will be copied
 * @param[in] *dest - pointer to the variable from where the data will be copied
 * @param[in] count - number of bytes to be copied
 */
void Ram2Ram(unsigned char *src,unsigned char *dest,unsigned char count)
{
  unsigned char i;
  for(i=0;(i < count);i++){
	*(dest+i) = *(src+i);
   }
}

/**
 * Transfers the data from RAM to RAM in reverse order
 * @param[in] *src - pointer to the variable where the data will be copied
 * @param[in] *dest - pointer to the variable from where the data will be copied
 * @param[in] count - number of bytes to be copied
 */
void Ram2RamR(unsigned char *src,unsigned char *dest,unsigned char count)
{
  unsigned char i,j;
  for(i=0,j=count-1;(i < count);i++,j--){
	*(dest+i) = *(src+j);
   }
}

