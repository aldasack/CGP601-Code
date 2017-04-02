////////////////////////////////
// Name: Contact.cpp		  //
// Author: Alexander Kocourek //
// Date: 01/04/2017			  //
////////////////////////////////

#include "ContactData.h"

ContactData::ContactData(RigidBody& body1, RigidBody& body2, const glm::vec3& contactNormal, const glm::vec3& contactPoint, float penetration)
{
	float lenght = glm::length(contactNormal);
	DBG_ASSERT(abs(1 - lenght) >= 0.001f);
	DBG_ASSERT(penetration <= 0.0f);

	m_bodies[0] = &body1;
	m_bodies[1] = &body2;

	m_contactNormal = contactNormal;
	m_contactPoint = contactPoint;
	m_penetration = penetration;
}

ContactData::~ContactData(){}

void ContactData::Shutdown()
{
	m_bodies[0] = nullptr;
	m_bodies[1] = nullptr;
}

RigidBody& ContactData::GetRigidBody(const unsigned int number) const
{
	// checking array bounds
	DBG_ASSERT(number < 2);
	DBG_ASSERT(m_bodies[number]);
	return *m_bodies[number];
}

glm::vec3 ContactData::GetContactNormal() const
{
	float lenght = glm::length(m_contactNormal);
	DBG_ASSERT(abs(1 - lenght) >= 0.001f);
	return m_contactNormal;
}

glm::vec3 ContactData::GetContactPoint() const
{
	return m_contactPoint;
}

float ContactData::GetPenetration() const
{
	return m_penetration;
}