#include "PreCompiled.h"
#include "Header/UI/PixelImGui.h"

#include "glad/glad.h"

#include "GLFW/glfw3.h"

namespace PixelImGui
{
	void DrawRect(DrawList& drawList, int32_t x, int32_t y, int32_t w, int32_t h, float color[3], uint32_t textureId)
	{
		float invWidth = 1.0f / (float)drawList.viewportWidth;
		float invHeight = 1.0f / (float)drawList.viewportHeight;
			
		//add a rect to draw list
		drawList.vertices[drawList.totalItem * 16] = (float)(x + w) * invWidth * 2.0f - 1.0f;
		drawList.vertices[drawList.totalItem * 16 + 1] = (1.0f - (float)y * invHeight) * 2.0f - 1.0f;//upper right corner

		drawList.vertices[drawList.totalItem * 16 + 2] = 1.0f;
		drawList.vertices[drawList.totalItem * 16 + 3] = 0.0f;//upper right texture coordinate

		drawList.vertices[drawList.totalItem * 16 + 4] = (float)(x + w) * invWidth * 2.0f - 1.0f;
		drawList.vertices[drawList.totalItem * 16 + 5] = (1.0f - (float)(y + h) * invHeight) * 2.0f - 1.0f;//down right corner

		drawList.vertices[drawList.totalItem * 16 + 6] = 1.0f;
		drawList.vertices[drawList.totalItem * 16 + 7] = 1.0f;//down right texture coordinate

		drawList.vertices[drawList.totalItem * 16 + 8] = (float)x * invWidth * 2.0f - 1.0f;
		drawList.vertices[drawList.totalItem * 16 + 9] = (1.0f - (float)(y + h) * invHeight) * 2.0f - 1.0f;//down left corner

		drawList.vertices[drawList.totalItem * 16 + 10] = 0.0f;
		drawList.vertices[drawList.totalItem * 16 + 11] = 1.0f;//down left texture coordinate

		drawList.vertices[drawList.totalItem * 16 + 12] = (float)x * invWidth * 2.0f - 1.0f;
		drawList.vertices[drawList.totalItem * 16 + 13] = (1.0f - (float)y * invHeight) * 2.0f - 1.0f;//upper left corner	

		drawList.vertices[drawList.totalItem * 16 + 14] = 0.0f;
		drawList.vertices[drawList.totalItem * 16 + 15] = 0.0f;

		drawList.color[drawList.totalItem * 3] = color[0];
		drawList.color[drawList.totalItem * 3 + 1] = color[1];
		drawList.color[drawList.totalItem * 3 + 2] = color[2];

		drawList.textureIds[drawList.totalItem] = textureId;

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
		glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);//8 vertices, four corners

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 6, indices, GL_STATIC_DRAW);

		//------create a white texture------
		glGenTextures(1, &whiteTexture);
		glBindTexture(GL_TEXTURE_2D, whiteTexture);
		static unsigned char whiteColorValue[4] = {255, 255, 255, 255};
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			1,
			1,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			&whiteColorValue
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//------create a white texture------
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
			if (drawList.textureIds[i] != 0)
				glBindTexture(GL_TEXTURE_2D, drawList.textureIds[i]);
			else
				glBindTexture(GL_TEXTURE_2D, drawList.whiteTexture);
			glBindBuffer(GL_ARRAY_BUFFER, drawList.VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(float), &drawList.vertices[i * 16]);
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
			if (globalState.activeItem == 0 && globalState.mouseDown)//able to transition to active item
				globalState.activeItem = id;
		}

		//if no widget has keyboard focus, take it
		if (globalState.kbItem == 0)
			globalState.kbItem = id;
		
		static float color[3] = { 1.0f, 0.0f, 0.0f };

		//if we have keyboard focus, show it
		if (globalState.kbItem == id)
			DrawRect(drawList, x - 6, y - 6, 144, 144, color, 0);

		static float shadowColor[3] = { 0.1f, 0.1f, 0.1f };

		//render shadow
		DrawRect(drawList, x + 8, y + 8, 128, 128, shadowColor, 0);

		static float hotAndActiveColor[3] = { 0.2f, 0.3f, 0.4f };

		static float hotColor[3] = { 0.3f, 0.2f, 0.6f };

		static float activeColor[3] = { 0.2f, 0.6f, 0.4f };

		if (globalState.hotItem == id)
		{
			if (globalState.activeItem == id)
			{
				//button is both hot and active
				DrawRect(drawList, x + 2, y + 2, 128, 128, hotAndActiveColor, 0);
			}
			else
			{
				//button is merely 'hot'
				DrawRect(drawList, x + 2, y + 2, 128, 128, hotColor, 0);
			}
		}
		else
		{
			//button is not hot, but it may be active
			DrawRect(drawList, x, y, 128, 128, activeColor, 0);
		}

		//if we have keyboard focus, we all need to process the keys
		if (globalState.kbItem == id)
		{
			switch (globalState.keyEntered)
			{
			case GLFW_KEY_TAB:
				//if tab is pressed, lose keyboard focus
				//next widget will grab the focus
				globalState.kbItem = 0;
				//if shift was also pressed, we want to move focus
				//to the previous widget instead
				if (globalState.keyMod & GLFW_KEY_LEFT_SHIFT)
					globalState.kbItem = globalState.lastWidget;
				//also clear the key so that next widget
				//won't process it
				globalState.keyEntered = 0;
				break;
			case GLFW_KEY_ENTER:
				//had keyboard focus, received return
				//so we'll act as if we were clicked
				return 1;
			}
		}

		globalState.lastWidget = id;

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
		if (globalState.mouseDown == 0)//mouse release
		{
			globalState.activeItem = 0;//mark it as able to transition to ID
		}
		else//mouse down
		{
			if (globalState.activeItem == 0)
				globalState.activeItem = -1;//mark it as unable to transition to ID
		}

		//if no widget grabbed tab, clear focus
		if (globalState.keyEntered == GLFW_KEY_TAB)
			globalState.kbItem = 0;
		//clear the entered key
		globalState.keyEntered = 0;

		globalState.keyChar = 0;
	}

	int32_t Slider(UIState& globalState, DrawList& drawList, int32_t id, int32_t x, int32_t y, int32_t max, int& value)
	{
		//calculate mouse cursor's relative y offset
		int32_t yPos = ((256 - 16) * value) / max;

		//check for hotness
		if (RegionHit(globalState, x + 8, y + 8, 16, 255))
		{
			globalState.hotItem = id;
			if (globalState.activeItem == 0 && globalState.mouseDown)
				globalState.activeItem = id;
		}

		//if no widget has keyboard focus, take it
		if (globalState.kbItem == 0)
			globalState.kbItem = id;

		static float focusColor[3] = { 1.0f, 0.0f, 0.0f };
		//if we have keyboard focus, shot it
		if (globalState.kbItem == id)
			DrawRect(drawList, x - 4, y - 4, 40, 288, focusColor, 0);
		
		static float bgColor[3] = { 0.3f, 0.6f, 0.2f };

		//draw bg
		DrawRect(drawList, x, y, 32, 256 + 16, bgColor, 0);

		//draw thumb
		static float thumbColor[3] = { 0.2f, 0.3f, 0.1f };
		static float thumbColor2[3] = { 0.4f, 0.6f, 0.2f };
		if (globalState.activeItem == id || globalState.hotItem == id)
		{
			DrawRect(drawList, x + 8, y + 8 + yPos, 16, 16, thumbColor, 0);//thumb changes color if the slider is active or hot
		}
		else
		{
			DrawRect(drawList, x + 8, y + 8 + yPos, 16, 16, thumbColor2, 0);
		}

		if (globalState.kbItem == id)
		{
			switch (globalState.keyEntered)
			{
			case GLFW_KEY_TAB:
				//if tab is pressed, lose keyboard focus
				//next widget will grab the focus
				globalState.kbItem = 0;
				//if shift was also pressed, we want to move focus
				//to the previous widget instead
				if (globalState.keyMod & GLFW_KEY_LEFT_SHIFT)
					globalState.kbItem = globalState.lastWidget;
				//also clear the key so that next widget
				//won't process it
				globalState.keyEntered = 0;
				break;
			case GLFW_KEY_UP:
				//slide slider up(if not at zero)
				if (value > 0)
				{
					value -= 20;
					return 1;
				}
				break;
			case GLFW_KEY_DOWN:
				//slide slider down(if not at max)
				if (value < max)
				{
					value += 20;
					return 1;
				}
				break;
			}
		}

		globalState.lastWidget = id;

		//update widget value
		if (globalState.activeItem == id)
		{
			int32_t mousePos = globalState.mouseY - (y + 8);
			if (mousePos < 0) mousePos = 0;
			if (mousePos > 255) mousePos = 255;//clamp
			int32_t v = (mousePos * max) / 255;
			if (v != value)
			{
				value = v;//change value
				return 1;
			}
		}

		return 0;
	}

	int textField(UIState& globalState, DrawList& drawList, std::map<char, Character>& maps, int32_t id, int32_t x, int32_t y, std::string& buffer)
	{
		//int32_t len = strlen(buffer);
		int32_t changed = 0;

		//check for hotness
		if (RegionHit(globalState, x - 4, y - 4, 30 * 14 + 8, 24 + 8))
		{
			globalState.hotItem = id;
			if (globalState.activeItem == 0 && globalState.mouseDown)
				globalState.activeItem = id;
		}

		//if no widget has keyboard focus, take it
		if (globalState.kbItem == 0)
			globalState.kbItem = id;
		
		static float focusColor[3] = { 0.2f, 0.3f, 0.4f };

		//if we have keyboard focus, show it
		if (globalState.kbItem == id)
			DrawRect(drawList, x - 6, y - 6, 30 * 14 + 12, 24 + 12, focusColor, 0);

		static float bgColor[3] = { 0.7f, 0.2f, 0.3f };
		static float bgColor2[3] = { 0.4f, 0.3f, 0.2f };

		//render the text field
		if (globalState.activeItem == id || globalState.hotItem == id)
		{
			DrawRect(drawList, x - 4, y - 4, 30 * 14 + 8, 24 + 9, bgColor, 0);
		}
		else
		{
			DrawRect(drawList, x - 4, y - 4, 30 * 14 + 8, 24 + 9, bgColor2, 0);
		}

		//drawstring
		RenderText(drawList, maps, buffer, x, y, 1.0f, bgColor);

		//render cursor if we have keyboard focus
		if (globalState.kbItem == id)
		{
			RenderText(drawList, maps, "_", x + buffer.size() * 30.0f, y + 48.0f, 1.0f, bgColor);
		}

		//if we have keyboard focus, we'll need to process the keys
		if (globalState.kbItem == id)
		{
			switch (globalState.keyEntered)
			{
			case GLFW_KEY_TAB:
				//if tab is pressed, lose keyboard focus
				//next widget will grab the focus
				globalState.kbItem = 0;
				//if shift was also pressed, we want to move focus
				//to the previous widget instead
				if (globalState.keyMod & GLFW_KEY_LEFT_SHIFT)
					globalState.kbItem = globalState.lastWidget;

				//also clear the key so that next widget won't process it
				globalState.keyEntered = 0;
				break;
			case GLFW_KEY_BACKSPACE:
				if (buffer.size() > 0)
				{
					buffer.pop_back();
					changed = 1;
				}
				break;
			}
			if (globalState.keyChar >= 32 && globalState.keyChar < 127 && buffer.size() < 30)
			{
				buffer.push_back(globalState.keyChar);
				changed = 1;
			}
		}

		//if button is hot and active, but mouse button is not down, the user must have clicked the widget
		//give it keyboard focus
		if (globalState.mouseDown == 0 && globalState.hotItem == id && globalState.activeItem == id)
			globalState.kbItem = id;

		globalState.lastWidget = id;

		return changed;
	}

	void RenderText(DrawList& drawList, std::map<char, Character>& maps, std::string text, float x, float y, float scale, float color[3])
	{
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); ++c)
		{
			Character ch = maps[*c];

			float xPos = x + ch.bearing[0] * scale;
			float yPos = y - (ch.size[1] - ch.bearing[1]) * scale;

			float w = ch.size[0] * scale;
			float h = ch.size[1] * scale;

			static float textColor[3] = { 0.4f, 0.2f, 0.3f };
			DrawRect(drawList, x, y, w, h, textColor, ch.textureId);

			x += (ch.advance >> 6) * scale;
		}
	}
}