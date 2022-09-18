#version 460 core

layout(location = 0) in vec4 aPosTex;

out vec2 tex;

void main()
{
	tex = aPosTex.zw;
	gl_Position = vec4(aPosTex.xy, 0.0f, 1.0f);
}