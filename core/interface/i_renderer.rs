pub trait IRenderer {
    type EventRender;
    fn init();
    fn process_render_events( & self, & [ Self::EventRender ] ) -> Result< (), & 'static str >;
}
