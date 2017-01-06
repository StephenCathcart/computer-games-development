#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec3 normal; 
in vec2 texCoord;

out Vertex {
	vec3 normal; 
	vec2 texCoord;
} OUT;

void main() {
	OUT.normal = normal;
    OUT.texCoord = texCoord;

	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
    gl_Position = mvp * vec4(position, 1.0);
}
