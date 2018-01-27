use interface::i_load::ILoad;

#[derive(Clone, Debug)]
pub struct LoadSingle < T > {
    _buf: Option< T >,
}

impl < T > Default for LoadSingle < T > {
    fn default() -> Self {
        Self {
            _buf: None,
        }
    }
}

impl < T > LoadSingle < T > {

    pub fn load( & mut self, t: T ) -> Result< (), & 'static str > {
        self._buf = Some( t );
        Ok( () )
    }

    pub fn apply< F >( & mut self, mut f: F ) -> Result< (), & 'static str >
        where F: FnMut( & T ) -> bool
    {
        let consumed = match self._buf {
            None => { false }, //do nothing if no data
            Some(ref x) => { f( x ); true },
        };

        if consumed {
            self._buf = None;
        }
        
        Ok( () )
    }
}

