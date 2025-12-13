#ifndef STATS_H
#define STATS_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "ast.h"

struct GensetNode {
    std::string name;
    std::string parent;
    std::vector<std::string> children;
    bool isDisjoint = false;
    bool isComplete = false;
};

struct RelationNode {
    std::string type;
    std::string stereotype;
    std::string name;
    std::string source;
    std::string target;
};

struct ClassNode {
    std::string name;
    std::string stereotype;
    std::vector<std::string> parents;
};

struct SyntaxStats {
    std::vector<std::string> packageNames;
    std::vector<ClassNode> classes;
    std::vector<std::string> dataTypeNames;
    std::vector<std::string> enumNames;
    std::vector<GensetNode> gensets;
    std::vector<RelationNode> relations;

    void printReport() {
        std::stringstream ss;
        ss << "\n--- Parsing Report ---\n";
        
        ss << "Packages (" << packageNames.size() << "):\n";
        for (const auto& name : packageNames) ss << "  - " << name << "\n";
        
        ss << "Classes (" << classes.size() << "):\n";
        for (const auto& cls : classes) {
            ss << "  - Nome: " << cls.name << "\n";
            ss << "    Esteriótipo: " << cls.stereotype << "\n";
            ss << "    Superclasses: ";
            if (cls.parents.empty()) {
                ss << "null";
            } else {
                for (size_t i = 0; i < cls.parents.size(); ++i) {
                    ss << cls.parents[i];
                    if (i < cls.parents.size() - 1) ss << ", ";
                }
            }
            ss << "\n";
        }
        
        ss << "New Data Types (" << dataTypeNames.size() << "):\n";
        for (const auto& name : dataTypeNames) ss << "  - " << name << "\n";
        
        ss << "Enums (" << enumNames.size() << "):\n";
        for (const auto& name : enumNames) ss << "  - " << name << "\n";

        ss << "Generalization Sets (" << gensets.size() << "):\n";
        for (const auto& gs : gensets) {
            ss << "  - Set Name: " << gs.name << "\n";
            ss << "    Parent: " << gs.parent << "\n";
            ss << "    Children: ";
            for(const auto& child : gs.children) ss << child << " ";
            ss << "\n";
        }

        ss << "Relations (" << relations.size() << "):\n";
        for (const auto& r : relations) {
            ss << "  - [" << r.type << "]";
            ss << " | " << r.source << " (<<" << r.stereotype << ">>";
            if (!r.name.empty()) ss << " " << r.name;
            ss << ") -> " << r.target << "\n";
        }

        std::ofstream outFile("output.txt");
        if (outFile.is_open()) {
            outFile << ss.str();
            outFile.close();
            std::cout << '\n' << BOLD_PURPLE << "Report written to output.txt" << COLOR_RESET << '\n';
        } else {
            std::cerr << '\n' << BOLD_RED << "Error: Could not open output.txt for writing." << COLOR_RESET << '\n';
        }
    }

    void clear() {
        packageNames.clear();
        classes.clear();
    }
};

extern SyntaxStats syntaxStats;
#endif
