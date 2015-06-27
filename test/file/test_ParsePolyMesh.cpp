#include <stdio.h>
#include <string.h>
#include <iostream>
#include "FormatPolyMesh.h"
using namespace std;
extern FILE * yyin;
extern int yylex();
extern int yyparse();
extern FormatPolyMesh_Base * root_data;

void visitNode( FormatPolyMesh_Base * node, int spaces ) {
    if( !node ){
	return;
    }
    for( int j = 0; j < spaces * 2; j++ ){
	cout << " ";
    }
    cout << "Name: " << node->strVarName << ", Val: " << node->strVarVal << endl;
    for( auto * i : node->children ){
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
    yyin = myfile;

    // parse through the input until there is no more:
    /* do { */
    yyparse();
    /* } while (!feof(yyin)); */


    if( !root_data ){
	cout << "root invalid" << endl;
	return -1;
    }

    visitNode( root_data, 0 );
    
    return 0;
}
