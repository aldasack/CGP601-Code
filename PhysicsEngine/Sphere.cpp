////////////////////////////////
// Name: Sphere.cpp			  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include "Sphere.h"
#include "RigidBody.h"

Sphere::Sphere() : GameObject(Collision::ColliderType::Sphere)
{
	m_radius = 0.5f;
	//m_collider.position = m_pRigidbody->GetPosition();
	//m_collider.radius = m_radius;
	// setting / calculating a inertia tensor for a sphere
	setInertiaTensor();
}

Sphere::Sphere(const glm::vec3 position) : GameObject(Collision::ColliderType::Sphere, position)
{
	m_radius = 0.5f;
	/*m_collider.position = position;
	m_collider.radius = m_radius;*/

	// setting / calculating a inertia tensor for a sphere
	setInertiaTensor();
}

Sphere::Sphere(const glm::vec3 position, const float radius) : GameObject(Collision::ColliderType::Sphere, position)
{
	m_radius = radius;
	/*m_collider.position = m_pRigidbody->GetPosition();
	m_collider.radius = m_radius;*/
	
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
	DBG_ASSERT(radius > 0.0f);
	if (radius <= 0.0f)
	{
		throw std::exception("Radius can't be zero or negative!");
		return;
	}
	m_radius = radius;
	setInertiaTensor();
}

void Sphere::setInertiaTensor()
{
	glm::mat3 tensor = glm::mat3();
	tensor *= (2.0f / 5.0f) * m_pRigidbody->GetMass() * pow(m_radius, 2);
	m_pRigidbody->SetInertiaTensor(tensor);
}