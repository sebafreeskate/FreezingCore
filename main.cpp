
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>


const GLuint WIDTH = 3200, HEIGHT = 1800;

const GLchar *vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 vertexColor; \n"
"void main()\n"
"{ \n"
"gl_Position = vec4(position, 1.0); \n"
"vertexColor = color; \n"
"} \n";

const GLchar *orangeFragmentShaderSrc =
"#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{ \n" 
" color= vec4(1.0f, 0.5f, 0.2f, 1.0f);  //the color orange \n"
" }	\n \0";

const GLchar *yellowFragmentShaderSrc =
"#version 330 core\n"
"out vec4 color;\n"
"in vec3 vertexColor; \n"
"void main()\n"
"{\n"
"color = vec4(vertexColor, 1.0f); \n"
"}\n \0";


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void checkShaderCompileStatus(const GLuint &shader) 
{
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void checkProgramLinkStatus(const GLuint &program)
{
	GLint success;
	GLchar infolog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infolog);
		std::cout << "ERROR::PRIGRAM::LINK_FAILED\n" << infolog << std::endl;
	}
}

GLuint create_VAO()
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	return VAO;
}

int main()
{

	//Инициализация GLFW
	glfwInit();
	//Настройка GLFW
	//Задается минимальная требуемая версия OpenGL. 
	//Мажорная 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Create a GLFWWindwow object that we can use for GLEW's functions
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGl", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		abort();
	}
	glfwMakeContextCurrent(window);

	//set the requred callback functions
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" <<  std::endl;
		abort();
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	//Shaders
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);

	GLuint orangeFragmentShader;
	orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(orangeFragmentShader, 1, &orangeFragmentShaderSrc, NULL);
	glCompileShader(orangeFragmentShader);

	GLuint yellowFragmentShader;
	yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSrc, NULL);
	glCompileShader(yellowFragmentShader);

	checkShaderCompileStatus(vertexShader);
	checkShaderCompileStatus(orangeFragmentShader);
	checkShaderCompileStatus(yellowFragmentShader);

	GLuint orangeShaderProgram;
	orangeShaderProgram = glCreateProgram();
	glAttachShader(orangeShaderProgram, vertexShader);
	glAttachShader(orangeShaderProgram, orangeFragmentShader);
	glLinkProgram(orangeShaderProgram);

	GLuint yellowShaderProgram;
	yellowShaderProgram = glCreateProgram();
	glAttachShader(yellowShaderProgram, vertexShader);
	glAttachShader(yellowShaderProgram, yellowFragmentShader);
	glLinkProgram(yellowShaderProgram);

	checkProgramLinkStatus(yellowShaderProgram);
	checkProgramLinkStatus(orangeShaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(orangeFragmentShader);
	glDeleteShader(yellowFragmentShader);

	//Geometry
	GLfloat first_triangle[] = {
		// Позиции         // Цвета
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Нижний правый угол
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Нижний левый угол
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Верхний угол
	};
	GLfloat second_triangle[] = {
		-0.5f, -0.5f, 0.0f, // Нижний левый угол
		-0.5f, 0.5f, 0.0f, // Верхний левый угол
		0.5f, -0.5f, 0.0f // Нижний правый угол
	};
	GLuint indices[] = { // Помните, что мы начинаем с 0!
		0, 1, 3, // Первый треугольник
		1, 2, 3 // Второй треугольник
	};

	//VAO, VBO , etc
	GLuint VAO1, VBO1, EBO;
	GLuint VAO2, VBO2;

	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	//glGenBuffers(1, &EBO);
	//bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointers
	glBindVertexArray(VAO1);

		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle), first_triangle, GL_STATIC_DRAW); 
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);


		glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); 

	glBindVertexArray(VAO2);

		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(second_triangle), second_triangle, GL_STATIC_DRAW); 
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); 

//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//draw triangle

		GLfloat time_value = glfwGetTime();
		GLfloat green_value = (sin(time_value) / 2) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation(yellowShaderProgram, "globalColor");
		glUseProgram(yellowShaderProgram);
		glUniform4f(vertexColorLocation, 0.0f, green_value, 0.0f, 1.0f);

		glBindVertexArray(VAO1);
	//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glUseProgram(orangeShaderProgram);

		glBindVertexArray(VAO2);
	//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	} 

	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glfwTerminate();

	return 0;
}