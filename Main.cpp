#include "stb_image.h"
#include "Model.h"
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


// ==========================================
// VÉRTICES DEL CUBO
// X, Y, Z, U, V
// ==========================================


GLfloat vertices[] =
{
    // CARA FRONTAL (Normal: 0, 0, 1)
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

    // CARA TRASERA (Normal: 0, 0, -1)
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f,  0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f,  1.0f, 1.0f,

    // CARA IZQUIERDA (Normal: -1, 0, 0)
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

    // CARA DERECHA (Normal: 1, 0, 0)
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

    // CARA SUPERIOR (Normal: 0, 1, 0)
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

    // CARA INFERIOR (Normal: 0, -1, 0)
    -0.5f, -0.5f, -0.5f,  0.0f,-1.0f, 0.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,-1.0f, 0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,-1.0f, 0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,-1.0f, 0.0f,  0.0f, 1.0f
};



// ==========================================
// ÍNDICES
// ==========================================

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
	// ==========================================
	// INICIALIZAR GLFW
	// ==========================================

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


	// ==========================================
	// SHADER
	// ==========================================

	Shader shaderProgram("default.vert", "default.frag");
	// CARGAR MODELO 3D
	Model box("Models/Box.glb");

	// ==========================================
	// VAO, VBO Y EBO
	// ==========================================

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));



	// POSICIÓN
	VAO1.LinkVBO(
		VBO1,
		0,
		3,
		8 * sizeof(float),
		0
	);

	// NORMALES
	VAO1.LinkVBO(
		VBO1,
		1,
		3,
		8 * sizeof(float),
		3 * sizeof(float)
	);

	// COORDENADAS DE TEXTURA
	VAO1.LinkVBO(
		VBO1,
		2,
		2,
		8 * sizeof(float),
		6 * sizeof(float)
	);

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// ==========================================
	// PROFUNDIDAD
	// ==========================================

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


	// FILTRADO

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


	// REPETICIÓN

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


	// CARGAR IMAGEN

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
	// CONFIGURAR TEXTURA EN SHADER
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
	// VARIABLES DE LA CÁMARA
	// ==========================================

	float cameraAngle = 0.0f;

	float cameraHeight = 0.0f;

	float cameraDistance = 2.0f;


	// ==========================================
	// BUCLE PRINCIPAL
	// ==========================================

	while (!glfwWindowShouldClose(window))
	{
		// ==========================================
		// ENTRADA DEL TECLADO
		// ==========================================

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cameraAngle += 0.02f;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cameraAngle -= 0.02f;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cameraHeight += 0.02f;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cameraHeight -= 0.02f;
		}


		// ==========================================
		// LIMPIAR PANTALLA
		// ==========================================

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

		// ==========================================
		// MATRIZ MODEL
		// ==========================================

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::rotate(
			model,
			(float)glfwGetTime(),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		// ==========================================
		// POSICIÓN DE LA CÁMARA
		// ==========================================

		glm::vec3 cameraPosition;

		cameraPosition.x =
			cameraDistance * sin(cameraAngle);

		cameraPosition.y =
			cameraHeight;

		cameraPosition.z =
			cameraDistance * cos(cameraAngle);


		// ==========================================
		// CONFIGURACIÓN DE LA LUZ
		// ==========================================

		// Posición de la luz
		glUniform3f(
			glGetUniformLocation(shaderProgram.ID, "lightPos"),
			1.2f, 1.0f, 2.0f
		);

		// Color de la luz (blanco)
		glUniform3f(
			glGetUniformLocation(shaderProgram.ID, "lightColor"),
			1.0f, 1.0f, 1.0f
		);

		// Posición de la cámara
		glUniform3fv(
			glGetUniformLocation(shaderProgram.ID, "viewPos"),
			1,
			glm::value_ptr(cameraPosition)
		);

		// ==========================================
		// MATRIZ VIEW
		// ==========================================

		glm::mat4 view = glm::lookAt(
			cameraPosition,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		// ==========================================
		// MATRIZ PROJECTION
		// ==========================================

		glm::mat4 projection = glm::perspective(
			glm::radians(45.0f),
			1.0f,
			0.1f,
			100.0f
		);

		// ==========================================
		// UBICACIÓN DE MATRICES
		// ==========================================

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

		// ==========================================
		// ENVIAR MATRICES
		// ==========================================

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

		// ==========================================
		// ACTIVAR TEXTURA
		// ==========================================

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(
			GL_TEXTURE_2D,
			texture
		);

		// ==========================================
		// DIBUJAR
		// ==========================================

	// DIBUJAR CUBO
		VAO1.Bind();

		glDrawElements(
			GL_TRIANGLES,
			36,
			GL_UNSIGNED_INT,
			0
		);

		// DIBUJAR MODELO GLTF
		glm::mat4 modelBox = glm::mat4(1.0f);

		modelBox = glm::translate(
			modelBox,
			glm::vec3(1.5f, 0.0f, 0.0f)
		);

		modelBox = glm::scale(
			modelBox,
			glm::vec3(0.7f)
		);

		glUniformMatrix4fv(
			modelLoc,
			1,
			GL_FALSE,
			glm::value_ptr(modelBox)
		);

		box.Draw();

		// ==========================================
		// ACTUALIZAR
		// ==========================================

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// ==========================================
	// LIMPIAR MEMORIA
	// ==========================================

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();

	shaderProgram.Delete();

	glDeleteTextures(
		1,
		&texture
	);

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}