extern crate gl;

use ::std::mem;
use ::std::ffi::CStr;
use ::std::os::raw::c_char;
use ::std::fmt;
use ::std::vec::Vec;

pub struct CapabilityGL {
    ver: String,
    opengl_ver_major: i32,
    opengl_ver_minor: i32,
    glsl_ver: String,
    vendor: String,
    renderer: String,
    extensions: Vec< String >,
}

impl fmt::Display for CapabilityGL {
    fn fmt( &self, f: & mut fmt::Formatter ) -> fmt::Result {
        write!( f, "version: {}\n", self.ver )?;
        write!( f, "opengl version: {}.{}\n", self.opengl_ver_major, self.opengl_ver_minor )?;
        write!( f, "glsl language version: {}.\n", self.glsl_ver )?;
        write!( f, "vendor: {}.\n", self.vendor )?;
        write!( f, "renderer: {}.\n", self.renderer )?;
        write!( f, "num extensions: {}", self.extensions.len() )?;
        for i in & self.extensions {
            write!( f, "extension: {}.\n", i )?;
        }
        Ok(())
    }
}

pub fn query_gl() -> CapabilityGL {
    unsafe {

        let renderer_ptr = mem::transmute::< * const u8, * const c_char >( gl::GetString( gl::RENDERER ) );
        let renderer = CStr::from_ptr( renderer_ptr ).to_str().expect("renderer retrieval failed");
        
        let vendor_ptr = mem::transmute::< * const u8, * const c_char >( gl::GetString( gl::VENDOR ) );
        let vendor = CStr::from_ptr( vendor_ptr ).to_str().expect("vendor retrieval failed");
        
        let version_ptr = mem::transmute::< * const u8, * const c_char >( gl::GetString( gl::VERSION ) );
        let version = CStr::from_ptr( version_ptr ).to_str().expect("version retrieval failed");
        
        let glsl_version_ptr = mem::transmute::< * const u8, * const c_char >( gl::GetString( gl::SHADING_LANGUAGE_VERSION ) );
        let glsl_version = CStr::from_ptr( glsl_version_ptr ).to_str().expect("glsl_version retrieval failed");
        
        let mut opengl_version_major : gl::types::GLint = 0;
        let mut opengl_version_minor : gl::types::GLint = 0;
        gl::GetIntegerv( gl::MAJOR_VERSION, & mut opengl_version_major );
        gl::GetIntegerv( gl::MINOR_VERSION, & mut opengl_version_minor );

        let mut nun_extensions : gl::types::GLint = 0;
        gl::GetIntegerv( gl::NUM_EXTENSIONS, & mut nun_extensions );
        
        let mut extensions = vec![];
        for i in 0..nun_extensions {
            let ext_ptr = mem::transmute::< * const u8, * const c_char >( gl::GetStringi( gl::EXTENSIONS, i as u32 ) );
            let ext = CStr::from_ptr( ext_ptr ).to_str().expect("extension retrieval failed");
            extensions.push( ext.to_string() );
        }
        
        let capability = CapabilityGL {
            ver: version.to_string(),
            glsl_ver: glsl_version.to_string(),
            opengl_ver_major: opengl_version_major,
            opengl_ver_minor: opengl_version_minor,
            vendor: vendor.to_string(),
            renderer: renderer.to_string(),
            extensions: extensions,
        };

        capability    
    }
}
