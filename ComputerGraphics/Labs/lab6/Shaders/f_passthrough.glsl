// Fragment shader: : Passthrough shading
// ================
#version 320 es
precision mediump float;
// Ouput data
out vec4 FragColor;

uniform vec4 Color;

void main()
{
   FragColor = Color;
}
