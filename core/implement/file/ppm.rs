use std::str;
use std::io::prelude::*;
use std::fs::File;
use std::io::BufReader;

pub fn read( file_path: & str ) -> Result< ( u64, u64, Vec< u8 > ), & 'static str > {
    let path = File::open( file_path ).expect("file path open invalid");
    let mut buf_reader = BufReader::new( path );
    let mut content = vec![];
    buf_reader.read_to_end( & mut content ).is_ok();
    println!("file content len: {:?}", content.len() );

    let mut header = [0u64;4]; // width, height, maxval
    if content[0] != 'P' as u8 ||
       content[1] != '6' as u8 {
           return Err( "invalid PPM file header" )
    }
    
    let mut i = 0;
    let mut index = 2;
    while index < content.len() {
        if i >= 3 {
            break;
        }
        if content[index] == '#' as u8 {
            //ignore comment
            index += 1;
            while content[index] != '\n' as u8 {
                index +=1;
            }
        } else {
            //read a number to header array
            if content[index] == ' ' as u8 ||
               content[index] == '\n' as u8 {
                index += 1;
                continue;
            }else if content[index] >= '0' as u8 &&
                     content[index] <= '9' as u8
            {
                let begin = index;
                index += 1;
                if index >= content.len() {
                    break;
                }
                while content[index] >= '0' as u8 &&
                      content[index] <= '9' as u8
                {
                    index += 1;
                    if index >= content.len() {
                        break;
                    }
                }
                let num = &content[begin..index];
                let mut base = 1;
                let mut v = 0;
                for j in 0..num.len() {
                    v += ( num[num.len()-1-j] - ('0' as u8 ) ) as u64 * base;
                    base *= 10;
                }
                // let val = u64::from_str( num.into() ).unwrap();
                header[i] = v;
                i += 1;
            }else{
                return Err( "unexpected character" )
            }
        }
    }
    //read a number to header array
    while content[index] == ' ' as u8 ||
          content[index] == '\n' as u8 {
            index += 1;
    }
            
    // println!( "index: {}", index );
    let width = header[0];
    let height = header[1];
    // println!( "w,h: {}, {}", header[0], header[1] );

    //read remaining file as bytes of rgb
    let img_bytes = content[index..].to_vec();
    if img_bytes.len() != ( width * height * 3 ) as usize {
        return Err( "read image byte length not match expected" )
    }
    Ok( ( width, height, img_bytes ) )
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
        Err(_) => return Err( "image write unsuccessful" ),
        _ => ()
    }
    Ok(())
}
