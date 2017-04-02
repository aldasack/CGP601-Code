////////////////////////////////
// Name: PhysicsManager.h     //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include <chrono>

#include "PhysicsManager.h"

#include "RigidBody.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

PhysicsManager* PhysicsManager::s_instance = nullptr;

PhysicsManager::PhysicsManager()
{
	m_targetFrameTime = (1.0f / 60.0f);
}

PhysicsManager::~PhysicsManager(){}

void PhysicsManager::Shutdown()
{
	// the rigidbodys gets deleted when corresponding gameobject gets deleted
	// Although bad code, because vector items point to an unvalid object
	for (size_t i = 0; i < m_rigidBodies.size(); i++)
	{
		m_rigidBodies[i] = nullptr;
	}

	delete s_instance;
	s_instance = nullptr;
}

void PhysicsManager::Update(float deltaTime)
{
	DBG_ASSERT(deltaTime > 0.0f);

	// collision loop. This loop checks for collision
	for (size_t i = 0; i < m_rigidBodies.size(); i++)
	{
		// Updating the rigidbodys
		// TODO: unsure weather using 'm_targetFrameTime' or 'deltaTime'
		m_rigidBodies[i]->Update(m_targetFrameTime);

		for (size_t j = 0; j < m_rigidBodies.size(); j++)
		{
			/*******************************************************************************/
			// TODO: Fix this hacky code
			// gameObjects[0] is the plane, and the plane has no collider yet
			if (i == 0 || j == 0)
				continue;
			/*******************************************************************************/
			if (&m_rigidBodies[i] != &m_rigidBodies[j])
			{
				if (spheresIntersect(m_rigidBodies[i]->GetSphereCollider(), m_rigidBodies[j]->GetSphereCollider()))
				{
					if (boxIntersects(m_rigidBodies[i]->GetBoxCollider(), m_rigidBodies[j]->GetBoxCollider()))
					{
						collisionResponse(*m_rigidBodies[i], *m_rigidBodies[j]);
					}
				}
			}
		}
	}
}

void PhysicsManager::AddRigidBody(RigidBody& rigidbody)
{
	DBG_ASSERT(&rigidbody != nullptr);
	m_rigidBodies.push_back(&rigidbody);
}

bool PhysicsManager::spheresIntersect(const Collision::SphereCollider &col1, const Collision::SphereCollider &col2)
{
	float minDis_sqrd = pow(col1.GetRadius() + col2.GetRadius(), 2);
	float realDis_sqrd = pow(col1.GetPosition().x - col2.GetPosition().x, 2) + pow(col1.GetPosition().y - col2.GetPosition().y, 2) + pow(col1.GetPosition().z - col2.GetPosition().z, 2);

	if (realDis_sqrd <= minDis_sqrd)
	{
		float intersection = sqrt(minDis_sqrd) - sqrt(realDis_sqrd);
		return true;
	}
	else
	{
		return false;
	}
}

bool PhysicsManager::boxIntersects(const Collision::BoxCollider& col1, const Collision::BoxCollider& col2)
{
	std::array<glm::vec3, 8> edges1 = col1.GetEdges();
	std::array<glm::vec3, 8> edges2 = col2.GetEdges();
	// stores all the calculated dot products (absulute value)
	float c[3][3];
	float dot[3][3];
	float d[3];
	float r, r0, r1;
	//std::vector<float> interpenetration; For contact information only

	glm::vec3 difference = col2.m_center - col1.m_center;

	// 1. Axis: col1 x (0) against col2 x, y & z
	for (int i = 0; i < 3; i++)
	{
		c[0][i] = glm::dot(col1.m_axes[0], col2.m_axes[i]);
		dot[0][i] = abs(c[0][i]);
	}
	d[0] = glm::dot(difference, col1.m_axes[0]);
	r = abs(d[0]);
	r0 = col1.m_extent.x;
	r1 = col1.m_extent.x * dot[0][0] + col1.m_extent.y * dot[0][1] + col1.m_extent.z * dot[0][2];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}


	// 2. Axis: col1 y (1) against col2 x, y & z
	for (int i = 0; i < 3; i++)
	{
		c[1][i] = glm::dot(col1.m_axes[1], col2.m_axes[i]);
		dot[1][i] = abs(c[1][i]);
	}
	d[1] = glm::dot(difference, col1.m_axes[1]);
	r = abs(d[1]);
	r0 = col1.m_extent.y;
	r1 = col1.m_extent.x * dot[1][0] + col1.m_extent.y * dot[1][1] + col1.m_extent.z * dot[1][2];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 3. Axis: col1 z (2) against col2 x, y & z
	for (int i = 0; i < 3; i++)
	{
		c[2][i] = glm::dot(col1.m_axes[2], col2.m_axes[i]);
		dot[2][i] = abs(c[2][i]);
	}
	d[2] = glm::dot(difference, col1.m_axes[2]);
	r = abs(d[2]);
	r0 = col1.m_extent.z;
	r1 = col1.m_extent.x * dot[2][0] + col1.m_extent.y * dot[2][1] + col1.m_extent.z * dot[2][2];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 4. Axis: col2 x (1) against col1 x, y & z (dot products already calculated)
	r = abs(glm::dot(difference, col1.m_axes[0]));
	r0 = col1.m_extent.x * dot[0][0] + col1.m_extent.y * dot[1][0] + col1.m_extent.z * dot[2][0];
	r1 = col1.m_extent.z;

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 5. Axis: col2 y (2) against col1 x, y & z (dot products already calculated)
	r = abs(glm::dot(difference, col1.m_axes[1]));
	r0 = col1.m_extent.x * dot[0][1] + col1.m_extent.y * dot[1][1] + col1.m_extent.z * dot[2][1];
	r1 = col1.m_extent.z;

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 6. Axis: col2 z (3) against col1 x, y & z (dot products already calculated)
	r = abs(glm::dot(difference, col1.m_axes[2]));
	r0 = col1.m_extent.x * dot[0][2] + col1.m_extent.y * dot[1][2] + col1.m_extent.z * dot[2][2];
	r1 = col1.m_extent.z;

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 7. Axis: col1 x (1) against col2 x
	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = col1.m_extent.y * dot[2][0] + col1.m_extent.z * dot[1][0];
	r1 = col2.m_extent.y * dot[0][2] + col2.m_extent.z * dot[0][1];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 8. Axis: col1 x (1) against col2 y
	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = col1.m_extent.y * dot[2][1] + col1.m_extent.z * dot[1][1];
	r1 = col2.m_extent.x * dot[0][2] + col2.m_extent.z * dot[0][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 9. Axis: col1 x (1) against col2 z
	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = col1.m_extent.y * dot[2][2] + col1.m_extent.z * dot[1][2];
	r1 = col2.m_extent.x * dot[0][1] + col2.m_extent.y * dot[0][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 10. Axis: col1 y (1) against col2 x
	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = col1.m_extent.x * dot[2][0] + col1.m_extent.z * dot[0][0];
	r1 = col2.m_extent.y * dot[1][2] + col2.m_extent.z * dot[1][1];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 11. Axis: col1 y (1) against col2 y
	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = col1.m_extent.x * dot[2][1] + col1.m_extent.z * dot[0][1];
	r1 = col2.m_extent.x * dot[1][2] + col2.m_extent.z * dot[1][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 12. Axis: col1 y (1) against col2 z
	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = col1.m_extent.x * dot[2][2] + col1.m_extent.z * dot[0][2];
	r1 = col2.m_extent.x * dot[1][1] + col2.m_extent.y * dot[1][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 13. Axis: col1 z (1) against col2 x
	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = col1.m_extent.x * dot[1][0] + col1.m_extent.y * dot[0][0];
	r1 = col2.m_extent.y * dot[2][2] + col2.m_extent.z * dot[2][1];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 14. Axis: col1 z (1) against col2 y
	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = col1.m_extent.x * dot[1][1] + col1.m_extent.y * dot[0][1];
	r1 = col2.m_extent.x * dot[2][2] + col2.m_extent.z * dot[2][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 15. Axis: col1 z (1) against col2 z
	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = col1.m_extent.x * dot[1][2] + col1.m_extent.y * dot[0][2];
	r1 = col2.m_extent.x * dot[2][1] + col2.m_extent.y * dot[2][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

#pragma region Contact Information
	/**************************************************************************
	* The following code generates the contact details.
	* SAT is only used as collision culling, hence this code is not needed.

	// search smallest interpenetration, excluding zero
	float min = std::numeric_limits<float>::max();
	int index = 0;
	for (int i = 0; i < interpenetration.size(); i++)
	{
	if (interpenetration[i] < min && interpenetration[i] > 0.0f)
	{
	min = interpenetration[i];
	index = i;
	}
	}

	// define wich type of contact occured and find the contact data
	if (index < 3)
	{
	glm::vec3 axis = col1.GetAxes()[index];
	// wich face is in contact
	if (glm::dot(axis, difference) > 0.0f)
	{
	axis *= -1.0f;
	}

	// wich vertex is in contact (in col2 local coordinates)
	glm::vec3 vertex = col2.GetExtents();
	if (glm::dot(col2.GetAxes()[0], axis) < 0.0f) vertex.x = -vertex.x;
	if (glm::dot(col2.GetAxes()[1], axis) < 0.0f) vertex.y = -vertex.y;
	if (glm::dot(col2.GetAxes()[2], axis) < 0.0f) vertex.z = -vertex.z;

	// convert to wolrd coordinates

	vertex = Math::rotateVector(vertex, col2.GetRotation());
	vertex += col2.GetCenter();
	vertex *= 1.0f;
	}
	else if (index < 6)
	{
	glm::vec3 axis = col2.GetAxes()[index - 3];
	// wich face is in contact
	if (glm::dot(axis, difference * -1.0f) > 0.0f)
	{
	axis *= -1.0f;
	}

	// wich vertex is in contact (in col2 local coordinates)
	glm::vec3 vertex = col1.GetExtents();
	if (glm::dot(col1.GetAxes()[0], axis) < 0.0f) vertex.x = -vertex.x;
	if (glm::dot(col1.GetAxes()[1], axis) < 0.0f) vertex.y = -vertex.y;
	if (glm::dot(col1.GetAxes()[2], axis) < 0.0f) vertex.z = -vertex.z;

	// convert to wolrd coordinates

	vertex = Math::rotateVector(vertex, col1.GetRotation());
	vertex += col1.GetCenter();
	vertex *= 1.0f;
	}
	else if (index < 15)
	{
	// find axis of contact
	index -= 6;

	}
	*********************************************************************************/
#pragma endregion

	return true;
}

void PhysicsManager::collisionResponse(RigidBody& g1, RigidBody& g2)
{
	glm::vec3 v1 = g1.GetVelocity();
	glm::vec3 v2 = g2.GetVelocity();
	float m1 = g1.GetMass();
	float m2 = g2.GetMass();
	float im1 = 1 / m1; // inverse mass
	float im2 = 1 / m2;
	glm::vec3 pos1 = g1.GetPosition();
	glm::vec3 pos2 = g2.GetPosition();


	glm::vec3 collisionNormal = glm::normalize(pos1 - pos2);

	// checking if collision response is needed
	// if objects move towards each other
	glm::vec3 relativeVelocity = v1 - v2;
	float separatingVelocity = glm::dot(relativeVelocity, collisionNormal);
	if (separatingVelocity > 0.0f)
		return;

	// making velocity positive
	//separatingVelocity *= -1.0f;

	// involve bounciness / restitution
	float newSeparatingVelocity = -separatingVelocity * 1.0f;
	float deltaVelocity = newSeparatingVelocity - separatingVelocity;

	// calculating the impuls (p). p = m * v
	//float p = (m1 + m2) * separatingVelocity;
	float p = deltaVelocity / (im1 + im2);

	// involving the collision normal
	glm::vec3 pv = p * collisionNormal;

	// set new velocity / apply impulse to objects
	g1.SetVelocity(v1 + pv * im1);
	g2.SetVelocity(v2 + pv * -im2);
}