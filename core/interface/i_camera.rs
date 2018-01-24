use implement::render::camera;

pub trait ICamera {
    fn clear_property_next( & mut self );
    fn set_property_next( & mut self, prop: ( camera::PropKey, camera::PropVal ) ) -> Result< (), & 'static str >;
    fn flush_property_next( & mut self ) -> Result< (), & 'static str >;
    ///run property interpolators for some steps
    fn run_interpolators( & mut self, _steps: i64 ) -> Result< (), & 'static str >;
    ///apply current interpolator values to camera properties
    fn apply_interp_to_cam( & mut self ) -> Result< ( & camera::Cam ), & 'static str >;
    fn get_cam( & mut self ) -> & camera::Cam;
}
