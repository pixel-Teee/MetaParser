#pragma once

#include "LanguageType.h"
#include "Invokable.h"

class Class;

class Method : public LanguageType, public Invokable
{
public:
	Method(const Cursor& cursor, const Namespace& currentNamespace, Class* parent = nullptr);

	virtual ~Method() {}

	bool ShouldCompile() const;

	kainjow::mustache::data CompileTemplate(const ReflectionParser* context) const override;

private:
	bool m_IsConst;

	Class* m_Parent;

	std::string m_Name;

	bool IsAccessible() const;

	std::string GetQualifiedSignature() const;
};