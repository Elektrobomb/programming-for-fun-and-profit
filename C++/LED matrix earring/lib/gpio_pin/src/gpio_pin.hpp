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

enum GpioPort {
    PortA = 0,
    PortB = 1,
    PortC = 2,
    PortD = 3,
    PortE = 4,
    PortF = 5,
    PortG = 6,
    PortH = 7,
};
enum GpioPinNumber {
    Pin0 = 0,
    Pin1 = 1,
    Pin2 = 2,
    Pin3 = 3,
    Pin4 = 4,
    Pin5 = 5,
    Pin6 = 6,
    Pin7 = 7,
    Pin8 = 8,
    Pin9 = 9,
    Pin10 = 10,
    Pin11 = 11,
    Pin12 = 12,
    Pin13 = 13,
    Pin14 = 14,
    Pin15 = 15
};

GpioPinState operator!(GpioPinState state);

class GpioPin {
public:
    GpioPin(GpioPort port, GpioPinNumber pin);
    ~GpioPin() {}

    virtual void init();
    virtual void setMode(GpioPinMode mode);
    virtual void digitalWrite(GpioPinState value);
    virtual GpioPinState digitalRead();
private:
    GpioPort m_port;
    GpioPinNumber m_pin;

    GPIO_TypeDef* m_hal_port;
    uint32_t m_hal_pin;

    GPIO_TypeDef* getHalPort(GpioPort port);
    uint32_t getHalPin(GpioPinNumber pin);
};

#endif // GPIO_PIN_HPP
