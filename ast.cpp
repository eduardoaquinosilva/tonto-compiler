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
        if (checkStandalonePatterns()) semanticError = true;
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

bool Ast::checkStandalonePatterns() {
    bool semanticError = false;

    for (const auto& cls : syntaxStats.classes) {
        if (cls.stereotype == "subkind") {
            semanticError = checkSubkindPattern(cls) || semanticError;
        }
        else if (cls.stereotype == "role") {
            semanticError = checkRolePattern(cls) || semanticError;
        }
        else if (cls.stereotype == "phase") {
            semanticError = checkPhasePattern(cls) || semanticError;
        }
    }

    return semanticError;
}

bool Ast::checkSubkindPattern(const ClassNode& cls) {
    bool semanticError = false;
    bool currentClassError = false;
    bool wasCoerced = false;

    std::string location = "[" + cls.fileName + "(" + std::to_string(cls.line) + "," + std::to_string(cls.column) +")]";

    if (cls.parents.empty()) {
        std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'subkind'" << COLOR_RESET << " must have at least one superclass. Found 0 superclasses. " << location << "\n";
        
        return true;
    }

    bool inheritsFromUltimateSortal = false;
    std::string ultimateSortalParentName = "";
    std::string commonAncestor = "";

    for (const auto& parentName : cls.parents) {
        ClassNode* parentNode = getClassNode(parentName, cls.fileName);
        
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

    return semanticError;
}

bool Ast::checkRolePattern(const ClassNode& cls) {
    bool semanticError = false;
    bool currentClassError = false;
    bool wasCoerced = false;

    std::string location = "[" + cls.fileName + "(" + std::to_string(cls.line) + "," + std::to_string(cls.column) +")]";

    if (cls.parents.empty()) {
        std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'role'" << COLOR_RESET << " must have at least one superclass. Found 0 superclasses. " << location << "\n";
        
        return true;
    }

    bool inheritsFromUltimateSortal = false;
    std::string ultimateSortalParentName = "";
    std::string commonAncestor = "";

    for (const auto& parentName : cls.parents) {
        ClassNode* parentNode = getClassNode(parentName, cls.fileName);

        if (parentNode == nullptr) {
            std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Superclass " << BOLD_RED << "'" << parentName << "'" << COLOR_RESET << " was not declared. " << location << "\n";
            
            currentClassError = true;
            semanticError = true;
            continue;
        }

        std::string parentStereotype = parentNode->stereotype;

        // Aplicar coerção
        if (parentStereotype == "event" || parentStereotype == "situation" || parentStereotype == "type") {
            std::cout << BOLD_YELLOW << "Semantic Warning: " << COLOR_RESET << "Conversion from " << BOLD_YELLOW << "'" << parentStereotype << "'" << COLOR_RESET << " to " << BOLD_YELLOW << "'kind'" << COLOR_RESET << ". " << location << "\n";

            parentNode->stereotype = "kind";
            parentStereotype = "kind";
            wasCoerced = true;
        }

        if (ultimateSortals.count(parentStereotype)) {
            if (inheritsFromUltimateSortal) {
                std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'role'" << COLOR_RESET << " cannot inherit from multiple ultimate sortals " << BOLD_YELLOW << "('" << ultimateSortalParentName << "' and '" << parentName << "')" << COLOR_RESET << ". " << location << "\n";
                
                semanticError = true;
                currentClassError = true;
            }

            inheritsFromUltimateSortal = true;
            ultimateSortalParentName = parentName;
        }
        else if (parentStereotype == "role" || parentStereotype == "subkind") {
            std::string ancestor = findUltimateSortalAncestor(parentName);
            
            if (commonAncestor.empty()) {
                commonAncestor = ancestor;
            } else if (!ancestor.empty() && ancestor != commonAncestor) {
                std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'role'" << COLOR_RESET << " has conflicting identities from its superclasses " << BOLD_YELLOW << "('" << commonAncestor << "' and '" << ancestor << "')" << COLOR_RESET << ". " << location << "\n";
                
                semanticError = true;
                currentClassError = true;
            }
        }
    }

    if (!inheritsFromUltimateSortal && commonAncestor.empty()) {
        bool allMixins = true;
        for(const auto& p : cls.parents) {
            std::string s = getStereotype(p);
            if(nonSortals.find(s) == nonSortals.end()) allMixins = false;
        }

        if (!allMixins) {
            std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " os stereotype " << BOLD_YELLOW << "'role'" << COLOR_RESET << " does not inherit identity from any Ultimate Sortal (Kind, Collective, etc). " << location << "\n";
            
            semanticError = true;
            currentClassError = true;
        }
    }

    if (!currentClassError) {
        Output output;
        output.patternName = "Role Pattern";
        output.element = "Class: " + cls.name;
        output.status = wasCoerced ? "Incompleto" : "Completo";
        syntaxStats.identifiedPatterns.push_back(output);
    }

    return semanticError;
}

bool Ast::checkPhasePattern(const ClassNode& cls) {
    bool semanticError = false;
    bool currentClassError = false;
    bool wasCoerced = false;

    std::string location = "[" + cls.fileName + "(" + std::to_string(cls.line) + "," + std::to_string(cls.column) +")]";

    if (cls.parents.empty()) {
        std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'phase'" << COLOR_RESET << " must have at least one superclass. Found 0 superclasses. " << location << "\n";

        return true;
    }

    bool inheritsFromUltimateSortal = false;
    std::string ultimateSortalParentName = "";
    std::string commonAncestor = "";

    for (const auto& parentName : cls.parents) {
        ClassNode* parentNode = getClassNode(parentName, cls.fileName);

        if (parentNode == nullptr) {
            std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Superclass " << BOLD_RED << "'" << parentName << "'" << COLOR_RESET << " was not declared. " << location << "\n";
            currentClassError = true;
            semanticError = true;
            continue;
        }

        std::string parentStereotype = parentNode->stereotype;

        // Aplicar coerção
        if (parentStereotype == "event" || parentStereotype == "situation" || parentStereotype == "type") {
            std::cout << BOLD_YELLOW << "Semantic Warning: " << COLOR_RESET << "Conversion from " << BOLD_YELLOW << "'" << parentStereotype << "'" << COLOR_RESET << " to " << BOLD_YELLOW << "'kind'" << COLOR_RESET << ". " << location << "\n";

            parentNode->stereotype = "kind";
            parentStereotype = "kind";
            wasCoerced = true;
        }

        int siblings = countPhaseChildren(parentName);
        
        if (siblings < 2) {
            std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << parentName << "'" << COLOR_RESET << " must have at least 2 phases. Found " << siblings << " phase(s). " << location << "\n";
            semanticError = true;
            currentClassError = true;
        }

        if (ultimateSortals.count(parentStereotype)) {
            if (inheritsFromUltimateSortal) {
                std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'role'" << COLOR_RESET << " cannot inherit from multiple ultimate sortals " << BOLD_YELLOW << "('" << ultimateSortalParentName << "' and '" << parentName << "')" << COLOR_RESET << ". " << location << "\n";
                
                semanticError = true;
                currentClassError = true;
            }

            inheritsFromUltimateSortal = true;
            ultimateSortalParentName = parentName;
        }
        else if (parentStereotype == "role" || parentStereotype == "subkind") {
            std::string ancestor = findUltimateSortalAncestor(parentName);
            
            if (commonAncestor.empty()) {
                commonAncestor = ancestor;
            } else if (!ancestor.empty() && ancestor != commonAncestor) {
                std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'role'" << COLOR_RESET << " has conflicting identities from its superclasses " << BOLD_YELLOW << "('" << commonAncestor << "' and '" << ancestor << "')" << COLOR_RESET << ". " << location << "\n";
                
                semanticError = true;
                currentClassError = true;
            }
        }
    }

    if (!inheritsFromUltimateSortal && commonAncestor.empty()) {
        std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Class " << BOLD_RED << "'" << cls.name << "'" << COLOR_RESET << " of stereotype " << BOLD_YELLOW << "'phase'" << COLOR_RESET << " does not inherit identity from any Ultimate Sortal (Kind, Collective, etc). " << location << "\n";
        
        semanticError = true;
        currentClassError = true;
    }

    if (!currentClassError) {
        Output output;
        output.patternName = "Phase Pattern";
        output.element = "Class: " + cls.name;
        output.status = wasCoerced ? "Incompleto" : "Completo";
        syntaxStats.identifiedPatterns.push_back(output);
    }

    return semanticError;
}

bool Ast::checkGensetPattern() {
    bool semanticError = false;
    bool wasCoerced = false;
    
    for (auto& genset : syntaxStats.gensets) {
        std::string parentStereotype = getStereotype(genset.parent);

        if (ultimateSortals.count(parentStereotype)) {
            bool childrenAreSubkinds = true;
            bool childrenAreRoles = true;
            bool childrenArePhases = true;

            if (!genset.children.empty()) {
                for (const auto& child : genset.children) {
                    std::string stereotype = getStereotype(child);
                    if (stereotype != "subkind") childrenAreSubkinds = false;
                    if (stereotype != "role") childrenAreRoles = false;
                    if (stereotype != "phase") childrenArePhases = false;
                }
            } else {
                childrenAreSubkinds = false;
                childrenAreRoles = false;
            }

            std::string location = "[" + genset.fileName + "(" + std::to_string(genset.line) + "," + std::to_string(genset.column) +")]";

            if (childrenAreSubkinds) {
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
            else if (childrenAreRoles) {
                Output output;
                output.patternName = "Role Pattern (Genset)";
                output.element = "Genset: " + genset.name;
                output.status = "Completo";
                syntaxStats.identifiedPatterns.push_back(output);
            }
            else if (childrenArePhases) {
                if (!genset.isDisjoint) {
                    std::cout << BOLD_YELLOW << "Semantic Warning: " << COLOR_RESET << "Conversion from " << BOLD_YELLOW << "'overlapping'" << COLOR_RESET << " to " << BOLD_YELLOW << "'disjoint'" << COLOR_RESET << " in genset " << BOLD_RED << "'" << genset.name << "'" << COLOR_RESET << ". " << location << "\n";

                    genset.isDisjoint = true;
                    wasCoerced = true;
                }
                else if (genset.children.size() < 2) {
                    std::cout << BOLD_RED << "Semantic Error: " << COLOR_RESET << "Genset " << BOLD_RED << "'" << genset.name << "'" << COLOR_RESET << " for the " << BOLD_YELLOW << "'Phase Pattern'" << COLOR_RESET << " must have at least 2 phases. Found " << genset.children.size() << " phase(s). " << location << "\n";
                    semanticError = true;
                }
                else {
                    Output output;
                    output.patternName = "Phase Pattern (Genset)";
                    output.element = "Genset: " + genset.name;
                    output.status = wasCoerced ? "Incompleto" : "Completo";
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

ClassNode* Ast::getClassNode(const std::string& className, const std::string& fileName) {
    ClassNode* classNode = nullptr;

    for (auto& cls : syntaxStats.classes) {
        if (cls.name == className) {
            classNode = &cls;
            break;
        }
    }

    if (!classNode) return nullptr;

    if (classNode->fileName == fileName) return classNode;

    const std::vector<std::string>& imports = syntaxStats.fileImports[fileName];
    bool isImported = false;
    for (const auto& pakage : imports) {
        if (pakage == classNode->packageName) {
            isImported = true;
            break;
        }
    }

    if (isImported) return classNode;

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

int Ast::countPhaseChildren(const std::string& parentName) {
    int count = 0;
    for (const auto& cls : syntaxStats.classes) {
        if (cls.stereotype == "phase") {
            for (const auto& p : cls.parents) {
                if (p == parentName) {
                    count++;
                    break;
                }
            }
        }
    }

    return count;
}
