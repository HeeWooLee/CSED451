#version 460 core
in vec3 vColor;
out vec4 FragColor;

uniform float alpha;
void main()
{
    FragColor = vec4(vColor, alpha);
}
