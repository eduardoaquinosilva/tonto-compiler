#include "parser.h"
#include "tokens.h"
#include <iostream>
using std::cout;

void Parser::Start()
{
    // enquanto não atingir o fim da entrada
    while ((lookahead = scanner.yylex()) != 0)
    {
        // trata o token recebido do analisador léxico
        switch(lookahead)
        {
            case GENSET: cout << "genset\n"; break;
            case DISJOINT: cout << "disjoint\n"; break;
            case COMPLETE: cout << "complete\n"; break;
            case GENERAL: cout << "general\n"; break;
            case SPECIFICS: cout << "specifics\n"; break;
            case WHERE: cout << "where\n"; break;
            case PACKAGE: cout << "package\n"; break;
        }
    }
}
