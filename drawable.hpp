#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

//lib for 3d mathematics
#include <glm/glm.hpp>

//magic lib
#include <GL/glew.h>

using namespace glm;


class Drawable
{
public:
	Drawable(int programID, const std::vector<GLfloat> g_vertex_buffer_data, const std::vector<GLfloat> g_color_buffer_data, glm::vec3 translatePos);
	mat4 Model, mvp;
	GLuint vertexbuffer, colorbuffer, MatrixID, VertexArrayID;
	size_t size_data;
};

#endif // DRAWABLE_HPP
