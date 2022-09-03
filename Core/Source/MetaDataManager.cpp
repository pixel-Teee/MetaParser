#include "PreCompiled.h"

#include "Header/MetaDataManager.h"

MetaDataManager::MetaDataManager(const Cursor& cursor)
{
	//get class's annotate

	//"Key = Value, Key2, Key = "Yep!""
	for (auto& child : cursor.GetChildren())
	{
		if(child.GetKind() != CXCursor_AnnotateAttr)
			continue;;

		for (auto& prop : extractProperties(child))
			m_Properties[prop.first] = prop.second;
	}
}

std::string MetaDataManager::GetProperty(const std::string& key) const
{
	auto search = m_Properties.find(key);

	//use an empty string by default

	return search == m_Properties.end() ? "" : search->second;
}

bool MetaDataManager::GetFlag(const std::string& key) const
{
	return m_Properties.find(key) == m_Properties.end() ? false : true;
}

std::string MetaDataManager::GetNativeString(const std::string& key) const
{
	auto search = m_Properties.find(key);

	//wasn't set
	if (search == m_Properties.end())
		return "";

	//quoted
	static const std::regex qutotedString(
		//opening quote
		"(?:\\s*\")"

		//actual string contents
		"([^\"]*)"

		//closing quote
		"\"",
		std::regex::icase
	);

	auto& value = search->second;

	auto flags = std::regex_constants::match_default;

	//from the : "xx" to get the xx
	std::smatch match;

	if (std::regex_search(value.cbegin(), value.cend(), match, qutotedString, flags))
	{
		//get actual string content
		return match[1].str();
	}

	//couldn't find one
	return "";
}

std::vector<MetaDataManager::Property> MetaDataManager::extractProperties(const Cursor& cursor) const
{
	//std::vector<std::pair<std::string, std::string>>
	std::vector<Property> properties;

	return std::vector<Property>();
}
