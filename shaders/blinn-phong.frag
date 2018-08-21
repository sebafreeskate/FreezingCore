
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 LightPos;
in vec3 CameraPos;
in mat3 TBN;

struct Material {
	sampler2D texture_diffuse0;
    sampler2D texture_specular0;
    sampler2D texture_normals0;
    vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float shininess;
};

uniform Material material;

uniform vec4 environmentColor;
uniform vec4 fullEmissiveColor;

vec3 lightColor = vec3(1.0, 0.8, 0.0);

void blendAmbient(in vec4 ambColor, in out vec3 fragmentColor)
{
	float rk = max(0.5-fragmentColor.r, 0);
	float gk = max(0.5-fragmentColor.g, 0);
	float bk = max(0.5-fragmentColor.b, 0);
	vec3 ambientMask = vec3(ambColor.r * rk, ambColor.g * gk, ambColor.b * bk);
	fragmentColor += ambientMask;	
} 

void main()
{   
	vec3 norm = normalize(texture(material.texture_normals0, TexCoords).rgb * 2.0 - 1.0);

	vec3 ambientColor = vec3(texture(material.texture_diffuse0, TexCoords)) * 0.2;
	blendAmbient(environmentColor, ambientColor);

	vec3 lightDir		= normalize( LightPos - FragPos);
	float diff			= max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor	= texture(material.texture_diffuse0, TexCoords).rgb * material.Kd * diff; 

	blendAmbient(environmentColor, diffuseColor);

	//specular 
	vec3 cameraDir	= normalize(CameraPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + cameraDir);
	float spec		= pow(max(dot(norm , halfwayDir), 0.0), material.shininess);
	vec3 specularColor = texture(material.texture_specular0, TexCoords).rgb * material.Ks * spec;

	FragColor = vec4( ambientColor + diffuseColor + specularColor , 1.0) + fullEmissiveColor;
}