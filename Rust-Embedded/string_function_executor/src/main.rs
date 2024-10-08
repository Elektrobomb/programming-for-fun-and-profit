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

use heapless::{Vec, LinearMap, String};
use core::str::FromStr; // for parsing numeric types

const MAX_PARAMS: usize = 4;
const MAX_FUNCTIONS: usize = 8;
const MAX_STRING_LEN: usize = 32;

type Function = fn(Vec<i32, MAX_PARAMS>) -> i32;

struct FunctionHandler {
    functions: LinearMap<&'static str, Function, MAX_FUNCTIONS>,
}

impl FunctionHandler {
    const fn new() -> Self {
        FunctionHandler {
            functions: LinearMap::new(),
        }
    }

    fn register(&mut self, name: &'static str, func: Function) {
        self.functions.insert(name, func).ok();
    }

    fn call_function(&self, input: &str) -> Result<i32, &'static str> {
        // Parse input string like "multiple(3,7)"
        let (func_name, params_str) = self.parse_input(input)?;

        // Split and convert params to i32
        let params = self.parse_params(params_str)?;

        // Call the registered function
        if let Some(function) = self.functions.get(func_name) {
            Ok(function(params))
        } else {
            Err("Function not found")
        }
    }

    fn parse_input<'a>(&self, input: &'a str) -> Result<(&'a str, &'a str), &'static str> {
        if let Some(pos) = input.find('(') {
            let func_name = &input[..pos];
            let params_str = &input[pos + 1..input.len() - 1];
            Ok((func_name, params_str))
        } else {
            Err("Invalid input format")
        }
    }

    fn parse_params(&self, params_str: &str) -> Result<Vec<i32, MAX_PARAMS>, &'static str> {
        let mut params: Vec<i32, MAX_PARAMS> = Vec::new();
        for param in params_str.split(',') {
            let num = i32::from_str(param.trim()).map_err(|_| "Invalid number")?;
            params.push(num).map_err(|_| "Too many params")?;
        }
        Ok(params)
    }
}

// Example functions that return i32
fn multiple(params: Vec<i32, MAX_PARAMS>) -> i32 {
    if params.len() == 2 {
        params[0] * params[1]
    } else {
        -1 // Error value for incorrect parameter count
    }
}

fn add(params: Vec<i32, MAX_PARAMS>) -> i32 {
    if params.len() == 2 {
        params[0] + params[1]
    } else {
        -1 // Error value for incorrect parameter count
    }
}

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    info!("INIT:START");

    let config: Config = generate_blackpill_clock_config();
    
    let p: Peripherals = embassy_stm32::init(config);

    let mut handler = FunctionHandler::new();

    // Register functions
    handler.register("multiple", multiple);
    handler.register("add", add);

    // Call the functions
    if let Ok(output) = handler.call_function("multiple(3,7)") {
        info!("Result: {}", output);
    }

    if let Ok(output) = handler.call_function("add(5,2)") {
        info!("Result: {}", output);
    }
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