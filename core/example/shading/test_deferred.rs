extern crate gl;
extern crate glutin;
extern crate rand;

extern crate e2rcore;

use std::fs::File;
use std::io::BufReader;
use std::str::FromStr;
use std::io::Read;
use std::str;
use rand::Rng;

use self::glutin::GlContext;

use self::e2rcore::interface::i_window::IWindow;
use self::e2rcore::implement::window::winglutin::WinGlutin;

use self::e2rcore::implement::capability::capability_gl;
use self::e2rcore::implement::render::util_gl;

pub fn file_open( file_path: & str ) -> Option<String> {
    let path = File::open( file_path ).expect("file path open invalid");
    let mut buf_reader = BufReader::new(path);
    let mut contents = String::new();
    match buf_reader.read_to_string( & mut contents ) {
        Err( e ) => { println!("{}", e ); return None },
        _ => (),
    }
    Some(contents)
}

#[allow(unused)]
fn main() {
    
    let geom_vs_src = file_open( "core/example/shading/deferred_geometry_pass.vs" ).expect("geometry vertex shader not retrieved");
    let geom_fs_src = file_open( "core/example/shading/deferred_geometry_pass.fs" ).expect("geometry fragment shader not retrieved");
    let light_vs_src = file_open( "core/example/shading/deferred_light_pass.vs" ).expect("light vertex shader not retrieved");
    let light_fs_src = file_open( "core/example/shading/deferred_light_pass.fs" ).expect("light fragment shader not retrieved");

    
    let mut window : WinGlutin = IWindow::init( 500, 500 );
    window.make_current().expect("window make_current failed");

    let cap = capability_gl::query_gl();
    println!( "{}", cap );

    unsafe {
        gl::Enable( gl::DEPTH_TEST );
    }

    let geom_vs = match util_gl::load_and_compile_shader( geom_vs_src.as_str(), util_gl::ShaderType::VERTEX ) { Ok( o ) => o, Err( o ) => { panic!( "{}", o ); } };
    let geom_fs = match util_gl::load_and_compile_shader( geom_fs_src.as_str(), util_gl::ShaderType::FRAGMENT ) { Ok( o ) => o, Err( o ) => { panic!( "{}", o ); } };

    let light_vs = match util_gl::load_and_compile_shader( light_vs_src.as_str(), util_gl::ShaderType::VERTEX ) { Ok( o ) => o, Err( o ) => { panic!( "{}", o ); } };
    let light_fs = match util_gl::load_and_compile_shader( light_fs_src.as_str(), util_gl::ShaderType::FRAGMENT ) { Ok( o ) => o, Err( o ) => { panic!( "{}", o ); } };

    //camera
    let cam_pos = [ 0f32, 0f32, 5f32 ];

    //rendering quad
    let mut quad_vao : u32 = 0;
    let mut quad_vbo : u32 = 0;
    
    let mut gl_program_geom = 0;
    let mut gl_program_light = 0;
    //render buffers
    let mut g_buffer = 0;
    let mut g_position = 0;
    let mut g_normal = 0;
    let mut g_albedo_spec = 0;
    unsafe {
        gl_program_geom = util_gl::create_program_from_shaders( &[ geom_vs, geom_fs ] );
        gl_program_light = util_gl::create_program_from_shaders( &[ light_vs, light_fs ] );

        gl::UseProgram( gl_program_geom );
        util_gl::check_last_op();
        gl::BindFramebuffer( gl::FRAMEBUFFER, 0 );
        
        //todo: deferred shading
        gl::GenFramebuffers(1, & mut g_buffer);
        gl::BindFramebuffer(gl::FRAMEBUFFER, g_buffer);
        // unsigned int g_position, g_normal, gColorSpec;

        // - position color buffer
        gl::GenTextures( 1, & mut g_position );
        gl::BindTexture( gl::TEXTURE_2D, g_position );
        gl::TexImage2D( gl::TEXTURE_2D, 0, gl::RGB16F as i32, 500, 500, 0, gl::RGB, gl::FLOAT, std::ptr::null() );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST as i32 );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST as i32 );
        gl::FramebufferTexture2D( gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, g_position, 0 );

        // - normal color buffer
        gl::GenTextures( 1, & mut g_normal );
        gl::BindTexture( gl::TEXTURE_2D, g_normal );
        gl::TexImage2D( gl::TEXTURE_2D, 0, gl::RGB16F as i32, 500, 500, 0, gl::RGB, gl::FLOAT, std::ptr::null() );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST as i32 );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST as i32 );
        gl::FramebufferTexture2D( gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT1, gl::TEXTURE_2D, g_normal, 0);

        // - color + specular color buffer
        gl::GenTextures( 1, & mut g_albedo_spec );
        gl::BindTexture( gl::TEXTURE_2D, g_albedo_spec );
        gl::TexImage2D( gl::TEXTURE_2D, 0, gl::RGBA as i32, 500, 500, 0, gl::RGBA, gl::UNSIGNED_BYTE, std::ptr::null() );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST as i32 );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST as i32 );
        gl::FramebufferTexture2D( gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT2, gl::TEXTURE_2D, g_albedo_spec, 0 );

        // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        let attachments : [u32;3] = [ gl::COLOR_ATTACHMENT0, gl::COLOR_ATTACHMENT1, gl::COLOR_ATTACHMENT2 ];
        gl::DrawBuffers( 3, &attachments[0] );

        //create depth buffer
        let mut rbo_depth_stencil = 0;
        gl::GenRenderbuffers( 1, & mut rbo_depth_stencil );
        gl::BindRenderbuffer( gl::RENDERBUFFER, rbo_depth_stencil );
        gl::RenderbufferStorage( gl::RENDERBUFFER, gl::DEPTH_COMPONENT32F, 500, 500 );
        gl::FramebufferRenderbuffer( gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, rbo_depth_stencil );
        if gl::FRAMEBUFFER_COMPLETE != gl::CheckFramebufferStatus( gl::FRAMEBUFFER ) {
            panic!("frambuffer incomplete");
        }
    }

    //create lights
    let mut light_position_colour : Vec<[f32;6]> = vec![];
    let mut rng = rand::thread_rng();
    for i in 0..50 {
        let pos_x = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 3f32;
        let pos_y = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 4f32;
        let pos_z = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 3f32;
        let colour_r = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 3f32;
        let colour_g = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 4f32;
        let colour_b = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 3f32;
        light_position_colour.push( [ pos_x, pos_y, pos_z, colour_r, colour_g, colour_b ] );
    }
    //configure shader parameters for lights
    unsafe {
        gl::UseProgram( gl_program_light );
        gl::Uniform1i( gl::GetUniformLocation( gl_program_light, "g_position".as_ptr() as * const i8 ), 0 );
        gl::Uniform1i( gl::GetUniformLocation( gl_program_light, "g_normal".as_ptr() as * const i8 ), 1 );
        gl::Uniform1i( gl::GetUniformLocation( gl_program_light, "g_albedo_spec".as_ptr() as * const i8 ), 2 );
    }

    
    //todo: create renderable objects; put this in a reusable module
    //create or load model object with data for vert pos, vert normal, vert tex coord, vert tangent, vert bitangent
    unsafe {
        //create vertex array object and vertex  buffer objects to store model data
        let mut obj_vao = 0;
        let mut obj_vbo = 0;
        gl::GenVertexArrays( 1, & mut obj_vao );
        gl::GenBuffers( 1, & mut obj_vbo );
        //gl::GenBuffers( 1, &obj_ebo ); //optional
        gl::BindVertexArray( obj_vao );
        gl::BindBuffer( gl::ARRAY_BUFFER, obj_vbo );

        let data_ptr = [ -1f32, -1f32, -1f32, 1f32, -1f32, -1f32, -1f32, 1f32, -1f32,
                          1f32,  1f32, -1f32, -1f32, 1f32, -1f32, 1f32, -1f32, -1f32 ];
        
        let data_length =  data_ptr.len() * std::mem::size_of::<f32>();
                          
        gl::BufferData( gl::ARRAY_BUFFER, data_length as isize, data_ptr.as_ptr() as _, gl::STATIC_DRAW );
        //EBO stuff //optional
        gl::EnableVertexAttribArray( 0 );
        
        // gl::VertexAttribPointer( 0, 3, gl::FLOAT, gl::FALSE, stride, offset_position ); //position
        // gl::EnableVertexAttribArray( 1 );
        // gl::VertexAttribPointer( 1, 3, gl::FLOAT, gl::FALSE, stride, offset_normal ); //normal
        // gl::EnableVertexAttribArray( 2 );
        //gl::VertexAttribPointer( 2, 2, gl::FLOAT, gl::FALSE, stride, offset_tc ); //texture coordinates
        //gl::EnableVertexAttribArray( 3 );
        //gl::VertexAttribPointer( 3, 3, gl::FLOAT, gl::FALSE, stride, offset_tangent ); //tangent
        //gl::EnableVertexAttribArray( 4 );
        //gl::VertexAttribPointer( 4, 3, gl::FLOAT, gl::FALSE, stride, offset_bitangent ); //bitangent
        gl::BindVertexArray( 0 );
    }

    //create multiple model instances with positional offset
    let mut obj_pos : Vec<[f32;3]> = vec![];
    obj_pos.push( [-3f32, -3f32, -3f32] );
    obj_pos.push( [ 0f32, -3f32, -3f32] );
    obj_pos.push( [ 3f32, -3f32, -3f32] );
    obj_pos.push( [-3f32, -3f32,  0f32] );
    obj_pos.push( [ 0f32, -3f32,  0f32] );
    obj_pos.push( [ 3f32, -3f32,  0f32] );
    obj_pos.push( [-3f32, -3f32,  3f32] );
    obj_pos.push( [ 0f32, -3f32,  3f32] );
    obj_pos.push( [ 3f32, -3f32,  3f32] );
    
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
            gl::ClearColor( 0.3, 0.3, 0.3, 1.0 );
            gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

            // 1. geometry pass
            gl::BindFramebuffer( gl::FRAMEBUFFER, g_buffer );
            {
                gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
                //todo: calculate projection matrix
                //todo: calculate view matrix
                //todo: calculate model matrix
                
                gl::UseProgram( gl_program_geom );
                //todo: set uniforms
                for ( i, v ) in obj_pos.iter().enumerate() {
                    //model = Identity4x4;
                    //model = model.translate( v );
                    //model = model.scale( Vec3( ... ) );
                    //gl::UniformMatrix4fv( gl::GetUniformLocation( gl_program_geom, "model", 1, gl::FALSE, &model[0] );
                    //draw object mesh
                    //gl::BindVertexArray(VAO);
                    //gl::DrawElements or gl::DrawArrays on model data
                    //gl::BindVertexArray(0);
                    //gl::ActiveTexture( gl::TEXTURE0 );
                }
            }
            gl::BindFramebuffer( gl::FRAMEBUFFER, 0 );

            //2. lighting pass
            {
                gl::Clear( gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT );

                gl::UseProgram( gl_program_light );
                
                // Bind AllbGBuffer Textures
                gl::ActiveTexture( gl::TEXTURE0 );
                gl::BindTexture( gl::TEXTURE_2D, g_position );
                gl::ActiveTexture( gl::TEXTURE1 );
                gl::BindTexture( gl::TEXTURE_2D, g_normal );
                gl::ActiveTexture( gl::TEXTURE2 );
                gl::BindTexture( gl::TEXTURE_2D, g_albedo_spec );

                //set light uniforms
                for ( i, v ) in light_position_colour.iter().enumerate() {
                    let mut uniform_light_pos = "lights[".to_owned();
                    uniform_light_pos.push_str( i.to_string().as_str() );
                    uniform_light_pos.push_str( String::from_str("].Position").unwrap().as_str() );
                    gl::Uniform3fv( gl::GetUniformLocation( gl_program_light, uniform_light_pos.as_ptr() as * const i8 ), 1, &v[0] );

                    let mut uniform_name_colour = "lights[".to_owned();
                    uniform_name_colour.push_str( i.to_string().as_str() );
                    uniform_name_colour.push_str( String::from_str("].Colour").unwrap().as_str() );
                    gl::Uniform3fv( gl::GetUniformLocation( gl_program_light, uniform_name_colour.as_ptr() as * const i8 ), 1, &v[0] );

                    let light_atten_const = 1.0;
                    let light_atten_linear = 0.7;
                    let light_atten_quadratic = 1.8;

                    let mut uniform_name_linear = "lights[".to_owned();
                    uniform_name_linear.push_str( i.to_string().as_str() );
                    uniform_name_linear.push_str( String::from_str("].Linear").unwrap().as_str() );
                    gl::Uniform3fv( gl::GetUniformLocation( gl_program_light, uniform_name_linear.as_ptr() as * const i8 ), 1, &v[0] );

                    let mut uniform_name_quadratic = "lights[".to_owned();
                    uniform_name_quadratic.push_str( i.to_string().as_str() );
                    uniform_name_quadratic.push_str( String::from_str("].Quadratic").unwrap().as_str() );
                    gl::Uniform3fv( gl::GetUniformLocation( gl_program_light, uniform_name_quadratic.as_ptr() as * const i8 ), 1, &v[0] );
                }
                gl::Uniform3fv( gl::GetUniformLocation( gl_program_light, "viewPos".as_ptr() as * const i8 ), 1, &cam_pos[0] );
            }
            //render quad
            {
                if quad_vao == 0 {
                    let quad_vertices = [ -1f32, 1f32, 0f32, 0f32, 1f32,
                                         -1f32, -1f32, 0f32, 0f32, 0f32,
                                          1f32, 1f32, 0f32, 1f32, 1f32,
                                          1f32, -1f32, 0f32, 1f32, 0f32, ];
                    gl::GenVertexArrays( 1, & mut quad_vao );
                    gl::GenBuffers( 1, & mut quad_vbo );
                    gl::BindVertexArray( quad_vao );
                    gl::BindBuffer( gl::ARRAY_BUFFER, quad_vbo );
                    gl::BufferData( gl::ARRAY_BUFFER, ( quad_vertices.len() * std::mem::size_of::<f32>() ) as isize, quad_vertices[..].as_ptr().offset(0) as _, gl::STATIC_DRAW );
                    gl::EnableVertexAttribArray( 0 );
                    gl::VertexAttribPointer( 0, 3, gl::FLOAT, gl::FALSE, ( 5 * std::mem::size_of::<f32>() ) as i32, std::ptr::null() );
            gl::EnableVertexAttribArray( 1 );
                    gl::VertexAttribPointer( 1, 2, gl::FLOAT, gl::FALSE, ( 5 * std::mem::size_of::<f32>() ) as i32, ( 3 * std::mem::size_of::<f32>() ) as * const std::os::raw::c_void );
                }
                gl::BindVertexArray( quad_vao );
                gl::DrawArrays( gl::TRIANGLE_STRIP, 0, 4 );
                gl::BindVertexArray( 0 );
            }

            let buf_width = 500;
            let buf_height = 500;
            //copy content of geometry depth buffer to default framebuffer's depth buffer
            gl::BindFramebuffer( gl::READ_FRAMEBUFFER, g_buffer );
            gl::BindFramebuffer( gl::DRAW_FRAMEBUFFER, 0 );
            //blit from g_buffer to default framebuffer
            gl::BlitFramebuffer( 0, 0, buf_width, buf_height, 0, 0, buf_width, buf_height, gl::DEPTH_BUFFER_BIT, gl::NEAREST );
            gl::BindFramebuffer( gl::FRAMEBUFFER, 0 );
            
            //3. render emissive light objects in the scene
            //todo
        }
        window.swap_buf();
    }
}
