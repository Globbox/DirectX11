

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <conio.h>
#include <sys/types.h>
#include <sys/timeb.h>

#include "engine_common.h"
#include "util.h"
#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "mesh.h"
#include "skybox.h"
#include "SimpleShader.h"
#include "Amesh.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1200


class Model {
public: 
	Model()
	{
		mesh = NULL;
		texture = NULL;
		Ntexture = NULL;
		 Dtexture = NULL;

	}
	bool Init(const std::string& Mname, const std::string& Tname, const std::string& Nname, const std::string& Dname)
	{
		mesh = new Mesh();
		if (!mesh->LoadMesh(Mname))
		{
			printf("Error! loading model\n");
			return false;
		}
		
		texture = new Texture(GL_TEXTURE_2D, Tname);

		if (!texture->Load())
		{
			printf("Error! loading texture\n");
			return false;
		}
		Ntexture = new Texture(GL_TEXTURE_2D, Nname);

		if (!Ntexture->Load())
		{
			printf("Error! loading Normal Map\n");
			
		}
		Dtexture = new Texture(GL_TEXTURE_2D, Dname);

		if (!Dtexture->Load())
		{
			printf("Error! loading Depth Map\n");

		}
		


		return true;
	}
	void Render()
	{
		texture->Bind(COLOR_TEXTURE_UNIT);
		Ntexture->Bind(NORMAL_TEXTURE_UNIT);
		Dtexture->Bind(HIEGHT_TEXTURE_UNIT);
		mesh->Render();
	}
private:
	Mesh * mesh;
	Texture * texture;
	Texture * Ntexture;
	Texture * Dtexture;

};

class Tutorial26 : public ICallbacks
{
public:

	Tutorial26()
	{
		
		m_pLightingTechnique = NULL;
		m_pGameCamera = NULL;
		m_pSphereMesh = NULL;
		m_scale = 0.0f;
		m_pTexture = NULL;
		m_pNormalMap = NULL;
		m_pTrivialNormalMap = NULL;
		simp = NULL;

		PointL[0].Attenuation.Linear = 0.0f;
		PointL[0].Attenuation.Exp = 1.0f;
		PointL[0].Attenuation.Constant = 0.0f;
		PointL[0].Color = Vector3f(1.0f, 0.02f, 0.02f);
		PointL[0].DiffuseIntensity = 0.6f;
		PointL[0].AmbientIntensity = 2.0f;


		m_dirLight.AmbientIntensity = 0.6f;
		m_dirLight.DiffuseIntensity = 1.0f;
		m_dirLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_dirLight.Direction = Vector3f(0.1f, -1.0f, 0.1f);
		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 0.01f;
		m_persProjInfo.zFar = 1000.0f;

		m_bumpMapEnabled = true;
	}


	~Tutorial26()
	{
		SAFE_DELETE(m_pLightingTechnique);
		SAFE_DELETE(m_pGameCamera);
		SAFE_DELETE(m_pSphereMesh);
		SAFE_DELETE(m_pTexture);
		SAFE_DELETE(m_pNormalMap);
		SAFE_DELETE(m_pTrivialNormalMap);
	}


	bool Init()
	{
		Vector3f Pos(0.5f, 1.025f, 0.25f);
		Vector3f Target(0.0f, -0.5f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		if (!A_mesh.LoadMesh("./Content/boblampclean.md5mesh")) {
			printf("Mesh load failed\n");
			return false;
		}


		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		m_pLightingTechnique = new LightingTechnique();
		simp = new SimpleShader();

		if (!simp->Init("Anim.vs", "Anim.fs")) {
			printf("Error initializing the simp\n");
			return false;
		}

		if (!m_pLightingTechnique->Init("Light.vs", "Light.fs")) {
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_pLightingTechnique->Enable();
		m_pLightingTechnique->SetMainLight(m_dirLight);
		m_pLightingTechnique->SetColorTextureUnit(0);
		m_pLightingTechnique->SetNormalMapTextureUnit(2);
		m_pLightingTechnique->SetDepthTextureUnit(3);
		

	//	m_pSphereMesh = new Mesh();

		
		
	//	if (!m_pSphereMesh->LoadMesh("./Content/box.obj")) {
	//		return false;
	//	}
		
		
		ground = new Model();
		meteor = new Model();
		ball = new Model();
		water = new Model();

		if (!ground->Init("./Content/Plane.obj", "./Content/tex.jpg", "./Content/tex_n.jpg", "./Content/tex_d.jpg")) {
			return false;
		}
		if (!meteor->Init("./Content/LavaBall.obj", "./Content/lava.jpg","","")) {
			return false;
		}
		if (!ball->Init("./Content/LittleBall.obj", "./Content/lava.jpg","", "")) {
			return false;
		}

		if (!water->Init("./Content/water.obj", "./Content/waterT.jpg", "", "")) {
			return false;
		}

/**/


		



		m_pSkyBox = new SkyBox(m_pGameCamera, m_persProjInfo);
		if (!m_pSkyBox->Init(".",
			"./Content/sp3right.jpg",
			"./Content/sp3left.jpg",
			"./Content/sp3top.jpg",
			"./Content/sp3bot.jpg",
			"./Content/sp3front.jpg",
			"./Content/sp3back.jpg")) {
			return false;
		}
		return true;
	}


	void Run()
	{
		GLUTBackendRun(this);
	}


	virtual void RenderSceneCB()
	{
		//
	//	glEnable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
	

		


		PointL[0].Color = ballCol;
		m_pGameCamera->OnRender();
		m_scale += 0.01f;
		m_dirLight.Direction = Vector3f(sinf(m_scale)*3.0f, -1.0f, 0.5f + cosf(m_scale)*2.0f);
		m_pLightingTechnique->SetMainLight(m_dirLight);
		m_pLightingTechnique->SetMatSpecularIntensity(intens);
		m_pLightingTechnique->SetMatSpecularPower(power);
		Vector3f tmp = Vector3f(2.0f+cosf(m_scale)*7.0f, 2,2.0f + sinf(m_scale)*7.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);
		PointL[0].Position = tmp;
		m_pLightingTechnique->SetPointLights(1, PointL);
		m_pLightingTechnique->Enable();

		glEnable(GL_STENCIL_TEST);
		// условие всегда выполнено и значение в буфере будет равно 1
		glStencilFunc(GL_ALWAYS, 1, 0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		
		p.Scale(0.1f, 0.1f, 0.1f);
		p.Rotate(0.0f, 0.0f, 0.0f);
		p.WorldPos(0.0f, 0.0f, 5.5f);
		m_pLightingTechnique->SetMVP(p.GetWVPTrans());
		m_pLightingTechnique->SetModel(p.GetWorldTrans());
		
		//glUseProgram(0);
		glDepthMask(FALSE);
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//	m_pLightingTechnique->SetAlfa(0.6f);

		water->Render();
	//	glDisable(GL_BLEND);
		glDepthMask(TRUE);
		glStencilFunc(GL_EQUAL, 0x1, 0xffffffff);
		// ничего не меняем в буфере 
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	//	m_pSphereMesh->Render();

		//m_pSkyBox->Render(0.0f, 0.0f);
		
		
		p.Scale(0.06f, 0.06f, 0.06f);
		p.Rotate(180.0f, 0.0f, 0.0f);
		p.WorldPos(0.0f, -2.0f, 3.0f);
		m_pLightingTechnique->SetMVP(p.GetWVPTrans());
		m_pLightingTechnique->SetModel(p.GetWorldTrans());

		meteor->Render();
		
		
		PointL[0].Position = Vector3f(tmp.x, -3, tmp.z);
		m_pLightingTechnique->SetPointLights(1, PointL);
		p.Scale(0.05f, 0.05f, 0.05f);
		p.Rotate(0.0f, 0.0f, 0.0f);
		p.WorldPos(tmp.x, -5, tmp.z);
		m_pLightingTechnique->SetMVP(p.GetWVPTrans());
		m_pLightingTechnique->SetModel(p.GetWorldTrans());
		ball->Render();
		
		
		m_pSkyBox->Render(180.f, 180.0f, 0.0f);
		glDisable(GL_STENCIL_TEST);
		
		m_pLightingTechnique->Enable();
		PointL[0].Position = tmp;
		m_pLightingTechnique->SetPointLights(1, PointL);
		p.Scale(0.1f, 0.1f, 0.1f);
		p.Rotate(0.0f, 0.0f, 0.0f);
		p.WorldPos(0.0f, 0.0f, 5.5f);
		m_pLightingTechnique->SetMVP(p.GetWVPTrans());
		m_pLightingTechnique->SetModel(p.GetWorldTrans());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		m_pLightingTechnique->SetAlfa(0.99f);

		water->Render();
		glDisable(GL_BLEND);


/*/*	/*	*/
		m_pSkyBox->Render(0.0f, 0.0f,0.0f);
		m_pLightingTechnique->Enable();
		
		m_pLightingTechnique->SetHeightScale(Hieght);
		
		p.Scale(0.1f, 0.1f, 0.1f);
		p.Rotate(0.0f, 0.0f, 0.0f);
		p.WorldPos(0.0f, 0.0f, 3.0f);
		

		
		
	

		m_pLightingTechnique->SetMVP(p.GetWVPTrans());
		m_pLightingTechnique->SetModel(p.GetWorldTrans());
		m_pLightingTechnique->SetAlfa(1.0f);

		//m_pSphereMesh->Render();
		ground->Render();


		p.Scale(0.06f, 0.06f, 0.06f);
		p.Rotate(0.0f, 0.0f, 0.0f);
		p.WorldPos(0.0f, 2.0f, 3.0f);

		m_pLightingTechnique->SetMVP(p.GetWVPTrans());
		m_pLightingTechnique->SetModel(p.GetWorldTrans());

		meteor->Render();

		p.Scale(0.05f, 0.05f, 0.05f);
		p.Rotate(0.0f, 0.0f, 0.0f);
		p.WorldPos(tmp.x, tmp.y, tmp.z);
		m_pLightingTechnique->SetMVP(p.GetWVPTrans());
		m_pLightingTechnique->SetModel(p.GetWorldTrans());
		ball->Render();


		
		
	//	m_pLightingTechnique->SetAlfa(0.3f);
	//	
		

		
		

		
		
		glutSwapBuffers();
	}


	virtual void IdleCB()
	{
		RenderSceneCB();
	}


	virtual void SpecialKeyboardCB(int Key, int x, int y)
	{
		m_pGameCamera->OnKeyboard(Key);
	}


	virtual void KeyboardCB(unsigned char Key, int x, int y)
	{
		switch (Key) {
		case 'q':
			glutLeaveMainLoop();
			break;

		case 'r':
			ballCol = Vector3f(1.0f, 0.0f, 0.0f);
			break;
		case 'g':
			ballCol = Vector3f(0.0f, 1.0f, 0.0f);
			break;
		case 'b':
			ballCol = Vector3f(0.0f, 0.0f, 1.0f);
			break;
		case 'a':
			Hieght += 0.0001f;
			break;
		case 's':
			Hieght -= 0.0001f;
			break;
		case 'P':
			power += 0.01f;
			break;
		case 'p':
			power -= 0.01f;
			break;
		case 'I':
			intens += 0.01f;
			break;
		case 'i':
			intens -= 0.01f;
			break;
			
		}
	}


	virtual void PassiveMouseCB(int x, int y)
	{
		m_pGameCamera->OnMouse(x, y);
	}

private:
	float scale;
	SkyBox* m_pSkyBox;
	PointLight PointL[1];
	Model * wall;
	Pipeline p;
	LightingTechnique* m_pLightingTechnique;
	Camera* m_pGameCamera;
	float m_scale;
	DirectionalLight m_dirLight;
	Mesh* m_pSphereMesh;
	Texture* m_pTexture;
	Texture* m_pNormalMap;
	Texture* m_pTrivialNormalMap;
	PersProjInfo m_persProjInfo;
	bool m_bumpMapEnabled;
	SimpleShader * simp;

	float Hieght = 0;
	float intens = 0;
	float power = 0;
	Model* ground;
	Model* meteor;
	Model* ball;
	Model* water;
	Vector3f ballCol;
	AMesh A_mesh;
};


int main(int argc, char** argv)
{
	GLUTBackendInit(argc, argv);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "Tutorial 26")) {
		return 1;
	}

	Tutorial26* pApp = new Tutorial26();

	if (!pApp->Init()) {
		_getch();
		return 1;
	}

	pApp->Run();

	delete pApp;

	return 0;
}

/*


class Tutorial26 : public ICallbacks
{
public:

    Tutorial26()
    {     
        m_pGameCamera = NULL;        
		LightSh = NULL;
		m_pSkyBox = NULL;		
		scale = 0;

		PointL[0].Attenuation.Linear = 0.0f;	
		PointL[0].Attenuation.Exp = 1.0f;
		PointL[0].Attenuation.Constant = 0.0f;
		PointL[0].Color= Vector3f(1.0f, 0.02f, 0.02f);
		PointL[0].DiffuseIntensity =0.6f;
		PointL[0].AmbientIntensity = 2.0f;

		m_dirLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_dirLight.AmbientIntensity = 0.2f;
		m_dirLight.DiffuseIntensity = 0.2f;
		m_dirLight.Direction = Vector3f(0.0f, 1.0f, 0.0f);
		
     
        
        m_persProjInfo.FOV = 60.0f;
        m_persProjInfo.Height = WINDOW_HEIGHT;
        m_persProjInfo.Width = WINDOW_WIDTH;
        m_persProjInfo.zNear = 0.001f;
        m_persProjInfo.zFar = 1000.0f;        

    }
    

    ~Tutorial26()
    {
        SAFE_DELETE(LightSh);
        SAFE_DELETE(m_pGameCamera);        
          
      //  SAFE_DELETE(m_pTexture);
      //  SAFE_DELETE(m_pNormalMap);
      //  SAFE_DELETE(m_pTrivialNormalMap);
    }

	void LightSet()
	{
		
	}

    bool Init()
    {
		scale = 0;
		Vector3f Pos(0.5f, 1.025f, 0.25f);
		Vector3f Target(0.0f, -0.5f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);
        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);     
		LightSh = new LightingTechnique();	

       if (!LightSh->Init("Light.vs","Light.fs")) {
            printf("Error initializing the lighting technique\n");
            return false;
        } 
	   ground = new Model();	  
	   meteor = new Model();
	   ball = new Model();
	   
	   if (!ground->Init("./Content/Plane.obj", "./Content/tex.jpg")) {
		   return false;
	   }
		if (!ground->Init("./Content/LavaBall.obj", "./Content/lava.jpg")) {
			return false;
		}
		if (!ground->Init("./Content/LittleBall.obj", "./Content/white.png")) {
			return false;
		}
		m_pNormalMap = new Texture(GL_TEXTURE_2D, "./Content/normal_map.jpg");
		if (!m_pNormalMap->Load()) {
			return false;
		}
		
		m_pSkyBox = new SkyBox(m_pGameCamera, m_persProjInfo);
		if (!m_pSkyBox->Init(".",
			"./Content/sp3right.jpg",
			"./Content/sp3left.jpg",
			"./Content/sp3top.jpg",
			"./Content/sp3bot.jpg",
			"./Content/sp3front.jpg",
			"./Content/sp3back.jpg")) {
			return false;
		}
		LightSh->Enable();
		LightSh->SetMainLight(m_dirLight);
		LightSh->SetColorTextureUnit(0);
		LightSh->SetNormalMapTextureUnit(2);

        return true;
		//printf("Doshlo!!!\n");
    }

    
    void Run()
    {
        GLUTBackendRun(this);
    }

    
    virtual void RenderSceneCB()
    {
      
		m_pGameCamera->OnRender();
		scale += 0.01f;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Vector3f tmp = Vector3f(sinf(scale)*5.5f, 5.0f + cosf(scale) * 2, cosf(scale) * 5.5f);
		

		Pipeline p;
		p.Scale(0.5f, 0.5f, 0.5f);
		p.Rotate(0.0f, 0.0f, 0.0f);
		p.WorldPos(0.0f, 0.0f, 3.0f);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);
		

		LightSh->Enable();
		LightSh->SetCamera(m_pGameCamera->GetPos());
		LightSh->SetModel(p.GetWorldTrans());
		LightSh->SetMVP(p.GetWVPTrans());
		PointL[0].Position = tmp;
		LightSh->SetPointLights(1, PointL);
		LightSh->SetMainLight(m_dirLight);
		LightSh->SetColorTextureUnit(0);
		//m_pNormalMap->Bind(NORMAL_TEXTURE_UNIT);

		ground->Render();


		m_pSkyBox->Render();
       
    }




    virtual void IdleCB()
    {
        RenderSceneCB();
    }
    

    virtual void SpecialKeyboardCB(int Key, int x, int y)
    {
        m_pGameCamera->OnKeyboard(Key);
    }


    virtual void KeyboardCB(unsigned char Key, int x, int y)
    {
        switch (Key) {
            case 'q':
                glutLeaveMainLoop();
                break;
          
				
        }
    }


    virtual void PassiveMouseCB(int x, int y)
    {
        m_pGameCamera->OnMouse(x, y);
    }

 private:
	 float scale;
	 SkyBox* m_pSkyBox;
    LightingTechnique* LightSh;
    Camera* m_pGameCamera;
	PersProjInfo m_persProjInfo;
	PointLight PointL[1];
   // float m_scale;
	//float inten;
    DirectionalLight m_dirLight; 

	Model* ground;
	Model* meteor;
	Model* ball;
	


	//Texture* m_pHeight;
    Texture* m_pNormalMap;
  //  Texture* m_pTrivialNormalMap;
  
   // bool m_bumpMapEnabled;
};


int main(int argc, char** argv)
{
    GLUTBackendInit(argc, argv);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "Tutorial 26")) {
        return 1;
    }

    Tutorial26* pApp = new Tutorial26();

    if (!pApp->Init()) {
		_getch();
        return 1;
    }

    pApp->Run();

    delete pApp;
 
    return 0;
}

*/