#version 330 core

//in vec3 ourColor;
//in vec2 TexCoord;

out vec4 color;

// Texture samplers
//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

uniform vec3 objectColor;
uniform vec3 lightColor;

//uniform float mixVal;

void main()
{
	//color = texture(ourTexture1, TexCoord) * vec4(ourColor, 1.0f);
	//color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2f);
	color = vec4(lightColor * objectColor, 1.0f);
	// Linearly interpolate between both textures (second texture is only slightly combined)
}
