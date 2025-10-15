%{
    #include <iostream>
    #include "tokens.h"
%}

%option noyywrap
delim [ \t\n]
whitespaces {delim}+
classes_stereotypes (event|situation|process|category|mixin|phaseMixin|roleMixin|historicalRoleMixin|kind|collective|quantity|quality|mode|intrisicMode|extrinsicMode|subkind|phase|role|historicalRole)
relations_stereotypes (material|derivation|comparative|mediation|characterization|externalDependence|componentOf|memberOf|subCollectionOf|subQualityOf|instantiation|termination|participational|participation|historicalDependence|creation|manifestation|bringsAbout|triggers|composition|aggregation|inherence|value|formal|constitution)
type (number|string|boolean|date|time|datetime)
meta (ordered|const|derived|subsets|redefines)

%%
{whitespaces}            ;  // Ignorar espaços em branco.
genset                   { return GENSET; }
disjoint                 { return DISJOINT; }
complete                 { return COMPLETE; }
general                  { return GENERAL; }
specifics                { return SPECIFICS; }
where                    { return WHERE; }
package                  { return PACKAGE; }
import                   { return IMPORT; }
functional-complexes     { return FUNCTIONAL_COMPLEXES; }
{classes_stereotypes}    { return CLASS_STEREOTYPE; }
{relations_stereotypes}  { return RELATIONS_STEREOTYPE; }
{meta}                   { return META; }
{type}                   { return TYPE; }
.                        { return -1; }  // Token inválido.
%%
