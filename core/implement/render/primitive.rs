use std::collections::HashMap;
// use std::cell::RefCell;

use interface::i_ele;
use interface::i_renderobj;

use implement::math::mat;
use implement::render::renderdevice_gl;

/// # 6 sided polyhedral / box
pub struct Poly6 {
    /// # _pos := center of the box
    pub _pos: mat::Mat3x1< f32 >,
    /// # _radius := approximation of the box size
    pub _radius: f32,
}

// impl Poly6 {
//     pub fn init( pos: mat::Mat3x1< f32 >, radius: f32 ) -> Poly6 {
//         Poly6 {
//             _pos: pos,
//             _radius: radius,
//         }
//     }
// }

impl i_ele::Facility for Poly6 {
    // type Concrete = Poly6;
}

impl i_renderobj::IRenderable for Poly6 {
    fn get_render_method( & self ) -> i_renderobj::RenderMethod {
        i_renderobj::RenderMethod::ADS
    }
}

impl i_renderobj::IRenderBuffer for Poly6 {
    fn load_into_buffer( & mut self, rd: & mut i_renderobj::RenderDevice ) -> Result< (), & 'static str > {       
        let x0 = self._pos[0] - self._radius/2.0;
        let x1 = self._pos[0] + self._radius/2.0;

        let y0 = self._pos[1] - self._radius/2.0;
        let y1 = self._pos[1] + self._radius/2.0;

        let z0 = self._pos[2] - self._radius/2.0;
        let z1 = self._pos[2] + self._radius/2.0;

        let mut pos = vec![];
        let mut normal = vec![];
        let mut tc = vec![];

        //face 1 (front) trig 1
        pos.extend_from_slice( &[ x0, y0, z1 ] );
        normal.extend_from_slice( &[ 0.0, 0.0, 1.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y0, z1 ] );
        normal.extend_from_slice( &[ 0.0, 0.0, 1.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y1, z1 ] );
        normal.extend_from_slice( &[ 0.0, 0.0, 1.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        //face 1 (front) trig 2
        pos.extend_from_slice( &[ x0, y0, z1 ] );
        normal.extend_from_slice( &[ 0.0, 0.0, 1.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y1, z1 ] );
        normal.extend_from_slice( &[ 0.0, 0.0, 1.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x0, y1, z1 ] );
        normal.extend_from_slice( &[ 0.0, 0.0, 1.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        //face 2 (back) trig 1.0
        pos.extend_from_slice( &[ x0, y0, z0 ] );
        normal.extend_from_slice( &[ 0.0, 0.0, -1.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x0, y1, z0 ] );
        normal.extend_from_slice( &[ 0.0, 0.0, -1.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y0, z0 ] );
        normal.extend_from_slice( &[ 0.0, 0.0, -1.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        //face 2 (back) trig 2
        pos.extend_from_slice( &[ x1, y0, z0 ] );
        normal.extend_from_slice( &[ 0.0, 0.0, -1.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x0, y1, z0 ] );
        normal.extend_from_slice( &[ 0.0, 0.0, -1.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y1, z0 ] );
        normal.extend_from_slice( &[ 0.0, 0.0, -1.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        //face 3 (left) trig 1.0
        pos.extend_from_slice( &[ x0, y0, z1 ] );
        normal.extend_from_slice( &[ -1.0, 0.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x0, y1, z1 ] );
        normal.extend_from_slice( &[ -1.0, 0.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x0, y1, z0 ] );
        normal.extend_from_slice( &[ -1.0, 0.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        //face 3 (left) trig 2
        pos.extend_from_slice( &[ x0, y0, z1 ] );
        normal.extend_from_slice( &[ -1.0, 0.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x0, y1, z0 ] );
        normal.extend_from_slice( &[ -1.0, 0.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x0, y0, z0 ] );
        normal.extend_from_slice( &[ -1.0, 0.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        //face 4 (right) trig 1.0
        pos.extend_from_slice( &[ x1, y0, z1 ] );
        normal.extend_from_slice( &[ 1.0, 0.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y0, z0 ] );
        normal.extend_from_slice( &[ 1.0, 0.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y1, z0 ] );
        normal.extend_from_slice( &[ 1.0, 0.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        //face 4 (right) trig 2
        pos.extend_from_slice( &[ x1, y0, z1 ] );
        normal.extend_from_slice( &[ 1.0, 0.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y1, z0 ] );
        normal.extend_from_slice( &[ 1.0, 0.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y1, z1 ] );
        normal.extend_from_slice( &[ 1.0, 0.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        //face 5 (top) trig 1.0
        pos.extend_from_slice( &[ x0, y1, z1 ] );
        normal.extend_from_slice( &[ 0.0, 1.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y1, z1 ] );
        normal.extend_from_slice( &[ 0.0, 1.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y1, z0 ] );
        normal.extend_from_slice( &[ 0.0, 1.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        //face 5 (top) trig 2
        pos.extend_from_slice( &[ x0, y1, z1 ] );
        normal.extend_from_slice( &[ 0.0, 1.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y1, z0 ] );
        normal.extend_from_slice( &[ 0.0, 1.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x0, y1, z0 ] );
        normal.extend_from_slice( &[ 0.0, 1.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        //face 6 (bottom) trig 1.0
        pos.extend_from_slice( &[ x0, y0, z1 ] );
        normal.extend_from_slice( &[ 0.0, -1.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x0, y0, z0 ] );
        normal.extend_from_slice( &[ 0.0, -1.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y0, z0 ] );
        normal.extend_from_slice( &[ 0.0, -1.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        //face 6 (bottom) trig 2
        pos.extend_from_slice( &[ x0, y0, z1 ] );
        normal.extend_from_slice( &[ 0.0, -1.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y0, z0 ] );
        normal.extend_from_slice( &[ 0.0, -1.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        pos.extend_from_slice( &[ x1, y0, z1 ] );
        normal.extend_from_slice( &[ 0.0, -1.0, 0.0 ] );
        tc.extend_from_slice( &[ 0.0, 0.0 ] );

        let data_map : HashMap< i_renderobj::BuffDataType, &[f32] > =  [ ( i_renderobj::BuffDataType::POS, pos.as_slice() ),
                                                                             ( i_renderobj::BuffDataType::NORMAL, normal.as_slice() ),
                                                                             ( i_renderobj::BuffDataType::TC, tc.as_slice() ) ].iter().cloned().collect();
        rd.store_buff_data( i_renderobj::RenderObjType::TRI, & data_map );

        println!( "load into render buffer: primitive (box): vertex count:{}", pos.len() / 3 );

        Ok( () )
    }
}

pub struct SphereIcosahedron {
    pub _pos: mat::Mat3x1< f32 >,
    pub _radius: f32,
    pub _index: usize,
    pub _positions: Vec<[f32;3]>,
}

impl SphereIcosahedron {
    pub fn init( pos: mat::Mat3x1< f32 >, radius: f32 ) -> SphereIcosahedron {
        SphereIcosahedron {
            _pos: pos,
            _radius: radius,
            _index: 0,
            _positions: vec![],
        }
    }
    /// # add vertex to mesh, fix position to be on unit sphere, return index
    pub fn add_vertex( & mut self, p: &[f32] ) {
        let length = (p[0] * p[0] + p[1] * p[1] + p[2] * p[2] ).sqrt();
        self._positions.push( [ p[0]/length, p[1]/length, p[2]/length ] );
        self._index += 1;
    }
    /// # return index of point in the middle of p1 and p2
    pub fn get_middle_point( & mut self, p1: usize, p2: usize ) -> usize {
        // first check if we have it already
        let firstIsSmaller : bool = p1 < p2;
        let smallerIndex = if firstIsSmaller { p1 } else { p2 };
        let greaterIndex = if firstIsSmaller { p2 } else { p1 };
        let key = (smallerIndex << 32) + greaterIndex;
        
        // int ret;
        // if (this.middlePointIndexCache.TryGetValue(key, out ret))
        // {
        //     return ret;
        // }

        // not in cache, calculate it
        let point1 = self._positions[p1];
        let point2 = self._positions[p2];
        let middle = [ (point1[0] + point2[0]) / 2.0,
                        (point1[1] + point2[1]) / 2.0,
                        (point1[2] + point2[2]) / 2.0 ];

        // add vertex makes sure point is on unit sphere
        let i = self._index;
        self.add_vertex( &middle );

        // // store it, return index
        // this.middlePointIndexCache.Add(key, i);
        i
    }
}

impl i_ele::Facility for SphereIcosahedron {
    // type Concrete = SphereIcosahedron;
}

impl i_renderobj::IRenderable for SphereIcosahedron {
    fn get_render_method( & self ) -> i_renderobj::RenderMethod {
        i_renderobj::RenderMethod::ADS
    }
}

impl i_renderobj::IRenderBuffer for SphereIcosahedron {
    fn load_into_buffer( & mut self, rd: & mut i_renderobj::RenderDevice ) -> Result< (), & 'static str > {
        //starting icosahedron
        let t = ( 1f32 + 5f32.sqrt() ) / 2f32;

        self.add_vertex( &[-1f32,  t,  0f32 ] );
        self.add_vertex( &[ 1f32,  t,  0f32 ] );
        self.add_vertex( &[-1f32, -t,  0f32 ] );
        self.add_vertex( &[ 1f32, -t,  0f32 ] );

        self.add_vertex( &[ 0f32, -1f32,  t ] );
        self.add_vertex( &[ 0f32,  1f32,  t ] );
        self.add_vertex( &[ 0f32, -1f32, -t ] );
        self.add_vertex( &[ 0f32,  1f32, -t ] );

        self.add_vertex( &[ t,  0f32, -1f32 ] );
        self.add_vertex( &[ t,  0f32,  1f32 ] );
        self.add_vertex( &[-t,  0f32, -1f32 ] );
        self.add_vertex( &[-t,  0f32,  1f32 ] );
            
        // create 20f32 triangles of the icosahedron
        let mut faces = vec![];

        // 5 faces around point 0
        faces.push( [0, 11, 5] );
        faces.push( [0, 5, 1] );
        faces.push( [0, 1, 7] );
        faces.push( [0, 7, 10] );
        faces.push( [0, 10, 11] );

        // 5 adjacent faces
        faces.push( [1, 5, 9] );
        faces.push( [5, 11, 4] );
        faces.push( [11, 10, 2] );
        faces.push( [10, 7, 6] );
        faces.push( [7, 1, 8] );

        // 5 faces around point 3
        faces.push( [3, 9, 4] );
        faces.push( [3, 4, 2] );
        faces.push( [3, 2, 6] );
        faces.push( [3, 6, 8] );
        faces.push( [3, 8, 9] );

        // 5 adjacent faces
        faces.push( [4, 9, 5] );
        faces.push( [2, 4, 11] );
        faces.push( [6, 2, 10] );
        faces.push( [8, 6, 7] );
        faces.push( [9, 8, 1] );

        let iterations = 2;
        // refine triangles
        for i in 0..iterations {
            let mut faces2 = vec![];
            for tri in faces.iter() {
                // subdivide triangle into 4
                let a = self.get_middle_point( tri[0], tri[1] );
                let b = self.get_middle_point( tri[1], tri[2] );
                let c = self.get_middle_point( tri[2], tri[0] );
                faces2.push( [ tri[0], a, c ] );
                faces2.push( [ tri[1], b, a ] );
                faces2.push( [ tri[2], c, b ] );
                faces2.push( [ a, b, c ] );
            }
            faces = faces2;
        }

        let mut pos = vec![];
        let mut normal = vec![];
        let mut tc = vec![];
        let r = self._radius;
        let offset = self._pos;
        for i in faces.iter() {
            let x = self._positions[ i[0] ].into_iter().enumerate().map(|(k,o)| o*r + offset[k as u32] ).collect::<Vec<f32> >();
            let y = self._positions[ i[1] ].into_iter().enumerate().map(|(k,o)| o*r + offset[k as u32] ).collect::<Vec<f32> >();
            let z = self._positions[ i[2] ].into_iter().enumerate().map(|(k,o)| o*r + offset[k as u32] ).collect::<Vec<f32> >();

            let n = mat::Mat3x1 { _val: [ z[0]-y[0], z[1]-y[1], z[2]-y[2] ] }.cross( & mat::Mat3x1 { _val: [ x[0]-y[0], x[1]-y[1], x[2]-y[2] ] } ).unwrap().normalize().unwrap();
            pos.extend_from_slice( &x[..] );
            pos.extend_from_slice( &y[..] );
            pos.extend_from_slice( &z[..] );
            normal.extend_from_slice( &n._val[..] );
            normal.extend_from_slice( &n._val[..] );
            normal.extend_from_slice( &n._val[..] );
            tc.extend_from_slice( &[ 0f32; 6 ] );            
        }

        let data_map : HashMap< i_renderobj::BuffDataType, &[f32] > =  [ ( i_renderobj::BuffDataType::POS, pos.as_slice() ),
                                                                             ( i_renderobj::BuffDataType::NORMAL, normal.as_slice() ),
                                                                             ( i_renderobj::BuffDataType::TC, tc.as_slice() ) ].iter().cloned().collect();
        rd.store_buff_data( i_renderobj::RenderObjType::TRI, & data_map );

        println!( "load into render buffer: primitive (sphere(isocahedron)): vertex count:{}", pos.len() / 3 );

        Ok( () )
    }
}

//todo
pub struct Point {
    pub pos: mat::Mat3x1< f32 >,
    pub radius: f32,
}

//todo
pub struct Line {
    pub pos_start: mat::Mat3x1< f32 >,
    pub pos_end: mat::Mat3x1< f32 >,
}

