/**
  ******************************************************************************
  * @file    Templates/Src/Flash.c 
  * @author  MCD Application Team
  * @brief   Fichero donde se realizan las funciones para la escritura y lectura
	*					 en la memoria Flash indicando los datos a escribir o leer y la dirección 
	*					 de memoria donde se quiere realizar la acción. Tambien se encuentra la 
	*					 función para obtener el sector en el a partir de la dirección de memoria.
	*
  * @note    modified by ARM
  *          The modifications allow to use this file as User Code Template
  *          within the Device Family Pack.
  ******************************************************************************
  *
  ******************************************************************************
  */

#include "Flash.h"

/**
  * @brief Función que realiza la escritura en la memoria Flash. Se introduce por párametro la direccion de memoria
	*				 donde se quiere comenazar la escritura, los datos a escribir y el tamaño de los datos. Se borra el sector
	*				 o secotres en el que se quiere escribir y se escribe palabra a palabra los datos en la memoria
	* @param StartSectorAddress: Dirección de memoria donde se quiere comenzar la escritura
	* @param *Data: Datos que se quieren escribir en memoria
	*	@param numberofwords: Indica el tamaño de los datos que se van a escribir en memoria
  * @retval Devuelve el estado en el que ha terminado la escritura en memoria (0 correcta)
  */
uint32_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords)
{

		static FLASH_EraseInitTypeDef EraseStruct;
		uint32_t SECTORError;
		int sofar=0;


		/* Unlock the Flash to enable the flash control register access *************/
	  HAL_FLASH_Unlock();
		
	  /* Erase the user Flash area */
		
	  /* Get the number of sector to erase from 1st sector */
	  uint32_t StartSector = GetSecNum(StartSectorAddress);
	  uint32_t EndSectorAddress = StartSectorAddress + numberofwords*4;
	  uint32_t EndSector = GetSecNum(EndSectorAddress);
		
	  /* Fill EraseInit structure*/
	  EraseStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	  EraseStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	  EraseStruct.Sector        = StartSector;
	  EraseStruct.NbSectors     = (EndSector - StartSector) + 1;

	  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
	     you have to make sure that these data are rewritten before they are accessed during code
	     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
	     DCRST and ICRST bits in the FLASH_CR register. */
	  if (HAL_FLASHEx_Erase(&EraseStruct, &SECTORError) != HAL_OK)
	  {
		  return HAL_FLASH_GetError ();
	  }

	  /*Se programa el area de memoria palabra a palabra*/
	   while (sofar<numberofwords)
	   {
	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartSectorAddress, Data[sofar]) == HAL_OK)
	     {
	    	 StartSectorAddress += 4;  // use StartPageAddress += 2 for half word and 8 for double word
	    	 sofar++;
	     }
	     else
	     {
	       /* Error occurred while writing data in Flash memory*/
	    	 return HAL_FLASH_GetError ();
	     }
	   }

	  /* Lock the Flash to disable the flash control register access (recommended
	     to protect the FLASH memory against possible unwanted operation) *********/
	  HAL_FLASH_Lock();

	   return 0;
}

/**
  * @brief Función que realiza la lectura en la memoria Flash. Se introduce por parámetro la direccion de memoria
	*				 donde se quieren leer los datos, el buffer donde se almacenan los datos leidos y el tamaño de los
	*				 datos. Y se guardan los datos desde la dirección de memoria indicada con el tamaño que se pasa por parámetro
	*		  	 en el buffer.
	* @param StartSectorAddress: Dirección de memoria donde se quiere comenzar la escritura
	* @param *Rxbuf: Puntero al buffer donde se almacena los datos leidos en memoria
	*	@param numberofwords: Indica el tamaño de los datos que se van a escribir en memoria
  * @retval None
  */
void Flash_Read_Data (uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords)
{
	while (numberofwords>0)
	{
		*RxBuf = *(__IO uint32_t *)StartSectorAddress;
		StartSectorAddress += 4;
		RxBuf++;
		numberofwords--;
	}
}

/**
  * @brief Función que devuelve el número del sector en el que se encuentra la dirección de memoria
	* @param addr: Dirección de memoria donde se quiere conocer el sector en el que se encuentra
  * @retval Devuelve el sector al que pertenece la dirección de memoria
  */
uint32_t GetSecNum (uint64_t addr)
{
	
	uint32_t sec = 0;

  if((addr < 0x08003FFF) && (addr >= 0x08000000))
  {
    sec = FLASH_SECTOR_0;
  }
  else if((addr < 0x08007FFF) && (addr >= 0x08004000))
  {
    sec = FLASH_SECTOR_1;
  }
  else if((addr < 0x0800BFFF) && (addr >= 0x08008000))
  {
    sec = FLASH_SECTOR_2;
  }
  else if((addr < 0x0800FFFF) && (addr >= 0x0800C000))
  {
    sec = FLASH_SECTOR_3;
  }
  else if((addr < 0x0801FFFF) && (addr >= 0x08010000))
  {
    sec = FLASH_SECTOR_4;
  }
  else if((addr < 0x0803FFFF) && (addr >= 0x08020000))
  {
    sec = FLASH_SECTOR_5;
  }
  else if((addr < 0x0805FFFF) && (addr >= 0x08040000))
  {
    sec = FLASH_SECTOR_6;
  }
  else if((addr < 0x0807FFFF) && (addr >= 0x08060000))
  {
    sec = FLASH_SECTOR_7;
  }
  else if((addr < 0x0809FFFF) && (addr >= 0x08080000))
  {
    sec = FLASH_SECTOR_8;
  }
  else if((addr < 0x080BFFFF) && (addr >= 0x080A0000))
  {
    sec = FLASH_SECTOR_9;
  }
  else if((addr < 0x080DFFFF) && (addr >= 0x080C0000))
  {
    sec = FLASH_SECTOR_10;
  }
  else if((addr < 0x080FFFFF) && (addr >= 0x080E0000))
  {
    sec = FLASH_SECTOR_11;
  }
	return sec;
}
