#version 460 core

in vec3 vColor;
in vec3 vNormal;
in vec2 vTex;
in vec3 fN;
in vec3 fL;
in vec3 fE;
in mat3 TBN;


out vec4 FragColor;

uniform mat4 model_view;
uniform mat4 projection;
uniform float alpha;

uniform vec4 LightPosition; // directional light
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;
uniform int ShadingMode;

uniform sampler2D normalMap;
uniform sampler2D texture;
void main()
{
    if (NormalMappingMode == 1) {
        normal = texture(normalMap, vTex).rgb;
        normal = normal * 2.0 - 1.0;
        normal = normalize(TBN * normal);
    }
    if (ShadingMode != 2) {
        FragColor = vec4(vColor, alpha);
    }
    else { // Phong Shading
        // Normalize the input lighting vectors
        vec3 N = normalize(fN);
        vec3 E = normalize(fE);
        vec3 L = normalize(fL);
        vec3 H = normalize(L + E);
        vec4 ambient = AmbientProduct;
        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd * DiffuseProduct;
        float Ks = pow(max(dot(N, H), 0.0), Shininess);
        vec4 specular = Ks * SpecularProduct;
        // discard the specular highlight if the light's behind the vertex
        if (dot(L, N) < 0.0)
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        FragColor = ambient + diffuse + specular;
        FragColor.a = alpha;
    }

    FragColor.a = alpha;
}
