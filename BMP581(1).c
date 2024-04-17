#include "BMP581.h"

void BMP581_Init(SPI_HandleTypeDef *hspi, BMP581_ConfigTypeDef *Config) {
    BMP581_Reset(hspi);
    BMP581_WriteRegister(hspi, BMP581_CTRL_MEAS_REG, (Config->oversampling_temp << 5) | (Config->oversampling_press << 2) | Config->power_mode);
    BMP581_WriteRegister(hspi, BMP581_CONFIG_REG, 0x14); 
}

void BMP581_Reset(SPI_HandleTypeDef *hspi) {
    BMP581_WriteRegister(hspi, BMP581_SOFTRESET_REG, 0xB6);
    HAL_Delay(100); // Wait for reset to complete
}

void BMP581_ReadTemperatureAndPressure(SPI_HandleTypeDef *hspi, float *temperature, float *pressure) {
    uint8_t data[6]; // Assuming 3 bytes for pressure and 3 bytes for temperature
    BMP581_Select();
    HAL_SPI_Receive(hspi, data, 6, HAL_MAX_DELAY);
    BMP581_Deselect();

    // Convert the data to temperature and pressure values
    
    *pressure = (float)((data[0] << 16) | (data[1] << 8) | data[2]); // Simplified conversion
    *temperature = (float)((data[3] << 16) | (data[4] << 8) | data[5]); // Simplified conversion
}

void BMP581_WriteRegister(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t data) {
    BMP581_Select();
    uint8_t buf[2] = {reg & 0x7F, data}; // Clear MSB for write operation
    HAL_SPI_Transmit(hspi, buf, 2, HAL_MAX_DELAY);
    BMP581_Deselect();
}

uint8_t BMP581_ReadRegister(SPI_HandleTypeDef *hspi, uint8_t reg) {
    uint8_t value;
    BMP581_Select();
    reg |= 0x80; // Set MSB for read operation
    HAL_SPI_Transmit(hspi, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(hspi, &value, 1, HAL_MAX_DELAY);
    BMP581_Deselect();
    return value;
}

void BMP581_Select(void) {
    HAL_GPIO_WritePin(BMP581_CS_GPIO_Port, BMP581_CS_Pin, GPIO_PIN_RESET);
}

void BMP581_Deselect(void) {
    HAL_GPIO_WritePin(BMP581_CS_GPIO_Port, BMP581_CS_Pin, GPIO_PIN_SET);
}
