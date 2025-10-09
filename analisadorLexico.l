%{
    #include <iostream>
    using namespace std;
%}

%option noyywrap

%%

"genset"                { cout << "TOKEN_GENSET" << endl; }
"disjoint"              { cout << "TOKEN_DISJOINT" << endl; }
"complete"                { cout << "TOKEN_COMPLETE" << endl; }
"general"              { cout << "TOKEN_GENERAL" << endl; }
"specifics"                { cout << "TOKEN_SPECIFICS" << endl; }
"where"              { cout << "TOKEN_WHERE" << endl; }
"package"                { cout << "TOKEN_PACKAGE" << endl; }

%%

int main() {
    yylex();
    return 0;
}