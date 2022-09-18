#include "Header/Range.h"

#include "Header/ReflectionModule.h"

#include "rttr/variant.h"

#include "Header/Control.h"

#include "Header/UI/PixelImGui.h"

#include "Header/Shader/Shader.h"

#include "Header/UI/Font.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <windows.h>

#include <memory>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouseCallBack(GLFWwindow* window, double xpos, double ypos);

void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);

void keyCallBack(GLFWwindow* window, int key, int scanCode, int action, int mods);

void charCallBack(GLFWwindow* window, unsigned int codePoint);

PixelImGui::UIState globalUIState;

float clearColor[3] = { 0.7f, 0.3f, 0.2f };

void GenerateCharacter(std::map<char, Character>& maps, FT_Face& face);

int main()
{
	//allocate reflection module
	AllocateModule();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "TestReflection", nullptr, nullptr);

	//register frame buffer size callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouseCallBack);

	glfwSetMouseButtonCallback(window, mouseButtonCallBack);

	glfwSetKeyCallback(window, keyCallBack);

	glfwSetCharCallback(window, charCallBack);

	if (window == nullptr)
	{
		std::cout << "failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize glad" << std::endl;
		return -1;
	}

	//-----create vertices-----
	float vertices[] = {
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	
	//-----create vertices-----

	//------create shader------
	Shader ourShader("Resource/shader.vs", "Resource/shader.fs");
	//Shader uiShader("Resource/ui.vs", "Resource/ui.fs");

	std::shared_ptr<Shader> uiShader = std::make_shared<Shader>("Resource/ui.vs", "Resource/ui.fs");
	//------create shader------

	PixelImGui::DrawList globalDrawList;

	int32_t triangleColor = 0;

	globalUIState = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	//------load text------
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, "Resource/COOPBL.TTF", 0, &face))
		std::cout << "ERROR:FREETYPE: Failed to load font" << std::endl;

	//define font size, height is 48, then width will be calculated from height 
	FT_Set_Pixel_Sizes(face, 0, 48);

	std::map<char, Character> Characters;
	GenerateCharacter(Characters, face);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	//------load text------

	std::string test;

	while (!glfwWindowShouldClose(window))
	{

		processInput(window);//process input

		//DrawRect(globalDrawList, globalUIState.mouseX, globalUIState.mouseY, 128, 128);

		PixelImGui::ImGuiPrepare(globalUIState);

		PixelImGui::Button(globalUIState, globalDrawList, GEN_ID, 50, 50);

		PixelImGui::Button(globalUIState, globalDrawList, GEN_ID, 200, 50);

		if (PixelImGui::Button(globalUIState, globalDrawList, GEN_ID, 50, 300))
		{
			clearColor[0] = 0.6f;
			clearColor[1] = 0.4f;
			clearColor[2] = 0.8f;
		}

		PixelImGui::Slider(globalUIState, globalDrawList, GEN_ID, 600, 50, 255, triangleColor);

		PixelImGui::textField(globalUIState, globalDrawList, Characters, GEN_ID, 600, 400, test);

		PixelImGui::ImGuiFinish(globalUIState);

		glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		TotalDraw(globalDrawList, uiShader);

		glBindVertexArray(VAO);
		ourShader.use();
		ourShader.setFloat("color", triangleColor / 255.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);//swap buffers
		glfwPollEvents();//poll events

		//Sleep(50);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	//globalDrawList.viewportWidth = width;
	//globalDrawList.viewportHeight = height;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouseCallBack(GLFWwindow* window, double xpos, double ypos)
{
	//update mouse position
	globalUIState.mouseX = (int32_t)xpos;
	globalUIState.mouseY = (int32_t)ypos;
	//std::cout << xpos << " " << ypos << std::endl;
}

void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
{
	//when left click, update the ui state
	if (button == GLFW_MOUSE_BUTTON_1)
	{
		if (action == GLFW_PRESS)
			globalUIState.mouseDown = 1;
		else if (action == GLFW_RELEASE)
			globalUIState.mouseDown = 0;
	}
}

void keyCallBack(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		//std::wcout << key << std::endl;
		//std::wcout << mods << std::endl;
		globalUIState.keyEntered = key;
		globalUIState.keyMod = mods;
	}	
}

void charCallBack(GLFWwindow* window, unsigned int codePoint)
{
	globalUIState.keyChar = codePoint;
	std::cout << codePoint << std::endl;
}

void GenerateCharacter(std::map<char, Character>& maps, FT_Face& face)
{
	//generate character
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (int32_t c = 0; c < 128; ++c)
	{
		//load 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}

		//generate texture
		uint32_t texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character;
		character.textureId = texture;
		character.size[0] = face->glyph->bitmap.width;
		character.size[1] = face->glyph->bitmap.rows;
		character.bearing[0] = face->glyph->bitmap_left;
		character.bearing[1] = face->glyph->bitmap_top;
		character.advance = face->glyph->advance.x;
		maps.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}