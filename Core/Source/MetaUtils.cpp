#include "PreCompiled.h"

#include "Header/MetaUtils.h"

namespace Utils {
	void ToString(const CXString& str, std::string& output)
	{
		//give the const char
		auto cstr = clang_getCString(str);

		output = cstr;

		//decrease the reference count
		clang_disposeString(str);
	}
}