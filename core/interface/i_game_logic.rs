use std::fmt::Debug;

use interface::i_ui::{ InputFiltered, KeyCode };

pub trait IGameLogic {
    type EventInput : Debug;
    type EventRender;
    type GameState : Default;
    type ComputeUnit;
    type ComputeSchedule;
    type RenderObj;
    
    fn new() -> Self;
    
    fn run_init_hook( & mut self ) -> Result< (), & 'static str >;

    ///computes changed game state given user inputs and current game state
    fn transition_states( & mut self, inputs: & [ Self::EventInput ] ) -> Self::GameState;

    ///compute constraints per cycle
    fn continue_compute( & mut self, changed_states: & Self::GameState ) -> bool;

    ///get what to compute based on changed game state
    fn get_computations( & mut self, changed_game_state: & Self::GameState ) -> Vec< Self::ComputeUnit >;

    ///schedule computations
    fn schedule_computes( & mut self, computes: Vec< Self::ComputeUnit > ) -> Vec< Self::ComputeSchedule >;

    ///apply computed change to game state
    fn apply_changes_after_compute( & mut self, s: & mut Self::GameState );

    ///get all renderable objects from current game state
    fn get_renderable_components( & mut self, s: & Self::GameState ) -> Vec< Self::RenderObj >;

    ///do optimization on renderable objects
    fn filter_renderables( & mut self, r: Vec< Self::RenderObj >, s: & Self::GameState ) -> Vec< Self::RenderObj >;

    ///transform renderable objects to render events
    fn get_render_events( & mut self, r: Vec< Self::RenderObj >, s: & Self::GameState ) -> Vec< Self::EventRender >;

    fn should_exit( & mut self, s: & Self::GameState ) -> bool;
    
    ///default implementation
    fn process_input_events( & mut self, e: & [ Self::EventInput ] ) -> ( Vec< Self::EventRender >, bool ) {

        let mut render_events = vec![];

        //process input events
        if e.len() > 0 {
            trace!( "filtered_input: {:?}", e );
        }
        
        //todo: get changed states
        let mut changed_states = self.transition_states( e );
        
        //todo: perform computations
        while self.continue_compute( & changed_states ) {

            //todo: transform changed game states to determine what to compute/update
            let computes = self.get_computations( & changed_states );

            //todo: schdule computations
            let s = self.schedule_computes( computes );

            // //todo: apply computations and collect changes
            // let changed_state = s.iter().map( |x| { unimplemented!(); } ).fold( Default::default(), | accum, x| { unimplemented!(); } );
            
            //todo: apply result of changes back to current game state
            self.apply_changes_after_compute( & mut changed_states );
        }
        
        let render_objects = self.get_renderable_components( & changed_states );

        //do some spatial optimization
        let render_objects_filtered = self.filter_renderables( render_objects, & changed_states );

        //do further render commands packaging here
        let mut events = self.get_render_events( render_objects_filtered, & changed_states );
        
        render_events.append( & mut events );

        // self.apply_changes( & changed_states );
        
        let sig_exit = self.should_exit( & changed_states );
        
        ( render_events, sig_exit )
    }
}
