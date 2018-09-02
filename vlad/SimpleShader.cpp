
#include <limits.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>





#include "SimpleShader.h"
#include "util.h"


SimpleShader::SimpleShader()
{
	
}


bool SimpleShader::Init(const char* PathV, const char* PathF)
{

	if (!Technique::Init()) {
		return false;
	}


	if (!AddShader(GL_VERTEX_SHADER, PathV)) {
		return false;
	}

	
	if (!AddShader(GL_FRAGMENT_SHADER, PathF)) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}



	
	return true;
}






// ------------------------------------------------------------------------

void SimpleShader::setFloat(const std::string &name, float value)
{
	glUniform1f(GetUniformLocation(name.c_str()), value);
}

void SimpleShader::setVec2(const std::string &name, const Vector2f& value)
{

	glUniform2f(GetUniformLocation(name.c_str()), value.x, value.y);
}

void SimpleShader::setVec3(const std::string &name, const Vector3f& value)
{
	
	glUniform3f(GetUniformLocation(name.c_str()), value.x,value.y,value.z);
}

void SimpleShader::setVec4(const std::string &name, const Vector4f& value)
{
	glUniform4f(GetUniformLocation(name.c_str()), value.x, value.y, value.z,value.w);
}



// ------------------------------------------------------------------------
void SimpleShader::setMat4(const std::string &name, const Matrix4f& mat)
{	
	glUniformMatrix4fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, (const GLfloat*)mat.m);
} 
void SimpleShader::setInt(const std::string &name, unsigned int value)
{
	glUniform1i(GetUniformLocation(name.c_str()), value);
}