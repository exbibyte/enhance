#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include "AssetManager.h"
#include "MatrixMath.h"
#include "Mat.hpp"

#include <vector>
#include <cassert>
#include <type_traits>

class RenderCameraData {
public:
    struct Coordinate {
        using Type = std::vector<double>;
        using Size = std::integral_constant<int, 3>;
    };
    struct Lookat {
        using Type = std::vector<double>;
        using Size = std::integral_constant<int, 3>;
    };
    struct Up {
        using Type = std::vector<double>;
        using Size = std::integral_constant<int, 3>;
    };
    struct Perspective {
        using Type = std::vector<double>;
        using Size = std::integral_constant<int, 4>;
    };
    struct Ambient {
        using Type = std::vector<double>;
        using Size = std::integral_constant<int, 3>;
    };
    struct Diffuse {
        using Type = std::vector<double>;
        using Size = std::integral_constant<int, 3>;
    };
    struct Specular {
        using Type = std::vector<double>;
        using Size = std::integral_constant<int, 3>;
    };
};

class RenderCamera : public AssetManager< RenderCameraData::Coordinate,    RenderCameraData::Coordinate::Type >,
		     public AssetManager< RenderCameraData::Lookat,        RenderCameraData::Lookat::Type >,
		     public AssetManager< RenderCameraData::Up,            RenderCameraData::Up::Type >,
		     public AssetManager< RenderCameraData::Perspective,   RenderCameraData::Perspective::Type >,
		     public AssetManager< RenderCameraData::Ambient,       RenderCameraData::Ambient::Type >,
		     public AssetManager< RenderCameraData::Diffuse,       RenderCameraData::Diffuse::Type >,
		     public AssetManager< RenderCameraData::Specular,      RenderCameraData::Specular::Type >
{
public:
    using AssetManager< RenderCameraData::Coordinate,    RenderCameraData::Coordinate::Type >::AddDataSingle;
    using AssetManager< RenderCameraData::Lookat,        RenderCameraData::Lookat::Type >::AddDataSingle;
    using AssetManager< RenderCameraData::Up,            RenderCameraData::Up::Type >::AddDataSingle;
    using AssetManager< RenderCameraData::Perspective,   RenderCameraData::Perspective::Type >::AddDataSingle;
    using AssetManager< RenderCameraData::Ambient,       RenderCameraData::Ambient::Type >::AddDataSingle;
    using AssetManager< RenderCameraData::Diffuse,       RenderCameraData::Diffuse::Type >::AddDataSingle;
    using AssetManager< RenderCameraData::Specular,      RenderCameraData::Specular::Type >::AddDataSingle;
    
    using AssetManager< RenderCameraData::Coordinate,    RenderCameraData::Coordinate::Type >::GetDataSingle;
    using AssetManager< RenderCameraData::Lookat,        RenderCameraData::Lookat::Type >::GetDataSingle;
    using AssetManager< RenderCameraData::Up,            RenderCameraData::Up::Type >::GetDataSingle;
    using AssetManager< RenderCameraData::Perspective,   RenderCameraData::Perspective::Type >::GetDataSingle;
    using AssetManager< RenderCameraData::Ambient,       RenderCameraData::Ambient::Type >::GetDataSingle;
    using AssetManager< RenderCameraData::Diffuse,       RenderCameraData::Diffuse::Type >::GetDataSingle;
    using AssetManager< RenderCameraData::Specular,      RenderCameraData::Specular::Type >::GetDataSingle;

    bool CheckIsValid(){
        RenderCameraData::Coordinate::Type check_coordinate;
        if( !GetDataSingle( RenderCameraData::Coordinate(), check_coordinate ) || RenderCameraData::Coordinate::Size::value > check_coordinate.size() ){
            assert( 0 && "Camera Coordinate not valid" );
        }
        RenderCameraData::Lookat::Type check_lookat;
        if( !GetDataSingle( RenderCameraData::Lookat(), check_lookat ) || RenderCameraData::Lookat::Size::value > check_lookat.size() ){
            assert( 0 && "Camera Lookat not valid" );
        }
        RenderCameraData::Up::Type check_up;
        if( !GetDataSingle( RenderCameraData::Up(), check_up ) || RenderCameraData::Up::Size::value > check_up.size() ){
            assert( 0 && "Camera Up not valid" );
        }
        RenderCameraData::Perspective::Type check_perspective;
        if( !GetDataSingle( RenderCameraData::Perspective(), check_perspective ) || RenderCameraData::Perspective::Size::value > check_perspective.size() ){
            assert( 0 && "Camera Perspective not valid" );
        }
        RenderCameraData::Ambient::Type check_ambient;
        if( !GetDataSingle( RenderCameraData::Ambient(), check_ambient ) || RenderCameraData::Ambient::Size::value > check_ambient.size() ){
            assert( 0 && "Camera Ambient not valid" );
        }
        RenderCameraData::Diffuse::Type check_diffuse;
        if( !GetDataSingle( RenderCameraData::Diffuse(), check_diffuse ) || RenderCameraData::Diffuse::Size::value > check_diffuse.size() ){
            assert( 0 && "Camera Diffuse not valid" );
        }
        RenderCameraData::Specular::Type check_specular;
        if( !GetDataSingle( RenderCameraData::Specular(), check_specular ) || RenderCameraData::Specular::Size::value > check_specular.size() ){
            assert( 0 && "Camera Specular not valid" );
        }
        return true;
    }
    bool Compute( RenderCameraData::Ambient::Type & data_ambient,
                  RenderCameraData::Diffuse::Type & data_diffuse,
                  RenderCameraData::Specular::Type & data_specular,
                  RenderCameraData::Coordinate::Type & data_coordinate,
		  Mat & perspective_frust_matrix,
                  Mat & view_matrix ){
        if( !CheckIsValid() ){
            return false;
        }

        RenderCameraData::Perspective::Type data_perspective;
        RenderCameraData::Up::Type data_up;
        RenderCameraData::Lookat::Type data_lookat;
        if( !GetDataSingle( RenderCameraData::Ambient(), data_ambient ) )         return false;
        if( !GetDataSingle( RenderCameraData::Diffuse(), data_diffuse ) )         return false;
        if( !GetDataSingle( RenderCameraData::Specular(), data_specular ) )       return false;
        if( !GetDataSingle( RenderCameraData::Coordinate(), data_coordinate ) )   return false;
        if( !GetDataSingle( RenderCameraData::Perspective(), data_perspective ) ) return false;
        if( !GetDataSingle( RenderCameraData::Lookat(), data_lookat ) )           return false;
        if( !GetDataSingle( RenderCameraData::Up(), data_up ) )                   return false;

	float temp_perspective_frust_mat[16];
	MatrixMath::Perspective( data_perspective[0], data_perspective[1], data_perspective[2], data_perspective[3], temp_perspective_frust_mat );
	size_t col = 4, row = 4;
	perspective_frust_matrix.SetFromArray( temp_perspective_frust_mat, col, row );

	float eye[3];
	float at[3];
	float up[3];
	for( int i = 0; i < 3; ++i ){
	    eye[i] = data_coordinate[i];
	    at[i] = data_lookat[i];
	    up[i] = data_up[i];
	}
	float temp_view_matrix[16];
	MatrixMath::LookAt( eye, at, up, temp_view_matrix );
	view_matrix.SetFromArray( temp_view_matrix, col, row );
        return true;
    }
};

#endif
