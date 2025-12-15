#ifndef AST_H
#define AST_H

#include <FlexLexer.h>
#include <fstream>
#include <unordered_set>
#include "stats.h"

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
	bool checkStandalonePatterns();
	bool checkSubkindPattern(const ClassNode&);
	bool checkRolePattern(const ClassNode&);
	bool checkPhasePattern(const ClassNode&);
	bool checkGensetPattern();
	bool checkModePattern(const ClassNode&);
	bool checkRelatorPattern(const ClassNode&);
	bool checkMaterialRelations();
	
	std::string getStereotype(const std::string&);
	ClassNode* getClassNode(const std::string&, const std::string&);
	std::string findUltimateSortalAncestor(const std::string&);
 	std::string findInherenceAncestor(const std::string&);

	int countPhaseChildren(const std::string&);
public:
	Ast(char** fileList, unsigned nFiles);
	void Start();
};

#endif
