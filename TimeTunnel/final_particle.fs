#version 330 core
out vec4 FragColor;
uniform vec4 Color;

in vec2 TexCoords;
uniform sampler2D image;


void main(){
	FragColor = texture(image, TexCoords);
}