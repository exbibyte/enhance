%{
  #include <stdlib.h>
  void yyerror(char *);
  #include "FormatPolyMesh.h"
  #include "testyac_PolyMesh.tab.h"
  #include <string.h>
  #include <iostream>
  using namespace std;
%}

%%

"-{"           {
                yylval.str = strdup(yytext);
                return BRACE_OPEN;
               }

"}"            {
                yylval.str = strdup(yytext);
                return BRACE_CLOSE;
               }
"["            {
                yylval.str = strdup(yytext);
                return BRACKET_OPEN;
               }
"]"            {
                yylval.str = strdup(yytext);
                return BRACKET_CLOSE;
               }
":"            {
                yylval.str = strdup(yytext);
                return COLON;
               }
","            {
                yylval.str = strdup(yytext);
                return COMMA;
               }
[\.\-a-z_0-9]+  {
                yylval.str = strdup(yytext);
                return VARIABLE;
               }
[ \t\n]        { }
.              return(0);

%%

int yywrap(void) {
  return 1;
}
