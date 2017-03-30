////////////////////////////////
// Name: Plane.cpp			  //
// Author: Alexander Kocourek //
// Date: 14/03/2017			  //
////////////////////////////////

#include "Plane.h"

#include "RigidBody.h"

Plane::Plane() : GameObject(Collision::ColliderType::Plane)
{
	m_pRigidbody->IsStatic(true);
	m_normal.x = 0.0f;
	m_normal.y = 1.0f;
	m_normal.z = 0.0f;
}

Plane::Plane(const glm::vec3 position) : GameObject(Collision::ColliderType::Plane, position)
{
	m_pRigidbody->IsStatic(true);
}

Plane::Plane(const glm::vec3 position, const glm::vec3 scale) : GameObject(Collision::ColliderType::Plane, position)
{
	m_pRigidbody->IsStatic(true);
}

Plane::~Plane(){}

void Plane::Shutdown()
{
	shutdown();
}

void Plane::Draw()
{
	prepareDraw();

	glBegin(GL_QUADS);
	glVertex3f(-100.0f, -1.0f, -100.0f);
	glVertex3f(-100.0f, -1.0f, 100.0f);
	glVertex3f(100.0f, -1.0f, 100.0f);
	glVertex3f(100.0f, -1.0f, -100.0f);
	glEnd();

	glPopMatrix();
}