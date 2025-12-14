%{
    #include <iostream>
    #include "parser.h"

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
classes_stereotypes (event|situation|process|category|mixin|phaseMixin|roleMixin|historicalRoleMixin|kind|collective|quantity|quality|mode|intrinsicMode|extrinsicMode|subkind|phase|role|historicalRole|relator)
relations_stereotypes (material|derivation|comparative|mediation|characterization|externalDependence|componentOf|memberOf|subCollectionOf|subQualityOf|instantiation|termination|participational|participation|historicalDependence|creation|manifestation|bringsAbout|triggers|composition|aggregation|inherence|value|formal|constitution)
type (number|string|boolean|date|time|datetime|int)
meta (ordered|const|derived|subsets|redefines)
inline_comment "//".*

%%
{endline}                      {}
{whitespaces}                  {}
{inline_comment}               {}
 /* inicio Palavras Reservadas */
genset                         { return GENSET; }
disjoint                       { return DISJOINT; }
complete                       { return COMPLETE; }
incomplete                     { return INCOMPLETE; }
overlapping                    { return OVERLAPPING; }
general                        { return GENERAL; }
specifics                      { return SPECIFICS; }
where                          { return WHERE; }
package                        { return PACKAGE; }
import                         { return IMPORT; }
functional-complexes           { return FUNCTIONAL_COMPLEXES; }
datatype                       { return DATATYPE; }
specializes                    { return SPECIALIZES; }
enum                           { return ENUM; }
relation                       { return RELATION; }
 /* fim Palavras Reservadas */
{classes_stereotypes}          { return CLASS_STEREOTYPE; }      /* Esteriótipos de Classe */
{relations_stereotypes}        { return RELATIONS_STEREOTYPE; }  /* Esteriótipos de Relações */
{meta}                         { return META; }                  /* Meta-atributos */
{type}                         { return TYPE; }                  /* Tipos de Dados Nativos */
({letter})*DataType            { return NEW_TYPE;}               /* Novos Tipos */
({letter})[_A-Za-z]*{digit}+   { return INSTANCE_NAME;}          /* Convenção para Nomes de Instâncias */
[A-Z]("_"|{letter})*           { return CLASS_NAME; }            /* Convenção para Nomes de Classes */
[a-z]("_"|{letter})*           { return RELATION_NAME; }         /* Convenção para Nomes de Relações */
 /* inicio Símbolos Especiais */
"{"                            { return LBRACE; }
"}"                            { return RBRACE; }
"("                            { return LP; }
")"                            { return RP; }
"["                            { return LBRACKET; }
"]"                            { return RBRACKET; }
".."                           { return DOTDOT; }
"<>--"                         { return LRELATION; }
"--<>"                         { return RRELATION; }
"--"                           { return MRELATION; }
"*"                            { return ASTHERISTICS; }
"@"                            { return AT; }
":"                            { return COLON; }
","                            { return COMMA; }
 /* fim Símbolos Especiais */
{digit}                        { return DIGIT; }   /* Números inteiros */
{number}                       { return NUMBER; }  /* Números */
[-_A-Za-z0-9]*-[A-Za-z0-9_-]*  { return -1; }
[0-9]+[_a-zA-Z][a-zA-Z0-9_]*   { return -1; }                      /* Token inválido */
[a-zA-Z0-9_]+                  { return -1; }                      /* Token inválido */
.                              { return -1; }                      /* Token Inválido */
%%

int GetCurrentColumn() {
    return yycolumn;
}

void ResetCounters() {
    yycolumn = 1;
    tmpcolumn = 1;
}
