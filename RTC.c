#include "RTC.h"
#include "mktime.h"


RTC_HandleTypeDef hrtc;
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
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
	return sTime.Seconds;
	
}
uint8_t getMin(void){
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
  return sTime.Minutes;
}
uint8_t getHora(void){
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
  return sTime.Hours;
}
uint8_t getDia(void){
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
  return sDate.Date;
}
uint8_t getMes(void){
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
  return sDate.Month;
}
uint16_t getAnio(void){
	uint16_t ye;
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
	ye = sDate.Year;
  return ye;
}
//devuelves segundos pasados desde 1 de enero de 1970
uint32_t getTotalSeconds(void){
uint32_t segundos;
	segundos=unix_time_in_seconds(getSeg(), getMin(), getHora(), getDia(), getMes(), getAnio()); 
	return segundos;
}

void MX_RTC_Init(void)
{
  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};
  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    printf("Fallo en la inicializacion");
  }
  /** Enable Calibrartion
  */
  if (HAL_RTCEx_SetCalibrationOutPut(&hrtc, RTC_CALIBOUTPUT_1HZ) != HAL_OK)
  {
        printf("Fallo en la calibracion");

  }
	/*sTime.Hours = 0x10;
  sTime.Minutes = 0x20;
  sTime.Seconds = 0x30;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
     printf("Fallo en la calibracion");
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JULY;
  sDate.Date = 0x12;
  sDate.Year = 0x21;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
     printf("Fallo en la calibracion");
  }*/
}

