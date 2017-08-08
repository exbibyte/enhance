use implement::math::mat;

pub trait Xform {
    fn get_xform() -> mat::Mat4< f32 >;
}

pub trait ObjPos {
    fn get_pos( & self ) -> mat::Mat3x1< f32 >;
}

pub trait RenderDevice {
    fn bind_buffer( & mut self ) -> Result< (), & 'static str >;
    fn draw_buffer_all( & mut self ) -> Result< (), & 'static str >;
    fn draw_buffer_range( & mut self ) -> Result< (), & 'static str >;
}

pub trait IRenderBuffer< T > where T : RenderDevice {
    fn load_into_buffer( & mut self, rd: & mut T ) -> Result< (), & 'static str >;
}
