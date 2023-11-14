#ifndef _PIC24F_GLOBAL_H
#define	_PIC24F_GLOBAL_H

#define FPER_CLK	4000000UL 
#define FCY         (FPER_CLK/2)

#define _DI()		__asm__ volatile("disi #0x3FFF")
#define _EI()		__asm__ volatile("disi #0")

#endif //_PIC24F_GLOBAL_H