#include "ast.h"

int main(int argc, char ** argv)
{
	char** fileList = argv + 1;
	unsigned nFiles = argc - 1;

	Ast ast = Ast(fileList, nFiles);
	ast.Start();
}
