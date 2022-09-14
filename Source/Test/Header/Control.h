#pragma once

#include "Slider.h"
#include "Range.h"

class Control
{
public:
	Meta(Range(0.0f, 1.0f), Slider(SliderType::Vertical))
	float SomeIntensityField;
};
