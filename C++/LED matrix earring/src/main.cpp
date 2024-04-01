#include "stm32f4xx_hal.h"
#include "led_matrix.hpp"

#include <cstdio>

void SystemClock_Config();
void GPIO_Config();
void USART_Config();
void SPI_Config();
void I2C_Config();
void TIM_Config();
void USB_Config();

void Error_Handler();



static uint8_t space_invader[8][8]  = {
  0xfe, 0xda, 0xda, 0xfe, 0xfe, 0xd5, 0xd5, 0xfe, 
  0xfe, 0xd2, 0x8b, 0xeb, 0xe9, 0x87, 0xd0, 0xfe, 
  0xfd, 0xa3, 0x10, 0x19, 0x19, 0x11, 0xa3, 0xfd, 
  0xe3, 0x38, 0x5b, 0x13, 0x15, 0x5e, 0x33, 0xdc, 
  0x65, 0x11, 0x00, 0x00, 0x00, 0x00, 0x11, 0x65, 
  0x77, 0x6c, 0x13, 0x1e, 0x1e, 0x12, 0x68, 0x77, 
  0xb4, 0xac, 0x77, 0xa8, 0xa7, 0x75, 0xad, 0xb2, 
  0xff, 0xfd, 0xc1, 0xbd, 0xbf, 0xc2, 0xfd, 0xff
};


UART_HandleTypeDef huart1;

int main() {
  HAL_Init();
  SystemClock_Config();
  GPIO_Config();
  USART_Config();
  SPI_Config();
  I2C_Config();
  TIM_Config();
  USB_Config();

  GpioPin col_pins[] = {
    GpioPin(GPIOB, GPIO_PIN_5),
    GpioPin(GPIOA, GPIO_PIN_2),
    GpioPin(GPIOA, GPIO_PIN_3),
    GpioPin(GPIOA, GPIO_PIN_15),
    GpioPin(GPIOA, GPIO_PIN_6),
    GpioPin(GPIOB, GPIO_PIN_3),
    GpioPin(GPIOB, GPIO_PIN_7),
    GpioPin(GPIOB, GPIO_PIN_8)
  };
  GpioPin row_pins[] = {
    GpioPin(GPIOA, GPIO_PIN_12),
    GpioPin(GPIOB, GPIO_PIN_6),
    GpioPin(GPIOA, GPIO_PIN_7),
    GpioPin(GPIOB, GPIO_PIN_4),
    GpioPin(GPIOA, GPIO_PIN_0),
    GpioPin(GPIOA, GPIO_PIN_5),
    GpioPin(GPIOA, GPIO_PIN_1),
    GpioPin(GPIOA, GPIO_PIN_4)
  };

  LedMatrix<8,8> matrix(col_pins, row_pins, GpioPinState::High, GpioPinState::Low);
  matrix.initialize();

  while (1)
  {
    matrix.clear();

    uint32_t pos = (HAL_GetTick() / 40) % 64;
    uint32_t pos_x = pos % 8;
    uint32_t pos_y = pos / 8;

    //matrix.setPixel(pos_x, pos_y, 1);
    //matrix.cloneFrame(space_invader);
    //matrix.invertFrame();

    //matrix.drawLine(0, 0, pos_x, pos_y, 255);

    matrix.drawCircle(pos_x, pos_y, 3, 255);

    matrix.update();
  }
}

extern "C" void SysTick_Handler() {
  HAL_IncTick();
}

void SystemClock_Config() {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
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

void USB_Config() {
  // Configure USB
}