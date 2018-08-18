
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 LightPos;
out vec3 CameraPos;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
    TexCoords = aTexCoords;    
	//vec3 Bitangent	= normalize(mat3(transpose(inverse(model))) * aBitangent);
	//an othogonalization with Gramm-Shmidt
	vec3 oTangent = normalize(aTangent - dot(aTangent, aNormal) * aTangent);
	vec3 oBitangent = normalize(cross(aNormal, oTangent));
	TBN			= mat3(model) * transpose(mat3(oTangent, oBitangent, aNormal));
	LightPos	= TBN * lightPos.xyz;
	CameraPos	= TBN * cameraPos.xyz;

	vec4 worldPos = model * vec4(aPos, 1.0);
	gl_Position = projection * view * worldPos;
	FragPos = TBN * vec3(worldPos);
}