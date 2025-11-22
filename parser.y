%{
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <FlexLexer.h>
#include "stats.h"

extern int yylex();
extern int yyerror(const char *s);

SyntaxStats syntaxStats;
std::string currentClassName = "";
%}

// %define parse.trace

%code requires {
    #include <string>
    #include <vector>
}

%union {
    char* sval;
    int ival;
    std::vector<std::string>* listVal;
}

%token <sval> PACKAGE CLASS_NAME CLASS_STEREOTYPE SPECIALIZES DATATYPE NEW_TYPE ENUM RELATIONS_STEREOTYPE RELATION_NAME

%token LBRACE RBRACE COLON TYPE META INSTANCE_NAME COMMA DISJOINT OVERLAPPING COMPLETE INCOMPLETE GENSET WHERE GENERAL SPECIFICS LBRACKET RBRACKET LRELATION MRELATION RRELATION DIGIT DOTDOT ASTHERISTICS AT RELATION IMPORT FUNCTIONAL_COMPLEXES LP RP NUMBER UNKOWN

%type <listVal> classList
%type <sval> cardinalityStructure

%start program

%%
program : declarations
        ;

declarations : declarations declaration
             | 
             ;

declaration : package
            | class
            | dataType
            | enum
            | generalizations
            | externalRelation
            ;

package : PACKAGE CLASS_NAME
        {
            syntaxStats.packageNames.push_back(std::string($2));
            free($2);
        }
        ;

class : classHead classTail
      ;

classHead : CLASS_STEREOTYPE CLASS_NAME
          {
            currentClassName = std::string($2);

            syntaxStats.classNames.push_back(std::string($2));
            free($2); free($1);
          }
          ;

classTail : LBRACE attribute internalRelation RBRACE
          | SPECIALIZES CLASS_NAME
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
         {
            syntaxStats.dataTypeNames.push_back(std::string($2));
            free($2);
         }
         ;

enum : ENUM CLASS_NAME LBRACE enumTail RBRACE
     {
        syntaxStats.enumNames.push_back(std::string($2));
        free($2);
     }
     ;

enumTail : INSTANCE_NAME
         | enumTail COMMA INSTANCE_NAME
         | 
         ;

opt_comma : COMMA 
          | 
          ;

generalizations : restrictionsHead GENSET CLASS_NAME WHERE classList SPECIALIZES CLASS_NAME
                {
                    GensetInfo info;
                    info.name = std::string($3);
                    info.parent = std::string($7);

                    info.children = *($5);
                    syntaxStats.gensets.push_back(info);

                    delete $5;
                    free($3); free($7);
                }
                | GENSET CLASS_NAME LBRACE GENERAL CLASS_NAME opt_comma SPECIFICS classList RBRACE
                {
                    GensetInfo info;
                    info.name = std::string($2);
                    info.parent = std::string($5);
                    
                    info.children = *($8);
                    syntaxStats.gensets.push_back(info);

                    delete $8;
                    free($2); free($5);
                }
                ;

restriction : DISJOINT
            | OVERLAPPING
            | COMPLETE
            | INCOMPLETE
            ;

restrictionsHead : restrictionsHead restriction {}
                 | {}
                 ;

classList : CLASS_NAME
          {
              $$ = new std::vector<std::string>();
              $$->push_back(std::string($1));
              free($1);
          }
          | classList COMMA CLASS_NAME
          {
              $$ = $1;
              $$->push_back(std::string($3));
              free($3);
          }

internalRelation : AT RELATIONS_STEREOTYPE cardinalityStructure CLASS_NAME
                 {
                    RelationInfo info;
                    info.type = "Internal";
                    info.stereotype = std::string($2);
                    info.source = currentClassName;
                    info.target = std::string($4);
                    info.name = std::string($3);

                    syntaxStats.relations.push_back(info);
                    free($2); free($3); free($4);
                 }
                 |
                 ;

externalRelation : AT RELATIONS_STEREOTYPE RELATION CLASS_NAME cardinalityStructure CLASS_NAME
                 {
                    RelationInfo info;
                    info.type = "External";
                    info.stereotype = std::string($2);
                    info.source = std::string($4); 
                    info.name = std::string($5);
                    info.target = std::string($6);

                    syntaxStats.relations.push_back(info);
                    free($2); free($4); free($5); free($6);
                 }
                 ;

cardinalityStructure : cardinality specialCardinalitySymbol cardinality
                     {
                        $$ = strdup("");
                     }
                     | cardinality specialCardinalitySymbol RELATION_NAME specialCardinalitySymbol cardinality
                     {
                        $$ = $3;
                     }
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

    if (token < 0) {
        std::cerr << "Lexical Error: '" << currentScanner->YYText() << "'' is not a valid identifier at line " << currentScanner->lineno() << std::endl;

        //std::cerr << "current line: " << currentScanner->yycolumn << std::endl;
        // fprintf(stderr, "Lexical Error: Unexpected input '%s' at line %d\n", currentScanner->YYText(), currentScanner->lineno());
        return UNKOWN;
    }

    if (token == PACKAGE || token == CLASS_NAME || token == CLASS_STEREOTYPE || token == SPECIALIZES || token == DATATYPE || token == NEW_TYPE || token == ENUM || token == RELATIONS_STEREOTYPE || token == RELATION_NAME) {
        yylval.sval = strdup(currentScanner->YYText());
    }

    return token;
}

int yyerror(const char *s) {
    std::cerr << "Syntax Error: " << s;

    if (currentScanner) {
        std::cerr << " at line " << currentScanner->lineno() << ", near token '" << currentScanner->YYText() << "'";
    }
    
    std::cerr << std::endl;

    /* fprintf(stderr, "Syntax Error: %s\n", s);
    if (currentScanner) {
        fprintf(stderr, "  at line %d, near token '%s'\n", currentScanner->lineno(), currentScanner->YYText());
    } */
    return 0;
}
