#version 320 es
precision mediump float;
in vec2 _TexCoord;
// Ouput data
out vec3 fragColor;

// Values that stay constant for the whole mesh.
uniform sampler2D textureBuffer;

void main(){

	fragColor = texture(textureBuffer,_TexCoord).xyz;
}
