#ifndef SIMPLESHADER_H
#define	SIMPLESHADER_H

#include "technique.h"
#include "math_3d.h"
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <gtc/matrix_transform.hpp>


class SimpleShader : public Technique
{
public: 
	SimpleShader();

	virtual bool Init(const char* PathV, const char* PathF);

	void setFloat(const std::string &name, float value);
	void setVec2(const std::string &name, const Vector2f& value);
	void setVec3(const std::string &name, const Vector3f& value);
	void setVec4(const std::string &name, const Vector4f& value);
	void setMat4(const std::string &name, const Matrix4f& mat);
	void setInt(const std::string &name,unsigned int value);
	
private:

	GLuint ColorT;
	GLuint HeightT;
	GLuint NormalT;
	GLuint Proj;
	GLuint Model;
	GLuint View;
	GLuint Light;
	GLuint ViewPos;
	

};

#endif