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

ReflectionParser::ReflectionParser()
{
}

ReflectionParser::~ReflectionParser()
{
}

void ReflectionParser::Parse()
{
}

void ReflectionParser::GenerateFiles()
{
}

void ReflectionParser::GenerateHeader(std::string& output) const
{
}

void ReflectionParser::GenerateSource(std::string& output) const
{
}
