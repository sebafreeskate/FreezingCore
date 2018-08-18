
#define STB_IMAGE_IMPLEMENTATION

#include "mesh.h"

#include <stb_image.h>

const std::vector<aiTextureType>& Mesh::getSupportedTextureTypes() const
{
	static std::vector<aiTextureType> suppTypes{aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_HEIGHT};

	return suppTypes;
}

Mesh::Mesh(std::string const & filename) : Mesh()
{
	// Load a Model from File
	Assimp::Importer loader;
	aiScene const * scene = loader.ReadFile(
		filename,
		aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_OptimizeGraph |
		aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace);

	// Walk the Tree of Scene Nodes
	auto index = filename.find_last_of("/");
	if (!scene) fprintf(stderr, "%s\n", loader.GetErrorString());
	else parse(filename.substr(0, index), scene->mRootNode, scene);
}

Mesh::Mesh(std::vector<Vertex> const & vertices,
	std::vector<GLuint> const & indices,
	Material&& material)
	: mIndices(indices)
	, mVertices(vertices)
	, mMaterial(material)
{

	// Bind a Vertex Array Object
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// Copy Vertex Buffer Data
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		mVertices.size() * sizeof(Vertex),
		&mVertices.front(), GL_DYNAMIC_DRAW);

	// Copy Index Buffer Data
	glGenBuffers(1, &mElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		mIndices.size() * sizeof(GLuint),
		&mIndices.front(), GL_DYNAMIC_DRAW);

	// Set Shader Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, normal));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, tangent));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, bitangent));
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, uv));
	glEnableVertexAttribArray(0); // Vertex Positions
	glEnableVertexAttribArray(1); // Vertex Normals
	glEnableVertexAttribArray(2); // Vertex Tangents
	glEnableVertexAttribArray(3); // Vertex Bitangents
	glEnableVertexAttribArray(4); // Vertex UVs

								  // Cleanup Buffers
	glBindVertexArray(0);
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mElementBuffer);
}

void Mesh::draw(Shader& shader) const
{
	GLuint textureUnit = 0;
	std::map<aiTextureType, unsigned> counts; //there could be more than one texture of any type, should incremet indexes properly;
	for (auto type : getSupportedTextureTypes()) {
		counts.insert({ type,	0 });
	}
	for (const TextureMap& texMap : mMaterial.texMaps)
	{   // Set Correct Uniform Names Using Texture Type (Omit ID for 0th Texture)
		std::string uniformName = "material.texture_" + texMap.name
			+ std::to_string(counts[texMap.type]++);

		// Bind Correct Textures and Vertex Array Before Drawing
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, texMap.id);
		shader.bind(uniformName, textureUnit++); //bind uniform to texture Unit in VRAM, not to id of texture itself. Was not obvious for me

	}   
	shader.bind("material.shininess", mMaterial.shininess);
	shader.bind("material.Ka", mMaterial.Ka);
	shader.bind("material.Kd", mMaterial.Kd);
	shader.bind("material.Ks", mMaterial.Ks);
	glBindVertexArray(mVertexArray);

	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);

	for (auto &i : mSubMeshes) i->draw(shader);
}

void Mesh::parse(std::string const & path, aiNode const * node, aiScene const * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
		parse(path, scene->mMeshes[node->mMeshes[i]], scene);
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		parse(path, node->mChildren[i], scene);
}

void Mesh::parse(std::string const & path, aiMesh const * mesh, aiScene const * scene)
{
	// Create Vertex Data from Mesh Node
	std::vector<Vertex> vertices; Vertex vertex;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		if (mesh->mTextureCoords[0]) {
			vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		vertex.position		= glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal		= glm::normalize(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
		vertex.tangent		= glm::normalize(glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z));
		vertex.bitangent	= glm::normalize(glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z));

		if (glm::dot(glm::cross(vertex.normal, vertex.tangent), vertex.bitangent) < 0.0f) { //for models with mirrored normal textures
			vertex.tangent = vertex.tangent * -1.0f;
		}

		vertices.push_back(vertex);
	}

	// Create Mesh Indices for Indexed Drawing
	std::vector<GLuint> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			indices.push_back(mesh->mFaces[i].mIndices[j]);

	// Create New Mesh Node
	mSubMeshes.push_back(std::unique_ptr<Mesh>(new Mesh(vertices, indices, 
		loadMaterial(path, scene->mMaterials[mesh->mMaterialIndex]))));
}

	Material Mesh::loadMaterial(std::string const & path,
	aiMaterial * material)
{
		Material newMaterial;
		
		aiColor3D Ka, Kd, Ks;
		material->Get(AI_MATKEY_SHININESS, newMaterial.shininess);
		material->Get(AI_MATKEY_COLOR_AMBIENT, Ka);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, Kd);
		material->Get(AI_MATKEY_COLOR_SPECULAR, Ks);
		newMaterial.Ka = glm::vec3(Ka.r, Ka.g, Ka.b);
		newMaterial.Kd = glm::vec3(Kd.r, Kd.g, Kd.b);
		newMaterial.Ks = glm::vec3(Ks.r, Ks.g, Ks.b);

		for (auto type : getSupportedTextureTypes()) {
			for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
			{
				// Load Mesh Textures into VRAM
				GLenum format;
				GLuint textureId;
				std::string typeString;

				// Load the Texture Image from File
				aiString str;
				material->GetTexture(type, i, &str);
				std::string filename = str.C_Str();
				filename = path + "/" + filename;
				int width, height, channels;
				unsigned char * image;

				/*if (type == aiTextureType_HEIGHT) 
				{
					stbi_set_flip_vertically_on_load(true);
					image = stbi_load(filename.c_str(), &width, &height, &channels, 0);
					stbi_set_flip_vertically_on_load(false);
				}
				else 
				{ */
					 image = stbi_load(filename.c_str(), &width, &height, &channels, 0);
				//} 

				if (!image) {
					fprintf(stderr, "%s %s\n", "Failed to Load Texture", filename.c_str());
				}

				// Set the Correct Channel Format
				switch (channels)
				{
				case 1: format = GL_ALPHA;     break;
					//case 2: format = GL_LUMINANCE; break;
				case 3: format = GL_RGB;       break;
				case 4: format = GL_RGBA;      break;
				}

				// Bind Texture and Set Filtering Levels
				glGenTextures(1, &textureId);
				glBindTexture(GL_TEXTURE_2D, textureId);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, format,
					width, height, 0, format, GL_UNSIGNED_BYTE, image);
				glGenerateMipmap(GL_TEXTURE_2D);

				// Release Image Pointer and Store the Texture
				stbi_image_free(image);
				if (type == aiTextureType_DIFFUSE)  typeString = "diffuse";
				else if (type == aiTextureType_SPECULAR) typeString = "specular";
				else if (type == aiTextureType_HEIGHT) typeString = "normals";
				newMaterial.texMaps.push_back(TextureMap{ textureId, type, typeString });
			}
		}
		return newMaterial;
}