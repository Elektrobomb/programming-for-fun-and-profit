#ifndef FAKE_GPIO_PIN_HPP
#define FAKE_GPIO_PIN_HPP

#include "gpio_pin.hpp"

class FakeGpioPin: public GpioPin {
public:
    FakeGpioPin(GpioPort port, GpioPinNumber pin);
    ~FakeGpioPin() {}

    virtual void init() override;
    virtual void setMode(GpioPinMode mode) override;
    virtual void digitalWrite(GpioPinState value) override;
    virtual GpioPinState digitalRead() override;
private:
    GpioPort m_port;
    GpioPinNumber m_pin;

    bool m_is_initialized;
    GpioPinState m_state;
    GpioPinMode m_mode;
};

#endif // FAKE_GPIO_PIN_HPP


