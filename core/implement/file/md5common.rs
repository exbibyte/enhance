use std::collections::HashMap;
use std::str::Chars;

// use std::fs::File;
// use std::io::Read;

// fn main() {
//     let mut data = String::new();
//     let mut f = File::open("/etc/hosts").expect("Unable to open file");
//     f.read_to_string(&mut data).expect("Unable to read string");
//     println!("{}", data);
// }   
enum Md5Token {
    Comment,
    Keyword,
    Str,
    Int,
    Float,
    Bracel,
    Bracer,
    Parenl,
    Parenr,
    Invalid,
    End,
}

fn skip_white_space<'a>( keyword_map: & HashMap< &str, u32>, strstream: & String, iter: & mut Chars ){
    
}

// fn get_token( keyword_map: & HashMap, strstream: & String ){
//     unimplemented!();
// }

// fn process_vec_int( keyword_map: & HashMap, strstream: & String ){
//     unimplemented!();
// }

// fn process_vec_float( keyword_map: & HashMap, strstream: & String ){
//     unimplemented!();
// }

// fn process_int( keyword_map: & HashMap, strstream: & String ){
//     unimplemented!();
// }

// fn process_float( keyword_map: & HashMap, strstream: & String ){
//     unimplemented!();
// }
