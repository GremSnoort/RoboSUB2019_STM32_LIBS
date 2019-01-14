/*
 * Copyright (C) 2018-2019 RC-AUV.
 * All rights reserved.
 */

#include "sensors/Si7021/Si7021_drv.h"

static uint16_t size;
static uint8_t data[256];

static float get_temp_code( uint16_t temp_code )
{
  return (float)( ( ( 175.72 * temp_code ) / 65536.0 ) - 46.85 );
}

static float get_humi_code( uint16_t humi_code )
{
  float value = (float)( ( ( 125.0 * humi_code ) / 65536.0 ) - 6.0 );

  if ( value < 0 )
  {  
		return 0;
	}
  else if ( value > 100 )
  {  
		return 100;
	}
  else
  {  
		return (float)value;
	}
}

DrvStatus Si7021ReadSingleParam( struct Si7021Device* dev, float* data, Si7021MeasurementType type )
{
  uint8_t cmd;
  uint8_t buffer[2];
  uint16_t code;

  if ( type == Si7021Humidity )
	{
		cmd = Si7021_HUMI_HM;
	}    
  else if ( type == Si7021Temperature )
	{
		cmd = Si7021_TEMP_HM;
	}    
  else
	{
		return DRV_FAILURE;
	}    

  if ( HAL_OK != HAL_I2C_Master_Transmit( dev->i2c, Si7021_ADDR, &cmd, 1, 10000 ) )
  {
		return DRV_TRANSMIT_FAILURE;
	}
	
  if ( HAL_OK != HAL_I2C_Master_Receive( dev->i2c, Si7021_ADDR, buffer, 2, 10000 ) )
  {
		return DRV_RECIEVE_FAILURE;
	}
	
  code = to_uint16( buffer );

  if ( type == Si7021Humidity )
	{
		*data = get_humi_code(code);
	}    
  else if ( type == Si7021Temperature )
	{
		*data = get_temp_code( code );
	}    

  return DRV_SUCCESS;
}

DrvStatus Si7021ReadAllParams( struct Si7021Device* dev, float* humidity, float* temperature )
{
  uint8_t cmd = Si7021_HUMI_HM;
  uint8_t buffer[2];
  uint16_t code;

  if ( HAL_OK != HAL_I2C_Master_Transmit( dev->i2c, Si7021_ADDR, &cmd, 1, 10000 ) )
  {
		return DRV_TRANSMIT_FAILURE;
	}

  if ( HAL_OK != HAL_I2C_Master_Receive( dev->i2c, Si7021_ADDR, buffer, 2, 10000 ) )
  {
		return DRV_RECIEVE_FAILURE;
	}

  code = to_uint16( buffer );
  *humidity = get_humi_code( code );

  /* There is a temperature measurement with each RH measurement */
  cmd = Si7021_TEMP_AH;

  if ( HAL_OK != HAL_I2C_Master_Transmit( dev->i2c, Si7021_ADDR, &cmd, 1, 10000 ) )
  {
		return DRV_TRANSMIT_FAILURE;
	}

  if(HAL_OK != HAL_I2C_Master_Receive(dev->i2c, Si7021_ADDR, buffer, 2, 10000))
  {
		return DRV_RECIEVE_FAILURE;
	}

  code = to_uint16( buffer );
  *temperature = get_temp_code( code );

  return DRV_SUCCESS;
}

void Si7021DataToUART( struct Si7021Device* dev )
{
	float temperature;
	float humidity;
	
	while ( Si7021ReadAllParams( dev, &humidity, &temperature ) != DRV_SUCCESS )
	{
		size = sprintf( (char *)data, "\tSi7021:\tReading data failed\r\n" );
		HAL_UART_Transmit( dev->huart, data, size, 1000 );
	}
		
	size = sprintf( (char *)data,"\tSi7021:\tHumidity: %.2f percents;\tTemperature: %.2f oC\r\n", humidity, temperature );
	HAL_UART_Transmit( dev->huart, data, size, 1000 );
}

Si7021Device Si7021GetNewDevice( I2C_HandleTypeDef* i2c, UART_HandleTypeDef* huart )
{
	Si7021Device dev;	
	
	dev.i2c = i2c;
	dev.huart = huart;
	
	return dev;
}
