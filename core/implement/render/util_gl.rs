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

pub fn create_program_from_shaders( shader_handles: Vec<gl::types::GLuint> ) -> gl::types::GLuint {
    create_program_from_shaders_with( shader_handles, | program, shader_handles |() ) //insert dummy lambda
}
    
pub fn create_program_from_shaders_with< F >( shader_handles: Vec<gl::types::GLuint>, f: F ) -> gl::types::GLuint  where F: Fn( gl::types::GLuint, & Vec<gl::types::GLuint> )->() {
    unsafe {
        let gl_program = gl::CreateProgram();
        if gl_program == 0 {
            panic!("gl_program creation failed");
        }
        for i in shader_handles.iter() {
            gl::AttachShader( gl_program, *i );
        }
        // additional operation before linking program
        f( gl_program, & shader_handles );
        gl::LinkProgram( gl_program );
        match check_program_link( gl_program ) {
            Err( o ) => panic!( "{}", o ),
            _ => ()
        }
        return gl_program
    }
}

pub fn delete_shader_program( handle: i64 ){
    unsafe {
        gl::DeleteProgram( handle as gl::types::GLuint );
    }
}
pub fn query_uniform_float_array( program: gl::types::GLuint, name: String, num_elem: usize ){
    unsafe {
        let loc = gl::GetUniformLocation( program, name.as_ptr() as * const i8 );
        println!("uniform location: {:?}", loc );
        let mut query_val = vec![0f32;32];
        gl::GetUniformfv( program, gl::GetUniformLocation( program, b"MVP\0".as_ptr() as * const i8 ), query_val.as_mut_ptr() );
        check_last_op();
        println!("query MVP: {:?}", query_val );
    }
}


