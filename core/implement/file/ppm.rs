use std::str;
use std::os;
use std::collections::HashMap;
use std::str::Chars;
use std::iter::Peekable;
use std::str::FromStr;
use std::clone::Clone;
use std::io;
use std::io::prelude::*;
use std::fs::File;
use std::io::BufReader;

pub fn read( file_path: & str, w: u64, h: u64 ) -> Result< Vec< u8 >, & 'static str > {
    let path = File::open( file_path ).expect("file path open invalid");
    let mut buf_reader = BufReader::new( path );
    let mut file_content = String::new();
    buf_reader.read_to_string( & mut file_content );
    let content = file_content.as_str().chars().collect::<Vec<_>>();
    let mut header = [0i64;3]; // width, height, maxval
    if content[0..2].iter().cloned().collect::<String>().as_str() != "P6" {
        return Err( "invalid PPM file header" )
    }
    let mut i = 0;
    let mut index = 2;
    while index < content.len() {
        if i >= 3 {
            break;
        }
        if content[index] == '#' {
            //ignore comment
            index += 1;
            while content[index] != '\n' {
                index +=1;
            }
        } else {
            //read a number to header array
            if content[index] == ' ' ||
               content[index] == '\n' {
                index += 1;
                continue;
            }else if content[index].is_digit(10) {
                let begin = index;
                index += 1;
                while content[index].is_digit(10) {
                    index += 1;
                }
                let val = i64::from_str( &file_content.as_str()[begin..index] ).unwrap();
                header[i] = val;
                i += 1;
            }else{
                return Err( "unexpected character" )
            }
        }
    }
    //read to newline
    while content[index] != '\n' {
        index +=1;
    }
    let width = header[0];
    let height = header[1];
    //read remaining file as bytes of rgb
    let img_bytes = content[index..].to_vec().into_iter().map(|x| x as u8).collect::<Vec<u8>>();
    if img_bytes.len() != ( width * height * 3 ) as usize {
        return Err( "read image byte length not match expected" )
    }
    Ok( img_bytes )
}

pub fn write( file_path: & str, w: u64, h: u64, img: & Vec < u8 > ) -> Result< (), & 'static str >
{
    let mut file = File::create( file_path ).expect("file create invalid");
    //write header
    let header = format!("P6\n{} {}\n255\n", w, h);
    let header_bytes = header.as_bytes();
    file.write( header_bytes ).expect("writing file header unsuccessful");
    //write data
    match file.write( &img[..] ) {
        Err(e) => return Err( "image write unsuccessful" ),
        _ => ()
    }
    Ok(())
}
