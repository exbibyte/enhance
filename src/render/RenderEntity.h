#ifndef RENDER_ENTITY_H
#define RENDER_ENTITY_H

#include "RenderPoly.h"
#include "RenderMaterial.h"
#include "RenderVertex.h"

class RenderEntity : public RenderPoly, public RenderMaterial, public RenderVertex {
public:
    using RenderPoly::AddDataSingle;
    using RenderMaterial::AddDataSingle;
    using RenderVertex::AddDataSingle;

    using RenderPoly::GetDataSingle;
    using RenderMaterial::GetDataSingle;
    using RenderVertex::GetDataSingle;

    using RenderVertex::GetDataSingleSize;
    
    bool CheckIsValid(){
	if( !RenderPoly::CheckIsValid() )     return false;
	if( !RenderMaterial::CheckIsValid() ) return false;
	if( !RenderVertex::CheckIsValid() )   return false;
        return true;
    }
    template< class ...Args >
    bool Compute( RenderPolyData id, Args &... args ){
	if( !RenderPoly::Compute( args... ) ) return false;
        return true;
    }
    template< class ...Args >
    bool Compute( RenderMaterialData id, Args &... args ){
	if( !RenderMaterial::Compute( args... ) ) return false;
        return true;
    }
    template< class ...Args >
    bool Compute( RenderVertexData id, Args &... args ){
	if( !RenderVertex::Compute( args... ) ) return false;
        return true;
    }
};

#endif
