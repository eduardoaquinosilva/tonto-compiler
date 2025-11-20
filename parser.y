%{
#include <stdio.h>
#include <FlexLexer.h>

extern int yylex();
extern int yyerror(const char *s);
%}

%token PACKAGE, CLASS_NAME, LBRACE, RBRACE, SPECIALIZES, CLASS_STEREOTYPE, RELATION_NAME, COLON, TYPE, DATATYPE, NEW_TYPE, META ENUM, INSTANCE_NAME, COMMA, DISJOINT, OVERLAPPING, COMPLETE, INCOMPLETE, GENSET, WHERE, GENERAL, SPECIFICS, RELATIONS_STEREOTYPE, LBRACKET, RBRACKET, LRELATION, MRELATION, RRELATION, DIGIT, DOTDOT, ASTHERISTICS, AT, RELATION

%%
package : PACKAGE CLASS_NAME
        ;

class : classHead LBRACE attribute internalRelation RBRACE
      | SPECIALIZES CLASS_NAME
      ;

classHead : CLASS_STEREOTYPE CLASS_NAME
          ;

attribute : RELATION_NAME COLON TYPE attributeTail attribute
          |
          ;

attributeTail : LBRACE META RBRACE
              |
              ;

dataType : DATATYPE NEW_TYPE LBRACE attribute RBRACE
         ;

enum : ENUM CLASS_NAME LBRACE enumTail RBRACE
     ;

enumTail : INSTANCE_NAME COMMA enumTail
         |
         ;

generalizations : restrictionsHead restriction GENSET CLASS_NAME WHERE classListHead classListElement SPECIALIZES CLASS_NAME
                | GENSET CLASS_NAME LBRACE GENERAL CLASS_NAME SPECIFICS classListHead classListElement RBRACE
                ;

restriction : DISJOINT
            | OVERLAPPING
            | COMPLETE
            | INCOMPLETE
            |
            ;

restrictionsHead : restrictionsHead restriction
                 |
                 ;

classListElement : CLASS_NAME
                 ;

classListHead : classListHead COMMA classListElement
              |
              ;

internalRelation : AT RELATIONS_STEREOTYPE cardinalityStructure CLASS_NAME
         |
         ;

externalRelation : AT RELATIONS_STEREOTYPE RELATION CLASS_NAME cardinalityStructure CLASS_NAME
         ;

cardinalityStructure : cardinality specialCardinalitySymbol cardinality
                     ;

cardinality : LBRACKET cardinalityContent RBRACKET
            ;

specialCardinalitySymbol : LRELATION
                         | MRELATION
                         | RRELATION
                         ;

cardinalityContent : DIGIT cardinalityContentTail
                   ;

cardinalityContentTail : DOTDOT ASTHERISTICS
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
