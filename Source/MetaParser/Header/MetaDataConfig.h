#pragma once

namespace nativeProperty
{
	const auto HaveAnnotate = "HaveAnnotate";
	const auto Enable = "Enable";
	const auto Disable = "Disable";
	const auto WhiteListMethods = "WhiteListMethods";

	const auto Register = "Register";

	const auto DisplayName = "DisplayName";

	const auto EnablePtrType = "EnablePtrType";
	const auto EnableConstPtrType = "EnableConstPtrType";

	const auto EnableArrayType = "EnableArrayType";

	const auto DisableNonDynamicCtor = "DisableNonDynamic";
	const auto DynamicCtorWrap = "WrapObject";

	const auto ExplicitGetter = "Getter";
	const auto ExplicitSetter = "Setter";

	const auto VeryExplicitGetter = "ExplicitGetter";
	const auto VeryExplicitSetter = "ExplicitSetter";
}

const auto kMetaExternalTypeDefName = "__META_EXTERNAL__";