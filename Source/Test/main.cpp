#include "Header/Range.h"

#include "Header/ReflectionModule.h"

#include "rttr/variant.h"

#include "Header/Control.h"

int main()
{
	AllocateModule();

	Control control;

	control.SomeIntensityField = 1.2f;

	rttr::variant instance = control;

	rttr::variant range = instance.get_type().get_property("SomeIntensityField").get_metadata("Range");

	if (range.can_convert<Range>())
	{
		Range value = range.convert<Range>();
		std::cout << value.m_Min << " " << value.m_Max << std::endl;
	}
	
	return 0;
}