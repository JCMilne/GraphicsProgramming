#include "MainScene.h"
#include "SceneCamera.h"
#include <iostream>
#include <string>


ModelTransform transform;

MainScene::MainScene()
{
	_sceneState = sceneState::Active;  //sets the state od the scene to active
	SceneDisplay* _sceneDisplay = new SceneDisplay(); //creates new display
	//creates objects for models and shaders
    ObjectMesh* model1();
	ObjectMesh* model2();
	ObjectMesh* model3();
	ObjectMesh* model4();
	ObjectMesh* model5();
	ShaderClass* adsShader();
	ShaderClass* blurShader();
	ShaderClass* toonShader();
	ShaderClass* rimLightShader();
	ShaderClass* toonRimShader();
	ShaderClass* toonRimFogShader();
}

MainScene::~MainScene() 
{
}

void MainScene::runProgram()  //runs the programme
{
	initalise(); 
	mainLoop();
}

void MainScene::initalise()
{
	//initalizes the display
	_sceneDisplay.createDisplay(); 

	//loading models and shaders from their external files
	adsShader.InitaliseShader("..\\res\\ADSshader.vert","..\\res\\ADSshader.frag");
	blurShader.InitaliseShader("..\\res\\shaderBlur.vert", "..\\res\\shaderBlur.frag");
	toonShader.InitaliseShader("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag");
	rimLightShader.InitaliseShader("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	toonRimShader.InitaliseShader("..\\res\\shaderToonRim.vert", "..\\res\\shaderToonRim.frag");
	toonRimFogShader.InitaliseShader("..\\res\\shaderToonRimFog.vert", "..\\res\\shaderToonRimFog.frag");

	model1.loadModelFromFile("..\\res\\cone.obj");
	model2.loadModelFromFile("..\\res\\monkey3.obj");
	model3.loadModelFromFile("..\\res\\box.obj");
	model4.loadModelFromFile("..\\res\\ring.obj");
	model5.loadModelFromFile("..\\res\\sphere.obj");
	
	//creates a camera
	sceneCamera.createCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_sceneDisplay.getDisplayWidth()/_sceneDisplay.getDisplayHeight(), 0.01f, 1000.0f);

	//initalises the timer
	timer = 1.0f;
}

void MainScene::mainLoop()
{
	while (_sceneState != sceneState::Close)
	{
		QuitFunction();
		drawScene();
		timer = timer + 0.01f;
	}
}

void MainScene::QuitFunction()
{
	SDL_Event Event;

	while(SDL_PollEvent(&Event)) //get and process events
	{
		switch (Event.type)
		{
			case SDL_QUIT:
				_sceneState = sceneState::Close;
				break;
		}
	}
	
}

void MainScene::toonLighting()
{
	toonShader.setVec3("lightDirection", glm::vec3(0.5, 0.5, 0.5));
}

void MainScene::rimShader()
{
	rimLightShader.setMat4("u_vm", sceneCamera.GetView());
	rimLightShader.setMat4("u_pm", sceneCamera.GetProjection());
}

void MainScene::toonRimLighting()
{
	toonRimShader.setVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));
	toonRimShader.setMat4("u_vm", sceneCamera.GetView());
	toonRimShader.setMat4("u_pm", sceneCamera.GetProjection());
}

void MainScene::ToonRimFog()
{
	toonRimFogShader.setVec3("lightDir", glm::vec3(0.8, 0.8, 0.8)); 
	
	toonRimFogShader.setMat4("u_vm", sceneCamera.GetView());
	toonRimFogShader.setMat4("u_pm", sceneCamera.GetProjection());

	toonRimFogShader.setVec3("fogColor", glm::vec3(0.2, 0.2, 0.2));
	toonRimFogShader.setFloat("minDistance", -5.0f);
	toonRimFogShader.setFloat("maxDistance", 5.0f);
}

void MainScene::blurLighting()
{
	blurShader.setVec4("innerColor", glm::vec4(1.0f, 1.0f, 0.75f, 1.0f));
	blurShader.setVec4("outerColor", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	blurShader.setFloat("innerRadius", 0.0f);
	blurShader.setFloat("outerRadius", 0.75f);
}

void MainScene::drawScene()
{
	_sceneDisplay.clearScreenDisplay(0.2f, 0.2f, 0.2f, 1.0f);

	modelDraw(1);
	model1.drawModel();
	
	modelDraw(2);
	model2.drawModel();

	modelDraw(3);
	model3.drawModel();

	modelDraw(4);
	model4.drawModel();

	modelDraw(5);
	model5.drawModel();

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_sceneDisplay.changeBuffers();
} 

void MainScene::modelDraw(int Index)
{
	switch (Index)
	{
		case(1):
		{
			transform.SetPosition(glm::vec3(4.0, -sinf(timer),0));
			transform.SetRotation(glm::vec3(270, timer * 5, 0));
			transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

			toonRimShader.BindShader();
			toonRimLighting();
			toonRimShader.Update(transform, sceneCamera);
		}
		break;

		case(2):
		{
			transform.SetPosition(glm::vec3(0.0, -1.0, -sinf(timer) * 5));
			transform.SetRotation(glm::vec3(0.0, 90, 0.0));
			transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

			toonRimFogShader.BindShader();
			ToonRimFog();
			toonRimFogShader.Update(transform, sceneCamera);
			cout << transform.GetPosition()->z << std::endl;
		}
		break;
		
		case(3):
		{
			transform.SetPosition(glm::vec3(-4.0, -sinf(timer), 0.0));
			transform.SetRotation(glm::vec3(270, timer * 5, 0));
			transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

			blurShader.BindShader();
			blurLighting();
			blurShader.Update(transform, sceneCamera);
		}
		break;		

		case(4):
		{
			transform.SetPosition(glm::vec3(0.0, -sinf(timer), 0.0));
			transform.SetRotation(glm::vec3(timer * 5, 0.0, -sinf(timer)));
			transform.SetScale(glm::vec3(0.2, 0.2, 0.2));

			toonShader.BindShader();
			toonLighting();
			toonShader.Update(transform, sceneCamera);
		}
		break;

		case(5):
		{
			transform.SetPosition(glm::vec3(sinf(timer), 0.5, 0.0));
			transform.SetRotation(glm::vec3(timer * 5, timer * 5, 0.0));
			transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

			rimLightShader.BindShader();
			rimShader();
			rimLightShader.Update(transform, sceneCamera);
		}
		break;
	}
}