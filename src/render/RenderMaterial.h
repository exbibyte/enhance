#ifndef RENDER_MATERIAL_H
#define RENDER_MATERIAL_H

#include "AssetManager.h"

#include <vector>
#include <cassert>
#include <type_traits>

class RenderMaterialData {
public:
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
    struct Shininess {
        using Type = std::vector<double>;
        using Size = std::integral_constant<int, 1>;
    };
};

class RenderMaterial : public AssetManager< RenderMaterialData::Ambient,       RenderMaterialData::Ambient::Type >,
		       public AssetManager< RenderMaterialData::Diffuse,       RenderMaterialData::Diffuse::Type >,
		       public AssetManager< RenderMaterialData::Specular,      RenderMaterialData::Specular::Type >,
		       public AssetManager< RenderMaterialData::Shininess,     RenderMaterialData::Shininess::Type >
{
public:
    using AssetManager< RenderMaterialData::Ambient,       RenderMaterialData::Ambient::Type >::AddDataSingle;
    using AssetManager< RenderMaterialData::Diffuse,       RenderMaterialData::Diffuse::Type >::AddDataSingle;
    using AssetManager< RenderMaterialData::Specular,      RenderMaterialData::Specular::Type >::AddDataSingle;
    using AssetManager< RenderMaterialData::Shininess,     RenderMaterialData::Shininess::Type >::AddDataSingle;
    
    using AssetManager< RenderMaterialData::Ambient,       RenderMaterialData::Ambient::Type >::GetDataSingle;
    using AssetManager< RenderMaterialData::Diffuse,       RenderMaterialData::Diffuse::Type >::GetDataSingle;
    using AssetManager< RenderMaterialData::Specular,      RenderMaterialData::Specular::Type >::GetDataSingle;
    using AssetManager< RenderMaterialData::Shininess,     RenderMaterialData::Shininess::Type >::GetDataSingle;

    bool CheckIsValid(){
        RenderMaterialData::Ambient::Type check_ambient;
        if( !GetDataSingle( RenderMaterialData::Ambient(), check_ambient ) || RenderMaterialData::Ambient::Size::value > check_ambient.size() ){
            assert( 0 && "Material Ambient not valid" );
        }
        RenderMaterialData::Diffuse::Type check_diffuse;
        if( !GetDataSingle( RenderMaterialData::Diffuse(), check_diffuse ) || RenderMaterialData::Diffuse::Size::value > check_diffuse.size() ){
            assert( 0 && "Material Diffuse not valid" );
        }
        RenderMaterialData::Specular::Type check_specular;
        if( !GetDataSingle( RenderMaterialData::Specular(), check_specular ) || RenderMaterialData::Specular::Size::value > check_specular.size() ){
            assert( 0 && "Material Specular not valid" );
        }
	RenderMaterialData::Shininess::Type check_shininess;
        if( !GetDataSingle( RenderMaterialData::Shininess(), check_shininess ) || RenderMaterialData::Shininess::Size::value > check_shininess.size() ){
            assert( 0 && "Material Shininess not valid" );
        }
        return true;
    }
    bool Compute( RenderMaterialData::Ambient::Type & data_ambient,
                  RenderMaterialData::Diffuse::Type & data_diffuse,
                  RenderMaterialData::Specular::Type & data_specular,
		  RenderMaterialData::Shininess::Type & data_shininess ){
        if( !CheckIsValid() ){
            return false;
        }

        if( !GetDataSingle( RenderMaterialData::Ambient(), data_ambient ) )         return false;
        if( !GetDataSingle( RenderMaterialData::Diffuse(), data_diffuse ) )         return false;
        if( !GetDataSingle( RenderMaterialData::Specular(), data_specular ) )       return false;
	if( !GetDataSingle( RenderMaterialData::Shininess(), data_shininess ) )     return false;

        return true;
    }
};

#endif
