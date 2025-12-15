#include "stats.h"

void SyntaxStats::printReport() {
    std::stringstream ss;
    
    ss << "\n--- Semantic Report ---\n";
    if (identifiedPatterns.empty()) {
        ss << "Nenhum padrao de projeto identificado.\n";
    } else {
        for (const auto& log : identifiedPatterns) {
            ss << "  - [" << log.patternName << "] em " << log.element << " (" << log.status << ")\n";
        }
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
