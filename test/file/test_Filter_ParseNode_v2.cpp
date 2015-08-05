#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ParseNode.h"
#include "Filter_ParseNode.h"
#include "Filter_ParsePolyMesh.h"
#include "PolyMesh_Data.h"

using namespace std;
extern FILE * yy_PolyMesh_in;
extern int yy_PolyMesh_lex();
extern int yy_PolyMesh_parse();
extern ParseNode * root_data;

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
    // set flex to read from it instead of defaulting to STDIN:
    yy_PolyMesh_in = myfile;

    // parse through the input until there is no more:
    /* do { */
    yy_PolyMesh_parse();
    /* } while (!feof(yyin)); */


    if( !root_data ){
	cout << "root invalid" << endl;
	return -1;
    }

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
    
    return 0;
}
