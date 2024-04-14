#![no_std]
#![no_main]

mod fmt;

#[cfg(not(feature = "defmt"))]
use panic_halt as _;
#[cfg(feature = "defmt")]
use {defmt_rtt as _, panic_probe as _};

use embassy_executor::Spawner;
use embassy_stm32::gpio::{AnyPin, Level, Output, Pin, Speed};
use embassy_time::{Duration, Timer};
use fmt::info;

use blackpill_led_matrix::embassy_led_matrix::{self, LedMatrix};

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    let p = embassy_stm32::init(Default::default());
    //let mut led = Output::new(p.PC13, Level::High, Speed::Low);

    let mut row_pins: [Output<'_, AnyPin>; 8] = [
        Output::new(p.PB5.degrade(), Level::Low, Speed::Low),
        Output::new(p.PA2.degrade(), Level::Low, Speed::Low),
        Output::new(p.PA3.degrade(), Level::Low, Speed::Low),
        Output::new(p.PA15.degrade(), Level::Low, Speed::Low),
        Output::new(p.PA6.degrade(), Level::Low, Speed::Low),
        Output::new(p.PB3.degrade(), Level::Low, Speed::Low),
        Output::new(p.PB7.degrade(), Level::Low, Speed::Low),
        Output::new(p.PB8.degrade(), Level::Low, Speed::Low),
    ];
    let mut col_pins: [Output<'_, AnyPin>; 8] = [
        Output::new(p.PA12.degrade(), Level::Low, Speed::Low),
        Output::new(p.PB6.degrade(), Level::Low, Speed::Low),
        Output::new(p.PA7.degrade(), Level::Low, Speed::Low),
        Output::new(p.PB4.degrade(), Level::Low, Speed::Low),
        Output::new(p.PA0.degrade(), Level::Low, Speed::Low),
        Output::new(p.PA5.degrade(), Level::Low, Speed::Low),
        Output::new(p.PA1.degrade(), Level::Low, Speed::Low),
        Output::new(p.PA4.degrade(), Level::Low, Speed::Low),
    ];
    let mut led_matrix = LedMatrix::new(row_pins, col_pins);
    
    loop {
        info!("Hello, World!");
        led_matrix.set_pixel(5, 5, 255);
        led_matrix.update();
        Timer::after(Duration::from_millis(500)).await;
        led_matrix.set_pixel(5, 5, 0);
        led_matrix.update();
        Timer::after(Duration::from_millis(500)).await;
    }
}