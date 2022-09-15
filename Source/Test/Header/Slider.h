#pragma once

enum class SliderType
{
	Horizontal,
	Vertical
};

struct Meta(Enable) Slider
{
	SliderType type;

	Slider(){}

	Slider(SliderType otherType) : type(otherType){}

	RTTR_REGISTRATION_FRIEND
};

