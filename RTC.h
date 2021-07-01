#ifndef __RTC_H
#define __RTC_H

#include "cmsis_os2.h" 
#include "stm32f4xx_hal.h"


extern RTC_HandleTypeDef hrtc;
void resetHoraFecha0(void);
void setHora(uint8_t seg, uint8_t min, uint8_t ho);
void setFecha(uint8_t dia, uint8_t mes,  uint16_t anio); 
uint8_t getSeg(void);
uint8_t getMin(void);
uint8_t getHora(void);
uint8_t getDia(void);
uint8_t getMes(void);
uint16_t getAnio(void);
void IAlarma(void);
void initRTC(void);
void RTC_IRQHandler (void);
extern osThreadId_t TID_Rtc_led;
extern osThreadId_t TID_Rtc_setTime;
extern osThreadId_t TID_Rtc_setDate;
uint32_t getTotalSeconds(void);
#endif