#[allow(unused_imports)]
use std::ops::Div;
#[allow(unused_imports)]
use std::cmp::Ordering;

use interface::i_comparable::IComparableError;

#[test]
fn test_mat4(){
    use implement::math::mat::Mat3;
    use implement::math::mat::Mat3x1;

    {
        let v = Mat3 { _val: [ 10f64; 9 ], _is_row_major: true };
        assert!( v.size().expect("size result invalid") == 9 );
    }        
    {
        let v0 = Mat3 { _val: [ 10f64; 9 ], _is_row_major: true };
        let v1 = Mat3 { _val: [ 10f64; 9 ], _is_row_major: true };
        let v2 = v0.plus(&v1).expect("mat plus invalid");
        assert!( v2.is_equal( &Mat3{ _val: [ 20f64; 9 ], _is_row_major: true }, 0.0001f64 ).expect("mat is_equal invalid") );
    }
    {
        let v0 = Mat3 { _val: [ 10f64; 9 ], _is_row_major: true };
        let v1 = Mat3 { _val: [ 10f64; 9 ], _is_row_major: false };
        assert!( !v0.is_equal( &v1, 0.0001f64 ).expect("mat is_equal invalid") );
    }
    {
        let v0 = Mat3 { _val: [ 10f64; 9 ], _is_row_major: true };
        let v1 = Mat3 { _val: [ 7f64; 9 ], _is_row_major: true };
        let v2 = v0.minus(&v1).expect("mat minus invalid");
        assert!( v2.is_equal( &Mat3{ _val: [ 3f64; 9 ], _is_row_major: true }, 0.0001f64 ).expect("mat is_equal invalid") );
    }
    {
        let v0 = Mat3 { _val: [ 20f64; 9 ], _is_row_major: true };
        let v1 = Mat3 { _val: [ 2f64; 9 ], _is_row_major: true };
        let v2 = v0.div(&v1).expect("mat div invalid");
        assert!( v2.is_equal( &Mat3{ _val: [ 10f64; 9 ], _is_row_major: true }, 0.0001f64 ).expect("mat is_equal invalid") );
    }
    {
        let v0 = Mat3 { _val: [ 10f64; 9 ], _is_row_major: true };
        let v1 = v0.normalize().expect("mat normalize invalid");
        assert!( v1.is_equal( &Mat3{ _val: [ 1f64; 9 ], _is_row_major: true }, 0.0001f64 ).expect("mat is_equal invalid") );
    }
    {
        let v0 = Mat3 { _val: [ 10f64; 9 ], _is_row_major: true };
        let v1 = v0.scale(3f64).expect("mat normalize invalid");
        assert!( v1.is_equal( &Mat3 { _val: [ 30f64; 9 ], _is_row_major: true }, 0.0001f64 ).expect("mat is_equal invalid") );
    }
    {
        let v0 = Mat3 { _val: [ 10f64; 9 ], _is_row_major: true };
        let v1 = Mat3 { _val: [ 10f64; 9 ], _is_row_major: true };
        let a = v0.is_equal( &v1, 0.0001f64 ).expect("mat is_equal invalid");
        assert!( a );
    }
    {
        let v0 = Mat3 { _val: [ 10f64; 9 ], _is_row_major: true };
        let v1 = Mat3 { _val: [ 10.01f64; 9 ], _is_row_major: true };
        let a = v0.is_equal( &v1, 0.0001f64 ).expect("mat is_equal invalid");
        assert!( !a );
    }
    {

        let mut v0 = Mat3 { _is_row_major: true, ..Mat3::default() };
        let mut v1 = Mat3 { _is_row_major: true, ..Mat3::default() };
        for x in 0..9 {
            v0._val[x] = x as f64;
        }
        for x in 10..19 {
            v1._val[x-10] = x as f64;
        }
        let v2 = v0.mul(&v1).expect("mat mul invalid");
        let v3 = Mat3{ _val: [ 45f64, 48f64, 51f64,
                               162f64, 174f64, 186f64,
                               279f64, 300f64, 321f64 ],
                        _is_row_major: true };
        let a = v2.is_equal( &v3, 0.0001f64 ).expect("mat mul invalid");
        assert!( a );
    }
    {
        //3x3 mul 3x1
        let v0 = Mat3x1 { _val: [ 1f64, 2f64, 3f64 ] };
        let mut arr = [0f64; 9];
        for x in 0..9 {
            arr[x] = x as f64;
        }
        let is_row_major = true;
        let v1 = Mat3::<f64>::init( arr, is_row_major );
        let v2 = v1.mul_mat3x1( &v0 ).expect("mat3x3 mul mat3x1 invalid");
        assert!( v2.is_equal( &Mat3x1{ _val: [ 8f64, 26f64, 44f64 ] }, 0.0001f64 ).expect("mat3x1 is_equal invalid") );
    }
    {
        //3x3 mul 3x1
        let v0 = Mat3x1 { _val: [ 1f64, 2f64, 3f64 ] };
        let mut arr = [0f64; 9];
        for x in 0..9 {
            arr[x] = x as f64;
        }
        let is_row_major = false;
        let v1 = Mat3::<f64>::init( arr, is_row_major );
        let v2 = v1.mul_mat3x1( &v0 ).expect("mat3x3 mul mat3x1 invalid");
        assert!( v2.is_equal( &Mat3x1{ _val: [ 8f64, 26f64, 44f64 ] }, 0.0001f64 ).expect("mat3x1 is_equal invalid") );
    }
    {
        //transpose
        let mut arr = [0f64; 9];
        for x in 0..9 {
            arr[x] = x as f64;
        }
        let is_row_major = true;
        let v1 = Mat3::<f64>::init( arr, is_row_major );
        let v2 = v1.transpose();
        assert!( v2.is_equal( &Mat3{ _val: [0f64, 3f64, 6f64,
                                            1f64, 4f64, 7f64,
                                            2f64, 5f64, 8f64], _is_row_major: true }, 0.0001f64 ).expect("mat3 is_equal invalid" ) );
    }
    {
        //identity
        let v1 = Mat3::<f64>::iden();
        assert!( v1.is_equal( &Mat3{ _val: [1f64, 0f64, 0f64,
                                            0f64, 1f64, 0f64,
                                            0f64, 0f64, 1f64], _is_row_major: true }, 0.0001f64 ).expect("mat3 is_equal invalid" ) );
    }
}
