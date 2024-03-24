#include "stm32f4xx_hal.h"
#include "gpio_pin.hpp"

#define LED_PIN                                GPIO_PIN_13
#define LED_GPIO_PORT                          GPIOC

void SystemClock_Config();
void GPIO_Config();
void USART_Config();
void SPI_Config();
void I2C_Config();
void TIM_Config();

void Error_Handler();

int main() {
  HAL_Init();
  SystemClock_Config();
  GPIO_Config();
  USART_Config();
  SPI_Config();
  I2C_Config();
  TIM_Config();

  GpioPin onboard_led(LED_GPIO_PORT, LED_PIN);
  onboard_led.setMode(GpioPinMode::Output);

  while (1)
  {
    onboard_led.digitalWrite((GpioPinState)(HAL_GetTick() % 1000 < 500));
  }
}

extern "C" void SysTick_Handler() {
  HAL_IncTick();
}

void SystemClock_Config() {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

void GPIO_Config() {
  // Enable GPIO clocks
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
}

void USART_Config() {
  // Configure USART
}

void SPI_Config() {
  // Configure SPI
}

void I2C_Config() {
  // Configure I2C
}

void TIM_Config() {
  // Configure TIM
}

void Error_Handler() {
  // Error handling code
}