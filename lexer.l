%{
    #include <iostream>
    #include "tokens.h"
    extern int columnNumber;
    extern int keyWordsCount;
%}

%option yylineno
endline [\n]
tab [\t]
space [ ]
whitespaces ({tab}|{space})+
letter [A-Za-z]
digit [0-9]
classes_stereotypes (event|situation|process|category|mixin|phaseMixin|roleMixin|historicalRoleMixin|kind|collective|quantity|quality|mode|intrisicMode|extrinsicMode|subkind|phase|role|historicalRole)
relations_stereotypes (material|derivation|comparative|mediation|characterization|externalDependence|componentOf|memberOf|subCollectionOf|subQualityOf|instantiation|termination|participational|participation|historicalDependence|creation|manifestation|bringsAbout|triggers|composition|aggregation|inherence|value|formal|constitution)
type (number|string|boolean|date|time|datetime)
meta (ordered|const|derived|subsets|redefines)

%%
{endline}                  { columnNumber = 1; }
{whitespaces}              { columnNumber += yyleng; }
 /* inicio Palavras Reservadas */
genset                     { columnNumber += yyleng; keyWordsCount++; return GENSET; }
disjoint                   { columnNumber += yyleng; keyWordsCount++; return DISJOINT; }
complete                   { columnNumber += yyleng; keyWordsCount++; return COMPLETE; }
general                    { columnNumber += yyleng; keyWordsCount++; return GENERAL; }
specifics                  { columnNumber += yyleng; keyWordsCount++; return SPECIFICS; }
where                      { columnNumber += yyleng; keyWordsCount++; return WHERE; }
package                    { columnNumber += yyleng; keyWordsCount++; return PACKAGE; }
import                     { columnNumber += yyleng; keyWordsCount++; return IMPORT; }
functional-complexes       { columnNumber += yyleng; keyWordsCount++; return FUNCTIONAL_COMPLEXES; }
 /* fim Palavras Reservadas */
{classes_stereotypes}      { columnNumber += yyleng; return CLASS_STEREOTYPE; }      /* Esteriótipos de Classe */
{relations_stereotypes}    { columnNumber += yyleng; return RELATIONS_STEREOTYPE; }  /* Esteriótipos de Relações */
{meta}                     { columnNumber += yyleng; return META; }                  /* Meta-atributos */
{type}                     { columnNumber += yyleng; return TYPE; }                  /* Tipos de Dados Nativos */
[A-Z][A-Za-z]*DataType     { columnNumber += yyleng; return NEW_TYPE;}               /* Novos Tipos */
[a-zA-Z][_A-Za-z]*{digit}+ { columnNumber += yyleng; return INSTANCE_NAME;}          /* Convenção para Nomes de Instâncias */
[A-Z]("_"|{letter})*       { columnNumber += yyleng; return CLASS_NAME; }            /* Convenção para Nomes de Classes */
[a-z]("_"|{letter})*       { columnNumber += yyleng; return RELATION_NAME; }         /* Convenção para Nomes de Relações */
 /* inicio Símbolos Especiais */
"{"                        { columnNumber += yyleng; return LBRACE; }
"}"                        { columnNumber += yyleng; return RBRACE; }
"("                        { columnNumber += yyleng; return LP; }
")"                        { columnNumber += yyleng; return RP; }
"["                        { columnNumber += yyleng; return LBRACKET; }
"]"                        { columnNumber += yyleng; return RBRACKET; }
".."                       { columnNumber += yyleng; return DOTDOT; }
"<>--"                     { columnNumber += yyleng; return L_RELATION; }
"--<>"                     { columnNumber += yyleng; return R_RELATION; }
"*"                        { columnNumber += yyleng; return ASTHERISTICS; }
"@"                        { columnNumber += yyleng; return AT; }
":"                        { columnNumber += yyleng; return COLON; }
 /* fim Símbolos Especiais */
.                          { columnNumber += yyleng; return -1; }                    /* Token Inválido */
%%
