#include "gpio_pin.hpp"

GpioPin::GpioPin(GpioPort port, GpioPinNumber pin):
    m_port(port),
    m_pin(pin) {
        GpioPin::setMode(GpioPinMode::Input);
    }

void GpioPin::init() {
    m_hal_port = getHalPort(m_port);
    m_hal_pin = getHalPin(m_pin);

    switch (m_port)
    {
    case GpioPort::PortA:
        #ifdef GPIOA
        __HAL_RCC_GPIOA_CLK_ENABLE();
        #endif
        break;
    case GpioPort::PortB:
        #ifdef GPIOB
        __HAL_RCC_GPIOB_CLK_ENABLE();
        #endif
        break;
    case GpioPort::PortC:
        #ifdef GPIOC
        __HAL_RCC_GPIOC_CLK_ENABLE();
        #endif
        break;
    case GpioPort::PortD:
        #ifdef GPIOD
        __HAL_RCC_GPIOD_CLK_ENABLE();
        #endif
        break;
    case GpioPort::PortE:
        #ifdef GPIOE
        __HAL_RCC_GPIOE_CLK_ENABLE();
        #endif
        break;
    case GpioPort::PortF:
        #ifdef GPIOF
        __HAL_RCC_GPIOF_CLK_ENABLE();
        #endif
        break;
    case GpioPort::PortG:
        #ifdef GPIOG
        __HAL_RCC_GPIOG_CLK_ENABLE();
        #endif
        break;
    case GpioPort::PortH:
        #ifdef GPIOH
        __HAL_RCC_GPIOH_CLK_ENABLE();
        #endif
        break;
    default:
        break;
    }
}

void GpioPin::setMode(GpioPinMode mode) {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = m_hal_pin;

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
    HAL_GPIO_Init(m_hal_port, &GPIO_InitStruct);
}

void GpioPin::digitalWrite(GpioPinState value) {
    HAL_GPIO_WritePin(m_hal_port, m_hal_pin, (value == GpioPinState::High) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

GpioPinState GpioPin::digitalRead() {
    if (HAL_GPIO_ReadPin(m_hal_port, m_hal_pin) == GPIO_PIN_SET) {
        return GpioPinState::High;
    } else {
        return GpioPinState::Low;
    }
}

GpioPinState operator!(GpioPinState state) {
    return (state == GpioPinState::Low) ? GpioPinState::High : GpioPinState::Low;
}

GPIO_TypeDef* GpioPin::getHalPort(GpioPort port) {
    switch (port)
    {
    case GpioPort::PortA:
        #ifdef GPIOA
        return GPIOA;
        #endif
    case GpioPort::PortB:
        #ifdef GPIOB
        return GPIOB;
        #endif
    case GpioPort::PortC:
        #ifdef GPIOC
        return GPIOC;
        #endif
    case GpioPort::PortD:
        #ifdef GPIOD
        return GPIOD;
        #endif
    case GpioPort::PortE:
        #ifdef GPIOE
        return GPIOE;
        #endif
    case GpioPort::PortF:
        #ifdef GPIOF
        return GPIOF;
        #endif
    case GpioPort::PortG:
        #ifdef GPIOG
        return GPIOG;
        #endif
    case GpioPort::PortH:
        #ifdef GPIOH
        return GPIOH;
        #endif
    default:
        return GPIOA;
    }

}
uint32_t GpioPin::getHalPin(GpioPinNumber pin) {
    switch (pin)
    {
    case GpioPinNumber::Pin0:
        return GPIO_PIN_0;
    case GpioPinNumber::Pin1:
        return GPIO_PIN_1;
    case GpioPinNumber::Pin2:
        return GPIO_PIN_2;
    case GpioPinNumber::Pin3:
        return GPIO_PIN_3;
    case GpioPinNumber::Pin4:
        return GPIO_PIN_4;
    case GpioPinNumber::Pin5:
        return GPIO_PIN_5;
    case GpioPinNumber::Pin6:
        return GPIO_PIN_6;
    case GpioPinNumber::Pin7:
        return GPIO_PIN_7;
    case GpioPinNumber::Pin8:
        return GPIO_PIN_8;
    case GpioPinNumber::Pin9:
        return GPIO_PIN_9;
    case GpioPinNumber::Pin10:
        return GPIO_PIN_10;
    case GpioPinNumber::Pin11:
        return GPIO_PIN_11;
    case GpioPinNumber::Pin12:
        return GPIO_PIN_12;
    case GpioPinNumber::Pin13:
        return GPIO_PIN_13;
    case GpioPinNumber::Pin14:
        return GPIO_PIN_14;
    case GpioPinNumber::Pin15:
        return GPIO_PIN_15;
    default:
        return GPIO_PIN_0;
    }
}