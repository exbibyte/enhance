#![feature(use_extern_macros)]

extern crate e2rcore;
extern crate pretty_env_logger;

use std::env;

use self::e2rcore::implement::kernel::kernel_impl_000::KernelImpl000;

#[allow(unused)]
fn main() {

    env::set_var("LOG_SETTING", "info" );
    
    pretty_env_logger::init_custom_env( "LOG_SETTING" );
    
    let mut k = KernelImpl000::init().unwrap();
    k.run();

}
