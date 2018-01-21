use std::collections::HashMap;

use implement::math::mat;
use implement::render::renderdevice_gl::RenderUniformCollection;

pub trait IGameLogic {
    type EventInput;
    type EventRender;
    fn init() -> Self;
    fn process_input_events( & mut self, & [ Self::EventInput ] ) -> ( Vec< Self::EventRender >, bool );
}
