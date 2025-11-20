%{
#include <stdio.h>
#include <FlexLexer.h>

extern int yylex();
extern int yyerror(const char *s);
%}

%%

input:
    ;

%%

int yylex() {
    static yyFlexLexer lexer; 

    return lexer.yylex(); 
}

int yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}
