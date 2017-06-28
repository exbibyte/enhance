#[test]
fn test_vector(){
    use implement::math::vector::Vector3;
    use interface::i_math_transform::*;

    let v = Vector3 { _val: [ 5u32; 3 ] };
    assert!( v.size() == 3 );

    let v2 = Vector3 { _val: [ 7u32; 3 ] };

    let v3 = v.dot(&v2);

    println!( "{:?}", v3._val );
    
    use std::cmp::Ordering;
    assert!( v3._val.cmp( &[ 35u32; 3 ] ) == Ordering::Equal );
}
