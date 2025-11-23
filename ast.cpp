#include <iostream>
#include <fstream>
#include <sstream>
#include "ast.h"
#include "stats.h"
// #include "tokens.h"

using namespace std;

extern void setScanner(yyFlexLexer* scanner); 
extern int yyparse();

extern void ResetCounters();

extern bool errorOccurred;

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

Ast::Ast(char** list, unsigned count) : lookahead(0) 
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

void Ast::Start()
{
    for (unsigned i = 0; i < ::nFiles; i++) {
        std::ifstream fileInput(::fileList[i]);
        
        if (!fileInput.is_open()) {
            std::cerr << BOLD_RED << "Could not open file: " << ::fileList[i] << COLOR_RESET << '\n';
            continue;
        }

        std::cout << "\n\033[32m[TontoCompiler] Building file: " << ::fileList[i] << "\033[m\n";

        yyFlexLexer localScanner;
        ResetCounters();
        localScanner.switch_streams(&fileInput, nullptr);
        setScanner(&localScanner);
        
        int parseResult = yyparse();

        if (parseResult == 0 && !errorOccurred) {
            std::cout << BOLD_GREEN << "Parse successful!" << COLOR_RESET << "\n";
        } else {
            std::cout << BOLD_RED << "Parse failed!" << COLOR_RESET << "\n";
        }

        errorOccurred = false;
        fileInput.close();
    }

    syntaxStats.printReport();
}
