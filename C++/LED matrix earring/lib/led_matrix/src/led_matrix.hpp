#ifndef LED_MATRIX_HPP
#define LED_MATRIX_HPP

// Include any necessary libraries here

#include <cstddef>

#include "stm32f4xx_hal.h"
#include "gpio_pin.hpp"

template<size_t COLS, size_t ROWS>
class LedMatrix {
public:
    /**
     * @brief Constructor for the LedMatrix class.
     * 
     * @param col_pins An array of GpioPin objects representing the column pins.
     * @param row_pins An array of GpioPin objects representing the row pins.
     */
    LedMatrix(GpioPin *col_pins, GpioPin *row_pins) : m_col_pins(col_pins), m_row_pins(row_pins) {}

    /**
     * @brief Destructor for the LedMatrix class.
     */
    ~LedMatrix();

    /**
     * @brief Initializes the LED matrix.
     */
    void initialize() {
        // Set the mode of all the pins to OUTPUT
        for (size_t i = 0; i < COLS; i++) {
            m_col_pins[i].setMode(GpioPinMode::Output);
        }
        for (size_t i = 0; i < ROWS; i++) {
            m_row_pins[i].setMode(GpioPinMode::Output);
        }
        // Clear the display
        clear();
    }

    /**
     * @brief Sets the value of a pixel at the specified coordinates.
     * 
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param value The value to set the pixel to.
     */
    void setPixel(int x, int y, uint8_t value) {
        m_pixel_buffer[x][y] = value;
    }

    /**
     * @brief Clears the LED matrix by setting all pixels to 0.
     */
    void clear() {
        // Clear m_pixel_buffer to all black
        for (size_t i = 0; i < COLS; i++) {
            for (size_t j = 0; j < ROWS; j++) {
                m_pixel_buffer[i][j] = 0;
            }
        }
    }

    /**
     * @brief Updates the LED matrix by refreshing the display.
     */
    void update() {
        // Perform multiplexing using the row and column pins
        for (size_t row = 0; row < ROWS; row++) {
            // Set the row pin to HIGH
            m_row_pins[row].digitalWrite(GpioPinState::High);

            for (size_t col = 0; col < COLS; col++) {
                // Set the column pin to the corresponding value from m_pixel_buffer
                if (m_pixel_buffer[col][row] == 0) {
                    m_col_pins[col].digitalWrite(GpioPinState::Low);
                } else {
                    m_col_pins[col].digitalWrite(GpioPinState::High);
                }
            }
            HAL_Delay(100);
            // Set the row pin to LOW
            m_row_pins[row].digitalWrite(GpioPinState::Low);
        }
    }

private:
    // Private member variables
    uint8_t m_pixel_buffer[COLS][ROWS];

    GpioPin* m_col_pins;
    GpioPin* m_row_pins;

    // Private member functions
    
};

#endif // LED_MATRIX_HPP