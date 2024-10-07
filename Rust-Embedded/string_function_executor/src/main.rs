#![no_std]
#![no_main]

mod fmt;

#[cfg(not(feature = "defmt"))]
use panic_halt as _;
#[cfg(feature = "defmt")]
use {defmt_rtt as _, panic_probe as _};

use embassy_executor::Spawner;
use fmt::info;

use embassy_stm32::{Config, Peripherals};
use embassy_stm32::time::Hertz;


#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    info!("INIT:START");

    let config: Config = generate_blackpill_clock_config();
    
    let p: Peripherals = embassy_stm32::init(config);

}

fn generate_blackpill_clock_config() -> Config {
    let mut config: Config = Config::default();
    {
        use embassy_stm32::rcc::*;
        config.rcc.hse = Some(Hse {
            freq: Hertz(25_000_000),
            mode: HseMode::Oscillator,
        });
        config.rcc.pll_src = PllSource::HSE;
        config.rcc.pll = Some(Pll {
            prediv: PllPreDiv::DIV25,
            mul: PllMul::MUL192,
            divp: Some(PllPDiv::DIV2), // 25mhz / 25 * 192 / 2 = 96Mhz.
            divq: Some(PllQDiv::DIV4), // 25mhz / 25 * 192 / 4 = 48Mhz.
            divr: None,
        });
        config.rcc.ahb_pre = AHBPrescaler::DIV1;
        config.rcc.apb1_pre = APBPrescaler::DIV4;
        config.rcc.apb2_pre = APBPrescaler::DIV2;
        config.rcc.sys = Sysclk::PLL1_P; // 96Mhz
        config.rcc.mux.clk48sel = mux::Clk48sel::PLL1_Q; // 48Mhz
    }
    return config;
}