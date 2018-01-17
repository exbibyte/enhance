extern crate glutin;

use interface::i_window::IWindow;
use interface::i_game_logic::IGameLogic;
use interface::i_renderer::IRenderer;

use implement::window::winglutin::WinGlutin;
use implement::kernel::kernel;
use implement::kernel::kernel_render;

pub struct GameLogic {
    //todo
}

impl IGameLogic for GameLogic {
    type EventInput = glutin::Event;
    type EventRender = kernel_render::DummyRenderEventType;
    fn init() -> GameLogic {
        GameLogic {}
    }
    fn process_input_events( & self, e: & [ Self::EventInput ] ) -> Vec< Self::EventRender > {
        vec![]
    }
}


pub type KernelImpl000 = kernel::Kernel < GameLogic, kernel_render::Renderer, WinGlutin, glutin::Event, kernel_render::DummyRenderEventType >;
