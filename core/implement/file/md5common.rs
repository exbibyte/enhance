use std::io;
use std::io::prelude::*;
use std::fs::File;
use std::io::BufReader;
use std::str;
use std::os;
use std::collections::HashMap;
use std::str::Chars;
use std::iter::Peekable;
use std::str::FromStr;

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

pub fn tokenize < KeywordTok > ( input: &str, idx: usize, hm: & HashMap< &str, KeywordTok > ) -> ( Token, Option< KeywordTok >, usize, usize, usize ) where KeywordTok : Copy {
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
        None => return ( Token::End, None, idxcurrent_start, idxcurrent, idxcurrent ),
        Some(x) => x
    };
    
    // println!("index:.. {}", idxcurrent );
    
    if c == '{' { //process groupings
        return ( Token::Bracel, None, idxcurrent_start, idxcurrent + 1, idxcurrent + 1 )
    }else if c == '}' {
        return ( Token::Bracer, None, idxcurrent_start, idxcurrent + 1, idxcurrent + 1 )
    }else if c == '(' {
        return ( Token::Parenl, None, idxcurrent_start, idxcurrent + 1, idxcurrent + 1 )
    }else if c == ')' {
        return ( Token::Parenr, None, idxcurrent_start, idxcurrent + 1, idxcurrent + 1 )
    }else if c == '/' { //process comments
        let c_2 = match iter.next() {
            None => return ( Token::Invalid, None, idxcurrent_start, idxcurrent + 1, idxcurrent + 1 ),
            Some(x) => x
        };
        idxcurrent += 1;
        if c_2 == '/' {
            while let Some(x) = iter.next() {
                if x == '\n' {
                    // println!("new line detected.");
                    // return ( Token::Comment, None, idxcurrent_start + 2, idxcurrent + 1, idxcurrent + 1 )
                    return tokenize( input, idxcurrent, hm )
                }
                idxcurrent += 1;
            }
             // return ( Token::Comment, None, idxcurrent_start + 2, idxcurrent, idxcurrent + 1 )
            return tokenize( input, idxcurrent, hm )
        }else{
            return ( Token::Invalid, None, idxcurrent_start, idxcurrent, idxcurrent + 1 )
        }
    }
    else if c == '"' { //process strings
        idxcurrent += 1;
        idxcurrent_start += 1;
        while let Some(x) = iter.next() {
            if x == '"' {
                return ( Token::Str, None, idxcurrent_start, idxcurrent, idxcurrent + 1 )
            }else if !x.is_alphanumeric() && x != '_' && x != '/'{
                println!("unexpected character in string: {}", x);
                return ( Token::Invalid, None, idxcurrent_start, idxcurrent, idxcurrent )
            }
            idxcurrent += 1;
        }
        return ( Token::Invalid, None, idxcurrent_start, idxcurrent, idxcurrent )
    }
    else if c.is_alphabetic() || c == '_' { //process keywords and strings
        idxcurrent += 1;
        while let Some(x) = iter.next() {
            if !x.is_alphanumeric() && x != '_' {
                if let Some(mapped_val) = hm.get( &input[ idxcurrent_start..idxcurrent ] ){
                    return ( Token::Keyword, Some(*mapped_val), idxcurrent_start, idxcurrent, idxcurrent ) 
                }else{
                    return ( Token::Str, None, idxcurrent_start, idxcurrent, idxcurrent )
                }
            }
            idxcurrent += 1;
        }
        if let Some(mapped_val) = hm.get( &input[ idxcurrent_start..idxcurrent ] ){
            return ( Token::Keyword, Some(*mapped_val), idxcurrent_start, idxcurrent, idxcurrent ) 
        }else{
            return ( Token::Str, None, idxcurrent_start, idxcurrent, idxcurrent )
        }
    }

    if c == '-' || c == '+' { //process optional signedness of a number
        idxcurrent += 1;
        is_float_signed = true;
        let next_c = iter.peek();
        match next_c { //expect a decimal or digit after
            None => return ( Token::Invalid, None, idxcurrent_start, idxcurrent, idxcurrent ),
            Some(x) => {
                if !x.is_numeric() || *x == '.' {
                    return ( Token::Invalid, None, idxcurrent_start, idxcurrent, idxcurrent )
                }
            }
        }
    }

    let c2 = if is_float_signed { //eat a character if the current character is +/-
        match iter.next() {
            None => return ( Token::Invalid, None, idxcurrent_start, idxcurrent, idxcurrent ),
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
                return ( Token::Float, None, idxcurrent_start, idxcurrent, idxcurrent )
            }
            idxcurrent += 1;
        }
        return ( Token::Str, None, idxcurrent_start, idxcurrent, idxcurrent )
    }else if c2.is_numeric() { //process int or float
        idxcurrent += 1;
        loop { //process digits
            {
                let x = match iter.peek() {
                    None => return ( Token::Int, None, idxcurrent_start, idxcurrent, idxcurrent ),
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
                    return ( Token::Int, None, idxcurrent_start, idxcurrent, idxcurrent )
                }else{
                    { //consume decimal
                        idxcurrent += 1; 
                        iter.next();
                    }
                    loop { //digits
                        if let Some(y) = iter.peek() {
                            if !y.is_numeric() {
                                return ( Token::Float, None, idxcurrent_start, idxcurrent, idxcurrent )
                            }   
                        }
                        iter.next();
                        idxcurrent += 1;
                    }
                    return ( Token::Float, None, idxcurrent_start, idxcurrent, idxcurrent )
                }
            }
        }
    }
    //skip unrecognized character
    ( Token::Undetermined, None, idxcurrent_start, idxcurrent + 1, idxcurrent + 1 )
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

pub fn expect_int_array( input: &str, idx: usize, count: usize, arr: & mut [i64] ) -> Result< usize, & 'static str > {
    let mut idx_current = idx;
    let mut hm : HashMap< &str, bool > = HashMap::new();
    for i in 0..count {
        let ( tok, kw_tok, idx_s, idx_e, idx_next ) = tokenize( input, idx_current, & hm );
        match tok {
            Token::Int => {
                arr[i] = i64::from_str( &input[idx_s..idx_e] ).expect("md5common parse int array invalid");
                idx_current = idx_next;
            },
            _ => return Err("unexpected token. int not found.")
        }
    }
    Ok( idx_current )
}

pub fn expect_float_array( input: &str, idx: usize, count: usize, arr: & mut [f64] ) -> Result< usize, & 'static str > {
    let mut idx_current = idx;
    let mut hm : HashMap< &str, bool > = HashMap::new();
    for i in 0..count {
        let ( tok, kw_tok, idx_s, idx_e, idx_next ) = tokenize( input, idx_current, & hm );
        match tok {
            Token::Float => {
                arr[i] = f64::from_str( &input[idx_s..idx_e] ).expect("md5common parse int array invalid");
                idx_current = idx_next;
            },
            _ => return Err("unexpected token. float not found.")
        }
    }
    Ok( idx_current )
}

pub fn expect_parenl( input: &str, idx: usize ) -> Result< usize, & 'static str > {
    let mut idx_current = idx;
    let mut hm : HashMap< &str, bool > = HashMap::new();
    let ( tok, kw_tok, idx_s, idx_e, idx_next ) = tokenize( input, idx_current, & hm );
    match tok {
        Token::Parenl => return Ok( idx_next ),
        _ => return Err("unexpected token. parenl not found.")
    }
}

pub fn expect_parenr( input: &str, idx: usize ) -> Result< usize, & 'static str > {
    let mut idx_current = idx;
    let mut hm : HashMap< &str, bool > = HashMap::new();
    let ( tok, kw_tok, idx_s, idx_e, idx_next ) = tokenize( input, idx_current, & hm );
    match tok {
        Token::Parenr => return Ok( idx_next ),
        _ => return Err("unexpected token. parenr not found.")
    }
}

pub fn expect_bracel( input: &str, idx: usize ) -> Result< usize, & 'static str > {
    let mut idx_current = idx;
    let mut hm : HashMap< &str, bool > = HashMap::new();
    let ( tok, kw_tok, idx_s, idx_e, idx_next ) = tokenize( input, idx_current, & hm );
    match tok {
        Token::Bracel => return Ok( idx_next ),
        _ => return Err("unexpected token. bracel not found.")
    }
}

pub fn expect_bracer( input: &str, idx: usize ) -> Result< usize, & 'static str > {
    let mut idx_current = idx;
    let mut hm : HashMap< &str, bool > = HashMap::new();
    let ( tok, kw_tok, idx_s, idx_e, idx_next ) = tokenize( input, idx_current, & hm );
    match tok {
        Token::Bracer => return Ok( idx_next ),
        _ => return Err("unexpected token. bracer not found.")
    }
}

pub fn expect_str< KeywordTok >( input: &str, idx: usize, hm: & HashMap< &str, KeywordTok >, ret_str: & mut String ) -> Result< usize, & 'static str > where KeywordTok : Copy {
    let mut idx_current = idx;
    let ( tok, kw_tok, idx_s, idx_e, idx_next ) = tokenize( input, idx_current, & hm );
    match tok {
        Token::Str => {
            *ret_str = input[idx_s..idx_e].to_string();
            return Ok( idx_next )
        },
        _ => { return Err("unexpected token. string not found."); }
    }
    return Err("unexpected token. string not found.")
}
