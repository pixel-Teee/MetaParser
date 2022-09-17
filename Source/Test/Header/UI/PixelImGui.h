#pragma once

#include "Header/Shader/Shader.h"

namespace PixelImGui
{
#ifdef IMGUI_SRC_ID
#define GEN_ID ((IMGUI_SRC_ID) + (__LINE__))
#else
#define GEN_ID (__LINE__)
#endif
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
		float vertices[3000];//3000 / 8 = 375
		uint32_t totalItem;
		uint32_t indices[6];
		uint32_t viewportWidth;
		uint32_t viewportHeight;
		float color[1125];//1125
		DrawList();
		~DrawList();
		uint32_t VBO;
		uint32_t VAO;
		uint32_t EBO;
	};//every frame will clear this

	void DrawRect(DrawList& drawList, int32_t x, int32_t y, int32_t w, int32_t h, float color[3]);

	void TotalDraw(DrawList& drawList, std::shared_ptr<Shader> pShader);

	int32_t RegionHit(UIState& globalState, int32_t x, int32_t y, int32_t w, int32_t h);

	int32_t Button(UIState& globalState, DrawList& drawList, int32_t id, int32_t x, int32_t y);

	void ImGuiPrepare(UIState& globalState);

	void ImGuiFinish(UIState& globalState);

	int32_t Slider(UIState& globalState, DrawList& drawList, int32_t id, int32_t x, int32_t y, int32_t max, int& value);
}

