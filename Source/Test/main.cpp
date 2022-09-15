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

	rttr::variant slider = instance.get_type().get_property("SomeIntensityField").get_metadata("Slider");

	if (slider.can_convert<Slider>())
	{
		//Range value = range.convert<Range>();
		Slider value = slider.convert<Slider>();
		//std::cout << value.type << std::endl;
		if (value.type == SliderType::Vertical)
			std::cout << "SliderType Vertical" << std::endl;
		else
			std::cout << "SliderType Horizontal" << std::endl;
	}

	rttr::method testMethod = instance.get_type().get_method("Test");

	testMethod.invoke(instance, 1, 2);
	
	return 0;
}