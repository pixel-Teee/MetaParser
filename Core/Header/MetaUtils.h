#pragma once

//#include "Mustache/mustache.hpp"

#include "Cursor.h"

#include "Mustache/mustache.hpp"

namespace Utils {

	void ToString(const CXString& str, std::string& output);

	//kainjow::mustache::data::type TemplateBool(bool value);

	kainjow::mustache::data::type TemplateBool(bool value);

	std::string GetQualifiedName(const CursorType& type);
}