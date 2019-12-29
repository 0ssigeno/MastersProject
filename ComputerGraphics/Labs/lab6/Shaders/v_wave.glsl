#version 320 es
precision mediump float;


// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 TexCoord;

// Output data ; will be interpolated for each fragment.
out vec2 _TexCoord;

// Values that stay constant for the whole mesh.
uniform mat4 P;
uniform mat4 V;
uniform mat4 M; // position*rotation*scaling
uniform float T;

//uniform vec4 Color;
in vec4 Color;
void main()
{
	float a=0.1;
	float omega=0.01;
	vec3 newPos=aPos;
	newPos.y=a * sin(omega*T + 10.*aPos.x )* sin(omega*T + 10.*aPos.z);

	gl_Position = P * V * M * vec4(newPos, 1.0);
	_TexCoord = TexCoord;
}
