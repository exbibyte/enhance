use std::any::Any;

// use interface::i_renderobj;
use interface::i_component;

/// # all available functionalities for an object
pub trait IObjImpl: IObjImplClone {
    fn as_any( & self ) -> & Any;
    fn update_components( & mut self, components: & mut Vec< Box< i_component::IComponent > > ) -> Result< (), & 'static str >;
}

pub trait IObjImplClone {
    fn clone_box( & self ) -> Box< IObjImpl >;
}

impl< T > IObjImplClone for T where T: 'static + IObjImpl + Clone {
    fn clone_box( & self ) -> Box< IObjImpl > {
        Box::new( self.clone() )
    }
}

impl Clone for Box< IObjImpl > {
    fn clone( & self ) -> Box< IObjImpl > {
        self.clone_box()
    }
}

/// # generic encapsulation for a create object
#[derive(Clone)]
pub struct Ele {
    pub _impl: Box< IObjImpl >,
    pub _components: Vec< Box< i_component::IComponent > >,
}

impl Ele {
    pub fn init< T >( c: T ) -> Ele where T : IObjImpl + 'static {
        Ele {
            _impl: Box::new(c),
            _components: vec![],
        }
    }
    pub fn update_components_from_impl( & mut self ) -> Result< (), & 'static str > {
        self._components.clear();
        self._impl.update_components( & mut self._components )
    }
    // pub fn apply_component< T >( & mut self, comp: T ) -> Result< (), & 'static str >
    //     where T: i_component::IComponent
    // {
    //     self._components.push( Box::new( comp ) );
    //     Ok( () )
    // }
    // pub fn get_components( & mut self ) -> Result< &[ Box< i_component::IComponent > ], & 'static str > {
    //     Ok( &self._components[..] )
    // }

    // pub fn get_impl( & mut self ) -> &IObjImpl {
    //     self._impl.deref_mut()
    // }
}

// impl Deref for Ele {
//     type Target = Ele;
//     fn deref( & self ) -> & Self::Target {
//         & self
//     }
// }

// impl DerefMut for Ele {
//     fn deref_mut( & mut self ) -> & mut Ele {
//         & mut self
//     }
// }
