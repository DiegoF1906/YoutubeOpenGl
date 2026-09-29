#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// VÉRTICES DEL CUBO
// X, Y, Z, U, V
GLfloat vertices[] =
{
	// CARA FRONTAL
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

	// CARA TRASERA
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

	// CARA IZQUIERDA
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

	// CARA DERECHA
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

	 // CARA SUPERIOR
	 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

	 // CARA INFERIOR
	 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 1.0f
};


// ÍNDICES DEL CUBO
GLuint indices[] =
{
	// CARA FRONTAL
	0, 1, 2,
	2, 3, 0,

	// CARA TRASERA
	4, 5, 6,
	6, 7, 4,

	// CARA IZQUIERDA
	8, 9, 10,
	10, 11, 8,

	// CARA DERECHA
	12, 13, 14,
	14, 15, 12,

	// CARA SUPERIOR
	16, 17, 18,
	18, 19, 16,

	// CARA INFERIOR
	20, 21, 22,
	22, 23, 20
};


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(
		800,
		800,
		"YoutubeOpenGL",
		NULL,
		NULL
	);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, 800, 800);


	// SHADER
	Shader shaderProgram("default.vert", "default.frag");


	// VAO Y VBO
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));


	// ATRIBUTO DE POSICIÓN
	VAO1.LinkVBO(
		VBO1,
		0,
		3,
		5 * sizeof(float),
		0
	);


	// ATRIBUTO DE TEXTURA
	VAO1.LinkVBO(
		VBO1,
		1,
		2,
		5 * sizeof(float),
		3 * sizeof(float)
	);


	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	// PROFUNDIDAD
	glEnable(GL_DEPTH_TEST);


	// ==========================================
	// CARGAR TEXTURA
	// ==========================================

	int widthImg;
	int heightImg;
	int numColCh;

	unsigned char* bytes = stbi_load(
		"textura.jpg",
		&widthImg,
		&heightImg,
		&numColCh,
		0
	);

	GLuint texture;

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);


	// CONFIGURACIÓN DE LA TEXTURA

	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR
	);

	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR
	);

	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S,
		GL_REPEAT
	);

	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		GL_REPEAT
	);


	// ENVIAR IMAGEN A OPENGL

	if (bytes)
	{
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB,
			widthImg,
			heightImg,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			bytes
		);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(bytes);

	glBindTexture(GL_TEXTURE_2D, 0);


	// ==========================================
	// SHADER
	// ==========================================

	shaderProgram.Activate();

	glUniform1i(
		glGetUniformLocation(
			shaderProgram.ID,
			"tex0"
		),
		0
	);


	// ==========================================
	// BUCLE PRINCIPAL
	// ==========================================

	while (!glfwWindowShouldClose(window))
	{
		// LIMPIAR PANTALLA
		glClearColor(
			0.07f,
			0.13f,
			0.17f,
			1.0f
		);

		glClear(
			GL_COLOR_BUFFER_BIT |
			GL_DEPTH_BUFFER_BIT
		);


		shaderProgram.Activate();


		// MATRICES

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);


		// ROTACIÓN DEL CUBO

		model = glm::rotate(
			model,
			(float)glfwGetTime(),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);


		// CÁMARA

		view = glm::translate(
			view,
			glm::vec3(0.0f, 0.0f, -2.0f)
		);


		// PROYECCIÓN

		projection = glm::perspective(
			glm::radians(45.0f),
			1.0f,
			0.1f,
			100.0f
		);


		// UBICACIÓN DE LAS MATRICES

		int modelLoc =
			glGetUniformLocation(
				shaderProgram.ID,
				"model"
			);

		int viewLoc =
			glGetUniformLocation(
				shaderProgram.ID,
				"view"
			);

		int projLoc =
			glGetUniformLocation(
				shaderProgram.ID,
				"projection"
			);


		// ENVIAR MATRICES

		glUniformMatrix4fv(
			modelLoc,
			1,
			GL_FALSE,
			glm::value_ptr(model)
		);

		glUniformMatrix4fv(
			viewLoc,
			1,
			GL_FALSE,
			glm::value_ptr(view)
		);

		glUniformMatrix4fv(
			projLoc,
			1,
			GL_FALSE,
			glm::value_ptr(projection)
		);


		// ACTIVAR TEXTURA

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);


		// DIBUJAR CUBO

		VAO1.Bind();

		glDrawElements(
			GL_TRIANGLES,
			36,
			GL_UNSIGNED_INT,
			0
		);


		// INTERCAMBIAR BUFFERS

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// LIMPIAR

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glDeleteTextures(1, &texture);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
