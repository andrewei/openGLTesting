//display stuff in console
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <iostream>

//magic lib
#include <GL/glew.h>

//handle windows and keyboard
#include <glfw3.h>

//lib for 3d mathematics
#include <glm/glm.hpp>

//mathy shit
#include <math.h>

#include <common/shader.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

//object data
#include "data.hpp"
#include "drawable.hpp"

using namespace glm;

GLFWwindow* window;

int opengl_init();
void camera_init(mat4 &Projection, mat4 &View);

int main(void) {
	int programID = opengl_init();

	if(programID == -1)
		return -1;
	mat4 Projection, View;
	camera_init(Projection, View);

	std::vector<Drawable> drawables;
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			for(int k = 0; k < 10; k++){
				drawables.emplace_back(programID, g_vertex_buffer_data ,g_color_buffer_data, glm::vec3(0.0f, 0.0f ,0.0f));
				drawables.emplace_back(programID, g_vertex_buffer_data ,g_color_buffer_data, glm::vec3(-50 + i * 10.0f, -50 + j * 10.0f, -50 + k * 10.0f));
				//drawables.emplace_back(programID, g_vertex_buffer_data_triangle ,g_color_buffer_data_triangle, glm::vec3(-50 + i * 10.0f, -50 + j * 10.0f, -50 + k * 10.0f));
			}
		}
	}

	float scale_var = 0;
	int sin_var = 0;
	mat4 scale_matrix;
	do {
		sin_var +=1;

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
			scale_var *= 1.01;
		}
		// Move backward
		else if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
			scale_var *= 0.99;
		}
		else {
			if(scale_var > 1.2){
				scale_var -= 0.1;
			}
			else if (scale_var < 0.8){
				scale_var += 0.002;
			}
		}



		// Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		std::cout << xpos << std::endl;

		//Rotate Box
		vec3 rotation_model( 1.0f, 0.0f, 0.0f);
		for (int i=0; i < drawables.size(); i++) {
			mat4 model_transform = rotate(float(xpos * 0.0001f), rotation_model);
			drawables[i].Model = drawables[i].Model * model_transform;
		}

		//Rotate view
		vec3 rotation_view(0.0f, 1.0f, 0.0f);
		mat4 view_transform = rotate(sin (static_cast<float>(sin_var*3.14/180))*0.02f - 0.025f, rotation_view);
		//View = View * view_transform;

		//scale
		//mat4 scale_matrix = scale(vec3(static_cast<float>(sin(sin_var*3.14/180)+2.0f), static_cast<float>(sin(sin_var*3.14/180)+2.0f), static_cast<float>(sin(sin_var*3.14/180)+2.0f)));
		scale_matrix = scale(vec3(scale_var, scale_var, scale_var));
		//Setting box position
		for (int i=0; i < drawables.size(); i++) {
			drawables[i].mvp = Projection * View * scale_matrix * drawables[i].Model;
		}

		//Clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw background
		//glClearColor(static_cast<float>(cos(sin_var/350)*0.58f), static_cast<float>(tan(sin_var/350)*0.58f), static_cast<float>(sin(sin_var/350)*0.58f), 0.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		for (int i=0; i < drawables.size(); i++) {
			glUniformMatrix4fv(drawables[i].MatrixID, 1, GL_FALSE, &drawables[i].mvp[0][0]);
			glBindVertexArray(drawables[i].VertexArrayID);
			glDrawArrays(GL_TRIANGLES, 0, drawables[i].size_data * 3);
		}

		//swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Reset mouse position for next frame
		//glfwSetCursorPos(window, 1024/2, 768/2);
	}
	while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		  glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


int opengl_init() {
	// Initialise GLFW
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return false;
	}

	//create openGL window
	glfwWindowHint(GLFW_SAMPLES, 4); //4x anialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3 ); //Set openGL version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3 ); //Set minor version (becomes 3.3)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //To make MacOS happy, should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We donæt want the old OpenGL

	// Open a windows and create its OpenGL context
	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Red triangle", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	//init GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return false;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glewInit();

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "simplevertexshader.vertexshader", "simplefragmentshader.fragmentshader" );

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Use our shader
	glUseProgram(programID);
	return programID;
}

void camera_init(mat4 &Projection, mat4 &View) {
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = perspective(radians(45.0f),  4.0f / 3.0f, 0.1f, 1000.0f);

	 // Or, for an ortho camera :
	//mat4 Projection = ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	View = lookAt(
		vec3(4,3,-60), // Camera is at (4,3,3), in World Space
		vec3(0,0,0), // and looks at the origin
		vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
}
