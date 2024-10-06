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
use embassy_futures::join::join;
use fmt::info;

use embassy_stm32::{bind_interrupts, peripherals, Config, usb};
use embassy_usb::class::cdc_acm::{CdcAcmClass, State};
use embassy_usb::Builder;
use embassy_usb::driver::EndpointError;
use embassy_stm32::usb::{Driver, Instance};
use embassy_stm32::time::Hertz;

bind_interrupts!(struct Irqs {
    OTG_FS => usb::InterruptHandler<peripherals::USB_OTG_FS>;
});

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    info!("INIT:START");

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
    
    let p = embassy_stm32::init(config);
    // Spawn the LED task.
    // Degrading the pin to AnyPin allows us to pass it to the task.
    // Usually Embassy pins are singletons but we can degrade them to AnyPin to allow for more generic handling
    _spawner.spawn(led_task(p.PC13.degrade())).unwrap();

    // Create the driver, from the HAL.
    let mut ep_out_buffer = [0u8; 256];
    let mut config = embassy_stm32::usb::Config::default();

    config.vbus_detection = false;

    let usb_driver = Driver::new_fs(p.USB_OTG_FS, Irqs, p.PA12, p.PA11, &mut ep_out_buffer, config);

    // Create embassy-usb Config
    let mut usb_config = embassy_usb::Config::new(0xc0de, 0xcafe);
    usb_config.manufacturer = Some("Embassy");
    usb_config.product = Some("USB-serial example");
    usb_config.serial_number = Some("12345678");

    // Required for windows compatibility.
    usb_config.device_class = 0xEF;
    usb_config.device_sub_class = 0x02;
    usb_config.device_protocol = 0x01;
    usb_config.composite_with_iads = true;

    // Create embassy-usb DeviceBuilder using the driver and config.
    // It needs some buffers for building the descriptors.
    let mut usb_config_descriptor = [0; 256];
    let mut usb_bos_descriptor = [0; 256];
    let mut usb_control_buf = [0; 64];

    let mut usb_state = State::new();

    let mut usb_builder = Builder::new(
        usb_driver,
        usb_config,
        &mut usb_config_descriptor,
        &mut usb_bos_descriptor,
        &mut [], // no msos descriptors
        &mut usb_control_buf,
    );

    // Create classes on the builder.
    let mut class = CdcAcmClass::new(&mut usb_builder, &mut usb_state, 64);

    // Build the builder.
    let mut usb = usb_builder.build();

    // Run the USB device.
    let usb_fut = usb.run();

    // Do stuff with the class!
    let echo_fut = async {
        loop {
            class.wait_connection().await;
            info!("Connected");
            let _ = echo(&mut class).await;
            info!("Disconnected");
        }
    };

    join(usb_fut, echo_fut).await;

}

#[embassy_executor::task]
async fn led_task(led_pin: AnyPin) {
    let mut led = Output::new(led_pin, Level::High, Speed::Low);

    loop {
        //info!("Heartbeat()");
        led.set_high();
        Timer::after(Duration::from_millis(500)).await;
        led.set_low();
        Timer::after(Duration::from_millis(500)).await;
    }
}

struct Disconnected {}

impl From<EndpointError> for Disconnected {
    fn from(val: EndpointError) -> Self {
        match val {
            EndpointError::BufferOverflow => panic!("Buffer overflow"),
            EndpointError::Disabled => Disconnected {},
        }
    }
}

async fn echo<'d, T: Instance + 'd>(class: &mut CdcAcmClass<'d, Driver<'d, T>>) -> Result<(), Disconnected> {
    let mut buf = [0; 64];
    loop {
        let n = class.read_packet(&mut buf).await?;
        let data = &buf[..n];
        info!("data: {:x}", data);
        class.write_packet(data).await?;

        if data == b"hi!" {
            class.write_packet(b"Hello, world!\r\n").await?;
        }
    }
}