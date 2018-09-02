/*
        Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIGHTING_TECHNIQUE_H
#define	LIGHTING_TECHNIQUE_H

#include "technique.h"
#include "math_3d.h"

struct BaseLight
{
    Vector3f Color;
    float AmbientIntensity;
    float DiffuseIntensity;

    BaseLight()
    {
        Color = Vector3f(0.0f, 0.0f, 0.0f);
        AmbientIntensity = 0.0f;
        DiffuseIntensity = 0.0f;
    }
};

struct DirectionalLight : public BaseLight
{        
    Vector3f Direction;

    DirectionalLight()
    {
        Direction = Vector3f(0.0f, 0.0f, 0.0f);
    }
};

struct PointLight : public BaseLight
{
    Vector3f Position;

    struct
    {
        float Constant;
        float Linear;
        float Exp;
    } Attenuation;

    PointLight()
    {
        Position = Vector3f(0.0f, 0.0f, 0.0f);
        Attenuation.Constant = 1.0f;
        Attenuation.Linear = 0.0f;
        Attenuation.Exp = 0.0f;
    }
};

struct SpotLight : public PointLight
{
    Vector3f Direction;
    float Cutoff;

    SpotLight()
    {
        Direction = Vector3f(0.0f, 0.0f, 0.0f);
        Cutoff = 0.0f;
    }
};

class LightingTechnique : public Technique 
{
public:

    static const unsigned int MAX_POINT_LIGHTS = 2;
    static const unsigned int MAX_SPOT_LIGHTS = 2;

    LightingTechnique();

    virtual bool Init(const char* PathV, const char* PathF);	
    void SetMVP(const Matrix4f& mat);
  
    void SetModel(const Matrix4f& mat);

    void SetColorTextureUnit(unsigned int TextureUnit);  
    void SetNormalMapTextureUnit(unsigned int TextureUnit);

	
    void SetMainLight(const DirectionalLight& Light);
    void SetPointLights(unsigned int NumLights, const PointLight* Po);
    void SetSpotLights(unsigned int NumLights, const SpotLight* Sp);
    void SetCamera(const Vector3f& Camera);
    void SetMatSpecularIntensity(float Intensity);
    void SetMatSpecularPower(float Power);
	void SetAlfa(float A);
	void SetDepthTextureUnit(unsigned int TextureUnit);
	void SetHeightScale(float H);
private:

	GLuint Alfa;
	GLuint DepthMap;
	GLuint Point;
	GLuint Spot;
	GLuint HeightScale;
	GLuint SpecInten;
	GLuint SpecPower;
	GLuint DiffuseMap;
	GLuint NormalMap;
	GLuint Camera;


	GLuint MVP;
	GLuint Model;



    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Direction;
    } MainLight;

    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Position;
        struct {
            GLuint Constant;
            GLuint Linear;
            GLuint Exp;
        } Atten;
    } Points[MAX_POINT_LIGHTS];

    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Position;
        GLuint Direction;
        GLuint Cutoff;
        struct {
            GLuint Constant;
            GLuint Linear;
            GLuint Exp;
        } Atten;
    } Spots[MAX_SPOT_LIGHTS];
};


#endif	/* LIGHTING_TECHNIQUE_H */
