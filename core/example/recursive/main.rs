use std::cell::RefCell;

struct NodeInterior {
    pub _num: i32,
}

impl Default for NodeInterior {
    fn default() -> NodeInterior {
        NodeInterior  {
            _num: 0i32,
        }
    }
}

impl NodeInterior {
    pub fn add( & mut self, count: i32 ) -> i32 {
        if count == 0 {
            return self._num
        }
        self._num += 1;
        self.add( count-1 )
    }
}

struct Node {
    pub _node_interior: RefCell< NodeInterior >,
}
impl Node {
    pub fn add( & self, count: i32 ) -> i32 {
        if count == 0 {
            return self._node_interior.borrow_mut()._num
        }
        self._node_interior.borrow_mut().add( count );
        self.add( count-1 )
    }
}

fn main(){
    let n = Node {
        _node_interior: RefCell::new( Default::default() ),
    };
    let v = n.add(2);
    println!("{}", v );
}
