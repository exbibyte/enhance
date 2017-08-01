extern crate gl;
extern crate libc;

use std::mem;
use std::fs::File;
use std::io::BufReader;
use std::str::FromStr;
use std::io::Read;
use std::ffi::CStr;
use std::os::raw::c_char;
use std::fmt;
use std::str;
use std::ptr;

pub enum ShaderType {
    VERTEX,
    FRAGMENT,
}

/// inputs: shader src as str, shader type.
/// outputs: shader handle
pub fn load_and_compile_shader( shader_src : &str, shader_type: ShaderType ) -> Result< gl::types::GLuint, String > {
    unsafe {
        let handle = match shader_type {
            ShaderType::VERTEX => gl::CreateShader( gl::VERTEX_SHADER ),
            ShaderType::FRAGMENT =>  gl::CreateShader( gl::FRAGMENT_SHADER ),
            _ => return Err( String::from("unknown shader type" ) ),
        };
        assert!( handle != 0 );
        let shader_src_arr = [ shader_src.as_ptr() as * const i8 ];
        let shader_src_len_arr = [ shader_src.len() as i32 ];
        gl::ShaderSource( handle, shader_src_arr.len() as i32, shader_src_arr.as_ptr(), &shader_src_len_arr[0] );
        gl::CompileShader( handle );
        let mut result = -1;
        gl::GetShaderiv( handle, gl::COMPILE_STATUS, & mut result );
        if 0 == result {
            let mut log_len = 0;
            gl::GetShaderiv( handle, gl::INFO_LOG_LENGTH, & mut log_len );
            let mut log = vec![ 0i8; log_len as usize ];
            if log_len > 0 {
                let mut written = 0;
                gl::GetShaderInfoLog( handle, log_len, & mut written, log.as_ptr() as * mut i8 );
                let log_u8 = log.iter().map(|&x| x as u8 ).collect::<Vec<u8> >();
                let log_str = str::from_utf8( &log_u8[..] ).unwrap();
                return Err( String::from( log_str ) )
            }else{
                return Err( String::from("unknown error during shader compilation") )
            }
        }
        return Ok( handle )
    }
}

pub fn check_program_link( handle: gl::types::GLuint ) -> Result< (), String > {
    let mut status = -1;
    unsafe {
        gl::GetProgramiv( handle, gl::LINK_STATUS, & mut status );
        if gl::FALSE as i32 == status {
            let mut log_len = -1;
            println!("get link status log length");
            gl::GetProgramiv( handle, gl::INFO_LOG_LENGTH, & mut log_len );
            println!( "log length: {}", log_len );
            check_last_op();
            let mut log = vec![ 0i8; log_len as usize ];
            if log_len > 0 {
                let mut written = 0;
                println!("get link status log");
                gl::GetProgramInfoLog( handle, log_len, & mut written, log.as_ptr() as * mut i8 );
                check_last_op();
                let log_u8 = log.iter().map(|&x| x as u8 ).collect::<Vec<u8> >();
                let log_str = str::from_utf8( &log_u8[..] ).unwrap();
                return Err( String::from( log_str ) )
            } else {
                return Err( String::from("unknown error during program linkage") )
            }
        }else{
            return Ok(())
        }
    }
}

pub fn check_last_op() {
    unsafe {
        let err = gl::GetError();
        match err {
            gl::NO_ERROR => (),
            gl::INVALID_ENUM => panic!("invalid_enum"),
            gl::INVALID_VALUE => panic!("invalid_value"),
            gl::INVALID_OPERATION => panic!("invalid_operation"),
            gl::INVALID_FRAMEBUFFER_OPERATION => panic!("invalid_framebuffer_operation"),
            gl::OUT_OF_MEMORY => panic!("out_of_memory"),
            gl::INVALID_ENUM => panic!("invalid_enum"),
            gl::STACK_OVERFLOW => panic!("stack_overflow"),
            _ => panic!("unknown"),
        }
    }
}