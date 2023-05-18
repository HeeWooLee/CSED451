#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aNormal;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBitangent;

out vec3 vColor;
out vec3 vNormal; 
out vec2 vTex;
out mat3 TBN;
out vec3 fN;
out vec3 fE;
out vec3 fL;


uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 LightPosition; // directional light
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;
uniform int ShadingMode;


uniform int NormalMappingMode;

uniform sampler2D normalMap;
void main()
{
    vec3 viewNormal;
    gl_Position = projection * model_view * vec4(vPosition, 1.0);
    if (NormalMappingMode == 1) {
        vec3 FragNormal = normalize((model_view * vec4(aNormal, 0.0)).xyz);
        vec3 FragTangent = normalize((model_view * vec4(aTangent, 0.0)).xyz);
        vec3 FragBitangent = normalize((model_view * vec4(aBitangent, 0.0)).xyz);
        mat3 TBN = mat3(FragTangent, FragBitangent, FragNormal);
    }

    // Normal Mapping

    // Gourad shading 
    vec3 pos = (model_view * vec4(vPosition, 1.0)).xyz;
    vec3 L = normalize(LightPosition.xyz - pos);
    vec3 E = normalize(-pos);
    vec3 H = normalize(L + E);
    vec3 N = normalize(model_view * vec4(aNormal, 0.0)).xyz;
    if (NormalMappingMode ==1 ) {
        N = viewNormal;
    }
    vec4 ambient = AmbientProduct;
    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd * DiffuseProduct;
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks * SpecularProduct;
    if (dot(L, N) < 0.0) specular = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 color = ambient + diffuse + specular;
    // Gourad shading 

    if (ShadingMode == 0) {
        vColor = aColor;
    }
    else if (ShadingMode == 1) { // Gourad Shading 
        vColor = color.rgb;
    }

    // Phong Shading
    fN = aNormal;
    if (NormalMappingMode == 1) {
        N = viewNormal;
    }
    fE = -vPosition.xyz;
    fL = LightPosition.xyz;
    if (LightPosition.w != 0.0) {
        fL = LightPosition.xyz - vPosition.xyz;
    }

    vNormal = aNormal;
    if (NormalMappingMode ==1) {
        vNormal = viewNormal;
    }
}