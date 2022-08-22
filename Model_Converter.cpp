#include "Model_Converter.h"

#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

using std::string; 
using std::cin; using std::cout; using std::endl; 
using std::exception; using std::runtime_error; 
using std::ifstream; using std::ofstream;
using std::istringstream; using std::ostringstream;
using std::getline; using std::stoi; using std::stoul;
using std::unordered_map;
using std::vector;


Model_Converter::Model_Converter(sourceFormat argFormat) {
	if (argFormat == sourceFormat::CUSTOM) 
		throw runtime_error("Custom converting has not yet been implemented!");

	format = argFormat;
}
Model_Converter::~Model_Converter() {
}

void Model_Converter::convert(const char* sourceFileName, const char* newFileName, const char* vertexShaderName, const char* fragmentShaderName, const char* geometryShaderName) {
	setShaderNames(vertexShaderName, fragmentShaderName, geometryShaderName);
	convert(sourceFileName, newFileName);
}
void Model_Converter::convert(const char* sourceFileName, const char* newFileName) {
	if(vertShaderName.empty()||fragShaderName.empty())
		throw runtime_error("Vertex and Fragment Shader need to be set!");

	string targetName;
	constexpr size_t NotFound{ string::npos };
	if (newFileName && string(newFileName).find_first_not_of(' ') != NotFound)
			targetName = newFileName;
	else {
		targetName = string(sourceFileName);
		if (targetName.find('.') == NotFound)
			targetName += ".k_model";
		else
			targetName = targetName.substr(0, targetName.find_last_of('.')+1) + "k_model";
	}

	switch (format) {
	case sourceFormat::CUSTOM:
		convert_Custom(sourceFileName, targetName.c_str());
		break;
	case sourceFormat::WAVEFRONT:
		convert_WaveFront_Obj(sourceFileName, targetName.c_str());
		break;
	default:
		throw runtime_error("Invalid sourceFormat");
		break;
	}
}

void Model_Converter::setShaderNames(const char* vName, const char* fName, const char* gName) {
	if (!vName || !fName) {
		if (!vName) 
			if (!fName) throw std::exception("The Vertex and Fragment Name can not be null!");
			else throw std::exception("The Vertex Name can not be null!");
		else throw std::exception("The Fragment Name can not be null!");
	}
	else {
		vertShaderName = vName;
		fragShaderName = fName;
		if (gName) geoShaderName = gName;
		else geoShaderName = "NULL";
	}
	cout << "VertexShaderName == " << vName << "\nFragmentShaderName == " << fName << '\n';

	if (geoShaderName == "NULL") cout << "No GeometryShaderName has been set. The Default is \"NULL\"" << endl;
	else cout << "GeometryShaderName == " << gName << endl;
}

void Model_Converter::setThreadsUsed(unsigned cnt) {
	if (!cnt) {
		throw exception("Model_Converter: The Program needs at least 1 thread!");
		return;
	}
	else if (cnt > 4) {
		cout << "It is highly discouraged to use more than 4 Threads!\nDo you want to proceed anyways?: Enter Yes/No\n";
		string line;
		while (getline(cin, line)) {
			if (line == "Y" || line == "y" || line == "yes" || line == "YES" || line == "1") {
				threadsUsed = cnt;
				break;
			}
			else if (line == "N" || line == "n" || line == "no" || line == "NO" || line == "0") {
				break;
			}
			else
				cout << "Invalid Input. Enter \"Yes\" or \"No\"" << endl;
		}
		cin.clear(cin.goodbit);
	}

}

inline string checkSourceFileExistsAndTargetFileNotExists(const char* sourceFileName, const char* newFileName, ifstream& src) {
	string newNewFileName(newFileName);

	src.open(newFileName);
	if (src.is_open()) {
		newNewFileName = "new_";
		newNewFileName += newFileName;
		cout << "Model_Converter: The targetFile named: " << newFileName << " already exists!\nYour new filename is " << newNewFileName<<endl;
		src.close();
	}
	src.open(sourceFileName);

	if (!src.is_open())
		throw runtime_error("The sourceFile named: " + string(sourceFileName) + " was not found!\nNothing happened\n");
	
	if (newNewFileName != string(newFileName)) {
		src.close();
		return checkSourceFileExistsAndTargetFileNotExists(sourceFileName, newNewFileName.c_str(), src);
	}
	else return newNewFileName;
}

void Model_Converter::convert_WaveFront_Obj(const char* sourceFileName, const char* newFileName) {
	// CHECKING FILES
	ifstream src;
	string tempTarget = checkSourceFileExistsAndTargetFileNotExists(sourceFileName, newFileName,src);
	newFileName = tempTarget.c_str();

	src.clear(src.goodbit);

	// EXTRACTING DATA FROM FILES
	unordered_map<unsigned, string> vertices, textureCoords, vertexNormals;
	string indices;
	unsigned verticesLineCnt{ 1 }, textureLineCnt{ 1 }, normalsLineCnt{ 1 };

	/*
	* WAVEFRONT.obj files created by Blender follow the official wavefront format
	* thus, it is assumed that every WAVEFRONT.obj looks like the following:
	* 
	* # various lines of comments
	* name of additional resources
	* o name of segment of the model
	* example: o Cube.028__0
	* v various lines of coordinates of Vertices
	* example:  v -0.556751 -0.023763 -0.141540 
	* vt various lines of coordinates of Textures
	* example: vt 0.283512 0.401314
	* vn various lines of coordinates of VertexNormals
	* example: vn -0.0089 -0.5418 0.8404
	* s one bool line regarding smoothing
	* example: s off
	* f various lines of faces. Inbetween smoothing can be turned off or on
	* example: f 55/1/1 57/2/2 58/3/2
	* 
	* A modern face is a triangle and contains 3 full vertices. Meaning, each full vertex contains space coordinates, texture coordinates and normals.
	* Other forms like squares are deprecated!!!
	* 
	*/
	string line;
	while (getline(src, line)) {
		
		for (; line.substr(0, 2) != "v "; getline(src, line));
		for (; line.substr(0, 2) == "v "; getline(src, line))
			vertices[verticesLineCnt++] = line.substr(2);

		for (; line.substr(0, 3) != "vt "; getline(src, line));
		for (; line.substr(0, 3) == "vt "; getline(src, line)) 
			textureCoords[textureLineCnt++] = line.substr(2);

		for (; line.substr(0, 3) != "vn "; getline(src, line));
		for (; line.substr(0, 3) == "vn "; getline(src, line)) 
			vertexNormals[normalsLineCnt++] = line.substr(2);

		for (; line.substr(0, 2) != "s "; getline(src, line)) ;
		

		//SKIPS SMOOTHING , note: (false) == "off"  (true) == "1" WHY???

		while (line.substr(0, 2) == "s ") {

			while (getline(src, line) && line.substr(0, 2) == "f ") {
				indices += line.substr(1);

			}
		}
	}
	src.close();
	indices = indices.substr(1);


	// LOOKING FOR UNIQUE COMBINATIONS OF COMPLETE VERTICES
	// THIS WILL TAKE SOME TIME!

	vector<string> comboVec;	//set of unique combinations of attributes
	vector<unsigned> idVec;		//mapping indices to unique vertices
	istringstream istr(indices);

	string combo;

	for (unsigned cnt{}; istr >> combo; idVec.push_back(cnt)) {
		cnt = 0;
		for (auto iter = comboVec.cbegin(); iter != comboVec.cend() && iter->compare(combo) != 0; ++iter, ++cnt)
			;

		if (cnt == comboVec.size()) comboVec.push_back(combo);

	}


	istr.clear(istr.goodbit);

	// WRITING FIRST TWO LINES CONTAINING ADDITIONAL DATA

	ofstream target(newFileName);

	target
		<< vertShaderName << ' ' << fragShaderName << ' ' << ((geoShaderName.empty()) ? "NULL" : geoShaderName) << '\n'
		<< texturesUsed << ' ' << comboVec.size() << ' ' << idVec.size() << endl;	// comboVec.size() == indices

	// WRITING UNIQUE COMPLETE VERTICES

	// EXAMPLE 12/55/47 position/texture/normal
	for (auto iter = comboVec.cbegin(); iter != comboVec.cend();++iter) {

		//extract position of position vertex
		size_t pos[2]{};
		pos[0] = iter->find('/');
		string strPos = iter->substr(0, pos[0]);
		unsigned unPos = stoul(strPos.c_str());

		istr.str(vertices[unPos]);
		float v[3]{};
		istr >>v[0]>>v[1]>>v[2];	//doing this removes all redundant 0's
		istr.clear(istr.goodbit);


		pos[1] = iter->find('/',pos[0]+1);
		strPos = iter->substr(pos[0]+1,pos[1]-1-pos[0]);
		unPos = stoul(strPos.c_str());
		istr.str(textureCoords[unPos]);

		
		float t[2]{};
		istr >> t[0] >> t[1];	//doing this removes all redundant 0's
		istr.clear(istr.goodbit);


		strPos = iter->substr(pos[1] + 1);
		unPos = stoul(strPos.c_str());
		istr.str(vertexNormals[unPos]);


		float n[3]{};
		istr >> n[0] >> n[1]>>n[2];	
		istr.clear(istr.goodbit);

		target << v[0]<<' '<<v[1]<<' '<<v[2]<<' '<< t[0] << ' ' << t[1] << ' ' << n[0] << ' ' << n[1]<<' '<<n[2]<<' ';
	}

	target <<'\n';

	// WRITING INDICES

	for(auto iter=idVec.cbegin();iter!=idVec.cend()-1;++iter)
			target << *iter <<' ';
	target << idVec.back();

		target.close();
}
void Model_Converter::convert_Custom(const char* sourceFileName, const char* newFileName) {
	throw exception("Custom converting has not yet been implemented! No File was created nor changed");
}
