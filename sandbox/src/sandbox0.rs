#![feature(libc)]

extern crate libc;

use std::mem;

fn main() {
    unsafe {
        let my_num: *mut i32 = libc::malloc(mem::size_of::<i32>()) as *mut i32;
        if my_num.is_null() {
            panic!("failed to allocate memory");
        }
        libc::free(my_num as *mut libc::c_void);
    }
}

// fn main(){
//     let s: &str = "123";
//     let ptr: *const u8 = s.as_ptr();
//     unsafe {
// 	println!("{}", *ptr.offset(1) as char);
// 	println!("{}", *ptr.offset(2) as char);
//     }
// }
