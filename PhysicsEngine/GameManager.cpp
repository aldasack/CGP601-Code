////////////////////////////////
// Name: GameManager.h		  //
// Author: Alexander Kocourek //
// Date: 15/02/2017			  //
////////////////////////////////

#include <time.h>

#include "GameManager.h"

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
	srand(static_cast<unsigned>(time(0)));
	glm::vec3 pos1;
	pos1.x = 0.5f;
	pos1.y = 5.5f;
	pos1.z = -5.0;

	glm::vec3 pos2;
	pos2.x = 0.0f;
	pos2.y = 5.5f;
	pos2.z = 0.0;

	m_gameObjects.push_back(new Sphere(pos1));
	m_gameObjects.push_back(new Sphere(pos2));
	pos2.x = 1.0f;
	//m_gameObjects.push_back(new Sphere(pos2));

	m_gameObjects[0]->SetVelocity(glm::vec3(-2.0f, 0.0f, -0.0f));
	//m_gameObjects[1]->SetVelocity(glm::vec3(0.0f, 0.0f, -2.0f));
	m_gameObjects[0]->SetColor(Colors::Blue);
	//m_gameObjects[0]->IsStatic(true);
	//m_gameObjects[1]->UseGravity(false);

	glm::vec3 color;
	for (int i = 0; i < 9; i++)
	{
		pos2.x = -10.0f + static_cast<float>(rand()) / (static_cast<float> (RAND_MAX / 10.0f - -10.0f));
		pos2.y = static_cast<float>(rand()) / (static_cast<float> (RAND_MAX / 5.0f));
		pos2.z = -10.0f + static_cast<float>(rand()) / (static_cast<float> (RAND_MAX / 10.0f - -10.0f));
		color.x = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
		color.y = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
		color.z = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
		m_gameObjects.push_back(new Sphere(pos2));
		m_gameObjects[i + 2]->SetColor(color);
	}
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

	case 43: // Plus
		// changing simulation speed
		m_targetFrameTime *= 2.0f;
		break;
	
	case 45: // Minus
		// changing simulation speed
		m_targetFrameTime *= 0.5f;
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
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1);
	// set camera / view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// looking from above
	//gluLookAt(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
	gluLookAt(0.0, 2.5, 5.0, 0.0, 2.5, 1.0, 0.0, 1.0, 0.0);

	/*glViewport(0.0f, 0.0f, m_windowWidth, m_windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0, (GLdouble)m_windowWidth / (GLdouble)m_windowHeight, 1.0, 100.0);*/
	
	drawPlane();
	if (m_isRendering)
	{
		for (int i = 0; i < m_gameObjects.size(); i++)
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

void GameManager::drawPlane()
{
	//glLoadIdentity();
	glColor3f(0.5f, 0.5f, 0.5f);

	glBegin(GL_QUADS);
	glVertex3f(-100.0f, -1.0f, -100.0f);
	glVertex3f(-100.0f, -1.0f, 100.0f);
	glVertex3f(100.0f, -1.0f, 100.0f);
	glVertex3f(100.0f, -1.0f, -100.0f);
	glEnd();
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
	//m_gameObjects[0]->AddForce(glm::vec3(-1.0f, 0.0f, 0.0f));
	//m_gameObjects[1]->AddForce(glm::vec3(1.0f, 0.0f, 0.0f));

	// update loop. This loop moves the objects
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Update(m_targetFrameTime);
	}
	
	// collision loop. This loop checks for collision
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		for (int j = 0; j < m_gameObjects.size(); j++)
		{
			if (m_gameObjects[i] != m_gameObjects[j])
			{
				Collider::SphereCollider col1 = m_gameObjects[i]->GetCollider();
				Collider::SphereCollider col2 = m_gameObjects[j]->GetCollider();
				float inters = collisionDetection(col1, col2);
				//if (inters >= 0.0f && b == false)
				if (inters >= 0.0f)
				{
					/*m_gameObjects[i]->SetColor(Colors::Red);
					m_gameObjects[j]->SetColor(Colors::Red);*/
					collisionResponse(*m_gameObjects[i], *m_gameObjects[j], inters);
					//b = true;
				}
			}
		}
	}
}

// returning lenght of intersection. returns -1 if no collsision was detected
float GameManager::collisionDetection(const Collider::SphereCollider &col1, const Collider::SphereCollider &col2)
{
	float minDis_sqrd = pow(col1.radius + col2.radius, 2);
	float realDis_sqrd = pow(col1.position.x - col2.position.x, 2) + pow(col1.position.y - col2.position.y, 2) + pow(col1.position.z - col2.position.z, 2);
	
	if (realDis_sqrd <= minDis_sqrd)
	{
		float intersection = sqrt(minDis_sqrd) - sqrt(realDis_sqrd);
		return intersection;
	}
	else
	{
		return -1.0f;
	}
}

void GameManager::collisionResponse(GameObject& g1, GameObject& g2, float instersection)
{
	glm::vec3 v1 = g1.GetVelocity();
	glm::vec3 v2 = g2.GetVelocity();
	float m1 = g1.GetMass();
	float m2 = g2.GetMass();
	float im1 = 1 / m1; // inverse mass
	float im2 = 1 / m2;
	glm::vec3 pos1 = g1.GetPosition();
	glm::vec3 pos2 = g2.GetPosition();
	glm::vec3 collisionNormal = glm::normalize(pos1 - pos2);

	// checking if collision response is needed
	// if objects move towards each other
	glm::vec3 relativeVelocity = v1 - v2;
	float separatingVelocity = glm::dot(relativeVelocity, collisionNormal);
	if (separatingVelocity > 0.0f)
		return;

	// making velocity positive
	//separatingVelocity *= -1.0f;

	// involve bounciness / restitution
	float newSeparatingVelocity = -separatingVelocity * 1.0f;
	float deltaVelocity = newSeparatingVelocity - separatingVelocity;

	// calculating the impuls (p). p = m * v
	//float p = (m1 + m2) * separatingVelocity;
	float p = deltaVelocity / (im1 + im2);

	// involving the collision normal
	glm::vec3 pv = p * collisionNormal;
	
	// set new velocity / apply impulse to objects
	g1.SetVelocity(v1 + pv * im1);
	g2.SetVelocity(v2 + pv * -im2);


	/////////////////////////////////////////////////////////////////////////////////////////////
	// only for central elastic pulse
	//glm::vec3 u1 = ((m1 - m2) * v1 + 2.0f * m2 * v2) / (m1 + m2); // velocity after collisison
	////u1 = u1 * collisionNormal;
	//g1.SetVelocity(u1);
	//
	//glm::vec3 u2 = ((m2 - m1) * v2 + 2.0f * m1 * v1) / (m1 + m2); // velocity after collisison
	////u2 = u2 * collisionNormal;
	//g2.SetVelocity(u2);
	/////////////////////////////////////////////////////////////////////////////////////////////
}