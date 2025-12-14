#ifndef STATS_H
#define STATS_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>

#define COLOR_RESET  "\033[0m"
#define BOLD_GREEN   "\033[1;32m"
#define NORMAL_RED   "\033[31m"
#define BOLD_RED     "\033[1;31m"
#define BOLD_YELLOW  "\033[1;33m"
#define BOLD_PURPLE  "\033[1;35m"
#define BOLD_BLUE    "\033[1;34m"

struct Output {
    std::string patternName;
    std::string element;
    std::string status;
};

struct GensetNode {
    std::string name;
    std::string parent;
    std::vector<std::string> children;
    bool isDisjoint = false;
    bool isComplete = false;

    std::string fileName;
    int line = 0;
    int column = 0;
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
    std::string packageName;

    std::string fileName;
    int line = 0;
    int column = 0;
};

struct SyntaxStats {
    std::vector<std::string> packageNames;
    std::vector<ClassNode> classes;
    std::vector<std::string> dataTypeNames;
    std::vector<std::string> enumNames;
    std::vector<GensetNode> gensets;
    std::vector<RelationNode> relations;
    
    std::vector<Output> identifiedPatterns;
    
    std::map<std::string, std::vector<std::string>> fileImports;

    void printReport();
    void clear();
};

inline void SyntaxStats::clear() { packageNames.clear(); classes.clear(); };

extern SyntaxStats syntaxStats;
#endif
