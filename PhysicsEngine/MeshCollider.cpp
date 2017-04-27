////////////////////////////////
// Name: MeshCollider.cpp	  //
// Author: Alexander Kocourek //
// Date: 13/04/2017			  //
////////////////////////////////

#include "MeshCollider.h"

#include "GameObject.h"

using namespace Collision;

MeshCollider::MeshCollider(const RigidBody& rigidbody) : Collider(rigidbody)
{
	m_pLocalVertices = nullptr;
	m_pWorldVertices = new std::vector<glm::vec3>();
}

MeshCollider::~MeshCollider()
{
}

void MeshCollider::Shutdown()
{
	m_pLocalVertices = nullptr;

	if (m_pWorldVertices)
	{
		delete m_pWorldVertices;
		m_pWorldVertices = nullptr;
	}
	Collider::Shutdown();
}

void MeshCollider::Update()
{
	// Rotate and translate vertices
	for (size_t i = 0; i < m_pLocalVertices->size(); i++)
	{
		m_pWorldVertices->operator[](i) = m_pLocalVertices->operator[](i);
		// Scale
		m_pWorldVertices->operator[](i) *= m_pRigidBody->GetGameObject().GetScale();
		// Rotate
		m_pWorldVertices->operator[](i) = Math::rotateVector(m_pWorldVertices->operator[](i), m_pRigidBody->GetQuaternionRotation());
		// Translate
		m_pWorldVertices->operator[](i) += m_pRigidBody->GetPosition();
	}
	//// Translate vertices
	//for (size_t i = 0; i < m_pLocalVertices->size(); i++)
	//{
	//	m_pWorldVertices->operator[](i) = m_pWorldVertices->operator[](i) + m_pRigidBody->GetPosition();
	//}
	//TODO: Transform vertices
}

void MeshCollider::AdjustCollider(std::vector<glm::vec3>& vertices)
{
	m_pLocalVertices = &vertices;
	DBG_ASSERT(m_pLocalVertices->size() > 0);
	m_pWorldVertices->reserve(m_pLocalVertices->size());

	// Copy vertices
	for (size_t i = 0; i < m_pLocalVertices->size(); i++)
	{
		m_pWorldVertices->push_back(m_pLocalVertices->operator[](i));
	}

	// Apply transformation
	Update();
}

std::vector<glm::vec3>& MeshCollider::GetVertices() const
{
	//DBG_ASSERT(m_pWorldVertices->size() != 0);
	return *m_pWorldVertices;
}

void MeshCollider::SetVertices(std::vector<glm::vec3>& vertices)
{
	DBG_ASSERT(false);
	DBG_ASSERT(vertices.size() > 0);
	m_pLocalVertices = &vertices;
	m_pWorldVertices->reserve(vertices.size());
	
	// Translate local coordinates
	for (size_t i = 0; i < m_pLocalVertices->size(); i++)
	{
		m_pWorldVertices->push_back(m_pLocalVertices->operator[](i) + m_pRigidBody->GetPosition());
	}
}