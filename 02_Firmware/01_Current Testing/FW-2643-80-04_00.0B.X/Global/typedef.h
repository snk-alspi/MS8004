/* 
 * File:   typedef.h
 * Author: Cristian
 *
 * Created on June 23, 2021, 2:32 PM
 */

#ifndef TYPEDEF_H
#define	TYPEDEF_H

#ifdef	__cplusplus
extern "C" {
#endif

    // Types definition
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

typedef signed char     int8;
typedef unsigned char   uint8;
typedef signed short    int16;
typedef unsigned short  uint16;
typedef signed long     int32;
typedef unsigned long   uint32;
typedef float           fp32;
typedef unsigned long long uint64;

typedef void (* handler_func_type)(void);

typedef uint16 HANDLE;
typedef uint16 SHORT_ADDR;
typedef uint8 LONG_ADDR[8];
typedef uint8 IPV6_ADDR[16];
typedef uint8 EUI64_ADDR[8];
typedef uint32 TIME;


/** Boolean type.*/
typedef enum
{
  FALSE = 0, /*!< 0 */
  TRUE  = 1 /*!< 1 */
} __attribute__((packed)) BOOL;

#define Boolean BOOL

/*
 * NULL definition
 */
#ifndef NULL
  #define NULL (void *)(0)
#endif


/** Structure member size */
#define mem_size(type, member) 	(	sizeof(((type *)0)->member) 	)

/** Structure member index */
#define mem_index(type, member) ( (uint16) &(((type *)0)->member) )

/**
*Global status type ... This type should be maybe extended.
*/
typedef enum
{
    SUCCESS           = 0, /*!< 0 */
    UNSPECIFIED_ERROR = 1, /*!< 1 */
    //  READ_ONLY,
    WRITE_ONLY        = 3, /*!< 3 */
    INVALID_PARAMETER = 4, /*!< 4 */
    INVALID_ADDRESS   = 5, /*!< 5 */
    DUPLICATE         = 6, /*!< 6 */
    OUT_OF_MEMORY     = 7, /*!< 7 */
    UID_OUT_OF_RANGE  = 8, /*!< 8 */
    DATA_TOO_LONG     = 9, /*!< 9 */
    NO_CHANNEL        = 10, /*!< 10 */
    NO_TIMESLOT       = 11, /*!< 11 */
    //  NO_NEIGHBOR,
    //  NO_SUPERFRAME,
    NO_LINK           = 12, /*!< 12 */
    RX_LINK           = 13, /*!< 13 */
    TX_LINK           = 14, /*!< 14 */
    NO_ROUTE          = 15, /*!< 15 */
    NO_CONTRACT       = 16, /*!< 16 */
    NO_UID            = 17, /*!< 17 */
    TOO_BIG           = 18, /*!< 18 */
    QUEUE_FULL        = 19, /*!< 19 */
    NACK              = 20, /*!< 20 */
    TIMEOUT           = 21, /*!< 21 */
    MSG_NOT_FOUND     = 22, /*!< 22 */
    NO_GRAPH          = 23, /*!< 23 */
    VALUE_OUT_OF_RANGE = 24, /*!< 24 */
    INVALID_SC        = 25 /*!< 25 */
} __attribute__((packed)) SC;



#ifdef	__cplusplus
}
#endif

#endif	/* TYPEDEF_H */

