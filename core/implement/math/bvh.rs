use std::boxed::Box;
use std::f64;
use std::u64;

use interface::i_spatial_accel;
use interface::i_bound::{ IBound, BoundType };

use implement::math::bound::AxisAlignedBBox;

/// implementation of spatial acceleration using bounding volume hierarchy with surface area heuristic
pub struct Bvh {
    _root: NodeBvh,
    _bins: u32,
}

///internal node structure for Bvh
pub struct NodeBvh {
    _bound: AxisAlignedBBox,    
    _left: BvhBranch,
    _right: BvhBranch,
    _obj: u64, //leaf data
}

pub enum BvhBranch {
    CHILD(Box< NodeBvh >),
    EMPTY,
}

impl Default for NodeBvh {
    fn default() -> NodeBvh {
        NodeBvh {
            _bound: AxisAlignedBBox {
                _bound_lower: [ f64::NEG_INFINITY; 3 ],
                _bound_upper: [ f64::INFINITY; 3 ],
            },
            _left: BvhBranch::EMPTY,
            _right: BvhBranch::EMPTY,
            _obj: u64::MAX,
        }
    }
}

impl NodeBvh {
    pub fn init_branches( b: AxisAlignedBBox, l: BvhBranch, r: BvhBranch ) -> NodeBvh {
        NodeBvh {
            _bound: b,
            _left: l,
            _right: r,
            _obj: u64::MAX,
        }
    }
    pub fn init_leaf( b: AxisAlignedBBox, o: u64 ) -> NodeBvh {
        NodeBvh {
            _bound: b,
            _left: BvhBranch::EMPTY,
            _right: BvhBranch::EMPTY,
            _obj: o,
        }
    }
    pub fn build_node( & mut self, num_bins: u32, objs: &[ (u64, &IBound) ] ) -> Result< (), & 'static str > {
        for i in objs {
            match i.1.get_type() {
                BoundType::AxisAlignBox => (),
                _ => return Err( "unsupported bound type" )
            }
        }
        let b = objs.iter().cloned().map( |x| x.1 ).collect::<Vec< &IBound > >();
        let mut u : AxisAlignedBBox = Default::default();
        u.get_union( &b[..] );

        println!( "bound union: {:?}", u );

        //check for leaf condition
        if objs.len() == 1 {
            self._bound = u;
            self._obj = objs[0].0;
            return Ok( () )
        }

        //todo: execute alternative branch when number of objects are low
        
        //get the longest axis of the bounding box
        let ( ref axis, ref length ) = u.get_longest_axis();

        self._bound = u;

        println!( "axis length: {}", length );
        
        let c = self._bound.get_centroid();
        let bin_len = length / num_bins as f64;

        let mut bin_offsets = vec![];
        for i in 0..num_bins {
            match axis {
                X => bin_offsets.push( i as f64 * bin_len + self._bound._bound_lower[0] ),
                Y => bin_offsets.push( i as f64 * bin_len + self._bound._bound_lower[1] ),
                Z => bin_offsets.push( i as f64 * bin_len + self._bound._bound_lower[2] ),
            }
        }

        //bin objects using their centroids in the direction of the longest axis
        let mut bins = vec![ 0; num_bins as usize ];
        let mut obj_bin = vec![];
        let epsilon = 0.001f64;
        for i in objs {
            let c = i.1.get_centroid();
            println!("centroid: {:?}", c );
            let bin_id = match axis {
                X => {
                    ( ( c[0] - self._bound._bound_lower[0] ) / length * (num_bins as f64) * ( 1.0 - epsilon ) ) as usize
                },
                Y => {
                    ( ( c[1] - self._bound._bound_lower[1] ) / length * (num_bins as f64) * ( 1.0 - epsilon ) ) as usize
                },
                Z => {
                    ( ( c[2] - self._bound._bound_lower[2] ) / length * (num_bins as f64) * ( 1.0 - epsilon ) ) as usize
                },
            };
            if bin_id >= ( num_bins as usize ) {
                println!("bin_id: {}, bin_len: {}", bin_id, num_bins );
                assert!( bin_id < ( num_bins as usize ) );
            }
            bins[bin_id] += 1;
            obj_bin.push( bin_id );
        }

        println!( "bins: {:?}", bins );
        
        let m = bins.iter().enumerate().max_by_key( | &x | x.1 ).unwrap();
        let split_bin_idx = m.0;

        println!( "split_bin_idx: {}", split_bin_idx );

        let mut bin_left : Vec< ( u64, &IBound ) > = vec![];
        let mut bin_right : Vec< ( u64, &IBound ) > = vec![];
        let mut idx_obj = 0;
        for i in obj_bin {
            let obj = objs[ idx_obj ];
            if i < split_bin_idx {
                bin_left.push( obj );
            } else {
                bin_right.push( obj );
            }
            idx_obj += 1;
        }

        if bin_left.len() > 0 {
            let mut l : NodeBvh = Default::default();
            println!("num left children: {}", bin_left.len() );
            l.build_node( num_bins, &bin_left[..] );
            self._left = BvhBranch::CHILD( Box::new( l ) );
        } else {
            self._left = BvhBranch::EMPTY;
        }

        if bin_right.len() > 0 {
            let mut r : NodeBvh = Default::default();
            println!("num right children: {}", bin_right.len() );
            r.build_node( num_bins, &bin_right[..] );
            self._right = BvhBranch::CHILD( Box::new( r ) );
        } else {
            self._right = BvhBranch::EMPTY;
        }

        Ok( () )
    }
}

impl i_spatial_accel::ISpatialAccel for Bvh {
    fn query( & self, input: &IBound ) -> Vec< u64 >
    {
        unimplemented!();
    }
    fn build_all( & mut self, objs: &[ (u64, &IBound) ] ) -> Result< (), & 'static str >
    {
        //initiate top down construction
        self._root.build_node( self._bins, objs )
    }
}

impl Bvh {
    pub fn init( bins: u32 ) -> Bvh {
        assert!( bins != 0 );
        Bvh {
            _root: NodeBvh {
                _bound: AxisAlignedBBox {
                    _bound_lower: [ f64::NEG_INFINITY; 3 ],
                    _bound_upper: [ f64::INFINITY; 3 ],
                },
                _left: BvhBranch::EMPTY,
                _right: BvhBranch::EMPTY,
                _obj: u64::MAX,
            },
            _bins: bins,
        }
    }
}
