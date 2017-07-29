extern crate gl;
extern crate glutin;
extern crate libc;

use implement::render::rendercommon::PipelineArgRequire;
use implement::render::rendercommon::Scene;
use interface::i_renderpass::IRenderPass;

pub struct RenderPassAds {
    _pipeline_req: PipelineArgRequire,
}

impl IRenderPass for RenderPassAds {
    fn process( pipeline_req: PipelineArgRequire, scene: Scene ) -> Result< (), & 'static str > {
        //check required arguments
        
        unsafe {
            gl::Clear( gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT );
            gl::ClearColor( 0.3, 0.3, 0.3, 1.0 );
            gl::Viewport( 0, 0, 500, 500 ); //todo set size according to input
            gl::CullFace( gl::BACK );


            //todo: handle different types of light, multiple lights
            let loc = gl::GetUniformLocation( 0, &("Light.Position".as_bytes()[0] as i8) );
            let light_pos = [1f32;4];
            gl::Uniform4fv( loc, 1, &light_pos[0] );
            let light_a = [1f32;4];
            gl::Uniform4fv( loc, 1, &light_a[0] );
            let light_d = [1f32;4];
            gl::Uniform4fv( loc, 1, &light_d[0] );
            let light_s = [1f32;4];
            gl::Uniform4fv( loc, 1, &light_s[0] );
            
            //camera
        }
        
        unimplemented!();
    }
}

