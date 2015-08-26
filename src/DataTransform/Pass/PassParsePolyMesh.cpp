#include <stdio.h>
#include <string>
#include <iostream>

#include "ParseNode.h"
#include "Filter_ParseNode.h"
#include "Filter_ParsePolyMesh.h"
#include "PolyMesh_Data.h"
#include "ParseData_PolyMesh.h"
#include "bison_PolyMesh.tab.h"
#include "Flex_PolyMesh.h"
#include "PassParsePolyMesh.h"
//#include "DataType.h"
//#include "DataTransformPass.h"

using namespace std;

//extern int yy_PolyMesh_parse( struct ParseData_PolyMesh * pp );

bool PassParsePolyMesh::ExecutePass( void * & data_in, void * & data_out ){

    DataType::Enum data_type;
    string data_path;
    DataTransformMetaInfo * meta_info;
    if( !GetDataTransformMetaInfo( meta_info ) ){
	return false;
    }
    if( !QueryFromMetaInfo( meta_info, DataQuery::INPUT, data_type, data_path ) ){
	return false;
    }
    if( DataType::FILE_POLYMESH != data_type ){
	return false;
    }

    cout << "Attempting to open file: " << data_path << endl;

    // open a file handle to a particular file:
    char * szPathFile = strdup( data_path.c_str() );
    FILE * input_file = fopen( szPathFile, "r");
    // make sure it is valid:
    if ( !input_file ) {
        cout << "I can't open input file: " << szPathFile << endl;
        return false;
    }

    struct ParseData_PolyMesh p;

//    if( yy_PolyMesh_lex_init_extra( pp, &p.scaninfo ) ){
    if( yy_PolyMesh_lex_init( & p.scaninfo ) ){
	cout << "yylex init failed" << endl;
	return false;
    }

    // set flex to read from it instead of defaulting to STDIN:
    yy_PolyMesh_set_in( input_file, p.scaninfo );

    cout << "Attempting to parse file: " << data_path << endl;

    // parse input
    yy_PolyMesh_parse( &p );

   cout << "Attempting to access parsed data from file: " << data_path << endl;

    if( !p.data_node ){
	cout << "root invalid" << endl;
	return false;
    }

    Filter_ParsePolyMesh filter_polymesh;
    filter_polymesh.VisitNode( p.data_node );

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
