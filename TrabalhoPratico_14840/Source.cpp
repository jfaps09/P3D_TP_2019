#include "Header.h"
GLfloat ZOOM = 10.0f;

void init(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {		// Incremento no zoom, varia com a distância da câmara
	if (yoffset == -1)		ZOOM += fabs(ZOOM) * 0.2f;	// Se faz zoom in

	else if (yoffset == 1) ZOOM -= fabs(ZOOM) * 0.2f;	//Senão, se faz zoom out

	std::cout << "ZOOM = " << ZOOM << std::endl;
}

std::vector<glm::vec3> Load3DModel(void) {
	// 6 faces * 4 vértices por face
	glm::vec3 point[6 * 4] = {
		// Frente
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		// Trás
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		// Direita
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		// Esquerda
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		// Cima
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		// Baixo
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f)
	};

	std::vector<glm::vec3> ret;
	for (auto i : point)
		ret.push_back(i);

	return ret;
}

void display(std::vector<glm::vec3> obj, glm::mat4 mvp) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float *vertex_stream = static_cast<float*>(glm::value_ptr(obj.front()));

	std::vector<glm::vec3> colors{
		glm::vec3(1.0f, 0.0f, 0.0f), // Red
		glm::vec3(1.0f, 1.0f, 0.0f), // Yellow
		glm::vec3(0.0f, 1.0f, 0.0f), // Green
		glm::vec3(0.0f, 1.0f, 1.0f), // Cyan
		glm::vec3(0.0f, 0.0f, 1.0f), // Blue
		glm::vec3(1.0f, 0.0f, 1.0f)  // Magenta
	};

	// Desenha quad em modo imediato
	glBegin(GL_QUADS);
	/* obj.size()*(obj.front().length()) é o mesmo que 6*4*3 */
	/* 6 faces * 4 vértices por face * 3 coordenadas por vértice */
	for (int nv = 0; nv < 6 * 4 * 3; nv += 3) {
		glColor3f(colors[nv / (4 * 3)].r, colors[nv / (4 * 3)].g, colors[nv / (4 * 3)].b);
		glm::vec4 vertex = glm::vec4(vertex_stream[nv], vertex_stream[nv + 1], vertex_stream[nv + 2], 1.0f);
		// Cálculo das coordenadas de recorte
		glm::vec4 transformed_vertex = mvp * vertex;
		// Divisão de Perspetiva
		glm::vec4 normalized_vertex = transformed_vertex / transformed_vertex.w;
		// Desenho do vértice
		glVertex3f(normalized_vertex.x, normalized_vertex.y, normalized_vertex.z);
	}
	glEnd();
}

int main(void) {
	GLFWwindow *window;

	/*std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.

	if (loadOBJ("Iron_Man.obj", vertices, uvs, normals))
		printf("LOAD SUCCESFUL!");
	else
		printf("not");

	for (auto i : vertices)
		std::cout << "x:" << i.x << " y:" << i.y << " z:" << i.z << std::endl;

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);*/

	std::vector<glm::vec3> obj = Load3DModel();	

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "P3D - Trabalho Pratico 1 (Part #1)", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewInit();
	init();

	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 0.01f, 10000.f);	//Projection

	double xPos, yPos, xPrevPos, yPrevPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	//!---------LIGHTING--------------!
		//glEnable(GL_NORMALIZE);
		//glEnable(GL_LIGHTING);
		//glEnable(GL_LIGHT0);
		//glEnable(GL_LIGHT1);

	glm::mat4 model = glm::mat4(1.0f);	//ModelO

	while (!glfwWindowShouldClose(window)) {	
		glClear(GL_COLOR_BUFFER_BIT);

		xPrevPos = xPos;
		yPrevPos = yPos;

		// View
		glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, 0.0f, ZOOM),	// Posição da câmara no mundo
			glm::vec3(0.0f, 0.0f, -1.0f),	// Direção para a qual a câmara esta apontada
			glm::vec3(0.0f, 1.0f, 0.0f)		// Vector vertical
		);

	
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
			//translate --> Move obejct --> model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			glfwGetCursorPos(window, &xPos, &yPos);

			// Vai efetuando uma rotação ao objeto (apenas para podermos ver todas as faces desenhadas).

			//X
			if(xPos < xPrevPos)			model = glm::rotate(model, -0.01f, glm::vec3(0, 1.0f, 0.0f));
			else if(xPos > xPrevPos)	model = glm::rotate(model, 0.01f, glm::vec3(0, 1.0f, 0.0f));

			//Y
			if (yPos < yPrevPos)		model = glm::rotate(model, -0.01f, glm::vec3(1.0f, 0, 0.0f));
			else if (yPos > yPrevPos)	model = glm::rotate(model, 0.01f, glm::vec3(1.0f, 0, 0.0f));
		}

		// MVP
		glm::mat4 mvp = projection * view * model;

		display(obj, mvp);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}