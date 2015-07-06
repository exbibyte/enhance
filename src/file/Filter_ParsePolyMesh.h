#ifndef FILTER_PARSEPOLYMESH_H
#define FILTER_PARSEPOLYMESH_H

#include "ParseNode.h"
#include "Filter_ParseNode.h"
#include "PolyMesh_Data.h"

#include <vector>

class Filter_ParsePolyMesh : public Filter_ParseNode {
public:
    Filter_ParsePolyMesh();
    bool TransformNode( ParseNode * );
    std::vector< PolyMesh_Data_Vert * > _vec_PolyMesh_Data_Vert;
    std::vector< PolyMesh_Data_Normal * > _vec_PolyMesh_Data_Normal;
    std::vector< PolyMesh_Data_Edge * > _vec_PolyMesh_Data_Edge;
    std::vector< PolyMesh_Data_Face * > _vec_PolyMesh_Data_Face;
    std::vector< PolyMesh_Data_TexSrc * > _vec_PolyMesh_Data_TexSrc;
    std::vector< PolyMesh_Data_TexCoord * > _vec_PolyMesh_Data_TexCoord;
};

#endif
