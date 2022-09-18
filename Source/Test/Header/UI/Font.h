#pragma once

struct Character {
	uint32_t textureId;
	
	float size[2];//font size
	float bearing[2];
	uint32_t advance;
};