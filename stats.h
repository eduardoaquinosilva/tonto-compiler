#ifndef STATS_H
#define STATS_H

#include <string>
#include <vector>
#include <iostream>

struct SyntaxStats {
    std::vector<std::string> packageNames;
    std::vector<std::string> classNames;

    void printReport() {
        std::cout << "--- Parsing Report ---\n";
        std::cout << "Packages (" << packageNames.size() << "):\n";
        for (const auto& name : packageNames) std::cout << "  - " << name << "\n";
        std::cout << "Classes (" << classNames.size() << "):\n";
        for (const auto& name : classNames) std::cout << "  - " << name << "\n";
    }

    void clear() {
        packageNames.clear();
        classNames.clear();
    }
};

extern SyntaxStats syntaxStats;
#endif
