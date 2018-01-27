use std::cmp;
use std::marker;

use interface::i_interpolate::*;
use interface::i_load;

use implement::util::loadqueue;

// #[derive(Debug)]
#[derive(Clone)]
pub struct Piecewise< T, V > where T: IInterpolate< V >, V : Clone {
    pub _pieces: Vec< T >,
    _current_piece: u64,
    pub _total_steps: u64,
    pub _phantom: marker::PhantomData< V >,
    pub _load: loadqueue::LoadQueue< V >,
}

impl < T, V > Piecewise< T, V > where T: IInterpolate< V >, V : Clone {
    pub fn init() -> Piecewise< T, V > {
        Piecewise { _pieces: Vec::new(),
                    _current_piece: 0u64,
                    _total_steps: 0u64,
                    _phantom: marker::PhantomData,
                    _load: Default::default(),
        }
    }
    pub fn add( & mut self, s: T ){
        self._total_steps = self._total_steps + s.num_steps();
        self._pieces.push( s );
    }
    pub fn interp_current( & self ) -> V {
        if self._current_piece as usize >= self._pieces.len() {
            self._pieces[ self._pieces.len() - 1 ].interp_current()
        } else {
            self._pieces[ self._current_piece as usize ].interp_current()
        }
    }
    pub fn reset( & mut self ) {
        for i in 0..self._pieces.len() {
            self._pieces[i].reset();
        }
        self._current_piece = 0u64;
    }
}

#[allow(unused_variables)]
impl < T, V > IInterpolate< V > for Piecewise< T, V > where T: IInterpolate< V >, V : Clone {
    fn num_steps( & self ) -> u64 {
        self._total_steps
    }
    fn interp_delta( & mut self, steps: i64 ) -> Option< V > {
        let mut ret = None;
        for i in 0..steps {
            match self.next() {
                Some(x) => {
                    ret = Some( self.interp_current() );
                },
                None => {
                    return ret
                },
            }
        }
        ret
    }
    fn interp_current( & self ) -> V {
        if self._current_piece as usize >= self._pieces.len() {
            self._pieces[ self._pieces.len() - 1 ].interp_current()
        } else {
            self._pieces[ self._current_piece as usize ].interp_current()
        }
    }
    fn interp_is_end( & self ) -> bool {
        (self._current_piece as usize) >= self._pieces.len() ||
            ( ( self._current_piece as usize == ( self._pieces.len() - 1 ) ) && 
                self._pieces[ self._current_piece as usize ].interp_is_end() )
    }
    fn interp_is_start( & self ) -> bool {
        self._current_piece == 0 && self._pieces[ self._current_piece as usize ].interp_is_start()        
    }
    fn reset( & mut self ){
        for i in 0..self._pieces.len() {
            self._pieces[i].reset();
        }
    }
}

/// required trait for IInterpolate
impl < T, V > Iterator for Piecewise< T, V > where T: IInterpolate< V >, V : Clone {
    type Item = V;
    fn next( & mut self ) -> Option< V > {
        if self._current_piece as usize >= self._pieces.len() {
            return None
        } else {
            let n = self._pieces[ self._current_piece as usize ].next();
            match n {
                Some(s) => return Some(s),
                None => {
                    self._current_piece = cmp::min( self._current_piece + 1, self._pieces.len() as _ );
                    if self._current_piece as usize >= self._pieces.len() {
                        return None
                    } else {
                        match self.next() {
                            Some( o ) => Some( o ),
                            None => {
                                if self._current_piece as usize == self._pieces.len() - 1 {
                                    self._current_piece += 1;
                                }
                                None
                            },
                        }
                    }
                }
            }
        }
    }
}

impl < T, V > DoubleEndedIterator for Piecewise< T, V > where T: IInterpolate< V > , V : Clone {
    fn next_back( & mut self ) -> Option< V > {
        unimplemented!();
    }
}


impl < T, V > i_load::ILoad for Piecewise< T, V > where T: IInterpolate< V >, V : Clone {

    type InputType = V;
    
    fn load( & mut self, t: Self::InputType ) -> Result< (), & 'static str > {
        self._load.load( t )?;
        Ok( () )
    }
    fn apply( & mut self ) -> Result< bool, & 'static str > {
        
        let mut count_fetched = 0;

        let mut fetched  = None;
        
        self._load.apply( | x: & Self::InputType | { if count_fetched < 1 {
            fetched = Some( x.clone() );
            count_fetched += 1;
            true
        } else {
            false
        } } )?;

        //todo: do something with fetch

        Ok( true )
    }    
}

