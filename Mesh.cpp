#include "Mesh.h"

    Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
		: vertices(vertices), indices(indices), textures(textures) //its ok by std
    {
		setupMesh();
    }
