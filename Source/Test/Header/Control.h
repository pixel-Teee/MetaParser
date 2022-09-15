#pragma once

#include "Slider.h"
#include "Range.h"

class Control
{
public:
	//Meta(Range(0.0f, 1.0f), Slider(SliderType::Vertical))
	Meta(Slider(SliderType::Vertical))
	float SomeIntensityField;

	void Test();

	void Test(int32_t a, int32_t b) const;
}Meta(Enable);
