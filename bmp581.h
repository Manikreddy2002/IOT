#include "stm32f4xx_hal.h"
#include "main.h"  // Contains pin and peripheral definitions

// Define CS Pin
#define BMP581_CS_GPIO_Port GPIOA
#define BMP581_CS_Pin GPIO_PIN_4

SPI_HandleTypeDef hspi1;

void MX_SPI1_Init(void)
{
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;  // Adjust based on clock settings
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    HAL_SPI_Init(&hspi1);

    // Initialize CS Pin
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = BMP581_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(BMP581_CS_GPIO_Port, &GPIO_InitStruct);

    // Set CS high
    HAL_GPIO_WritePin(BMP581_CS_GPIO_Port, BMP581_CS_Pin, GPIO_PIN_SET);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
    if(spiHandle->Instance==SPI1)
    {
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();  // Adjust as necessary for your setup
    }
}

void BMP581_Select(void)
{
    HAL_GPIO_WritePin(BMP581_CS_GPIO_Port, BMP581_CS_Pin, GPIO_PIN_RESET); // Select BMP581
}

void BMP581_Deselect(void)
{
    HAL_GPIO_WritePin(BMP581_CS_GPIO_Port, BMP581_CS_Pin, GPIO_PIN_SET); // Deselect BMP581
}

void BMP581_Transmit(uint8_t* data, uint16_t size)
{
    BMP581_Select();
    HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY);
    BMP581_Deselect();
}

void BMP581_Receive(uint8_t* data, uint16_t size)
{
    BMP581_Select();
    HAL_SPI_Receive(&hspi1, data, size, HAL_MAX_DELAY);
    BMP581_Deselect();
}

int main(void)
{
    HAL_Init();
    SystemClock_Config(); // Ensure this is set up correctly
    MX_GPIO_Init();
    MX_SPI1_Init();

    uint8_t txData[2] = {0x00, 0x00}; // Example data to write
    uint8_t rxData[2] = {0};

    while (1)
    {
        BMP581_Transmit(txData, 2);  // Transmit data to BMP581
        BMP581_Receive(rxData, 2);   // Receive data from BMP581

        HAL_Delay(1000);  // 1 second delay
    }
}

