#[allow(unused_imports)]
use std::ops::Div;
#[allow(unused_imports)]
use std::cmp::Ordering;

#[test]
fn test_mat1x3(){
    use implement::math::mat::Mat1x3;
    use implement::math::mat::Mat3;
    {
        let v = Mat1x3 { _val: [ 10f64; 3 ] };
        assert!( v.size().expect("size result invalid") == 3 );

        let v2 = Mat1x3 { _val: [ 2f64; 3 ] };

        {
            let v3 = v.mul_elem(&v2).expect("mul result invalid");   
            assert!( v3.is_equal( &Mat1x3{ _val: [ 20f64; 3 ] }, 0.0001f64 ).expect("Mat1x3 is_equal invalid") );
        }
        {
            let v3 = v.div(&v2).expect("div result invalid");  
            assert!( v3.is_equal( &Mat1x3{ _val: [ 5f64; 3 ] }, 0.0001f64 ).expect("Mat1x3 is_equal invalid") );
        }
        {
            let v3 = v.plus(&v2).expect("plus result invalid");  
            assert!( v3.is_equal( &Mat1x3{ _val: [ 12f64; 3 ] }, 0.0001f64 ).expect("Mat1x3 is_equal invalid") );
        }
        {
            let v3 = v.minus(&v2).expect("plus result invalid");  
            assert!( v3.is_equal( &Mat1x3{ _val: [ 8f64; 3 ] }, 0.0001f64 ).expect("Mat1x3 is_equal invalid") );
        }

        assert!( v2.magnitude().expect("magnitude invalid") == ( (2f64 * 2f64) * 3f64).sqrt() );

        let v4 = v2.normalize().expect("normalize invalid");
        assert!( v4.is_equal( &Mat1x3{ _val: [ 2f64.div( (2f64 * 2f64 * 3f64 ).sqrt() ); 3 ] }, 0.0001f64 ).expect("Mat1x3 is_equal invalid") );

        let v5 = v2.scale( 4f64 ).expect("scale invalid");
        assert!( v5.is_equal( &Mat1x3{ _val: [ 8f64; 3 ] }, 0.0001f64 ).expect("Mat1x3 is_equal invalid") );

        let v6 = Mat1x3 { _val: [ 5.0f64, 5.3f64, 7.7f64 ] };
        let v7 = Mat1x3 { _val: [ 5.1f64, 5.4f64, 7.8f64 ] };
        assert!( v6.is_equal( &v7, 0.1001f64 ).expect("Mat1x3 is_equal invalid") );
        assert!( v6.is_equal( &v7, 0.0999f64 ).expect("Mat1x3 is_equal invalid") == false );
    }
    {
        //mat1x3 mul mat3x3
        let v2 = Mat1x3 { _val: [ 1f64, 2f64, -4f64 ] };
        let mut arr = [0f64; 9];
        for x in 0..9 {
            arr[x] = x as f64;
        }
        let is_row_major = true;
        let v1 = Mat3::init( arr, is_row_major );
        let v3 = v2.mul_mat3x3( &v1 ).expect("mat1x3 mul mat3x3 invalid");
        assert!( v3.is_equal( &Mat1x3{ _val: [-18f64, -19f64, -20f64] }, 0.00001f64 ).expect("mat1x4 is_equal invalid") );   
    }
}
