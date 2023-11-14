/* 
 * File:   watchdog.h
 * Author: Cristian
 *
 * Created on June 23, 2021, 3:38 PM
 */

#ifndef _WATCHDOG_H_
#define	_WATCHDOG_H_

#include "xc.h"
#include "../Global/typedef.h"

#ifdef	__cplusplus
extern "C" {
#endif

    void Watchdog_Init(void);
    void Watchdog_Feed(void);



#ifdef	__cplusplus
}
#endif

#endif	/* WATCHDOG_H */

