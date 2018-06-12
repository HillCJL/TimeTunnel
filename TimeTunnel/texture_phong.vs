#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNVector;

out vec3 NVector;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   NVector = mat3(transpose(inverse(model))) * aNVector;
   FragPos = vec3(model * vec4(aPos, 1.0));
}