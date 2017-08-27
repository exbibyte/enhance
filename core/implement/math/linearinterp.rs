use std::cmp;

use implement::math::mat::*;
use interface::i_interpolate::*;

#[derive(Debug)]
#[derive(Clone)]
pub struct LinearInterp {
    pub _ctl: [ Mat4x1< f64 > ; 2 ],
    pub _point: Mat4x1< f64 >,
    pub _steps: i64,
    pub _step_current: i64,
}

impl LinearInterp {
    pub fn init( s: i64, cp0: Mat4x1< f64 >, cp1: Mat4x1< f64 > ) -> LinearInterp {
        assert!( s > 2 );
        LinearInterp {
            _ctl: [ cp0, cp1 ],
            _point: cp0, //start point
            _steps: s,
            _step_current: -1i64,
        }
    }
}

#[allow(unused_variables)]
impl IInterpolateMat4x1f64 for LinearInterp {
    type InterpVal = Mat4x1< f64 >;
    fn num_steps( & self ) -> u64 {
        self._steps as u64
    }
    fn interp_delta( & mut self, steps: i64 ) -> Option< Mat4x1< f64 > > {
        self._step_current = cmp::min( self._step_current + steps, self._steps + 1);
        self._step_current = cmp::max( self._step_current, -1 );

        let clamp = if self._step_current < 0 {
            0
        } else if self._step_current > self._steps {
            self._steps
        } else {
            self._step_current
        };
        let fraction = clamp as f64 / self._steps as f64;
        let d = self._ctl[1].minus( &self._ctl[0] ).unwrap();
        let offset = d.scale( fraction ).unwrap();
        self._point = self._ctl[0].plus( &offset ).unwrap();
        Some( self._point )
    }
    fn interp_current( & self ) -> Mat4x1< f64 > {
        self._point
    }
    fn interp_is_end( & self ) -> bool {
        self._step_current == self._steps + 1
    }
    fn interp_is_start( & self ) -> bool {
        self._step_current == -1
    }
    fn reset( & mut self ){
        self._step_current = -1i64;
    }
}

impl Iterator for LinearInterp { //required by IInterpolateMat4x1f64
    type Item = Mat4x1< f64 >;
    fn next( & mut self ) -> Option< Mat4x1< f64 > > {
        if self._step_current == self._steps + 1 {
            None
        } else {
            self._step_current = cmp::min( self._step_current + 1, self._steps + 1);

            let clamp = if self._step_current < 0 {
                0
            } else if self._step_current > self._steps {
                self._steps
            } else {
                self._step_current
            };
            
            let fraction = clamp as f64 / self._steps as f64;
            let d = self._ctl[1].minus( &self._ctl[0] ).unwrap();
            let offset = d.scale( fraction ).unwrap();
            self._point = self._ctl[0].plus( &offset ).unwrap();
            Some( self._point )
        }
    }
}

impl DoubleEndedIterator for LinearInterp {
    fn next_back( & mut self ) -> Option< Mat4x1< f64 > > {
        if self._step_current == -1 {
            None 
        } else {
            self._step_current = cmp::max( self._step_current - 1, -1 );

            let clamp = if self._step_current < 0 {
                0
            } else if self._step_current > self._steps {
                self._steps
            } else {
                self._step_current
            };
            
            let fraction = clamp as f64 / self._steps as f64;
            let d = self._ctl[1].minus( &self._ctl[0] ).unwrap();
            let offset = d.scale( fraction ).unwrap();
            self._point = self._ctl[0].plus( &offset ).unwrap();
            Some( self._point )
        }
    }
}
