#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "AssetManager.h"
#include "DualQuat.hpp"

#include <vector>
#include <cassert>
#include <type_traits>
#include <string>

class RenderContextData {
public:
    struct WindowSize {
        using Type = std::vector<int>;
        using Size = std::integral_constant<int, 2>;
    };
    struct TextureSizeShadowMap {
        using Type = std::vector<int>;
        using Size = std::integral_constant<int, 2>;
    };
    struct Title {
        using Type = std::string;
        using Size = std::integral_constant<int, 1>;
    };
};

class RenderContext : public AssetManager< RenderContextData::WindowSize,            RenderContextData::WindowSize::Type >,
		      public AssetManager< RenderContextData::TextureSizeShadowMap,  RenderContextData::TextureSizeShadowMap::Type >,
		      public AssetManager< RenderContextData::Title,                 RenderContextData::Title::Type >
{
public:
    using AssetManager< RenderContextData::WindowSize,            RenderContextData::WindowSize::Type >::AddDataSingle;
    using AssetManager< RenderContextData::TextureSizeShadowMap,  RenderContextData::TextureSizeShadowMap::Type >::AddDataSingle;
    using AssetManager< RenderContextData::Title,                 RenderContextData::Title::Type >::AddDataSingle;
    
    using AssetManager< RenderContextData::WindowSize,            RenderContextData::WindowSize::Type >::GetDataSingle;
    using AssetManager< RenderContextData::TextureSizeShadowMap,  RenderContextData::TextureSizeShadowMap::Type >::GetDataSingle;
    using AssetManager< RenderContextData::Title,                 RenderContextData::Title::Type >::GetDataSingle;

    bool CheckIsValid(){
        RenderContextData::WindowSize::Type check_windowsize;
        if( !GetDataSingle( RenderContextData::WindowSize(), check_windowsize ) || RenderContextData::WindowSize::Size::value > check_windowsize.size() ){
            assert( 0 && "Context WindowSize not valid" );
        }
        RenderContextData::TextureSizeShadowMap::Type check_texturesize_shadowmap;
        if( !GetDataSingle( RenderContextData::TextureSizeShadowMap(), check_texturesize_shadowmap ) || RenderContextData::TextureSizeShadowMap::Size::value > check_texturesize_shadowmap.size() ){
            assert( 0 && "Context TextureSizeShadowMap not valid" );
        }
	RenderContextData::Title::Type check_title;
        if( !GetDataSingle( RenderContextData::Title(), check_title ) || RenderContextData::Title::Size::value > check_title.size() ){
            assert( 0 && "Context Title not valid" );
        }
        return true;
    }
    bool Compute( RenderContextData::WindowSize::Type & data_windowsize,
		  RenderContextData::TextureSizeShadowMap::Type & data_texturesize_shadowmap,
		  RenderContextData::Title::Type & data_title ){
        if( !CheckIsValid() ){
            return false;
        }

        if( !GetDataSingle( RenderContextData::WindowSize(), data_windowsize ) )                          return false;
	if( !GetDataSingle( RenderContextData::TextureSizeShadowMap(), data_texturesize_shadowmap ) )     return false;
	if( !GetDataSingle( RenderContextData::Title(), data_title ) )                                    return false;

        return true;
    }
};

#endif
