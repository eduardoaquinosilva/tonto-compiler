%{
    #include <iostream>
    #include "tokens.h"
    extern int columnNumber;
    extern int keyWordsCount;
    extern int classCount;
    extern int relationsCount;
    extern int instanceCount;
    extern int classStereotypesCount;
    extern int relationsStereotypesCount;
    extern int metaAttributesCount;
    extern int typesCount;
    extern int newTypesCount;
    extern int specialSymbolsCount;
    extern int numbersCount;

    int yycolumn = 1;
    int tmpcolumn = 1;

    #define YY_USER_ACTION \
    yycolumn = tmpcolumn; \
    for (int i = 0; i < yyleng; i++) { \
        if (yytext[i] == '\n') \
            tmpcolumn = 1; \
        else \
            tmpcolumn++; \
    }
%}

%option c++
%option yylineno

endline [\n]
tab [\t]
space [ ]
whitespaces ({tab}|{space})+
letter [A-Za-z]
digit [0-9]
number {digit}+(\.{digit}+)?(E[+-]?{digit}+)?
classes_stereotypes (event|situation|process|category|mixin|phaseMixin|roleMixin|historicalRoleMixin|kind|collective|quantity|quality|mode|intrisicMode|extrinsicMode|subkind|phase|role|historicalRole)
relations_stereotypes (material|derivation|comparative|mediation|characterization|externalDependence|componentOf|memberOf|subCollectionOf|subQualityOf|instantiation|termination|participational|participation|historicalDependence|creation|manifestation|bringsAbout|triggers|composition|aggregation|inherence|value|formal|constitution)
type (number|string|boolean|date|time|datetime)
meta (ordered|const|derived|subsets|redefines)

%%
{endline}                    { columnNumber = 1; }
{whitespaces}                { columnNumber += yyleng; }
 /* inicio Palavras Reservadas */
genset                       { columnNumber += yyleng; keyWordsCount++; return GENSET; }
disjoint                     { columnNumber += yyleng; keyWordsCount++; return DISJOINT; }
complete                     { columnNumber += yyleng; keyWordsCount++; return COMPLETE; }
general                      { columnNumber += yyleng; keyWordsCount++; return GENERAL; }
specifics                    { columnNumber += yyleng; keyWordsCount++; return SPECIFICS; }
where                        { columnNumber += yyleng; keyWordsCount++; return WHERE; }
package                      { columnNumber += yyleng; keyWordsCount++; return PACKAGE; }
import                       { columnNumber += yyleng; keyWordsCount++; return IMPORT; }
functional-complexes         { columnNumber += yyleng; keyWordsCount++; return FUNCTIONAL_COMPLEXES; }
datatype                     { columnNumber += yyleng; keyWordsCount++; return DATATYPE; }
specializes                  { columnNumber += yyleng; keyWordsCount++; return SPECIALIZES; }
enum                         { columnNumber += yyleng; keyWordsCount++; return ENUM; }
 /* fim Palavras Reservadas */
{classes_stereotypes}        { columnNumber += yyleng; classStereotypesCount++; return CLASS_STEREOTYPE; }      /* Esteriótipos de Classe */
{relations_stereotypes}      { columnNumber += yyleng; relationsStereotypesCount++; return RELATIONS_STEREOTYPE; }  /* Esteriótipos de Relações */
{meta}                       { columnNumber += yyleng; metaAttributesCount++; return META; }                  /* Meta-atributos */
{type}                       { columnNumber += yyleng; typesCount++; return TYPE; }                  /* Tipos de Dados Nativos */
({letter})*DataType          { columnNumber += yyleng; newTypesCount++; return NEW_TYPE;}               /* Novos Tipos */
({letter})[_A-Za-z]*{digit}+ { columnNumber += yyleng; instanceCount++; return INSTANCE_NAME;}          /* Convenção para Nomes de Instâncias */
[A-Z]("_"|{letter})*         { columnNumber += yyleng; classCount++; return CLASS_NAME; }            /* Convenção para Nomes de Classes */
[a-z]("_"|{letter})*         { columnNumber += yyleng; relationsCount++; return RELATION_NAME; }         /* Convenção para Nomes de Relações */
 /* inicio Símbolos Especiais */
"{"                          { columnNumber += yyleng; specialSymbolsCount++; return LBRACE; }
"}"                          { columnNumber += yyleng; specialSymbolsCount++; return RBRACE; }
"("                          { columnNumber += yyleng; specialSymbolsCount++; return LP; }
")"                          { columnNumber += yyleng; specialSymbolsCount++; return RP; }
"["                          { columnNumber += yyleng; specialSymbolsCount++; return LBRACKET; }
"]"                          { columnNumber += yyleng; specialSymbolsCount++; return RBRACKET; }
".."                         { columnNumber += yyleng; specialSymbolsCount++; return DOTDOT; }
"<>--"                       { columnNumber += yyleng; specialSymbolsCount++; return LRELATION; }
"--<>"                       { columnNumber += yyleng; specialSymbolsCount++; return RRELATION; }
"*"                          { columnNumber += yyleng; specialSymbolsCount++; return ASTHERISTICS; }
"@"                          { columnNumber += yyleng; specialSymbolsCount++; return AT; }
":"                          { columnNumber += yyleng; specialSymbolsCount++; return COLON; }
","                          { columnNumber += yyleng; specialSymbolsCount++; return COMMA; }
 /* fim Símbolos Especiais */
{number}                      { columnNumber += yyleng; numbersCount++; return NUMBER; }  /* Números */
[-_A-Za-z0-9]*-[A-Za-z0-9_-]* { columnNumber += yyleng; return -1; }
[0-9][_A-Za-z0-9]*            { columnNumber += yyleng; return -1; }                      /* Token inválido */
[_A-Za-z][_A-Za-z0-9]*        { columnNumber += yyleng; return -1; }                      /* Token inválido */
.                             { columnNumber += yyleng; return -1; }                      /* Token Inválido */
%%
