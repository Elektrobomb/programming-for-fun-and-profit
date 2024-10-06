# Setting Up Embassy for Rust Embedded Development

This guide will help you set up Embassy for Rust embedded development. Follow the steps below to get started.

## Prerequisites
 - See here for embassy prereqs: https://embassy.dev/book/#_getting_started
    - Ignore the examples given as their config is broken and will not work correctly (hence this guide)
 - Set up your debug probe here: https://probe.rs/docs/getting-started/probe-setup/
 - Install cargo dfu for dfu uploads: https://crates.io/crates/cargo-dfu
 - Install cargo-embassy to config your project super easily (this is a godsend)
    - Run `cargo install cargo-embassy`

## Step-by-Step guide to making a new project
### 1. Create a New Project with cargo-embassy

Create a new Rust project:
 - Replace project_name with your desired project name
 - Replace chip_name with your MCU ID (e.g. stm32f411ce)

```sh
cargo embassy init {project_name} --chip {chip_name}
cd {project_name}
```

This will generate a full Cargo file structure with appropriate packages and a blink light example.  

Try changing the LED pin to match your dev board! (PC13 for the STM32F411 BlackPill)

### 2. Build and Flash Your Application

Build your project:

```sh
cargo run
```

If the debug probe is setup correctly, this should push the code over your debug probe to the MCU and start it running

If you run into an error, try reducing the SWD baudrate:
```sh
cargo run -- --speed 100
```

## Conclusion

You have now set up Embassy for Rust embedded development. Happy coding!

For more information, visit the [Embassy documentation](https://embassy.dev/book/).

# Further findings 6/10/2024
 - A lot of the features of embassy seem to be in flux right now and the documentation + examples are all written for the main branch of embassy rather than the latest release!!!  

 - If you run into issues caused by this (e.g. a feature / parameter listed in the documentation is missing), try removing the version tags from all of the embassy dependencies in your Cargo.toml and replacing them with `git = git = "https://github.com/embassy-rs/embassy"`

 - This will download the latest features etc from `main` and should be more compatible with their documentation etc.  IMO this is a dumb problem.  
