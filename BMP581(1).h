#ifndef BMP581_H
#define BMP581_H

#include "stm32f4xx_hal.h"

// Define sensor interface (uncomment the appropriate line)
//#define BMP581_USE_I2C
#define BMP581_USE_SPI

// BMP581 Registers (Example addresses, please adjust according to the actual datasheet)
#define BMP581_CHIP_ID_REG         0x00
#define BMP581_DATA_REG            0x04
#define BMP581_CTRL_MEAS_REG       0x1D
#define BMP581_CONFIG_REG          0x1E
#define BMP581_SOFTRESET_REG       0xE0

// Interface specific definitions
#ifdef BMP581_USE_SPI
#define BMP581_CS_GPIO_Port        GPIOA
#define BMP581_CS_Pin              GPIO_PIN_4
#endif

#ifdef BMP581_USE_I2C
#define BMP581_I2C_ADDRESS         0x76 // Adjust as per datasheet
#endif

// Struct to hold BMP581 configuration
typedef struct {
    uint8_t oversampling_temp;  // Oversampling for temperature
    uint8_t oversampling_press; // Oversampling for pressure
    uint8_t power_mode;         // Sleep, forced, normal modes
} BMP581_ConfigTypeDef;

// Function prototypes
void BMP581_Init(SPI_HandleTypeDef *hspi, BMP581_ConfigTypeDef *Config);
void BMP581_Reset(SPI_HandleTypeDef *hspi);
void BMP581_ReadTemperatureAndPressure(SPI_HandleTypeDef *hspi, float *temperature, float *pressure);
void BMP581_WriteRegister(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t data);
uint8_t BMP581_ReadRegister(SPI_HandleTypeDef *hspi, uint8_t reg);

#endif // BMP581_H
