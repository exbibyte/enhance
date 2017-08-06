#![feature(use_extern_macros)]

extern crate gl;
extern crate glutin;
extern crate libc;
extern crate rand;

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
use rand::Rng;

use self::glutin::GlContext;

use self::e2rcore::interface::i_window::IWindow;
use self::e2rcore::implement::window::winglutin::WinGlutin;
use self::e2rcore::implement::capability::capability_gl;
use self::e2rcore::implement::render::util_gl;
use self::e2rcore::implement::math;

use self::e2rcore::implement::commonutil::util_str;

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
    // let vs_src = file_open( "core/example/shading/simple.vs" ).expect("vertex shader not retrieved");
    // let fs_src = file_open( "core/example/shading/simple.fs" ).expect("fragment shader not retrieved");

    let mut window : WinGlutin = IWindow::init( 500, 500 );
    window.make_current().expect("window make_current failed");

    let cap = capability_gl::query_gl();
    println!( "{}", cap );

    // unsafe {
    //     gl::Enable( gl::DEPTH_TEST );
    // }

    let vs = match util_gl::load_and_compile_shader( vs_src.as_str(), util_gl::ShaderType::VERTEX ) { Ok( o ) => o, Err( o ) => { panic!( "{}", o ); } };
    let fs = match util_gl::load_and_compile_shader( fs_src.as_str(), util_gl::ShaderType::FRAGMENT ) { Ok( o ) => o, Err( o ) => { panic!( "{}", o ); } };

    //camera
    let mut cam_pos : math::mat::Mat3x1<f32> = Default::default();
    cam_pos[0] = 5f32;
    cam_pos[1] = 5f32;
    cam_pos[2] = 20f32;
    
    let mut gl_program = 0;
    //render buffers
    unsafe {
        gl_program = util_gl::create_program_from_shaders( vec![ vs, fs ] );
        gl::UseProgram( gl_program );
        util_gl::check_last_op();
        gl::BindFramebuffer( gl::FRAMEBUFFER, 0 );
    }

    // //create lights
    // let mut light_position_colour : Vec<[f32;6]> = vec![];
    // let mut rng = rand::thread_rng();
    // for i in 0..50 {
    //     let pos_x = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 3f32;
    //     let pos_y = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 4f32;
    //     let pos_z = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 3f32;
    //     let colour_r = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 3f32;
    //     let colour_g = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 4f32;
    //     let colour_b = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 3f32;
    //     light_position_colour.push( [ pos_x, pos_y, pos_z, colour_r, colour_g, colour_b ] );
    // }
    
    //todo: create renderable objects; put this in a reusable module
    //create or load model object with data for vert pos, vert normal, vert tex coord, vert tangent, vert bitangent
    let mut obj_vao = 0;
    let mut obj_vbo = 0;
    unsafe {
        //create vertex array object and vertex  buffer objects to store model data

        // gl::FrontFace( gl::CW );
        
        gl::GenVertexArrays( 1, & mut obj_vao );
        gl::BindVertexArray( obj_vao );
        gl::GenBuffers( 1, & mut obj_vbo );
        gl::BindBuffer( gl::ARRAY_BUFFER, obj_vbo );

        //triangle vert positions and normals
        let data_ptr = [ -1f32, -1f32, -1f32, 0f32, 0f32, 1f32,
                          5f32, -1f32, -1f32, 0f32, 0f32, 1f32,
                         -1f32,  1f32, -1f32, 0f32, 0f32, 1f32,
                          4f32, -1f32, 15f32, 0f32, 0f32, 1f32,
                          6f32, -1f32,  15f32, 0f32, 0f32, 1f32,
                          4f32, 1f32,  15f32, 0f32, 0f32, 1f32 ];
        
        let data_length = data_ptr.len() * std::mem::size_of::<f32>();
                          
        gl::BufferData( gl::ARRAY_BUFFER, data_length as isize, data_ptr.as_ptr() as _, gl::STATIC_DRAW );
        util_gl::check_last_op();
        gl::VertexAttribPointer( 0, 3, gl::FLOAT, gl::FALSE, (6*std::mem::size_of::<f32>()) as i32, 0 as _ ); //position
        gl::EnableVertexAttribArray( 0 );
        util_gl::check_last_op();
        gl::VertexAttribPointer( 1, 3, gl::FLOAT, gl::FALSE, (6*std::mem::size_of::<f32>()) as i32, (3*std::mem::size_of::<f32>()) as _ ); //normal
        util_gl::check_last_op();
        gl::EnableVertexAttribArray( 1 );
        util_gl::check_last_op();
        gl::BindVertexArray( 0 );
    }

    //configure shader parameters for lights
    unsafe {
        gl::UseProgram( gl_program );
        util_gl::check_last_op();
        gl::Uniform3fv( gl::GetUniformLocation( gl_program, b"Material.Ka\0".as_ptr() as * const i8 ), 1, [0.2f32,0.2f32,0.2f32].as_ptr() as _ );
        gl::Uniform3fv( gl::GetUniformLocation( gl_program, b"Material.Kd\0".as_ptr() as * const i8 ), 1, [0.1f32,0.1f32,0.9f32].as_ptr() as _ );
        gl::Uniform3fv( gl::GetUniformLocation( gl_program, b"Material.Ks\0".as_ptr() as * const i8 ), 1, [0.95f32,0.1f32,0.1f32].as_ptr() as _ );
        gl::Uniform1f( gl::GetUniformLocation( gl_program, b"Material.Shininess\0".as_ptr() as * const i8 ), 3f32 );

        gl::Uniform3fv( gl::GetUniformLocation( gl_program, b"Light.Position\0".as_ptr() as * const i8 ), 1, [-5f32,-5f32,20.0f32].as_ptr() as _ );
        gl::Uniform3fv( gl::GetUniformLocation( gl_program, b"Light.La\0".as_ptr() as * const i8 ), 1, [0.0f32,0.0f32,0.0f32].as_ptr() as _ );
        gl::Uniform3fv( gl::GetUniformLocation( gl_program, b"Light.Ld\0".as_ptr() as * const i8 ), 1, [0.8f32,0.8f32,0.8f32].as_ptr() as _ );
        gl::Uniform3fv( gl::GetUniformLocation( gl_program, b"Light.Ls\0".as_ptr() as * const i8 ), 1, [0.8f32,0.8f32,0.8f32].as_ptr() as _ );

        util_gl::check_last_op();
    }

    let fov = 120f32;
    let aspect = 1f32;
    let near = 0.001f32;
    let far = 1000f32;
    let persp_transform = math::util::perspective( fov, aspect, near, far );

    let mut cam_foc_pos : math::mat::Mat3x1<f32> = Default::default();
    cam_foc_pos[0] = 0f32;
    cam_foc_pos[1] = 0f32;
    cam_foc_pos[2] = 5f32;

    let mut cam_up : math::mat::Mat3x1<f32> = Default::default();
    cam_up[0] = 0f32;
    cam_up[1] = 1f32;
    cam_up[2] = 0f32;
    
    let cam_view = math::util::look_at( cam_pos, cam_foc_pos, cam_up );
    
    let model_transform = math::mat::Mat4::<f32> { _val: [ 1f32, 0f32, 0f32, 0f32,
                                                           0f32, 1f32, 0f32, 0f32,
                                                           0f32, 0f32, 1f32, 0f32,
                                                           0f32, 0f32, 0f32, 1f32 ],
                                                    _is_row_major: true };


    let mvp_transform = persp_transform.mul( &cam_view ).unwrap().mul( &model_transform ).unwrap();
    let model_view_transform = cam_view.mul( &model_transform ).unwrap();
    let normal_inv_transpose = model_view_transform.submat_mat3().inverse().unwrap().transpose();

    println!( "mv: {:?}", model_view_transform );
    println!( "mv submat: {:?}", model_view_transform.submat_mat3() );
    println!( "mvp: {:?}", mvp_transform );
    println!( "normal matrix: {:?}", normal_inv_transpose );
    
    unsafe {
        gl::UniformMatrix4fv( gl::GetUniformLocation( gl_program, b"ModelViewMatrix\0".as_ptr() as * const i8 ), 1, gl::TRUE, model_view_transform._val.as_ptr() );
        util_gl::check_last_op();
        gl::UniformMatrix3fv( gl::GetUniformLocation( gl_program, b"NormalMatrix\0".as_ptr() as * const i8 ), 1, gl::TRUE, normal_inv_transpose._val.as_ptr() );
        util_gl::check_last_op();
        gl::UniformMatrix4fv( gl::GetUniformLocation( gl_program, b"ProjectionMatrix\0".as_ptr() as * const i8 ), 1, gl::TRUE, persp_transform._val.as_ptr() );
        util_gl::check_last_op();
        gl::UniformMatrix4fv( gl::GetUniformLocation( gl_program, b"MVP\0".as_ptr() as * const i8 ), 1, gl::TRUE, mvp_transform._val.as_ptr() );
        util_gl::check_last_op();
        // let loc = gl::GetUniformLocation( gl_program, b"MVP\0".as_ptr() as * const i8 );
        // println!("uniform location: {:?}", loc );
        // let mut query_val = [0f32;32];
        // gl::GetUniformfv( gl_program, gl::GetUniformLocation( gl_program, b"MVP\0".as_ptr() as * const i8 ), query_val.as_mut_ptr() );
        // util_gl::check_last_op();
        // println!("query MVP: {:?}", query_val );
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
            gl::ClearColor( 0.1, 0.1, 0.1, 1.0 );
            gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
            {
                gl::BindVertexArray( obj_vao );
                gl::DrawArrays(gl::TRIANGLES, 0, 6 );
            }
        }
        window.swap_buf();
    }
}
