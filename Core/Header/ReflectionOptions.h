#pragma once

struct ReflectionOptions
{
	bool displayDiagnostics;//display error message

	std::string targetName;

	std::string SourceRoot;//header and source's directory

	std::string InputSourceFile;//?

	std::string ModuleHeaderFile;//?

	std::string OutputModuleSource;//module source, will reference meta file to register
	
	std::string OutputModuleFileDirectory;//output reflection file directory

	std::string TemplateDirectory;//template data file directory

	std::vector<std::string> Arguments;
};