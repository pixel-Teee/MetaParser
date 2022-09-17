#pragma once

#include "Header/Shader/Shader.h"

namespace PixelImGui
{
	struct UIState
	{
		int32_t mouseX;
		int32_t mouseY;//mouse position

		int32_t mouseDown;//button state

		int32_t hotItem;//current hot and current active item IDs
		int32_t activeItem;
	};//global ui state, could put into a singleton

	struct DrawList
	{
		float vertices[3000];
		uint32_t totalItem;
		uint32_t indices[6];
		uint32_t viewportWidth;
		uint32_t viewportHeight;
		DrawList();
		~DrawList();
		uint32_t VBO;
		uint32_t VAO;
		uint32_t EBO;
	};//every frame will clear this

	void DrawRect(DrawList& drawList, int32_t x, int32_t y, int32_t w, int32_t h);

	void TotalDraw(DrawList& drawList, std::shared_ptr<Shader> pShader);
}

