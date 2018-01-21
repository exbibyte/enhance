#![feature(use_extern_macros)]

extern crate e2rcore;

use self::e2rcore::implement::kernel::kernel_impl_000::KernelImpl000;

#[allow(unused)]
fn main() {

    let mut k = KernelImpl000::init().unwrap();
    k.run();

}
