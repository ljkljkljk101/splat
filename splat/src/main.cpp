

// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "GaussianData.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tools/camera.h>
#include <tools/shader.h>
#include "GaussianSplatRenderable.h"




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings 
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 670;

// camera 
Camera camera(glm::vec3(0, 0.0f, 0));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing  
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float scale = 1.0;
float updateScale = 1.0;
bool pressUp = false;
bool pressDown = false;
bool isPointCloud = false;
bool attenuationMode = true;
int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Splat", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);



	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	CGaussianSplatRenderable g;

	g.loadFile("C:/Users/14948/Desktop/cg/3d_GS/gaussian-splatting-main/gaussian-splatting/output/9b4238f9-c/point_cloud/iteration_30000/point_cloud.ply");
	float t = 0;
	glfwSwapInterval(0);
	bool mode = false;
	int cnt = 0;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		t += deltaTime;
		if (t > 1) {
			std::cout << cnt + 1 << " \n";
			t -= 1;
			cnt = 0;
		}
		else {
			cnt++;
		}

		glm::vec4 viewport(0.0f, 0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT);
		float width = viewport.z;


		float height = viewport.w;
		float aspectRatio = width / height;
		glm::mat4 viewMat = camera.GetViewMatrix();
		glm::vec3 eye = glm::vec3(glm::inverse(camera.GetViewMatrix())[3]);
		glm::vec2 nearFar = glm::vec2(0.1f, 1000.0f);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, nearFar.x, nearFar.y);
		if (mode != isPointCloud) {
			mode = isPointCloud;
			g.changeMode(mode);
		}
		g.setAttenuation(attenuationMode);

		float tan_fovy = tan(glm::radians(camera.Zoom) / 2.0f);
		float tan_fovx = tan_fovy * SCR_WIDTH / SCR_HEIGHT;
		float focaly = 0.5f * SCR_HEIGHT / tan_fovy;
		float focalx = 0.5f * SCR_WIDTH / tan_fovx;
		g.render(deltaTime, { focalx,focaly,tan_fovx,tan_fovy }, projection, viewMat, viewport, nearFar, updateScale);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		scale = 1.0;
		updateScale = 1.0;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		pressUp = true;
		scale += deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && pressUp) {
		pressUp = false;
		updateScale = scale;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		pressDown = true;
		scale -= deltaTime;
		scale = std::max(scale, 0.5f);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && pressDown) {
		pressDown = false;
		updateScale = scale;
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		isPointCloud = true;
	}

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		isPointCloud = false;
	}


	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		attenuationMode = true;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		attenuationMode = true;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		attenuationMode = false;
	}
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset * 2, yoffset * 2);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
