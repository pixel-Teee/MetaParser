#pragma once

struct Range
{
	float m_Min;

	float m_Max;

	Range(){}

	Range(float min, float max)
		:m_Min(min), m_Max(max) {}
}Meta(Enable);