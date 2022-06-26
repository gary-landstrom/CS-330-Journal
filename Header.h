#include <iostream>   
#include <cstdlib>         
#include <GL/glew.h>       
#include <GLFW/glfw3.h>  
#include <chrono>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>      

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

namespace
{
	const char* const winTitle = "Gary Landstrom";

	//window size is set
	const int winWidth = 800;
	const int winHeight = 600;

	//camera variables declared global
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	//mouse movements for camera
	bool firstMouse = true;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0f / 2.0;
	float fov = 45.0f;

	//movement speed adjustment
	float speed = 0.3f;

	//controls camera angle in window
	int cameraAngle = 0;

	//establish delta timing so it is the same on all machines
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	//garage position and scale
	glm::mat4 gScale = glm::scale(glm::vec3(0.95f, 0.60f, 0.5f));
	glm::vec3 gPosition(-5.0f, -0.42f, -4.5f);

	//garage roof position and scale
	glm::mat4 gScaleRoofSide = glm::scale(glm::vec3(0.8f, 0.61f, 0.5f));
	glm::vec3 gPositionRoofSide(-5.09f, -0.44f, -4.5f);

	glm::mat4 gScaleRoof = glm::scale(glm::vec3(0.8f, 0.61f, 0.5f));
	glm::vec3 gPositionRoof(-5.09f, -0.44f, -4.5f);


	/*cylinder scale and positions*/ 
	glm::mat4 gScaleCylinder = glm::scale(glm::vec3(0.04f, 0.9f, 0.04f));
	glm::vec3 gPositionCylinder(-2.2f, 1.5f, -1.74f);

	glm::mat4 gScaleCylinderB = glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
	glm::vec3 gPositionCylinderB(-2.2f, 1.5f, -1.71f);
	

	/*pyramid scale and positions */ 
	glm::mat4 gScalePyramid = glm::scale(glm::vec3(0.15f, 0.1f, 0.15f));
	glm::vec3 gPositionPyramid(-2.2f, 1.55f, -1.76f);

	glm::mat4 gScalePyramidB = glm::scale(glm::vec3(0.2f, 0.1f, 0.2f));
	glm::vec3 gPositionPyramidB(-2.2f, 1.5f, -1.76f);

	glm::mat4 gScalePyramidC = glm::scale(glm::vec3(0.45f, 0.1f, 0.45f));
	glm::vec3 gPositionPyramidC(-2.2f, 1.45f, -1.76f);

	glm::mat4 gScalePyramidD = glm::scale(glm::vec3(0.38f, -0.3f, 0.38f));
	glm::vec3 gPositionPyramidD(-2.2f, 1.24f, -1.76f);

	glm::mat4 gScalePyramidE = glm::scale(glm::vec3(0.31f, 0.3f, 0.31f));
	glm::vec3 gPositionPyramidE(-2.2f, 1.3f, -1.76f);
	
	glm::mat4 gScalePyramidF = glm::scale(glm::vec3(1.0f, 0.2f, 1.6f));
	glm::vec3 gPositionPyramidF(0.09f, 0.24f, -5.56f);

	/*cube scale and positions*/

	glm::mat4 gScaleCubeB = glm::scale(glm::vec3(0.8f, 0.05f, 0.05f));
	glm::vec3 gPositionCubeB(-2.4f, 1.38f, -1.94f);

	glm::mat4 gScaleCubeC = glm::scale(glm::vec3(0.8f, 0.05f, 0.05f));
	glm::vec3 gPositionCubeC(-2.4f, 1.38f, -1.56f);

	glm::mat4 gScaleCubeD = glm::scale(glm::vec3(0.05f, 0.05f, 0.81f));
	glm::vec3 gPositionCubeD(-2.02f, 1.38f, -1.56f);

	glm::mat4 gScaleCubeE = glm::scale(glm::vec3(0.05f, 0.05f, 0.81f));
	glm::vec3 gPositionCubeE(-2.41f, 1.38f, -1.56f);

	glm::mat4 gScaleCubeF = glm::scale(glm::vec3(2.0f, 0.5f, 4.5f));
	glm::vec3 gPositionCubeF(-0.41f, -0.11f, -4.76f);

	glm::mat4 gScaleCubeG = glm::scale(glm::vec3(0.4f, 0.3f, 3.3f));
	glm::vec3 gPositionCubeG(-0.41f, 0.14f, -5.35f);

	glm::mat4 gScaleCubeH = glm::scale(glm::vec3(0.4f, 0.3f, 3.3f));
	glm::vec3 gPositionCubeH(0.39f, 0.14f, -5.35f);

	glm::mat4 gScaleCubeJ = glm::scale(glm::vec3(2.0f, 0.8f, 0.9f));
	glm::vec3 gPositionCubeJ(-0.41f, 0.14f, -5.47f);

	glm::mat4 gScaleCubeK = glm::scale(glm::vec3(1.8f, 0.1f, 0.7f));
	glm::vec3 gPositionCubeK(-0.36f, 0.53f, -5.52f);

	glm::mat4 gScaleSlope = glm::scale(glm::vec3(1.0f, 0.38f, 1.0f));
	glm::vec3 gPositionSlope(0.09f, 0.32f, -5.52f);

	//structure for mesh data
	struct Mesh {

		GLuint vertexObject;
		GLuint vertexBuffer[2];

		//FIXME::DELETE
		GLuint vertexObject2;
		GLuint vertexBuffer2[2];
		GLuint vertexObject3;
		GLuint vertexBuffer3[2];

		GLuint planeVO;
		GLuint planeVB[2];

		GLuint houseVO;
		GLuint houseVB[2];

		GLuint roofVO;
		GLuint roofVB[2];

		GLuint roofMVO;
		GLuint roofMVB[2];

		GLuint pillarVO;
		GLuint pillarVB[2];

		GLuint porchVO;
		GLuint porchVB[2];

		GLuint cylinderVO;
		GLuint cylinderVB[2];

		GLuint cubeVO;
		GLuint cubeVB[2];

		GLuint pyramidVO;
		GLuint pyramidVB[2];

		GLuint slopeVO;
		GLuint slopeVB[2];

		GLuint nVertices;
		//FIXME:: DELETE
		GLuint nVertices2;
		GLuint nVertices3;
		GLuint planeVerts;
		GLuint houseVerts;
		GLuint roofVerts;
		GLuint roofMVerts;
		GLuint pillarVerts;
		GLuint porchVerts;
		GLuint cylinderVerts;
		GLuint cubeVerts;
		GLuint pyramidVerts;
		GLuint slopeVerts;
	};

	//main window, mesh data, and shader
	GLFWwindow* glfwWindow = nullptr;

	Mesh glfwMesh;
	//FIXME::DELETE
	Mesh glfwMesh2;
	Mesh glfwMesh3;
	Mesh scenePlane;
	Mesh houseMain;
	Mesh roofMesh;
	Mesh roofMain;
	Mesh pillar;
	Mesh porchMesh;
	Mesh cylinderMesh;
	Mesh cubeMesh;
	Mesh pyramidMesh;
	Mesh slopeMesh;

	GLuint gTextureId;
	glm::vec2 gUVScale(1.0f, 1.0f);
	GLint gTexWrapMode = GL_CLAMP_TO_EDGE;

	GLuint gTextureId2;
	GLuint gTextureId3;
	GLuint gTextureId4;

	GLuint gHouseTexId;
	GLuint gRoofTexId;
	GLuint gRoofSideTex;
	GLuint gPillarTexId;
	GLuint gPorchTexId;
	GLuint gCylinderTexId;
	GLuint gCubeTexId;
	GLuint gPyramidTextId;
	GLuint gSlopeTextId;

	GLuint glfwProgram;
	GLuint glfwProgram2;
	GLuint glfwProgram3;
	GLuint sceneProgram;

	GLuint houseProgram;
	GLuint roofProgram;
	GLuint roofSideProg;

	GLuint pillarProg;
	GLuint porchProg;
	GLuint cylinderProg;
	GLuint cubeProgram;
	GLuint pyramidProg;
	GLuint slopeProgram;

	GLuint lampShader;
}

//primary lamp position and color
glm::vec3 lampPos(1.5f, 1.8f, 1.5f);
glm::vec3 lightColor(0.96f, 0.94f, 0.74f);

//lamp scale 
glm::vec3 lampScale(0.5f);

//fill light color and position 
glm::vec3 fillLightColor(1.0f, 1.0f, 1.0f);
glm::vec3 fillLightPos(1.5f, 1.8f, 1.5f);

//vertex shader sourced from tutorial
const GLchar * vShaderSource = GLSL(440,
	layout(location = 0) in vec3 position;
	layout(location = 1) in vec3 normal;
	layout(location = 2) in vec2 textureCoords;

	out vec3 vertexNormal;
	out vec3 vertexFragPos;
	out vec2 vertexTextureCoords;


	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
{
		//gl_position set for clip coords for camera
		gl_Position = projection * view * model * vec4(position, 1.0f);
	
		//pixel position in the environment 
		vertexFragPos = vec3(model * vec4(position, 1.0f));

		//normal vectors in the environment
		vertexNormal = mat3(transpose(inverse(model))) * normal;

		vertexTextureCoords = textureCoords;
	}
);


//fragment shader sources from tutorial
const GLchar * fragmentShaderSource = GLSL(440,

	in vec3 vertexNormal;
	in vec3 vertexFragPos;
	in vec2 vertexTextureCoords;

	out vec4 fragColor;

	uniform vec3 sLampPos;
	uniform vec3 sLightColor;

	uniform vec3 sFillLightPos;
	uniform vec3 sFillLightColor;

	uniform vec3 viewPosition;
	uniform sampler2D uTexture;
	uniform vec2 uvScale;

void main()
{

	//ambient light calc
	float ambientStrength = 0.2f;
	vec3 ambient = ambientStrength * sFillLightColor;

	//diffuse lighting calc
	vec3 norm = normalize(vertexNormal);
	vec3 lightDirection = normalize(sLampPos - vertexFragPos);
	float impact = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = impact * sLightColor;

	//specular lighting
	//FIXME:: ONLY NEEDED FOR THE TRUCK AND WATER
	float specularIntensity = 0.2f;
	float highlightSize = 1.0f;
	vec3 viewDir = normalize(viewPosition - vertexFragPos);
	vec3 reflectDir = reflect(-lightDirection, norm);

	//specular comp
	//FIXME:: ONLY NEEDED FOR THE TRUCK AND WATER
	float specularComp = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
	vec3 specular = specularIntensity * specularComp * sLightColor;

	//texture color
	vec4 textureColor = texture(uTexture, vertexTextureCoords);

	vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

	fragColor = vec4(phong, 1.0);
}
);

//textures will need to have their y-axis flips 
void flipImageVertically(unsigned char *image, int width, int height, int channels)
{
	for (int j = 0; j < height / 2; ++j)
	{
		int index1 = j * width * channels;
		int index2 = (height - 1 - j) * width * channels;

		for (int i = width * channels; i > 0; --i)
		{
			unsigned char tmp = image[index1];
			image[index1] = image[index2];
			image[index2] = tmp;
			++index1;
			++index2;
		}
	}
}

//initialize the program, create the window and render the mesh
bool Init(int, char*[], GLFWwindow** window);
void Resize(GLFWwindow* window, int width, int height);
void Input(GLFWwindow* window);
void changeAngle(GLFWwindow* window);

void CreateOBJ(Mesh &mesh);
void DestroyOBJ(Mesh &mesh);

bool UCreateTexture(const char* filename, GLuint &textureId);
void UDestroyTexture(GLuint textureId);

void Render();
bool CreateShader(const char* vtxShaderSource, const char* fragShaderSource, GLuint &programId);
void DestroyShader(GLuint programId);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);


//Initialize glfw, and glew
bool Init(int argc, char* argv[], GLFWwindow** window) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//window is created 
	*window = glfwCreateWindow(winWidth, winHeight, winTitle, NULL, NULL);
	if (*window == NULL)
	{
		std::cout << "Window ceration failed.." << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(*window);
	glfwSetFramebufferSizeCallback(*window, Resize);

	glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
	//glfw functions to start mouse/scroll wheel tracking
	glfwSetCursorPosCallback(*window, mouse_callback);
	glfwSetScrollCallback(*window, scroll_callback);

	//glfw will capture mouse movements
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//glew initialized
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult)
	{

		std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
		return false;
	}

	return true;
}


//glfw function used to read user input
void Input(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

		glfwSetWindowShouldClose(window, true);
	}

	//camera speed can be adjusted
	//const float cameraSpeed = 0.5f;
	float cameraSpeed = static_cast<float>(speed * deltaTime);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

		cameraPos += cameraSpeed * cameraFront;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

		cameraPos -= cameraSpeed * cameraFront;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraUp;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraUp;
	}
}

//function will scan for keypress "p" when pressed there is a delay then the camera angle will change
void changeAngle(GLFWwindow* window) {

	//used for delay functionality 
	//Source: https://stackoverflow.com/questions/158585/how-do-you-add-a-timed-delay-to-a-c-program

	using namespace std::this_thread;
	using namespace std::chrono;

	//link variable to key "p"
	int state = glfwGetKey(window, GLFW_KEY_P);

	//wait for key press
	if (state == GLFW_PRESS) {

		cameraAngle++;

		//delay cameraAngle change
		sleep_for(nanoseconds(10));
		sleep_until(system_clock::now() + milliseconds(300));
	}

	//ensure that cameraAngle is never more than 2
	if (cameraAngle > 2) {
		cameraAngle = 0;
	}
}

//if window is resized this function will initialize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);
}

//if mouse is moved 
void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn) {

	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	if (firstMouse) {

		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffSet = xPos - lastX;
	float yOffSet = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	float sensitivity = 0.1f;
	xOffSet *= sensitivity;
	yOffSet *= sensitivity;

	yaw += xOffSet;
	pitch += yOffSet;

	if (pitch > 89.0f) {

		pitch = 89.0f;
	}
	if (pitch < -89.0f) {

		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

//when mouse is scrolled
void scroll_callback(GLFWwindow* window, double xOffSet, double yOffSet) {

	//speed will only go as low as 0.1 and as high as 5.0
	speed -= (float)yOffSet;
	if (speed < 0.1f) {
		speed = 0.1f;
	}

	if (speed > 5.0f) {
		speed = 5.0f;
	}
}


//glfw function used to resize the window if needed 
void Resize(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);
}

void Render() {

	//enable depth, clear the buffers and frame
	glEnable(GL_DEPTH_TEST);

	////enable alpha on RGB
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//scales of object is set
	glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

	//object is rotated in the x axis
	glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));

	//object is moved to center
	glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

	//apply from right to the left
	glm::mat4 model = translation * rotation * scale;

	glm::mat4 view;

	//new projection allows for camera functionality 
	glm::mat4 projection;

	//perspective free cam 
	if (cameraAngle == 0) {
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		projection = glm::perspective(glm::radians(fov), (float)winWidth / (float)winHeight, 0.1f, 100.0f);
	}

	//perspective static 3D view
	if (cameraAngle == 1) {
		view = glm::lookAt
		(glm::vec3(5.5f, 5.5f, 15.0f),
			glm::vec3(0.0f, 4.0f, 2.0f),
			glm::vec3(0.0f, 2.0f, 0.0f));

		projection = glm::perspective(glm::radians(fov), (float)winWidth / (float)winHeight, 0.1f, 100.0f);
	}

	//orthographic static 2D view
	if (cameraAngle == 2) {
		view = glm::translate(glm::vec3(0.0f, 0.0f, -5.0f));

		projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
	}
	
	//use shader
	glUseProgram(glfwProgram); {

		//transformations are passed to shader
		GLint modelLoc = glGetUniformLocation(glfwProgram, "model");
		GLint viewLoc = glGetUniformLocation(glfwProgram, "view");
		GLint projLoc = glGetUniformLocation(glfwProgram, "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLoc = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLoc = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLoc = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLoc = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLoc = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLoc, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLoc, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLoc, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(glfwMesh.vertexObject);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gTextureId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, glfwMesh.nVertices);
	}


	//FIXME:: Implement instancing NOT two shader programs
	glUseProgram(glfwProgram2); {

		//model = glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLo = glGetUniformLocation(glfwProgram2, "model");
		GLint viewLo = glGetUniformLocation(glfwProgram2, "view");
		GLint projLo = glGetUniformLocation(glfwProgram2, "projection");

		glUniformMatrix4fv(modelLo, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLo, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLo, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLo = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLo, 1, glm::value_ptr(gUVScale));


		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLoc2 = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLoc2 = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLoc2 = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLoc2 = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLoc2, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLoc2, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLoc2, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLoc2, fillLightPos.x, fillLightPos.y, fillLightPos.z);


		glBindVertexArray(glfwMesh2.vertexObject2);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gTextureId2);

		glDrawArrays(GL_TRIANGLES, 0, glfwMesh2.nVertices2);
		//FIXME:: Implement instancing NOT two shader programs
	}

	//FIXME:: Implement instancing NOT two shader programs
	glUseProgram(glfwProgram3); {

		//model = glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelL = glGetUniformLocation(glfwProgram3, "model");
		GLint viewL = glGetUniformLocation(glfwProgram3, "view");
		GLint projL = glGetUniformLocation(glfwProgram3, "projection");

		glUniformMatrix4fv(modelL, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewL, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projL, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleL = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleL, 1, glm::value_ptr(gUVScale));


		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLoc3 = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLoc3 = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLoc3 = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLoc3 = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLoc3, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLoc3, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLoc3, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLoc3, fillLightPos.x, fillLightPos.y, fillLightPos.z);


		glBindVertexArray(glfwMesh3.vertexObject3);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gTextureId3);

		glDrawArrays(GL_TRIANGLES, 0, glfwMesh3.nVertices3);
		//FIXME:: Implement instancing NOT two shader programs
	}

	glUseProgram(sceneProgram); {

		//model = glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLocScene = glGetUniformLocation(sceneProgram, "model");
		GLint viewLocScene = glGetUniformLocation(sceneProgram, "view");
		GLint projLocScene = glGetUniformLocation(sceneProgram, "projection");

		glUniformMatrix4fv(modelLocScene, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocScene, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocScene, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocScene = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocScene, 1, glm::value_ptr(gUVScale));


		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocScene = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocScene = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocScene = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocScene = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocScene, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocScene, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocScene, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocScene, fillLightPos.x, fillLightPos.y, fillLightPos.z);


		glBindVertexArray(scenePlane.planeVO);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gTextureId4);

		glDrawArrays(GL_TRIANGLES, 0, scenePlane.planeVerts);
		//FIXME:: Implement instancing NOT two shader programs
	}

	//use shader
	glUseProgram(houseProgram); {

		model = glm::translate(glm::vec3(-5.0f, -0.65f, -0.5f));

		//transformations are passed to shader
		GLint modelLocHouse = glGetUniformLocation(houseProgram, "model");
		GLint viewLocHouse = glGetUniformLocation(houseProgram, "view");
		GLint projLocHouse = glGetUniformLocation(houseProgram, "projection");

		glUniformMatrix4fv(modelLocHouse, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocHouse, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocHouse, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocHouse = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocHouse, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocHouse = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocHouse = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocHouse = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocHouse = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocHouse, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocHouse, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocHouse, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocHouse, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(houseMain.houseVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, gHouseTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, houseMain.houseVerts);
	}

	glUseProgram(houseProgram); {

		model = glm::translate(gPosition) * (gScale);

		//transformations are passed to shader
		GLint modelLocHouse = glGetUniformLocation(houseProgram, "model");
		GLint viewLocHouse = glGetUniformLocation(houseProgram, "view");
		GLint projLocHouse = glGetUniformLocation(houseProgram, "projection");

		glUniformMatrix4fv(modelLocHouse, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocHouse, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocHouse, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocHouse = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocHouse, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocHouse = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocHouse = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocHouse = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocHouse = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocHouse, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocHouse, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocHouse, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocHouse, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(houseMain.houseVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, gHouseTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, houseMain.houseVerts);
	}

	//draw roof sides
	glUseProgram(roofSideProg); {

		model = glm::translate(glm::vec3(-5.0f, -0.65f, -0.5f));

		//transformations are passed to shader
		GLint modelLocRoof = glGetUniformLocation(roofSideProg, "model");
		GLint viewLocRoof = glGetUniformLocation(roofSideProg, "view");
		GLint projLocRoof = glGetUniformLocation(roofSideProg, "projection");

		glUniformMatrix4fv(modelLocRoof, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocRoof, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocRoof, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocRoof = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocRoof, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocRoof = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocRoof = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocRoof = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocRoof = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocRoof, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocRoof, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocRoof, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocRoof, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(roofMesh.roofVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, gRoofSideTex);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, roofMesh.roofVerts);
	}

	//draw roof sides
	glUseProgram(roofSideProg); {

		model = glm::translate(gPositionRoofSide) * (gScaleRoofSide);

		//transformations are passed to shader
		GLint modelLocRoof = glGetUniformLocation(roofSideProg, "model");
		GLint viewLocRoof = glGetUniformLocation(roofSideProg, "view");
		GLint projLocRoof = glGetUniformLocation(roofSideProg, "projection");

		glUniformMatrix4fv(modelLocRoof, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocRoof, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocRoof, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocRoof = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocRoof, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocRoof = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocRoof = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocRoof = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocRoof = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocRoof, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocRoof, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocRoof, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocRoof, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(roofMesh.roofVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, gRoofSideTex);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, roofMesh.roofVerts);
	}

	//draw main roof
	glUseProgram(roofProgram); {

		model = glm::translate(glm::vec3(-5.0f, -0.65f, -0.5f));

		//transformations are passed to shader
		GLint modelLocRoofM = glGetUniformLocation(roofProgram, "model");
		GLint viewLocRoofM = glGetUniformLocation(roofProgram, "view");
		GLint projLocRoofM = glGetUniformLocation(roofProgram, "projection");

		glUniformMatrix4fv(modelLocRoofM, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocRoofM, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocRoofM, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocRoofM = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocRoofM, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocRoofM = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocRoofM = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocRoofM = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocRoofM = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocRoofM, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocRoofM, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocRoofM, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocRoofM, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(roofMain.roofMVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, gRoofTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, roofMain.roofMVerts);
	}

	//draw main roof
	glUseProgram(roofProgram); {

		model = glm::translate(gPositionRoof) * (gScaleRoof);

		//transformations are passed to shader
		GLint modelLocRoofM = glGetUniformLocation(roofProgram, "model");
		GLint viewLocRoofM = glGetUniformLocation(roofProgram, "view");
		GLint projLocRoofM = glGetUniformLocation(roofProgram, "projection");

		glUniformMatrix4fv(modelLocRoofM, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocRoofM, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocRoofM, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocRoofM = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocRoofM, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocRoofM = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocRoofM = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocRoofM = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocRoofM = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocRoofM, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocRoofM, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocRoofM, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocRoofM, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(roofMain.roofMVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, gRoofTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, roofMain.roofMVerts);
	}

	//pillar1
	glUseProgram(pillarProg); {

		model = glm::translate(glm::vec3(-1.8f, -0.65f, -0.2f));

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(pillarProg, "model");
		GLint viewLocPillar = glGetUniformLocation(pillarProg, "view");
		GLint projLocPillar = glGetUniformLocation(pillarProg, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(pillar.pillarVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, gPillarTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, pillar.pillarVerts);
	}

	//pillar2
	glUseProgram(pillarProg); {

		model = glm::translate(glm::vec3(-1.8f, -0.65f, -1.5f));

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(pillarProg, "model");
		GLint viewLocPillar = glGetUniformLocation(pillarProg, "view");
		GLint projLocPillar = glGetUniformLocation(pillarProg, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(pillar.pillarVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, gPillarTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, pillar.pillarVerts);
	}

	//pillar3
	glUseProgram(pillarProg); {

		model = glm::translate(glm::vec3(-1.8f, -0.65f, -3.1f));

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(pillarProg, "model");
		GLint viewLocPillar = glGetUniformLocation(pillarProg, "view");
		GLint projLocPillar = glGetUniformLocation(pillarProg, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(pillar.pillarVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, gPillarTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, pillar.pillarVerts);
	}

	//pillar4
	glUseProgram(pillarProg); {

		model = glm::translate(glm::vec3(-1.8f, -0.65f, -4.4f));

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(pillarProg, "model");
		GLint viewLocPillar = glGetUniformLocation(pillarProg, "view");
		GLint projLocPillar = glGetUniformLocation(pillarProg, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(pillar.pillarVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, gPillarTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, pillar.pillarVerts);
	}

	//porch
	glUseProgram(porchProg); {

		model = glm::translate(glm::vec3(-2.0f, -0.04f, -0.5f));

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(porchProg, "model");
		GLint viewLocPillar = glGetUniformLocation(porchProg, "view");
		GLint projLocPillar = glGetUniformLocation(porchProg, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(porchMesh.porchVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, gPorchTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, porchMesh.porchVerts);
	}

	//lamp cylinder
	glUseProgram(cylinderProg); {

		model = glm::translate(gPositionCylinder) * (gScaleCylinder);

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(cylinderProg, "model");
		GLint viewLocPillar = glGetUniformLocation(cylinderProg, "view");
		GLint projLocPillar = glGetUniformLocation(cylinderProg, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(cylinderMesh.cylinderVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, gCylinderTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, cylinderMesh.cylinderVerts);
	}

	//lamp cylinder2
	glUseProgram(cylinderProg); {

		model = glm::translate(gPositionCylinderB) * (gScaleCylinderB);

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(cylinderProg, "model");
		GLint viewLocPillar = glGetUniformLocation(cylinderProg, "view");
		GLint projLocPillar = glGetUniformLocation(cylinderProg, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(cylinderMesh.cylinderVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, gCylinderTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, cylinderMesh.cylinderVerts);
	}

	//lamp cube2
	glUseProgram(cubeProgram); {

		model = glm::translate(gPositionCubeB) * (gScaleCubeB);

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(cubeProgram, "model");
		GLint viewLocPillar = glGetUniformLocation(cubeProgram, "view");
		GLint projLocPillar = glGetUniformLocation(cubeProgram, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(cubeMesh.cubeVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, gCubeTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, cubeMesh.cubeVerts);
	}

	//lamp cube3
	glUseProgram(cubeProgram); {

		model = glm::translate(gPositionCubeC) * (gScaleCubeC);

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(cubeProgram, "model");
		GLint viewLocPillar = glGetUniformLocation(cubeProgram, "view");
		GLint projLocPillar = glGetUniformLocation(cubeProgram, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(cubeMesh.cubeVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, gCubeTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, cubeMesh.cubeVerts);
	}

	//lamp cube4
	glUseProgram(cubeProgram); {

		model = glm::translate(gPositionCubeD) * (gScaleCubeD);

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(cubeProgram, "model");
		GLint viewLocPillar = glGetUniformLocation(cubeProgram, "view");
		GLint projLocPillar = glGetUniformLocation(cubeProgram, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(cubeMesh.cubeVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, gCubeTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, cubeMesh.cubeVerts);
	}

	//lamp cube5
	glUseProgram(cubeProgram); {

		model = glm::translate(gPositionCubeE) * (gScaleCubeE);

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(cubeProgram, "model");
		GLint viewLocPillar = glGetUniformLocation(cubeProgram, "view");
		GLint projLocPillar = glGetUniformLocation(cubeProgram, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(cubeMesh.cubeVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, gCubeTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, cubeMesh.cubeVerts);
	}

	//lamp cube6
	glUseProgram(cubeProgram); {

		model = glm::translate(gPositionCubeF) * (gScaleCubeF);

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(cubeProgram, "model");
		GLint viewLocPillar = glGetUniformLocation(cubeProgram, "view");
		GLint projLocPillar = glGetUniformLocation(cubeProgram, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(cubeMesh.cubeVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, gCubeTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, cubeMesh.cubeVerts);
	}

	//lamp cube7
	glUseProgram(cubeProgram); {

		model = glm::translate(gPositionCubeG) * (gScaleCubeG);

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(cubeProgram, "model");
		GLint viewLocPillar = glGetUniformLocation(cubeProgram, "view");
		GLint projLocPillar = glGetUniformLocation(cubeProgram, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(cubeMesh.cubeVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, gCubeTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, cubeMesh.cubeVerts);
	}

	//lamp cube8
	glUseProgram(cubeProgram); {

		model = glm::translate(gPositionCubeH) * (gScaleCubeH);

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(cubeProgram, "model");
		GLint viewLocPillar = glGetUniformLocation(cubeProgram, "view");
		GLint projLocPillar = glGetUniformLocation(cubeProgram, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(cubeMesh.cubeVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, gCubeTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, cubeMesh.cubeVerts);
	}

	//lamp cube9
	glUseProgram(cubeProgram); {

		model = glm::translate(gPositionCubeJ) * (gScaleCubeJ);

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(cubeProgram, "model");
		GLint viewLocPillar = glGetUniformLocation(cubeProgram, "view");
		GLint projLocPillar = glGetUniformLocation(cubeProgram, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(cubeMesh.cubeVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, gCubeTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, cubeMesh.cubeVerts);
	}

	//lamp cube10
	glUseProgram(cubeProgram); {

		model = glm::translate(gPositionCubeK) * (gScaleCubeK);

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(cubeProgram, "model");
		GLint viewLocPillar = glGetUniformLocation(cubeProgram, "view");
		GLint projLocPillar = glGetUniformLocation(cubeProgram, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(cubeMesh.cubeVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, gCubeTexId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, cubeMesh.cubeVerts);
	}

	//lamp pyramid
	glUseProgram(pyramidProg); {

		model = glm::translate(gPositionPyramid) * (gScalePyramid);
		

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(pyramidProg, "model");
		GLint viewLocPillar = glGetUniformLocation(pyramidProg, "view");
		GLint projLocPillar = glGetUniformLocation(pyramidProg, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(pyramidMesh.pyramidVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, gPyramidTextId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, pyramidMesh.pyramidVerts);
	}

	//lamp pyramid2
	glUseProgram(pyramidProg); {

		model = glm::translate(gPositionPyramidB) * (gScalePyramidB);


		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(pyramidProg, "model");
		GLint viewLocPillar = glGetUniformLocation(pyramidProg, "view");
		GLint projLocPillar = glGetUniformLocation(pyramidProg, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(pyramidMesh.pyramidVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, gPyramidTextId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, pyramidMesh.pyramidVerts);
	}

	//lamp pyramid3
	glUseProgram(pyramidProg); {

		model = glm::translate(gPositionPyramidC) * (gScalePyramidC);


		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(pyramidProg, "model");
		GLint viewLocPillar = glGetUniformLocation(pyramidProg, "view");
		GLint projLocPillar = glGetUniformLocation(pyramidProg, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(pyramidMesh.pyramidVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, gPyramidTextId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, pyramidMesh.pyramidVerts);
	}

	//lamp pyramid4
	glUseProgram(pyramidProg); {

		model = glm::translate(gPositionPyramidD) * (gScalePyramidD);


		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(pyramidProg, "model");
		GLint viewLocPillar = glGetUniformLocation(pyramidProg, "view");
		GLint projLocPillar = glGetUniformLocation(pyramidProg, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(pyramidMesh.pyramidVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, gPyramidTextId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, pyramidMesh.pyramidVerts);
	}

	//lamp pyramid5
	glUseProgram(pyramidProg); {

		model = glm::translate(gPositionPyramidE) * (gScalePyramidE);


		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(pyramidProg, "model");
		GLint viewLocPillar = glGetUniformLocation(pyramidProg, "view");
		GLint projLocPillar = glGetUniformLocation(pyramidProg, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(pyramidMesh.pyramidVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, gPyramidTextId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, pyramidMesh.pyramidVerts);
	}

	//lamp pyramid6
	glUseProgram(pyramidProg); {

		model = glm::translate(gPositionPyramidF) * (gScalePyramidF);

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(pyramidProg, "model");
		GLint viewLocPillar = glGetUniformLocation(pyramidProg, "view");
		GLint projLocPillar = glGetUniformLocation(pyramidProg, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(pyramidMesh.pyramidVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, gPyramidTextId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, pyramidMesh.pyramidVerts);
	}

	//lamp pyramid6
	glUseProgram(slopeProgram); {

		model = glm::translate(gPositionSlope) * (gScaleSlope);

		//transformations are passed to shader
		GLint modelLocPillar = glGetUniformLocation(slopeProgram, "model");
		GLint viewLocPillar = glGetUniformLocation(slopeProgram, "view");
		GLint projLocPillar = glGetUniformLocation(slopeProgram, "projection");

		glUniformMatrix4fv(modelLocPillar, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocPillar, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocPillar, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLocPillar = glGetUniformLocation(glfwProgram, "uvScale");
		glUniform2fv(UVScaleLocPillar, 1, glm::value_ptr(gUVScale));

		//matric uniforms are passed to the obj from the shader 
		GLint lightColorLocPillar = glGetUniformLocation(glfwProgram, "sLightColor");
		GLint lampPosLocPillar = glGetUniformLocation(glfwProgram, "sLampPos");

		GLint fillLightColorLocPillar = glGetUniformLocation(glfwProgram, "sFillLightColor");
		GLint fillLightPosLocPillar = glGetUniformLocation(glfwProgram, "sFillLightPos");

		glUniform3f(lightColorLocPillar, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lampPosLocPillar, lampPos.x, lampPos.y, lampPos.z);

		glUniform3f(fillLightColorLocPillar, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPosLocPillar, fillLightPos.x, fillLightPos.y, fillLightPos.z);

		// Activate the VBOs contained within the mesh's VAO
		glBindVertexArray(slopeMesh.slopeVO);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE12);
		glBindTexture(GL_TEXTURE_2D, gSlopeTextId);

		// Draws the triangles
		glDrawArrays(GL_TRIANGLES, 0, slopeMesh.slopeVerts);
	}

	glBindVertexArray(0);
	glfwSwapBuffers(glfwWindow);
}

void CreateOBJ(Mesh &mesh)
{
	
	GLfloat verts[] = {

		/*******************front cube********************/

		//Positions    //negative z normal      //Texture Coordinates

		//front
		 0.5f,  0.2f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //top right
		-0.5f, -0.1f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //bottom left
		 0.5f, -0.1f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, //bottom right 

		-0.5f,  0.2f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, //top left
		-0.5f, -0.1f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //bottom left
		 0.5f,  0.2f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //top right


		 //back
		 0.5f,  0.2f, -0.1f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, //top right
		-0.5f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, //bottom left
		 0.5f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f, //bottom right 

		-0.5f,  0.2f, -0.1f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f, //top left
		-0.5f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.2f, -0.1f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, //top right


		 //right
		 0.5f,  0.2f, -0.1f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right
		 0.5f, -0.1f, -0.1f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.5f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f, //bottom right 

		 0.5f,  0.2f,  0.0f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f, //top left
		 0.5f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.2f, -0.1f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right


		 //left
		-0.5f,  0.2f, -0.1f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right
		-0.5f, -0.1f, -0.1f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		-0.5f, -0.1f,  0.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, //bottom right 

		-0.5f,  0.2f,  0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, //top left
		-0.5f, -0.1f,  0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		-0.5f,  0.2f, -0.1f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right


		//top
		-0.5f,  0.2f, -0.1f,   0.0f, 1.0f,  0.0f,   1.0f, 1.0f, //top left
		-0.5f,  0.2f,  0.0f,   0.0f, 1.0f,  0.0f,   0.0f, 1.0f, //bottom left
		 0.5f,  0.2f,  0.0f,   0.0f, 1.0f,  0.0f,   0.0f, 0.0f, //top right

		 0.5f,  0.2f, -0.1f,   0.0f, 1.0f,  0.0f,   0.0f, 1.0f, //top left
		-0.5f,  0.2f, -0.1f,   0.0f, 1.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.2f,  0.0f,   0.0f, 1.0f,  0.0f,   1.0f, 1.0f, //top right


		 //bottom
		-0.5f, -0.1f, -0.1f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, //top left
		-0.5f, -0.1f,  0.0f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f, //bottom left
		 0.5f, -0.1f,  0.0f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f, //top right

		 0.5f, -0.1f, -0.1f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f, //top left
		-0.5f, -0.1f, -0.1f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.5f, -0.1f,  0.0f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, //top right

		 /*******************front cube********************/



		/*******************back cube********************/

		//Positions    //negative z normal      //Texture Coordinates

		//front
		 0.5f,  0.2f, -1.0f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f, //top right
		-0.5f, -0.1f, -1.0f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f, //bottom left
		 0.5f, -0.1f, -1.0f,  0.0f,  0.0f,  1.0f,   1.0f, 0.0f, //bottom right 

		-0.5f,  0.2f, -1.0f,  0.0f,  0.0f,  1.0f,   0.0f, 1.0f, //top left
		-0.5f, -0.1f, -1.0f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.2f, -1.0f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f, //top right


		 //back
		 0.5f,  0.2f, -1.1f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, //top right
		-0.5f, -0.1f, -1.1f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, //bottom left
		 0.5f, -0.1f, -1.1f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f, //bottom right 

		-0.5f,  0.2f, -1.1f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f, //top left
		-0.5f, -0.1f, -1.1f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.2f, -1.1f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, //top right


		 //right
		 0.5f,  0.2f, -1.1f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right
		 0.5f, -0.1f, -1.1f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.5f, -0.1f, -1.0f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f, //bottom right 

		 0.5f,  0.2f, -1.0f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f, //top left
		 0.5f, -0.1f, -1.0f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.2f, -1.1f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right


		 //left
		-0.5f,  0.2f, -1.1f, -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right
		-0.5f, -0.1f, -1.1f, -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		-0.5f, -0.1f, -1.0f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, //bottom right 

		-0.5f,  0.2f, -1.0f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, //top left
		-0.5f, -0.1f, -1.0f, -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		-0.5f,  0.2f, -1.1f, -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right


		//top
		-0.5f,  0.2f, -1.1f,  0.0f,  1.0f,  0.0f,   1.0f, 1.0f, //top left
		-0.5f,  0.2f, -1.0f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f, //bottom left
		 0.5f,  0.2f, -1.0f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f, //top right

		 0.5f,  0.2f, -1.1f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f, //top left
		-0.5f,  0.2f, -1.1f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.2f, -1.0f,  0.0f,  1.0f,  0.0f,   1.0f, 1.0f, //top right


		 //bottom
		-0.5f, -0.1f, -1.1f,  0.0f, -1.0f,  0.0f,   1.0f, 1.0f, //top left
		-0.5f, -0.1f, -1.0f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f, //bottom left
		 0.5f, -0.1f, -1.0f,  0.0f, -1.0f,  0.0f,   0.0f, 0.0f, //top right

		 0.5f, -0.1f, -1.1f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f, //top left
		-0.5f, -0.1f, -1.1f,  0.0f, -1.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.5f, -0.1f, -1.0f,  0.0f, -1.0f,  0.0f,   1.0f, 1.0f, //top right

		 /*******************back cube********************/



		 /*******************right cube********************/

		//Positions     //negative z normal     //Texture Coordinates

		//front
		 0.5f,  0.2f, -1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //top right
		 0.4f, -0.1f, -1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bottom left
		 0.5f, -0.1f, -1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, //bottom right 

		 0.4f,  0.2f, -1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, //top left
		 0.4f, -0.1f, -1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bottom left
		 0.5f,  0.2f, -1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //top right


		 //back
		 0.5f,  0.2f, -0.1f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f, //top right
		 0.4f, -0.1f, -0.1f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, //bottom left
		 0.5f, -0.1f, -0.1f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, //bottom right 

		 0.4f,  0.2f, -0.1f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f, //top left
		 0.4f, -0.1f, -0.1f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, //bottom left
		 0.5f,  0.2f, -0.1f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f, //top right


		 //right
		 0.5f,  0.2f, -0.1f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //top right
		 0.5f, -0.1f, -0.1f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, //bottom left
		 0.5f, -0.1f, -1.0f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f, //bottom right 

		 0.5f,  0.2f, -1.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //top left
		 0.5f, -0.1f, -1.0f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f, //bottom left
		 0.5f,  0.2f, -0.1f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //top right


		 //left
		 0.4f,  0.2f, -0.1f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //top right
		 0.4f, -0.1f, -0.1f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f, //bottom left
		 0.4f, -0.1f, -1.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, //bottom right 

		 0.4f,  0.2f, -1.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //top left
		 0.4f, -0.1f, -1.0f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, //bottom left
		 0.4f,  0.2f, -0.1f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //top right


		//top
		 0.4f,  0.2f, -0.1f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, //top left
		 0.4f,  0.2f, -1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, //bottom left
		 0.5f,  0.2f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, //top right

		 0.5f,  0.2f, -0.1f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //top left
		 0.4f,  0.2f, -0.1f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.2f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, //top right


		 //bottom
		 0.4f, -0.1f, -0.1f,   0.0f, -1.0f,  0.0f,  0.0f, 0.0f, //top left
		 0.4f, -0.1f, -1.0f,   0.0f, -1.0f,  0.0f,  0.0f, 1.0f, //bottom left
		 0.5f, -0.1f, -1.0f,   0.0f, -1.0f,  0.0f,  1.0f, 1.0f, //top right

		 0.5f, -0.1f, -0.1f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f, //top left
		 0.4f, -0.1f, -0.1f,   0.0f, -1.0f,  0.0f,  0.0f, 0.0f, //bottom left
		 0.5f, -0.1f, -1.0f,   0.0f, -1.0f,  0.0f,  1.0f, 1.0f, //top right

		 /*******************right cube********************/



		/*******************left cube********************/

		//Positions     //negative z normal     //Texture Coordinates

		//front
		-0.5f,  0.2f, -1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //top right
		-0.4f, -0.1f, -1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bottom left
		-0.5f, -0.1f, -1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, //bottom right 

		-0.4f,  0.2f, -1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, //top left
		-0.4f, -0.1f, -1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bottom left
		-0.5f,  0.2f, -1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //top right


		 //back
		-0.5f,  0.2f, -0.1f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f, //top right
		-0.4f, -0.1f, -0.1f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, //bottom left
		-0.5f, -0.1f, -0.1f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, //bottom right 

		-0.4f,  0.2f, -0.1f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f, //top left
		-0.4f, -0.1f, -0.1f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, //bottom left
		-0.5f,  0.2f, -0.1f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f, //top right


		 //right
		-0.5f,  0.2f, -0.1f, -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //top right
		-0.5f, -0.1f, -0.1f, -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, //bottom left
		-0.5f, -0.1f, -1.0f, -1.0f, 0.0f, 0.0f,   1.0f, 0.0f, //bottom right 

		-0.5f,  0.2f, -1.0f, -1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //top left
		-0.5f, -0.1f, -1.0f, -1.0f, 0.0f, 0.0f,   1.0f, 0.0f, //bottom left
		-0.5f,  0.2f, -0.1f, -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //top right


		 //left
		-0.4f,  0.2f, -0.1f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //top right
		-0.4f, -0.1f, -0.1f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f, //bottom left
		-0.4f, -0.1f, -1.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, //bottom right 

		-0.4f,  0.2f, -1.0f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //top left
		-0.4f, -0.1f, -1.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, //bottom left
		-0.4f,  0.2f, -0.1f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //top right


		//top
		-0.4f,  0.2f, -0.1f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, //top left
		-0.4f,  0.2f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, //bottom left
		-0.5f,  0.2f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, //top right

		-0.5f,  0.2f, -0.1f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, //top left
		-0.4f,  0.2f, -0.1f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, //bottom left
		-0.5f,  0.2f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, //top right


		 //bottom
		-0.4f, -0.1f, -0.1f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, //top left
		-0.4f, -0.1f, -1.0f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f, //bottom left
		-0.5f, -0.1f, -1.0f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f, //top right

		-0.5f, -0.1f, -0.1f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, //top left
		-0.4f, -0.1f, -0.1f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, //bottom left
		-0.5f, -0.1f, -1.0f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f, //top right

		 /*******************left cube********************/
		
	};

	GLfloat verts2[] = {
	

		/*******************front top cube********************/

		//Positions          //Texture Coordinates

		//front
		 0.53f,  0.22f,  0.03f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //top right
		-0.53f,  0.2f,   0.03f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bottom left
		 0.53f,  0.2f,   0.03f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, //bottom right 

		-0.53f,  0.22f,  0.03f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, //top left
		-0.53f,  0.2f,   0.03f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bottom left
		 0.53f,  0.22f,  0.03f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //top right


		 //back
		 0.53f,  0.22f, -0.13f,   0.0f, 0.0f,-1.0f,  1.0f, 1.0f, //top right
		-0.53f,  0.2f,  -0.13f,   0.0f, 0.0f,-1.0f,  0.0f, 0.0f, //bottom left
		 0.53f,  0.2f,  -0.13f,   0.0f, 0.0f,-1.0f,  1.0f, 0.0f, //bottom right 

		-0.53f,  0.22f, -0.13f,   0.0f, 0.0f,-1.0f,  0.0f, 1.0f, //top left
		-0.53f,  0.2f,  -0.13f,   0.0f, 0.0f,-1.0f,  0.0f, 0.0f, //bottom left
		 0.53f,  0.22f, -0.13f,   0.0f, 0.0f,-1.0f,  1.0f, 1.0f, //top right


		 //right
		 0.53f,  0.22f, -0.13f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //top right
		 0.53f,  0.2f,  -0.13f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, //bottom left
		 0.53f,  0.2f,   0.03f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f, //bottom right 

		 0.53f,  0.22f,  0.03f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //top left
		 0.53f,  0.2f,   0.03f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, //bottom left
		 0.53f,  0.22f, -0.13f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //top right


		 //left
		-0.53f,  0.22f, -0.13f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //top right
		-0.53f,  0.2f,  -0.13f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, //bottom left
		-0.53f,  0.2f,   0.03f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f, //bottom right 

		-0.53f,  0.22f,  0.03f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //top left
		-0.53f,  0.2f,   0.03f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, //bottom left
		-0.53f,  0.22f, -0.13f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //top right


		//top
		-0.53f,  0.22f, -0.13f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, //top left
		-0.53f,  0.22f,  0.03f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, //bottom left
		 0.53f,  0.22f,  0.03f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //top right

		 0.53f,  0.22f, -0.13f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, //top left
		-0.53f,  0.22f, -0.13f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, //bottom left
		 0.53f,  0.22f,  0.03f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //top right


		 //bottom
		-0.53f,  0.2f,  -0.13f,   0.0f,-1.0f, 0.0f,   1.0f, 1.0f, //top left
		-0.53f,  0.2f,   0.03f,   0.0f,-1.0f, 0.0f,   0.0f, 1.0f, //bottom left
		 0.53f,  0.2f,   0.03f,   0.0f,-1.0f, 0.0f,   0.0f, 0.0f, //top right

		 0.53f,  0.2f,  -0.13f,   0.0f,-1.0f, 0.0f,   0.0f, 1.0f, //top left
		-0.53f,  0.2f,  -0.13f,   0.0f,-1.0f, 0.0f,   0.0f, 0.0f, //bottom left
		 0.53f,  0.2f,   0.03f,   0.0f,-1.0f, 0.0f,   1.0f, 1.0f, //top right

		 /*******************front top cube********************/


		 /*******************back top cube********************/

		//Positions          //Texture Coordinates

		//front
		 0.53f,  0.22f, -0.97f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f, //top right
		-0.53f,  0.2f,  -0.97f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, //bottom left
		 0.53f,  0.2f,  -0.97f,  0.0f, 0.0f, 1.0f,   1.0f, 0.0f, //bottom right 

		-0.53f,  0.22f, -0.97f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f, //top left
		-0.53f,  0.2f,  -0.97f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, //bottom left
		 0.53f,  0.22f, -0.97f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f, //top right


		 //back
		 0.53f,  0.22f, -1.13f,  0.0f, 0.0f,-1.0f,   1.0f, 1.0f, //top right
		-0.53f,  0.2f,  -1.13f,  0.0f, 0.0f,-1.0f,   0.0f, 0.0f, //bottom left
		 0.53f,  0.2f,  -1.13f,  0.0f, 0.0f,-1.0f,   1.0f, 0.0f, //bottom right 

		-0.53f,  0.22f, -1.13f,  0.0f, 0.0f,-1.0f,   0.0f, 1.0f, //top left
		-0.53f,  0.2f,  -1.13f,  0.0f, 0.0f,-1.0f,   0.0f, 0.0f, //bottom left
		 0.53f,  0.22f, -1.13f,  0.0f, 0.0f,-1.0f,  1.0f, 1.0f, //top right


		 //right
		 0.53f,  0.22f, -1.13f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top right
		 0.53f,  0.2f,  -1.13f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //bottom left
		 0.53f,  0.2f,  -0.97f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, //bottom right 

		 0.53f,  0.22f, -0.97f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, //top left
		 0.53f,  0.2f,  -0.97f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //bottom left
		 0.53f,  0.22f, -1.13f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top right


		 //left
		-0.53f,  0.22f, -1.13f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top right
		-0.53f,  0.2f,  -1.13f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //bottom left
		-0.53f,  0.2f,  -0.97f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, //bottom right 

		-0.53f,  0.22f, -0.97f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, //top left
		-0.53f,  0.2f,  -0.97f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //bottom left
		-0.53f,  0.22f, -1.13f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top right


		//top
		-0.53f,  0.22f, -1.13f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f, //top left
		-0.53f,  0.22f, -0.97f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, //bottom left
		 0.53f,  0.22f, -0.97f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //top right

		 0.53f,  0.22f, -1.13f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f, //top left
		-0.53f,  0.22f, -1.13f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f, //bottom left
		 0.53f,  0.22f, -0.97f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //top right


		 //bottom
		-0.53f,  0.2f,  -1.13f,  0.0f,-1.0f, 0.0f,   1.0f, 1.0f, //top left
		-0.53f,  0.2f,  -0.97f,  0.0f,-1.0f, 0.0f,   0.0f, 1.0f, //bottom left
		 0.53f,  0.2f,  -0.97f,  0.0f,-1.0f, 0.0f,   0.0f, 0.0f, //top right

		 0.53f,  0.2f,  -1.13f,  0.0f,-1.0f, 0.0f,   0.0f, 1.0f, //top left
		-0.53f,  0.2f,  -1.13f,  0.0f,-1.0f, 0.0f,   0.0f, 0.0f, //bottom left
		 0.53f,  0.2f,  -0.97f,  0.0f,-1.0f, 0.0f,   1.0f, 1.0f, //top right

		 /*******************back top cube********************/


		/*******************right top cube********************/

		//Positions          //Texture Coordinates

		//front
		 0.53f,  0.22f, -0.13f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //top right
		 0.37f,  0.2f,  -0.13f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bottom left
		 0.53f,  0.2f,  -0.13f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, //bottom right 

		 0.37f,  0.22f, -0.13f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, //top left
		 0.37f,  0.2f,  -0.13f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bottom left
		 0.53f,  0.22f, -0.13f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //top right


		 //back
		 0.53f,  0.22f, -0.97f,  0.0f, 0.0f,-1.0f,  1.0f, 1.0f, //top right
		 0.37f,  0.2f,  -0.97f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f, //bottom left
		 0.53f,  0.2f,  -0.97f,  0.0f, 0.0f,-1.0f,  1.0f, 0.0f, //bottom right 

		 0.37f,  0.22f, -0.97f,  0.0f, 0.0f,-1.0f,  0.0f, 1.0f, //top left
		 0.37f,  0.2f,  -0.97f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f, //bottom left
		 0.53f,  0.22f, -0.97f,  0.0f, 0.0f,-1.0f,  1.0f, 1.0f, //top right


		 //right
		 0.53f,  0.22f, -0.97f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top right
		 0.53f,  0.2f,  -0.97f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //bottom left
		 0.53f,  0.2f,  -0.13f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, //bottom right 

		 0.53f,  0.22f, -0.13f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, //top left
		 0.53f,  0.2f,  -0.13f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //bottom left
		 0.53f,  0.22f, -0.97f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top right


		 //left
		 0.37f,  0.22f, -0.97f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top right
		 0.37f,  0.2f,  -0.97f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //bottom left
		 0.37f,  0.2f,  -0.13f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, //bottom right 

		 0.37f,  0.22f, -0.13f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, //top left
		 0.37f,  0.2f,  -0.13f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //bottom left
		 0.37f,  0.22f, -0.97f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top right


		//top
		 0.37f,  0.22f, -0.97f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f, //top left
		 0.37f,  0.22f, -0.13f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, //bottom left
		 0.53f,  0.22f, -0.13f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //top right

		 0.53f,  0.22f, -0.97f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f, //top left
		 0.37f,  0.22f, -0.97f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f, //bottom left
		 0.53f,  0.22f, -0.13f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //top right


		 //bottom
		 0.37f,  0.2f,  -0.97f,  0.0f,-1.0f, 0.0f,   1.0f, 1.0f, //top left
		 0.37f,  0.2f,  -0.13f,  0.0f,-1.0f, 0.0f,   0.0f, 1.0f, //bottom left
		 0.53f,  0.2f,  -0.13f,  0.0f,-1.0f, 0.0f,   0.0f, 0.0f, //top right

		 0.53f,  0.2f,  -0.97f,  0.0f,-1.0f, 0.0f,   0.0f, 1.0f, //top left
		 0.37f,  0.2f,  -0.97f,  0.0f,-1.0f, 0.0f,   0.0f, 0.0f, //bottom left
		 0.53f,  0.2f,  -0.13f,  0.0f,-1.0f, 0.0f,   1.0f, 1.0f, //top right

		 /*******************right top cube********************/


		/*******************left top cube********************/

		//Positions          //Texture Coordinates

		//front
		-0.37f,  0.22f, -0.13f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //top right
		-0.53f,  0.2f,  -0.13f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bottom left
		 0.53f,  0.2f,  -0.13f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, //bottom right 

		-0.53f,  0.22f, -0.13f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, //top left
		-0.53f,  0.2f,  -0.13f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bottom left
		-0.37f,  0.22f, -0.13f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //top right


		 //back
		-0.37f,  0.22f, -0.97f,  0.0f, 0.0f,-1.0f,  1.0f, 1.0f, //top right
		-0.53f,  0.2f,  -0.97f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f, //bottom left
		-0.37f,  0.2f,  -0.97f,  0.0f, 0.0f,-1.0f,  1.0f, 0.0f, //bottom right 

		-0.53f,  0.22f, -0.97f,  0.0f, 0.0f,-1.0f,  0.0f, 1.0f, //top left
		-0.53f,  0.2f,  -0.97f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f, //bottom left
		-0.37f,  0.22f, -0.97f,  0.0f, 0.0f,-1.0f,  1.0f, 1.0f, //top right


		 //right
		-0.37f,  0.22f, -0.97f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top right
		-0.37f,  0.2f,  -0.97f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //bottom left
		-0.37f,  0.2f,  -0.13f,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f, //bottom right 

		-0.37f,  0.22f, -0.13f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f, //top left
		-0.37f,  0.2f,  -0.13f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //bottom left
		-0.37f,  0.22f, -0.97f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top right


		 //left
		-0.53f,  0.22f, -0.97f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top right
		-0.53f,  0.2f,  -0.97f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //bottom left
		-0.53f,  0.2f,  -0.13f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, //bottom right 

		-0.53f,  0.22f, -0.13f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, //top left
		-0.53f,  0.2f,  -0.13f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //bottom left
		-0.53f,  0.22f, -0.97f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top right


		//top
		-0.53f,  0.22f, -0.97f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, //top left
		-0.53f,  0.22f, -0.13f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, //bottom left
		-0.37f,  0.22f, -0.13f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, //top right

		-0.37f,  0.22f, -0.97f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f, //top left
		-0.53f,  0.22f, -0.97f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, //bottom left
		-0.37f,  0.22f, -0.13f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, //top right


		 //bottom
		-0.53f,  0.2f,  -0.97f,   0.0f,-1.0f, 0.0f,  1.0f, 1.0f, //top left
		-0.53f,  0.2f,  -0.13f,   0.0f,-1.0f, 0.0f,  0.0f, 1.0f, //bottom left
		-0.37f,  0.2f,  -0.13f,   0.0f,-1.0f, 0.0f,  0.0f, 0.0f, //top right

		-0.37f,  0.2f,  -0.97f,   0.0f,-1.0f, 0.0f,  0.0f, 1.0f, //top left
		-0.53f,  0.2f,  -0.97f,   0.0f,-1.0f, 0.0f,  0.0f, 0.0f, //bottom left
		-0.37f,  0.2f,  -0.13f,   0.0f,-1.0f, 0.0f,  1.0f, 1.0f, //top right

		 /*******************left top cube********************/
	};

	GLfloat verts3[] = {
	
		/*************water****************/
		-0.5f,  0.04f, -1.0f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, //top left
		-0.5f,  0.04f, -0.1f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, //bottom left
		 0.5f,  0.04f, -0.1f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, //top right

		 0.5f,  0.04f, -1.0f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f, //top left
		-0.5f,  0.04f, -1.0f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, //bottom left
		 0.5f,  0.04f, -0.1f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, //top right

		/*************water****************/

	};

	GLfloat scenePlane[] = {

		/******************scene plane********************/

		-7.0f, -0.11f, -8.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, //top left
		-7.0f, -0.11f,  1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, //bottom left
		 1.0f, -0.11f,  1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, //top right

		 1.0f, -0.11f, -8.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, //top left
		-7.0f, -0.11f, -8.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, //bottom left
		 1.0f, -0.11f,  1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, //top right

		 /******************scene plane********************/

	};

	GLfloat house[] = {

		/*******************front cube********************/

		//Positions    //negative z normal      //Texture Coordinates

		//front
		 2.5f,  3.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //top right
		-1.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //bottom left
		 2.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, //bottom right 

		-1.5f,  3.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, //top left
		-1.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //bottom left
		 2.5f,  3.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //top right


		 //back
		 2.5f,  3.0f, -3.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, //top right
		-1.5f,  0.5f, -3.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, //bottom left
		 2.5f,  0.5f, -3.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f, //bottom right 

		-1.5f,  3.0f, -3.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f, //top left
		-1.5f,  0.5f, -3.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, //bottom left
		 2.5f,  3.0f, -3.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, //top right


		 //right
		 2.5f,  3.0f, -3.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right
		 2.5f,  0.5f, -3.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f, //bottom left
		 2.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom right 

		 2.5f,  3.0f,  1.0f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f, //top left
		 2.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 2.5f,  3.0f, -3.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right


		 //left
		-1.5f,  3.0f, -3.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right
		-1.5f,  0.5f, -3.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, //bottom left
		-1.5f,  0.5f,  1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom right 

		-1.5f,  3.0f,  1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, //top left
		-1.5f,  0.5f,  1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		-1.5f,  3.0f, -3.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right


		//top
		-1.5f,  3.0f, -3.5f,   0.0f, 1.0f,  0.0f,   0.0f, 1.0f, //top left
		-1.5f,  3.0f,  1.0f,   0.0f, 1.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 2.5f,  3.0f,  1.0f,   0.0f, 1.0f,  0.0f,   1.0f, 0.0f, //top right

		 2.5f,  3.0f, -3.5f,   0.0f, 1.0f,  0.0f,   1.0f, 1.0f, //top left
		-1.5f,  3.0f, -3.5f,   0.0f, 1.0f,  0.0f,   0.0f, 1.0f, //bottom left
		 2.5f,  3.0f,  1.0f,   0.0f, 1.0f,  0.0f,   1.0f, 0.0f, //top right


		 //bottom
		-1.5f,  0.5f, -3.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f, //top left
		-1.5f,  0.5f,  1.0f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 2.5f,  0.5f,  1.0f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f, //top right

		 2.5f,  0.5f, -3.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, //top left
		-1.5f,  0.5f, -3.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f, //bottom left
		 2.5f,  0.5f,  1.0f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f, //top right

		 /*******************front cube********************/

	};

	GLfloat roofSides[] = {

		/******************roof************************/
		//left side
		 0.5f, 4.0f, 0.9f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f,
		-1.7f, 3.0f, 0.9f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		 3.3f, 3.0f, 0.9f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

		 //right side
		 0.5f, 4.0f, -3.4f,  0.0f, 0.0f,-1.0f,  0.5f, 1.0f,
		-1.7f, 3.0f, -3.4f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,
		 3.3f, 3.0f, -3.4f,  0.0f, 0.0f,-1.0f,  1.0f, 0.0f,
	
	};

	GLfloat roofMain[] = {

		/******************main roof*************************/
		//front
		0.5f, 4.0f, 1.12f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		3.3f, 3.0f, 1.1f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 
		0.5f, 4.0f,-3.7f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 

		0.5f, 4.0f,-3.7f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
		3.3f, 3.0f,-3.6f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 
		3.3f, 3.0f, 1.1f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

		//back 
		0.5f, 4.0f, 1.12f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	   -1.7f, 3.0f, 1.1f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		0.5f, 4.0f,-3.7f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,

		0.5f, 4.0f,-3.7f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	   -1.7f, 3.0f,-3.6f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	   -1.7f, 3.0f, 1.1f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

	   //bottom
	    3.3f, 3.0f, 1.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		3.3f, 3.0f,-3.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 
	   -1.7f, 3.0f,-3.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f, 

	    3.3f, 3.0f, 1.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 
	   -1.7f, 3.0f, 1.0f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f, 
	   -1.7f, 3.0f,-3.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,

	};

	GLfloat pillars[] = {
	
		/***************pillars****************/
		//front
		 0.0f,  3.0f,  0.6f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //top right
		-0.1f,  0.6f,  0.6f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //bottom left
		 0.0f,  0.6f,  0.6f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, //bottom right 

		-0.1f,  3.0f,  0.6f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, //top left
		-0.1f,  0.6f,  0.6f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //bottom left
		 0.0f,  3.0f,  0.6f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //top right


		 //back
		 0.0f,  3.0f,  0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, //top right
		-0.1f,  0.6f,  0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, //bottom left
		 0.0f,  0.6f,  0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f, //bottom right 

		-0.1f,  3.0f,  0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f, //top left
		-0.1f,  0.6f,  0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, //bottom left
		 0.0f,  3.0f,  0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, //top right


		 //right
		 0.0f,  3.0f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right
		 0.0f,  0.6f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f, //bottom left
		 0.0f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom right 

		 0.0f,  3.0f,  0.6f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f, //top left
		 0.0f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.0f,  3.0f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right


		 //left
		-0.1f,  3.0f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right
		-0.1f,  0.6f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, //bottom left
		-0.1f,  0.6f,  0.6f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom right 

		-0.1f,  3.0f,  0.6f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, //top left
		-0.1f,  0.6f,  0.6f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		-0.1f,  3.0f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right
	
	};

	GLfloat porch[] = {
	
		//front
		 0.4f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //top right
		-0.5f, -0.1f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //bottom left
		 0.4f, -0.1f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, //bottom right 

		-0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, //top left
		-0.5f, -0.1f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //bottom left
		 0.4f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //top right


		 //back
		 0.4f,  0.0f, -3.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, //top right
		-0.5f, -0.1f, -3.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, //bottom left
		 0.4f, -0.1f, -3.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f, //bottom right 

		-0.5f,  0.0f, -3.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f, //top left
		-0.5f, -0.1f, -3.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, //bottom left
		 0.4f,  0.0f, -3.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, //top right


		 //right
		 0.4f,  0.0f, -3.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right
		 0.4f, -0.1f, -3.5f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.4f, -0.1f,  1.0f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f, //bottom right 

		 0.4f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f, //top left
		 0.4f, -0.1f,  1.0f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.4f,  0.0f, -3.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right


		 //left
		-0.5f,  0.0f, -3.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right
		-0.5f, -0.1f, -3.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		-0.5f, -0.1f,  1.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, //bottom right 

		-0.5f,  0.0f,  1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, //top left
		-0.5f, -0.1f,  1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		-0.5f,  0.0f, -3.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right


		//top
		-0.5f,  0.0f, -3.5f,   0.0f, 1.0f,  0.0f,   1.0f, 1.0f, //top left
		-0.5f,  0.0f,  1.0f,   0.0f, 1.0f,  0.0f,   0.0f, 1.0f, //bottom left
		 0.4f,  0.0f,  1.0f,   0.0f, 1.0f,  0.0f,   0.0f, 0.0f, //top right

		 0.4f,  0.0f, -3.5f,   0.0f, 1.0f,  0.0f,   1.0f, 0.0f, //top left
		-0.5f,  0.0f, -3.5f,   0.0f, 1.0f,  0.0f,   1.0f, 1.0f, //bottom left
		 0.4f,  0.0f,  1.0f,   0.0f, 1.0f,  0.0f,   0.0f, 0.0f, //top right


		 //bottom
		-0.5f, -0.1f, -3.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, //top left
		-0.5f, -0.1f,  1.0f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f, //bottom left
		 0.4f, -0.1f,  1.0f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f, //top right

		 0.4f, -0.1f, -3.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f, //top left
		-0.5f, -0.1f, -3.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.4f, -0.1f,  1.0f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, //top right
	
	};

	GLfloat cylinder[] = {

		//top front
		0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.1f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		0.1f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

		0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		0.1f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 
		0.3f, 1.0f,-0.1f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

		0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.1f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	   -0.3f, 1.0f,-0.1f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

	   //left front middle
	    0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.43f, 1.0f,-0.23f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	   -0.3f, 1.0f,-0.1f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

		//back
		0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.1f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		0.1f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

		0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 
		0.1f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 
		0.3f, 1.0f,-0.9f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

		0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.1f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	   -0.3f, 1.0f,-0.9f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

	   //left back middle
	    0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 
	   -0.43f, 1.0f,-0.77f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	   -0.3f, 1.0f,-0.9f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

		//right
		0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	    0.5f, 1.0f,-0.6f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		0.5f, 1.0f,-0.4f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

		0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 
		0.5f, 1.0f,-0.4f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 
		0.43f, 1.0f,-0.23f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 

		0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		0.5f, 1.0f,-0.6f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		0.43f, 1.0f,-0.77f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

		//right back middle
		0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 
	    0.3f, 1.0f,-0.9f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	    0.43f, 1.0f,-0.77f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

		//left
		0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.5f, 1.0f,-0.6f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	   -0.5f, 1.0f,-0.4f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

	    0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.5f, 1.0f,-0.4f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
       -0.43f, 1.0f,-0.23f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

	    0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.5f, 1.0f,-0.6f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.43f, 1.0f,-0.77f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

	   //right front middle
	    0.0f, 1.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 
	    0.3f, 1.0f,-0.1f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
	    0.43f, 1.0f,-0.23f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

		/*************************************************/
		//bottom front
		0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.1f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		0.1f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

		0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		0.1f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		0.3f, 0.0f,-0.1f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

		0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.1f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	   -0.3f, 0.0f,-0.1f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

	   //left front middle
		0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.43f, 0.0f,-0.23f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	   -0.3f, 0.0f,-0.1f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

	   //back
	    0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.1f, 0.0f,-1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	    0.1f, 0.0f,-1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

	    0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	    0.1f, 0.0f,-1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	    0.3f, 0.0f,-0.9f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

	    0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.1f, 0.0f,-1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	   -0.3f, 0.0f,-0.9f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

	  //left back middle
	    0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   -0.43f, 0.0f,-0.77f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	   -0.3f, 0.0f,-0.9f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

	  //right
	   0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   0.5f, 0.0f,-0.6f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	   0.5f, 0.0f,-0.4f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

	   0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   0.5f, 0.0f,-0.4f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   0.43f, 0.0f,-0.23f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

	   0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   0.5f, 0.0f,-0.6f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   0.43f, 0.0f,-0.77f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

	  //right back middle
	   0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   0.3f, 0.0f,-0.9f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	   0.43f, 0.0f,-0.77f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

	  //left
	   0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	  -0.5f, 0.0f,-0.6f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	  -0.5f, 0.0f,-0.4f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

	   0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	  -0.5f, 0.0f,-0.4f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	  -0.43f, 0.0f,-0.23f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

	   0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	  -0.5f, 0.0f,-0.6f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	  -0.43f, 0.0f,-0.77f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

	 //right front middle
	   0.0f, 0.0f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	   0.3f, 0.0f,-0.1f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
	   0.43f, 0.0f,-0.23f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		

		//face front
	   0.1f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.1f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.1f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	   0.1f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.1f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.1f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

		//everything between front and right
	   0.1f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.1f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.3f, 0.0f,-0.1f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	   0.1f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.3f, 1.0f,-0.1f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.3f, 0.0f,-0.1f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


	   0.43f, 1.0f,-0.23f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.43f, 0.0f,-0.23f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.3f, 0.0f,-0.1f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	   0.43f, 1.0f,-0.23f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.3f, 1.0f,-0.1f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.3f, 0.0f,-0.1f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


	   0.43f, 1.0f,-0.23f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.43f, 0.0f,-0.23f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.5f, 0.0f,-0.4f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	   0.43f, 1.0f,-0.23f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.5f, 1.0f,-0.4f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.5f, 0.0f,-0.4f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


		//everything between front and left
	  -0.1f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.1f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.3f, 0.0f,-0.1f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	  -0.1f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.3f, 1.0f,-0.1f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.3f, 0.0f,-0.1f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


	  -0.43f, 1.0f,-0.23f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.43f, 0.0f,-0.23f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.3f, 0.0f,-0.1f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	  -0.43f, 1.0f,-0.23f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.3f, 1.0f,-0.1f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.3f, 0.0f,-0.1f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


	  -0.43f, 1.0f,-0.23f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.43f, 0.0f,-0.23f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.5f, 0.0f,-0.4f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	  -0.43f, 1.0f,-0.23f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.5f, 1.0f,-0.4f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.5f, 0.0f,-0.4f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


		//face back
	   0.1f, 1.0f,-1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.1f, 0.0f,-1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.1f, 0.0f,-1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	   0.1f, 1.0f,-1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.1f, 1.0f,-1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.1f, 0.0f,-1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


		   //everything between back and left
	  -0.3f, 1.0f,-0.9f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.3f, 0.0f,-0.9f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.1f, 0.0f,-1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	  -0.3f, 1.0f,-0.9f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.1f, 1.0f,-1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.1f, 0.0f,-1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


	  -0.43f, 1.0f,-0.77f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.43f, 0.0f,-0.77f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.3f, 0.0f,-0.9f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	  -0.43f, 1.0f,-0.77f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.3f, 1.0f,-0.9f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.3f, 0.0f,-0.9f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


	  -0.43f, 1.0f,-0.77f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.43f, 0.0f,-0.77f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.5f, 0.0f,-0.6f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	  -0.43f, 1.0f,-0.77f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.5f, 1.0f,-0.6f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.5f, 0.0f,-0.6f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


	//everything between back and right
	   0.3f, 1.0f,-0.9f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.3f, 0.0f,-0.9f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.1f, 0.0f,-1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	   0.3f, 1.0f,-0.9f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.1f, 1.0f,-1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.1f, 0.0f,-1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


	   0.43f, 1.0f,-0.77f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.43f, 0.0f,-0.77f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.3f, 0.0f,-0.9f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	   0.43f, 1.0f,-0.77f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.3f, 1.0f,-0.9f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.3f, 0.0f,-0.9f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


	   0.43f, 1.0f,-0.77f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.43f, 0.0f,-0.77f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.5f, 0.0f,-0.6f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	   0.43f, 1.0f,-0.77f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.5f, 1.0f,-0.6f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.5f, 0.0f,-0.6f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,


      //face left
	  -0.5f, 1.0f,-0.4f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.5f, 0.0f,-0.4f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.5f, 0.0f,-0.6f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	  -0.5f, 1.0f,-0.4f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	  -0.5f, 1.0f,-0.6f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	  -0.5f, 0.0f,-0.6f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	   //face right
	   0.5f, 1.0f,-0.4f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.5f, 0.0f,-0.4f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.5f, 0.0f,-0.6f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	   0.5f, 1.0f,-0.4f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	   0.5f, 1.0f,-0.6f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	   0.5f, 0.0f,-0.6f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
	
	};

	GLfloat cube[] = {
		
		/*******************cube********************/

		//Positions    //negative z normal      //Texture Coordinates

		//front
		 0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //top right
		 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //bottom left
		 0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, //bottom right 

		 0.0f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, //top left
		 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //bottom left
		 0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //top right


		 //back
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, //top right
		 0.0f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f, //bottom right 

		 0.0f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f, //top left
		 0.0f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, //top right


		 //right
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right
		 0.5f,  0.0f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f, //bottom right 

		 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f, //top left
		 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right


		 //left
		 0.0f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right
		 0.0f,  0.0f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.0f,  0.0f,  0.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, //bottom right 

		 0.0f,  0.5f,  0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, //top left
		 0.0f,  0.0f,  0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.0f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, //top right


		//top
		 0.0f,  0.5f, -0.5f,   0.0f, 1.0f,  0.0f,   1.0f, 1.0f, //top left
		 0.0f,  0.5f,  0.0f,   0.0f, 1.0f,  0.0f,   0.0f, 1.0f, //bottom left
		 0.5f,  0.5f,  0.0f,   0.0f, 1.0f,  0.0f,   0.0f, 0.0f, //top right

		 0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  0.0f,   0.0f, 1.0f, //top left
		 0.0f,  0.5f, -0.5f,   0.0f, 1.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.5f,  0.0f,   0.0f, 1.0f,  0.0f,   1.0f, 1.0f, //top right


		 //bottom
		 0.0f,  0.0f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, //top left
		 0.0f,  0.0f,  0.0f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f, //bottom left
		 0.5f,  0.0f,  0.0f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f, //top right

		 0.5f,  0.0f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f, //top left
		 0.0f,  0.0f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f, //bottom left
		 0.5f,  0.0f,  0.0f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, //top right

		 /*******************cube********************/
	
	};

	GLfloat pyramid[] = {
	
		//Back Face          //Negative Z Normal  Texture Coords.
	   0.0f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.5f, 1.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,

	   //Front Face         //Positive Z Normal
	   0.0f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.5f, 1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
	   

	  //Left Face          //Negative X Normal
	   0.0f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.5f, 1.0f,
	  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 //Right Face         //Positive X Normal
	   0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f, 1.0f,
	   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,


	 //Bottom Face        //Negative Y Normal
	  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f
	
	};

	GLfloat slope[] = {
	
		/**********slope****************/
		//Back Face          //Negative Z Normal  Texture Coords.
	  -0.5f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.5f, 1.0f,
	  -0.5f, -0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	   0.5f, -0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,

	   0.5f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.5f, 1.0f,
	   0.5f, -0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	  -0.5f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,

	   //Front Face         //Positive Z Normal
	  -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.5f, 1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,

	   0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.5f, 1.0f,
	   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
	  -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
	   

	  //Left Face          //Negative X Normal
	  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.5f, 1.0f,
	  -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 //Right Face         //Positive X Normal
	   0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f, 1.0f,
	   0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,


	 //Bottom Face        //Negative Y Normal
	  -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	   0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

       0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f


	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.nVertices2 = sizeof(verts2) / (sizeof(verts2[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.nVertices3 = sizeof(verts3) / (sizeof(verts3[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.planeVerts = sizeof(scenePlane) / (sizeof(scenePlane[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.houseVerts = sizeof(house) / (sizeof(house[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.roofVerts = sizeof(roofSides) / (sizeof(roofSides[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.roofMVerts = sizeof(roofMain) / (sizeof(roofMain[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.pillarVerts = sizeof(pillars) / (sizeof(pillars[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.porchVerts = sizeof(porch) / (sizeof(porch[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.cylinderVerts = sizeof(cylinder) / (sizeof(cylinder[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.cubeVerts = sizeof(cube) / (sizeof(cube[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.pyramidVerts = sizeof(pyramid) / (sizeof(pyramid[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.slopeVerts = sizeof(slope) / (sizeof(slope[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	glGenVertexArrays(1, &mesh.vertexObject); {
		glBindVertexArray(mesh.vertexObject);

		//buffers for the vertex data and indices
		glGenBuffers(1, mesh.vertexBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);
	}
	//FIXME:: verts2 draw 

	glGenVertexArrays(1, &mesh.vertexObject2); {
		glBindVertexArray(mesh.vertexObject2);

		glGenBuffers(1, mesh.vertexBuffer2);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer2[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts2), verts2, GL_STATIC_DRAW);

		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);
	}

	//FIXME:: verts3 draw 
	
	glGenVertexArrays(1, &mesh.vertexObject3); {
		glBindVertexArray(mesh.vertexObject3);

		glGenBuffers(1, mesh.vertexBuffer3);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer3[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts3), verts3, GL_STATIC_DRAW);

		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);
	}

	//scene plane
	glGenVertexArrays(1, &mesh.planeVO); {
		glBindVertexArray(mesh.planeVO);

		glGenBuffers(1, mesh.planeVB);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.planeVB[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(scenePlane), scenePlane, GL_STATIC_DRAW);

		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);
	}

	//house
	glGenVertexArrays(1, &mesh.houseVO); {
		glBindVertexArray(mesh.houseVO);

		glGenBuffers(1, mesh.houseVB);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.houseVB[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(house), house, GL_STATIC_DRAW);

		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);
	}

	//roof
	glGenVertexArrays(1, &mesh.roofVO); {
		glBindVertexArray(mesh.roofVO);

		glGenBuffers(1, mesh.roofVB);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.roofVB[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(roofSides), roofSides, GL_STATIC_DRAW);

		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);
	}

	//roofmain
	glGenVertexArrays(1, &mesh.roofMVO); {
		glBindVertexArray(mesh.roofMVO);

		glGenBuffers(1, mesh.roofMVB);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.roofMVB[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(roofMain), roofMain, GL_STATIC_DRAW);

		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);
	}

	//pillars
	glGenVertexArrays(1, &mesh.pillarVO); {
		glBindVertexArray(mesh.pillarVO);

		glGenBuffers(1, mesh.pillarVB);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.pillarVB[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pillars), pillars, GL_STATIC_DRAW);

		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);
	}

	//porch
	glGenVertexArrays(1, &mesh.porchVO); {
		glBindVertexArray(mesh.porchVO);

		glGenBuffers(1, mesh.porchVB);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.porchVB[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(porch), porch, GL_STATIC_DRAW);

		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);
	}

	//cylinder
	glGenVertexArrays(1, &mesh.cylinderVO); {
		glBindVertexArray(mesh.cylinderVO);

		glGenBuffers(1, mesh.cylinderVB);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.cylinderVB[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder), cylinder, GL_STATIC_DRAW);

		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);
	}

	//cube
	glGenVertexArrays(1, &mesh.cubeVO); {
		glBindVertexArray(mesh.cubeVO);

		glGenBuffers(1, mesh.cubeVB);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.cubeVB[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);
	}

	//pyramid
	glGenVertexArrays(1, &mesh.pyramidVO); {
		glBindVertexArray(mesh.pyramidVO);

		glGenBuffers(1, mesh.pyramidVB);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.pyramidVB[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid), pyramid, GL_STATIC_DRAW);

		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);
	}

	//slope
	glGenVertexArrays(1, &mesh.slopeVO); {
		glBindVertexArray(mesh.slopeVO);

		glGenBuffers(1, mesh.slopeVB);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.slopeVB[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(slope), slope, GL_STATIC_DRAW);

		glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
		glEnableVertexAttribArray(2);
	}
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


void DestroyOBJ(Mesh &mesh) {

	glDeleteVertexArrays(1, &mesh.vertexObject);
	glDeleteBuffers(2, mesh.vertexBuffer);

	//FIXME::DELETE
	glDeleteVertexArrays(1, &mesh.vertexObject2);
	glDeleteBuffers(2, mesh.vertexBuffer2);

	glDeleteVertexArrays(1, &mesh.vertexObject3);
	glDeleteBuffers(2, mesh.vertexBuffer3);
}

/*Generate and load the texture*/
bool UCreateTexture(const char* filename, GLuint &textureId)
{
	int width, height, channels;
	unsigned char *image = stbi_load(filename, &width, &height, &channels, 0);
	if (image)
	{
		flipImageVertically(image, width, height, channels);

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			cout << "Not implemented to handle image with " << channels << " channels" << endl;
			return false;
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		return true;
	}

	// Error loading the image
	return false;
}


void UDestroyTexture(GLuint textureId)
{
	glGenTextures(1, &textureId);
}

bool CreateShader(const char* vtxShaderSource, const char* fragShaderSource, GLuint &programId) {

	int success = 0;
	char infoLog[512];

	//object created for shader 
	programId = glCreateProgram();

	//vertex and fragment shader objects
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
	glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

	//vertex shader is compiled and errors are reported
	glCompileShader(vertexShaderId);
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

	if (!success) {


		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "VERTEX SHADER COMPILATION FAILED::\n" << infoLog << std::endl;

		return false;
	}

	//fragment shader is compiled and checked for errors 
	glCompileShader(fragmentShaderId);
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

	if (!success) {


		glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
		std::cout << "FRAGMENT SHADER COMPILATION FAILED::\n" << infoLog << std::endl;

		return false;
	}

	//shader program must be linked and checked for errors
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &success);

	if (!success) {


		glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
		std::cout << "PROGRAM LINKING ERROR::\n" << infoLog << std::endl;

		return false;
	}

	glUseProgram(programId);
	return true;
}


void DestroyShader(GLuint programId) {

	glDeleteProgram(programId);
}



/* Sources:

	https://learnopengl.com/Getting-started/Camera for the camera functionality
	https://learnopengl.com/Advanced-OpenGL/Blending for the functions to enable alpha
	CS-330-master document was referenced for the shader functionality
*/