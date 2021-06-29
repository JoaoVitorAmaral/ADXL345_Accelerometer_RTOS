/* Includes ------------------------------------------------------------------*/
#include "ADXL345.h"

uint8_t chave = 0;

uint8_t devID;
uint8_t DATA_FORMAT_setting;
uint8_t POWER_CTL_setting;

//Cleaning data buffers
int8_t x_y_z_data[6] = {0};

typedef struct{
	int16_t X_OUT;
	int16_t Y_OUT;
	int16_t Z_OUT;
	float X_OUT_g;
	float Y_OUT_g;
	float Z_OUT_g;
} data_out_type;

//Configura a I2C1 para comunicar com o ADXL345
/*
static void MX_I2C1_Init(void){

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
}
*/

void ADXL345_Configuration(I2C_HandleTypeDef* i2c_instance, uint8_t TMP_resolution)
{
	//Chip select high
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, SET);

	//HAL_I2C_Mem_Write(i2c_instance, 0x92, 0x01, 1, &TMP_resolution, 1, HAL_MAX_DELAY);
}

void ADXL345_DevIDRead(I2C_HandleTypeDef* i2c_instance)
{
	//Seleciona o dispositivo
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, SET);

	//Lê a device ID do dispositivo (deve retornar 0b11100101 = 229)
	HAL_I2C_Mem_Read(i2c_instance, 0xA7, 0x00, 1, &devID, 1, HAL_MAX_DELAY);
}


void ADXL345_DataRead(I2C_HandleTypeDef* i2c_instance)
{
	//Setting accelerometer's range to +-2g (by setting DATA_FORMAT's D0 and D1 bits)
	HAL_I2C_Mem_Read(i2c_instance, 0xA7, 0x31, 1, &DATA_FORMAT_setting, 1, HAL_MAX_DELAY);
	POWER_CTL_setting = POWER_CTL_setting & (0XFF & 00);
	HAL_I2C_Mem_Write(i2c_instance, 0xA6, 0x31, 1, &DATA_FORMAT_setting, 1, HAL_MAX_DELAY);

	//Setting accelerometer in measuring mode (by setting the D3 bit from POWER_CTL register to HIGH)
	HAL_I2C_Mem_Read(i2c_instance, 0xA7, 0x2D, 1, &POWER_CTL_setting, 1, HAL_MAX_DELAY);
	POWER_CTL_setting = POWER_CTL_setting | (1 << 3);
	HAL_I2C_Mem_Write(i2c_instance, 0xA6, 0x2D, 1, &POWER_CTL_setting, 1, HAL_MAX_DELAY);

	// Taking 3 axis (x, y e z) measuring, where:
	// x_y_z_data[0] = DATAX0 , x_y_z_data[1] = DATAX1, ... , x_y_z_data[5] = DATAZ1
	HAL_I2C_Mem_Read(i2c_instance, 0xA7, 0x32, 1, (uint8_t *)x_y_z_data, 6, HAL_MAX_DELAY);

	//Formatting data to +-2g resolution mode
	data_out_type data_OUT;
	data_OUT.X_OUT = ((x_y_z_data[1] << 8) | x_y_z_data[0]);
	data_OUT.Y_OUT = ((x_y_z_data[3] << 8) | x_y_z_data[2]);
	data_OUT.Z_OUT = ((x_y_z_data[5] << 8) | x_y_z_data[4]);

	data_OUT.X_OUT_g = (data_OUT.X_OUT)*0.0039;
	data_OUT.Y_OUT_g = (data_OUT.Y_OUT)*0.0039;
	data_OUT.Z_OUT_g = (data_OUT.Z_OUT)*0.0039;

	//Setting accelerometer in stand-by mode (by setting the D3 bit from POWER_CTL register to LOW)
	POWER_CTL_setting = POWER_CTL_setting & (0XFF & 0 << 3);
	HAL_I2C_Mem_Write(i2c_instance, 0xA6, 0x2D, 1, &POWER_CTL_setting, 1, HAL_MAX_DELAY);
}

