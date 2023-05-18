#version 460 core

struct Material
{
    vec3 colour;
    int useColour;
};

struct PointLight
{
	vec3 colour;
	// Light position is assumed to be in view coordinates
	vec3 position;
	float intensity;
};

struct DirectionalLight
{
	vec3 colour;
	vec3 direction;
	float intensity;
};
	
in vec4 color; 
in vec2 outTexCoord;
in vec3 mvVertexNormal;
in vec3 mvVertexPos;

out vec4 fragColor;

uniform sampler2D texture_sampler;
uniform sampler2D norm_sampler;
uniform float alpha;
uniform vec3 ambientLight;
uniform float specularPower;
uniform Material material;
uniform PointLight pointLight;
uniform DirectionalLight directionalLight;

vec4 calcLightColour(vec3 light_colour, float light_intensity, vec3 position, vec3 to_light_dir, vec3 normal)
{
	vec4 diffuseColour = vec4(0, 0, 0, 0);
	vec4 specColour = vec4(0, 0, 0, 0);

	// Diffuse Light
	float diffuseFactor = max(dot(normal, to_light_dir), 0.0);
	diffuseColour = vec4(light_colour, 1.0) * light_intensity * diffuseFactor;

	// Specular Light
	vec3 camera_direction = normalize(-position);
	vec3 from_light_dir = -to_light_dir;
	vec3 reflected_light = normalize(reflect(from_light_dir , normal));
	float specularFactor = max( dot(camera_direction, reflected_light), 0.0);
	specularFactor = pow(specularFactor, specularPower);
	specColour = light_intensity  * specularFactor  * vec4(light_colour, 1.0);

	return (diffuseColour + specColour);
}

vec4 calcPointLight(PointLight light, vec3 position, vec3 normal)
{
	vec3 light_direction = light.position - position;
	vec3 to_light_dir  = normalize(light_direction);
	vec4 light_colour = calcLightColour(light.colour, light.intensity, position, to_light_dir, normal);

	return light_colour ;
}

vec4 calcDirectionalLight(DirectionalLight light, vec3 position, vec3 normal)
{
	return calcLightColour(light.colour, light.intensity, position, normalize(light.direction), normal);
}


void main()
{
	
    vec4 baseColour; 
	// Texture Mapping
    if ( material.useColour == 1 )
    {
        baseColour = vec4(material.colour, 1);
    }
    else // if use texture mapping 
    {
        baseColour = texture(texture_sampler, outTexCoord);
    }
	// Texture Mapping

	// Gourad Shading
	fragColor = color * baseColour;
	// Gourad Shading

	// Phong Shading
	vec4 totalLight = vec4(ambientLight, 1.0);
	totalLight += calcDirectionalLight(directionalLight, mvVertexPos, mvVertexNormal);
	totalLight += calcPointLight(pointLight, mvVertexPos, mvVertexNormal);
	fragColor = baseColour * totalLight; // base color put in 
	// Phong Shading



	fragColor.a = alpha;
}