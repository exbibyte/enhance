use std::collections::HashMap;
use std::collections::HashSet;

pub fn reverse_graph( rel: &[(i64,i64)], rel_rev: & mut [(i64,i64)] ) {
    for (i,v) in rel.iter().enumerate() {
        rel_rev[i] = ( v.1, v.0 );
    }
}

pub fn adj_list( rel: &[(i64,i64)] ) -> HashMap< i64, HashSet< i64 > > {
    let mut rev_adj = HashMap::new();
    for i in rel.iter() {
        let exists = rev_adj.contains_key(&i.0);
        if !exists {
            let mut new_set = HashSet::new();
            new_set.insert( i.1 );
            rev_adj.insert( i.0, new_set );
        } else {
            match rev_adj.get_mut( &i.0 ) {
                Some(x) => {
                    x.insert( i.1 );
                },
                None => {
                    // panic!("unexpected result");
                },
            };
        }
    }
    rev_adj
}

pub fn visit_post_order( nodes: &[i64], adj: & HashMap< i64, HashSet< i64 > > ) -> Vec<i64> {
    let mut visited = vec![ false; nodes.len() ];
    let mut queue = vec![];
    let mut visit_order = vec![];
    for i in nodes.iter(){
        if visited[ *i as usize ] == false {
            visited[ *i as usize ] = true;
            queue.push(*i);
        }
        loop {
            let n = match queue.last() {
                Some(n) => {  
                    *n
                },
                _ => { break; },
            };
            let mut found = false;
            if let Some( y ) = adj.get( &n ) {
                for k in y.iter() {
                    if visited[*k as usize ] == false {
                        visited[*k as usize ] = true;
                        queue.push(*k);
                        found = true;
                    }
                }
            };
            if found {
                continue;
            }
            visit_order.push( n );
            queue.pop();
        }
    }
    visit_order
}

pub fn compute( nodes: &[i64], rel: &[(i64,i64)], out: & mut [i64] ) {
    //computes strongly connected components using Tarjan's algorithm
    let adj = adj_list( &rel[..] );
    // println!( "adj: {:?}", adj );
    
    //get post order of the graph
    let visit_order = { let mut a = visit_post_order( nodes, &adj ); a.reverse(); a };
    // println!( "visit order: {:?}", visit_order );
    
    //get transpose graph
    let mut rel_rev = vec![ (0i64, 0i64); rel.len() ];
    reverse_graph( rel, & mut rel_rev[..] );
    // println!( "reverse rel: {:?}", rel_rev );
    let rev_adj = adj_list( &rel_rev[..] );
    // println!( "reverse adj lists: {:?}", rev_adj );

    //compute the ssc components
    let mut component_id = 0;
    let mut node_to_component = HashMap::new();
    for i in visit_order.iter() {
        // println!( "visit: {}", i );
        if !node_to_component.contains_key(i) {
            component_id += 1;
            let mut queue = vec![i];
            loop {
                let n = match queue.last() {
                    Some(n) => {  
                        *n
                    },
                    _ => { break; },
                };
                let mut found = false;
                if !node_to_component.contains_key( n ) {
                    node_to_component.insert( *n, component_id as i64 );
                    found = true;
                    if let Some( y ) = rev_adj.get( n ) {
                        for k in y.iter() {
                            queue.push(k);
                        }
                    };
                }
                if !found {
                    queue.pop();
                }
            }
        }
    }

    for (k, v) in node_to_component {
        out[ k as usize ] = v;
    }
}
