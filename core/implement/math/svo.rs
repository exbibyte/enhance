///this is useful for a number of cases such as intersection testing and voxel rendering
use std::boxed::Box;
use std::f64;
use std::u64;

use interface::i_spatial_accel::ISpatialAccel;
use interface::i_bound::{ IBound, BoundType };
use interface::i_stat_tree::IStatTree;

use implement::math::bound::AxisAlignedBBox;

#[derive(Debug, Clone)]
pub struct Svo {
    _root: NodeSvo,
}

///internal node structure for Svo
#[derive(Debug, Clone)]
pub struct NodeSvo {
    _bound: AxisAlignedBBox,
    _branches: Vec< SvoBranch >,
    _obj: u64, //leaf data
}

#[derive(Debug, Clone)]
pub enum SvoBranch {
    CHILD(Box< NodeSvo >),
    EMPTY,
}

impl Default for NodeSvo {
    fn default() -> NodeSvo {
        NodeSvo {
            _bound: AxisAlignedBBox {
                _bound_lower: [ f64::NEG_INFINITY; 3 ],
                _bound_upper: [ f64::INFINITY; 3 ],
            },
            _branches: vec![ SvoBranch::EMPTY; 8 ],
            _obj: u64::MAX,
        }
    }
}

impl NodeSvo {
    pub fn init_branches( b: AxisAlignedBBox, branches: Vec< SvoBranch > ) -> NodeSvo {
        NodeSvo {
            _bound: b,
            _branches: branches,
            _obj: u64::MAX,
        }
    }
    pub fn init_leaf( b: AxisAlignedBBox, o: u64 ) -> NodeSvo {
        NodeSvo {
            _bound: b,
            _branches: vec![ SvoBranch::EMPTY; 8 ],
            _obj: o,
        }
    }
    pub fn build_node( & mut self, objs: &[ (u64, &IBound) ] ) -> Result< (), & 'static str > {
        for i in objs {
            match i.1.get_type() {
                BoundType::AxisAlignBox => (),
                _ => { return Err( "unsupported bound type" ) },
            }
        }
        let b = objs.iter().cloned().map( |x| x.1 ).collect::<Vec< &IBound > >();
        let mut u : AxisAlignedBBox = Default::default();
        u.get_union( &b[..] );

        // println!( "bound union: {:?}", u );

        //check for leaf condition
        if objs.len() == 1 {
            self._bound = u;
            self._obj = objs[0].0;
            return Ok( () )
        }

        unimplemented!();
    }
    pub fn search< F >( _n : & NodeSvo, _b: & IBound, mut _f : F ) where F : FnMut( u64 ) -> ()
    {
        unimplemented!();
    }
}

impl Svo {
    pub fn init() -> Svo {
        Svo {
            _root: NodeSvo {
                _bound: AxisAlignedBBox {
                    _bound_lower: [ f64::NEG_INFINITY; 3 ],
                    _bound_upper: [ f64::INFINITY; 3 ],
                },
                _branches: vec![ SvoBranch::EMPTY; 8 ],
                _obj: u64::MAX,
            },
        }
    }
}

impl ISpatialAccel for Svo {
    fn query_intersect( & self, input: &IBound ) -> Result< Vec< u64 >, & 'static str >
    {
        match input.get_type() {
            BoundType::AxisAlignBox => (),
            _ => { return Err( "unsupported bound type" ) },
        }
        let mut out = vec![];
        {
            let func_collect = | x | { out.push( x ); () };
            NodeSvo::search( &self._root, input, func_collect );
        }
        Ok( out )
    }
    fn build_all( & mut self, objs: &[ (u64, &IBound) ] ) -> Result< (), & 'static str >
    {
        self._root.build_node( objs )
    }
}

impl IStatTree for Svo {
    fn sum_subtree_child_count( & self ) -> Option< u64 > {
        unimplemented!();
    }
    fn node_degree( & self ) -> Option< u64 > {
        unimplemented!();
    }
    fn max_depth( & self ) -> Option< u64 > {
        unimplemented!();
    }
    fn mean_depth( & self ) -> Option< u64 > {
        unimplemented!();
    }
    fn min_depth( & self ) -> Option< u64 > {
        unimplemented!();
    }
    fn balance_ratio( & self ) -> Option< f64 > {
        unimplemented!();
    }
}
