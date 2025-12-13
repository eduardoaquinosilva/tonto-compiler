%{
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <FlexLexer.h>
#include "stats.h"
#include "ast.h"

extern int yylex();
extern int yyerror(const char *s);

SyntaxStats syntaxStats;
std::string currentClassName = "";
extern int GetCurrentColumn();
bool errorOccurred = false;
%}

%define parse.error verbose

%code requires {
    #include <string>
    #include <vector>
}

%union {
    char* sval;
    int ival;
    std::vector<std::string>* listVal;
}

%token <sval> PACKAGE "package" CLASS_NAME "class name" CLASS_STEREOTYPE "class stereotype" SPECIALIZES "specializes" DATATYPE "datatype" NEW_TYPE "new dataype name" ENUM "enum" RELATIONS_STEREOTYPE "relation stereotype" RELATION_NAME "relation name"

%token LBRACE "{" RBRACE "}" COLON ":" TYPE "primitive data type" META "meta attributes" INSTANCE_NAME "instance name" COMMA "," DISJOINT "disjoint" OVERLAPPING "overlapping" COMPLETE "complete" INCOMPLETE "incomplete" GENSET "genset" WHERE "where" GENERAL "general" SPECIFICS "specifics" LBRACKET "[" RBRACKET "]" LRELATION "<>--" MRELATION "--" RRELATION "--<>" DIGIT "digit" DOTDOT ".." ASTHERISTICS "*" AT "@" RELATION "relation" IMPORT "import" FUNCTIONAL_COMPLEXES "functional-complexes" LP "(" RP ")" NUMBER "number" UNKOWN ""

%type <listVal> classList
%type <sval> cardinalityStructure
%type <ival> restriction restrictionList

%start program

%%
program : importHead package declarations
        | importHead error { yyerrok; } declarations
        ;

declarations : declarations declaration
             | 
             ;

declaration : class
            | dataType
            | enum
            | generalizations
            | externalRelation
            | error { yyclearin; }
            ;

importHead : importHead import
           |
           ;

import : IMPORT CLASS_NAME
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

            ClassNode node;
            node.name = std::string($2);
            node.stereotype = std::string($1);
            node.parents = std::vector<std::string>();
            syntaxStats.classes.push_back(node);

            free($2); free($1);
          }
          ;

classTail : LBRACE attribute internalRelation RBRACE
          | SPECIALIZES CLASS_NAME LBRACE attribute internalRelation RBRACE
          {
            if (!syntaxStats.classes.empty()) {
                syntaxStats.classes.back().parents.push_back(std::string($2));
            }
            
            free($2);
          }
          | SPECIALIZES classList
          {
            if (!syntaxStats.classes.empty() && $2 != nullptr) {
                syntaxStats.classes.back().parents = *($2);
            }

            delete $2;
          }
          |
          ;

attribute : RELATION_NAME COLON attributeType attributeCardinality attributeTail attribute
          |
          ;

attributeTail : LBRACE META RBRACE
              |
              ;

attributeType : TYPE
              | NEW_TYPE
              | CLASS_NAME
              ;

attributeCardinality : LBRACKET innerAttributeCardinality RBRACKET
                     |
                     ;

innerAttributeCardinality : DIGIT
                          | DIGIT DOTDOT DIGIT
                          | DIGIT DOTDOT ASTHERISTICS
                          | ASTHERISTICS
                          ;

dataType : DATATYPE NEW_TYPE LBRACE attribute RBRACE
         {
            syntaxStats.dataTypeNames.push_back(std::string($2));
            free($2);
         }
         | DATATYPE NEW_TYPE LBRACE error RBRACE
         {
            yyerrok;
            free($2);
         }
         ;

enum : ENUM CLASS_NAME LBRACE enumTail RBRACE
     {
        syntaxStats.enumNames.push_back(std::string($2));
        free($2);
     }
     | ENUM CLASS_NAME LBRACE error RBRACE
     {
        yyerrok;
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

generalizations : restrictionList GENSET CLASS_NAME WHERE classList SPECIALIZES CLASS_NAME
                {
                    GensetNode node;
                    node.name = std::string($3);
                    node.parent = std::string($7);
                    node.children = *($5);
                    node.isDisjoint = ($1 & 1);
                    node.isComplete = ($1 & 2);

                    syntaxStats.gensets.push_back(node);

                    delete $5;
                    free($3); free($7);
                }
                | GENSET CLASS_NAME WHERE classList SPECIALIZES CLASS_NAME
                {
                    GensetNode node;
                    node.name = std::string($2);
                    node.parent = std::string($6);
                    
                    node.children = *($4);
                    syntaxStats.gensets.push_back(node);
                    
                    delete $4;
                    free($2); free($6);
                }
                | restrictionList GENSET CLASS_NAME LBRACE GENERAL CLASS_NAME opt_comma SPECIFICS classList RBRACE
                {
                    GensetNode node;
                    node.name = std::string($3);
                    node.parent = std::string($6);
                    node.children = *($9);
                    node.isDisjoint = ($1 & 1);
                    node.isComplete = ($1 & 2);

                    syntaxStats.gensets.push_back(node);
                    
                    delete $9;
                    free($3); free($6);
                }
                | GENSET CLASS_NAME LBRACE GENERAL CLASS_NAME opt_comma SPECIFICS classList RBRACE
                {
                    GensetNode node;
                    node.name = std::string($2);
                    node.parent = std::string($5);
                    
                    node.children = *($8);
                    syntaxStats.gensets.push_back(node);

                    delete $8;
                    free($2); free($5);
                }
                | GENSET CLASS_NAME LBRACE error RBRACE
                {
                    yyerrok; 
                    free($2);
                }
                ;

restriction : DISJOINT { $$ = 1; }
            | OVERLAPPING { $$ = 2; }
            | COMPLETE { $$ = 4; }
            | INCOMPLETE { $$ = 8; }
            ;

restrictionList : restriction { $$ = $1;}
                | restrictionList restriction { $$ = $1 | $2; }
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

internalRelation : AT RELATIONS_STEREOTYPE cardinalityStructure CLASS_NAME internalRelation
                 {
                    RelationNode node;
                    node.type = "Internal";
                    node.stereotype = std::string($2);
                    node.source = currentClassName;
                    node.target = std::string($4);
                    node.name = std::string($3);

                    syntaxStats.relations.push_back(node);
                    free($2); free($3); free($4);
                 }
                 |
                 ;

externalRelation : AT RELATIONS_STEREOTYPE RELATION CLASS_NAME cardinalityStructure CLASS_NAME
                 {
                    RelationNode node;
                    node.type = "External";
                    node.stereotype = std::string($2);
                    node.source = std::string($4); 
                    node.name = std::string($5);
                    node.target = std::string($6);

                    syntaxStats.relations.push_back(node);
                    free($2); free($4); free($5); free($6);
                 }
                 ;

cardinalityStructure : cardinality specialCardinalitySymbol cardinality { $$ = strdup(""); }
                     | cardinality specialCardinalitySymbol RELATION_NAME specialCardinalitySymbol cardinality { $$ = $3; }
                     | specialCardinalitySymbol RELATION_NAME specialCardinalitySymbol cardinality { $$ = $2; }
                     | specialCardinalitySymbol cardinality { $$ = strdup(""); }
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
        std::cerr << NORMAL_RED << "Lexical Error:" << BOLD_RED << " '" << currentScanner->YYText() << "'" << COLOR_RESET << " is not a " << BOLD_YELLOW << "valid identifier" << COLOR_RESET << " at line " << currentScanner->lineno() << ", column " << GetCurrentColumn() << std::endl;
        return UNKOWN;
    }

    if (token == PACKAGE || token == CLASS_NAME || token == CLASS_STEREOTYPE || token == SPECIALIZES || token == DATATYPE || token == NEW_TYPE || token == ENUM || token == RELATIONS_STEREOTYPE || token == RELATION_NAME) {
        yylval.sval = strdup(currentScanner->YYText());
    }

    return token;
}

int yyerror(const char *s) {
    errorOccurred = true;
    std::string errorMessage = s;

    if (currentScanner) {
        std::string lexeme = currentScanner->YYText();

        if (!lexeme.empty() && errorMessage.find(lexeme) == std::string::npos) {
            std::string anchor = "unexpected ";
            size_t start = errorMessage.find(anchor);

            if (start != std::string::npos) {
                size_t tokenStart = start + anchor.length();
                size_t insertPos = errorMessage.find(',', tokenStart);
                
                std::string insertion = " '" + lexeme + "'";
                
                if (insertPos == std::string::npos) {
                    errorMessage += insertion; 
                } else {
                    errorMessage.insert(insertPos, insertion);
                }
            } else {
                errorMessage += " '" + lexeme + "'";
            }
        }
    }

    size_t unexpectedPos = errorMessage.find("unexpected ");
    size_t expectingPos = errorMessage.find("expecting ");
    
    size_t redStart = std::string::npos;
    size_t redEnd = std::string::npos;
    size_t yellowStart = std::string::npos;

    if (unexpectedPos != std::string::npos) {
        redStart = unexpectedPos + 11;
        
        size_t boundary = errorMessage.find(", expecting ", redStart);
        
        if (boundary != std::string::npos) {
            redEnd = boundary;
        } else {
            redEnd = errorMessage.length();
        }
    }

    if (expectingPos != std::string::npos) {
        yellowStart = expectingPos + 10;
    }

    if (yellowStart != std::string::npos) {
        errorMessage += COLOR_RESET;
        errorMessage.insert(yellowStart, BOLD_YELLOW);
    }

    if (redStart != std::string::npos && redEnd != std::string::npos) {
        errorMessage.insert(redEnd, COLOR_RESET);
        errorMessage.insert(redStart, BOLD_RED);
    }

    std::cerr << NORMAL_RED << "Syntax Error: " << COLOR_RESET << errorMessage;
    
    if (currentScanner) {
        std::cerr << " at line " << currentScanner->lineno() << ", column " << GetCurrentColumn();
    }
    
    std::cerr << std::endl;
    return 0;
}
