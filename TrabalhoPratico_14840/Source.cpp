#include "Header.h"

#include "texture.hpp"

GLFWwindow* window;

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 2129;
glm::mat4 ModelMatrix, ViewMatrix, ProjectionMatrix;
GLuint programa;
GLuint MatrixID;
GLuint Texture;
GLuint TextureID;
GLuint vertexbuffer;
GLuint uvbuffer;

GLuint programID;
GLuint VertexArrayID;

GLfloat ZOOM = 5.0f;
float anguloX = 0.0f, anguloY = 0.0f;
bool buttonPressed = false;
double xPrevPos, yPrevPos;
static float updateRotX = 0, updateRotY = 0;

glm::mat4 model1 = glm::mat4(1.0f);	//Model1
glm::mat4 model2 = glm::mat4(1.0f);	//Model2

std::vector< glm::vec3 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec3 > normals;

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {		// Incremento no zoom, varia com a distância da câmara
	if (yoffset == -1)		ZOOM += fabs(ZOOM) * 0.2f;	// Se faz zoom in

	else if (yoffset == 1) ZOOM -= fabs(ZOOM) * 0.2f;	//Senão, se faz zoom out

	std::cout << "ZOOM = " << ZOOM << std::endl;
}

void cursorPosCallback(GLFWwindow *window, double xPos, double yPos) {
	if (buttonPressed) {
		anguloX = (float)(((xPos - xPrevPos) * 180.0) / WIDTH) + updateRotX;
		anguloY = (float)(((yPos - yPrevPos) * 180.0) / HEIGHT)+ updateRotY;
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

void display() {
	ProjectionMatrix = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 0.01f, 10000.f);	//Projection
	ViewMatrix = glm::lookAt(
		glm::vec3(0.0f, 3.0f, ZOOM),	// Posição da câmara no mundo
		glm::vec3(0.0f, 1.0f, -1.0f),	// Direção para a qual a câmara esta apontada
		glm::vec3(0.0f, 1.0f, 0.0f)		// Vector vertical
	);

	//X
	model1 = glm::rotate(glm::mat4(), anguloX*0.01f, glm::vec3(0, 1, 0));

	//Y
	model2 = glm::rotate(glm::mat4(), anguloY*0.01f, glm::vec3(1, 0, 0));

	ModelMatrix = model2 * model1;

	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
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

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	
	//print_gl_info();
	
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	ShaderInfo  shaders[] = {
	{ GL_VERTEX_SHADER,   "shaders/TransformVertexShader.vert" },
	{ GL_FRAGMENT_SHADER, "shaders/TextureFragmentShader.frag" },
	{ GL_NONE, NULL }
	};

	programID = LoadShaders(shaders);
	glUseProgram(programID);

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");

	// Load the texture
	Texture = loadTGA_glfw("Iron_Man_D.tga");

	// Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Read our .obj file
	bool res = loadOBJ("Iron_Man.obj", vertices, uvs, normals);

	// Load it into a VBO

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	do{	
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}