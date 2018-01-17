#![feature(use_extern_macros)]

/// example using ads rendering path to render primitives

extern crate gl;
extern crate glutin;
extern crate rand;
extern crate image;
extern crate e2rcore;

use std::fs::File;
use std::path::Path;
use std::io::BufReader;
use std::io::Read;
use std::str;
use rand::Rng;

use self::image::GenericImage;

use self::glutin::GlContext;

use self::e2rcore::interface::i_ele;
use self::e2rcore::interface::i_window::IWindow;
use self::e2rcore::interface::i_renderobj;
use self::e2rcore::interface::i_renderer::IRenderer;

use self::e2rcore::implement::render::util_gl;
use self::e2rcore::implement::math;
use self::e2rcore::implement::render::camera;
use self::e2rcore::implement::render::light;
use self::e2rcore::implement::render::mesh;
use self::e2rcore::implement::render::primitive;
use self::e2rcore::implement::render::texture;
// use self::e2rcore::implement::render::renderpass_default;

use self::e2rcore::implement::window::winglutin::WinGlutin;

use self::e2rcore::implement::kernel::kernel_render::Renderer;
use self::e2rcore::implement::render::render_commands;

pub fn file_open( file_path: & str ) -> Option<String> {
    let path = File::open( file_path ).expect("file path open invalid");
    let mut buf_reader = BufReader::new(path);
    let mut contents = String::new();
    match buf_reader.read_to_string( & mut contents ){
        Err( e ) => { println!("{}", e ); return None },
        _ => (),
    }
    Some(contents)
}

#[allow(unused)]
fn main() {

    let img = image::open( &Path::new( "core/asset/images/texture0.jpg" ) ).unwrap();
    println!( "image dimension: {:?}", img.dimensions() );
    println!( "image type: {:?}", img.color() );

    let texture0 = texture::Texture::from( &img );

    let mut win : WinGlutin = IWindow::init( 500, 500 );

    match win.make_current() {
        Err( e ) => { panic!( e ); },
        _ => (),
    }
    
    let mut kr = Renderer::init().unwrap();
    
    let vs_src = file_open( "core/example/shading/ads.vs" ).expect("vertex shader not retrieved");
    let fs_src = file_open( "core/example/shading/ads.fs" ).expect("fragment shader not retrieved");
    
    let shader_program_external = kr.load_shader( &[ ( vs_src, util_gl::ShaderType::VERTEX ),
                                                ( fs_src, util_gl::ShaderType::FRAGMENT ) ] ).unwrap();
    util_gl::check_last_op();

    let image = vec![200, 200, 150, 0, 0, 255 ];
    let texture_data = Vec::from( texture0 );
    let ( w, h ) = img.dimensions();
    let texture = match kr.load_texture( String::from("texture0"), &texture_data[..], w as _, h as _ ) {
        Ok( t ) => t,
        Err( e ) => panic!( e ),
    };
    util_gl::check_last_op();
    
    //camera
    let fov = 120f32;
    let aspect = 1f32;
    let near = 0.001f32;
    let far = 1000f32;
    let cam_foc_pos = math::mat::Mat3x1 { _val: [0f32, 0f32, 5f32] };
    let cam_up = math::mat::Mat3x1 { _val: [0f32, 1f32, 0f32] };
    let cam_pos = math::mat::Mat3x1 { _val: [5f32, 5f32, 20f32] };
    let cam_id = 0;
    let cam = camera::Cam::init( cam_id, fov, aspect, near, far, cam_pos, cam_foc_pos, cam_up );
    
    //create lights
    let mut lights : Vec< light::LightAdsPoint > = vec![];
    let mut rng = rand::thread_rng();
    for i in 0..50 {
        let pos_x = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 3f32;
        let pos_y = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 - 4f32;
        let pos_z = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 6f32 + 10f32;
        let colour_r = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 1f32;
        let colour_g = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 1f32;
        let colour_b = ( (rng.gen::<u8>() % 100) as f32 / 100f32 ) * 1f32;
        let l = light::LightAdsPoint {
            _id: i as u64,
            _pos: math::mat::Mat3x1 { _val: [ pos_x, pos_y, pos_z ] },
            _ads_val_spec: math::mat::Mat3x1 { _val: [ colour_r, colour_g, colour_b ] },
            _ads_val_diff: math::mat::Mat3x1 { _val: [ colour_r, colour_g, colour_b ] },
            _ads_val_amb: math::mat::Mat3x1 { _val: [ colour_r, colour_g, colour_b ] },
        };
        lights.push( l );
    }


    let ( vao, vbo, draw_group ) = kr.create_draw_group( i_renderobj::RenderObjType::TRI ).unwrap();
    // let ( vao, vbo, draw_group ) = kr.create_draw_group( i_renderobj::RenderObjType::POINT ).unwrap();
    
    //set triangle vert positions and normals
    let mut mesh = mesh::Mesh::init( 0 );
    mesh._pos.extend_from_slice( &[ math::mat::Mat3x1 { _val: [-1f32, -1f32, -1f32 ] },
                                    math::mat::Mat3x1 { _val: [ 5f32, -1f32, -1f32 ] },
                                    math::mat::Mat3x1 { _val: [-1f32,  1f32, -1f32 ] },
                                    math::mat::Mat3x1 { _val: [ 4f32, -1f32, 15f32 ] },
                                    math::mat::Mat3x1 { _val: [ 6f32, -1f32, 15f32 ] },
                                    math::mat::Mat3x1 { _val: [ 4f32,  1f32, 15f32 ] }, ] );

    mesh._normal.extend_from_slice( &[ math::mat::Mat3x1 { _val: [ 0f32, 0f32, 1f32 ] },
                                       math::mat::Mat3x1 { _val: [ 0f32, 0f32, 1f32 ] },
                                       math::mat::Mat3x1 { _val: [ 0f32, 0f32, 1f32 ] },
                                       math::mat::Mat3x1 { _val: [ 0f32, 0f32, 1f32 ] },
                                       math::mat::Mat3x1 { _val: [ 0f32, 0f32, 1f32 ] },
                                       math::mat::Mat3x1 { _val: [ 0f32, 0f32, 1f32 ] }, ] );
    
    mesh._tc.extend_from_slice( &[ math::mat::Mat2x1 { _val: [ 0f32, 0f32 ] },
                                   math::mat::Mat2x1 { _val: [ 0f32, 0f32 ] },
                                   math::mat::Mat2x1 { _val: [ 0f32, 0f32 ] },
                                   math::mat::Mat2x1 { _val: [ 0f32, 0f32 ] },
                                   math::mat::Mat2x1 { _val: [ 0f32, 0f32 ] },
                                   math::mat::Mat2x1 { _val: [ 0f32, 0f32 ] }, ] );

    let mesh_copy = mesh.clone();
    
    util_gl::check_last_op();

    let shader_program = kr._shader_collection.borrow_mut().get( shader_program_external ).unwrap();
        
    let mut running = true;
    let mut delta = 0f32;
    while running {
        let mut new_win_dim = None;
        win.handle_events( |event| {
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
            win._win._wingl.resize(w, h);
        }
        unsafe {
            gl::ClearColor( 0.9, 0.9, 0.9, 1.0 );
            gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
        }
        Renderer::add_obj( & mut kr, "cmd_clear_draw_group", i_ele::Ele::init( render_commands::CmdDrawGroupClear::init( draw_group ) ) ).is_ok();

        // primitives and objects start
        let mut mesh2 = mesh_copy.clone();
        mesh2._pos.clear();
        mesh2._pos.extend_from_slice( &[ math::mat::Mat3x1 { _val: [-1f32+delta, -1f32, -1f32 ] },
                                         math::mat::Mat3x1 { _val: [ 5f32+delta, -1f32, -1f32 ] },
                                         math::mat::Mat3x1 { _val: [-1f32+delta,  1f32, -1f32 ] },
                                         math::mat::Mat3x1 { _val: [ 4f32+delta, -1f32, 15f32 ] },
                                         math::mat::Mat3x1 { _val: [ 6f32+delta, -1f32, 15f32 ] },
                                         math::mat::Mat3x1 { _val: [ 4f32+delta,  1f32, 15f32 ] }, ] );
        Renderer::add_obj( & mut kr, "mesh_triangles", i_ele::Ele::init( mesh2 ) ).is_ok();

        let prim_box = primitive::Poly6 { _pos: math::mat::Mat3x1 { _val: [ -5f32, -10f32, 5f32 ] },
                                           _radius: 5f32 };

        Renderer::add_obj( & mut kr, "box", i_ele::Ele::init( prim_box ) ).is_ok();

        let prim_sphere = primitive::SphereIcosahedron::init( math::mat::Mat3x1 { _val: [ -20f32, -10f32, 0f32 ] }, 5f32 );

        Renderer::add_obj( & mut kr, "sphere", i_ele::Ele::init( prim_sphere ) ).is_ok();

        //todo: add support for point rendering
        // let mut prim_point = primitive::Point { _pos: math::mat::Mat3x1 { _val: [ 0f32, 0f32, 5f32 ] },
        //                                         _radius: 3f32 };
        
        // Renderer::add_obj( & mut kr, "point", i_ele::Ele::init( prim_point ) ).is_ok();
        
        let l = &lights[0];
        Renderer::add_obj( & mut kr, "light_ads", i_ele::Ele::init( l.clone() ) ).is_ok();
        
        Renderer::add_obj( & mut kr, "camera", i_ele::Ele::init( cam.clone() ) ).is_ok();
        //primitives and objects end

        Renderer::add_obj( & mut kr, "cmd_bind_draw_group", i_ele::Ele::init( render_commands::CmdDrawGroupBind::init( draw_group ) ) ).is_ok();

        Renderer::add_obj( & mut kr, "cmd_set_draw_group_dependent_uniforms", i_ele::Ele::init( render_commands::CmdDrawGroupDependentUniforms::init( draw_group, &[0u64,1u64] ) ) ).is_ok();
        
        Renderer::add_obj( & mut kr, "cmd_dispatch_draw_group", i_ele::Ele::init( render_commands::CmdDrawGroupDispatch::init( draw_group ) ) ).is_ok();

        delta -= 0.01f32;

        win.swap_buf();

        println!("swapped buffer");
    }
}
