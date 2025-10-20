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
int numbersCount = 0;
Counting tokenCounts;

char **fileList = nullptr;
unsigned nFiles = 0;
unsigned currentFile = 0;
bool openFile = false;
string fileName = "";
ifstream fin;
stringstream ss;

Parser::Parser(char** list, unsigned count) : lookahead(0) 
{
    ::fileList = list;
    ::nFiles = count;
}

int yyFlexLexer::yywrap()
{
	if (openFile) {
		openFile = false;
		fin.close();

        ss << "\nQuantidade de cada token identificados:\n";
        ss << "Classes: " << classCount << " | Relações: " << relationsCount << " | Palavras Reservadas: " << keyWordsCount;
        ss << " | Instâncias: " << instanceCount << " | Esteriótipos de Classes: " << classStereotypesCount;
        ss << " | Esteriótipos de Relações: " << relationsStereotypesCount << " | Meta atributos: " << metaAttributesCount;
        ss << " | Tipos: " << typesCount << " | Novos tipos: " << newTypesCount << " | Simbolos especiais: ";
        ss << specialSymbolsCount << " | Números: " << numbersCount << "\n\n";

        tokenCounts.Add(classCount, relationsCount, keyWordsCount, instanceCount, classStereotypesCount, relationsStereotypesCount, metaAttributesCount, typesCount, newTypesCount, specialSymbolsCount, numbersCount);
        
        columnNumber = 1;
        keyWordsCount = 0;
        classCount = 0;
        relationsCount = 0;
        instanceCount = 0;
        classStereotypesCount = 0;
        relationsStereotypesCount = 0;
        metaAttributesCount = 0;
        typesCount = 0;
        newTypesCount = 0;
        specialSymbolsCount = 0;
        numbersCount = 0;
	}

	while (!openFile && (currentFile < nFiles)) {
        fin.open(fileList[currentFile]); 

		if (fin.is_open()) {
			openFile = true;
			fileName = string(fileList[currentFile]);
            ss << "Arquivo " << fileName << ":\n";
            currentFile++;
		}
		else {
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

    // enquanto não atingir o fim da entrada
    while ((lookahead = scanner.yylex()) != 0) {
        int tokenStartColumn = columnNumber - scanner.YYLeng();

        // trata o token recebido do analisador léxico
        switch (lookahead) {
            case GENSET: ss << "<GENSET> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case DISJOINT: ss << "<DISJOINT> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case COMPLETE: ss << "<COMPLETE> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case GENERAL: ss << "<GENERAL> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case SPECIFICS: ss << "<SPECIFICS> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case WHERE: ss << "<WHERE> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case PACKAGE: ss << "<PACKAGE> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case IMPORT: ss << "<IMPORT> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case FUNCTIONAL_COMPLEXES: ss << "<FUNCTIONAL_COMPLEXES> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;

            case CLASS_STEREOTYPE: ss << "<CLASS_STEREOTYPE, \"" << scanner.YYText() << "\"> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case RELATIONS_STEREOTYPE: ss << "<RELATIONS_STEREOTYPE, \"" << scanner.YYText() << "\"> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case META: ss << "<META, \"" << scanner.YYText() << "\"> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case TYPE: ss << "<TYPE, \"" << scanner.YYText() << "\"> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case NEW_TYPE: ss << "<NEW_TYPE, \"" << scanner.YYText() << "\"> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case INSTANCE_NAME: ss << "<INSTANCE_NAME, \"" << scanner.YYText() << "\"> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case CLASS_NAME: ss << "<CLASS_NAME, \"" << scanner.YYText() << "\"> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case RELATION_NAME: ss << "<RELATION_NAME, \"" << scanner.YYText() << "\"> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;

            case LBRACE: ss << "<LBRACE> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case RBRACE: ss << "<RBRACE> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case LBRACKET: ss << "<LBRACKET> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case RBRACKET: ss << "<RBRACKET> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case LP: ss << "<LP> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case RP: ss << "<RP> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case COLON: ss << "<COLON> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case DOTDOT: ss << "<DOTDOT> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case LRELATION: ss << "<LRELATION> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case RRELATION: ss << "<RRELATION> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case ASTHERISTICS: ss << "<ASTHERISTICS> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case AT: ss << "<AT> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            case NUMBER: ss << "<NUMBER, \"" << scanner.YYText() << "\"> | Linha " << scanner.lineno() << " | Coluna " << tokenStartColumn << "\n"; break;
            
            default: cout << "ERRO: " << scanner.YYText() << " não é um identificador válido (Linha " << scanner.lineno() << ", Coluna " << tokenStartColumn << ", Arquivo " << fileName << ")\n"; break;
        }
    }

    ss << "-------------------------------\n";
    ss << "Total de cada token identificado:\n";
    ss << "Classes: " << tokenCounts.classCount << " | Relações: " << tokenCounts.relationsCount << " | Palavras Reservadas: ";
    ss << tokenCounts.keyWordsCount << " | Instâncias: " << tokenCounts.instanceCount << " | Esteriótipos de Classes: ";
    ss << tokenCounts.classStereotypesCount << " | Esteriótipos de Relações: " << tokenCounts.relationsStereotypesCount;
    ss << " | Meta atributos: " << tokenCounts.metaAttributesCount << " | Tipos: " << tokenCounts.typesCount;
    ss << " | Novos tipos: " << tokenCounts.newTypesCount << " | Simbolos especiais: " << tokenCounts.specialSymbolsCount;
    ss << " | Números: " << tokenCounts.numbersCount << "\n";

    std::ofstream file("../output.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file.\n";
    }

    file << ss.str();

    file.close();

    std::cout << "Data written to output.txt\n";
}
