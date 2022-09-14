#pragma once
enum class SliderType
{
	Horizontal,
	Vertical
};

struct Slider
{
	SliderType type;

	Slider(){}

	Slider(SliderType otherType) : type(otherType){}
};

