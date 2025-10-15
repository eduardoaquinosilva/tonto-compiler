#include <FlexLexer.h>

extern int columnNumber;
extern int keyWordsCount;

class Parser
{
private:
	yyFlexLexer scanner;
	int lookahead;
	
public:
	void Start();
};
