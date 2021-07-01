#include "RTC.h"
#include <time.h>
#include <stdio.h>
#include "mktime.h"
uint8_t valor =1;
RTC_DateTypeDef sDate; 
RTC_TimeTypeDef sTime;

#define RTCCIF	0
#define RTCALF	1
#define s		0
#define mi	0
#define h		0
#define d		1
#define m		1
#define y		2020 //desde 1970
#define dw		1
#define wy		1


void resetHoraFecha0(void){
 setHora(s,mi,h);
	setFecha(d,m,y);
}
void setHora(uint8_t seg, uint8_t min, uint8_t hor){
	sTime.Hours = hor; // set hours 
  sTime.Minutes = min; // set minutes 
  sTime.Seconds = seg; // set seconds 
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE; 
  sTime.StoreOperation = RTC_STOREOPERATION_RESET; 
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) 
  { 
    //Error_Handler(__FILE__, __LINE__); 
  } 
}

void setFecha(uint8_t dia, uint8_t mes, uint16_t anio ){
	sDate.WeekDay = RTC_WEEKDAY_SATURDAY; // day 
  sDate.Month = mes; // month 
  sDate.Date = dia; // date 
  sDate.Year = anio; // year 
	 if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) 
  { 
    //_Error_Handler(__FILE__, __LINE__); 
  } 
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);
}
uint8_t getSeg(void){
return sTime.Seconds;
}
uint8_t getMin(void){
  return sTime.Minutes;
}
uint8_t getHora(void){
  return sTime.Hours;
}
uint8_t getDia(void){
  return sDate.Date;
}
uint8_t getMes(void){
  return sDate.Month;
}
uint16_t getAnio(void){
  return sDate.Year;
}
//devuelves segundos pasados desde 1 de enero de 1970
uint32_t getTotalSeconds(void){
uint32_t segundos;
	segundos=unix_time_in_seconds(getSeg(), getMin(), getHora(), getDia(), getMes(), getAnio()); 
	return segundos;
}


void IAlarma(void){

}
/*void RTC_IRQHandler (void){ 
  if(LPC_RTC->ILR==(1<<RTCALF)){//la de 1min
    LPC_RTC->ILR|=(1<<RTCALF);
		osThreadFlagsSet (TID_Rtc_led, 0x02);
	//Enviar señal para el led
  }else if(LPC_RTC->ILR==(1<<RTCCIF)){
    LPC_RTC->ILR|=(1<<RTCCIF);
	//Enviar señal para el lcd
		osThreadFlagsSet (TID_Rtc_setTime, 0x01);
		
	}

}
*/
