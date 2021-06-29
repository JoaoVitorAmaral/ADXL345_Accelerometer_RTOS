/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __adxl345_H
#define __adxl345_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

//static void MX_I2C1_Init(void);
void ADXL345_Configuration(I2C_HandleTypeDef* i2c_instance, uint8_t TMP_resolution);
void ADXL345_DevIDRead(I2C_HandleTypeDef* i2c_instance);
void ADXL345_DataRead(I2C_HandleTypeDef* i2c_instance);


/*
void I2C_Setup();
void ADXL345_Configuration(I2C_HandleTypeDef* i2c_instance, uint8_t TMP_resolution);
float ADXL345_Read(I2C_HandleTypeDef* i2c_instance);
*/

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */
