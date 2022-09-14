#include "PreCompiled.h"

#include "Header/Method.h"
#include "Header/Class.h"
#include "Header/MetaDataConfig.h"

Method::Method(const Cursor& cursor, const Namespace& currentNamespace, Class* parent /*= nullptr*/)
	:LanguageType(cursor, currentNamespace)
	,Invokable(cursor)
	,m_IsConst(cursor.IsConst())
	,m_Parent(parent)
	,m_Name(cursor.GetSpelling())
{

}

bool Method::ShouldCompile() const
{
	return IsAccessible();
}

kainjow::mustache::data Method::CompileTemplate(const ReflectionParser* context) const
{
	kainjow::mustache::data data{ kainjow::mustache::data::type::object };

	data["name"] = m_Name;

	data["parentQualifiedName"] = m_Parent->m_QualifiedName;

	data["qualifiedSignature"] = GetQualifiedSignature();

	m_MetaData.CompileTemplateData(data, context);

	return data;
}

bool Method::IsAccessible() const
{
	if (m_accessModifier != CX_CXXPublic)
		return false;
	//if the parent wants white listed method, then we must have the enable flag
	if (m_Parent->GetMetaData().GetFlag(nativeProperty::WhiteListMethods))
		return m_MetaData.GetFlag(nativeProperty::Enable);

	//must not be explicitly disabled
	return !m_MetaData.GetFlag(nativeProperty::Disable);
}

std::string Method::GetQualifiedSignature() const
{
	//argument list
	std::string argumentList;

	if (m_Signature.size() == 1)
	{
		argumentList = m_Signature[0];
	}
	else if (m_Signature.size() >= 2)
	{
		for (uint32_t i = 0; i < m_Signature.size() - 1; ++i)
		{
			argumentList = argumentList + m_Signature[i] + ", ";
		}

		argumentList += m_Signature.back();
	}

	std::string constNess = m_IsConst ? " const" : "";

	return m_ReturnType + "(" + m_Parent->m_QualifiedName + "::*)(" + argumentList + ")" + constNess;
}

