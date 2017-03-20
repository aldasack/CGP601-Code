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
	// setting / calculating a inertia tensor for a sphere
	setInertiaTensor();
}

Sphere::Sphere(const glm::vec3 position) : GameObject(position)
{
	m_radius = 0.5f;
	m_collider.position = position;
	m_collider.radius = m_radius;

	// setting / calculating a inertia tensor for a sphere
	setInertiaTensor();
}

Sphere::Sphere(const glm::vec3 position, const float radius) : GameObject(position)
{
	m_radius = radius;
	m_collider.position = m_pRigidbody->GetPosition();
	m_collider.radius = m_radius;
	
	// setting / calculating a inertia tensor for a sphere
	setInertiaTensor();
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
	//glutSolidCube(1.0);
	glPopMatrix();
}

void Sphere::SetRadius(const float radius)
{
	m_radius = radius;
	setInertiaTensor();
}

void Sphere::setInertiaTensor()
{
	glm::mat3 tensor = glm::mat3();
	tensor *= (2.0f / 5.0f) * m_pRigidbody->GetMass() * pow(m_radius, 2);
	m_pRigidbody->SetInertiaTensor(tensor);
}