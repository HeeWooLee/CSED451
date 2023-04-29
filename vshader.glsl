#version 460 core

layout(location = 0) in vec3 vPosition;

uniform mat4 model_view;

void main()
{
    gl_Position = model_view * vec4(vPosition, 1.0);
}