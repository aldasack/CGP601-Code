////////////////////////////////
// Name: Collider.cpp		  //
// Author: Alexander Kocourek //
// Date: 22/03/2017			  //
////////////////////////////////

#include "Collider.h"

using namespace Collision;

Collider::Collider(const RigidBody& rigidBody)
{
	m_pRigidBody = &rigidBody;
}

Collider::~Collider(){}

void Collider::Shutdown()
{
	// should be deleted by the owning GameObject
	// setting to nullptr is not necessary
	m_pRigidBody = nullptr;
}

ColliderType Collider::GetColliderType() const
{
	return m_colliderType;
}