// Vertex shader: Texture Phong shading
// ================
#version 320 es
precision mediump float;
// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 TexCoord;

// Output data ; will be interpolated for each fragment.
out vec2 _TexCoord;
out vec3 E;
out vec3 N;
out vec3 L;

struct PointLight{
	vec3 position;
	vec3 color;
	float power;
};
uniform PointLight light;

// Values that stay constant for the whole mesh.
uniform mat4 P;
uniform mat4 V;
uniform mat4 M; // position*rotation*scaling


void main()
{
	gl_Position = P * V * M * vec4(aPos, 1.0);

	// Position in VCS
	vec4 eyePosition = V * M * vec4(aPos, 1.0);
	// LightPos in VCS
	vec4 eyeLightPos = V * vec4(light.position, 1.0);
	// Compute vectors H,L,N in VCS
	E = normalize(-eyePosition.xyz);
	L = normalize((eyeLightPos - eyePosition).xyz);
	N = normalize(transpose(inverse(mat3(V * M))) * aNormal);

	// we pass the texture coordinate to the fragment shader
	_TexCoord = TexCoord;
}