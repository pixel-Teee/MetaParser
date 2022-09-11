#include "PreCompiled.h"

#include "Header/ReflectionOptions.h"
#include "Header/ReflectionParser.h"

int main(int argc, char* argv[])
{
	//auto currentPath = std::filesystem::current_path();//E:\MetaParser\build\Core

	//std::cout << currentPath.string() << std::endl;

	auto exeDir = std::filesystem::path(argv[0]).parent_path();

	//std::cout << exeDir.string() << std::endl;

	if (!exeDir.empty())
		std::filesystem::current_path(exeDir);

	auto start = std::chrono::system_clock::now();

	//------from the command argument line to get file name------
	//std::cout << argv[0] << std::endl; argv[0] is executable's full path
	//std::cout << argc << std::endl; argc is 1
	ReflectionOptions options;

	if (argv[1] == nullptr || argv[2] == nullptr || argv[3] == nullptr || argv[4] == nullptr
		|| argv[5] == nullptr || argv[6] == nullptr || argv[7] == nullptr)
	{
		std::cout << "command line argument is error!" << std::endl;
	}
	else
	{
		options.targetName = argv[1];
		options.SourceRoot = argv[2];
		options.InputSourceFile = argv[3];
		options.ModuleHeaderFile = argv[4];
		options.OutputModuleSource = argv[5];
		options.TemplateDirectory = argv[6];
		options.TemplateDirectory = argv[7];
	}

	try
	{
		ReflectionParser parser(options);
		parser.Parse();
		parser.GenerateFiles();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	//------from the command argument line to get file name------

	//------calculate time------
	auto duration = std::chrono::system_clock::now() - start;

	std::cout << "completed in "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << std::endl;
	//------calculate time------
	return 0;
}