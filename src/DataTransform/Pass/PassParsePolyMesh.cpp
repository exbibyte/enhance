#include "PassParsePolyMesh.h"
#include "DataType.h"
#include "Filter_ParsePolyMesh.h"

extern ParseNode * root_data;

bool PassParsePolyMesh::ExecutePath( DataTransformMetaInfo * meta_info ){    
    DataType data_type;
    string data_path;
    if( !GetInputFileFromMetaInfo( data_type, data_path ) ){
	return false;
    }
    if( DataType::FILE_POLYMESH != data_type ){
	return false;
    }
    //TODO: select PolyMesh parser and feed data_path to parser
    if( !SetParser( DataType::FILE_POLYMESH, data_path ) ){
	return false;
    }
    //process parsed nodes
    Filter_ParsePolyMesh filter_polymesh;
    filter_polymesh.VisitNode( root_data );

    for( auto i : filter_polymesh._vec_PolyMesh_Data_Vert ){
	i->PrintData();
    }
    for( auto i : filter_polymesh._vec_PolyMesh_Data_Normal ){
	i->PrintData();
    }
    for( auto i : filter_polymesh._vec_PolyMesh_Data_Edge ){
	i->PrintData();
    }
    for( auto i : filter_polymesh._vec_PolyMesh_Data_Face ){
	i->PrintData();
    }
    for( auto i : filter_polymesh._vec_PolyMesh_Data_TexSrc ){
	i->PrintData();
    }
    for( auto i : filter_polymesh._vec_PolyMesh_Data_TexCoord ){
	i->PrintData();
    }

    return true;
}
