#[allow(unused_imports)]
use std::ops::Div;
#[allow(unused_imports)]
use std::cmp::Ordering;

#[test]
fn test_mat4(){
    use implement::math::mat::Mat3;
    use implement::math::mat::Mat4;
    use implement::math::mat::Mat4x1;

    {
        let v = Mat4 { _val: [ 10f64; 16 ], _is_row_major: true };
        assert!( v.size().expect("size result invalid") == 16 );
    }        
    {
        let v0 = Mat4 { _val: [ 10f64; 16 ], _is_row_major: true };
        let v1 = Mat4 { _val: [ 10f64; 16 ], _is_row_major: true };
        let v2 = v0.plus(&v1).expect("mat plus invalid");
        assert!( v2.is_equal( &Mat4{ _val: [ 20f64; 16 ], _is_row_major: true }, 0.0001f64 ).expect("mat is_equal invalid") );
    }
    {
        let v0 = Mat4 { _val: [ 10f64; 16 ], _is_row_major: true };
        let v1 = Mat4 { _val: [ 10f64; 16 ], _is_row_major: false };
        assert!( !v0.is_equal( &v1, 0.0001f64 ).expect("mat is_equal invalid") );
    }
    {
        let v0 = Mat4 { _val: [ 10f64; 16 ], _is_row_major: true };
        let v1 = Mat4 { _val: [ 7f64; 16 ], _is_row_major: true };
        let v2 = v0.minus(&v1).expect("mat minus invalid");
        assert!( v2.is_equal( &Mat4{ _val: [ 3f64; 16 ], _is_row_major: true }, 0.0001f64 ).expect("mat is_equal invalid") );
    }
    {
        let v0 = Mat4 { _val: [ 20f64; 16 ], _is_row_major: true };
        let v1 = Mat4 { _val: [ 2f64; 16 ], _is_row_major: true };
        let v2 = v0.div(&v1).expect("mat div invalid");
        assert!( v2.is_equal( &Mat4{ _val: [ 10f64; 16 ], _is_row_major: true }, 0.0001f64 ).expect("mat is_equal invalid") );
    }
    {
        let v0 = Mat4 { _val: [ 10f64; 16 ], _is_row_major: true };
        let v1 = v0.normalize().expect("mat normalize invalid");
        assert!( v1.is_equal( &Mat4{ _val: [ 1f64; 16 ], _is_row_major: true }, 0.0001f64 ).expect("mat is_equal invalid") );
    }
    {
        let v0 = Mat4 { _val: [ 10f64; 16 ], _is_row_major: true };
        let v1 = v0.scale(3f64).expect("mat normalize invalid");
        assert!( v1.is_equal( &Mat4 { _val: [ 30f64; 16 ], _is_row_major: true }, 0.0001f64 ).expect("mat is_equal invalid") );
    }
    {
        let v0 = Mat4 { _val: [ 10f64; 16 ], _is_row_major: true };
        let v1 = Mat4 { _val: [ 10f64; 16 ], _is_row_major: true };
        let a = v0.is_equal( &v1, 0.0001f64 ).expect("mat is_equal invalid");
        assert!( a );
    }
    {
        let v0 = Mat4 { _val: [ 10f64; 16 ], _is_row_major: true };
        let v1 = Mat4 { _val: [ 10.01f64; 16 ], _is_row_major: true };
        let a = v0.is_equal( &v1, 0.0001f64 ).expect("mat is_equal invalid");
        assert!( !a );
    }
    {

        let mut v0 = Mat4 { _is_row_major: true, ..Mat4::default() };
        let mut v1 = Mat4 { _is_row_major: true, ..Mat4::default() };
        for x in 0..16 {
            v0._val[x] = (x + 1) as f64;
        }
        for x in 10..26 {
            v1._val[x-10] = x as f64;
        }
        let v2 = v0.mul(&v1).expect("mat mul invalid");
        let v3 = Mat4{ _val: [ 180f64, 190f64, 200f64, 210f64,
                               436f64, 462f64, 488f64, 514f64,
                               692f64, 734f64, 776f64, 818f64,
                               948f64, 1006f64, 1064f64, 1122f64 ],
                        _is_row_major: true };
        let a = v2.is_equal( &v3, 0.001f64 ).expect("mat mul invalid");
        assert!( a );
    }
    {
        //4x4 mul 4x1
        let v0 = Mat4x1 { _val: [ 1f64, 2f64, 3f64, 4f64 ] };
        let mut v1 = Mat4 { _is_row_major: true, ..Mat4::default() };
        for x in 0..16 {
            v1._val[x] = x as f64;
        }
        let v2 = v1.mul_mat4x1( &v0 ).expect("mat4x4 mul mat4x1 invalid");
        assert!( v2.is_equal( &Mat4x1{ _val: [ 20f64, 60f64, 100f64, 140f64 ] }, 0.0001f64 ).expect("mat4x1 is_equal invalid") );
    }
    {
        //4x4 mul 4x1
        let v0 = Mat4x1 { _val: [ 1f64, 2f64, 3f64, 4f64 ] };
        let mut arr = [0f64; 16];
        for x in 0..16 {
            arr[x] = x as f64;
        }
        let is_row_major = true;
        let v1 = Mat4::init( arr, is_row_major );
        let v2 = v1.mul_mat4x1( &v0 ).expect("mat4x4 mul mat4x1 invalid");
        assert!( v2.is_equal( &Mat4x1{ _val: [ 20f64, 60f64, 100f64, 140f64 ] }, 0.0001f64 ).expect("mat4x1 is_equal invalid") );
    }
    {
        //4x4 mul 4x1
        let v0 = Mat4x1 { _val: [ 1f64, 2f64, 3f64, 4f64 ] };
        let mut v1 = Mat4 { _is_row_major: false, ..Mat4::default() };
        let mut val = 0f64;
        for x in 0..4 {
            for y in 0..4 {
                v1._val[x + y * 4 ] = val;
                val = val + 1f64;
            }
        }
        let v2 = v1.mul_mat4x1( &v0 ).expect("mat4x4 mul mat4x1 invalid");
        assert!( v2.is_equal( &Mat4x1{ _val: [ 20f64, 60f64, 100f64, 140f64 ] }, 0.0001f64 ).expect("mat4x1 is_equal invalid") );
    }
    {
        //4x4 mul 4x1
        let v0 = Mat4x1 { _val: [ 1f64, 2f64, 3f64, 4f64 ] };
        let mut arr = [0f64; 16];
        for x in 0..16 {
            arr[x] = x as f64;
        }
        let is_row_major = false;
        let v1 = Mat4::init( arr, is_row_major );
        let v2 = v1.mul_mat4x1( &v0 ).expect("mat4x4 mul mat4x1 invalid");
        assert!( v2.is_equal( &Mat4x1{ _val: [ 20f64, 60f64, 100f64, 140f64 ] }, 0.0001f64 ).expect("mat4x1 is_equal invalid") );
    }
    {
        //transpose
        let mut arr = [0f64; 16];
        for x in 0..16 {
            arr[x] = x as f64;
        }
        let is_row_major = true;
        let v1 = Mat4::init( arr, is_row_major );
        let v2 = v1.transpose();
        assert!( v2.is_equal( &Mat4{ _val: [0f64, 4f64, 8f64, 12f64,
                                            1f64, 5f64, 9f64, 13f64,
                                            2f64, 6f64, 10f64, 14f64,
                                            3f64, 7f64, 11f64, 15f64], _is_row_major: true }, 0.0001f64 ).expect("mat4 is_equal invalid" ) );
    }
    {
        //transpose self
        let mut arr = [0f64; 16];
        for x in 0..16 {
            arr[x] = x as f64;
        }
        let is_row_major = true;
        let mut v1 = Mat4::init( arr, is_row_major );
        v1.transpose_self();
        assert!( v1.is_equal( &Mat4{ _val: [0f64, 4f64, 8f64, 12f64,
                                            1f64, 5f64, 9f64, 13f64,
                                            2f64, 6f64, 10f64, 14f64,
                                            3f64, 7f64, 11f64, 15f64], _is_row_major: true }, 0.0001f64 ).expect("mat4 is_equal invalid" ) );
    }
    {
        //transpose col major
        let mut arr = [0f64; 16];
        for x in 0..16 {
            arr[x] = x as f64;
        }
        let is_row_major = false;
        let mut v1 = Mat4::init( arr, is_row_major );
        assert!( v1.is_equal( &Mat4{ _val: [0f64, 4f64, 8f64, 12f64,
                                            1f64, 5f64, 9f64, 13f64,
                                            2f64, 6f64, 10f64, 14f64,
                                            3f64, 7f64, 11f64, 15f64], _is_row_major: false }, 0.0001f64 ).expect("mat4 is_equal invalid" ) );
        v1.transpose_self();
        assert!( v1.is_equal( &Mat4{ _val: arr, _is_row_major: false }, 0.0001f64 ).expect("mat4 is_equal invalid" ) );
    }
    {
        //extract upper left 3x3 matrix
        let mut arr = [0f64; 16];
        for x in 0..16 {
            arr[x] = x as f64;
        }
        let is_row_major = true;
        let v1 = Mat4::init( arr, is_row_major );
        let extract = v1.submat_mat3();
        assert!( extract.is_equal( &Mat3{ _val: [0f64, 1f64, 2f64,
                                                 4f64, 5f64, 6f64,
                                                 8f64, 9f64, 10f64, ],
                                           _is_row_major: true }, 0.00001f64 ).expect("mat4 submat failed" ) );
    }
}
