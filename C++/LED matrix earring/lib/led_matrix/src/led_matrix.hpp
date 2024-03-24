#ifndef LED_MATRIX_HPP
#define LED_MATRIX_HPP

// Include any necessary libraries here

#include <cstddef>

#include "stm32f4xx_hal.h"
#include "gpio_pin.hpp"

template <size_t SizeX, size_t SizeY>
class LedMatrix {
public:
    /**
     * @brief Constructor for the LedMatrix class.
     * 
     * @param col_pins An array of GpioPin objects representing the column pins.
     * @param row_pins An array of GpioPin objects representing the row pins.
     */
    LedMatrix(GpioPin* col_pins, GpioPin* row_pins);

    /**
     * @brief Destructor for the LedMatrix class.
     */
    ~LedMatrix();

    /**
     * @brief Initializes the LED matrix.
     */
    void initialize();

    /**
     * @brief Sets the value of a pixel at the specified coordinates.
     * 
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param value The value to set the pixel to.
     */
    void setPixel(int x, int y, uint8_t value);

    /**
     * @brief Clears the LED matrix by setting all pixels to 0.
     */
    void clear();

    /**
     * @brief Updates the LED matrix by refreshing the display.
     */
    void update();

private:
    // Private member variables
    uint8_t m_pixel_buffer[SizeX][SizeY];

    GpioPin* m_col_pins;
    GpioPin* m_row_pins;

    // Private member functions
    
};

#endif // LED_MATRIX_HPP