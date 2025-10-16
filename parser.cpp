#include "parser.h"
#include "tokens.h"
#include <iostream>
using std::cout;

int columnNumber = 1;
int keyWordsCount = 0;

void Parser::Start()
{
    cout << "Token\t\tLinha\tColuna\n";
    cout << "-------------------------------\n";

    // enquanto não atingir o fim da entrada
    while ((lookahead = scanner.yylex()) != 0)
    {
        int tokenStartColumn = columnNumber - scanner.YYLeng();

        // trata o token recebido do analisador léxico
        switch(lookahead)
        {
            case GENSET: cout << "<GENSET>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case DISJOINT: cout << "<DISJOINT>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case COMPLETE: cout << "<COMPLETE>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case GENERAL: cout << "<GENERAL>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case SPECIFICS: cout << "<SPECIFICS>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case WHERE: cout << "<WHERE>\t\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case PACKAGE: cout << "<PACKAGE>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case IMPORT: cout << "<IMPORT>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case FUNCTIONAL_COMPLEXES: cout << "<FUNCTIONAL_COMPLEXES>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            
            case TYPE: cout << "token type " << scanner.YYText() << '\n'; break;
            case NEW_TYPE: cout << "token new_type " << scanner.YYText() << '\n'; break;
            case INSTANCE_NAME: cout << "token name_instance " << scanner.YYText() << '\n'; break;
            default: cout << "token " << scanner.YYText() << '\n'; break;
        }
    }

    cout << "-------------------------------\n";
    cout << "Total de palavras reservadas: " << keyWordsCount << "\n";
}
