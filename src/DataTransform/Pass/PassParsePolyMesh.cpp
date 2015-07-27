#include "PassParsePolyMesh.h"
#include "DataType.h"
#include "ParseNode.h"
#include "Filter_ParseNode.h"
#include "Filter_ParsePolyMesh.h"
#include "PolyMesh_Data.h"

#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

extern FILE * yyin;
extern int yylex();
extern int yyparse();
extern ParseNode * root_data;

bool PassParsePolyMesh::ExecutePath( DataTransformMetaInfo * meta_info_input, DataTransformMetaInfo * meta_info_output ){
    DataType::Enum data_type;
    string data_path;
    if( !GetInputFileFromMetaInfo( meta_info_input, data_type, data_path ) ){
	return false;
    }
    if( DataType::FILE_POLYMESH != data_type ){
	return false;
    }
    // //TODO: select PolyMesh parser and feed data_path to parser
    // if( !SetParser( DataType::FILE_POLYMESH, data_path ) ){
    // 	return false;
    // }

    FILE * myfile = fopen( data_path.c_str(), "r" );
    // make sure it is valid:
    if (!myfile) {
        cout << "I can't open input file!" << endl;
        return -1;
    }
    // set flex to read from it instead of defaulting to STDIN:
    yyin = myfile;

    // parse through the input until there is no more:
    do {
	yyparse();
    } while (!feof(yyin));

    if( !root_data ){
	cout << "root invalid" << endl;
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
