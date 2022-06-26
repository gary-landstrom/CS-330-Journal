#include "Header.h"

int main(int argc, char* argv[])
{
	if (!Init(argc, argv, &glfwWindow)) {

		return EXIT_FAILURE;
	}

	float currFrame = static_cast<float>(glfwGetTime());
	deltaTime = currFrame - lastFrame;
	lastFrame = currFrame;

	//mesh is created
	CreateOBJ(glfwMesh);
	CreateOBJ(glfwMesh2);
	CreateOBJ(glfwMesh3);
	CreateOBJ(scenePlane);
	CreateOBJ(houseMain);
	CreateOBJ(roofMesh);
	CreateOBJ(roofMain);
	CreateOBJ(pillar);
	CreateOBJ(porchMesh);
	CreateOBJ(cylinderMesh);
	CreateOBJ(cubeMesh);
	CreateOBJ(pyramidMesh);
	CreateOBJ(slopeMesh);

	//shader is created 
	if (!CreateShader(vShaderSource, fragmentShaderSource, glfwProgram)) {

		return EXIT_FAILURE;
	}

	//shader is created 
	if (!CreateShader(vShaderSource, fragmentShaderSource, glfwProgram2)) {

		return EXIT_FAILURE;
	}

	//shader is created 
	if (!CreateShader(vShaderSource, fragmentShaderSource, glfwProgram3)) {

		return EXIT_FAILURE;
	}

	//shader is created 
	if (!CreateShader(vShaderSource, fragmentShaderSource, sceneProgram)) {

		return EXIT_FAILURE;
	}

	//shader is created 
	if (!CreateShader(vShaderSource, fragmentShaderSource, houseProgram)) {

		return EXIT_FAILURE;
	}

	//shader is created 
	if (!CreateShader(vShaderSource, fragmentShaderSource, roofProgram)) {

		return EXIT_FAILURE;
	}

	//shader is created 
	if (!CreateShader(vShaderSource, fragmentShaderSource, roofSideProg)) {

		return EXIT_FAILURE;
	}

	//shader is created 
	if (!CreateShader(vShaderSource, fragmentShaderSource, pillarProg)) {

		return EXIT_FAILURE;
	}

	//porch mesh is passed to shader
	if (!CreateShader(vShaderSource, fragmentShaderSource, porchProg)) {

		return EXIT_FAILURE;
	}

	if (!CreateShader(vShaderSource, fragmentShaderSource, cylinderProg)) {

		return EXIT_FAILURE;
	}

	if (!CreateShader(vShaderSource, fragmentShaderSource, cubeProgram)) {

		return EXIT_FAILURE;
	}

	if (!CreateShader(vShaderSource, fragmentShaderSource, pyramidProg)) {

		return EXIT_FAILURE;
	}

	if (!CreateShader(vShaderSource, fragmentShaderSource, slopeProgram)) {

		return EXIT_FAILURE;
	}


	// Load texture
	const char * texFilename = "C:/Users/garyg/source/repos/Project-3D-Scene-C/textures/wall3.png";
	if (!UCreateTexture(texFilename, gTextureId))
	{
		cout << "Failed to load texture " << texFilename << endl;
		return EXIT_FAILURE;
	}
	//tell opengl for each sampler to which texture unit it belongs to
	glUseProgram(glfwProgram);
	//set the texture as texture unit 0
	glUniform1i(glGetUniformLocation(glfwProgram, "uTexture"), 0);

	//FIXME:: Not sure about this texture call
	// Load texture 2
	const char * texFilename2 = "C:/Users/garyg/source/repos/Project-3D-Scene-C/textures/wood.png";
	if (!UCreateTexture(texFilename2, gTextureId2))
	{
		cout << "Failed to load texture " << texFilename2 << endl;
		return EXIT_FAILURE;
	}
	//tell opengl for each sampler to which texture unit it belongs to 
	glUseProgram(glfwProgram2);
	//set the texture as texture unit 0
	glUniform1i(glGetUniformLocation(glfwProgram2, "uTexture"), 1);
	//FIXME:: Not sure about this texture call

	//FIXME:: Not sure about this texture call
	// Load texture 3
	const char * texFilename3 = "C:/Users/garyg/source/repos/Project-3D-Scene-C/textures/water.png";
	if (!UCreateTexture(texFilename3, gTextureId3))
	{
		cout << "Failed to load texture " << texFilename3 << endl;
		return EXIT_FAILURE;
	}
	//tell opengl for each sampler to which texture unit it belongs to 
	glUseProgram(glfwProgram3);
	//set the texture as texture unit 2
	glUniform1i(glGetUniformLocation(glfwProgram3, "uTexture"), 2);
	//FIXME:: Not sure about this texture call


	//FIXME:: Not sure about this texture call
	// Load texture 3
	const char * texFilename4 = "C:/Users/garyg/source/repos/Project-3D-Scene-C/textures/grid.png";
	if (!UCreateTexture(texFilename4, gTextureId4))
	{
		cout << "Failed to load texture " << texFilename4 << endl;
		return EXIT_FAILURE;
	}
	//tell opengl for each sampler to which texture unit it belongs to 
	glUseProgram(sceneProgram);
	//set the texture as texture unit 3
	glUniform1i(glGetUniformLocation(sceneProgram, "uTexture"), 3);
	//FIXME:: Not sure about this texture call


	//FIXME:: Not sure about this texture call
	// Load texture 4
	const char * texFilename5 = "C:/Users/garyg/source/repos/Project-3D-Scene-C/textures/test2.png";
	if (!UCreateTexture(texFilename5, gHouseTexId))
	{
		cout << "Failed to load texture " << texFilename5 << endl;
		return EXIT_FAILURE;
	}
	//tell opengl for each sampler to which texture unit it belongs to 
	glUseProgram(houseProgram);
	//set the texture as texture unit 4
	glUniform1i(glGetUniformLocation(houseProgram, "uTexture"), 4);

	// Load texture 5
	const char * texFilename6 = "C:/Users/garyg/source/repos/Project-3D-Scene-C/textures/roof.png";
	if (!UCreateTexture(texFilename6, gRoofTexId))
	{
		cout << "Failed to load texture " << texFilename6 << endl;
		return EXIT_FAILURE;
	}
	//tell opengl for each sampler to which texture unit it belongs to 
	glUseProgram(roofProgram);
	//set the texture as texture unit 5
	glUniform1i(glGetUniformLocation(roofProgram, "uTexture"), 5);


	// Load texture 6
	const char * texFilename7 = "C:/Users/garyg/source/repos/Project-3D-Scene-C/textures/sideB.png";
	if (!UCreateTexture(texFilename7, gRoofSideTex))
	{
		cout << "Failed to load texture " << texFilename7 << endl;
		return EXIT_FAILURE;
	}
	//tell opengl for each sampler to which texture unit it belongs to 
	glUseProgram(roofSideProg);
	//set the texture as texture unit 5
	glUniform1i(glGetUniformLocation(roofSideProg, "uTexture"), 6);

	// Load texture 7
	const char * texFilename8 = "C:/Users/garyg/source/repos/Project-3D-Scene-C/textures/pillar.png";
	if (!UCreateTexture(texFilename8, gPillarTexId))
	{
		cout << "Failed to load texture " << texFilename8 << endl;
		return EXIT_FAILURE;
	}
	//tell opengl for each sampler to which texture unit it belongs to 
	glUseProgram(pillarProg);
	//set the texture as texture unit 5
	glUniform1i(glGetUniformLocation(pillarProg, "uTexture"), 7);


	// Load texture 8
	const char * texFilename9 = "C:/Users/garyg/source/repos/Project-3D-Scene-C/textures/porch.png";
	if (!UCreateTexture(texFilename9, gPorchTexId))
	{
		cout << "Failed to load texture " << texFilename9 << endl;
		return EXIT_FAILURE;
	}
	//tell opengl for each sampler to which texture unit it belongs to 
	glUseProgram(porchProg);
	//set the texture as texture unit 5
	glUniform1i(glGetUniformLocation(porchProg, "uTexture"), 8);

	// Load texture 9
	const char * texFilename10 = "C:/Users/garyg/source/repos/Project-3D-Scene-C/textures/red.png";
	if (!UCreateTexture(texFilename10, gCylinderTexId))
	{
		cout << "Failed to load texture " << texFilename10 << endl;
		return EXIT_FAILURE;
	}
	//tell opengl for each sampler to which texture unit it belongs to 
	glUseProgram(cylinderProg);
	//set the texture as texture unit 5
	glUniform1i(glGetUniformLocation(cylinderProg, "uTexture"), 9);

	// Load texture 10
	const char * texFilename11 = "C:/Users/garyg/source/repos/Project-3D-Scene-C/textures/red2.png";
	if (!UCreateTexture(texFilename11, gCubeTexId))
	{
		cout << "Failed to load texture " << texFilename11 << endl;
		return EXIT_FAILURE;
	}
	//tell opengl for each sampler to which texture unit it belongs to 
	glUseProgram(cubeProgram);
	//set the texture as texture unit 5
	glUniform1i(glGetUniformLocation(cubeProgram, "uTexture"), 10);

	// Load texture 11
	const char * texFilename12 = "C:/Users/garyg/source/repos/Project-3D-Scene-C/textures/red3.png";
	if (!UCreateTexture(texFilename12, gPyramidTextId))
	{
		cout << "Failed to load texture " << texFilename12 << endl;
		return EXIT_FAILURE;
	}
	//tell opengl for each sampler to which texture unit it belongs to 
	glUseProgram(pyramidProg);
	//set the texture as texture unit 5
	glUniform1i(glGetUniformLocation(pyramidProg, "uTexture"), 11);

	// Load texture 12
	const char * texFilename13 = "C:/Users/garyg/source/repos/Project-3D-Scene-C/textures/red4.png";
	if (!UCreateTexture(texFilename13, gSlopeTextId))
	{
		cout << "Failed to load texture " << texFilename13 << endl;
		return EXIT_FAILURE;
	}
	//tell opengl for each sampler to which texture unit it belongs to 
	glUseProgram(slopeProgram);
	//set the texture as texture unit 5
	glUniform1i(glGetUniformLocation(slopeProgram, "uTexture"), 12);

	//FIXME:: Not sure about this texture call

	//window background color is set 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//rendering loop for program
	while (!glfwWindowShouldClose(glfwWindow)) {

		Input(glfwWindow);
		Render();
		glfwPollEvents();
		changeAngle(glfwWindow);
	}

	//program termination
	DestroyOBJ(glfwMesh);
	UDestroyTexture(gTextureId);
	UDestroyTexture(gTextureId2);
	UDestroyTexture(gTextureId3);

	DestroyShader(glfwProgram);
	DestroyShader(glfwProgram2);
	DestroyShader(glfwProgram3);
	DestroyShader(sceneProgram);
	exit(EXIT_SUCCESS);
}

