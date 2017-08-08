use implement::math::mat::*;
use interface::i_interpolate::*;

#[allow(unused_variables)]
#[derive(Debug)]
pub struct SplineBezier {
    pub _ctl: [ Mat4x1< f64 > ; 4 ],
    pub _point: Mat4x1< f64 >,
    pub _point_d: Mat4x1< f64 >,
    pub _point_dd: Mat4x1< f64 >,
    pub _point_ddd: Mat4x1< f64 >,
    pub _basis: Mat4< f64 >,
    pub _steps: u64,
    pub _step_current: u64,
}

impl SplineBezier {
    pub fn init( s: u64, cp0: Mat4x1< f64 >, cp1: Mat4x1< f64 >, cp2: Mat4x1< f64 >, cp3: Mat4x1< f64 > ) -> SplineBezier {
        assert!( s > 2 );
        let d = 1f64 / ( s as f64 - 1f64 );
        let d2 = d * d;
        let d3 = d * d2;
        
        let px = Mat1x4 { _val: [ cp0._val[0], cp1._val[0], cp2._val[0], cp3._val[0] ] };
        let py = Mat1x4 { _val: [ cp0._val[1], cp1._val[1], cp2._val[1], cp3._val[1] ] };
        let pz = Mat1x4 { _val: [ cp0._val[2], cp1._val[2], cp2._val[2], cp3._val[2] ] };
        let pw = Mat1x4 { _val: [ cp0._val[3], cp1._val[3], cp2._val[3], cp3._val[3] ] };
        
        let basis = Mat4 { _val: [ 1f64, -3f64, 3f64, -1f64,
                                   0f64, 3f64, -6f64, 3f64,
                                   0f64, 0f64, 3f64, -3f64,
                                   0f64, 0f64, 0f64, 1f64 ],
                           _is_row_major: true
        };
        
        let mut cvec: [ Mat1x4<_>; 4 ] = [ Mat1x4 { _val: [0f64; 4] } ; 4 ];
        cvec[0] = px.mul_mat4x4( &basis ).expect("mat1x4 mul mat4x4 invalid");
        cvec[1] = py.mul_mat4x4( &basis ).expect("mat1x4 mul mat4x4 invalid");
        cvec[2] = pz.mul_mat4x4( &basis ).expect("mat1x4 mul mat4x4 invalid");
        cvec[3] = pw.mul_mat4x4( &basis ).expect("mat1x4 mul mat4x4 invalid");
        
        let mut dq : Mat4x1<_> = Mat4x1::default();
        let mut ddq : Mat4x1<_> = Mat4x1::default();
        let mut dddq : Mat4x1<_> = Mat4x1::default();

        for i in 0..4 {
            let a = cvec[i]._val[3]; //t^3 term
            let b = cvec[i]._val[2]; //t^2
            let c = cvec[i]._val[1]; //t^1
            dq._val[i] = a * d3 + b * d2 + c * d;
            ddq._val[i] = 6f64 * a * d3 + 2f64 * b * d2;
            dddq._val[i] = 6f64 * a * d3;
        }
        
        SplineBezier {
                        _ctl: [ cp0, cp1, cp2, cp3 ],
                        _point: cp0, //start point
                        _point_d: dq,
                        _point_dd: ddq,
                        _point_ddd: dddq,
                        _basis: basis,
                        _steps: s,
                        _step_current: 0u64,
        }
    }
}
#[allow(unused_variables)]
impl IInterpolateMat4x1f64 for SplineBezier {
    fn interp_increment( & mut self, step_increment: u64 ) -> Mat4x1< f64 > {
        let ret = self._point.clone();
        for x in 0..step_increment {
            if self._step_current >= self._steps {
                self._point = self._ctl[3];
                return ret
            }
            self._point = self._point.plus( & self._point_d ).expect("point plus invalid");
            self._point_d = self._point_d.plus( & self._point_dd ).expect("point d plus invalid");
            self._point_dd = self._point_dd.plus( & self._point_ddd ).expect("point dd plus invalid");
            self._step_current = self._step_current + 1;
        }
        ret
    }
    fn interp_current( & self ) -> Mat4x1< f64 > {
        self._point
    }
    fn interp_done( & self ) -> bool {
        self._step_current >= self._steps
    }
}

impl Iterator for SplineBezier { //required by IInterpolateMat4x1f64
    type Item = Mat4x1< f64 >;
    fn next( & mut self ) -> Option< Self::Item > {
        if self._step_current >= self._steps {
            return None
        } else {
            let ret = self._point.clone();
            self._point = self._point.plus( & self._point_d ).expect("point plus invalid");
            self._point_d = self._point_d.plus( & self._point_dd ).expect("point d plus invalid");
            self._point_dd = self._point_dd.plus( & self._point_ddd ).expect("point dd plus invalid");
            self._step_current = self._step_current + 1;
            return Some( ret )
        }
    }
}

pub struct SplinePiecewise< T > where T : IInterpolateMat4x1f64 {
    pub _splines: Vec< T >,
    _current_spline: u64,
}

impl < T > SplinePiecewise< T > where T : IInterpolateMat4x1f64 {
    pub fn init() -> SplinePiecewise< T > {
        SplinePiecewise { _splines: Vec::new(), _current_spline: 0u64 }
    }
    pub fn add( & mut self, s: T ){
        self._splines.push( s );
    }
}

impl < T > Iterator for SplinePiecewise< T > where T : IInterpolateMat4x1f64 {
    type Item = T::Item;
    fn next( & mut self ) -> Option< Self::Item > {
        if self._current_spline as usize >= self._splines.len() {
            return None;
        } else {
            let n = self._splines[ self._current_spline as usize ].next();
            match n {
                Some(s) => return Some(s),
                _ => {
                    if (self._current_spline + 1) as usize >= self._splines.len() {
                        return None;
                    } else {
                        self._current_spline = self._current_spline + 1;
                        return self.next();
                    }
                }
            }
        }
    }
}
