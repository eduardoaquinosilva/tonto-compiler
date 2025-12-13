#ifndef AST_H
#define AST_H

#define COLOR_RESET  "\033[0m"
#define BOLD_GREEN   "\033[1;32m"
#define NORMAL_RED   "\033[31m"
#define BOLD_RED     "\033[1;31m"
#define BOLD_YELLOW  "\033[1;33m"
#define BOLD_PURPLE  "\033[1;35m"

#include <FlexLexer.h>
#include <fstream>
#include <unordered_set>
#include "stats.h"

extern int columnNumber;
extern int keyWordsCount;

extern char **fileList;
extern unsigned nFiles;
extern unsigned currentFile;
extern bool openFile;
extern std::ifstream fin;

class Ast
{
private:
	yyFlexLexer scanner;
	int lookahead;

	const std::unordered_set<std::string> ultimateSortals = {
        "kind", "collective", "quantity", "relator", "quality", "mode"
    };

    const std::unordered_set<std::string> nonSortals = {
        "category", "mixin", "roleMixin", "phaseMixin"
    };
	
	bool checkSubkindPattern();
	bool checkGensetPattern();
	
	std::string getStereotype(const std::string& className);
	ClassNode* getClassNode(const std::string& className);
	std::string findUltimateSortalAncestor(const std::string& className);
public:
	Ast(char** fileList, unsigned nFiles);
	void Start();
};

#endif
