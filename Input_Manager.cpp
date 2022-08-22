#include "Input_Manager.h"

#include <sstream>
#include <iostream>

using std::string; using std::istringstream;
using std::cin;using std::cout;using std::endl;
using std::exception;


Input_Manager::Input_Manager() {
	commandMap={
		{"f", I_EXIT},{"F",I_EXIT },{"exit",I_EXIT },{"Exit",I_EXIT },{"EXIT",I_EXIT },
		{"help",I_HELP},{"Help",I_HELP},{"info",I_HELP},
		{"open",I_OPEN}, {"Open",I_OPEN},{"OPEN",I_OPEN},
		{"converter",I_CONVERTER},{"Converter",I_CONVERTER},{"CONVERTER",I_CONVERTER},
		{"back",I_BACK}, {"Back",I_BACK},{"BACK",I_BACK},
		{"info",I_INFO}, {"Info",I_INFO},{"INFO",I_INFO},
		{"shader",C_SHADER}, {"Shader",C_SHADER},{"SHADER",C_SHADER},
		{"convert",C_CONVERT}, {"Convert",C_CONVERT},{"CONVERT",C_CONVERT},{"con",C_CONVERT},
		{"convert_f",C_CONVERT_FOLDER}, {"Convert_F",C_CONVERT_FOLDER},{"CONVERT_F",C_CONVERT_FOLDER},{"conF",C_CONVERT_FOLDER},
		{"threads",C_CHANGE_THREADS},{"thread",C_CHANGE_THREADS},{"Threads",C_CHANGE_THREADS},{"THREADS",C_CHANGE_THREADS}
	};

	currentMode = mode::CONVERTER;
	
	cout
		<< "::Initialised Program\n"
		<< "::The Converter is open\n"
		<< "::Enter \"back\" to return to the Menu or \"help\" to list all commands\n";

}

void Input_Manager::receiveInput(const std::string str) {
	istringstream stream(str);
	string first, more{""};
	stream >> first;
	stream.clear(stream.goodbit);

	for (string out; stream.rdstate() == stream.goodbit; more +=out+' ' )
		stream >> out;
	stream.clear(stream.goodbit);

	cout << endl;

	if (!first.empty())
		if (comExists(first)) execute(commandMap[first], more);
		else cout << "The Program does not know: " << first << endl;
}

void Input_Manager::execute(const instructions ID, string furtherCommands) {
	switch (currentMode) {
	case mode::EXIT_FATAL_ERROR:
		cout << "Warning::This program encountered an unknown Fatal Error!\n";
		break;

	case mode::EXIT_NO_ERROR:
		cout << "Warning::This program should have finished!\n";
		currentMode = mode::EXIT_FATAL_ERROR;
		break;

	case mode::MENU:
		executeCommandMenue(ID, furtherCommands);

		break;

	case mode::CONVERTER:
		executeCommandConverter(ID, furtherCommands);

		break;
	
	case mode::EXIT_WITH_ERROR:
	default:
		cout << "Warning::An Unknown Error happened!\n";
		
	}
	
}

void Input_Manager::executeCommandMenue(const instructions id, const std::string& commands) {
	istringstream istr(commands);
	string temp1{}, temp2{}, temp3{};

	switch (id) {
	case I_EXIT:
		cout << "::Initiated Exit\n";
		currentMode = mode::EXIT_NO_ERROR;
		break;

	case I_HELP:
		cout
			<< "Enter f/F/exit/EXIT/Exit To Finish The Program\n"
			<< "Enter open converter\n"
			<< "Enter \"help\" for more information\n\n";

		break;

	case I_OPEN:
		istr >> temp1;
		istr.clear(istr.goodbit);

		if (comExists(temp1)) {
			instructions cmID = commandMap[temp1];

			switch (cmID) {
			case I_CONVERTER:
				cout << "::Opening: " << temp1 << endl;
				currentMode = mode::CONVERTER;
				break;

			default:
				cout << "::Can not open: " << temp1 << endl;

			}
		}
		else if (!temp1.empty()) 
				cout << "::Can not open: " << temp1 << endl;
			
		break;

	case I_BACK:
		if (currentMode == mode::MENU) {
			cout
				<< "You are already at the main Menu!\n"
				<< "::Enter \"help\" to list all commands\n";
		}
		else {
			cout << "::An Error happened!\n::There is nothing to go back to!\n";
			currentMode = mode::EXIT_FATAL_ERROR;
		}

		break;

	case I_INFO:
		switch (currentMode) {
		case mode::MENU:
			noImpl();
			break;

		case mode::CONVERTER:
			noImpl();
			break;

		default:
			cout << "::An Error happened!\n::There is nothing to get Info from!\n";
			currentMode = mode::EXIT_FATAL_ERROR;

		}
		break;
	default:
		cout << "::Menu does not know that command!\n";
	}
}
void Input_Manager::executeCommandConverter(const instructions id, const std::string& commands) {
	istringstream istr(commands);
	string temp1{}, temp2{}, temp3{};

	switch (id) {
	case I_EXIT:

		cout << "::Initiated Exit\n";
		currentMode = mode::EXIT_NO_ERROR;
		break;

	case I_HELP:
		cout
			<< "Enter f/F/exit/EXIT/Exit To Finish The Program\n"
			<< "Enter SHADER/shader VERTEXNAME FRAGMENTNAME\n"
			<< "Enter SHADER/shader VERTEXNAME FRAGMENTNAME GEOMETRYNAME\n"
			<< "Enter convert SOURCENAME TARGETNAME\n"
			<< "Enter convert SOURCENAME\n";


		break;

	case I_BACK:
		if (currentMode == mode::CONVERTER) {
			cout 
				<< "::Switchting to Main Menu\n"
				<< "::Enter \"help\" to list all commands\n";

			currentMode = mode::MENU;
		}
		else {
			cout << "::An Error happened!\n::There is nothing to go back to!\n";
			currentMode = mode::EXIT_FATAL_ERROR;
		}
		

		break;
	case I_INFO:
		switch (currentMode) {
		case mode::MENU:
			noImpl();
			break;

		case mode::CONVERTER:
			noImpl();
			break;

		default:
			cout << "::An Error happened!\n::There is nothing to get Info from!\n";
			currentMode = mode::EXIT_FATAL_ERROR;

		}
		break;
	case C_SHADER:
		istr >> temp1 >> temp2 >> temp3;
		istr.clear(istr.goodbit);
		if (!temp1.empty() && !temp2.empty()) {
			cout << "::Changing Shader names\n";
			try {
				if(!temp3.empty()) converter.setShaderNames(temp1.c_str(), temp2.c_str(), temp3.c_str());
				else converter.setShaderNames(temp1.c_str(), temp2.c_str());
			}
			catch (exception& e) {
				cout << e.what() << endl;
			}
		}
		else {
			cout << "::The converter needs valid names for the Vertex and Fragment Shaders\n";
		}

		break;
	case C_CONVERT:
		istr >> temp1 >> temp2;
		istr.clear(istr.goodbit);

		if (!temp1.empty()) {
			cout << "::Converting Model. This might take some time!\n";
			try {
				if(temp2.empty())
					converter.convert(temp1.c_str());
				else
					converter.convert(temp1.c_str(), temp2.c_str());
				cout << "::Conversion Finished\n";

			}
			catch (exception& e) {
				cout << "::Conversion Incomplete!\n"
					 << e.what() << endl;
			}
		}
		else {
			cout << "::The converter needs a valid Sourcefile\n";
		}

		break;

	case C_CONVERT_FOLDER:
		noImpl(); // This has not yet been implemented!
		break;

	case C_CHANGE_THREADS:
		noImpl();	// This has not yet been implemented!
		break;		//
		unsigned tcnt;
		if (istr >> tcnt) {
			try {
				converter.setThreadsUsed(tcnt);
			}
			catch (exception& e) {
				cout << e.what()<<endl;
			}
		}
		else {
			cout << "::Invalid number!\n";
		}
		istr.clear(istr.goodbit);
		break;

	default:
		cout << "::Converter does not know that command!\n";
	}
}

inline void Input_Manager::noImpl() {
	cout << "::This Instruction has not been implemented yet!\n";
}

inline bool Input_Manager::comExists(string str) {
	return commandMap.find(str) != commandMap.cend();
}