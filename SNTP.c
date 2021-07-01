
#include "SNTP.h"
#include "cmsis_os2.h"                  // CMSIS RTOS definitions
#include "rl_net.h"
#include <time.h>
#include <stdio.h>
 const NET_ADDR4 ntp_server = { NET_ADDR_IP4, 0, 150, 214, 94, 5};
																										//	130.206.3.166
 static void time_callback (uint32_t seconds, uint32_t seconds_fraction);
 

 
void get_time(void) {
  if (netSNTPc_GetTime ((NET_ADDR *)&ntp_server, time_callback) == netOK) {
   printf ("SNTP request sent.\n");
  }
  else {
    printf ("SNTP not ready or bad parameters.\n");
  }
}
 
static void time_callback (uint32_t seconds, uint32_t seconds_fraction) {
	uint8_t seg, min, hor, dia, mes;
	uint16_t anio;
	time_t now=seconds+3600;//Para obtener utc+1
	struct tm *ts;

	if (seconds == 0) {
    printf ("Server not responding or bad response.\n");
  }else {
		ts=localtime(&now); 
		seg=ts->tm_sec;
		min= ts->tm_min;
		hor= ts->tm_hour;
		dia= ts->tm_mday;
		mes= (ts->tm_mon)+1;	//Enero =0 
		anio= (ts->tm_year)+1900; //Desde 1900;
		setHora(seg,  min,  hor);
		setFecha( dia,  mes, anio); 
		
		osThreadFlagsSet (TID_SNTP, 0x10);
  }
}
