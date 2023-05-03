#version 460 core

layout(location = 0) in vec3 vPosition;
layout (location = 1) in vec3 aColor;

out vec3 vColor; 

uniform mat4 model_view;

void main()
{
    gl_Position = model_view * vec4(vPosition, 1.0);
    vColor = aColor;
}