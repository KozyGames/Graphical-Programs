#pragma once
#include <string>

/*
* CUSTOM: not yet implemented
* WAVEFRONT: as it is used in the software Blender
*
*/
enum class sourceFormat : int {
	CUSTOM = 0,
	WAVEFRONT = 1

};

class Model_Converter {
public:

	Model_Converter(sourceFormat argFormat = sourceFormat::CUSTOM);
	Model_Converter() = delete;

	~Model_Converter();

	sourceFormat format;

	/* Conversions */

	/*
	* throws exception when:
	* - sourceFile is not found
	* - fileName already exists
	*/
	void convert(const char* sourceFileName, const char* newFileName, const char* vertexShaderName, const char* fragmentShaderName, const char* geometryShaderName = nullptr);
	/*
	* uses the shaderNames that have been previously set
	* throws exception when:
	* - sourceFile is not found
	* - vertex and fragment shaderNames are not set
	* - fileName already exists
	* 
	* - if newFileName is null or does not contain any characters, then it takes the sourcefilename and replaces everything after the last occurence of '.' with "k_model"
	* - - if there is not at least one '.', ".k_model" is added
	*/
	void convert(const char* sourceFileName, const char* newFileName=nullptr); 

	//TO DO
	/*	add a function that adds content to an existing file
	* hints:
	* - think about what to do when shaders are the same
	* - - partially different
	* - - completely different
	* - - geometry Shader is different
	*/


	/* Settings */

	void setShaderNames(const char* vertexShaderName, const char* fragmentShaderName, const char* geometryShaderName = nullptr);	

	unsigned texturesUsed{ 1 }; //might be only temporary

	/*
	* can not be 0. It is highly discouraged to use more than 4. 
	* 
	*/
	void setThreadsUsed(unsigned cnt);	
	const unsigned getThreadsUsed() { return threadsUsed; }

private:
	std::string vertShaderName{ "" }, fragShaderName{ "" }, geoShaderName{ "" };	

	unsigned threadsUsed{ 1 };


	void convert_WaveFront_Obj(const char* sourceFileName, const char* newFileName);
	void convert_Custom(const char* sourceFileName, const char* newFileName);

};