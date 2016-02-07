#ifndef RENDER_VERTEX_H
#define RENDER_VERTEX_H

#include "AssetManager.h"

#include <vector>
#include <cassert>
#include <type_traits>

class RenderVertexData {
public:
    struct Vertices {
	using Type = std::vector<double>;
	using SizeMin = std::integral_constant<int, 3>; //variable
    };
    struct Normals {
	using Type = std::vector<double>;
	using SizeMin = std::integral_constant<int, 3>; //variable
    };
};

class RenderVertex : public AssetManager< RenderVertexData::Vertices,    RenderVertexData::Vertices::Type >,
                     public AssetManager< RenderVertexData::Normals,     RenderVertexData::Normals::Type > {
public:
    using AssetManager< RenderVertexData::Vertices,    RenderVertexData::Vertices::Type >::AddDataSingle;
    using AssetManager< RenderVertexData::Normals,     RenderVertexData::Normals::Type >::AddDataSingle;

    using AssetManager< RenderVertexData::Vertices,    RenderVertexData::Vertices::Type >::GetDataSingle;
    using AssetManager< RenderVertexData::Normals,     RenderVertexData::Normals::Type >::GetDataSingle;

    using AssetManager< RenderVertexData::Vertices,    RenderVertexData::Vertices::Type >::GetDataSingleSize;
    using AssetManager< RenderVertexData::Normals,     RenderVertexData::Normals::Type >::GetDataSingleSize;
    
    bool CheckIsValid(){
	unsigned int sizeVertices;
	unsigned int sizeNormals;
	GetDataSingleSize( RenderVertexData::Vertices(), sizeVertices );
        if( sizeVertices % RenderVertexData::Vertices::SizeMin::value != 0 ){
            assert( 0 && "Vertices data count not valid" );
        }
	GetDataSingleSize( RenderVertexData::Normals(), sizeNormals );
        if( sizeNormals % RenderVertexData::Normals::SizeMin::value != 0 ){
            assert( 0 && "Normals data count not valid" );
        }
	if( sizeVertices != sizeNormals ){
	    assert( 0 && "Vertices and Normals count not equal" );
	}
        return true;
    }
    bool Compute( RenderVertexData::Vertices::Type & vertices, RenderVertexData::Normals::Type & normals ){
        if( !CheckIsValid() ){
            return false;
        }
        if( !GetDataSingle( RenderVertexData::Vertices(), vertices ) )   return false;
	if( !GetDataSingle( RenderVertexData::Normals(), normals ) )     return false;
        return true;
    }
};

#endif
