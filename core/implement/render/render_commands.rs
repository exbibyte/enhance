use std::vec::Vec;
use std::any::Any;

use interface::i_ele;
use interface::i_component;

/// # command for resetting draw group content

pub struct CmdDrawGroupClear ( pub i_component::ComponentDrawGroupClear );

impl CmdDrawGroupClear {

    pub fn init( group_id: usize ) -> CmdDrawGroupClear {
        CmdDrawGroupClear( i_component::ComponentDrawGroupClear { _group_id: group_id } )
    }
}

impl i_ele::IObjImpl for CmdDrawGroupClear {
    fn as_any( & self ) -> & Any {
        self
    }
    
    fn update_components( & mut self, components: & mut Vec< Box< i_component::IComponent > > ) -> Result< (), & 'static str > {
        let c = self.0.clone();
        components.push( Box::new( c ) );
        println!( "loaded command: draw group: clear" );
        Ok( () )
    }
}

pub struct CmdDrawGroupDependentObjects ( pub i_component::ComponentDrawGroupDependentObjects );

impl CmdDrawGroupDependentObjects {

    pub fn init( group_id: usize, obj_ids: &[ usize ] ) -> CmdDrawGroupDependentObjects {
        CmdDrawGroupDependentObjects( i_component::ComponentDrawGroupDependentObjects { _group_id: group_id, _obj_ids: obj_ids.to_vec() } )
    }
}

impl i_ele::IObjImpl for CmdDrawGroupDependentObjects {
    fn as_any( & self ) -> & Any {
        self
    }
    
    fn update_components( & mut self, components: & mut Vec< Box< i_component::IComponent > > ) -> Result< (), & 'static str > {
        let c = self.0.clone();
        components.push( Box::new( c ) );
        println!( "loaded command: draw group: set objects" );
        Ok( () )
    }
}

pub struct CmdDrawGroupBind ( pub i_component::ComponentDrawGroupBind );

impl CmdDrawGroupBind {

    pub fn init( group_id: usize ) -> CmdDrawGroupBind {
        CmdDrawGroupBind( i_component::ComponentDrawGroupBind { _group_id: group_id } )
    }
}

impl i_ele::IObjImpl for CmdDrawGroupBind {
    fn as_any( & self ) -> & Any {
        self
    }
    
    fn update_components( & mut self, components: & mut Vec< Box< i_component::IComponent > > ) -> Result< (), & 'static str > {
        let c = self.0.clone();
        components.push( Box::new( c ) );
        println!( "loaded command: draw group: set objects" );
        Ok( () )
    }
}

pub struct CmdDrawGroupDispatch ( pub i_component::ComponentDrawGroupDispatch );

impl CmdDrawGroupDispatch {

    pub fn init( group_id: usize ) -> CmdDrawGroupDispatch {
        CmdDrawGroupDispatch( i_component::ComponentDrawGroupDispatch { _group_id: group_id } )
    }
}

impl i_ele::IObjImpl for CmdDrawGroupDispatch {
    fn as_any( & self ) -> & Any {
        self
    }
    
    fn update_components( & mut self, components: & mut Vec< Box< i_component::IComponent > > ) -> Result< (), & 'static str > {
        let c = self.0.clone();
        components.push( Box::new( c ) );
        println!( "loaded command: draw group: set objects" );
        Ok( () )
    }
}


pub struct CmdDrawGroupDependentUniforms ( pub i_component::ComponentDrawGroupDependentUniforms );

impl CmdDrawGroupDependentUniforms {

    pub fn init( group_id: usize, uniform_ids: &[ u64 ] ) -> CmdDrawGroupDependentUniforms {
        CmdDrawGroupDependentUniforms( i_component::ComponentDrawGroupDependentUniforms { _group_id: group_id, _uniform_ids: uniform_ids.to_vec() } )
    }
}

impl i_ele::IObjImpl for CmdDrawGroupDependentUniforms {
    fn as_any( & self ) -> & Any {
        self
    }
    
    fn update_components( & mut self, components: & mut Vec< Box< i_component::IComponent > > ) -> Result< (), & 'static str > {
        let c = self.0.clone();
        components.push( Box::new( c ) );
        println!( "loaded command: draw group: set objects" );
        Ok( () )
    }
}
