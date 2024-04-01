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
    LedMatrix(GpioPin col_pins[COLS], GpioPin row_pins[ROWS], GpioPinState colActiveState, GpioPinState rowActiveState) : m_col_pins(col_pins), m_row_pins(row_pins), m_col_active_state(colActiveState), m_row_active_state(rowActiveState) {}

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

        // Update the display
        update();
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
        for (size_t brightness = 64; brightness < 255; brightness+=10){
            // Perform multiplexing using the row and column pins
            for (size_t row = 0; row < ROWS; row++) {

                // Prep the column pins
                for (size_t col = 0; col < COLS; col++) {
                    // Set the column pin to the corresponding value from m_pixel_buffer
                    if (m_pixel_buffer[col][row] > brightness) {
                        m_col_pins[col].digitalWrite(m_col_active_state);
                    } else {
                        m_col_pins[col].digitalWrite(!m_col_active_state);
                    }
                }
                // Set the row pin to HIGH
                m_row_pins[row].digitalWrite(m_row_active_state);
                for (size_t i = 0; i < 10; i++){
                    __NOP();
                }
                //HAL_Delay(1);
                // Set the row pin to LOW
                m_row_pins[row].digitalWrite(!m_row_active_state);

            }
        }
    }

    /**
     * @brief Clones the frame to the LED matrix.
     * 
     * @param frame The frame to clone.
     */
    void cloneFrame(uint8_t frame[COLS][ROWS]) {
        for (size_t i = 0; i < COLS; i++) {
            for (size_t j = 0; j < ROWS; j++) {
                m_pixel_buffer[i][j] = frame[i][j];
            }
        }
    }

    void invertFrame() {
        for (size_t i = 0; i < COLS; i++) {
            for (size_t j = 0; j < ROWS; j++) {
                m_pixel_buffer[i][j] = 255 - m_pixel_buffer[i][j];
            }
        }
    }

private:
    // Private member variables
    uint8_t m_pixel_buffer[COLS][ROWS];

    GpioPin* m_col_pins;
    GpioPin* m_row_pins;

    GpioPinState m_col_active_state;
    GpioPinState m_row_active_state;

    // Private member functions
    
};

#endif // LED_MATRIX_HPP