#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Geometry
{
public:
	Geometry() = default;
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 uvcoords;
	};
	GLuint VAO, VBO;
	GLuint quadVAO, quadVBO, quadEBO;
	GLuint textVAO, textVBO, textEBO;
	GLuint ShaderProgramGeometry;
	GLuint Texture;
	//uniform locations
	GLuint textureLOC, modelLOC,viewLOC,projectionLOC, isUseMatrixLOC;
	//matrix
	glm::mat4 model, view, projection;
	GLuint isUseTextureLOC;
	GLuint cubeVAO,cubeVBO, cubeEBO;

	void init(GLFWwindow* window);
	void setupTriangle();
	void setupQuad();
	void setupCube();
	void setupTexture();
    void setupshaders();
	GLuint LoadTexture(const char* path);
	GLuint createProgram(const std::string& vertexFile, const std::string& fragmentFile);
	GLuint compileShader(GLenum Type,const std::string & FileName);
	void checkCompileErrors(GLuint shader, std::string type);
	std::string readShaderFile(const char* path);
	void render();
	void shutdown();
	



};
