use std::sync::Arc;
use std::sync::atomic::{AtomicUsize, Ordering};

mod mt;
mod init;
mod dsc;

use mt::threadpool::interface::i_threadpool;
use init::i_init;

fn myfunc()->() {
    println!("myfunc!");
}

fn main() {
    
    let var = Arc::new( AtomicUsize::new(0) );
    let v = var.clone();
    let t  = std::thread::spawn( move || {
        v.fetch_add( 1, Ordering::Release );
    });
    let ret = t.join();
    assert!( !ret.is_err() );
    let a = var.load( Ordering::Acquire );
    assert_eq!( 1, a );

    println!( "{}", a );

    
    let mut mypool = mt::threadpool::threadpool0::tp0::new( myfunc );
    assert_eq!( true, mypool.init() );
    let val = mypool.withdraw();
    assert_eq!( true, val.is_some() );
    assert_eq!( 2, val.unwrap() );

    (mypool._f)();
    
    println!("end.");
}
