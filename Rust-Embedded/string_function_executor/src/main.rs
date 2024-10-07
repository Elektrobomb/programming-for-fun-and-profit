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

// Macro to generate the Func enum and call implementations
macro_rules! define_func_enum {
    // Base case for when no parameters are needed
    () => {};

    // Match for defining enum with functions of up to 8 parameters
    ($name:ident, $([$variant:ident, $func_type:ty, ($($arg:ident: $arg_ty:ty),*)]),*) => {
        // Define the enum with variants for each function signature
        enum $name {
            $(
                $variant(fn($($arg_ty),*) -> i32),
            )*
        }

        // Implement the `call` method for each function signature
        impl $name {
            $(
                fn $variant(&self, $($arg: $arg_ty),*) -> i32 {
                    match self {
                        $name::$variant(f) => f($($arg),*),
                        _ => panic!("Called the wrong function signature"),
                    }
                }
            )*
        }
    };
}

// Define the enum for storing functions with up to 8 parameters (i32 and f32)
define_func_enum!(Func,
    [FnOneI32, fn(i32) -> i32, (x: i32)],
    [FnOneF32, fn(f32) -> i32, (x: f32)],
    [FnTwoI32, fn(i32, i32) -> i32, (x: i32, y: i32)],
    [FnTwoMixed, fn(i32, f32) -> i32, (x: i32, y: f32)],
    [FnThreeMixed, fn(i32, i32, f32) -> i32, (x: i32, y: i32, z: f32)],
    [FnFourMixed, fn(i32, i32, f32, f32) -> i32, (x: i32, y: i32, z: f32, w: f32)],
    [FnFiveMixed, fn(i32, f32, f32, i32, f32) -> i32, (x: i32, y: f32, z: f32, a: i32, b: f32)],
    [FnSixMixed, fn(i32, i32, i32, f32, f32, f32) -> i32, (x: i32, y: i32, z: i32, a: f32, b: f32, c: f32)]
);

fn add_one(x: i32) -> i32 {
    x + 1
}

fn add_one_f32(x: f32) -> i32 {
    (x + 1.0) as i32
}

fn add_two(x: i32, y: i32) -> i32 {
    x + y
}

fn add_mixed(x: i32, y: f32) -> i32 {
    (x as f32 + y) as i32
}

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    info!("INIT:START");

    let config: Config = generate_blackpill_clock_config();
    
    let p: Peripherals = embassy_stm32::init(config);

    // Create an array-like structure (Vec) of different functions
    let funcs: [Func; 4] = [
        Func::FnOneI32(add_one),     // Function with one i32 argument
        Func::FnOneF32(add_one_f32), // Function with one f32 argument
        Func::FnTwoI32(add_two),     // Function with two i32 arguments
        Func::FnTwoMixed(add_mixed), // Function with i32 and f32 arguments
    ];

    // Call the functions using the abstracted enum
    let result1 = funcs[0].FnOneI32(5);
    info!("Result of add_one: {}", result1);

    let result2 = funcs[1].FnOneF32(5.5);
    info!("Result of add_one_f32: {}", result2);

    let result3 = funcs[2].FnTwoI32(5, 10);
    info!("Result of add_two: {}", result3);

    let result4 = funcs[3].FnTwoMixed(5, 10.5);
    info!("Result of add_mixed: {}", result4);
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