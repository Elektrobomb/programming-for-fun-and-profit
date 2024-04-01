#ifndef LED_MATRIX_HPP
#define LED_MATRIX_HPP

// Include any necessary libraries here

#include <cstddef>
#include <cstdlib>

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
     * @param colActiveState The active state of the column pins.
     * @param rowActiveState The active state of the row pins.
     */
    LedMatrix(GpioPin col_pins[COLS], GpioPin row_pins[ROWS], GpioPinState colActiveState, GpioPinState rowActiveState) : m_col_pins(col_pins), m_row_pins(row_pins), m_col_active_state(colActiveState), m_row_active_state(rowActiveState) {}

    /**
     * @brief Destructor for the LedMatrix class.
     */
    ~LedMatrix();

    /**
     * @brief Initializes the LED matrix.
     * 
     * This function sets the mode of all the pins to OUTPUT, clears the display, and updates the display.
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
    void setPixel(size_t x, size_t y, uint8_t value) {
        if(x < COLS && y < ROWS && x >= 0 && y >= 0) {
            m_pixel_buffer[x][y] = value;
        }
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
     * 
     * This function performs multiplexing using the row and column pins to update the display.
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

    /**
     * @brief Inverts the values of all pixels in the LED matrix.
     */
    void invertFrame() {
        for (size_t i = 0; i < COLS; i++) {
            for (size_t j = 0; j < ROWS; j++) {
                m_pixel_buffer[i][j] = 255 - m_pixel_buffer[i][j];
            }
        }
    }

    /**
     * Draws a line between two points on the LED matrix.
     *
     * @param x0 The x-coordinate of the starting point.
     * @param y0 The y-coordinate of the starting point.
     * @param x1 The x-coordinate of the ending point.
     * @param y1 The y-coordinate of the ending point.
     * @param value The value to set for each pixel on the line.
     */
    void drawLine(int x0, int y0, int x1, int y1, uint8_t value) {
        int dx = std::abs(x1-x0);
        int dy = std::abs(y1-y0);
        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;
        int err = dx-dy;

        while (true) {
            setPixel(x0, y0, value);
            if (x0 == x1 && y0 == y1) {
                break;
            }
            int e2 = 2*err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    }

    /**
     * Draws a circle on the LED matrix.
     *
     * @param x0 The x-coordinate of the center of the circle.
     * @param y0 The y-coordinate of the center of the circle.
     * @param radius The radius of the circle.
     * @param value The value to set for the pixels that make up the circle.
     */
    void drawCircle(int x0, int y0, int radius, uint8_t value) {
        int x = radius;
        int y = 0;
        int err = 0;

        while (x >= y) {
            setPixel(x0 + x, y0 + y, value);
            setPixel(x0 + y, y0 + x, value);
            setPixel(x0 - y, y0 + x, value);
            setPixel(x0 - x, y0 + y, value);
            setPixel(x0 - x, y0 - y, value);
            setPixel(x0 - y, y0 - x, value);
            setPixel(x0 + y, y0 - x, value);
            setPixel(x0 + x, y0 - y, value);

            if (err <= 0) {
                y += 1;
                err += 2*y + 1;
            }
            if (err > 0) {
                x -= 1;
                err -= 2*x + 1;
            }
        }
    }


private:
    // Private member variables
    uint8_t m_pixel_buffer[COLS][ROWS]; /**< The buffer for storing pixel values. */

    GpioPin* m_col_pins; /**< An array of GpioPin objects representing the column pins. */
    GpioPin* m_row_pins; /**< An array of GpioPin objects representing the row pins. */

    GpioPinState m_col_active_state; /**< The active state of the column pins. */
    GpioPinState m_row_active_state; /**< The active state of the row pins. */

    // Private member functions
    
};

#endif // LED_MATRIX_HPP