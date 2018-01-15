extern crate glutin;

use interface::i_window::IWindow;
use interface::i_game_logic::IGameLogic;
use interface::i_renderer::IRenderer;

use implement::window::winglutin::WinGlutin;
use implement::kernel::kernel;

pub struct GameLogic {
    //todo
}

impl IGameLogic for GameLogic {
    type EventInput = glutin::WindowEvent;
    type EventRender = DummyRenderEventType;
    fn init() -> GameLogic {
        GameLogic {}
    }
    fn process_input_events( & self, e: & [ Self::EventInput ] ) -> Vec< Self::EventRender > {
        vec![]
    }
}

pub struct Renderer {
    //todo
}

impl IRenderer for Renderer {
    type EventRender = DummyRenderEventType;
    fn init(){}
    fn process_render_events( & self, e: & [ Self::EventRender ] ) -> Result< (), & 'static str > {
        Ok( () )
    }
}

pub struct DummyRenderEventType {
    //todo
}

type KernelImpl000 = kernel::Kernel < GameLogic, Renderer, WinGlutin, glutin::WindowEvent, DummyRenderEventType >;
