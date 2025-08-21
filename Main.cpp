#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Vari�veis globais
const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shaderProgram; //inteiro n�o sinalizado, vamos guardar o VAO e o VBO, e qual progrma est� rodando (shader)

//C�digo GLSL (em C, n�o tem string em C)
static const char* vertexShader = "           \n\
#version 330                                  \n\
                                              \n\
layout(location=0) in vec2 pos;               \n\
                                              \n\
void main() {                                 \n\
   gl_position = vec4(pos.x,pos.y, 0.0, 1.0); \n\
}                                             \n\
";

static const char* fragmentShader = "          \n\
#version 330                                   \n\
                                               \n\
uniform in vec3 triColor;                      \n\
                                               \n\
void main() {                                  \n\
	                                           \n\
   color = vec4(triColor,1.0);                 \n\
}                                              \n\
";

void criaTriangulo() {
	GLfloat vertices[] = {
		0.0f, 1.0f, //Vertice 1
		-1.0f,-1.0f, //Vertice 2
		1.0f,-1.0f //Vertice 3
	};

	glGenVertexArrays(1, &VAO); 
	glBindVertexArray(VAO); //Aponte para o endere�o que acabou de criar
		
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
			
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0); //Location

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void adicionaTriangulo(GLuint program, const char* shaderCode, GLenum type) {
	GLuint _shader = glCreateShader(type);

	//Converte char para GL_CHAR
	const GLchar* code[1];
	code[0] = shaderCode;

	glShaderSource(_shader, 1, code, NULL);
	glCompileShader(_shader);

	//Tratar os erros

	glAttachShader(program, _shader);
}

void adicionaPrograma() {
	shaderProgram = glCreateProgram();
	if (!shaderProgram) {
		printf("Erro ao criar o programa!");
		return;
	}

	adicionaTriangulo(shaderProgram, vertexShader, GL_VERTEX_SHADER);
	adicionaTriangulo(shaderProgram, fragmentShader, GL_FRAGMENT_SHADER);
	glLinkProgram(shaderProgram);
}


//Programa principal
int main() {
	//Iniciar o GLFW
	if (!glfwInit()) {
		printf("GLFW n�o foi iniciado!");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Glew gerencia conforme a arquitetura da placa de v�deo para as vari�veis
	
	//Classe janela
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL); //Vai retornar o endere�o de mem�ria onde a janela est�, window vai ser um ponteiro


	if (!window) {
		printf("Erro ao criar a janela!");
		glfwTerminate();
		return 1;
	}

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		printf("GLEW n�o foi iniciado!");
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);

	while (!glfwWindowShouldClose(window)) { //Vai verificar se a janela precisa ser fechada, vai verificar com o SO
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f); //RGB e canal alpha (transparencia)
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}