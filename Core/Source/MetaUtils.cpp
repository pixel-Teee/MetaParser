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



	kainjow::mustache::data::type TemplateBool(bool value)
	{
		return value ? kainjow::mustache::data::type::bool_true : kainjow::mustache::data::type::bool_false;
	}
}