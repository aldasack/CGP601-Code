#pragma once

////////////////////////////////
// Name: Contact.h			  //
// Author: Alexander Kocourek //
// Date: 01/04/2017			  //
////////////////////////////////

#ifndef _CONTACT_H
#define _CONTACT_H

#include "Defs.h"

/**
*	This class stores all the contact data that is gained from the collision detection.
*	This data is used by the contact resolver.
*/

class ContactData
{
public:
	ContactData(RigidBody& body1, RigidBody& body2, const glm::vec3& contacNormal, const glm::vec3& contactPoint, float penetration);
	~ContactData();
	void Shutdown();

	RigidBody& GetRigidBody(const unsigned int number) const;
	glm::vec3 GetContactNormal() const;
	glm::vec3 GetContactPoint() const;
	float GetPenetration() const;

private:
	RigidBody* m_bodies[2];
	glm::vec3 m_contactNormal;
	glm::vec3 m_contactPoint;
	float m_penetration;
};

#endif // !_CONTACT_H