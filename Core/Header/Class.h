#pragma once

#include "LanguageType.h"

struct BaseClass
{
	BaseClass(const Cursor& cursor);

	std::string name;
};

class ReflectionParser;
class Class : public LanguageType
{
	friend class Field;
public:
	Class(const Cursor& cursor, const Namespace& currentNamespace);

	virtual bool ShouldCompile() const;

	kainjow::mustache::data CompileTemplate(const ReflectionParser* context) const override;
private:
	std::string m_Name;
	std::string m_DisplayName;
	std::string m_QualifiedName;

	template<typename T>
	using SharedPtrVector = std::vector<std::shared_ptr<T>>;

	SharedPtrVector<BaseClass> m_BaseClasses;

	SharedPtrVector<Field> m_fields;

	bool isAccessible() const;
};