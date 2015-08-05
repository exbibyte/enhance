%option noyywrap nodefault yylineno
%option prefix="yy_PolyMesh_"

%{
  #include <stdlib.h>
  void yy_PolyMesh_error(char *);
  #include "ParseNode.h"
  #include "bison_PolyMesh.tab.h"
  #include <string.h>
  #include <iostream>
  using namespace std;
%}

%%

"-{"           {
                yy_PolyMesh_lval.str = strdup(yy_PolyMesh_text);
                return BRACE_OPEN;
               }

"}"            {
                yy_PolyMesh_lval.str = strdup(yy_PolyMesh_text);
                return BRACE_CLOSE;
               }
"["            {
                yy_PolyMesh_lval.str = strdup(yy_PolyMesh_text);
                return BRACKET_OPEN;
               }
"]"            {
                yy_PolyMesh_lval.str = strdup(yy_PolyMesh_text);
                return BRACKET_CLOSE;
               }
":"            {
                yy_PolyMesh_lval.str = strdup(yy_PolyMesh_text);
                return COLON;
               }
","            {
                yy_PolyMesh_lval.str = strdup(yy_PolyMesh_text);
                return COMMA;
               }
[\.\-a-z_0-9]+  {
                yy_PolyMesh_lval.str = strdup(yy_PolyMesh_text);
                return VARIABLE;
               }
[ \t\n]        { }
.              return(0);

%%
