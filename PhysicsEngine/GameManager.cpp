////////////////////////////////
// Name: GameManager.h		  //
// Author: Alexander Kocourek //
// Date: 15/02/2017			  //
////////////////////////////////

#include "GameManager.h"

#include "PhysicsManager.h"
#include "RigidBody.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "GameObject.h"

GameManager* GameManager::s_instance = nullptr;

GameManager::GameManager()
{
	m_exitCode = EXIT_SUCCESS;
	m_isRunning = true;
	m_isRendering = true;
	m_isExiting = false;
	m_deltaTime = 0.0f;
	m_targetFrameTime = (1.0f / 60.0f);

	oldTime = g_timer.now();
	newTime = g_timer.now();
	loopOldTime = g_timer.now();
	loopNewTime = g_timer.now();

	for (int i = 0; i < 256; i++)
	{
		m_keyState[i] = false;
	}

	m_pPhysicsManager = &PhysicsManager::GetInstance();
}

GameManager::~GameManager(){}

void GameManager::Shutdown()
{
	for (size_t i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i])
		{
			m_gameObjects[i]->Shutdown();
			delete m_gameObjects[i];
			m_gameObjects[i] = nullptr;
		}
	}
	
	m_pPhysicsManager->Shutdown();

	for (size_t i = 0; i < m_rigidBodys.size(); i++)
	{
		// the rigidbodys gets deleted when corresponding gameobject gets deleted
		// Although bad code, because vector items point to an unvalid object
		/*if (m_rigidBodys[i])
		{
			m_rigidBodys[i]->Shutdown();
			delete m_rigidBodys[i];
			m_rigidBodys[i] = nullptr;
		}*/
		m_rigidBodys[i] = nullptr;
	}

	delete s_instance;
	s_instance = nullptr;
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
	//update();
	m_pPhysicsManager->Update(m_deltaTime);

	newTime = g_timer.now();
	float dt = std::chrono::duration_cast<ms>(newTime - oldTime).count();
	dt *= 0.001f;
	if (dt < m_targetFrameTime)
	{
		float wasteTime = (m_targetFrameTime - dt) * 1000.0f;
		Sleep(wasteTime);
	}
}

void GameManager::InitScene()
{
	srand(static_cast<unsigned>(time(0)));
	glm::vec3 pos1;
	pos1.x = 0.0f;
	pos1.y = -0.5f;
	pos1.z = 0.0f;

	glm::vec3 pos2;
	pos2.x = 0.0f;
	pos2.y = 1.1f;
	pos2.z = 0.0;

	m_gameObjects.push_back(new Plane(glm::vec3(0.0f, -1.0f, 0.0f)));
	m_gameObjects[0]->SetColor(glm::vec3(0.5f, 0.5f, 0.5f));
	m_gameObjects[0]->SetScale(glm::vec3(100.0f, 1.0f, 100.0f));
	m_gameObjects[0]->IsStatic(true);
	//m_gameObjects.push_back(new Sphere(pos1));
	m_gameObjects.push_back(new Box(pos1));
	m_gameObjects[1]->SetColor(Colors::Blue);
	m_gameObjects[1]->UseGravity(false);
	m_gameObjects.push_back(new Box(pos2));
	m_gameObjects[2]->SetColor(Colors::White);
	m_gameObjects[2]->UseGravity(true);
	//m_gameObjects[1]->SetRotation(glm::vec3(0.0f, Constants::Deg2Rad * 45.0f, 0.0f));
	//m_gameObjects[2]->SetRotation(glm::vec3(0.0f, Constants::Deg2Rad * 45.0f, 0.0f));

	//m_gameObjects[1]->SetVelocity(glm::vec3(0.0f, 0.0f, -1.0f));
	//m_gameObjects[1]->AddTorque(glm::vec3(0.0f, 10.0f, 0.0f));
	//m_gameObjects[1]->SetColor(glm::vec3(0.0f, 0.0f, 0.0f));

//	m_gameObjects.push_back(new Sphere());

	/*glm::vec3 color;
	for (int i = 0; i < 9; i++)
	{
		pos2.x = -10.0f + static_cast<float>(rand()) / (static_cast<float> (RAND_MAX / 10.0f - -10.0f));
		pos2.y = static_cast<float>(rand()) / (static_cast<float> (RAND_MAX / 5.0f));
		pos2.z = -10.0f + static_cast<float>(rand()) / (static_cast<float> (RAND_MAX / 10.0f - -10.0f));
		color.x = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
		color.y = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
		color.z = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
		m_gameObjects.push_back(new Box(pos2));
		m_gameObjects[i + 3]->SetColor(color);
	}*/
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
		
		//_CrtDumpMemoryLeaks();
		std::exit(EXIT_SUCCESS);
		
		break;

	case 32: // Space
		m_isRunning = !m_isRunning;
		break;

	case 43: // Plus
		// changing simulation speed
		m_targetFrameTime *= 0.5f;
		break;
	
	case 45: // Minus
		// changing simulation speed
		m_targetFrameTime *= 2.0f;
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

void GameManager::AddRigidBody(RigidBody& rigidbody)
{
	DBG_ASSERT(&rigidbody != nullptr);
	m_rigidBodys.push_back(&rigidbody);
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
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1);
	
	glViewport(0.0f, 0.0f, m_windowWidth, m_windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0, (GLdouble)m_windowWidth / (GLdouble)m_windowHeight, 1.0, 100.0);

	// set camera / view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// looking from above
	//gluLookAt(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
	gluLookAt(5.0, 2.5, 0.0, 0.0, 2.5, 0.0, 0.0, 1.0, 0.0);


	//DrawCube();
	/*glViewport(0.0f, 0.0f, m_windowWidth, m_windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0, (GLdouble)m_windowWidth / (GLdouble)m_windowHeight, 1.0, 100.0);*/
	
	//drawPlane();
	if (m_isRendering)
	{
		for (size_t i = 0; i < m_gameObjects.size(); i++)
		{
			m_gameObjects[i]->Draw();
		}
	}

#pragma region stats
	// Drawing stats to the screen
	glm::vec2 pos{ 10.0f, 750.0f };
	glm::vec3 color{ 1.0f, 1.0f ,1.0f };
	float fps = 1.0f / m_deltaTime;
	std::string text = "FPS: " + std::to_string(fps);
	text = text.substr(0, 7);
	drawText(pos, color, text);
	pos.y = 720.0f;
	text = "Object count: " + std::to_string(m_gameObjects.size());
	drawText(pos, color, text);
#pragma endregion

	//glFlush();
	glutSwapBuffers();
}

void GameManager::drawText(glm::vec2 position, glm::vec3 color, std::string text)
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
	glRasterPos2f(position.x, position.y);
	glColor3i(1.0f, 1.0f, 1.0f);

	for (size_t i = 0; i < text.size(); i++)
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