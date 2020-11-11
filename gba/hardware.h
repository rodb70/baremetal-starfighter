/*
 * hardware.h
 *
 *  Created on: 10 Nov 2020
 *      Author: rod
 */

#ifndef GBA_HARDWARE_H_
#define GBA_HARDWARE_H_
#include "mylib.h"

#define BUTTONS (*(vuint16_t*)0x04000130)

#define REG_DISPCNT *((uint16_t*) 0x4000000)
#define SCANLINECOUNTER (*(vuint16_t *)0x4000006)

#define BG2_ENABLE (1 << 10)
#define MODE3 3

static inline void hardware_video_init( void )
{
    REG_DISPCNT = MODE3 | BG2_ENABLE;
}


#endif /* GBA_HARDWARE_H_ */
