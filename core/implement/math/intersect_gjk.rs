//based on reference tutorial from http://www.dyn4j.org/2010/04/gjk-gilbert-johnson-keerthi/

use implement::math::mat::Mat3x1;
use interface::i_shape::{ IShape, ShapeType };

fn support( a: & IShape, b: & IShape, v: & Mat3x1<f64> ) -> Option< Mat3x1< f64 > > {
    let p0 = match a.get_support( &v ){
        Some( o ) => {
            o
        },
        _ => { return None },
    };
    let v_oppose = v.scale( -1f64 ).unwrap();
    let p1 = match b.get_support( &v_oppose ){
        Some( o ) => {
            o
        },
        _ => { return None },
    };
    let p10 = p0.minus( &p1 ).unwrap();
    Some( p10 )
}

fn pass_minkowski_origin( last_vert: & Mat3x1< f64 >, support: & Mat3x1< f64 > ) -> bool {
    // println!( "last vert dot product: {}", last_vert.dot( &support ).unwrap() );
    last_vert.dot( &support ).unwrap() > 0f64
}

fn contains_minkowski_origin( simplex : & mut Vec< Mat3x1< f64 > >, support: & mut Mat3x1< f64 > ) -> bool {
    
    let a = simplex.last().unwrap().clone();
    let ao = a.scale( -1f64 ).unwrap();
    if simplex.len() == 3 { //triangle case
        let b = simplex[1];
        let c = simplex[0];
        let ab = b.minus( & a ).unwrap();
        let ac = c.minus( & a ).unwrap();
        let ab_normal = ac.cross( & ab ).unwrap().cross( & ab ).unwrap();
        let ac_normal = ab.cross( & ac ).unwrap().cross( & ac ).unwrap();
        if ab_normal.dot( & ao ).unwrap() > 0f64 {
            //remove c and set new direction to ab_normal
            let simplex_new = vec![ simplex[1], simplex[2] ];
            *simplex = simplex_new;
            *support =  ab_normal.clone();
        } else if ac_normal.dot( & ao ).unwrap() > 0f64 {
            //remove b and set new direction to ac_normal
            let simplex_new = vec![ simplex[0], simplex[2] ];
            *simplex = simplex_new.clone();
            *support =  ac_normal.clone();
        } else { //minkowski origin is enclosed by the triangle
            return true
        }
    } else { //line segment case
        //set direction towards minkowski origin
        let b = simplex[0];
        let ab = b.minus( & a ).unwrap();
        let ab_normal = ab.cross( & ao ).unwrap().cross( & ao ).unwrap();
        if ab_normal.magnitude().unwrap() == 0f64 {
            return true
        } else {
            *support = ab_normal.clone();
        }
    }
    return false
}

pub fn query_intersect( a: &IShape, b: &IShape ) -> Option< bool > {
    match a.get_type() {
        ShapeType::SPHERE => {},
        _ => { panic!("unsupported shape type"); },
    }
    match b.get_type() {
        ShapeType::SPHERE => {},
        _ => { panic!("unsupported shape type"); },
    }
    //set initial minkowski vertex from an arbitrary support vector
    let mut d = Mat3x1 { _val: [ -1f64, 0f64, 0f64 ] };
    let mut simplex = vec![];
    {
        let sup = support( a, b, &d ).unwrap();
        simplex.push( sup );
    }
    
    d = d.scale( -1f64 ).unwrap();
    loop {
        // println!( "support vector: {:?}", d );
        {
            let sup = support( a, b, &d ).unwrap();
            simplex.push( sup );
        }
        assert!( simplex.len() <= 3, "simplex vertices count unexpected" );
        // println!( "simplex len: {}", simplex.len() );
        if !pass_minkowski_origin( simplex.last().unwrap(), & d ) {
            // println!( "new vert not pass origin" );
            return Some( false )
        } else {
            if contains_minkowski_origin( & mut simplex, & mut d ) {
                return Some( true )
            }
        }
    }
}
