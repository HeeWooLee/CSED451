#version 460 core

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoord;
layout (location=2) in vec3 vertexNormal;
layout (location=2) in vec3 vertexTangent;
layout (location=2) in vec3 vertexBitangent;

out vec2 outTexCoord;
out vec3 mvVertexNormal;
out vec3 mvVertexPos;
out mat3 TBN;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;


void main()
{

    vec4 mvPos = modelViewMatrix * vec4(position, 1.0);
    gl_Position = projectionMatrix * mvPos;
    outTexCoord = texCoord;
    mvVertexNormal = normalize(modelViewMatrix * vec4(vertexNormal, 0.0)).xyz;
    mvVertexPos = mvPos.xyz;

    
    mat3 normalMatrix = mat3(transpose(inverse(modelViewMatrix)));
    vec3 viewTangent = normalize(normalMatrix * vertexTangent);
    vec3 viewBitangent = normalize(normalMatrix * vertexTangent);
    vec3 viewNormal = normalize(normalMatrix * vertexNormal);
    
    TBN = mat3(viewTangent, viewBitangent, viewNormal);
}