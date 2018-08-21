
#include "SceneGraph.h"

	bool isVisible = true;
	bool isLightSource = false;

	SceneNode::SceneNode(const std::string& pathToMesh) : mMesh(pathToMesh){}

	void SceneNode::translate(const glm::vec3& dir)
	{
		mTranslateVector = dir;
	}

	void SceneNode::scale(const glm::vec3& val)
	{
		mScaleVector = val;
	}

	void SceneNode::rotate(const glm::vec3& axis, const GLfloat val)
	{
		mRotationAxis = axis;
		mRotationAngle = val;
	}

	void SceneNode::addChild(const std::shared_ptr<SceneNode>& child)
	{
		mChildren.push_back(child);
	}

	void SceneNode::draw(Shader& shader, const glm::mat4& basis /* = glm::mat4(1.0f) */) const 
	{
		if (!isVisible)
		{
			return;
		}

		const glm::mat4 modelMatrix = getLocalTransformMatrix() * basis;
		shader.bind("model", modelMatrix);

		if (isLightSource)
		{
			glm::vec3 lightPos(glm::vec3(glm::vec4(mTranslateVector, 1.0f) * basis));
			shader.bind("lightPos", mTranslateVector);
			shader.bind("fullEmissiveColor", glm::vec4(1.0f));
		}
		else
		{
			shader.bind("fullEmissiveColor", glm::vec4(0.0f));
		}

		mMesh.draw(shader);

		for (const std::shared_ptr<SceneNode>& child : mChildren)
		{
			child->draw(shader, modelMatrix);
		}
	}

	const glm::mat4 SceneNode::getLocalTransformMatrix() const
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, mTranslateVector);
		matrix = glm::rotate(matrix, mRotationAngle, mScaleVector);
		matrix = glm::scale(matrix, mScaleVector);

		return matrix; //NRVO
	}
