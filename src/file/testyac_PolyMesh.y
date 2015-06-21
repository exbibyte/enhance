%union 
{
    int num;
    char * str;
}

%token<str> VARIABLE
%token<str> BRACE_OPEN
%token<str> BRACE_CLOSE
%token<str> BRACKET_OPEN
%token<str> BRACKET_CLOSE
%token<str> COLON
%token<str> COMMA


%{
  #include <stdio.h>
  #include <string.h>
  #include <iostream>
  using namespace std;
  extern FILE * yyin;
  extern int yylex();
  extern int yyparse();
  void yyerror(char *);
%}

%%

     root    : expr
             | { printf("nothing found\n"); }
             ;

     expr                   : expr expr
                            | expr keyval_item
                            | expr var_list_array
                            | keyval_item expr
                            | var_list_array expr
                            | keyval_item
                            | var_list_array

     var_list_array         : VARIABLE COLON BRACKET_OPEN array_multiple_items BRACKET_CLOSE { printf("var_array: %s\n", $1 ); }
                            | VARIABLE COLON var_list { printf("var_list: %s\n", $1 ); }

     var_list               : var_list var_list_item
                            | var_list_item

     var_list_item          : BRACE_OPEN array_multiple_items BRACE_CLOSE

     array_multiple_items   : array_multiple_items COMMA keyval_item
                            | array_multiple_items COMMA var_list_array
	                    | var_list_array
                            | keyval_item

     keyval_item            : VARIABLE COLON VARIABLE { printf("key: %s, val: %s\n", $1, $3 ); }
                            | VARIABLE { printf("val: %s\n", $1 ); }
     
%%

void yyerror(char * s){
  fprintf(stderr, "%s\n", s);
}
