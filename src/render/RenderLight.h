#ifndef RENDER_LIGHT_H
#define RENDER_LIGHT_H

#include "AssetManager.h"
#include "MatrixMath.h"

#include <vector>
#include <cassert>
#include <type_traits>

//math library
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
using glm::mat4;
using glm::vec3;

class RenderLightData {
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

class RenderLight : public AssetManager< RenderLightData::Coordinate,    RenderLightData::Coordinate::Type >,
                    public AssetManager< RenderLightData::Lookat,        RenderLightData::Lookat::Type >,
                    public AssetManager< RenderLightData::Up,            RenderLightData::Up::Type >,
                    public AssetManager< RenderLightData::Perspective,   RenderLightData::Perspective::Type >,
                    public AssetManager< RenderLightData::Ambient,       RenderLightData::Ambient::Type >,
                    public AssetManager< RenderLightData::Diffuse,       RenderLightData::Diffuse::Type >,
                    public AssetManager< RenderLightData::Specular,      RenderLightData::Specular::Type >
{
public:
    using AssetManager< RenderLightData::Coordinate,    RenderLightData::Coordinate::Type >::AddDataSingle;
    using AssetManager< RenderLightData::Lookat,        RenderLightData::Lookat::Type >::AddDataSingle;
    using AssetManager< RenderLightData::Up,            RenderLightData::Up::Type >::AddDataSingle;
    using AssetManager< RenderLightData::Perspective,   RenderLightData::Perspective::Type >::AddDataSingle;
    using AssetManager< RenderLightData::Ambient,       RenderLightData::Ambient::Type >::AddDataSingle;
    using AssetManager< RenderLightData::Diffuse,       RenderLightData::Diffuse::Type >::AddDataSingle;
    using AssetManager< RenderLightData::Specular,      RenderLightData::Specular::Type >::AddDataSingle;
    
    using AssetManager< RenderLightData::Coordinate,    RenderLightData::Coordinate::Type >::GetDataSingle;
    using AssetManager< RenderLightData::Lookat,        RenderLightData::Lookat::Type >::GetDataSingle;
    using AssetManager< RenderLightData::Up,            RenderLightData::Up::Type >::GetDataSingle;
    using AssetManager< RenderLightData::Perspective,   RenderLightData::Perspective::Type >::GetDataSingle;
    using AssetManager< RenderLightData::Ambient,       RenderLightData::Ambient::Type >::GetDataSingle;
    using AssetManager< RenderLightData::Diffuse,       RenderLightData::Diffuse::Type >::GetDataSingle;
    using AssetManager< RenderLightData::Specular,      RenderLightData::Specular::Type >::GetDataSingle;

    bool CheckIsValid(){
        RenderLightData::Coordinate::Type check_coordinate;
        if( !GetDataSingle( RenderLightData::Coordinate(), check_coordinate ) || RenderLightData::Coordinate::Size::value > check_coordinate.size() ){
            assert( 0 && "Light Coordinate not valid" );
        }
        RenderLightData::Lookat::Type check_lookat;
        if( !GetDataSingle( RenderLightData::Lookat(), check_lookat ) || RenderLightData::Lookat::Size::value > check_lookat.size() ){
            assert( 0 && "Light Lookat not valid" );
        }
        RenderLightData::Up::Type check_up;
        if( !GetDataSingle( RenderLightData::Up(), check_up ) || RenderLightData::Up::Size::value > check_up.size() ){
            assert( 0 && "Light Up not valid" );
        }
        RenderLightData::Perspective::Type check_perspective;
        if( !GetDataSingle( RenderLightData::Perspective(), check_perspective ) || RenderLightData::Perspective::Size::value > check_perspective.size() ){
            assert( 0 && "Light Perspective not valid" );
        }
        RenderLightData::Ambient::Type check_ambient;
        if( !GetDataSingle( RenderLightData::Ambient(), check_ambient ) || RenderLightData::Ambient::Size::value > check_ambient.size() ){
            assert( 0 && "Light Ambient not valid" );
        }
        RenderLightData::Diffuse::Type check_diffuse;
        if( !GetDataSingle( RenderLightData::Diffuse(), check_diffuse ) || RenderLightData::Diffuse::Size::value > check_diffuse.size() ){
            assert( 0 && "Light Diffuse not valid" );
        }
        RenderLightData::Specular::Type check_specular;
        if( !GetDataSingle( RenderLightData::Specular(), check_specular ) || RenderLightData::Specular::Size::value > check_specular.size() ){
            assert( 0 && "Light Specular not valid" );
        }
        return true;
    }
    bool Compute( RenderLightData::Ambient::Type & data_ambient,
                  RenderLightData::Diffuse::Type & data_diffuse,
                  RenderLightData::Specular::Type & data_specular,
                  RenderLightData::Coordinate::Type & data_coordinate,
                  mat4 & perspective_frust_matrix,
                  mat4 & view_matrix ){
        if( !CheckIsValid() ){
            return false;
        }

        RenderLightData::Perspective::Type data_perspective;
        RenderLightData::Up::Type data_up;
        RenderLightData::Lookat::Type data_lookat;
        if( !GetDataSingle( RenderLightData::Ambient(), data_ambient ) )         return false;
        if( !GetDataSingle( RenderLightData::Diffuse(), data_diffuse ) )         return false;
        if( !GetDataSingle( RenderLightData::Specular(), data_specular ) )       return false;
        if( !GetDataSingle( RenderLightData::Coordinate(), data_coordinate ) )   return false;
        if( !GetDataSingle( RenderLightData::Perspective(), data_perspective ) ) return false;
        if( !GetDataSingle( RenderLightData::Lookat(), data_lookat ) )           return false;
        if( !GetDataSingle( RenderLightData::Up(), data_up ) )                   return false;

        /* perspective_frust_matrix = glm::perspective( data_perspective[0], data_perspective[1], data_perspective[2], data_perspective[3] ); */
	float temp_perspective_frust_mat[16];
	MatrixMath::Perspective( data_perspective[0], data_perspective[1], data_perspective[2], data_perspective[3], temp_perspective_frust_mat );
	perspective_frust_matrix = glm::make_mat4( temp_perspective_frust_mat );

        view_matrix = glm::lookAt( vec3( data_coordinate[0], data_coordinate[1], data_coordinate[2] ), 
                                   vec3( data_lookat[0],     data_lookat[1],     data_lookat[2]     ),
                                   vec3( data_up[0],         data_up[1],         data_up[2])        );
        return true;
    }
};

#endif
