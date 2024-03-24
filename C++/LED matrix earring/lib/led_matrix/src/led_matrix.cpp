#include "led_matrix.hpp"

// Constructor
template <size_t SizeX, size_t SizeY>
LedMatrix<SizeX, SizeY>::LedMatrix(GpioPin *col_pins, GpioPin *row_pins) : m_col_pins(col_pins), m_row_pins(row_pins) {}

template <size_t SizeX, size_t SizeY>
void LedMatrix<SizeX, SizeY>::initialize() {
    // Set the mode of all the pins to OUTPUT
    for (size_t i = 0; i < SizeX; i++) {
        m_col_pins[i].setMode(GpioPinMode::Output);
    }
    for (size_t i = 0; i < SizeY; i++) {
        m_row_pins[i].setMode(GpioPinMode::Output);
    }
    // Clear the display
    clear();
}

template <size_t SizeX, size_t SizeY>
void LedMatrix<SizeX, SizeY>::setPixel(int x, int y, uint8_t value) {
    m_pixel_buffer[x][y] = value;
}

template <size_t SizeX, size_t SizeY>
void LedMatrix<SizeX, SizeY>::clear() {
    // Clear m_pixel_buffer to all black
    for (size_t i = 0; i < SizeX; i++) {
        for (size_t j = 0; j < SizeY; j++) {
            m_pixel_buffer[i][j] = 0;
        }
    }
}

template <size_t SizeX, size_t SizeY>
void LedMatrix<SizeX, SizeY>::update() {
    // Perform multiplexing using the row and column pins
    for (size_t row = 0; row < SizeY; row++) {
        // Set the row pin to HIGH
        m_row_pins[row].digitalWrite(GpioPinState::High);

        for (size_t col = 0; col < SizeX; col++) {
            // Set the column pin to the corresponding value from m_pixel_buffer
            if (m_pixel_buffer[col][row] == 0) {
                m_col_pins[col].digitalWrite(GpioPinState::Low);
            } else {
                m_col_pins[col].digitalWrite(GpioPinState::High);
            }
        }
        HAL_Delay(1);
        // Set the row pin to LOW
        m_row_pins[row].digitalWrite(GpioPinState::Low);
    }
}