#include "PreCompiled.h"

#include "Header/MetaUtils.h"
#include "Header/Namespace.h"

namespace Utils {
	void ToString(const CXString& str, std::string& output)
	{
		//give the const char
		auto cstr = clang_getCString(str);

		output = cstr;

		//decrease the reference count
		clang_disposeString(str);
	}

	std::string GetQualifiedName(const CursorType& type)
	{
		if (type.GetKind() != CXType_Typedef)
			return type.GetDisplayName();

		auto declaration = type.GetDeclaration();

		auto parent = declaration.GetLexicalParent();

		Namespace parentNamespace;

		//walk up to the root namespace
		while (parent.GetKind() == CXCursor_Namespace)
		{
			parentNamespace.emplace(parentNamespace.begin(), parent.GetDisplayName());

			parent = parent.GetLexicalParent();
		}

		//add the display name as the end of the namespace
		parentNamespace.emplace_back(type.GetDisplayName());

		std::string result;
		
		if (parentNamespace.size() == 1)
		{
			return parentNamespace[1];
		}
		else
		{
			for (size_t i = 0; i < parentNamespace.size() - 1; ++i)
			{
				result += parentNamespace[i];

				result += "::";
			}
			result += parentNamespace.back();

			return result;
		}
	}

	void LoadText(const std::string& fileName, std::string& output)
	{
		std::ifstream input(fileName);

		if (!input)
		{
			std::stringstream error;

			error << "unable to open file \""
				<< fileName
				<< "\" for reading." << std::endl;
			error << strerror(errno);

			throw std::runtime_error(error.str());
		}

		//move the file pointer to end
		input.seekg(0, std::ios::end);

		output.reserve(static_cast<std::string::size_type>(input.tellg()));

		output.assign((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

		input.close();
	}

	void WriteText(const std::string& fileName, const std::string& text)
	{
		std::ofstream output(fileName);

		if (!output)
		{
			std::stringstream error;

			error << "unable to open file \""
				<< fileName << "\" for writing."
				<< std::endl;

			error << strerror(errno);

			throw std::runtime_error(error.str());
		}

		output << text;

		output.close();
	}

	void FatalError(const std::string& error)
	{
		std::cerr << "error: " << error << std::endl;

		exit(EXIT_FAILURE);
	}

	kainjow::mustache::data::type TemplateBool(bool value)
	{
		return value ? kainjow::mustache::data::type::bool_true : kainjow::mustache::data::type::bool_false;
	}
}