#include "geometry.h"
#include <fstream>
#include <sstream>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint Geometry::LoadTexture(const char* path)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLint width, height, channels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	if (data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GLenum formate = (channels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, formate, width, height, 0, formate, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load the texture " << path << "\n";
	}
	stbi_image_free(data);
	return texture;
	
}

void Geometry::init(GLFWwindow* window)
{
	setupshaders();
	textureLOC = glGetUniformLocation(ShaderProgramGeometry, "sampler");
	isUseTextureLOC  = glGetUniformLocation(ShaderProgramGeometry, "isUseTexture");
	modelLOC = glGetUniformLocation(ShaderProgramGeometry, "model");
	viewLOC = glGetUniformLocation(ShaderProgramGeometry, "view");
	projectionLOC = glGetUniformLocation(ShaderProgramGeometry, "projection");
	isUseMatrixLOC = glGetUniformLocation(ShaderProgramGeometry,"isUseMatrix");
	model = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0, -3.0));
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	projection = glm::perspective(glm::radians(45.0f), (float)w / h, 0.1f, 100.f);
	setupTriangle();
	setupQuad();
	setupCube();
	setupTexture();
	Texture = LoadTexture("flower.jpg");
}

void Geometry::render()
{
	glClearColor(0.188, 0.686, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(ShaderProgramGeometry);

	glUniform1i(isUseTextureLOC, false);
	glUniform1i(isUseMatrixLOC,false);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(quadVAO);
	glUniform1i(isUseTextureLOC, false);
	glUniform1i(isUseMatrixLOC, false);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(cubeVAO);
	glUniform1i(isUseTextureLOC, false);
	glUniform1i(isUseMatrixLOC, true);
	glUniformMatrix4fv(projectionLOC,1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelLOC, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLOC, 1, GL_FALSE, glm::value_ptr(view));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); 

	glBindVertexArray(textVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(textureLOC, 0);
	glUniform1i(isUseTextureLOC, true);
	glUniform1i(isUseMatrixLOC, false);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Geometry::setupTriangle()
{
	Vertex vertices[] = {
		{{-0.8f,-0.8f,0.0f},{ 0.741, 0.333, 0.475},{0,0}},
		{{-0.2f,-0.8f,0.0f},{ 0.741, 0.333, 0.475},{ 0,0 }},
		{{-0.5,-0.2f,0.0f},{ 0.741, 0.333, 0.475},{0,0}},
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Geometry::setupQuad()
{
	Vertex vertices[] = {
		{{0.2f,0.2f,0.0f},{ 0.333, 0.518, 0.404},{0,0}},
		{{0.8f,0.2f,0.0f},{ 0.333, 0.518, 0.404},{1,0}},
		{{0.8f,0.8f,0.0f},{0.333, 0.518, 0.404},{ 1,1 }},
		{{0.2f,0.8f,0.0f},{ 0.333, 0.518, 0.404},{0,1}},
	};
	GLuint indices[] = { 0,1,2,0,2,3 };

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &quadEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Geometry::setupCube()
{
	Vertex vertices[] = {
		{{0.2f,-0.2f,0.0f},{ 1, 0.961, 0.541},{0,0}},
		{{0.8f,-0.2f,0.0f},{ 1, 0.961, 0.541},{1,0}},
		{{0.8f,-0.8f,0.0f},{1, 0.961, 0.541},{ 1,1 }},
		{{0.2f,-0.8f,0.0f},{ 1, 0.961, 0.541},{0,1}},
		//backface - z = 0.6
		{{0.2f,-0.2f,0.6f},{ 1, 0.961, 0.541},{0,0}},
		{{0.8f,-0.2f,0.6f},{ 1, 0.961, 0.541},{1,0}},
		{{0.8f,-0.8f,0.6f},{1, 0.961, 0.541},{ 1,1 }},
		{{0.2f,-0.8f,0.6f},{ 1, 0.961, 0.541},{0,1}},
	};
		GLuint indices[] = {
			// Front
			0, 1, 2,
			2, 3, 0,

			// Back
			4, 7, 6,
			6, 5, 4,

			// Top
			0, 4, 5,
			5, 1, 0,

			// Bottom
			3, 2, 6,
			6, 7, 3,

			// Left
			0, 3, 7,
			7, 4, 0,

			// Right
			1, 5, 6,
			6, 2, 1
		};


	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &cubeEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Geometry::setupTexture()
{
	Vertex vertices[] = {
		{{-0.2f,0.2f,0.0f},{ 0.333, 0.518, 0.404},{0,0}},
		{{-0.8f,0.2f,0.0f},{ 0.333, 0.518, 0.404},{1,0}},
		{{-0.8f,0.8f,0.0f},{0.333, 0.518, 0.404},{1,1}},
		{{-0.2f,0.8f,0.0f},{ 0.333, 0.518, 0.404},{0,1}},
	};
	GLuint indices[] = { 0,1,2,0,2,3 };

	glGenVertexArrays(1, &textVAO);
	glBindVertexArray(textVAO);
	glGenBuffers(1, &textVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &textEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uvcoords));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

std::string Geometry::readShaderFile(const char* path)
{
	std::ifstream file(path);
	std::stringstream buffer;
	if (!file.is_open())
	{
		std::cout << "failed to open file: " << std::endl;
		return "";
	}
	buffer << file.rdbuf();
	return buffer.str();

}

void Geometry::checkCompileErrors(GLuint shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR TYPE : " << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR : " << type << "\n" << infoLog << std::endl;
		}
	}
}

GLuint Geometry::compileShader(GLenum Type, const std::string & FileName)
{
	const std::string sourceCode = readShaderFile(FileName.c_str());
	const char* source = sourceCode.c_str();
	GLuint shader = glCreateShader(Type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	checkCompileErrors(shader, Type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");

	return shader;
}

GLuint Geometry::createProgram(const std::string& vertexFile, const std::string& fragmentFile)
{
	GLuint VertexShader = compileShader(GL_VERTEX_SHADER, vertexFile);
	GLuint FragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentFile);
	GLuint program = glCreateProgram();
	glAttachShader(program, VertexShader);
	glAttachShader(program, FragmentShader);
	glLinkProgram(program);
	checkCompileErrors(program, "PROGRAM");
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
	return program;
}

void Geometry::setupshaders()
{
	ShaderProgramGeometry = createProgram("shaders/Vertex.vert", "shaders/Fragment.frag");
}

void Geometry::shutdown()
{
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &quadEBO);
	glDeleteBuffers(1, &textVAO);
	glDeleteBuffers(1, &textVBO);
	glDeleteBuffers(1, &textEBO);
	glDeleteBuffers(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &cubeEBO);
	glDeleteProgram(ShaderProgramGeometry);
}
