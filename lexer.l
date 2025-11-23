%{
    #include <iostream>
    #include "parser.h"
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
classes_stereotypes (event|situation|process|category|mixin|phaseMixin|roleMixin|historicalRoleMixin|kind|collective|quantity|quality|mode|intrisicMode|extrinsicMode|subkind|phase|role|historicalRole|relator)
relations_stereotypes (material|derivation|comparative|mediation|characterization|externalDependence|componentOf|memberOf|subCollectionOf|subQualityOf|instantiation|termination|participational|participation|historicalDependence|creation|manifestation|bringsAbout|triggers|composition|aggregation|inherence|value|formal|constitution)
type (number|string|boolean|date|time|datetime|int)
meta (ordered|const|derived|subsets|redefines)

%%
{endline}                      {}
{whitespaces}                  {}
 /* inicio Palavras Reservadas */
genset                         { keyWordsCount++; return GENSET; }
disjoint                       { keyWordsCount++; return DISJOINT; }
complete                       { keyWordsCount++; return COMPLETE; }
incomplete                     { keyWordsCount++; return INCOMPLETE; }
overlapping                    { keyWordsCount++; return OVERLAPPING; }
general                        { keyWordsCount++; return GENERAL; }
specifics                      { keyWordsCount++; return SPECIFICS; }
where                          { keyWordsCount++; return WHERE; }
package                        { keyWordsCount++; return PACKAGE; }
import                         { keyWordsCount++; return IMPORT; }
functional-complexes           { keyWordsCount++; return FUNCTIONAL_COMPLEXES; }
datatype                       { keyWordsCount++; return DATATYPE; }
specializes                    { keyWordsCount++; return SPECIALIZES; }
enum                           { keyWordsCount++; return ENUM; }
relation                       { keyWordsCount++; return RELATION; }
 /* fim Palavras Reservadas */
{classes_stereotypes}          { classStereotypesCount++; return CLASS_STEREOTYPE; }      /* Esteriótipos de Classe */
{relations_stereotypes}        { relationsStereotypesCount++; return RELATIONS_STEREOTYPE; }  /* Esteriótipos de Relações */
{meta}                         { metaAttributesCount++; return META; }                  /* Meta-atributos */
{type}                         { typesCount++; return TYPE; }                  /* Tipos de Dados Nativos */
({letter})*DataType            { newTypesCount++; return NEW_TYPE;}               /* Novos Tipos */
({letter})[_A-Za-z]*{digit}+   { instanceCount++; return INSTANCE_NAME;}          /* Convenção para Nomes de Instâncias */
[A-Z]("_"|{letter})*           { classCount++; return CLASS_NAME; }            /* Convenção para Nomes de Classes */
[a-z]("_"|{letter})*           { relationsCount++; return RELATION_NAME; }         /* Convenção para Nomes de Relações */
 /* inicio Símbolos Especiais */
"{"                            { specialSymbolsCount++; return LBRACE; }
"}"                            { specialSymbolsCount++; return RBRACE; }
"("                            { specialSymbolsCount++; return LP; }
")"                            { specialSymbolsCount++; return RP; }
"["                            { specialSymbolsCount++; return LBRACKET; }
"]"                            { specialSymbolsCount++; return RBRACKET; }
".."                           { specialSymbolsCount++; return DOTDOT; }
"<>--"                         { specialSymbolsCount++; return LRELATION; }
"--<>"                         { specialSymbolsCount++; return RRELATION; }
"--"                           { specialSymbolsCount++; return MRELATION; }
"*"                            { specialSymbolsCount++; return ASTHERISTICS; }
"@"                            { specialSymbolsCount++; return AT; }
":"                            { specialSymbolsCount++; return COLON; }
","                            { specialSymbolsCount++; return COMMA; }
 /* fim Símbolos Especiais */
{digit}                        { numbersCount++; return DIGIT; }   /* Números inteiros */
{number}                       { numbersCount++; return NUMBER; }  /* Números */
[-_A-Za-z0-9]*-[A-Za-z0-9_-]*  { return -1; }
[0-9][_A-Za-z0-9]*             { return -1; }                      /* Token inválido */
[_A-Za-z][_A-Za-z0-9]*         { return -1; }                      /* Token inválido */
.                              { return -1; }                      /* Token Inválido */
%%

int GetCurrentColumn() {
    return yycolumn;
}

void ResetCounters() {
    yycolumn = 1;
    tmpcolumn = 1;
}
