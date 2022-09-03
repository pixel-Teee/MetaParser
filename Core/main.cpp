#include "PreCompiled.h"
/*
std::ostream& operator<<(std::ostream& stream, const CXString& str)
{
	stream << clang_getCString(str);
	clang_disposeString(str);
	return stream;
}
*/

int main(int argc, char* argv[])
{
	/*
	kainjow::mustache::mustache tmpl("Hello {{what}}!");

	std::cout << tmpl.render({ "what", "World" }) << std::endl;
	*/

	//CXIndex index = clang_createIndex(0, true);

	//auto& path = std::filesystem::current_path();

	//std::cout << path.string() << std::endl;

	/*
	std::vector<const char*> arguments = {
		{"-x"},
		{"c++"},
		{"-std=c++11"}
	};
	

	CXTranslationUnit unit = clang_parseTranslationUnit(index, "Debug//test//Test.h", arguments.data(), static_cast<int>(arguments.size()), nullptr, 0, CXTranslationUnit_None);

	if (unit == nullptr)
	{
		std::cerr << "unable to parse translation unit" << std::endl;
		exit(1);
	}

	CXCursor cusor = clang_getTranslationUnitCursor(unit);
	clang_visitChildren(cusor, [](CXCursor c, CXCursor parent, CXClientData clientData)
	{
		//std::cout << "Cusor kind: " << clang_getCursorKind(c) << std::endl;
		//return CXChildVisit_Recurse;

		std::cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
			<< clang_getCursorKindSpelling(clang_getCursorKind(c)) << "'\n";
		return CXChildVisit_Recurse;
	}, nullptr);

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);
	*/

	auto start = std::chrono::system_clock::now();

	//------from the command argument line to get file name------

	//------from the command argument line to get file name------

	//------calculate time------
	auto duration = std::chrono::system_clock::now() - start;

	std::cout << "completed in "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << std::endl;
	//------calculate time------
	return 0;
}