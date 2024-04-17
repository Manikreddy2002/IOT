#include "BMP581.h"

SPI_HandleTypeDef hspi1; // Assume SPI is initialized properly

int main(void) {
    HAL_Init();
    SystemClock_Config(); // Ensure this is set up correctly
    MX_GPIO_Init();
    MX_SPI1_Init();

    BMP581_ConfigTypeDef bmpConfig = {1, 1, 3}; // example settings
    BMP581_Init(&hspi1, &bmpConfig);

    float pressure = 0.0, temperature = 0.0;
    while(1) {
        BMP581_ReadTemperatureAndPressure(&hspi1, &temperature, &pressure);
        printf("Temperature: %.2f °C, Pressure: %.2f Pa\n", temperature, pressure);
        HAL_Delay(1000);
    }
}
