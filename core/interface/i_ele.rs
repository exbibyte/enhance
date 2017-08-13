use std::ops::{ Deref, DerefMut };

use interface::i_renderobj;

/// # all available functionalities for an object
pub trait Facility : i_renderobj::IRenderable + i_renderobj::IRenderBuffer + i_renderobj::IRenderUniform + 'static {}

/// # generic encapsulation for various types of objects
pub struct Ele {
    pub _comp: Box< Facility >,
}

impl Ele {
    pub fn init< T >( c: T ) -> Ele where T : Facility {
        Ele {
            _comp: Box::new(c),
        }
    }
}

impl Deref for Ele {
    type Target = Box< Facility >;
    fn deref( & self ) -> & Self::Target {
        & self._comp
    }
}

impl DerefMut for Ele {
    fn deref_mut( & mut self ) -> & mut Box< Facility > {
        & mut self._comp
    }
}
