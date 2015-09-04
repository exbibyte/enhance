%option noyywrap nodefault yylineno 
%option reentrant
%option prefix="yy_PolyMesh_"
%option bison-bridge

%{
/*%option extra-type="struct ParseData_PolyMesh*"*/
  #include <stdlib.h>
  void yy_PolyMesh_error( struct ParseData_PolyMesh * pp, char * );
//  void yy_PolyMesh_error( char * );
  #include "ParseNode.h"
  #include "bison_PolyMesh.tab.h"
  #include <string.h>
  //#include <iostream>
  using namespace std;
%}

%{
  #include "ParseData_PolyMesh.h"
%}

%%

%{
//  struct ParseData_PolyMesh * scanner_instance_data = yyextra;
%}

"-{"           {
                yylval->str = strdup(yytext);
                return BRACE_OPEN;
               }

"}"            {
                yylval->str = strdup(yytext);
                return BRACE_CLOSE;
               }
"["            {
                yylval->str = strdup(yytext);
                return BRACKET_OPEN;
               }
"]"            {
                yylval->str = strdup(yytext);
                return BRACKET_CLOSE;
               }
":"            {
                yylval->str = strdup(yytext);
                return COLON;
               }
","            {
                yylval->str = strdup(yytext);
                return COMMA;
               }
[\.\-a-z_0-9A-Z]+  {
                yylval->str = strdup(yytext);
                return VARIABLE;
               }
[ \t\n]        { }
.              return(0);

%%
