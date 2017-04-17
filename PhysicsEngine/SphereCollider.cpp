////////////////////////////////
// Name: SphereCollider.cpp	  //
// Author: Alexander Kocourek //
// Date: 22/03/2017			  //
////////////////////////////////

#include "SphereCollider.h"

#include "GameObject.h"

using namespace Collision;

SphereCollider::SphereCollider(RigidBody& rigidBody) : Collider(rigidBody)
{
	m_position = m_pRigidBody->GetPosition();
	m_offset = { 0.0f, 0.0f, 0.0f };
	m_radius = 0.5f;
}

SphereCollider::~SphereCollider(){}

void SphereCollider::Update()
{
	m_position = m_pRigidBody->GetPosition() + m_offset;
}

void SphereCollider::AdjustCollider()
{
	std::vector<glm::vec3>& vertices = m_pRigidBody->GetGameObject().GetVertices();

	// Calcualte centre
	glm::vec3 minBounds = vertices[0];
	glm::vec3 maxBounds = vertices[0];
	for (int i = 1; i < vertices.size(); i++)
	{
		glm::vec3 vertex = vertices[i];
		if (minBounds.x > vertex.x || minBounds.y > vertex.y || minBounds.z > vertex.z)
			minBounds = vertex;

		if (maxBounds.x < vertex.x || maxBounds.y < vertex.y || maxBounds.z < vertex.z)
			maxBounds = vertex;
	}
	m_offset.x = minBounds.x + ((maxBounds.x - minBounds.x) / 2);
	m_offset.y = minBounds.y + ((maxBounds.y - minBounds.y) / 2);
	m_offset.z = minBounds.z + ((maxBounds.z - minBounds.z) / 2);	
	
	// Calculate radius
	float maxDistance = 0.0f;
	for (int i = 1; i < vertices.size(); i++)
	{
		float distance = glm::length(vertices[i] - m_offset);
		if (distance > maxDistance)
			maxDistance = distance;
	}
	m_radius = maxDistance;
}

void SphereCollider::SetPosition(const glm::vec3& position)
{
	m_position = position;
}

glm::vec3 SphereCollider::GetPosition() const
{
	return m_position;
}

void SphereCollider::SetRadius(const float radius)
{
	// Radius can't be zero or negative
	DBG_ASSERT(radius > 0.0f);
	m_radius = radius;
}

float SphereCollider::GetRadius() const
{
	return m_radius;
}