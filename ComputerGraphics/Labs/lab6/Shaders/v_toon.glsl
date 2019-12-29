// Vertex shader: Gouraud shading
// ================
#version 320 es
precision mediump float;
// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 TexCoord;


out vec3 LightingColor; // resulting color from lighting calculations




// Values that stay constant for the whole mesh.
uniform mat4 P;
uniform mat4 V;
uniform mat4 M; // position*rotation*scaling

struct PointLight{
	vec3 position;
	vec3 color;
	float power;
 };
uniform PointLight light;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

void main()
{
    gl_Position = P * V * M * vec4(aPos, 1.0);

    // Position in VCS
	vec4 eyePosition = V * M * vec4(aPos, 1.0);
	// LightPos in VCS
	vec4 eyeLightPos = V * vec4(light.position, 1.0);

	// Compute vectors E,L,N in VCS
	vec3 E = -eyePosition.xyz;
	vec3 L = (eyeLightPos - eyePosition).xyz;
    vec3 N = transpose(inverse(mat3(V * M))) * aNormal;


    // diffuse
    vec3 norm = normalize(N);
    vec3 lightDir = normalize(L);

    vec3 color1 = material.diffuse;
    vec3 color2;

    float intensity = dot(lightDir,norm);

    if (intensity > 0.80)      color2 = vec3(1.0, 1.0, 1.0);
    else if (intensity > 0.60) color2 = vec3(0.6, 0.6, 0.6);
    else if(intensity > 0.30)    color2 = vec3(0.4, 0.4, 0.4);
    else                       color2 = vec3(0.1, 0.1, 0.1);
    LightingColor = color1 * color2;


}