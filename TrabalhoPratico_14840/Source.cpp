#include "Header.h"
#include "texture.h"

GLFWwindow* window;

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 2129;
glm::mat4 ModelMatrix, ViewMatrix, ProjectionMatrix;
GLuint	MatrixID,
		textura,
		TextureID,
		ViewMatrixID,
		ModelMatrixID,
		LightID,
		vertexbuffer,
		uvbuffer,
		normalbuffer,
		programa,
		frameCountID,
		VAO;

GLfloat ZOOM = 5.0f;
float anguloX = 0.0f, anguloY = 0.0f;
bool buttonPressed = false;
double xPrevPos, yPrevPos;
static float updateRotX = 0, updateRotY = 0;
float randomCount = 0.0f;

//2 Models so it's easier to apply rotation on X and Y coordinates
glm::mat4 model1 = glm::mat4(1.0f);
glm::mat4 model2 = glm::mat4(1.0f);

std::vector< glm::vec3 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec3 > normals;

//Mtl Properties
glm::vec3 ka, kd, ks;
float ns, ni, d;
int illum;
bool ambLight = true, dirLight = true, pLight = true, cLight = true, changeColor = false;

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {		// Incremento no zoom, varia com a distância da câmara
	if(yoffset == -1)		ZOOM += fabs(ZOOM) * 0.2f;	// Se faz zoom in
	else if (yoffset == 1) ZOOM -= fabs(ZOOM) * 0.2f;	//Senão, se faz zoom out

	std::cout << "ZOOM = " << ZOOM << std::endl;
}

void cursorPosCallback(GLFWwindow *window, double xPos, double yPos) {
	if (buttonPressed) {
		anguloX = (float)(((xPos - xPrevPos) * 180.0) / WIDTH) + updateRotX;
		anguloY = (float)(((yPos - yPrevPos) * 180.0) / HEIGHT)+ updateRotY;
	}
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		if (ambLight) {
			ambLight = false;
			cout << "Ambient Light: OFF" << endl;
		}else {
			ambLight = true;
			cout << "Ambient Light: ON" << endl;
		}
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		if (dirLight) {
			dirLight = false;
			cout << "Directional Light: OFF" << endl;
		}else{
			dirLight = true;
			cout << "Directional Light: ON" << endl;
		}
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		if (pLight){
			pLight = false;
			cout << "Ponctual Light: OFF" << endl;
		}else {
			pLight = true;
			cout << "Ponctual Light: ON" << endl;
		}
	}
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		if (cLight) {
			cLight = false;
			cout << "Cone Light: OFF" << endl;
		}else{
			cLight = true;
			cout << "Cone Light: ON" << endl;
		}
	}
	else if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
		if (changeColor) {
			changeColor = false;
			cout << "Color Effect: OFF" << endl;
		}else {
			changeColor = true;
			cout << "Color Effect: ON" << endl;
		}
	}
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {		//coise
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &xPrevPos, &yPrevPos);

		updateRotX = anguloX;
		updateRotY = anguloY;

		buttonPressed = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		buttonPressed = false;
}

void print_error(int error, const char *description) {
	std::cout << description << std::endl;
}

void print_gl_info() {
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	cout << "\nOpenGL version " << major << '.' << minor << endl;

	const GLubyte *glversion = glGetString(GL_VERSION);
	const GLubyte *glvendor = glGetString(GL_VENDOR);
	const GLubyte *glrenderer = glGetString(GL_RENDERER);
	cout << "\nVersion:  " << glversion << endl <<
		"Vendor:   " << glvendor << endl <<
		"Renderer: " << glrenderer << endl;

	cout << "\nSupported GLSL versions:\n";
	const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	cout << "Higher supported version:\n\t" << glslversion << endl;
	GLint numglslversions;
	cout << "Other supported versions:\n";
	glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &numglslversions);
	for (int n = 0; n < numglslversions; n++) {
		cout << '\t' << glGetStringi(GL_SHADING_LANGUAGE_VERSION, n) << endl;
	}

	// ****************************************************
	// Informações de Uniform
	// ****************************************************

	GLint maxUniformLocations;

	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &maxUniformLocations);

	cout << "\nUniform info:\n";
	// N.º máximo de localizações que o programador pode atribuir a uniforms.
	cout << "\tGL_MAX_UNIFORM_LOCATIONS = " << maxUniformLocations;
	cout << " -> Maximum uniform locations = " << maxUniformLocations << endl;

	// ****************************************************
	// Informações do Vertex Shader
	// ****************************************************

	GLint maxVertexAttribs, maxVertexTextureUnits;
	GLint maxVertexUniformComponents, maxVertexUniformsvec4, maxVertexUniformBlocks;
	GLint maxVertexOutputComponents;

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextureUnits);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniformComponents);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxVertexUniformsvec4);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks);
	glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &maxVertexOutputComponents);

	cout << "\nVertex Shader info:\n";
	// N.º máximo de atributos de vértices genéricos vec4.
	cout << "\tGL_MAX_VERTEX_ATTRIBS = " << maxVertexAttribs;
	cout << " -> Maximum Vertex Shader input locations = " << maxVertexAttribs << endl;
	// N.º máximo de unidades de textura, acessíveis pelo vertex shader.
	cout << "\tGL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = " << maxVertexTextureUnits << endl;
	// N.º máximo de valores (reais, inteiros ou booleanos) que podem ser armazenados numa variável uniform num vertex shader.
	cout << "\tGL_MAX_VERTEX_UNIFORM_COMPONENTS = " << maxVertexUniformComponents << endl;
	// N.º máximo de vec4 que podem ser armazenados numa variável uniform num vertex shader.
	cout << "\tGL_MAX_VERTEX_UNIFORM_VECTORS = " << maxVertexUniformsvec4 << endl;
	// N.º máximo de blocos uniform por vertex shader.
	cout << "\tGL_MAX_VERTEX_UNIFORM_BLOCKS = " << maxVertexUniformBlocks << endl;
	// N.º máximo de componentes de saída de um vertex shader.
	// Por exemplo, um vec3 tem 3 componentes. Um vec4 tem 4 componentes.
	cout << "\tGL_MAX_VERTEX_OUTPUT_COMPONENTS = " << maxVertexOutputComponents;
	cout << " -> Maximum Vertex Shader output locations = " << maxVertexOutputComponents / 4 /*vec4*/ << endl;

	// ****************************************************
	// Informações do Fragment Shader
	// ****************************************************

	GLint maxFragmentInputComponents;
	GLint maxFragmentUniformComponents, maxFragmentUniformsvec4, maxFragmentUniformBlocks;
	GLint maxDrawBuffers;

	glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &maxFragmentInputComponents);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniformComponents);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxFragmentUniformsvec4);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &maxFragmentUniformBlocks);
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);

	cout << "\nFragment Shader info:\n";
	// N.º máximo de componentes de entrada em um fragment shader.
	// Por exemplo, um vec3 tem 3 componentes. Um vec4 tem 4 componentes.
	cout << "\tGL_MAX_FRAGMENT_INPUT_COMPONENTS = " << maxFragmentInputComponents;
	cout << " -> Maximum Fragment Shader input locations = " << maxFragmentInputComponents / 4 /*vec4*/ << endl;
	// N.º máximo de valores (reais, inteiros ou booleanos) que podem ser armazenados numa variável uniform num fragment shader.
	cout << "\tGL_MAX_FRAGMENT_UNIFORM_COMPONENTS = " << maxFragmentUniformComponents << endl;
	// N.º máximo de vec4 que podem ser armazenados numa variável uniform num fragment shader.
	cout << "\tGL_MAX_FRAGMENT_UNIFORM_VECTORS = " << maxFragmentUniformsvec4 << endl;
	// N.º máximo de blocos uniform por fragment shader.
	cout << "\tGL_MAX_FRAGMENT_UNIFORM_BLOCKS = " << maxFragmentUniformBlocks << endl;
	// N.º máximo de saídas de um fragment shader.
	cout << "\tGL_MAX_DRAW_BUFFERS = " << maxDrawBuffers;
	cout << " -> Maximum Fragment Shader output locations = " << maxDrawBuffers << endl;

	// ****************************************************
	// Informações de Textura
	// ****************************************************

	GLint maxTextureImageUnits, maxTextureSize;

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureImageUnits);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

	cout << "\nTexture info:\n";
	// N.º máximo de unidades de textura de imagem suportadas e que estão acessíveis ao fragment shader.
	cout << "\tGL_MAX_TEXTURE_IMAGE_UNITS = " << maxTextureImageUnits << endl;
	// Dimensão, em pixéis, do maior tamanho de textura que o OpenGL pode suportar.
	cout << "\tGL_MAX_TEXTURE_SIZE = " << maxTextureSize << endl;
}

int main(void) {
	glfwSetErrorCallback(print_error);
	if (!glfwInit()) return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "P3D - Trabalho Pratico", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetKeyCallback(window, keyCallback);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	print_gl_info();
	init();

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				//Clear the screen

		display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	CleanupVBOsShader();
	glfwTerminate();				//Close OpenGL window and terminate GLFW
	return 0;
}

void init(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);			//Accept fragment if it's closer to the camera than the previous one
	glEnable(GL_CULL_FACE);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	ShaderInfo  shaders[] = {
	{ GL_VERTEX_SHADER,   "shaders/vertexShader.vert" },
	{ GL_FRAGMENT_SHADER, "shaders/fragmentShader.frag" },
	{ GL_NONE, NULL }
	};

	programa = LoadShaders(shaders);
	glUseProgram(programa);

	LightID = glGetUniformLocation(programa, "lightPos");

	//Get a "handle" for the MVP, V and M uniforms in the current shaders
	MatrixID = glGetUniformLocation(programa, "MVP");
	ViewMatrixID = glGetUniformLocation(programa, "V");
	ModelMatrixID = glGetUniformLocation(programa, "M");

	const char * mtl = loadOBJ("Iron_Man.obj", vertices, uvs, normals);
	loadMTL(mtl, ka, kd, ks, ns, ni, d, illum);

	textura = loadFileTGA("textures/Iron_Man_D.tga");			//Load texture

	//Get a "handle" for the "textureSampler" and "random" uniforms in the current shaders
	TextureID = glGetUniformLocation(programa, "textureSampler");
	frameCountID = glGetUniformLocation(programa, "random");

	//if (loadOBJ("Iron_Man.obj", vertices, uvs, normals)) {			//Read the .obj file
	//	cout << "File loaded successfuly!" << endl;
	//
	//}

	//Create and Bind/Activate VBOs
	// - Model Vertices
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	// - Model Texture
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	// - Model Normals
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
}

void display() {
	//Texture's color change effect (Press 5 on your keyboard to Activate/Deactivate it)
	if (changeColor) {
		randomCount += 0.01;
		//Send 3 float values to the currently bound shader, in the "random" uniform
		//Used to apply a color change in the texture
		glUniform3f(frameCountID, sin(randomCount), cos(randomCount), sin(randomCount));
	}
	else glUniform3f(frameCountID, 1, 1, 1);

	ProjectionMatrix = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 0.01f, 10000.f);	//Projection
	ViewMatrix = glm::lookAt(
		glm::vec3(0.0f, 3.0f, ZOOM),	// Posição da câmara no mundo
		glm::vec3(0.0f, 1.0f, -1.0f),	// Direção para a qual a câmara esta apontada
		glm::vec3(0.0f, 1.0f, 0.0f)		// Vector vertical
	);

	model1 = glm::rotate(glm::mat4(), anguloX*0.01f, glm::vec3(0, 1, 0));	//X
	model2 = glm::rotate(glm::mat4(), anguloY*0.01f, glm::vec3(1, 0, 0));	//Y

	ModelMatrix = model2 * model1;

	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	//Send transformation to the currently bound shader, in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

	//Send the light coordinates to the currently bound shader, in the "lightPos" uniform
	glm::vec3 lightPos = glm::vec3(4, 4, 4);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	glActiveTexture(GL_TEXTURE0);				//Bind textura in Texture Unit 0
	glBindTexture(GL_TEXTURE_2D, textura);
	glUniform1i(TextureID, 0);					//Set "textureSampler" sampler to use Texture Unit 0

	//1st Attribute Buffer -> Vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  //attribute
		3,                  //size
		GL_FLOAT,           //type
		GL_FALSE,           //normalized?
		0,                  //stride
		(void*)0            //array buffer offset
	);

	//2nd Attribute Buffer -> UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//3rd Attribute Buffer -> Normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void CleanupVBOsShader(void) {
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programa);
	glDeleteTextures(1, &textura);
	glDeleteVertexArrays(1, &VAO);
}