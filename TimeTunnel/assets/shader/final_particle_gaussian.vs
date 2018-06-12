#version 330 core
layout (location = 0) in vec3 aPos;
out vec2 TexCoords;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main()
{
    vec4 Frag = projection * view * model * vec4(aPos, 1.0f);
    gl_Position = Frag;
	TexCoords = Frag.xy/Frag.w * 0.5 + 0.5;
}