#pragma once

struct Meta(Enable) Range
{
	float m_Min;

	float m_Max;

	Range(){}

	Range(float min, float max)
		:m_Min(min), m_Max(max) {}

	RTTR_REGISTRATION_FRIEND
};