#![no_std]
#![no_main]

mod fmt;

use cortex_m::peripheral;
#[cfg(not(feature = "defmt"))]
use panic_halt as _;
#[cfg(feature = "defmt")]
use {defmt_rtt as _, panic_probe as _};

use embassy_executor::Spawner;
use embassy_stm32::gpio::{AnyPin, Level, Output, Speed};
use embassy_time::{Duration, Timer};
use fmt::info;

/// This crate provides a library for controlling LED matrices using the Embassy framework.
use embassy_initial_test::embassy_led_matrix::{self, LedMatrix};

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    let p: embassy_stm32::Peripherals = embassy_stm32::init(Default::default());
    let mut led: Output<'_, embassy_stm32::peripherals::PB7> = Output::new(p.PB7, Level::High, Speed::Low);


    let raw_pins: [AnyPin; 8] = [
        p.PA0.into(),
        p.PA1.into(),
        p.PA2.into(),
        p.PA3.into(),
        p.PA4.into(),
        p.PA5.into(),
        p.PA6.into(),
        p.PA7.into(),
    ];

    let output_pins: [Output<'_, AnyPin>; 8] = [
        Output::new(raw_pins[0], Level::Low, Speed::Low),
        Output::new(raw_pins[1], Level::Low, Speed::Low),
        Output::new(raw_pins[2], Level::Low, Speed::Low),
        Output::new(raw_pins[3], Level::Low, Speed::Low),
        Output::new(raw_pins[4], Level::Low, Speed::Low),
        Output::new(raw_pins[5], Level::Low, Speed::Low),
        Output::new(raw_pins[6], Level::Low, Speed::Low),
        Output::new(raw_pins[7], Level::Low, Speed::Low),
    ];

    loop {
        info!("Hello, World!");
        led.set_high();
        Timer::after(Duration::from_millis(500)).await;
        led.set_low();
        Timer::after(Duration::from_millis(500)).await;
    }
}