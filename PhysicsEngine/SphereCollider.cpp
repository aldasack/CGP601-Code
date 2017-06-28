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
	m_center = m_pRigidBody->GetPosition();
	m_offset = { 0.0f, 0.0f, 0.0f };
	m_radius = 0.5f;
}

SphereCollider::~SphereCollider(){}

void SphereCollider::Update()
{
	m_center = m_pRigidBody->GetPosition() + m_offset;
	
	glm::vec3& scale = m_pRigidBody->GetGameObject().GetScale();
	float factor = scale.x;
	if (scale.y > factor)
		factor = scale.y;
	if (scale.z > factor)
		factor = scale.z;
	m_radius *= factor;
}

void SphereCollider::AdjustCollider()
{
	std::vector<glm::vec3>& vertices = m_pRigidBody->GetGameObject().GetVertices();

	// Calculating center / offset from GameObject origin
	glm::vec3 minBounds = vertices[0];
	glm::vec3 maxBounds = vertices[0];
	for (size_t i = 1; i < vertices.size(); i++)
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
	for (size_t i = 1; i < vertices.size(); i++)
	{
		float distance = glm::length(vertices[i] - m_offset);
		if (distance > maxDistance)
			maxDistance = distance;
	}
	m_radius = maxDistance;
	
	// Applying translation and scale
	Update();
}

void SphereCollider::SetCenter(const glm::vec3& center)
{
	m_center = center;
}

glm::vec3 SphereCollider::GetCenter() const
{
	return m_center;
}

void SphereCollider::SetRadius(const float radius)
{
	// Radius can't be zero or negative
	DBG_ASSERT(radius > 0.0f);
	m_radius = radius;
}

float SphereCollider::GetRadius() const
{
	// Radius can't be zero or negative
	DBG_ASSERT(m_radius > 0.0f);
	return m_radius;
}