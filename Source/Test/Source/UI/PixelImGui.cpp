#include "PreCompiled.h"
#include "Header/UI/PixelImGui.h"

#include "glad/glad.h"

namespace PixelImGui
{
	void DrawRect(DrawList& drawList, int32_t x, int32_t y, int32_t w, int32_t h)
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
			glBindBuffer(GL_ARRAY_BUFFER, drawList.VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), &drawList.vertices[i * 8]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		drawList.totalItem = 0;
		glBindVertexArray(0);
	}
}