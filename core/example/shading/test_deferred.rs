extern crate gl;
extern crate glutin;
extern crate libc;

extern crate e2rcore;

use std::mem;
use std::fs::File;
use std::io::BufReader;
use std::str::FromStr;
use std::io::Read;
use std::ffi::CStr;
use std::os::raw::c_char;
use std::fmt;
use std::str;

use self::glutin::GlContext;

use self::e2rcore::interface::i_window::IWindow;
use self::e2rcore::implement::window::winglutin::WinGlutin;

use self::e2rcore::implement::capability::capability_gl;
use self::e2rcore::implement::render::util_gl;

pub fn file_open( file_path: & str ) -> Option<String> {
    let path = File::open( file_path ).expect("file path open invalid");
    let mut buf_reader = BufReader::new(path);
    let mut contents = String::new();
    buf_reader.read_to_string( & mut contents );
    Some(contents)
}

fn main() {
    
    let vs_src = file_open( "core/example/shading/ads.vs" ).expect("vertex shader not retrieved");
    let fs_src = file_open( "core/example/shading/ads.fs" ).expect("fragment shader not retrieved");
    
    let mut window : WinGlutin = IWindow::init( 500, 500 );
    window.make_current().expect("window make_current failed");

    let cap = capability_gl::query_gl();
    println!( "{}", cap );

    let vs = match util_gl::load_and_compile_shader( vs_src.as_str(), util_gl::ShaderType::VERTEX ) { Ok( o ) => o, Err( o ) => { panic!( "{}", o ); } };
    let fs = match util_gl::load_and_compile_shader( fs_src.as_str(), util_gl::ShaderType::FRAGMENT ) { Ok( o ) => o, Err( o ) => { panic!( "{}", o ); } };

    let mut gl_program = 0;
    //render buffers
    let mut gBuffer = 0;
    let mut gPosition = 0;
    let mut gNormal = 0;
    let mut gAlbedoSpec = 0;
    unsafe {
        gl_program = gl::CreateProgram();
        if gl_program == 0 {
            panic!("gl program creation failed");
        }
        gl::AttachShader( gl_program, vs );
        gl::AttachShader( gl_program, fs );
        gl::LinkProgram( gl_program );
        match util_gl::check_program_link( gl_program ) {
            Err( o ) => panic!( "{}", o ),
            _ => ()
        }
        gl::UseProgram( gl_program );
        util_gl::check_last_op();
        gl::BindFramebuffer( gl::FRAMEBUFFER, 0 );

        
        //todo: deferred shading
        gl::GenFramebuffers(1, & mut gBuffer);
        gl::BindFramebuffer(gl::FRAMEBUFFER, gBuffer);
        // unsigned int gPosition, gNormal, gColorSpec;

        // - position color buffer
        gl::GenTextures( 1, & mut gPosition );
        gl::BindTexture( gl::TEXTURE_2D, gPosition );
        gl::TexImage2D( gl::TEXTURE_2D, 0, gl::RGB16F as i32, 500, 500, 0, gl::RGB, gl::FLOAT, std::ptr::null() );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST as i32 );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST as i32 );
        gl::FramebufferTexture2D( gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, gPosition, 0 );

        // - normal color buffer
        gl::GenTextures( 1, & mut gNormal );
        gl::BindTexture( gl::TEXTURE_2D, gNormal );
        gl::TexImage2D( gl::TEXTURE_2D, 0, gl::RGB16F as i32, 500, 500, 0, gl::RGB, gl::FLOAT, std::ptr::null() );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST as i32 );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST as i32 );
        gl::FramebufferTexture2D( gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT1, gl::TEXTURE_2D, gNormal, 0);


        // - color + specular color buffer
        gl::GenTextures( 1, & mut gAlbedoSpec );
        gl::BindTexture( gl::TEXTURE_2D, gAlbedoSpec );
        gl::TexImage2D( gl::TEXTURE_2D, 0, gl::RGBA as i32, 500, 500, 0, gl::RGBA, gl::UNSIGNED_BYTE, std::ptr::null() );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST as i32 );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST as i32 );
        gl::FramebufferTexture2D( gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT2, gl::TEXTURE_2D, gAlbedoSpec, 0 );

        // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        let attachments : [u32;3] = [ gl::COLOR_ATTACHMENT0, gl::COLOR_ATTACHMENT1, gl::COLOR_ATTACHMENT2 ];
        gl::DrawBuffers( 3, &attachments[0] );

        let mut rboDepthStencil = 0;
        gl::GenRenderbuffers( 1, & mut rboDepthStencil );
        gl::BindRenderbuffer( gl::RENDERBUFFER, rboDepthStencil );
        gl::RenderbufferStorage( gl::RENDERBUFFER, gl::DEPTH_COMPONENT32F, 500, 500 );

        gl::FramebufferRenderbuffer( gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, rboDepthStencil );
    }

    
    
    let mut running = true;
    while running {
        let mut new_win_dim = None;
        window.handle_events( |event| {
            match event {
                glutin::Event::WindowEvent{ event, .. } => match event {
                    glutin::WindowEvent::Closed => running = false,
                    glutin::WindowEvent::Resized(w, h) => new_win_dim = Some( (w,h) ),
                    glutin::WindowEvent::KeyboardInput {
                        input: glutin::KeyboardInput {
                            state: glutin::ElementState::Pressed,
                            virtual_keycode: Some( glutin::VirtualKeyCode::Q ),
                            ..
                        }, ..
                    } => running = false,
                    _ => (),
                },
                _ => ()
            }
        } );
        if let Some( ( w, h ) ) = new_win_dim {
            window._win._wingl.resize(w, h);
        }
        unsafe {
            // gl::ClearColor( 0.3, 0.3, 0.3, 1.0 );
            // gl::Clear(gl::COLOR_BUFFER_BIT);

            // 1. geometry pass: render all geometric/color data to g-buffer
            gl::BindFramebuffer(gl::FRAMEBUFFER, gBuffer);
            gl::Enable( gl::DEPTH_TEST );
            gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
            //todo: bind vertext array object
            
            //gBufferShader.use();
            // for(Object obj : Objects)
            // {
            //     ConfigureShaderTransformsAndUniforms();
            //     obj.Draw();
            // }
            // 2. lighting pass: use g-buffer to calculate the scene's lighting
            gl::BindFramebuffer( gl::FRAMEBUFFER, 0 );
            //gl::BindVertexArray( vao_quad );
            gl::Disable( gl::DEPTH_TEST );
            gl::Clear( gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT );
            // Bind AllbGBuffer Textures
            gl::ActiveTexture( gl::TEXTURE0 );
            gl::BindTexture( gl::TEXTURE_2D, gPosition );
            gl::ActiveTexture( gl::TEXTURE1 );
            gl::BindTexture( gl::TEXTURE_2D, gNormal );
            gl::ActiveTexture( gl::TEXTURE2 );
            gl::BindTexture( gl::TEXTURE_2D, gAlbedoSpec );
            // Set Lighting Uniforms
            //shaderLightingPass.use();
            //SendAllLightUniformsToShader( shaderLightingPass );
            //shaderLightingPass.setVec3( "viewPos", camera.Position );
            //Render texture Quad
            //gl::DrawArrays( gl::TRIANGLES, 0, 6 );
        }
        window.swap_buf();
    }
}
