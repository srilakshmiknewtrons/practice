#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "geometry.h"
const unsigned int OPENGLWinWIDTH = 1500, OPENGLWinHEIGHT = 1500;
int main()
{
	if (!glfwInit())
	{
		std::cout << " failed to load";
		return -1;

	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(OPENGLWinWIDTH, OPENGLWinHEIGHT, "OPENGL", nullptr, nullptr);
	if (!window)
	{
		std::cout << "failed to create window \n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to init glad";
		return -1;
	}
	glEnable(GL_MULTISAMPLE);
	Geometry task;
	
	glfwSetWindowUserPointer(window, &task);
	task.init(window);
	while (!glfwWindowShouldClose(window))
	{
		task.render();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	task.shutdown();
	glfwTerminate();
	return 0;

}

