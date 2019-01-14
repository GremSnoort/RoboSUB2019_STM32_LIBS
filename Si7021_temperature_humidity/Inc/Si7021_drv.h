/*
 * Copyright (C) 2018-2019 RC-AUV.
 * All rights reserved.
 */

#ifndef _Si7021_H_
#define _Si7021_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include "stm32f4xx_hal.h"

#include "sensors/Sensors_priv.h"

static const uint16_t Si7021_ADDR = (0x40<<1); // Si7021 I2C address

#define Si7021_RES0				0
#define Si7021_RES1				7
#define Si7021_VDDS				6
#define Si7021_HTRE				2

// Define CMDs:
#define Si7021_HUMI_HM    			0xE5 // Measure Relative Humidity, Hold Master Mode
#define Si7021_HUMI_NHM   			0xF5 // Measure Relative Humidity, No Hold Master Mode
#define Si7021_TEMP_HM    			0xE3 // Measure Temperature, Hold Master Mode
#define Si7021_TEMP_NHM   			0xF3 // Measure Temperature, No Hold Master Mode
#define Si7021_TEMP_AH    			0xE0 // Read Temperature Value from Previous RH Measurement
#define Si7021_RESET      			0xFE // Reset
#define Si7021_W_RHT_U_REG      	0xE6 // Write RH/T User Register 1
#define Si7021_R_RHT_U_REG      	0xE7 // Read RH/T User Register 1
#define Si7021_W_HEATER_C_REG   	0x51 // Write Heater Control Register
#define Si7021_R_HEATER_C_REG   	0x11 // Read Heater Control Register
#define Si7021_R_ID_BYTE11      	0xFA // Read Electronic ID 1st Byte, first part
#define Si7021_R_ID_BYTE12      	0x0F // Read Electronic ID 1st Byte, second part
#define Si7021_R_ID_BYTE21      	0xFC // Read Electronic ID 2nd Byte, first part
#define Si7021_R_ID_BYTE22      	0xC9 // Read Electronic ID 2nd Byte, second part
#define Si7021_R_FIRM_REV1      	0x84 // Read Firmware Revision, first part
#define Si7021_R_FIRM_REV2      	0xB8  // Read Firmware Revision, second part

typedef struct Si7021Device
{	
	I2C_HandleTypeDef* i2c;
	UART_HandleTypeDef* huart;

}Si7021Device;

typedef enum Si7021MeasurementType
{
  Si7021Humidity,
  Si7021Temperature
	
}Si7021MeasurementType;

static float get_temp_code( uint16_t temp_code );
static float get_humi_code( uint16_t humi_code );

DrvStatus Si7021ReadSingleParam( struct Si7021Device* device, float* data, Si7021MeasurementType type );
DrvStatus Si7021ReadAllParams( struct Si7021Device* device, float* humidity, float* temperature );

void Si7021DataToUART( struct Si7021Device* dev );

Si7021Device Si7021GetNewDevice( I2C_HandleTypeDef* i2c, UART_HandleTypeDef* huart );

#ifdef __cplusplus
}
#endif

#endif /* _Si7021_H_ */
