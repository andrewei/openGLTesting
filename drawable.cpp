#include <vector>
#include "drawable.hpp"
#include <glm/gtx/transform.hpp>
#include "bmploader.hpp"
#include "data.hpp"

Drawable::Drawable(int programID, const std::vector<GLfloat> g_vertex_buffer_data, const std::vector<GLfloat> g_color_buffer_data, glm::vec3 translatePos) {
	size_data = g_vertex_buffer_data.size();
	//Creating and binding vertex buffer buffers box (2)
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, size_data*sizeof(GLfloat), g_vertex_buffer_data.data(), GL_STATIC_DRAW);

	// create MVP
	MatrixID = glGetUniformLocation(programID, "MVP");

	//Creating VAO (2)
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);


	if(loadBMP_custom("uvtemplate.bmp") == 0){
			std::cout << "could not load .bmp" << std::endl;
		}

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);

	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	Model = glm::translate(glm::mat4(), translatePos);

}
