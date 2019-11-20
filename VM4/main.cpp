#include <iostream>
#include "Loader.h"
using namespace std;

int main (int argc, char *argv[])
{
	Processor p;
	  
	for (size_t i = 1; i < argc; i++)
	{
		cout << "Program " << argv[i] << " to execute\n";

		fstream program(argv[i]);
		if (program) {
			if (parse(program,p)) {
				cout << "Program has been loaded succesfully\n";
			}
			else {
				cout << "Load failure\n";
			}
			if (!p.run())		cout << "Error in command, Processor shot down.";
		}
		else {
			cout << "Program file can't be open.\n VM ends its work.";
		}
	}

	system("pause");
	return 0;
}