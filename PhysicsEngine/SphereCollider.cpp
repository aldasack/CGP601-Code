////////////////////////////////
// Name: SphereCollider.cpp	  //
// Author: Alexander Kocourek //
// Date: 22/03/2017			  //
////////////////////////////////

#include "SphereCollider.h"

using namespace Collision;

SphereCollider::SphereCollider(RigidBody& rigidBody) : Collider(rigidBody)
{
	m_colliderType = ColliderType::Sphere;
	m_radius = 0.5;
	m_position = m_pRigidBody->GetPosition();
}

SphereCollider::~SphereCollider(){}

void SphereCollider::Update()
{
	m_position = m_pRigidBody->GetPosition();
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