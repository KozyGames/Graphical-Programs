#include "Input_Manager.h"

#include <string>
#include <iostream>

using std::cin;using std::endl;using std::cout;
using std::string;
using std::exception;

int main() {

	string input;
	Input_Manager manager;
	

	while (!manager.wantsToExit() && getline(cin, input)) {
		try {
			manager[input];
		}
		catch (exception& e) {
			cout << e.what() << endl;
		}
	}


	cout << "\nProgram Finished!" << endl;

	if (manager.getCurrentMode() == Input_Manager::mode::EXIT_NO_ERROR)
		return 0;
	else
		return static_cast<int>(manager.getCurrentMode());
}