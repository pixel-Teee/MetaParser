#pragma once

#include "ModuleFile.h"

#include "Cursor.h"
#include "Namespace.h"

class ReflectionParser
{
public:
	ReflectionParser();

	~ReflectionParser();

	void Parse();

	void GenerateFiles();

	kainjow::mustache::mustache LoadTemplate(const std::string& name) const;

	void GenerateHeader(std::string& output) const;
	
	void GenerateSource(std::string& output) const;

private:
	
	CXIndex m_Index;

	CXTranslationUnit m_TranslationUnit;

	//header template
	kainjow::mustache::mustache m_ModuleFileHaderTemplate;
	//source template

	//mutable std::unordered_map<std::string, std::string> m_templatePartialCache;

	std::unordered_map<std::string, ModuleFile> m_ModuleFiles;//generated reflection information

	void BuildClasses(const Cursor& cursor, Namespace& currentNamespace);
};