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

        if (parseResult == 0 && !errorOccurred && !checkSubkindPattern() && !checkGensetPattern()) {
            std::cout << BOLD_GREEN << "Parse successful!" << COLOR_RESET << "\n";
        } else {
            std::cout << BOLD_RED << "Parse failed!" << COLOR_RESET << "\n";
        }

        errorOccurred = false;
        fileInput.close();
    }

    syntaxStats.printReport();
}

bool Ast::checkSubkindPattern() {
    bool semanticError = false;

    for (const auto& cls : syntaxStats.classes) {
        if (cls.stereotype == "subkind") {
            if (cls.parents.empty()) {
                std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'subkind'" << COLOR_RESET << " must have at least one superclass. Found 0 superclasses.\n";
                semanticError = true;
                continue;
            }

            bool inheritsFromUltimateSortal = false;
            std::string ultimateSortalParentName = "";
            std::string commonAncestor = "";

            for (const auto& parentName : cls.parents) {
                std::string parentStereotype = getStereotype(parentName);

                if (ultimateSortals.count(parentStereotype)) {
                    if (inheritsFromUltimateSortal) {
                        std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'subkind'" << COLOR_RESET << " cannot inherit from multiple ultimate sortals " << BOLD_YELLOW << "('" << ultimateSortalParentName << "' and '" << parentName << "')" << COLOR_RESET << ".\n";
                        
                        semanticError = true;
                    }

                    inheritsFromUltimateSortal = true;
                    ultimateSortalParentName = parentName;
                }

                else if (parentStereotype == "subkind") {
                    std::string ancestor = findUltimateSortalAncestor(parentName);
                    
                    if (commonAncestor.empty()) {
                        commonAncestor = ancestor;
                    } else if (!ancestor.empty() && ancestor != commonAncestor) {
                        std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'subkind'" << COLOR_RESET << " has conflicting identities from its superclasses " << BOLD_YELLOW << "('" << commonAncestor << "' and '" << ancestor << "')" << COLOR_RESET << ".\n";

                        semanticError = true;
                    }
                }
            }

            if (inheritsFromUltimateSortal) {
                if (cls.parents.size() > 1) {
                    std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'subkind'" << COLOR_RESET << " inherits from ultimate sortal " << BOLD_YELLOW << "('" << ultimateSortalParentName << "')" << COLOR_RESET << " and therefore cannot have multiple inheritance.\n";

                    semanticError = true;
                }
            }
        }
    }

    return semanticError;
}

bool Ast::checkGensetPattern() {
    bool semanticError = false;
    
    for (const auto& genset : syntaxStats.gensets) {
        std::string parentStereotype = getStereotype(genset.parent);

        if (ultimateSortals.count(parentStereotype)) {
            bool childrenAreSubkinds = true;

            if (!genset.children.empty()) {
                for (const auto& child : genset.children) {
                    if (getStereotype(child) != "subkind") {
                        childrenAreSubkinds = false;
                        break;
                    }
                }
            }

            if (childrenAreSubkinds) {
                if (!genset.isDisjoint) {
                    std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Genset " << BOLD_RED << "'" << genset.name << "'" << COLOR_RESET << " for the " << BOLD_YELLOW << "'Subkind Pattern'" << COLOR_RESET << " must be " << BOLD_YELLOW << "'disjoint'" << COLOR_RESET << ".\n";
                    semanticError = true;
                }
            }
        }
    }

    return semanticError;
}

std::string Ast::getStereotype(const std::string& className) {
    for (const auto& cls : syntaxStats.classes) {
        if (cls.name == className) {
            return cls.stereotype;
        }
    }
    return "unkown";
}

std::string Ast::findUltimateSortalAncestor(const std::string& className) {
    const ClassNode* node = nullptr;
    for (const auto& cls : syntaxStats.classes) {
        if (cls.name == className) {
            node = &cls;
            break;
        }
    }

    if (!node) return "";

    if (ultimateSortals.count(node->stereotype)) {
        return node->name;
    }

    if (node->stereotype == "subkind" || node->stereotype == "phase" || node->stereotype == "role") {
        for (const auto& parentName : node->parents) {
            std::string root = findUltimateSortalAncestor(parentName);
            if (!root.empty()) {
                return root;
            }
        }
    }

    return "";
}
