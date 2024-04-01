#ifndef GPIO_PIN_HPP
#define GPIO_PIN_HPP

#include "stm32f4xx_hal.h"

enum class GpioPinMode {
    Input,
    Output
};

enum GpioPinState {
    Low = 0,
    High = 1
};

GpioPinState operator!(GpioPinState state);

class GpioPin {
public:
    GpioPin(GPIO_TypeDef* port, uint16_t pin);
    ~GpioPin() {}

    void setMode(GpioPinMode mode);
    void digitalWrite(enum GpioPinState value);
    GpioPinState digitalRead();
private:
    GPIO_TypeDef* m_port;
    uint32_t m_pin;
};

#endif // GPIO_PIN_HPP
