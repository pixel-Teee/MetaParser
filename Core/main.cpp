#include "PreCompiled.h"

int main(int argc, char* argv[])
{
	auto start = std::chrono::system_clock::now();

	//------from the command argument line to get file name------
	//std::cout << argv[0] << std::endl; argv[0] is executable's full path
	//std::cout << argc << std::endl; argc is 1
	
	for (int32_t i = 1; i < argc; ++i)
	{

	}
	//------from the command argument line to get file name------

	//------calculate time------
	auto duration = std::chrono::system_clock::now() - start;

	std::cout << "completed in "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << std::endl;
	//------calculate time------
	return 0;
}