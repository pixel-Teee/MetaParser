#include "Header/Range.h"

#include "Header/ReflectionModule.h"

#include "rttr/variant.h"

int main()
{
	AllocateModule();

	Range range;
	range.m_Min = 4.0f;
	range.m_Max = 100.0f;

	rttr::instance test = range;
	rttr::type classType = rttr::type::get_by_name("Range");
	
	rttr::variant value = classType.get_property_value("m_Min", test);
	if (value.can_convert<int32_t>())
	{
		std::cout << value.convert<int32_t>() << std::endl;
	}

	rttr::variant slider = classType.get_property("m_Min").get_metadata("Slider");

	if (slider.get_value<SliderType>() == SliderType::Horizontal)
	{
		std::cout << "Slider Type Horizontal" << std::endl;
	}
	return 0;
}