/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server.c
 * Purpose: HTTP Server example
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "RTC.h"
#include "main.h"
#include <time.h>
#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE
#include "SNTP.h"
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
//#include "Board_LED.h"                  // ::Board Support:LED
//#include "Board_Buttons.h"              // ::Board Support:Buttons
//#include "Board_ADC.h"                  // ::Board Support:A/D Converter
//#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
//#include "GLCD_Config.h"                // Keil.MCBSTM32F400::Board Support:Graphic LCD

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
static uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk),
};
extern char time_text[2][20+1];
char time_text[2][20+1] = { "XX:XX:XX",
                           "XX/XX/XXXX" };
/*extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

extern uint16_t AD_in          (uint32_t ch);
extern uint8_t  get_button     (void);


extern bool LEDrun;
extern char lcd_text[2][20+1];

extern osThreadId_t TID_Display;
extern osThreadId_t TID_Led;

bool LEDrun;
char lcd_text[2][20+1] = { "LCD line 1",
                           "LCD line 2" };
*/
 extern void     netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len);
/* Thread IDs */
//osThreadId_t TID_Display;
//osThreadId_t TID_Led;
osThreadId_t TID_Rtc_setTime;
osThreadId_t TID_Rtc_setDate;
osThreadId_t TID_SNTP;
osTimerId_t Timer_SNTP;
/* Thread declarations */
//static void BlinkLed (void *arg);
//static void Display  (void *arg);

__NO_RETURN void app_main (void *arg);
__NO_RETURN static void Rtc_setTime  (void *arg);
__NO_RETURN static void Rtc_setDate  (void *arg);
__NO_RETURN static void SNTP_thread(void*args);

static void Timer_SNTP_callback(void *args);
int getNumber(char caracter);
/* Read analog inputs */
uint16_t AD_in (uint32_t ch) {
 /* int32_t val = 0;

  if (ch == 0) {
    ADC_StartConversion();
    while (ADC_ConversionDone () < 0);
    val = ADC_GetValue();
  }
  return ((uint16_t)val);/
	*/
}

/* Read digital inputs */
uint8_t get_button (void) {
  //return ((uint8_t)Buttons_GetState ());
}

/* IP address change notification */
void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len) {

  (void)if_num;
  (void)val;
  (void)len;

  if (option == NET_DHCP_OPTION_IP_ADDRESS) {
    /* IP address change, trigger LCD update */
   // osThreadFlagsSet (TID_Display, 0x01);
  }
}

/*----------------------------------------------------------------------------
  Thread 'Display': LCD display handler
 *---------------------------------------------------------------------------*/
//static __NO_RETURN void Display (void *arg) {
//  static uint8_t ip_addr[NET_ADDR_IP6_LEN];
//  static char    ip_ascii[40];
//  static char    buf[24];
//  uint32_t x = 0;

//  (void)arg;

//  /*GLCD_Initialize         ();
//  GLCD_SetBackgroundColor (GLCD_COLOR_BLUE);
//  GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
//  GLCD_ClearScreen        ();
//  GLCD_SetFont            (&GLCD_Font_16x24);
//  GLCD_DrawString         (x*16U, 1U*24U, "       MDK-MW       ");
//  GLCD_DrawString         (x*16U, 2U*24U, "HTTP Server example ");

//  GLCD_DrawString (x*16U, 4U*24U, "IP4:Waiting for DHCP");*/

//  /* Print Link-local IPv6 address */
//  netIF_GetOption (NET_IF_CLASS_ETH,
//                   netIF_OptionIP6_LinkLocalAddress, ip_addr, sizeof(ip_addr));

//  netIP_ntoa(NET_ADDR_IP6, ip_addr, ip_ascii, sizeof(ip_ascii));

//  sprintf (buf, "IP6:%.16s", ip_ascii);
//  //GLCD_DrawString ( x    *16U, 5U*24U, buf);
//  sprintf (buf, "%s", ip_ascii+16);
// // GLCD_DrawString ((x+10U)*16U, 6U*24U, buf);

//  while(1) {
//    /* Wait for signal from DHCP */
//    osThreadFlagsWait (0x01U, osFlagsWaitAny, osWaitForever);

//    /* Retrieve and print IPv4 address */
//    netIF_GetOption (NET_IF_CLASS_ETH,
//                     netIF_OptionIP4_Address, ip_addr, sizeof(ip_addr));

//    netIP_ntoa (NET_ADDR_IP4, ip_addr, ip_ascii, sizeof(ip_ascii));

//    sprintf (buf, "IP4:%-16s",ip_ascii);
//   // GLCD_DrawString (x*16U, 4U*24U, buf);

//    /* Display user text lines */
//  //  sprintf (buf, "%-20s", lcd_text[0]);
//  //  GLCD_DrawString (x*16U, 7U*24U, buf);
//  //  sprintf (buf, "%-20s", lcd_text[1]);
//  ///  GLCD_DrawString (x*16U, 8U*24U, buf);
//  }
//}

///*----------------------------------------------------------------------------
//  Thread 'BlinkLed': Blink the LEDs on an eval board
// *---------------------------------------------------------------------------*/
//static __NO_RETURN void BlinkLed (void *arg) {
//  const uint8_t led_val[16] = { 0x48,0x88,0x84,0x44,0x42,0x22,0x21,0x11,
//                                0x12,0x0A,0x0C,0x14,0x18,0x28,0x30,0x50 };
//  uint32_t cnt = 0U;

//  (void)arg;

// // LEDrun = true;
//  while(1) {
//    /* Every 100 ms */
//  /*  if (LEDrun == true) {
//      LED_SetOut (led_val[cnt]);
//      if (++cnt >= sizeof(led_val)) {
//        cnt = 0U;
//      }
//    }
//    osDelay (100);*/
//  }
//}
static __NO_RETURN void Rtc_setTime (void *arg) {

	char date[8];
	uint8_t  seg, min, hor ;
	while(1){

	osThreadFlagsWait (0x20U, osFlagsWaitAny, osWaitForever);
	sprintf (date, "%s",time_text[0]);
	date[2] = date[5] = '\0';
	hor=10*getNumber(date[0])+getNumber(date[1]);
	min=10*getNumber(date[3])+getNumber(date[4]);
	seg=10*getNumber(date[6])+getNumber(date[7]);
	setHora(seg, min, hor);
	}
	

}
static __NO_RETURN void Rtc_setDate (void *arg) {

	char date[10];
	uint8_t  dia, mes;
	uint16_t anio;
	while(1){

	osThreadFlagsWait (0x20U, osFlagsWaitAny, osWaitForever);
	sprintf (date, "%s",time_text[1]);
	date[2] = date[5] = '\0';
	dia=10*getNumber(date[0])+getNumber(date[1]);
	mes=10*getNumber(date[3])+getNumber(date[4]);
	anio=10*getNumber(date[8])+getNumber(date[9]);
	setFecha(dia, mes, anio);
	}
}


/*----------------------------------------------------------------------------
  TIMER_SNTP, gets time every 3 minutes.
 *---------------------------------------------------------------------------*/
static  void Timer_SNTP_callback(void *args){

	osThreadFlagsSet (TID_SNTP, 0x04);
}

/*----------------------------------------------------------------------------
  SNTP_Thread, gets date every 3 min
 *---------------------------------------------------------------------------*/
static __NO_RETURN void SNTP_thread(void *args){

	while(1) {
		osThreadFlagsWait (0x04U, osFlagsWaitAny, osWaitForever);
		get_time (); 
		osThreadFlagsWait (0x10U, osFlagsWaitAny, osWaitForever);
//     if(!LEDrun) 
//			LED_SetOut (0x04);
//		osThreadFlagsWait (0x80U, osFlagsWaitAny, 1000);
//		if(!LEDrun) 
//		 LED_SetOut (0x00);
  }
}


/*----------------------------------------------------------------------------
  Main Thread 'main': Run Network
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  (void)arg;
	uint32_t exec1=1U;
	uint32_t time;
  //LED_Initialize();
 // Buttons_Initialize();
  //ADC_Initialize();

  netInitialize ();
	MX_RTC_Init();
	setHora(10,20,11);
	setFecha(10,05,15);
	TID_Rtc_setTime		= osThreadNew (Rtc_setTime, 		 NULL, NULL);
	TID_Rtc_setDate		= osThreadNew (Rtc_setDate, 		 NULL, NULL);
	TID_SNTP				= osThreadNew (SNTP_thread, NULL, NULL); 

  //TID_Led     = osThreadNew (BlinkLed, NULL, NULL);
  //TID_Display = osThreadNew (Display,  NULL, NULL);
	Timer_SNTP=osTimerNew(Timer_SNTP_callback, osTimerPeriodic, &exec1, NULL);
	 if (Timer_SNTP != NULL)  {
    time = 15000U; //15s 
	  osStatus_t status = osTimerStart(Timer_SNTP, time);       // start timer
  }	
	 osThreadFlagsWait (0x80U, osFlagsWaitAny, 13000);//Esperamos 13 segundoss
	 osThreadFlagsSet (TID_SNTP, 0x04);//mandamos señal para actualizar hora del sntp

  osThreadExit();
}

int getNumber(char caracter){
	int ret=0;
	switch (caracter){
		case '1':
			ret= 1;
			break;
		case '2':
			ret= 2;
			break;
		case '3':
			ret= 3;
				break;
		case '4':
			ret= 4;
				break;
		case '5':
			ret= 5;
				break;
		case '6':
			ret= 6;
				break;
		case '7':
			ret= 7;
				break;
		case '8':
			ret= 8;	
				break;
		case '9':
			ret= 9;			
				break;
		default: 
			ret= 0;
			break;
	}
	return ret;
}