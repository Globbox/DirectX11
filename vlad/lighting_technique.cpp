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

#include <limits.h>
#include <string.h>

#include "lighting_technique.h"
#include "util.h"





LightingTechnique::LightingTechnique()
{   
}

bool LightingTechnique::Init(const char* PathV, const char* PathF)
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, PathV)) { //"MainShader.vs"
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, PathF)) { //"MainShader.fs"
        return false;
    }

    if (!Finalize()) {
        return false;
    }

	Point = GetUniformLocation("Point");
	  Spot = GetUniformLocation("Spot");
	  HeightScale = GetUniformLocation("HeightScale");
	  SpecInten = GetUniformLocation("SpecInten");
	  SpecPower = GetUniformLocation("SpecPower");
	  DiffuseMap = GetUniformLocation("DiffuseMap");
	  NormalMap = GetUniformLocation("NormalMap");
	  Camera = GetUniformLocation("Camera");
	  Alfa = GetUniformLocation("Alfa");
	  DepthMap = GetUniformLocation("DepthMap");

	  MVP = GetUniformLocation("MVP");
	  Model = GetUniformLocation("Model");

	  MainLight.AmbientIntensity = GetUniformLocation("MainLight.Base.AmbientIntensity");
	 MainLight.Color = GetUniformLocation("MainLight.Base.Color");
	MainLight.DiffuseIntensity = GetUniformLocation("MainLight.Base.DiffuseIntensity");
	MainLight.Direction = GetUniformLocation("MainLight.Direction");
		
		  
		  
		  for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(Points); i++) {
			  char Name[128];
			  memset(Name, 0, sizeof(Name));
			  snprintf(Name, sizeof(Name), "Points[%d].Base.Color", i);
			  Points[i].Color = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Points[%d].Base.AmbientIntensity", i);
			  Points[i].AmbientIntensity = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Points[%d].Position", i);
			  Points[i].Position = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Points[%d].Base.DiffuseIntensity", i);
			  Points[i].DiffuseIntensity = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Points[%d].Atten.Constant", i);
			  Points[i].Atten.Constant = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Points[%d].Atten.Linear", i);
			  Points[i].Atten.Linear = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Points[%d].Atten.Exp", i);
			  Points[i].Atten.Exp = GetUniformLocation(Name);

		  }

		  for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(Spots); i++) {
			  char Name[128];
			  memset(Name, 0, sizeof(Name));
			  snprintf(Name, sizeof(Name), "Spots[%d].Base.Base.Color", i);
			  Spots[i].Color = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Spots[%d].Base.Base.AmbientIntensity", i);
			  Spots[i].AmbientIntensity = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Spots[%d].Base.Position", i);
			  Spots[i].Position = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Spots[%d].Direction", i);
			  Spots[i].Direction = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Spots[%d].Cutoff", i);
			  Spots[i].Cutoff = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Spots[%d].Base.Base.DiffuseIntensity", i);
			  Spots[i].DiffuseIntensity = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Spots[%d].Base.Atten.Constant", i);
			  Spots[i].Atten.Constant = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Spots[%d].Base.Atten.Linear", i);
			  Spots[i].Atten.Linear = GetUniformLocation(Name);

			  snprintf(Name, sizeof(Name), "Spots[%d].Base.Atten.Exp", i);
			  Spots[i].Atten.Exp = GetUniformLocation(Name);

		
		  }
	
/*
    if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
        m_WVPLocation == INVALID_UNIFORM_LOCATION ||
        m_LightWVPLocation == INVALID_UNIFORM_LOCATION ||
        m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
        m_colorMapLocation == INVALID_UNIFORM_LOCATION ||
        m_shadowMapLocation == INVALID_UNIFORM_LOCATION ||
        m_normalMapLocation == INVALID_UNIFORM_LOCATION ||
        m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION ||
        m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
        m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
        m_numPointLightsLocation == INVALID_UNIFORM_LOCATION ||
        m_numSpotLightsLocation == INVALID_UNIFORM_LOCATION) {
        return false;
    } 

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation) ; i++) {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
        m_pointLightsLocation[i].Color = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
        m_pointLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Position", i);
        m_pointLightsLocation[i].Position = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
        m_pointLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
        m_pointLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
        m_pointLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
        m_pointLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

        if (m_pointLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
            m_pointLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
            return false;
        }
    }

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_spotLightsLocation) ; i++) {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
        m_spotLightsLocation[i].Color = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
        m_spotLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
        m_spotLightsLocation[i].Position = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
        m_spotLightsLocation[i].Direction = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
        m_spotLightsLocation[i].Cutoff = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
        m_spotLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
        m_spotLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
        m_spotLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
        m_spotLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

        if (m_spotLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].Direction == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].Cutoff == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
            m_spotLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
            return false;
        }
    }
	*/
    return true;
}


void LightingTechnique::SetMVP(const Matrix4f& mat)
{
    glUniformMatrix4fv(MVP, 1, GL_TRUE, (const GLfloat*)mat.m);
}



void LightingTechnique::SetModel(const Matrix4f& mat)
{
    glUniformMatrix4fv(Model, 1, GL_TRUE, (const GLfloat*)mat.m);
}


void LightingTechnique::SetColorTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(DiffuseMap, TextureUnit);
}

void LightingTechnique::SetDepthTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(DepthMap, TextureUnit);
}



void LightingTechnique::SetNormalMapTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(NormalMap, TextureUnit);
}



void LightingTechnique::SetMainLight(const DirectionalLight& Light)
{
    glUniform3f(MainLight.Color, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(MainLight.AmbientIntensity, Light.AmbientIntensity);
    Vector3f Direction = Light.Direction;
    Direction.Normalize();
    glUniform3f(MainLight.Direction, Direction.x, Direction.y, Direction.z);
    glUniform1f(MainLight.DiffuseIntensity, Light.DiffuseIntensity);
}


void LightingTechnique::SetCamera(const Vector3f& EyeWorldPos)
{
	
    glUniform3f(Camera, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}


void LightingTechnique::SetMatSpecularIntensity(float Intensity)
{
    glUniform1f(SpecInten, Intensity);
}

void LightingTechnique::SetAlfa(float A)
{
	glUniform1f(Alfa, A);
}
void LightingTechnique::SetHeightScale(float H)
{
	glUniform1f(HeightScale, H);
}


void LightingTechnique::SetMatSpecularPower(float Power)
{
    glUniform1f(SpecPower, Power);
}




void LightingTechnique::SetPointLights(unsigned int NumLights, const PointLight* pLights)
{
    glUniform1i(Point, NumLights);
    
    for (unsigned int i = 0 ; i < NumLights ; i++) {
        glUniform3f(Points[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
        glUniform1f(Points[i].AmbientIntensity, pLights[i].AmbientIntensity);
        glUniform1f(Points[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
        glUniform3f(Points[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
        glUniform1f(Points[i].Atten.Constant, pLights[i].Attenuation.Constant);
        glUniform1f(Points[i].Atten.Linear, pLights[i].Attenuation.Linear);
        glUniform1f(Points[i].Atten.Exp, pLights[i].Attenuation.Exp);
    }
}


void LightingTechnique::SetSpotLights(unsigned int NumLights, const SpotLight* pLights)
{
    glUniform1i(Spot, NumLights);

    for (unsigned int i = 0 ; i < NumLights ; i++) {
        glUniform3f(Spots[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
        glUniform1f(Spots[i].AmbientIntensity, pLights[i].AmbientIntensity);
        glUniform1f(Spots[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
        glUniform3f(Spots[i].Position,  pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
        Vector3f Direction = pLights[i].Direction;
        Direction.Normalize();
        glUniform3f(Spots[i].Direction, Direction.x, Direction.y, Direction.z);
        glUniform1f(Spots[i].Cutoff, cosf(ToRadian(pLights[i].Cutoff)));
        glUniform1f(Spots[i].Atten.Constant, pLights[i].Attenuation.Constant);
        glUniform1f(Spots[i].Atten.Linear,   pLights[i].Attenuation.Linear);
        glUniform1f(Spots[i].Atten.Exp,      pLights[i].Attenuation.Exp);
    }
}