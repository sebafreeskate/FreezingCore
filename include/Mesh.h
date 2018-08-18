
#ifndef MESH_H
#	define MESH_H

#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <map>
#include <memory>
#include <vector>

#include <Shader.h>

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::vec2 uv;
	};

	struct TextureMap {
		GLuint id;
		aiTextureType type;
		std::string name;
	};

	struct Material {
		GLfloat shininess = 0.0f;
		glm::vec3 Ka = glm::vec3(1.0f);
		glm::vec3 Kd = glm::vec3(1.0f);
		glm::vec3 Ks = glm::vec3(1.0f);
		std::vector<TextureMap> texMaps;
	};

	class Mesh
	{
	public:


		Mesh() { glGenVertexArrays(1, &mVertexArray); }
		~Mesh() { glDeleteVertexArrays(1, &mVertexArray); }

		Mesh(std::string const & filename);
		Mesh(std::vector<Vertex> const & vertices,
			std::vector<GLuint> const & indices,
			Material&& material);

		void draw(Shader& shader) const;

		const std::vector<aiTextureType>& getSupportedTextureTypes() const;

	private:

		Mesh(Mesh const &) = delete;
		Mesh & operator=(Mesh const &) = delete;

		void parse(std::string const & path, aiNode const * node, aiScene const * scene);
		void parse(std::string const & path, aiMesh const * mesh, aiScene const * scene);
		Material loadMaterial(std::string const & path,
			aiMaterial * material);

		std::vector<std::unique_ptr<Mesh>> mSubMeshes;
		std::vector<GLuint> mIndices;
		std::vector<Vertex> mVertices;
		Material mMaterial;

		GLuint mVertexArray;
		GLuint mVertexBuffer;
		GLuint mElementBuffer;

	};

#endif