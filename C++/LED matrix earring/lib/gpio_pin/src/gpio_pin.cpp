#include "gpio_pin.hpp"

GpioPin::GpioPin(GPIO_TypeDef* port, uint16_t pin):
    m_port(port),
    m_pin(pin) {
        if (m_port == GPIOA) {
            __HAL_RCC_GPIOA_CLK_ENABLE();
        }
        #ifdef GPIOB
        else if (m_port == GPIOB) {
            __HAL_RCC_GPIOB_CLK_ENABLE();
        }
        #endif
        #ifdef GPIOC
        else if (m_port == GPIOC) {
            __HAL_RCC_GPIOC_CLK_ENABLE();
        }
        #endif
        #ifdef GPIOD
        else if (m_port == GPIOD) {
            __HAL_RCC_GPIOD_CLK_ENABLE();
        }
        #endif
        #ifdef GPIOE
        else if (m_port == GPIOE) {
            __HAL_RCC_GPIOE_CLK_ENABLE();
        }
        #endif
        #ifdef GPIOF
        else if (m_port == GPIOF) {
            __HAL_RCC_GPIOF_CLK_ENABLE();
        }
        #endif
        #ifdef GPIOG
        else if (m_port == GPIOG) {
            __HAL_RCC_GPIOG_CLK_ENABLE();
        }
        #endif
        #ifdef GPIOH
        else if (m_port == GPIOH) {
            __HAL_RCC_GPIOH_CLK_ENABLE();
        }
        #endif
        #ifdef GPIOI
        else if (m_port == GPIOI) {
            __HAL_RCC_GPIOI_CLK_ENABLE();
        }
        #endif
        #ifdef GPIOJ
        else if (m_port == GPIOJ) {
            __HAL_RCC_GPIOJ_CLK_ENABLE();
        }
        #endif

        GpioPin::setMode(GpioPinMode::Input);
    }

void GpioPin::setMode(GpioPinMode mode) {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = m_pin;

    switch (mode)
    {
    case GpioPinMode::Input:
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        break;
    case GpioPinMode::Output:
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        break;
    default:
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        break;
    }

    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(m_port, &GPIO_InitStruct);
}

void GpioPin::digitalWrite(enum GpioPinState value) {
    HAL_GPIO_WritePin(m_port, m_pin, (value == GpioPinState::High) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

enum GpioPinState GpioPin::digitalRead() {
    if (HAL_GPIO_ReadPin(m_port, m_pin) == GPIO_PIN_SET) {
        return GpioPinState::High;
    } else {
        return GpioPinState::Low;
    }
}