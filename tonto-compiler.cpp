#include "parser.h"

int main(int argc, char ** argv)
{
	char** fileList = argv + 1;
	unsigned nFiles = argc - 1;

	Parser parser = Parser(fileList, nFiles);
	parser.Start();
}
