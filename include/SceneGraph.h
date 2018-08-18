#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include <Mesh.h>
#include <Shader.h>

//basic scene graph
class SceneNode
{
public:

	bool isVisible = true;
	bool isLightSource = false;

	SceneNode() = default;

	SceneNode(const std::string& pathToMesh);

	virtual ~SceneNode() = default;

	void translate(const glm::vec3& dir);

	void scale(const glm::vec3& val);

	void rotate(const glm::vec3& axis, const GLfloat val);

	void addChild(const std::shared_ptr<SceneNode>& child);

	virtual void draw(Shader& shader, const glm::mat4& basis = glm::mat4(1.0f)) const;

private:

	Mesh mMesh;
	glm::vec3 mTranslateVector = glm::vec3(0.0f);
	glm::vec3 mScaleVector = glm::vec3(1.0f);
	glm::vec3 mRotationAxis= glm::vec3(0.0f);
	float mRotationAngle = 0.0f;
	std::vector<std::shared_ptr<SceneNode>> mChildren;

	const glm::mat4 getLocalTransformMatrix() const;
};
