#include <FlexLexer.h>
#include <fstream>

extern int columnNumber;
extern int keyWordsCount;

extern char **fileList;
extern unsigned nFiles;
extern unsigned currentFile;
extern bool openFile;
extern std::ifstream fin;

class Parser
{
private:
	yyFlexLexer scanner;
	int lookahead;
	
public:
	Parser(char** fileList, unsigned nFiles);
	void Start();
};
