%{
#include <stdio.h>
#include <string>
#include <cstring>
#include <FlexLexer.h>
#include "stats.h"

extern int yylex();
extern int yyerror(const char *s);

SyntaxStats syntaxStats;
%}

%union {
    char* sval;
    int ival;
}

%token <sval> PACKAGE CLASS_NAME CLASS_STEREOTYPE SPECIALIZES

%token LBRACE RBRACE RELATION_NAME COLON TYPE DATATYPE NEW_TYPE META ENUM INSTANCE_NAME COMMA DISJOINT OVERLAPPING COMPLETE INCOMPLETE GENSET WHERE GENERAL SPECIFICS RELATIONS_STEREOTYPE LBRACKET RBRACKET LRELATION MRELATION RRELATION DIGIT DOTDOT ASTHERISTICS AT RELATION IMPORT FUNCTIONAL_COMPLEXES LP RP NUMBER

%%

package : PACKAGE CLASS_NAME
        {
            syntaxStats.packageNames.push_back(std::string($2));
            free($2);
        }
        ;

class : classHead LBRACE attribute internalRelation RBRACE
      | SPECIALIZES CLASS_NAME
      {   
        syntaxStats.classNames.push_back(std::string($2));
        free($2);
      }
      ;

classHead : CLASS_STEREOTYPE CLASS_NAME
          {   
            syntaxStats.classNames.push_back(std::string($2));
            free($2);
          }
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

static yyFlexLexer* currentScanner = nullptr;

void setScanner(yyFlexLexer* scanner) {
    currentScanner = scanner;
}

int yylex() {
    if (!currentScanner) return 0;

    int token = currentScanner->yylex();

    if (token == PACKAGE || token == CLASS_NAME || token == CLASS_STEREOTYPE || token == SPECIALIZES) {
        yylval.sval = strdup(currentScanner->YYText());
    }

    return token;
}

int yyerror(const char *s) {
    fprintf(stderr, "Syntax Error: %s\n", s);
    if (currentScanner) {
        fprintf(stderr, "  at line %d, near token '%s'\n",
                currentScanner->lineno(), 
                currentScanner->YYText());
    }
    return 0;
}
