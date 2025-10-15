%{
    #include <iostream>
    #include "tokens.h"
    extern int columnNumber;
    extern int keyWordsCount;
%}

%option yylineno
%option noyywrap
endline [\n]
tab [\t]
space [ ]
whitespaces ({tab}|{space})+
classes_stereotypes (event|situation|process|category|mixin|phaseMixin|roleMixin|historicalRoleMixin|kind|collective|quantity|quality|mode|intrisicMode|extrinsicMode|subkind|phase|role|historicalRole)
relations_stereotypes (material|derivation|comparative|mediation|characterization|externalDependence|componentOf|memberOf|subCollectionOf|subQualityOf|instantiation|termination|participational|participation|historicalDependence|creation|manifestation|bringsAbout|triggers|composition|aggregation|inherence|value|formal|constitution)
type (number|string|boolean|date|time|datetime)
meta (ordered|const|derived|subsets|redefines)

%%
{endline}                { columnNumber = 1; }
{whitespaces}            { columnNumber += yyleng; }
genset                   { columnNumber += yyleng; keyWordsCount++; return GENSET; }
disjoint                 { columnNumber += yyleng; keyWordsCount++; return DISJOINT; }
complete                 { columnNumber += yyleng; keyWordsCount++; return COMPLETE; }
general                  { columnNumber += yyleng; keyWordsCount++; return GENERAL; }
specifics                { columnNumber += yyleng; keyWordsCount++; return SPECIFICS; }
where                    { columnNumber += yyleng; keyWordsCount++; return WHERE; }
package                  { columnNumber += yyleng; keyWordsCount++; return PACKAGE; }
import                   { columnNumber += yyleng; keyWordsCount++; return IMPORT; }
functional-complexes     { columnNumber += yyleng; keyWordsCount++; return FUNCTIONAL_COMPLEXES; }
{classes_stereotypes}    { columnNumber += yyleng; return CLASS_STEREOTYPE; }
{relations_stereotypes}  { columnNumber += yyleng; return RELATIONS_STEREOTYPE; }
{meta}                   { columnNumber += yyleng; return META; }
{type}                   { columnNumber += yyleng; return TYPE; }
.                        { columnNumber += yyleng; return -1; }  // Token inválido.
%%
