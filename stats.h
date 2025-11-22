#ifndef STATS_H
#define STATS_H

#include <string>
#include <vector>
#include <iostream>

struct GensetInfo {
    std::string name;
    std::string parent;
    std::vector<std::string> children;
};

struct RelationInfo {
    std::string type;
    std::string stereotype;
    std::string name;
    std::string source;
    std::string target;
};

struct SyntaxStats {
    std::vector<std::string> packageNames;
    std::vector<std::string> classNames;
    std::vector<std::string> dataTypeNames;
    std::vector<std::string> enumNames;
    std::vector<GensetInfo> gensets;
    std::vector<RelationInfo> relations;

    void printReport() {
        std::cout << "\n--- Parsing Report ---\n";
        
        std::cout << "Packages (" << packageNames.size() << "):\n";
        for (const auto& name : packageNames) std::cout << "  - " << name << "\n";
        
        std::cout << "Classes (" << classNames.size() << "):\n";
        for (const auto& name : classNames) std::cout << "  - " << name << "\n";
        
        std::cout << "New Data Types (" << dataTypeNames.size() << "):\n";
        for (const auto& name : dataTypeNames) std::cout << "  - " << name << "\n";
        
        std::cout << "Enums (" << enumNames.size() << "):\n";
        for (const auto& name : enumNames) std::cout << "  - " << name << "\n";

        std::cout << "Generalization Sets (" << gensets.size() << "):\n";
        for (const auto& gs : gensets) {
            std::cout << "  - Set Name: " << gs.name << "\n";
            std::cout << "    Parent: " << gs.parent << "\n";
            std::cout << "    Children: ";
            for(const auto& child : gs.children) std::cout << child << " ";
            std::cout << "\n";
        }

        std::cout << "Relations (" << relations.size() << "):\n";
        for (const auto& r : relations) {
            std::cout << "  - [" << r.type << "]";
            std::cout << " | " << r.source << " (<<" << r.stereotype << ">>";
            if (!r.name.empty()) std::cout << " " << r.name;
            std::cout << ") -> " << r.target << "\n";
        }
    }

    void clear() {
        packageNames.clear();
        classNames.clear();
    }
};

extern SyntaxStats syntaxStats;
#endif
