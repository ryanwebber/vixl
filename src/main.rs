mod application;
mod client;
mod render;
mod util;

use crate::application::Application;
use futures::executor::block_on;

fn main() {
    env_logger::init();
    log::info!("Booting vixl...");
    block_on(Application::new().run())
}
