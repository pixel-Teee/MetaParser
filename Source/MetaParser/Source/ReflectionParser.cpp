#include "PreCompiled.h"

#include "Header/ReflectionParser.h"

#include "Header/Class.h"

#include "Header/Templates.h"

#include "Header/MetaUtils.h"

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

//from the LanguageType to get the source file's complete path
#define TRY_ADD_LANGUAGE_TYPE(handle, container) \
	if(handle->ShouldCompile())				\
	{										\
		auto file = handle->GetSourceFile();\
		m_ModuleFiles[file].container.emplace_back(handle);\
	}

#define COMPILE_TYPE_TEMPLATES(mustacheData, listName, container) \
	{													  \
		kainjow::mustache::data typeData { kainjow::mustache::data::type::list};\
		for(auto& handle : container) \
			typeData << handle->CompileTemplate(this);\
		mustacheData.set(listName, typeData);\
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

void ReflectionParser::AddGlobalTemplateData(kainjow::mustache::data& data)
{
	data["version"] = "1.0";
	data["targetName"] = m_Options.targetName;//reflection module target name
	data["inputSourceFile"] = m_Options.InputSourceFile;//???
	data["moduleHeaderFile"] = m_Options.ModuleHeaderFile;//module header file
	data["outputModuleSourceFile"] = m_Options.OutputModuleSource;//module source file
}

void ReflectionParser::GenerateModuleFile(const std::filesystem::path& fileHeader, const std::filesystem::path& fileSource, const std::string& SourceHeader, const ModuleFile& file)
{
	//header file
	{
		kainjow::mustache::data headerData{ kainjow::mustache::data::type::object };
		
		AddGlobalTemplateData(headerData);

		headerData["moduleFileName"] = file.name;//header file name(needed parsed header file name)

		std::filesystem::create_directory(fileHeader.parent_path());//generated reflection module header file parent directory

		Utils::WriteText(
			fileHeader.string(),
			m_ModuleFileHaderTemplate.render(headerData));
	}

	//source file
	{
		kainjow::mustache::data SourceData{ kainjow::mustache::data::type::object };

		AddGlobalTemplateData(SourceData);

		SourceData["moduleFileName"] = file.name;//header file name(needed parsed header file name)
		SourceData["moduleFileSourceHeader"] = SourceHeader;//need parsed header file path
		SourceData["moduleFileHeader"] = fileHeader.string();//generated reflection header file
		
		COMPILE_TYPE_TEMPLATES(SourceData, "class", file.classes);

		
		std::filesystem::create_directory(fileSource.parent_path());

		Utils::WriteText(fileSource.string(), m_ModuleFileSourceTemplate.render(SourceData));
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
	//need parsed header and source file root directory
	std::filesystem::path SourceRootDirectory(m_Options.SourceRoot);

	//output generated file directory
	std::filesystem::path OutputFileDirectory(m_Options.OutputModuleFileDirectory);

	//load the template file
	//m_ModuleFileHaderTemplate = LoadTemplate()
	m_ModuleFileHaderTemplate = LoadTemplate(TemplateModuleFileHeader);

	if (!m_ModuleFileHaderTemplate.is_valid())
	{
		std::stringstream error;

		error << "unable to compile module file header template" << std::endl;
		error << m_ModuleFileHaderTemplate.error_message();

		throw std::runtime_error(error.str());
	}

	m_ModuleFileSourceTemplate = LoadTemplate(TemplateModuleFileSource);

	if (!m_ModuleFileSourceTemplate.is_valid())
	{
		std::stringstream error;

		error << "unable to compile module file source template." << std::endl;
		error << m_ModuleFileSourceTemplate.error_message();

		throw std::runtime_error(error.str());
	}

	//module source file's data
	kainjow::mustache::data moduleFilesData{ kainjow::mustache::data::type::list };

	std::filesystem::path metaCacheFileName = m_Options.OutputModuleFileDirectory;

	metaCacheFileName /= ".meta-cache";

	//check the meta cache file exists?
	bool metaCacheFileExists = std::filesystem::exists(metaCacheFileName);

	std::string moduleFileCache;

	std::regex SpecialCharsRegex("[^a-zA-Z0-9]+");

	for (auto& file : m_ModuleFiles)
	{
		std::filesystem::path filePath(file.first);//file.first is complete path

		//path relative to the source root
		std::string relativeDir = std::filesystem::relative(filePath, SourceRootDirectory).replace_extension("").string();

		//filePath must in the SourceRootDirectory
		if(relativeDir.find_first_of("..") != std::string::npos)
			continue;

		std::filesystem::path outputFile = OutputFileDirectory / relativeDir;
		std::filesystem::path outputFileHeader = outputFile.replace_extension("Generated.h");
		std::filesystem::path outputFileSource = outputFile.replace_extension("Generated.cpp");

		//module file name
		//second name is relative path
		file.second.name = std::regex_replace(
			relativeDir,//relative path
			SpecialCharsRegex,
			"_"
		);

		moduleFileCache += file.second.name + "\n";

		kainjow::mustache::data moduleFileData{ kainjow::mustache::data::type::object };

		moduleFileData["name"] = file.second.name;//relative path
		moduleFileData["header"] = outputFileHeader.string();//generated header file

		moduleFilesData << moduleFileData;

		//if the generated file header/source doesn't exist, we need to regenerate
		if (!metaCacheFileExists || !std::filesystem::exists(outputFileHeader) || !std::filesystem::exists(outputFileSource))
		{
			GenerateModuleFile(outputFileHeader, outputFileSource, file.first, file.second);
		}
	}

	//module source file
	{
		auto SourceTemplate = LoadTemplate(TemplateModuleSource);

		if (!SourceTemplate.is_valid())
		{
			std::stringstream error;

			error << "unable to compile module source template." << std::endl;
			error << SourceTemplate.error_message();

			throw std::runtime_error(error.str().c_str());
		}

		kainjow::mustache::data SourceData{ kainjow::mustache::data::type::object };

		AddGlobalTemplateData(SourceData);

		SourceData.set("moduleFile", moduleFilesData);//contains multiple reflection header file

		std::filesystem::path SourcePath(m_Options.OutputModuleSource);

		std::filesystem::create_directory(SourcePath.parent_path());

		Utils::WriteText(SourcePath.string(), SourceTemplate.render(SourceData));
	}
}

kainjow::mustache::mustache ReflectionParser::LoadTemplate(const std::string& name) const
{
	std::filesystem::path path = std::filesystem::path(m_Options.TemplateDirectory);

	path.append(name);

	try {
		std::string text;

		//from the template directory to load the template
		Utils::LoadText(path.string(), text);

		return text;
	}
	catch (std::exception& e)
	{
		std::stringstream error;

		error << "unable to load template ";
		error << name << "." << std::endl;
		error << "-- " << e.what();
		
		Utils::FatalError(error.str());
	}

	//this will never happen
	return { "" };
}