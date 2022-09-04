#include "PreCompiled.h"

#include "Header/ReflectionParser.h"

#include "Header/Class.h"

#define RECURSE_NAMESPACES(kind, cursor, method, ns)\
	if(kind == CXCursor_Namespace)\
	{							\
		auto displayName = cursor.GetDisplayName();\
		if(!displayName.empty())\
		{								\
			ns.emplace_back(displayName);\
			method(cursor, ns);\
			ns.pop_back();\
		}\
	}

#define TRY_ADD_LANGUAGE_TYPE(handle, container) \
	if(handle->ShouldCompile())				\
	{										\
		auto file = handle->GetSourceFile();\
		m_ModuleFiles[file].container.emplace_back(handle);\
	}

void ReflectionParser::BuildClasses(const Cursor& cursor, Namespace& currentNamespace)
{
	for (auto& child : cursor.GetChildren())
	{
		auto kind = child.GetKind();

		//actual definition and a class or struct
		if (child.IsDefinition() && (kind == CXCursor_ClassDecl || kind == CXCursor_StructDecl))
		{
			auto klass = std::make_shared<Class>(child, currentNamespace);

			//add to m_Modules[SourceFile].classes = klass
			TRY_ADD_LANGUAGE_TYPE(klass, classes);
		}
		else if (kind == CXCursor_TypedefDecl)
		{
			auto displayName = child.GetDisplayName();

			//external declaration, they're always compiled, but only registered
			
		}

		//if the cursor is namespace, then recurse invoke BuildClasses, and place the namespace to currentNamespace
		RECURSE_NAMESPACES(kind, child, BuildClasses, currentNamespace);
	}
}

ReflectionParser::ReflectionParser(const ReflectionOptions& options)
	:m_Options(options),
	m_Index(nullptr),
	m_TranslationUnit(nullptr),
	m_ModuleFileHaderTemplate("")
{
}

ReflectionParser::~ReflectionParser()
{
	if (m_TranslationUnit)
		clang_disposeTranslationUnit(m_TranslationUnit);

	if (m_Index)
		clang_disposeIndex(m_Index);
}

void ReflectionParser::Parse()
{
	m_Index = clang_createIndex(true, m_Options.displayDiagnostics);

	std::vector<const char*> arguments;

#if defined(SYSTEM_INCLUDE_DIRECTORY)
	arguments.emplace_back("-I"SYSTEM_INCLUDE_DIRECTORY);
#endif

	for (auto& argument : m_Options.Arguments)
		arguments.emplace_back(argument.c_str());

	m_TranslationUnit = clang_createTranslationUnitFromSourceFile(m_Index,
		m_Options.InputSourceFile.c_str(),
		static_cast<int32_t>(arguments.size()),
		arguments.data(),
		0,
		nullptr);

	auto cursor = clang_getTranslationUnitCursor(m_TranslationUnit);

	Namespace tempNamespace;

	BuildClasses(cursor, tempNamespace);

	tempNamespace.clear();
}

void ReflectionParser::GenerateFiles()
{
}

kainjow::mustache::mustache ReflectionParser::LoadTemplate(const std::string& name) const
{
	return kainjow::mustache::mustache();
}

void ReflectionParser::GenerateHeader(std::string& output) const
{
}

void ReflectionParser::GenerateSource(std::string& output) const
{
}
