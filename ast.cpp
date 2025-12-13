#include <iostream>
#include <fstream>
#include <sstream>
#include "ast.h"
#include "stats.h"

using namespace std;

extern void setScanner(yyFlexLexer* scanner); 
extern int yyparse();

extern void ResetCounters();

extern bool errorOccurred;
std::string currentFileName = "";

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
    bool overallParseSuccess = true;

    for (unsigned i = 0; i < ::nFiles; i++) {
        currentFileName = std::string(::fileList[i]);
        std::ifstream fileInput(::fileList[i]);
        
        if (!fileInput.is_open()) {
            std::cerr << BOLD_RED << "Could not open file: " << ::fileList[i] << COLOR_RESET << '\n';
            continue;
        }

        std::cout << "\n\033[32m[TontoCompiler] Building file: " << ::fileList[i] << "\033[m";

        yyFlexLexer localScanner;
        ResetCounters();

        localScanner.switch_streams(&fileInput, nullptr);
        setScanner(&localScanner);
        
        int parseResult = yyparse();

        if (parseResult != 0 || errorOccurred) {
            overallParseSuccess = false;
        }

        errorOccurred = false;
        fileInput.close();
    }

    if (overallParseSuccess) {
        std::cout << "\n" << BOLD_BLUE << "[TontoCompiler] Building Project" << COLOR_RESET << "\n";

        bool semanticError = false;
        if (checkSubkindPattern()) semanticError = true;
        if (checkGensetPattern()) semanticError = true;

        if (!semanticError) {
            std::cout << BOLD_GREEN << "Build successful!" << COLOR_RESET << "\n";
        } else {
            std::cout << BOLD_RED << "Build failed!" << COLOR_RESET << "\n";
        }
    } else {
        std::cout << BOLD_RED << "Build failed!" << COLOR_RESET << "\n";
    }

    syntaxStats.printReport();
}

bool Ast::checkSubkindPattern() {
    bool semanticError = false;

    for (const auto& cls : syntaxStats.classes) {
        if (cls.stereotype == "subkind") {
            bool currentClassError = false;
            bool wasCoerced = false;

            std::string location = "[" + cls.fileName + "(" + std::to_string(cls.line) + "," + std::to_string(cls.column) +")]";

            if (cls.parents.empty()) {
                std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'subkind'" << COLOR_RESET << " must have at least one superclass. Found 0 superclasses. " << location << "\n";
                
                semanticError = true;
                continue;
            }

            bool inheritsFromUltimateSortal = false;
            std::string ultimateSortalParentName = "";
            std::string commonAncestor = "";

            for (const auto& parentName : cls.parents) {
                ClassNode* parentNode = getClassNode(parentName);
                
                if (parentNode == nullptr) {
                    std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Superclass " << BOLD_RED << "'" << parentName << "'" << COLOR_RESET << " was not declared. " << location << "\n";
                    
                    currentClassError = true;
                    semanticError = true;
                    continue;
                }

                std::string parentStereotype = parentNode->stereotype;

                // Aplicar coerção
                if (parentStereotype == "role" || parentStereotype == "phase" || parentStereotype == "roleMixin" || parentStereotype == "phaseMixin") {
                    std::cout << BOLD_YELLOW << "Semantic Warning: " << COLOR_RESET << "Conversion from " << BOLD_YELLOW << "'" << parentStereotype << "'" << COLOR_RESET << " to " << BOLD_YELLOW << "'kind'" << COLOR_RESET << ". " << location << "\n";

                    parentNode->stereotype = "kind";
                    parentStereotype = "kind";
                    wasCoerced = true;
                }

                if (ultimateSortals.count(parentStereotype)) {
                    if (inheritsFromUltimateSortal) {
                        std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'subkind'" << COLOR_RESET << " cannot inherit from multiple ultimate sortals " << BOLD_YELLOW << "('" << ultimateSortalParentName << "' and '" << parentName << "')" << COLOR_RESET << ". " << location << "\n";
                        
                        semanticError = true;
                        currentClassError = true;
                    }

                    inheritsFromUltimateSortal = true;
                    ultimateSortalParentName = parentName;
                }

                else if (parentStereotype == "subkind") {
                    std::string ancestor = findUltimateSortalAncestor(parentName);
                    
                    if (commonAncestor.empty()) {
                        commonAncestor = ancestor;
                    } else if (!ancestor.empty() && ancestor != commonAncestor) {
                        std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'subkind'" << COLOR_RESET << " has conflicting identities from its superclasses " << BOLD_YELLOW << "('" << commonAncestor << "' and '" << ancestor << "')" << COLOR_RESET << ". " << location << "\n";

                        semanticError = true;
                        currentClassError = true;
                    }
                }
            }

            if (inheritsFromUltimateSortal) {
                if (cls.parents.size() > 1) {
                    std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'subkind'" << COLOR_RESET << " inherits from ultimate sortal " << BOLD_YELLOW << "('" << ultimateSortalParentName << "')" << COLOR_RESET << " and therefore cannot have multiple inheritance. " << location << "\n";

                    semanticError = true;
                    currentClassError = true;
                }
            }

            if (!currentClassError) {
                Output output;
                output.patternName = "Subkind Pattern";
                output.element = "Class: " + cls.name;
                
                if (wasCoerced) {
                    output.status = "Incompleto";
                } else {
                    output.status = "Completo";
                }
                syntaxStats.identifiedPatterns.push_back(output);
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
                std::string location = "[" + genset.fileName + "(" + std::to_string(genset.line) + "," + std::to_string(genset.column) +")]";

                if (!genset.isDisjoint) {
                    std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Genset " << BOLD_RED << "'" << genset.name << "'" << COLOR_RESET << " for the " << BOLD_YELLOW << "'Subkind Pattern'" << COLOR_RESET << " must be " << BOLD_YELLOW << "'disjoint'" << COLOR_RESET << ". " << location << "\n";
                    semanticError = true;
                } else {
                    Output output;
                    output.patternName = "Subkind Pattern (Genset)";
                    output.element = "Genset: " + genset.name;
                    output.status = "Completo";
                    syntaxStats.identifiedPatterns.push_back(output);
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

ClassNode* Ast::getClassNode(const std::string& className) {
    for (auto& cls : syntaxStats.classes) {
        if (cls.name == className) {
            return &cls;
        }
    }

    return nullptr;
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
