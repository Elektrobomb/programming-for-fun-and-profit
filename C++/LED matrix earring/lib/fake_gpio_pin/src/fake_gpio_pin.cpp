#include "fake_gpio_pin.hpp"

FakeGpioPin::FakeGpioPin(GpioPort port, GpioPinNumber pin) : GpioPin(port, pin) {}

void FakeGpioPin::init() {
    m_is_initialized = true;
    m_state = GpioPinState::Low;
    m_mode = GpioPinMode::Input;
}

void FakeGpioPin::setMode(GpioPinMode mode) {
    m_mode = mode;
    if (mode == GpioPinMode::Output) {
        m_state = GpioPinState::Low;
    }
}

void FakeGpioPin::digitalWrite(GpioPinState value) {
    m_state = value;
}

GpioPinState FakeGpioPin::digitalRead() {
    return m_state;
}