#pragma once
#include "Slider.h"

struct Range
{
	Meta(Slider(SliderType::Horizontal))
	int32_t m_Min;

	int32_t m_Max;
};