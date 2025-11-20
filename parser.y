%{
#include <stdio.h>
#include <FlexLexer.h>

extern int yylex();
extern int yyerror(const char *s);
%}

%token PACKAGE, CLASS_NAME, LBRACE, RBRACE, SPECIALIZES, CLASS_STEREOTYPE, ATTRIBUTE_NAME, COLON, TYPE, DATATYPE, NEW_TYPE, META

%%

package : PACKAGE CLASS_NAME
        ;

class : classHead LBRACE attribute RBRACE
      | SPECIALIZES CLASS_NAME
      ;

classHead : CLASS_STEREOTYPE CLASS_NAME
          ;

attribute : ATTRIBUTE_NAME COLON TYPE attributeTail attribute
          |
          ;

attributeTail : LBRACE META RBRACE
              |
              ;

dataType : DATATYPE NEW_TYPE LBRACE attribute RBRACE
         ;

enum : ENUM CLASS_NAME LBRACE enumTail RBRACE
     ;

enumTail : INTANCE_NAME COMMA enumTail
         |
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
