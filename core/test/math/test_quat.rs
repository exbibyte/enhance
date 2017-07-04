#[allow(unused_imports)]
use std::ops::Div;
#[allow(unused_imports)]
use std::cmp::Ordering;

#[test]
fn test_quat(){
    use implement::math::mat::Mat3;
    use implement::math::mat::Mat3x1;
    use implement::math::mat::Mat4;
    use implement::math::mat::Mat4x1;
    use implement::math::quat::Quat;
    use std::f64::consts::PI;
    {
        //convert axis angle to quaternion representation and back
        let axis = Mat3x1 { _val: [ 1f64, 2f64, 3f64 ] };
        let axis_normalize = axis.normalize().expect("normalize invalid");
        let q = Quat::init_from_axis_angle_degree( (axis, 90f64) );
        let ( axis, angle ) = q.to_axis_angle();
        assert!( axis.is_equal( &axis_normalize, 0.0001f64 ).expect("is_equal invalid") );
        assert!( angle/PI*180f64 == 90f64 );            
    }
    {
        //compute rotation using quaternion
        //rotate a vector using the rotation matrix and compare to rotation using quaternions
        let p = Mat3x1 { _val: [ 1f64, 5f64, -3f64 ] };

        let axis = Mat3x1 { _val: [ 1f64, 0f64, 0f64 ] };
        let axis_normalize = axis.normalize().expect("normalize invalid");
        let q = Quat::init_from_axis_angle_degree( (axis, 90f64) );
        let ( axis, angle ) = q.to_axis_angle();
        assert!( axis.is_equal( &axis_normalize, 0.0001f64 ).expect("is_equal invalid") );
        assert!( angle/PI*180f64 == 90f64 );

        let row_major = true;
        let rot = q.to_rotation_matrix( row_major );

        assert!( rot.is_equal( &Mat4 { _val: [1f64,0f64,0f64,0f64,
                                              0f64,0f64,-1f64,0f64,
                                              0f64,1f64,0f64,0f64,
                                              0f64,0f64,0f64,1f64], _is_row_major: true }, 0.0001f64 ).expect("rotation matrix is_equal invalid") );

        let ans_1 = rot.mul_mat4x1( &Mat4x1 { _val: [ p._val[0], p._val[1], p._val[2], 1f64 ] } ).expect("rotation mul p invalid");
        let ans_2 = q.rotate_vector( p );
        assert!( ans_2.is_equal( &Mat3x1 { _val: [ ans_1._val[0], ans_1._val[1], ans_1._val[2] ] }, 0.00001f64 ).expect("rotations is_equal invalid" ) );
    }
}
