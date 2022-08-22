#pragma once
/* @author Michael Koslowski
*
* Last Revision: 22.08.2022
*/

#include "Model_Converter.h"

#include <unordered_map>
#include <string>

class Input_Manager {
public:
	enum class mode:int {
		EXIT_FATAL_ERROR = -3,
		EXIT_WITH_ERROR = -2,
		EXIT_NO_ERROR= -1,
		MENU=0,
		CONVERTER = 1
		
	};
	Input_Manager();
	
	void operator[](const std::string str) { receiveInput(str); };
	/*
	* only one command line at a time is allowed
	*/
	void receiveInput(const std::string str);
	const mode getCurrentMode() { return currentMode; }
	const bool wantsToExit() { return (currentMode < mode::MENU); }

private:

	enum instructions :unsigned {
		/* General */

		I_EXIT=1,
		I_HELP=2, 
		I_OPEN=3, 
		I_CONVERTER = 4, 
		I_BACK=5, 
		I_INFO=6,

		/* Converter */

		C_SHADER=7, 
		C_CONVERT=8, 
		C_CONVERT_FOLDER=9,
		C_CHANGE_THREADS = 10

	};

	mode currentMode;

	/* command/ID */
	std::unordered_map<std::string, instructions> commandMap;

	void execute(const instructions ID,std::string="");
	Model_Converter converter{ sourceFormat::WAVEFRONT };

	void executeCommandMenue(const instructions id, const std::string& command);
	void executeCommandConverter(const instructions id, const std::string& command);

	inline bool comExists(std::string str);

	/* placeholder for future features */
	inline void noImpl();

};
