#pragma once

struct Range
{
	int32_t m_Min;

	int32_t m_Max;

	Range(){}
	Range(float min, float max)
		:m_Min(min), m_Max(max) {}
};