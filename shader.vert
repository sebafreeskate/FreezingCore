#version 330 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

//uniform float xOffset;
uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	ourColor = vec3(position.x, position.y, position.z);
	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
	// We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
	// TexCoord = texCoord;
}
