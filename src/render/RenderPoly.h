#ifndef RENDER_POLY_H
#define RENDER_POLY_H

#include "AssetManager.h"
#include "DualQuat.hpp"
#include "Mat.hpp"

#include <vector>
#include <cassert>
#include <type_traits>

class RenderPolyData {
public:
    struct Coordinate {
        using Type = std::vector<double>;
        using Size = std::integral_constant<int, 3>;
    };
    struct RotationAxis {
        using Type = std::vector<double>;
        using Size = std::integral_constant<int, 3>;
    };
    struct RotationAngle {
        using Type = std::vector<double>;
        using Size = std::integral_constant<int, 1>;
    };
};

class RenderPoly : public AssetManager< RenderPolyData::Coordinate,    RenderPolyData::Coordinate::Type >,
		   public AssetManager< RenderPolyData::RotationAxis,  RenderPolyData::RotationAxis::Type >,
		   public AssetManager< RenderPolyData::RotationAngle, RenderPolyData::RotationAngle::Type >
{
public:
    using AssetManager< RenderPolyData::Coordinate,    RenderPolyData::Coordinate::Type >::AddDataSingle;
    using AssetManager< RenderPolyData::RotationAxis,  RenderPolyData::RotationAxis::Type >::AddDataSingle;
    using AssetManager< RenderPolyData::RotationAngle, RenderPolyData::RotationAngle::Type >::AddDataSingle;
    
    using AssetManager< RenderPolyData::Coordinate,    RenderPolyData::Coordinate::Type >::GetDataSingle;
    using AssetManager< RenderPolyData::RotationAxis,  RenderPolyData::RotationAxis::Type >::GetDataSingle;
    using AssetManager< RenderPolyData::RotationAngle, RenderPolyData::RotationAngle::Type >::GetDataSingle;

    bool CheckIsValid(){
        RenderPolyData::Coordinate::Type check_coordinate;
        if( !GetDataSingle( RenderPolyData::Coordinate(), check_coordinate ) || RenderPolyData::Coordinate::Size::value > check_coordinate.size() ){
            assert( 0 && "Poly Coordinate not valid" );
        }
        RenderPolyData::RotationAxis::Type check_rotation_axis;
        if( !GetDataSingle( RenderPolyData::RotationAxis(), check_rotation_axis ) || RenderPolyData::RotationAxis::Size::value > check_rotation_axis.size() ){
            assert( 0 && "Poly RotationAxis not valid" );
        }
	RenderPolyData::RotationAngle::Type check_rotation_angle;
        if( !GetDataSingle( RenderPolyData::RotationAngle(), check_rotation_angle ) || RenderPolyData::RotationAngle::Size::value > check_rotation_angle.size() ){
            assert( 0 && "Poly RotationAngle not valid" );
        }
        return true;
    }
    bool Compute( RenderPolyData::Coordinate::Type & data_coordinate,
		  RenderPolyData::RotationAxis::Type & data_rotation_axis,
		  RenderPolyData::RotationAngle::Type & data_rotation_angle,
		  Mat & orientation_matrix ){
        if( !CheckIsValid() ){
            return false;
        }

        if( !GetDataSingle( RenderPolyData::Coordinate(), data_coordinate ) )          return false;
	if( !GetDataSingle( RenderPolyData::RotationAxis(), data_rotation_axis ) )     return false;
	if( !GetDataSingle( RenderPolyData::RotationAngle(), data_rotation_angle ) )   return false;

	float translation[3];
	float rotation_axis[3];
	float rotation_angle;
	translation[0] = (float) data_coordinate[0];
	translation[1] = (float) data_coordinate[1];
	translation[2] = (float) data_coordinate[2];
	rotation_axis[0] = (float) data_rotation_axis[0];
	rotation_axis[1] = (float) data_rotation_axis[1];
	rotation_axis[2] = (float) data_rotation_axis[2];
	rotation_angle = (float) data_rotation_angle[0];
	
	DualQuat dual_quat; 
	dual_quat.SetTranslation( translation );
	dual_quat.AxisAngleDegree( rotation_axis, rotation_angle );

	float rigid_transform[16];
	dual_quat.GetRigidTransform( rigid_transform );
	size_t col = 4, row = 4;
	orientation_matrix.SetFromArray( rigid_transform, col, row );

        return true;
    }
};

#endif
