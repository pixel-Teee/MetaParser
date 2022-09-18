#version 460 core

uniform vec3 color;

in vec2 tex;
out vec4 FragColor;

uniform sampler2D text;

void main()
{
    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(text, tex).r);

    FragColor = vec4(color, 1.0f) * sampled;
}

