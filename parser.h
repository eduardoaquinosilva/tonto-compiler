#include <FlexLexer.h>
#include <fstream>

struct Counting {
	int classCount = 0;
	int relationsCount = 0;
	int keyWordsCount = 0;
	int instanceCount = 0;
	int classStereotypesCount = 0;
	int relationsStereotypesCount = 0;
	int metaAttributesCount = 0;
	int typesCount = 0;
	int newTypesCount = 0;
	int specialSymbolsCount = 0;

	void Add(int classCount, int relationsCount, int keywordsCount, int instanceCount, int classStereotypesCount, int relationsStereotypesCount, int metaAttributesCount, int typesCount, int newTypeCount, int specialSymbolsCount) {
		this->classCount += classCount;
		this->relationsCount += relationsCount;
		this->keyWordsCount += keywordsCount;
		this->instanceCount += instanceCount;
		this->classStereotypesCount += classStereotypesCount;
		this->relationsStereotypesCount += relationsStereotypesCount;
		this->metaAttributesCount += metaAttributesCount;
		this->typesCount += typesCount;
		this->newTypesCount += newTypeCount;
		this->specialSymbolsCount += specialSymbolsCount;
	}
};

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
