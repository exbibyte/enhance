#[cfg(test)]
use interface::i_spatial_accel::ISpatialAccel;
use interface::i_bound::IBound;
use interface::i_shape::ShapeType;
use implement::math::bvh::Bvh;
use implement::math::bound::AxisAlignedBBox;
use implement::math::bound_sphere::BoundSphere;

#[test]
fn test_bvh_supported_bounds(){
    let mut a = Bvh::init( 10 ); //bin count of 10
    let aabb = AxisAlignedBBox::init( ShapeType::SPHERE, &[ 0f64, 0f64, 0f64, 5f64 ] );
    let objs =  [ ( 0u64, &aabb as &IBound ) ];
    match a.build_all( &objs[..] ) {
        Ok( () ) => (),
        _ => { panic!("unexpected result for supported bound type"); },
    }
}

#[test]
fn test_bvh_unsupported_bounds(){
    let mut a = Bvh::init( 10 );
    let aabb = BoundSphere::init( ShapeType::SPHERE, &[ 0f64, 0f64, 0f64, 5f64 ] );
    let objs =  [ ( 0u64, &aabb as &IBound ) ];
    match a.build_all( &objs[..] ) {
        Err(_) => (),
        _ => { panic!("unexpected result for unsupported bound type"); },
    }
}

#[test]
fn test_bvh_construction(){
    let mut a = Bvh::init( 30 );
    let mut bounds = vec![];
    let mut bound_refs = vec![];
    for i in 0..20 {
        let aabb = AxisAlignedBBox::init( ShapeType::SPHERE, &[ i as f64, i as f64, i as f64, 5f64 ] );
        bounds.push( aabb );
    }
    for i in 0..20 {
        bound_refs.push( ( i as u64, &bounds[i] as &IBound ) );
    }
    
    match a.build_all( &bound_refs[..] ) {
        Ok( () ) => (),
        _ => { panic!("unexpected result for supported bound type"); },
    }
    
}
