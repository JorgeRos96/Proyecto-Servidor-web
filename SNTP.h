#ifndef __SNTP_H
#define __SNTP_H

#include "cmsis_os2.h"                  // CMSIS RTOS definitions
#include "rl_net.h"                     // Network definitions

extern const NET_ADDR4 ntp_server ;
void get_time (void);
static void time_callback (uint32_t seconds, uint32_t seconds_fraction);
extern void setFecha(uint8_t dia, uint8_t mes, uint16_t anio );
extern void setHora(uint8_t seg, uint8_t min, uint8_t hor);
extern osThreadId_t TID_SNTP; 
#endif
