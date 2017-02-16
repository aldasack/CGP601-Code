////////////////////////////////
// Name: GameManager.h		  //
// Author: Alexander Kocourek //
// Date: 15/02/2017			  //
////////////////////////////////

#include "GameManager.h"

GameManager::GameManager()
{
	m_exitCode = EXIT_SUCCESS;
	m_isRunning = true;
	m_isRendering = true;
	m_isExiting = false;
	m_deltaTime = 0.0f;
	m_targetFrameTime = (1.0f / 60.0f);
	m_windowHeight = 768;
	m_windowWidth = 1024;

	oldTime = g_timer.now();
	newTime = g_timer.now();
	loopOldTime = g_timer.now();
	loopNewTime = g_timer.now();

	for (int i = 0; i < 256; i++)
	{
		m_keyState[i] = false;
	}
}

GameManager::~GameManager(){}

void GameManager::Shutdown()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Shutdown();
		m_gameObjects[i] = nullptr;
		delete m_gameObjects[i];
	}
}

void GameManager::Loop()
{
	if (!m_isRunning)
		return;
	loopOldTime = loopNewTime;
	loopNewTime = g_timer.now();
	m_deltaTime = std::chrono::duration_cast<ms>(loopNewTime - loopOldTime).count() * 0.001f;

	oldTime = g_timer.now();

	draw();
	update();

	newTime = g_timer.now();
	float dt = std::chrono::duration_cast<ms>(newTime - oldTime).count();
	dt *= 0.001f;
	if (dt < m_targetFrameTime)
	{
		float wasteTime = (m_targetFrameTime - dt) * 1000.0f;
		Sleep(wasteTime);
	}
}

void GameManager::InitEngine()
{
	Math::Vector3 v;
	v.x = 0.0f;
	v.y = 5.5f;
	v.z = -5.0;

	m_gameObjects.push_back(new GameObject(v));

	//for (int i = 0; i < 10; i++)
	//{
	//	v.x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 10.0f);
	//	v.y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 10.0f);
	//	//v.z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 10.0f)) * -1.0f;
	//	Sphere* s = new Sphere(v);
	//	m_spheres.push_back(s);
	//	m_rigidBodys.push_back(s);
	//}
}

void GameManager::KeyboardHandle(unsigned char key, int x, int y)
{
	m_keyState[key] = true;

	switch (key)
	{
	case 27: // Escape
		m_isRunning = false;
		this->Shutdown();
		// exit program, only way using GLUT
		std::exit(EXIT_SUCCESS);
		break;

	case 32: // Space
		m_isRunning = !m_isRunning;
		break;

	case 114: // R
		m_isRendering = !m_isRendering;
		break;
	}
}

void GameManager::Exit(int exitCode)
{
	m_exitCode = exitCode;
}

void GameManager::clearKeyState()
{
	for (int i = 0; i < 256; i++)
	{
		m_keyState[i] = false;
	}
}

void GameManager::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_isRendering)
	{
		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			m_gameObjects[i]->Draw();
		}
		//PlaneDraw();
	}

	Math::Vector2 pos{ 10.0f, 750.0f };
	Math::Vector3 color{ 1.0f, 1.0f ,1.0f };
	float fps = 1.0f / m_deltaTime;
	std::string text = "FPS: " + std::to_string(fps);
	text = text.substr(0, 7);
	drawText(pos, color, text);
	pos.y = 720.0f;
	text = "Object count: " + std::to_string(m_gameObjects.size());
	drawText(pos, color, text);

	glutSwapBuffers();
}

void GameManager::drawText(Math::Vector2 position, Math::Vector3 color, std::string text)
{
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, m_windowWidth, 0, m_windowHeight, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glRasterPos2i(position.x, position.y);
	
	for (int i = 0; i < text.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
	}
	
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void GameManager::update()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Update(m_targetFrameTime);
	}
}