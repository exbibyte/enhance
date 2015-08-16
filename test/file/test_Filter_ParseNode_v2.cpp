#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ParseNode.h"
#include "Filter_ParseNode.h"
#include "Filter_ParsePolyMesh.h"
#include "PolyMesh_Data.h"
#include "ParseData_PolyMesh.h"
#include "bison_PolyMesh.tab.h"
#include "Flex_PolyMesh.h"

using namespace std;
//extern FILE * yy_PolyMesh_in;
//extern int yy_PolyMesh_lex();
extern int yy_PolyMesh_parse( struct ParseData_PolyMesh * pp );
//extern ParseNode * root_data;

void visitNode( ParseNode * node, int spaces ) {
    if( !node ){
	return;
    }
    for( int j = 0; j < spaces * 2; j++ ){
	cout << " ";
    }
    cout << "Name: " << node->_strVarName << ", Val: " << node->_strVarVal << endl;
    for( auto * i : node->_children ){
	visitNode( i, spaces + 1 );
    }
}

int main(int argc, char** argv){
    // open a file handle to a particular file:
    FILE *myfile = fopen(argv[1], "r");
    // make sure it is valid:
    if (!myfile) {
        cout << "I can't open input file!" << endl;
        return -1;
    }

    struct ParseData_PolyMesh p { 0, 0 };
    // if( yy_PolyMesh_lex_init_extra( &p, &(p.scaninfo) ) ){
    // 	cout << "yylex init failed" << endl;
    // 	return -1;
    // }

    // set flex to read from it instead of defaulting to STDIN:
    yy_PolyMesh_set_in( myfile, &p.scaninfo );

    if( yy_PolyMesh_lex_init_extra( &p, &(p.scaninfo) ) ){
	cout << "yylex init failed" << endl;
	return -1;
    }
    
    // parse through the input until there is no more:
    /* do { */
    yy_PolyMesh_parse( &p );
    /* } while (!feof(yyin)); */


    if( !p.data_node ){
	cout << "root invalid" << endl;
	return -1;
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
    
    return 0;
}
