#pragma once

#include "Slider.h"
#include "Range.h"

class Meta(Enable) Control
{
public:
	Meta()
	Control() {}

	Meta()
	Control(int32_t a, int32_t b) {}

	Meta()
	Control(bool color) {}

	//Meta(Range(0.0f, 1.0f), Slider(SliderType::Vertical))
	Meta(Slider(SliderType::Vertical), Range(0.0f, 1.0f))
	float SomeIntensityField;

	void Test();
	//Meta(Slider(SliderType::Horizontal), Range(0.0f, 1.0f))
	Meta()
	void Test(int32_t a, int32_t b) const;

	Meta(Slider(SliderType::Horizontal))
	void Test(int32_t a, int32_t b);
private:

	Meta()
	float QAQ;

	Meta()
	void ABB(int32_t a, int32_t b);

	RTTR_REGISTRATION_FRIEND
};
