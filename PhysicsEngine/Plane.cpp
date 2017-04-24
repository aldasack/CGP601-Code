////////////////////////////////
// Name: Plane.cpp			  //
// Author: Alexander Kocourek //
// Date: 14/03/2017			  //
////////////////////////////////

#include "Plane.h"

#include "RigidBody.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "MeshCollider.h"

Plane::Plane() : GameObject()
{
	m_pRigidbody->IsStatic(true);
	m_normal.x = 0.0f;
	m_normal.y = 1.0f;
	m_normal.z = 0.0f;

	setVertices();
	m_pRigidbody->GetSphereCollider().AdjustCollider();
	m_pRigidbody->GetBoxCollider().AdjustCollider();
	m_pRigidbody->GetMeshCollider().AdjustCollider(m_vertices);
}

Plane::Plane(const glm::vec3 position) : GameObject(position)
{
	m_pRigidbody->IsStatic(true);
	m_normal.x = 0.0f;
	m_normal.y = 1.0f;
	m_normal.z = 0.0f;

	setVertices();
	m_pRigidbody->GetSphereCollider().AdjustCollider();
	m_pRigidbody->GetBoxCollider().AdjustCollider();
	m_pRigidbody->GetMeshCollider().AdjustCollider(m_vertices);
}

Plane::Plane(const glm::vec3 position, const glm::vec3 scale) : GameObject(position)
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

	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		glVertex3f(m_vertices[i].x, m_vertices[i].y, m_vertices[i].z);
	}


	glEnd();

	glPopMatrix();
}

void Plane::setVertices()
{
	m_vertices.push_back(glm::vec3(-100.0f, -1.0f, -100.0f));
	m_vertices.push_back(glm::vec3(-100.0f, -1.0f, 100.0f));
	m_vertices.push_back(glm::vec3(100.0f, -1.0f, 100.0f));
	m_vertices.push_back(glm::vec3(100.0f, -1.0f, -100.0f));
}