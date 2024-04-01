#ifndef LED_MATRIX_HPP
#define LED_MATRIX_HPP

// Include any necessary libraries here

#include <cstddef>
#include <cstdlib>

#include "stm32f4xx_hal.h"
#include "gpio_pin.hpp"

#define BRIGHTNESS_STEP_SIZE 7
#define BRIGHTNESS_DELAY_MULTIPLIER 100

// Gamma brightness lookup table <https://victornpb.github.io/gamma-table-generator>
// gamma = 3.00 steps = 256 range = 0-255
const uint8_t gamma_lut[256] = {
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,
     2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   4,   4,
     4,   4,   4,   5,   5,   5,   5,   6,   6,   6,   6,   6,   7,   7,   7,   8,
     8,   8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  12,  12,  12,  13,  13,
    14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,  19,  20,  20,  21,
    22,  22,  23,  23,  24,  25,  25,  26,  27,  27,  28,  29,  29,  30,  31,  32,
    32,  33,  34,  35,  35,  36,  37,  38,  39,  40,  40,  41,  42,  43,  44,  45,
    46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  60,  61,  62,
    63,  64,  65,  67,  68,  69,  70,  72,  73,  74,  76,  77,  78,  80,  81,  82,
    84,  85,  87,  88,  90,  91,  93,  94,  96,  97,  99, 101, 102, 104, 105, 107,
   109, 111, 112, 114, 116, 118, 119, 121, 123, 125, 127, 129, 131, 132, 134, 136,
   138, 140, 142, 144, 147, 149, 151, 153, 155, 157, 159, 162, 164, 166, 168, 171,
   173, 175, 178, 180, 182, 185, 187, 190, 192, 195, 197, 200, 202, 205, 207, 210,
   213, 215, 218, 221, 223, 226, 229, 232, 235, 237, 240, 243, 246, 249, 252, 255,
  };

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
     * Multiplexing is performed on a per-LED basis to reduce instantaneous power consumption.  
     */
    void update() {
        // Perform multiplexing using the row and column pins
        for (size_t row = 0; row < ROWS; row++) {
            // Set the row pin to HIGH
            m_row_pins[row].digitalWrite(m_row_active_state);

            // Prep the column pins
            for (size_t col = 0; col < COLS; col++) {
                uint8_t gamma_corrected_brightness = gammaCorrect(m_pixel_buffer[col][row]);
                // Set the column pin
                if (gamma_corrected_brightness > 0) {
                    m_col_pins[col].digitalWrite(m_col_active_state);
                }
                // Delay for a short period of time to control brightness
                for (size_t i = 0; i < gamma_corrected_brightness * BRIGHTNESS_DELAY_MULTIPLIER; i+=BRIGHTNESS_STEP_SIZE){
                    __NOP();
                }
                // Reset the column pin
                m_col_pins[col].digitalWrite(!m_col_active_state);
            }
            // Set the row pin to LOW
            m_row_pins[row].digitalWrite(!m_row_active_state);

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

    /**
     * Applies gamma correction to the given value.
     *
     * @param value The input value to be gamma corrected.
     * @return The gamma corrected value.
     */
    uint8_t gammaCorrect(uint8_t value) {
        return gamma_lut[value];
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