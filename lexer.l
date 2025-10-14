%{
    #include <iostream>
    #include "tokens.h"
    using namespace std;
%}

%option noyywrap

%%

"genset"                { return  GENSET; }
"disjoint"              { return  DISJOINT; }
"complete"              { return  COMPLETE; }
"general"               { return  GENERAL; }
"specifics"             { return  SPECIFICS; }
"where"                 { return  WHERE; }
"package"               { return  PACKAGE; }
. ;
%%
