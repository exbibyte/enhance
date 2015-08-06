%name-prefix="yy_PolyMesh_"
%pure_parser
%parse-param { struct ParseData_PolyMesh * pp }

%{
  #include "ParseNode.h"
  #include <stdio.h>
  #include <string.h>
  #include <iostream>
  #include <vector>
  #include <string>
  #include <utility>
  using namespace std;
%}

%union 
{
    int num;
    char * str;
    ParseNode * data_node;
};

%{
  #include "ParseData_PolyMesh.h"
//  #define YYLEX_PARAM pp
%}

%lex-param{ pp }

%{
  #include "Flex_PolyMesh.h"
  void yy_PolyMesh_error( struct ParseData_PolyMesh * pp, char *);
%}

%token<str> VARIABLE
%token<str> BRACE_OPEN
%token<str> BRACE_CLOSE
%token<str> BRACKET_OPEN
%token<str> BRACKET_CLOSE
%token<str> COLON
%token<str> COMMA

%type<data_node> root
%type<data_node> expr
%type<data_node> var_list_array
%type<data_node> var_list
%type<data_node> var_list_item
%type<data_node> array_multiple_items
%type<data_node> keyval_item

%%

     root    : expr {
	          pp->data_node = $1;
               }
             | {
	          printf("nothing found\n");
               }
             ;

     expr                   : expr keyval_item {
                                 $$ = $1;				 
				 $$->_children.push_back( $2 );
                              }
                            | expr var_list_array {
                                 $$ = $1;				 
				 $$->_children.push_back( $2 );
                              }
                            | keyval_item {
                                 $$ = new ParseNode();
				 $$->_strVarName = "";
				 $$->_strVarVal = "";
				 $$->_children.push_back( $1 );
                              }
                            | var_list_array {
                                 $$ = new ParseNode();
				 $$->_strVarName = "";
				 $$->_strVarVal = "";
				 $$->_children.push_back( $1 );
                              }
                            ;

     var_list_array         : VARIABLE COLON BRACKET_OPEN array_multiple_items BRACKET_CLOSE {
	                          //printf("var_array: %s\n", $1 );
				  $$ = $4;
				  $$->_strVarName = $1;
                              }
                            | VARIABLE COLON var_list {
				  //printf("var_list: %s\n", $1 );
				  $$ = $3;
				  $$->_strVarName = $1;
                              }
                            ;

     var_list               : var_list var_list_item {
	                          $$ = $1;
				  $$->_children.push_back( $2 );
                              }
                            | var_list_item {
                                  $$ = new ParseNode();
				  $$->_strVarName = "";
				  $$->_strVarVal = "";
				  $$->_children.push_back( $1 );
                              }
                            ;

     var_list_item          : BRACE_OPEN array_multiple_items BRACE_CLOSE {
				  $$ = $2;
                              }
                            ;

     array_multiple_items   : array_multiple_items COMMA keyval_item {
	                          $$ = $1;
				  $$->_children.push_back( $3 );
                              }
                            | array_multiple_items COMMA var_list_array {
				  $$ = $1;				  
				  $$->_children.push_back( $3 );
                              }
                            | var_list_array {
	                          $$ = new ParseNode();
				  $$->_strVarName = "";
				  $$->_strVarVal = "";
				  $$->_children.push_back( $1 );
                              }
                            | keyval_item {
	                          $$ = new ParseNode();
				  $$->_strVarName = "";
				  $$->_strVarVal = "";
				  $$->_children.push_back( $1 );
                              }
                            ;

     keyval_item            : VARIABLE COLON VARIABLE {
	                          //printf("key: %s, val: %s\n", $1, $3 );
				  $$ = new ParseNode();
				  $$->_strVarName = $1;
				  $$->_strVarVal = $3;
                              }
                            | VARIABLE
			      {
				  //printf("val: %s\n", $1 );
				  $$ = new ParseNode();
				  $$->_strVarName = "";
				  $$->_strVarVal = $1;
			      }
                            ;

%%

void yy_PolyMesh_error( struct ParseData_PolyMesh * pp, char * s){
  fprintf(stderr, "%s\n", s);
}
