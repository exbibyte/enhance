use std::boxed::Box;
use std::f64;
use std::u64;

use interface::i_spatial_accel::ISpatialAccel;
use interface::i_bound::{ IBound, BoundType };

use implement::math::bound::AxisAlignedBBox;

/// implementation of sptial acceleration using bounding volume hierarchy
pub struct Bvh {
    _root: Box< NodeBvh >,
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
}

impl ISpatialAccel for Bvh {
    fn query( input: & IBound ) -> Vec< u64 > {
        unimplemented!();
    }
    fn build( objs: &[ (u64, &IBound) ] ) -> Result< (), & 'static str > {
        for i in objs {
            match i.1.get_type() {
                BoundType::AxisAlignBox => (),
                _ => return Err( "unsupported bound type" )
            }
        }
        unimplemented!();
    }
}
