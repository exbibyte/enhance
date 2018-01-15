extern crate glutin;

use interface::i_window::IWindow;
use interface::i_game_logic::IGameLogic;
use interface::i_renderer::IRenderer;

use implement::window::winglutin::WinGlutin;


pub struct Kernel < G, R, W, EInput, ERender >
    where G: IGameLogic< EventInput = EInput, EventRender = ERender >,
          R: IRenderer< EventRender = ERender >,
          W: IWindow< EventType = EInput >
{   
    _windowing: W,
    _game_logic: G,
    _renderer: Vec< R >, //alternative rendering pipelines
    // _textures: Vec< texture::Texture >,
    // _lights: Vec< light::LightAdsPoint >,
    // _cameras: Vec< camera::Cam >,
}

impl < G, R, W, EInput, ERender > Kernel < G, R, W, EInput, ERender >
    where G: IGameLogic< EventInput = EInput, EventRender = ERender >,
          R: IRenderer< EventRender = ERender >,
          W: IWindow< EventType = EInput >
{
    pub fn init() -> Result< Self, & 'static str > {
        Ok(
            Kernel {
                _windowing: IWindow::init( 500, 500 ),
                _game_logic: G::init(),
                _renderer: vec![],
                // _textures: Vec< texture::Texture >,
                // _lights: Vec< light::LightAdsPoint >,
                // _cameras: Vec< camera::Cam >,

            }
        )
    }
    pub fn run( & mut self ) -> Result< (), & 'static str > {
        //foever loop and process results until exit conditions are caught
        let mut running = true;

        let mut sig_window_close = false;
        
        let mut new_win_dim = Some( ( 500, 500 ) );
        
        let mut sigs_for_window = vec![];

        while running {

            //process windowing events into buffer
            self._windowing.make_current();

            self._windowing.handle_signal_request( sigs_for_window.as_slice() );
            
            let mut events_window : Vec< EInput > = vec![];
            match self._windowing.handle_events_pass_thru() {
                Some( x ) => {
                    events_window.push( x );
                },
                _ => {},
            }
            
            // //process game logic
            //todo: put this in a parallel thread
            // let events_render: Vec< ERender > = game_logic.process_input_events( & events_window[..] ).unwrap();

            // //detect exit condition
            
            // //process rendering
            //todo: put this in a parallel thread
            // renderer.process_render_events( & events_render[..] ).is_ok();
        }
        Ok( () )
    }
}

