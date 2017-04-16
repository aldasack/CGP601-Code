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
	pos1.z = 5.0;

	glm::vec3 pos2;
	pos2.x = -0.5f;
	pos2.y = -0.5f;
	pos2.z = 0.0;

	m_gameObjects.push_back(new Plane());
	m_gameObjects[0]->SetColor(glm::vec3(0.5f, 0.5f, 0.5f));
	//m_gameObjects.push_back(new Sphere(pos1));
	m_gameObjects.push_back(new Box(pos1));
	m_gameObjects[1]->SetColor(Colors::Blue);
	m_gameObjects.push_back(new Box(pos2));
	m_gameObjects[2]->SetColor(Colors::White);
	m_gameObjects[1]->SetRotation(glm::vec3(0.0f, Constants::Deg2Rad * 45.0f, 0.0f));

	m_gameObjects[1]->SetVelocity(glm::vec3(0.0f, 0.0f, -5.0f));
	//m_gameObjects[1]->SetColor(glm::vec3(0.0f, 0.0f, 0.0f));

	glm::vec3 color;
	for (int i = 0; i < 9; i++)
	{
		pos2.x = -10.0f + static_cast<float>(rand()) / (static_cast<float> (RAND_MAX / 10.0f - -10.0f));
		//pos2.y = static_cast<float>(rand()) / (static_cast<float> (RAND_MAX / 5.0f));
		pos2.z = -10.0f + static_cast<float>(rand()) / (static_cast<float> (RAND_MAX / 10.0f - -10.0f));
		color.x = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
		color.y = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
		color.z = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
		m_gameObjects.push_back(new Box(pos2));
		m_gameObjects[i + 3]->SetColor(color);
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
		
		//_CrtDumpMemoryLeaks();
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
	gluLookAt(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
	//gluLookAt(0.0, 2.5, 5.0, 0.0, 2.5, 1.0, 0.0, 1.0, 0.0);


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

//void GameManager::drawPlane()
//{
//	//glLoadIdentity();
//	glColor3f(0.5f, 0.5f, 0.5f);
//
//	glBegin(GL_QUADS);
//	glVertex3f(-100.0f, -1.0f, -100.0f);
//	glVertex3f(-100.0f, -1.0f, 100.0f);
//	glVertex3f(100.0f, -1.0f, 100.0f);
//	glVertex3f(100.0f, -1.0f, -100.0f);
//	glEnd();
//}

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

void GameManager::update()
{
	//// update loop. This loop moves the objects
	//for (int i = 0; i < m_rigidBodys.size(); i++)
	//{
	//	m_rigidBodys[i]->Update(m_targetFrameTime);
	//}

	// collision loop. This loop checks for collision
	for (size_t i = 0; i < m_rigidBodys.size(); i++)
	{
		// Updating the rigidbodys
		m_rigidBodys[i]->Update(m_targetFrameTime);
		
		for (size_t j = 0; j < m_rigidBodys.size(); j++)
		{
/*******************************************************************************/
			// TODO: Fix this hacky code
			// gameObjects[0] is the plane, and the plane has no collider yet
			if (i == 0 || j == 0)
				continue;
/*******************************************************************************/			
			if (&m_rigidBodys[i] != &m_rigidBodys[j])
			{
				if (spheresIntersect(m_rigidBodys[i]->GetSphereCollider(), m_rigidBodys[j]->GetSphereCollider()))
				{
					if (boxIntersects(m_rigidBodys[i]->GetBoxCollider(), m_rigidBodys[j]->GetBoxCollider()))
					{
						collisionResponse(*m_rigidBodys[i], *m_rigidBodys[j]);
					}
				}
			}
		}
	}
}

bool GameManager::spheresIntersect(const Collision::SphereCollider &col1, const Collision::SphereCollider &col2)
{
	float minDis_sqrd = pow(col1.GetRadius() + col2.GetRadius(), 2);
	float realDis_sqrd = pow(col1.GetPosition().x - col2.GetPosition().x, 2) + pow(col1.GetPosition().y - col2.GetPosition().y, 2) + pow(col1.GetPosition().z - col2.GetPosition().z, 2);
	
	if (realDis_sqrd <= minDis_sqrd)
	{
		float intersection = sqrt(minDis_sqrd) - sqrt(realDis_sqrd);
		return true;
	}
	else
	{
		return false;
	}
}

bool GameManager::boxIntersects(const Collision::BoxCollider& col1, const Collision::BoxCollider& col2)
{
	std::array<glm::vec3, 8> edges1 = col1.GetEdges();
	std::array<glm::vec3, 8> edges2 = col2.GetEdges();
	// stores all the calculated dot products (absulute value)
	float c[3][3];
	float dot[3][3];
	float d[3];
	float r, r0, r1;
	//std::vector<float> interpenetration; For contact information only

	glm::vec3 difference = col2.m_center - col1.m_center;

	// 1. Axis: col1 x (0) against col2 x, y & z
	for (int i = 0; i < 3; i++)
	{
		c[0][i] = glm::dot(col1.m_axes[0], col2.m_axes[i]);
		dot[0][i] = abs(c[0][i]);
	}
	d[0] = glm::dot(difference, col1.m_axes[0]);
	r = abs(d[0]);
	r0 = col1.m_extent.x;
	r1 = col1.m_extent.x * dot[0][0] + col1.m_extent.y * dot[0][1] + col1.m_extent.z * dot[0][2];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}
		

	// 2. Axis: col1 y (1) against col2 x, y & z
	for (int i = 0; i < 3; i++)
	{
		c[1][i] = glm::dot(col1.m_axes[1], col2.m_axes[i]);
		dot[1][i] = abs(c[1][i]);
	}
	d[1] = glm::dot(difference, col1.m_axes[1]);
	r = abs(d[1]);
	r0 = col1.m_extent.y;
	r1 = col1.m_extent.x * dot[1][0] + col1.m_extent.y * dot[1][1] + col1.m_extent.z * dot[1][2];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 3. Axis: col1 z (2) against col2 x, y & z
	for (int i = 0; i < 3; i++)
	{
		c[2][i] = glm::dot(col1.m_axes[2], col2.m_axes[i]);
		dot[2][i] = abs(c[2][i]);
	}
	d[2] = glm::dot(difference, col1.m_axes[2]);
	r = abs(d[2]);
	r0 = col1.m_extent.z;
	r1 = col1.m_extent.x * dot[2][0] + col1.m_extent.y * dot[2][1] + col1.m_extent.z * dot[2][2];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 4. Axis: col2 x (1) against col1 x, y & z (dot products already calculated)
	r = abs(glm::dot(difference, col1.m_axes[0]));
	r0 = col1.m_extent.x * dot[0][0] + col1.m_extent.y * dot[1][0] + col1.m_extent.z * dot[2][0];
	r1 = col1.m_extent.z;

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 5. Axis: col2 y (2) against col1 x, y & z (dot products already calculated)
	r = abs(glm::dot(difference, col1.m_axes[1]));
	r0 = col1.m_extent.x * dot[0][1] + col1.m_extent.y * dot[1][1] + col1.m_extent.z * dot[2][1];
	r1 = col1.m_extent.z;

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 6. Axis: col2 z (3) against col1 x, y & z (dot products already calculated)
	r = abs(glm::dot(difference, col1.m_axes[2]));
	r0 = col1.m_extent.x * dot[0][2] + col1.m_extent.y * dot[1][2] + col1.m_extent.z * dot[2][2];
	r1 = col1.m_extent.z;

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 7. Axis: col1 x (1) against col2 x
	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = col1.m_extent.y * dot[2][0] + col1.m_extent.z * dot[1][0];
	r1 = col2.m_extent.y * dot[0][2] + col2.m_extent.z * dot[0][1];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 8. Axis: col1 x (1) against col2 y
	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = col1.m_extent.y * dot[2][1] + col1.m_extent.z * dot[1][1];
	r1 = col2.m_extent.x * dot[0][2] + col2.m_extent.z * dot[0][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 9. Axis: col1 x (1) against col2 z
	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = col1.m_extent.y * dot[2][2] + col1.m_extent.z * dot[1][2];
	r1 = col2.m_extent.x * dot[0][1] + col2.m_extent.y * dot[0][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 10. Axis: col1 y (1) against col2 x
	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = col1.m_extent.x * dot[2][0] + col1.m_extent.z * dot[0][0];
	r1 = col2.m_extent.y * dot[1][2] + col2.m_extent.z * dot[1][1];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}
	
	// 11. Axis: col1 y (1) against col2 y
	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = col1.m_extent.x * dot[2][1] + col1.m_extent.z * dot[0][1];
	r1 = col2.m_extent.x * dot[1][2] + col2.m_extent.z * dot[1][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 12. Axis: col1 y (1) against col2 z
	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = col1.m_extent.x * dot[2][2] + col1.m_extent.z * dot[0][2];
	r1 = col2.m_extent.x * dot[1][1] + col2.m_extent.y * dot[1][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 13. Axis: col1 z (1) against col2 x
	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = col1.m_extent.x * dot[1][0] + col1.m_extent.y * dot[0][0];
	r1 = col2.m_extent.y * dot[2][2] + col2.m_extent.z * dot[2][1];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}
	
	// 14. Axis: col1 z (1) against col2 y
	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = col1.m_extent.x * dot[1][1] + col1.m_extent.y * dot[0][1];
	r1 = col2.m_extent.x * dot[2][2] + col2.m_extent.z * dot[2][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 15. Axis: col1 z (1) against col2 z
	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = col1.m_extent.x * dot[1][2] + col1.m_extent.y * dot[0][2];
	r1 = col2.m_extent.x * dot[2][1] + col2.m_extent.y * dot[2][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

#pragma region Contact Information
/**************************************************************************
	* The following code generates the contact details.
	* SAT is only used as collision culling, hence this code is not needed.
	
	// search smallest interpenetration, excluding zero
	float min = std::numeric_limits<float>::max();
	int index = 0;
	for (int i = 0; i < interpenetration.size(); i++)
	{
		if (interpenetration[i] < min && interpenetration[i] > 0.0f)
		{
			min = interpenetration[i];
			index = i;
		}
	}
	
	// define wich type of contact occured and find the contact data
	if (index < 3)
	{
		glm::vec3 axis = col1.GetAxes()[index];
		// wich face is in contact
		if (glm::dot(axis, difference) > 0.0f)
		{
			axis *= -1.0f;
		}

		// wich vertex is in contact (in col2 local coordinates)
		glm::vec3 vertex = col2.GetExtents();
		if (glm::dot(col2.GetAxes()[0], axis) < 0.0f) vertex.x = -vertex.x;
		if (glm::dot(col2.GetAxes()[1], axis) < 0.0f) vertex.y = -vertex.y;
		if (glm::dot(col2.GetAxes()[2], axis) < 0.0f) vertex.z = -vertex.z;

		// convert to wolrd coordinates
		
		vertex = Math::rotateVector(vertex, col2.GetRotation());
		vertex += col2.GetCenter();
		vertex *= 1.0f;
	}
	else if (index < 6)
	{
		glm::vec3 axis = col2.GetAxes()[index - 3];
		// wich face is in contact
		if (glm::dot(axis, difference * -1.0f) > 0.0f)
		{
			axis *= -1.0f;
		}

		// wich vertex is in contact (in col2 local coordinates)
		glm::vec3 vertex = col1.GetExtents();
		if (glm::dot(col1.GetAxes()[0], axis) < 0.0f) vertex.x = -vertex.x;
		if (glm::dot(col1.GetAxes()[1], axis) < 0.0f) vertex.y = -vertex.y;
		if (glm::dot(col1.GetAxes()[2], axis) < 0.0f) vertex.z = -vertex.z;

		// convert to wolrd coordinates

		vertex = Math::rotateVector(vertex, col1.GetRotation());
		vertex += col1.GetCenter();
		vertex *= 1.0f;
	}
	else if (index < 15)
	{
		// find axis of contact
		index -= 6;

	}
*********************************************************************************/
#pragma endregion
	
	return true;
}

void GameManager::collisionResponse(RigidBody& g1, RigidBody& g2)
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
}