////////////////////////////////
// Name: Box.cpp		      //
// Author: Alexander Kocourek //
// Date: 27/03/2017			  //
////////////////////////////////

#include "Box.h"

#include "RigidBody.h"
#include "MeshCollider.h"

Box::Box() : GameObject()
{	
	// Set the vertices
	setVertices();
	m_pRigidbody->GetMeshCollider().SetVertices(m_vertices);
}

Box::Box(const glm::vec3& position) : GameObject(position)
{
	setVertices();
	m_pRigidbody->GetMeshCollider().SetVertices(m_vertices);
}

Box::~Box(){}

void Box::Shutdown()
{
	shutdown();
}

void Box::Draw()
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

void Box::setVertices()
{
	 //front face (normal pointing +z)
	m_vertices.push_back(glm::vec3(-0.5f, 1.0f, 0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 1.0f, 0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 0.0f, 0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 0.0f, 0.5f));

	// right face (normal pointing +x)
	m_vertices.push_back(glm::vec3(0.5f, 1.0f, 0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 1.0f, -0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 0.0f, -0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 0.0f, 0.5f));

	// left face (normal pointing -x)
	m_vertices.push_back(glm::vec3(-0.5f, 1.0f, 0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 1.0f, -0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 0.0f, -0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 0.0f, 0.5f));

	//back face (normal pointing -z)
	m_vertices.push_back(glm::vec3(0.5f, 1.0f, -0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 1.0f, -0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 0.0f, -0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 0.0f, -0.5f));

	// top face (normal pointing +y)
	m_vertices.push_back(glm::vec3(-0.5f, 1.0f, -0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 1.0f, -0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 1.0f, 0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 1.0f, 0.5f));

	// bottom face (normal pointing -y)
	m_vertices.push_back(glm::vec3(0.5f, 0.0f, 0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 0.0f, 0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 0.0f, -0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 0.0f, -0.5f));
}