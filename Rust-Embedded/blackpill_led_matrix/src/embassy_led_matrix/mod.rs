use embassy_stm32::gpio::{AnyPin, Level, Output, Pin, Speed};

// Struct to represent the LED matrix
// Stores the number of rows and columns, pins for the rows and columns (in separate arrays) and the state of the LED matrix (it is a 2D array of type u8)
pub struct LedMatrix<'d, const ROWS: usize, const COLUMNS: usize> {
    row_pins: [Output<'d, AnyPin>; ROWS],
    column_pins: [Output<'d, AnyPin>; COLUMNS],
    display: [[u8; COLUMNS]; ROWS],
}

impl<'d, const ROWS: usize, const COLUMNS: usize> LedMatrix<'d, ROWS, COLUMNS> {
    pub fn new(input_row_pins: [Output<'d, AnyPin>; ROWS], input_column_pins: [Output<'d, AnyPin>; COLUMNS]) -> Self {
        let display = [[0; COLUMNS]; ROWS];

        Self {
            row_pins : input_row_pins,
            column_pins : input_column_pins,
            display,
        }
    }

    pub fn get_rows(&self) -> usize {
        ROWS
    }

    pub fn get_columns(&self) -> usize {
        COLUMNS
    }

    pub fn get_display(&self) -> [[u8; COLUMNS]; ROWS] {
        self.display
    }

    pub fn set_pixel(&mut self, row: usize, column: usize, value: u8) {
        self.display[row][column] = value;
    }

    pub fn get_pixel(&self, row: usize, column: usize) -> u8 {
        self.display[row][column]
    }

    pub fn clear(&mut self) {
        self.display = [[0; COLUMNS]; ROWS];
    }

    pub fn update(&mut self) {
        for i in 0..ROWS {
            self.row_pins[i].set_low();
            for j in 0..COLUMNS {
                if self.display[i][j] != 0 {
                    self.column_pins[j].set_high();
                } else {
                    self.column_pins[j].set_low();
                }
            }
            self.row_pins[i].set_high();
        }
    }
}
