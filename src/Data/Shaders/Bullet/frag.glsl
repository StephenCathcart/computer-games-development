#version 330 core

uniform sampler2D colourTex;

in Vertex {
	vec3 normal; 
	vec2 texCoord;
} IN;

void main() {
    vec4 texColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    gl_FragColor = texColor;
}