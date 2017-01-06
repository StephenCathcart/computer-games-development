#version 330 core

uniform sampler2D colourTex;

in Vertex {
	vec3 normal; 
	vec2 texCoord;
} IN;

void main() {
    vec4 texColor = texture(colourTex, IN.texCoord);
    gl_FragColor = texColor;
}