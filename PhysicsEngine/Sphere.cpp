////////////////////////////////
// Name: Sphere.cpp			  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include "Sphere.h"

Sphere::Sphere() : GameObject()
{
	m_radius = 0.5f;
	m_collider.position = m_pRigidbody->GetPosition();
	m_collider.radius = m_radius;
}

Sphere::Sphere(const glm::vec3 position) : GameObject(position)
{
	m_radius = 0.5f;
	m_collider.position = position;
	m_collider.radius = m_radius;
}

Sphere::Sphere(const glm::vec3 position, const float radius) : GameObject(position)
{
	m_radius = radius;
	m_collider.position = m_pRigidbody->GetPosition();
	m_collider.radius = m_radius;
}

Sphere::~Sphere(){}

void Sphere::Shutdown()
{
	shutdown();
}

void Sphere::Draw()
{
	prepareDraw();
	glutSolidSphere(m_radius, m_segments, m_segments);
	glPopMatrix();
}

void Sphere::SetRadius(const float radius)
{
	m_radius = radius;
}