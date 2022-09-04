#include "PreCompiled.h"

#include "Header/Class.h"
#include "Header/Field.h"
#include "Header/MetaDataConfig.h"

const auto kTypeObject = "ursine::meta::Object";
const auto kTypeMetaProperty = "ursine::meta::MetaProperty";

static const std::vector<std::string> nativeTypes
{
	kTypeObject,
	kTypeMetaProperty
};

static bool isNativeType(const std::string& qualifinedName)
{
	return std::find(
		nativeTypes.begin(),
		nativeTypes.end(),
		qualifinedName
	) != nativeTypes.end();
}

BaseClass::BaseClass(const Cursor& cursor)
	:name(cursor.GetType().GetCanonicalType().GetDisplayName())
{
}

Class::Class(const Cursor& cursor, const Namespace& currentNamespace)
	:LanguageType(cursor, currentNamespace)
	,m_Name(cursor.GetDisplayName())
	,m_QualifiedName(cursor.GetType().GetDisplayName())
{
	//from the meta data to get the display name
	auto displayName = m_MetaData.GetNativeString(nativeProperty::DisplayName);

	if (displayName.empty())
	{
		m_DisplayName = m_QualifiedName;
	}
	else
	{
		m_DisplayName = displayName;
	}

	for (auto& child : cursor.GetChildren())
	{
		switch(child.GetKind())
		{
			case CXCursor_CXXBaseSpecifier:
			{
				auto baseClass = new BaseClass(child);

				m_BaseClasses.emplace_back(baseClass);

				if (isNativeType(baseClass->name))
					m_Enabled = !m_MetaData.GetFlag(nativeProperty::Disable);

				break;
			}
			case CXCursor_FieldDecl:
			{
				m_fields.emplace_back(new Field(child, currentNamespace, this));
				break;
			}
			default:
				break;
		}

	}
}

bool Class::ShouldCompile() const
{
	return isAccessible() && !isNativeType(m_QualifiedName);
}

kainjow::mustache::data Class::CompileTemplate(const ReflectionParser* context) const
{
	kainjow::mustache::data data{ kainjow::mustache::data::type::object };

	data["displayName"] = m_DisplayName;
	
	data["qualifiedName"] = m_QualifiedName;

	data["ptrTypeEnabled"] = m_ptrTypeEnabled ? kainjow::mustache::data::type::bool_true : kainjow::mustache::data::type::bool_false;

	m_MetaData.CompileTemplateData(data, context);

	//base classes
	{
		kainjow::mustache::data baseClasses{ kainjow::mustache::data::type::list };

		int32_t i = 0;

		for (auto& baseClass : m_BaseClasses)
		{
			//ignore native types, MetaProperty and Object will be ignore
			if(isNativeType(baseClass->name))
				continue;;

			kainjow::mustache::data item{ kainjow::mustache::data::type::object };

			item["name"] = baseClass->name;

			item["isLast"] = (i == m_BaseClasses.size() - 1) ? kainjow::mustache::data::type::bool_true : kainjow::mustache::data::type::bool_false;

			baseClasses << item;

			++i;
		}

		data.set("baseClass", baseClasses);
	}

	//don't do anything else if only registering
	if (m_MetaData.GetFlag(nativeProperty::Register))
		return data;

	//fields
	{
		kainjow::mustache::data fields{ kainjow::mustache::data::type::list };

		/*
		for (auto& filed : m_fields)
		{

		}
		*/
		for (auto& field : m_fields)
		{
			if (field->ShouldCompile())
				fields << field->CompileTemplate(context);
		}

		data.set("field", fields);
	}

	return data;
}

bool Class::isAccessible() const
{
	return false;
}
