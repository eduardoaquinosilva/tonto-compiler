#include <iostream>
#include <fstream>
#include <sstream>
#include "parser.h"
#include "tokens.h"
using namespace std;

int columnNumber = 1;
int keyWordsCount = 0;
int classCount = 0;
int relationsCount = 0;
int instanceCount = 0;
int classStereotypesCount = 0;
int relationsStereotypesCount = 0;
int metaAttributesCount = 0;
int typesCount = 0;
int newTypesCount = 0;
int specialSymbolsCount = 0;

char **fileList = nullptr;
unsigned nFiles = 0;
unsigned currentFile = 0;
bool openFile = false;
ifstream fin;
stringstream ss;

Parser::Parser(char** list, unsigned count) : lookahead(0) 
{
    ::fileList = list;
    ::nFiles = count;
}

int yyFlexLexer::yywrap()
{
	if (openFile)
	{
		openFile = false;
		fin.close();
	}
	while (!openFile && (currentFile < nFiles))
    {
        fin.open(fileList[currentFile]); 

		if (fin.is_open())
        {
			openFile = true;
			currentFile++; 
		}
		else
		{
			cout << "Error: Could not open file: " << fileList[currentFile] << "\n";
            currentFile++;
		}		
	}

	return (openFile ? 0 : 1);	
}

void Parser::Start()
{
    scanner.switch_streams(&fin);

    

    if (scanner.yywrap() == 1) {
        cout << "No input files provided or all files failed to open.\n";
        return;
    }

    ss << "Token\t\tLinha\tColuna\n";
    ss << "-------------------------------\n";

    // enquanto não atingir o fim da entrada
    while ((lookahead = scanner.yylex()) != 0)
    {
        int tokenStartColumn = columnNumber - scanner.YYLeng();

        // trata o token recebido do analisador léxico
        switch(lookahead)
        {
            case GENSET: ss << "<GENSET>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case DISJOINT: ss << "<DISJOINT>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case COMPLETE: ss << "<COMPLETE>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case GENERAL: ss << "<GENERAL>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case SPECIFICS: ss << "<SPECIFICS>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case WHERE: ss << "<WHERE>\t\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case PACKAGE: ss << "<PACKAGE>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case IMPORT: ss << "<IMPORT>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            case FUNCTIONAL_COMPLEXES: ss << "<FUNCTIONAL_COMPLEXES>\t" << scanner.lineno() << "\t" << tokenStartColumn << "\n"; break;
            
            case TYPE: ss << "token type " << scanner.YYText() << '\n'; break;
            case NEW_TYPE: ss << "token new_type " << scanner.YYText() << '\n'; break;
            case INSTANCE_NAME: ss << "token name_instance " << scanner.YYText() << '\n'; break;
            case CLASS_NAME: ss << "token name_class " << scanner.YYText() << '\n'; break;
            case RELATION_NAME: ss << "token name_relation " << scanner.YYText() << '\n'; break;
            default: ss << "token " << scanner.YYText() << '\n'; break;
        }

    }
    

    ss << "-------------------------------\n";
    ss << "Total de palavras reservadas: " << keyWordsCount << "\n";

    std::ofstream file("../output.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file.\n";
    }

    file << ss.str();

    file.close();

    std::cout << "Data written to output.txt\n";
}
