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

use heapless::{Vec, LinearMap, String}; // heapless::String is used here
use core::str::FromStr;

const MAX_PARAMS: usize = 4;
const MAX_FUNCTIONS: usize = 8;
const MAX_STRING_LEN: usize = 32;

// Enum to handle both i32 and f32 types
#[derive(Debug, Clone, Copy)]
enum Value {
    I32(i32),
    F32(f32),
}

impl Value {
    fn as_i32(self) -> Option<i32> {
        if let Value::I32(val) = self {
            Some(val)
        } else {
            None
        }
    }

    fn as_f32(self) -> Option<f32> {
        if let Value::F32(val) = self {
            Some(val)
        } else {
            None
        }
    }
}

// Type for function signatures
type Function = fn(Vec<Value, MAX_PARAMS>) -> Value;

struct FunctionRegistry {
    functions: LinearMap<heapless::String<MAX_STRING_LEN>, Function, MAX_FUNCTIONS>,
}

impl FunctionRegistry {
    const fn new() -> Self {
        FunctionRegistry {
            functions: LinearMap::new(),
        }
    }

    fn register(&mut self, name: &'static str, func: Function) {
        let mut string_name: heapless::String<MAX_STRING_LEN> = heapless::String::new();
        string_name.push_str(name).ok();  // Add static str to heapless::String
        self.functions.insert(string_name, func).ok();
    }

    fn call_function(&self, input: heapless::String<MAX_STRING_LEN>) -> Result<Value, &'static str> {
        // Parse input like "multiple(3,7)"
        let (func_name, params_str) = self.parse_input(&input)?;

        // Parse and convert params to Value (either i32 or f32)
        let params = self.parse_params(&params_str)?;

        // Call the registered function
        if let Some(function) = self.functions.get(&func_name) {
            Ok(function(params))
        } else {
            Err("Function not found")
        }
    }

    fn parse_input(
        &self,
        input: &heapless::String<MAX_STRING_LEN>,
    ) -> Result<(heapless::String<MAX_STRING_LEN>, heapless::String<MAX_STRING_LEN>), &'static str> {
        if let Some(pos) = input.find('(') {
            let mut func_name: heapless::String<MAX_STRING_LEN> = heapless::String::new();
            func_name.push_str(&input[..pos]).map_err(|_| "String too long")?;
            
            let mut params_str: heapless::String<MAX_STRING_LEN> = heapless::String::new();
            params_str.push_str(&input[pos + 1..input.len() - 1]).map_err(|_| "String too long")?;
            
            Ok((func_name, params_str))
        } else {
            Err("Invalid input format")
        }
    }

    fn parse_params(
        &self,
        params_str: &heapless::String<MAX_STRING_LEN>,
    ) -> Result<Vec<Value, MAX_PARAMS>, &'static str> {
        let mut params: Vec<Value, MAX_PARAMS> = Vec::new();
        
        // Split the parameter string by comma
        for param in params_str.split(',') {
            let param = param.trim();  // Trim whitespace
            let mut param_string: heapless::String<MAX_STRING_LEN> = heapless::String::new();
            param_string.push_str(param).map_err(|_| "Param string too long")?;
            
            // Try parsing as i32, else try f32
            if let Ok(num) = i32::from_str(&param_string) {
                params.push(Value::I32(num)).map_err(|_| "Too many params")?;
            } else if let Ok(num) = f32::from_str(&param_string) {
                params.push(Value::F32(num)).map_err(|_| "Too many params")?;
            } else {
                return Err("Invalid number format");
            }
        }
        
        Ok(params)
    }
}

// Example functions to handle both i32 and f32
fn multiple(params: Vec<Value, MAX_PARAMS>) -> Value {
    if params.len() == 2 {
        match (params[0], params[1]) {
            (Value::I32(a), Value::I32(b)) => Value::I32(a * b),
            (Value::F32(a), Value::F32(b)) => Value::F32(a * b),
            _ => Value::I32(-1), // Handle mismatched types
        }
    } else {
        Value::I32(-1) // Error value for incorrect parameter count
    }
}

fn add(params: Vec<Value, MAX_PARAMS>) -> Value {
    if params.len() == 2 {
        match (params[0], params[1]) {
            (Value::I32(a), Value::I32(b)) => Value::I32(a + b),
            (Value::F32(a), Value::F32(b)) => Value::F32(a + b),
            _ => Value::I32(-1), // Handle mismatched types
        }
    } else {
        Value::I32(-1) // Error value for incorrect parameter count
    }
}

// Macro to register functions more easily
macro_rules! register_functions {
    ($registry:expr, $( $name:expr => $func:expr ),* ) => {
        $(
            $registry.register($name, $func);
        )*
    };
}

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    info!("INIT:START");

    let config: Config = generate_blackpill_clock_config();
    
    let p: Peripherals = embassy_stm32::init(config);

    let mut function_registry = FunctionRegistry::new();

    // Register functions
    register_functions!(function_registry,
        "multiple" => multiple,
        "add" => add
    );

    // Example: Call function with dynamically created string
    let mut input: heapless::String<MAX_STRING_LEN> = heapless::String::new();
    input.push_str("multiple(3,7)").ok();

    if let Ok(output) = function_registry.call_function(input) {
        // Handle i32 or f32 output
        match output {
            Value::I32(val) => {
                info!("Result: {}", val);
            }
            Value::F32(val) => {
                info!("Result: {}", val);
            }
        }
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