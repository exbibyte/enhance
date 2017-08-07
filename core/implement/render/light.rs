use std::collections::HashMap;
use std::vec::Vec;

use implement::math::mat::Mat4;
use implement::math::mat::Mat3x1;
use implement::render::camera;
use interface::i_ads;
use interface::i_renderobj;

pub struct LightAdsPoint {
    pub _id: u64,
    pub _pos: Mat3x1< f32 >,

    /// # ads values
    pub _ads_val_spec: Mat3x1< f32 >,
    pub _ads_val_diff: Mat3x1< f32 >,
    pub _ads_val_amb: Mat3x1< f32 >,
}

impl i_ads::AdsVal for LightAdsPoint {
    fn get_ads_val_spec( & self ) -> Mat3x1< f32 > {
        self._ads_val_spec
    }
    fn get_ads_val_diff( & self ) -> Mat3x1< f32 > {
        self._ads_val_spec
    }
    fn get_ads_val_amb( & self ) -> Mat3x1< f32 > {
        self._ads_val_amb
    }
}

impl i_renderobj::ObjPos for LightAdsPoint {
    fn get_pos( & self ) -> Mat3x1< f32 > {
        self._pos
    }
}

pub struct LightAdsSpot {
    pub _id: u64,
    pub _pos: Mat3x1< f32 >,

    /// # ads values
    pub _ads_val_spec: Mat3x1< f32 >,
    pub _ads_val_diff: Mat3x1< f32 >,
    pub _ads_val_amb: Mat3x1< f32 >,

    pub _fov: f32,
    pub _focus: Mat4< f32 >,

    /// # computed view from _pos, _focus
    pub _light_view_xform: Mat4< f32 >,
}

impl i_ads::AdsVal for LightAdsSpot {
    fn get_ads_val_spec( & self ) -> Mat3x1< f32 > {
        self._ads_val_spec
    }
    fn get_ads_val_diff( & self ) -> Mat3x1< f32 > {
        self._ads_val_spec
    }
    fn get_ads_val_amb( & self ) -> Mat3x1< f32 > {
        self._ads_val_amb
    }
}

impl i_renderobj::ObjPos for LightAdsSpot {
    fn get_pos( & self ) -> Mat3x1< f32 > {
        self._pos
    }
}

pub struct LightAdsDir {
    pub _id: u64,
    pub _pos: Mat3x1< f32 >,

    /// # ads values
    pub _ads_val_spec: Mat3x1< f32 >,
    pub _ads_val_diff: Mat3x1< f32 >,
    pub _ads_val_amb: Mat3x1< f32 >,

    pub _dir: Mat3x1< f32 >,
    pub _light_view_xform: Mat4< f32 >,
}

impl i_ads::AdsVal for LightAdsDir {
    fn get_ads_val_spec( & self ) -> Mat3x1< f32 > {
        self._ads_val_spec
    }
    fn get_ads_val_diff( & self ) -> Mat3x1< f32 > {
        self._ads_val_spec
    }
    fn get_ads_val_amb( & self ) -> Mat3x1< f32 > {
        self._ads_val_amb
    }
}
