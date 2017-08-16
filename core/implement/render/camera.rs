use std::collections::HashMap;
use std::vec::Vec;
use std::any::Any;

use interface::i_renderobj;
use interface::i_ele;
use interface::i_component;

use implement::render::rendercommon;
use implement::render::camera;
use implement::math;
use implement::math::mat::Mat4;
use implement::math::mat::Mat3x1;

pub struct Cam {
    /// # helper id for the camera
    pub _id: u64,

    pub _proj_xform: math::mat::Mat4< f32 >,
    /// # The following generates the projection matrix
    pub _fov: f32,
    pub _aspect: f32,
    pub _near: f32,
    pub _far: f32,

    pub _view_xform: math::mat::Mat4< f32 >,
    /// # The following generates the view matrix
    pub _pos: math::mat::Mat3x1< f32 >,
    pub _up: math::mat::Mat3x1< f32 >,
    pub _focus: math::mat::Mat3x1< f32 >,
}

impl Cam {
    pub fn init( id: u64, fov: f32, aspect: f32, near: f32, far: f32, pos: math::mat::Mat3x1< f32 >, focus: math::mat::Mat3x1< f32 >, up: math::mat::Mat3x1< f32 > ) -> Cam {
        Cam {
            _id: id,
            _fov: fov,
            _aspect: aspect,
            _near: near,
            _far: far,
            _pos: pos,
            _up: up,
            _focus: focus,
            _proj_xform: math::util::perspective( fov, aspect, near, far ),
            _view_xform: math::util::look_at( pos, focus, up ),
        }
    }    
}

impl i_ele::IObjImpl for Cam {
    fn as_any( & self ) -> & Any {
        self
    }
    fn update_components( & mut self, components: & mut Vec< Box< i_component::IComponent > > ) -> Result< (), & 'static str > {

        //store uniform data
        {
            let model_transform = math::mat::Mat4::<f32> { _val: [ 1f32, 0f32, 0f32, 0f32,
                                                                   0f32, 1f32, 0f32, 0f32,
                                                                   0f32, 0f32, 1f32, 0f32,
                                                                   0f32, 0f32, 0f32, 1f32 ],
                                                            _is_row_major: true };

            let mvp_transform = self._proj_xform.mul( &self._view_xform ).unwrap().mul( &model_transform ).unwrap();
            let model_view_transform = self._view_xform.mul( &model_transform ).unwrap();
            let normal_inv_transpose = model_view_transform.submat_mat3().inverse().unwrap().transpose();

            // println!( "mv: {:?}", model_view_transform );
            // println!( "mv submat: {:?}", model_view_transform.submat_mat3() );
            // println!( "mvp: {:?}", mvp_transform );
            // println!( "normal matrix: {:?}", normal_inv_transpose );

            let data_map_mat4f : HashMap< String, Vec<f32> > =  [ ( String::from("ModelViewMatrix\0"), &model_view_transform._val[..] ),
                                                                  ( String::from("ProjectionMatrix\0"), &self._proj_xform._val[..] ),
                                                                  ( String::from("MVP\0"), &mvp_transform._val[..] ),
            ].into_iter().map(|&( ref k, ref v)| ( k.clone(), v[..].to_vec() ) ).collect();
            
            let data_map_mat3f : HashMap< String, Vec<f32> > =  [ ( String::from("NormalMatrix\0"), &normal_inv_transpose._val[..] ),
            ].into_iter().map(|&( ref k, ref v)| ( k.clone(), v[..].to_vec() ) ).collect();                                                              

            let uniform_group_id = 1;
            let data_group : HashMap< u64, Vec<String> > = [ ( uniform_group_id, [ String::from("ModelViewMatrix\0"), String::from("NormalMatrix\0"), //todo: add warning message on unmatched uniform name in uniform manager
                                                                                   String::from("ProjectionMatrix\0"), String::from("MVP\0") ] ),
            ].into_iter().map(|&( ref k, ref v)| ( k.clone(), v[..].to_vec() ) ).collect();
            
            let c = i_component::ComponentRenderUniform {
                _data_dict_mat4f: data_map_mat4f,
                _data_dict_mat3f: data_map_mat3f,
                _data_uniform_group: data_group,
                ..Default::default()
            };
            components.push( Box::new(c) );
            println!( "load into render buffer: uniform: camera" );
        }
        Ok( () )
    }
}

impl i_renderobj::ObjPos for Cam {
    fn get_pos( & self ) -> math::mat::Mat3x1< f32 > {
        self._pos
    }
}
