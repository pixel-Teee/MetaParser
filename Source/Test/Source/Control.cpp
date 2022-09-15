#include "PreCompiled.h"

#include "Header/Control.h"

void Control::Test()
{
	std::cout << "Control Test" << std::endl;
}

void Control::Test(int32_t a, int32_t b) const
{
	std::cout << a + b << std::endl;
}
