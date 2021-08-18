/**
  ******************************************************************************
  * @file    Templates/Src/HTTP_Server.c 
  * @author  MCD Application Team
  * @brief   Fichero donde se realiza la gestión de los hilos. En ellos se realiza
	*					 la obtención de la hora del servidor SNTP. Esto se realiza con un 
	*					 Timer que obtiene la hora cada 15s. Tambien se establece la hora en 
	*					 el RTC cuando se establece en la página web.
	*
  * @note    modified by ARM
  *          The modifications allow to use this file as User Code Template
  *          within the Device Family Pack.
  ******************************************************************************
  *
  ******************************************************************************
  */

#include <stdio.h>
#include "RTC.h"
#include "main.h"
#include <time.h>
#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "Watchdog.h"


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

 extern void     netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len);
													 
/* Thread IDs */
osThreadId_t TID_Rtc_setTime;
osThreadId_t TID_Rtc_setDate;

													 
/* Thread declarations */
__NO_RETURN void app_main (void *arg);
__NO_RETURN static void Rtc_setTime  (void *arg);
__NO_RETURN static void Rtc_setDate  (void *arg);


static void Timer_SNTP_callback(void *args);
int getNumber(char caracter);


/* IP address change notification */
void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len) {

  (void)if_num;
  (void)val;
  (void)len;

  if (option == NET_DHCP_OPTION_IP_ADDRESS) {
    /* IP address change, trigger LCD update */
  }
}

/**
  * @brief Hilo de gestión de la hora donde se espera a la actualzación de la hora desde la página web y
	*				 se establece en el RTC.
	* @param arg
  * @retval None
  */
static __NO_RETURN void Rtc_setTime (void *arg) {

	char date[8];
	uint8_t  seg, min, hor ;
	uint32_t flag;
	
	while(1){

		/*Se espera a recibir el flag de que se ha establecido la hora desde la página web*/
		flag = osThreadFlagsWait (0x20U, osFlagsWaitAny, 100);
		
		if (flag == 0x20){
			/*Establece la hora en el RTC*/
			sprintf (date, "%s",time_text[0]);
			date[2] = date[5] = '\0';
			hor=10*getNumber(date[0])+getNumber(date[1]);
			min=10*getNumber(date[3])+getNumber(date[4]);
			seg=10*getNumber(date[6])+getNumber(date[7]);
			setHora(seg, min, hor);
		}
		reset_Watchdog();
	}
}

/**
  * @brief Hilo de gestión de la fecha donde se espera a la actualzación de la fecha desde la página web y
	*				 se establece en el RTC.
	* @param arg
  * @retval None
  */
static __NO_RETURN void Rtc_setDate (void *arg) {
	uint32_t flag;
	char date[10];
	uint8_t  dia, mes;
	uint16_t anio;
	
	while(1){

		/*Se espera a recibir el flag de que se ha establecido la fecha desde la página web*/
		flag = osThreadFlagsWait (0x20U, osFlagsWaitAny, 100);
			
		if (flag == 0x20){
			
			/*Establece la fecha en el RTC*/
			sprintf (date, "%s",time_text[1]);
			date[2] = date[5] = '\0';
			dia=10*getNumber(date[0])+getNumber(date[1]);
			if (dia > 31)
				dia = 1;
			mes=10*getNumber(date[3])+getNumber(date[4]);
			if (mes > 12 || mes < 1)
				mes = 1;
			anio=10*getNumber(date[8])+getNumber(date[9]);
			setFecha(dia, mes, anio);
		}
		reset_Watchdog();
	}
}


/**
  * @brief Hilo main donde se incializac el RTC y se establece la hora incial. Además, se crean los hilos y el Timer. 
	* @param arg
  * @retval None
  */
__NO_RETURN void app_main (void *arg) {
  (void)arg;
	uint32_t exec1=1U;
	uint32_t time;
	
	/*Inicialización del comonente de red*/
  netInitialize ();
	/*Inicializació del RTC*/
	MX_RTC_Init();
	/*Se establece la hora y fecha incial*/
	setHora(00,20,13);
	setFecha(29,07,21);
	
	/*Se crean los hilos*/
	TID_Rtc_setTime		= osThreadNew (Rtc_setTime, 		 NULL, NULL);
	TID_Rtc_setDate		= osThreadNew (Rtc_setDate, 		 NULL, NULL);
	
  osThreadExit();
}

/**
  * @brief Función que transforma el carácter en número.
	* @param arg
  * @retval None
  */
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