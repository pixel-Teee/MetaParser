#include "PreCompiled.h"
#include "Header/UI/PixelImGui.h"

#include "glad/glad.h"

namespace PixelImGui
{
	void DrawRect(DrawList& drawList, int32_t x, int32_t y, int32_t w, int32_t h, float color[3])
	{
		float invWidth = 1.0f / (float)drawList.viewportWidth;
		float invHeight = 1.0f / (float)drawList.viewportHeight;
			
		//add a rect to draw list
		drawList.vertices[drawList.totalItem * 8] = (float)(x + w) * invWidth * 2.0f - 1.0f;
		drawList.vertices[drawList.totalItem * 8 + 1] = (1.0f - (float)y * invHeight) * 2.0f - 1.0f;//upper right corner

		drawList.vertices[drawList.totalItem * 8 + 2] = (float)(x + w) * invWidth * 2.0f - 1.0f;
		drawList.vertices[drawList.totalItem * 8 + 3] = (1.0f - (float)(y + h) * invHeight) * 2.0f - 1.0f;//down right corner

		drawList.vertices[drawList.totalItem * 8 + 4] = (float)x * invWidth * 2.0f - 1.0f;
		drawList.vertices[drawList.totalItem * 8 + 5] = (1.0f - (float)(y + h) * invHeight) * 2.0f - 1.0f;//down left corner

		drawList.vertices[drawList.totalItem * 8 + 6] = (float)x * invWidth * 2.0f - 1.0f;
		drawList.vertices[drawList.totalItem * 8 + 7] = (1.0f - (float)y * invHeight) * 2.0f - 1.0f;//upper left corner	

		drawList.color[drawList.totalItem * 3] = color[0];
		drawList.color[drawList.totalItem * 3 + 1] = color[1];
		drawList.color[drawList.totalItem * 3 + 2] = color[2];

		++drawList.totalItem;

		//std::cout << x << " " << y << std::endl;
	}

	DrawList::DrawList()
	{
		totalItem = 0;
		//indices = { 0, 1, 3, 1, 2, 3 };
		indices[0] = 0, indices[1] = 1, indices[2] = 3, indices[3] = 1, indices[4] = 2, indices[5] = 3;

		viewportWidth = 800, viewportHeight = 600;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);//8 vertices, four corners

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 6, indices, GL_STATIC_DRAW);
	}

	DrawList::~DrawList()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void TotalDraw(DrawList& drawList, std::shared_ptr<Shader> pShader)
	{
		pShader->use();
		glBindVertexArray(drawList.VAO);
		//glBindVertexArray(drawList.VBO);
		for (uint32_t i = 0; i < drawList.totalItem; ++i)
		{
			pShader->setFloat3("color", &drawList.color[i * 3]);
			glBindBuffer(GL_ARRAY_BUFFER, drawList.VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), &drawList.vertices[i * 8]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		drawList.totalItem = 0;
		glBindVertexArray(0);
	}

	int32_t RegionHit(UIState& globalState, int32_t x, int32_t y, int32_t w, int32_t h)
	{
		if (globalState.mouseX < x || globalState.mouseY < y || globalState.mouseX >= x + w || globalState.mouseY >= y + h)
			return 0;
		return 1;
	}

	int32_t Button(UIState& globalState, DrawList& drawList, int32_t id, int32_t x, int32_t y)
	{
		//check whether the button should be hot
		if (RegionHit(globalState, x, y, 128, 128))
		{
			globalState.hotItem = id;
			if (globalState.activeItem == 0 && globalState.mouseDown)
				globalState.activeItem = id;
		}

		static float shadowColor[3] = { 0.1f, 0.1f, 0.1f };

		//render shadow
		DrawRect(drawList, x + 8, y + 8, 128, 128, shadowColor);

		static float hotAndActiveColor[3] = { 0.2f, 0.3f, 0.4f };

		static float hotColor[3] = { 0.3f, 0.2f, 0.6f };

		static float activeColor[3] = { 0.2f, 0.6f, 0.4f };

		if (globalState.hotItem == id)
		{
			if (globalState.activeItem == id)
			{
				//button is both hot and active
				DrawRect(drawList, x + 2, y + 2, 128, 128, hotAndActiveColor);
			}
			else
			{
				//button is merely 'hot'
				DrawRect(drawList, x + 2, y + 2, 128, 128, hotColor);
			}
		}
		else
		{
			//button is not hot, but it may be active
			DrawRect(drawList, x, y, 128, 128, activeColor);
		}

		//button release
		//if button is hot and active, but mouse button is not down, the user must have clicked the button
		if (globalState.mouseDown == 0 && globalState.hotItem == id && globalState.activeItem == id)
			return 1;

		//otherwise, no click
		return 0;
	}

	void ImGuiPrepare(UIState& globalState)
	{
		globalState.hotItem = 0;
	}

	void ImGuiFinish(UIState& globalState)
	{
		if (globalState.mouseDown == 0)
		{
			globalState.activeItem = 0;
		}
		else
		{
			if (globalState.activeItem == 0)
				globalState.activeItem = -1;
		}
	}
}