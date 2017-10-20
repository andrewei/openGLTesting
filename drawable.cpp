#include <vector>
#include "drawable.hpp"
#include <glm/gtx/transform.hpp>

Drawable::Drawable(int programID, const std::vector<GLfloat> g_vertex_buffer_data, const std::vector<GLfloat> g_color_buffer_data, glm::vec3 translatePos) {
	size_data = g_vertex_buffer_data.size();
	//Creating and binding vertex buffer buffers box (2)
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, size_data*sizeof(GLfloat), g_vertex_buffer_data.data(), GL_STATIC_DRAW);

	//color buffer Box
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, size_data*sizeof(GLfloat), g_color_buffer_data.data(), GL_STATIC_DRAW);

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
	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	Model = glm::translate(glm::mat4(), translatePos);

}
