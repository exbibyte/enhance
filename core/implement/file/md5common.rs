use std::io;
use std::io::prelude::*;
use std::fs::File;
use std::io::BufReader;
use std::str;
use std::os;
use std::collections::HashMap;
use std::str::Chars;
use std::iter::Peekable;

#[derive(Debug)]
#[derive(PartialEq)]
pub enum Token {
    Undetermined,
    Comment,
    Keyword,
    Str,
    Int,
    Float,
    Parenl,
    Parenr,
    Bracel,
    Bracer,
    Invalid,
    End,
}

pub fn tokenize < KeywordVal > ( input: &str, idx: usize, hm: & mut HashMap< &str, KeywordVal > ) -> ( Token, usize, usize, usize ) {
    let mut is_float_signed = false;
    let mut idxcurrent = idx;
    let mut idxcurrent_start = idx;
    let mut iter = (&input[ idx.. ]).chars().peekable(); //create peekable iterator
    {
        let count_skip = skip_white_space( & mut iter );
        idxcurrent += count_skip;
        idxcurrent_start += count_skip;
    }

    let c = match iter.next() {
        None => return ( Token::End, idxcurrent_start, idxcurrent, idxcurrent ),
        Some(x) => x
    };
    
    // println!("index:.. {}", idxcurrent );
    
    if c == '{' { //process groupings
        return ( Token::Bracel, idxcurrent_start, idxcurrent + 1, idxcurrent + 1 )
    }else if c == '}' {
        return ( Token::Bracer, idxcurrent_start, idxcurrent + 1, idxcurrent + 1 )
    }else if c == '(' {
        return ( Token::Parenl, idxcurrent_start, idxcurrent + 1, idxcurrent + 1 )
    }else if c == ')' {
        return ( Token::Parenr, idxcurrent_start, idxcurrent + 1, idxcurrent + 1 )
    }else if c == '/' { //process comments
        let c_2 = match iter.next() {
            None => return ( Token::Invalid, idxcurrent_start, idxcurrent + 1, idxcurrent + 1 ),
            Some(x) => x
        };
        idxcurrent += 1;
        if c_2 == '/' {
            while let Some(x) = iter.next() {
                if x == '\n' {
                    // println!("new line detected.");
                    return ( Token::Comment, idxcurrent_start + 2, idxcurrent + 1, idxcurrent + 1 )
                }
                idxcurrent += 1;
            }
            return ( Token::Comment, idxcurrent_start + 2, idxcurrent, idxcurrent + 1 )
        }else{
            return ( Token::Invalid, idxcurrent_start, idxcurrent, idxcurrent + 1 )
        }
    }
    else if c == '"' { //process strings
        idxcurrent += 1;
        idxcurrent_start += 1;
        while let Some(x) = iter.next() {
            if x == '"' {
                return ( Token::Str, idxcurrent_start, idxcurrent, idxcurrent + 1 )
            }else if !x.is_alphanumeric() && x != '_' && x != '/'{
                println!("unexpected character in string: {}", x);
                return ( Token::Invalid, idxcurrent_start, idxcurrent, idxcurrent )
            }
            idxcurrent += 1;
        }
        return ( Token::Invalid, idxcurrent_start, idxcurrent, idxcurrent )
    }
    else if c.is_alphabetic() || c == '_' { //process keywords and strings
        idxcurrent += 1;
        while let Some(x) = iter.next() {
            if !x.is_alphanumeric() && x != '_' {
                if let Some(mapped_val) = hm.get( &input[ idxcurrent_start..idxcurrent ] ){
                    return ( Token::Keyword, idxcurrent_start, idxcurrent, idxcurrent ) 
                }else{
                    return ( Token::Str, idxcurrent_start, idxcurrent, idxcurrent )
                }
            }
            idxcurrent += 1;
        }
        if let Some(mapped_val) = hm.get( &input[ idxcurrent_start..idxcurrent ] ){
            return ( Token::Keyword, idxcurrent_start, idxcurrent, idxcurrent ) 
        }else{
            return ( Token::Str, idxcurrent_start, idxcurrent, idxcurrent )
        }
    }

    if c == '-' || c == '+' { //process optional signedness of a number
        idxcurrent += 1;
        is_float_signed = true;
        let next_c = iter.peek();
        match next_c { //expect a decimal or digit after
            None => return ( Token::Invalid, idxcurrent_start, idxcurrent, idxcurrent ),
            Some(x) => {
                if !x.is_numeric() || *x == '.' {
                    return ( Token::Invalid, idxcurrent_start, idxcurrent, idxcurrent )
                }
            }
        }
    }

    let c2 = if is_float_signed { //eat a character if the current character is +/-
        match iter.next() {
            None => return ( Token::Invalid, idxcurrent_start, idxcurrent, idxcurrent ),
            Some(x) => x
        }
    }else{
        c
    };
    
    if c2 == '.' { //process float
        idxcurrent += 1;
        while let Some(x) = iter.next() {
            if !x.is_numeric() {
                // println!("number detected.");
                return ( Token::Float, idxcurrent_start, idxcurrent, idxcurrent )
            }
            idxcurrent += 1;
        }
        return ( Token::Str, idxcurrent_start, idxcurrent, idxcurrent )
    }else if c2.is_numeric() { //process int or float
        idxcurrent += 1;
        loop { //process digits
            {
                let x = match iter.peek() {
                    None => return ( Token::Int, idxcurrent_start, idxcurrent, idxcurrent ),
                    Some(x) => x
                };
                if !x.is_numeric() {
                    break;
                }
            }
            iter.next();
            idxcurrent += 1;
        }
        { //process decimal
            {
                if *iter.peek().expect("character invalid") != '.' { //int
                    return ( Token::Int, idxcurrent_start, idxcurrent, idxcurrent )
                }else{
                    { //consume decimal
                        idxcurrent += 1; 
                        iter.next();
                    }
                    loop { //digits
                        if let Some(y) = iter.peek() {
                            if !y.is_numeric() {
                                return ( Token::Float, idxcurrent_start, idxcurrent, idxcurrent )
                            }   
                        }
                        iter.next();
                        idxcurrent += 1;
                    }
                    return ( Token::Float, idxcurrent_start, idxcurrent, idxcurrent )
                }
            }
        }
    }
    //skip unrecognized character
    ( Token::Undetermined, idxcurrent_start, idxcurrent + 1, idxcurrent + 1 )
}

fn skip_white_space( iter: & mut Peekable<Chars> ) -> usize {
    let mut skip_count = 0;
    loop { //skip whitespace
        let is_white_space = match iter.peek() {
            Some(x) => {
                let result = if *x == ' ' || *x == '\n' || *x == '\t' || *x == '\r' {
                    skip_count += 1;
                    true
                }else{
                    false
                };
                result
            },
            None => return skip_count
        };
        if is_white_space {
            iter.next();
        }else{
            break;
        }
    }
    return skip_count
}

pub fn file_open( file_path: & str ) -> Option<String> {
    let path = File::open( file_path ).expect("file path open invalid");
    let mut buf_reader = BufReader::new(path);
    let mut contents = String::new();
    buf_reader.read_to_string( & mut contents );
    Some(contents)
}
