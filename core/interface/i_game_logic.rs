pub trait IGameLogic {
    type EventInput;
    type EventRender;
    fn new() -> Self;
    fn process_input_events( & mut self, & [ Self::EventInput ] ) -> ( Vec< Self::EventRender >, bool );
}
