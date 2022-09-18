#pragma once

#include "Header/Shader/Shader.h"
#include "Header/UI/Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

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

		int32_t kbItem;//stores the id of the widget that has the keyboard focus, zero means no widget has keyboard focus
		int32_t keyEntered;//stores the key that was pressed, zero in keyentered means no key was pressed
		int32_t keyMod;//stores the modifier flags(such as shift pressed)
		int32_t keyChar;
		int32_t lastWidget;//store the id of the last widget that was pressed
	};//global ui state, could put into a singleton

	struct DrawList
	{
		float vertices[3200];//3200 / 16 = 200
		uint32_t totalItem;
		uint32_t indices[6];
		uint32_t viewportWidth;
		uint32_t viewportHeight;
		float color[600];//200 * 3 = 600
		DrawList();
		~DrawList();
		uint32_t VBO;
		uint32_t VAO;
		uint32_t EBO;
		uint32_t textureIds[600];
		uint32_t whiteTexture;
	};//every frame will clear this

	void DrawRect(DrawList& drawList, int32_t x, int32_t y, int32_t w, int32_t h, float color[3], uint32_t textureId);

	void TotalDraw(DrawList& drawList, std::shared_ptr<Shader> pShader);

	void RenderText(DrawList& drawList, std::map<char, Character>& maps, std::string text, float x, float y, float scale, float color[3]);

	int32_t RegionHit(UIState& globalState, int32_t x, int32_t y, int32_t w, int32_t h);

	int32_t Button(UIState& globalState, DrawList& drawList, int32_t id, int32_t x, int32_t y);

	void ImGuiPrepare(UIState& globalState);

	void ImGuiFinish(UIState& globalState);

	int32_t Slider(UIState& globalState, DrawList& drawList, int32_t id, int32_t x, int32_t y, int32_t max, int& value);

	int textField(UIState& globalState, DrawList& drawList, std::map<char, Character>& maps, int32_t id, int32_t x, int32_t y, std::string& buffer);
}

