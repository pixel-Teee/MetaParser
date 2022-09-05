#pragma once

struct ReflectionOptions
{
	bool displayDiagnostics;//display error message

	std::string targetName;

	std::string SourceRoot;//header and source's directory

	std::string InputSourceFile;//need parsed header and source

	std::string ModuleHeaderFile;//in terms of the module header file, to generate the module source file

	std::string OutputModuleSource;//module source, will reference meta file to register
	
	std::string OutputModuleFileDirectory;//output reflection file directory

	std::string TemplateDirectory;//template data file directory

	std::vector<std::string> Arguments;
};