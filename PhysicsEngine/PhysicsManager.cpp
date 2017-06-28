////////////////////////////////
// Name: PhysicsManager.h     //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include <chrono>
#include <iostream>
#include <list>

#include "PhysicsManager.h"

#include "RigidBody.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
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

	for (size_t i = 0; i < m_rigidBodies.size(); i++)
	{
		// Updating the rigidbodys
		// TODO: unsure weather using 'm_targetFrameTime' or 'deltaTime'
		m_rigidBodies[i]->Update(m_targetFrameTime);
	}

	// collision loop. This loop checks for collision
	for (size_t i = 0; i < m_rigidBodies.size(); i++)
	{
		for (size_t j = 0; j < m_rigidBodies.size(); j++)
		{
			/*******************************************************************************/
			// TODO: Fix this hacky code
			// gameObjects[0] is the plane, and the plane has no collider yet
			//if (i == 0 || j == 0)
//				continue;
			/*******************************************************************************/
			//if (&m_rigidBodies[i] != &m_rigidBodies[j])
			if(i != j)
			{
				if (spheresIntersect(m_rigidBodies[i]->GetSphereCollider(), m_rigidBodies[j]->GetSphereCollider()))
				{
					if (boxIntersects(m_rigidBodies[i]->GetBoxCollider(), m_rigidBodies[j]->GetBoxCollider()))
					{
						Collision::ContactData contact;
						if (meshIntersects(*m_rigidBodies[i], *m_rigidBodies[j], contact))
						{
							//jiggling object to get more contact points for more precise collision response
							
							glm::quat rot = m_rigidBodies[i]->GetEulerRotation();
							// Rotation on x-axis
							m_rigidBodies[i]->Rotate(glm::vec3(1.0f * Constants::Deg2Rad, 0.0f, 0.0f));
							meshIntersects(*m_rigidBodies[i], *m_rigidBodies[j], contact);
							collisionResponse(contact);

							m_rigidBodies[i]->SetQuaterionRotation(rot);

							m_rigidBodies[i]->Rotate(glm::vec3(-1.0f * Constants::Deg2Rad, 0.0f, 0.0f));
							meshIntersects(*m_rigidBodies[i], *m_rigidBodies[j], contact);
							collisionResponse(contact);

							m_rigidBodies[i]->SetQuaterionRotation(rot);

							// Rotation on y-axis
							m_rigidBodies[i]->Rotate(glm::vec3(0.0f, 1.0f * Constants::Deg2Rad, 0.0f));
							meshIntersects(*m_rigidBodies[i], *m_rigidBodies[j], contact);
							collisionResponse(contact);
							
							m_rigidBodies[i]->SetQuaterionRotation(rot);

							m_rigidBodies[i]->Rotate(glm::vec3(0.0f, -1.0f * Constants::Deg2Rad, 0.0f));
							meshIntersects(*m_rigidBodies[i], *m_rigidBodies[j], contact);
							collisionResponse(contact);

							m_rigidBodies[i]->SetQuaterionRotation(rot);

							// Rotation on z-axis
							m_rigidBodies[i]->Rotate(glm::vec3(0.0f, 0.0f, 1.0f * Constants::Deg2Rad));
							meshIntersects(*m_rigidBodies[i], *m_rigidBodies[j], contact);
							collisionResponse(contact);

							m_rigidBodies[i]->SetQuaterionRotation(rot);

							m_rigidBodies[i]->Rotate(glm::vec3(0.0f, 0.0f, -1.0f * Constants::Deg2Rad));
							meshIntersects(*m_rigidBodies[i], *m_rigidBodies[j], contact);
							collisionResponse(contact);
							
							m_rigidBodies[i]->SetQuaterionRotation(rot);

							collisionResponse(contact);
						}
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
	float realDis_sqrd = pow(col1.GetCenter().x - col2.GetCenter().x, 2) + pow(col1.GetCenter().y - col2.GetCenter().y, 2) + pow(col1.GetCenter().z - col2.GetCenter().z, 2);

	if (realDis_sqrd <= minDis_sqrd)
		return true;
	else
		return false;
}

bool PhysicsManager::boxIntersects(const Collision::BoxCollider& col1, const Collision::BoxCollider& col2)
{
	// stores all the calculated dot products (absulute value)
	float c[3][3];
	float dot[3][3];
	float d[3];
	float r, r0, r1;

	glm::vec3 difference = col2.GetCenter() - col1.GetCenter();
	std::array<glm::vec3, 3>& col1Axes = col1.GetAxes();
	std::array<glm::vec3, 3>& col2Axes = col2.GetAxes();
	glm::vec3& col1Extent = col1.GetExtents();
	glm::vec3& col2Extent = col2.GetExtents();

	
	// 1. Axis: col1 x (0) against col2 x, y & z
	for (int i = 0; i < 3; i++)
	{
		c[0][i] = glm::dot(col1Axes[0], col2Axes[i]);
		dot[0][i] = abs(c[0][i]);
	}
	d[0] = glm::dot(difference, col1Axes[0]);
	r = abs(d[0]);
	r0 = col1Extent.x;
	r1 = col1Extent.x * dot[0][0] + col1Extent.y * dot[0][1] + col1Extent.z * dot[0][2];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}


	// 2. Axis: col1 y (1) against col2 x, y & z
	for (int i = 0; i < 3; i++)
	{
		c[1][i] = glm::dot(col1Axes[1], col2Axes[i]);
		dot[1][i] = abs(c[1][i]);
	}
	d[1] = glm::dot(difference, col1Axes[1]);
	r = abs(d[1]);
	r0 = col1Extent.y;
	r1 = col1Extent.x * dot[1][0] + col1Extent.y * dot[1][1] + col1Extent.z * dot[1][2];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 3. Axis: col1 z (2) against col2 x, y & z
	for (int i = 0; i < 3; i++)
	{
		c[2][i] = glm::dot(col1Axes[2], col2Axes[i]);
		dot[2][i] = abs(c[2][i]);
	}
	d[2] = glm::dot(difference, col1Axes[2]);
	r = abs(d[2]);
	r0 = col1Extent.z;
	r1 = col1Extent.x * dot[2][0] + col1Extent.y * dot[2][1] + col1Extent.z * dot[2][2];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 4. Axis: col2 x (1) against col1 x, y & z (dot products already calculated)
	r = abs(glm::dot(difference, col1Axes[0]));
	r0 = col1Extent.x * dot[0][0] + col1Extent.y * dot[1][0] + col1Extent.z * dot[2][0];
	r1 = col1Extent.z;

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 5. Axis: col2 y (2) against col1 x, y & z (dot products already calculated)
	r = abs(glm::dot(difference, col1Axes[1]));
	r0 = col1Extent.x * dot[0][1] + col1Extent.y * dot[1][1] + col1Extent.z * dot[2][1];
	r1 = col1Extent.z;

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 6. Axis: col2 z (3) against col1 x, y & z (dot products already calculated)
	r = abs(glm::dot(difference, col1Axes[2]));
	r0 = col1Extent.x * dot[0][2] + col1Extent.y * dot[1][2] + col1Extent.z * dot[2][2];
	r1 = col1Extent.z;

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 7. Axis: col1 x (1) against col2 x
	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = col1Extent.y * dot[2][0] + col1Extent.z * dot[1][0];
	r1 = col2Extent.y * dot[0][2] + col2Extent.z * dot[0][1];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 8. Axis: col1 x (1) against col2 y
	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = col1Extent.y * dot[2][1] + col1Extent.z * dot[1][1];
	r1 = col2Extent.x * dot[0][2] + col2Extent.z * dot[0][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 9. Axis: col1 x (1) against col2 z
	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = col1Extent.y * dot[2][2] + col1Extent.z * dot[1][2];
	r1 = col2Extent.x * dot[0][1] + col2Extent.y * dot[0][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 10. Axis: col1 y (1) against col2 x
	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = col1Extent.x * dot[2][0] + col1Extent.z * dot[0][0];
	r1 = col2Extent.y * dot[1][2] + col2Extent.z * dot[1][1];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 11. Axis: col1 y (1) against col2 y
	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = col1Extent.x * dot[2][1] + col1Extent.z * dot[0][1];
	r1 = col2Extent.x * dot[1][2] + col2Extent.z * dot[1][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 12. Axis: col1 y (1) against col2 z
	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = col1Extent.x * dot[2][2] + col1Extent.z * dot[0][2];
	r1 = col2Extent.x * dot[1][1] + col2Extent.y * dot[1][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 13. Axis: col1 z (1) against col2 x
	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = col1Extent.x * dot[1][0] + col1Extent.y * dot[0][0];
	r1 = col2Extent.y * dot[2][2] + col2Extent.z * dot[2][1];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 14. Axis: col1 z (1) against col2 y
	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = col1Extent.x * dot[1][1] + col1Extent.y * dot[0][1];
	r1 = col2Extent.x * dot[2][2] + col2Extent.z * dot[2][0];

	//interpenetration.push_back(r0 + r1 - r);
	if (r > r0 + r1)
	{
		return false;
	}

	// 15. Axis: col1 z (1) against col2 z
	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = col1Extent.x * dot[1][2] + col1Extent.y * dot[0][2];
	r1 = col2Extent.x * dot[2][1] + col2Extent.y * dot[2][0];

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

#pragma region GJK
//bool PhysicsManager::meshIntersects(const Collision::MeshCollider& col1, const Collision::MeshCollider& col2, Collision::ContactData& contact)
bool PhysicsManager::meshIntersects(RigidBody& rb1, RigidBody& rb2, Collision::ContactData& contact)
{
	std::vector<glm::vec3>& A = rb1.GetMeshCollider().GetVertices();
	std::vector<glm::vec3>& B = rb2.GetMeshCollider().GetVertices();

	DBG_ASSERT(A.size() != 0);
	DBG_ASSERT(B.size() != 0);

	//std::vector<glm::vec3> simplex;
	std::vector<SupportPoint> simplex;
	//glm::vec3 a = A[0] - B[0];
	SupportPoint a = { A[0] - B[0], A[0], B[0] };
	simplex.push_back(a);
	glm::vec3 direction = -a.v; // from point a to the origin

	int i = 0;
	while (true)
	{
		//a.v = support(direction, A) - support(-direction, B);
		a.A = support(direction, A);
		a.B = support(-direction, B);
		a.v = a.A - a.B;

		if (glm::dot(a.v, direction) < 0)
		{
			// No intersection
			// furthest point reached, can not go further than the origin
			return false;
		}
		else
		{
			//if (i > 32) return false;
			simplex.push_back(a);
			DBG_ASSERT(simplex.size() <= 4);
			if (doSimplex(simplex, direction))
			{
				contact.m_bodies[0] = &rb1;
				contact.m_bodies[1] = &rb2;
				generateContactData(simplex, A, B, contact);
				return true;
			}
			else
			{
				i++;
				if (direction.x == 0.0f && direction.y == 0.0f && direction.z == 0.0f)
					return false;
				if (i > 320)
					return false;
			}

		}
	}
}

glm::vec3 PhysicsManager::support(const glm::vec3& direction, const std::vector<glm::vec3>& points)
{
	glm::vec3 a;
	float max = glm::dot(direction, points[0]);//std::numeric_limits<float>::max() * -1.0f;
	float dot = 0.0f;
	int index = 0;
	for (size_t i = 1; i < points.size(); i++)
	{
		dot = glm::dot(direction, points[i]);
		if (dot > max)
		{
			max = dot;
			index = i;
		}
	}

	return points[index];
}

bool PhysicsManager::doSimplex(std::vector<SupportPoint>& simplex, glm::vec3& direction)
{
	// a x b x a = cross(cross(a,b),a)

	size_t size = simplex.size();
	// simplex can not be empty
	DBG_ASSERT(size != 0);

	// get the last added point
	//glm::vec3& A = simplex[size - 1];
	SupportPoint& A = simplex[size - 1];

	switch (size)
	{
		// Simpex is a point (A)
		// case 1 will not appear, because doSimplex will always be called after atleast 2 points had been added to the simplex list
	case 1:
	{
		direction = -A.v;
		return false;
	} // Case 1

	  // Simplex is a line (AB)
	case 2:
	{
		SupportPoint& B = simplex[0];

		if (A.v == B.v)
		{
			simplex.pop_back();
			direction *= -1.0f;
			return false;
		}
		glm::vec3& AB = B.v - A.v;

		if (glm::dot(AB, -A.v) > 0)
		{
			// is between A and B
			direction = glm::cross(glm::cross(AB, -A.v), AB);
		}
		else
		{
			// is behind A
			direction = -A.v;
		}
		return false;
	} // Case 2

	  // Simplex is a triangle (ABC)
	case 3:
	{
		SupportPoint& B = simplex[1];
		SupportPoint& C = simplex[0];
		glm::vec3 AB = B.v - A.v;
		glm::vec3 AC = C.v - A.v;

		DBG_ASSERT(A.v != B.v);
		DBG_ASSERT(A.v != C.v);
		DBG_ASSERT(B.v != C.v);

		// cross-product / normal of the triangle ABC
		glm::vec3 n = glm::cross(AB, AC);

		if (glm::dot(glm::cross(n, AC), -A.v) > 0)
		{
			if (glm::dot(AC, -A.v) > 0)
			{
				direction = glm::cross(glm::cross(AC, -A.v), AC);
				return false;
			}

			else
			{
#pragma region star
				if (glm::dot(AB, -A.v) > 0)
				{
					direction = glm::cross(glm::cross(AB, -A.v), AB);
				}
				else
				{
					direction = -A.v;
				}
#pragma endregion
			}
		}
		else
		{
			if (glm::dot(glm::cross(AB, n), -A.v) > 0)
			{
#pragma region star
				if (glm::dot(AB, -A.v) > 0)
				{
					direction = glm::cross(glm::cross(AB, -A.v), AB);
					return false;
				}

				else
				{
					direction = -A.v;
				}
#pragma endregion
			}
			else
			{
				// above triangle
				if (glm::dot(n, -A.v) > 0)
				{
					direction = n; // vorzeichen geändert
					return false;
				}
				// under triangle
				else
				{
					direction = -n; // vorzeichen geändert
					return false;
				}
			}
		}
		__debugbreak();
	} // Case 3

	  // Simplex is a tetrahedron (ABCD)
	case 4:
	{
		// Points of simples
		//glm::vec3& A = simplex[3]; already know it, dunno why no compile error
		/*glm::vec3& B = simplex[2];
		glm::vec3& C = simplex[1];
		glm::vec3& D = simplex[0];*/

		SupportPoint B = simplex[2];
		SupportPoint C = simplex[1];
		SupportPoint D = simplex[0];

		DBG_ASSERT(A.v != B.v);
		DBG_ASSERT(A.v != C.v);
		DBG_ASSERT(A.v != D.v);
		DBG_ASSERT(B.v != C.v);
		DBG_ASSERT(B.v != D.v);
		DBG_ASSERT(C.v != D.v);

		// Line segments
		glm::vec3 AB = B.v - A.v;
		glm::vec3 AC = C.v - A.v;
		glm::vec3 AD = D.v - A.v;

		// Normals of triangles
		glm::vec3 nABC = glm::cross(AB, AC);
		glm::vec3 nABD = glm::cross(AD, AB);
		glm::vec3 nACD = glm::cross(AC, AD);
		// probably unnecessary
		glm::vec3 nBCD = glm::cross(C.v - B.v, D.v - B.v);


		// Cheking if all points lay on a single plane
		nBCD = glm::normalize(nBCD);
		nABC = glm::normalize(nABC);
		float length = glm::length(nBCD - nABC);
		if (abs(length) < Constants::Precision)
		{
			direction *= -1.0f;
			direction.y += 2.0f;
			simplex.pop_back();
			return false;
		}
		length = glm::length(nBCD + nABC);
		if (abs(length) < Constants::Precision)
		{
			direction *= -1.0f;
			direction.y += 2.0f;
			simplex.pop_back();
			return false;
		}


		// check if origin is inside simplex
		// ABC
		float dot = glm::dot(nABC, -A.v);
		dot = glm::dot(nABD, -A.v);
		dot = glm::dot(nACD, -A.v);
		if (glm::dot(nABC, -A.v) < 0)
		{
			// ABD
			dot = glm::dot(nABD, -A.v);
			if (glm::dot(nABD, -A.v) < 0)
			{
				// ACD
				dot = glm::dot(nACD, -A.v);
				if (glm::dot(nACD, -A.v) < 0)
				{
					return true;
				}
			}
		}

		// ABC
		dot = glm::dot(nABC, -A.v);
		if (glm::dot(nABC, -A.v) > 0)
		{
			simplex[0] = C;
			simplex[1] = B;
			simplex[2] = A;
			simplex.pop_back();
			direction = nABC;

			return false;
		}

		// ABD
		dot = glm::dot(nABD, -A.v);
		if (glm::dot(nABD, -A.v) > 0)
		{
			simplex[0] = B;
			simplex[1] = D; // because counter clockwise
			simplex[2] = A;
			simplex.pop_back();
			direction = nABD;
			return false;
		}

		// ACD
		dot = glm::dot(nACD, -A.v);
		if (glm::dot(nACD, -A.v) > 0)
		{
			simplex[0] = D;
			simplex[1] = C;
			simplex[2] = A;
			simplex.pop_back();
			direction = nACD;
			return false;
		}

		// BCD
		dot = glm::dot(nBCD, -A.v);
		if (glm::dot(nBCD, -A.v) > 0)
		{
			__debugbreak();
		}

		__debugbreak();
		direction = { 0.0f, 0.0f, 0.0f };
		return false;
		break;
	} // Case 4
	} // Switch

	  //DBG_ASSERT(direction != tmpDirection);
	return false;
}

// Maintaining list of all open edges.
void addEdge(const SupportPoint& A, const SupportPoint& B, std::list<Edge>& edges)
{
	for (auto it = edges.begin(); it != edges.end(); it++)
	{
		if (it->A.v == B.v && it->B.v == A.v)
		{
			edges.erase(it);
			return;
		}
	}
	edges.emplace_back(A, B);
}

void barycentric(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, float* u, float* v, float* w)
{
	glm::vec3 v0 = b - a;
	glm::vec3 v1 = c - a;
	glm::vec3 v2 = p - a;
	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);
	float d21 = glm::dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	*v = (d11 * d20 - d01 * d21) / denom;
	*w = (d00 * d21 - d01 * d20) / denom;
	*u = 1.0f - *v - *w;
}

void PhysicsManager::generateContactData(std::vector<SupportPoint>& simplex, const std::vector<glm::vec3>& A, const std::vector<glm::vec3>& B, Collision::ContactData& contact)
{
	DBG_ASSERT(simplex.size() == 4);

	// List of all triangles of the polytope
	std::list<Triangle> triangles;
	// List of all edges froming a hole when a triangle is removed
	std::list<Edge> edges;

	triangles.emplace_back(simplex[3], simplex[2], simplex[1]);
	triangles.emplace_back(simplex[3], simplex[1], simplex[0]);
	triangles.emplace_back(simplex[3], simplex[0], simplex[2]);
	triangles.emplace_back(simplex[2], simplex[0], simplex[1]);

	float minDot;
	Triangle closest = Triangle(SupportPoint(), SupportPoint(), SupportPoint());
#pragma region Find closest trianlge
	while (true)
	{
		minDot = std::numeric_limits<float>().max();
		closest = Triangle(SupportPoint(), SupportPoint(), SupportPoint());
		// Searching closest triangle to the origin
		// Project vector a (a to origin) onto the normal to get the lenght
		for (auto it = triangles.begin(); it != triangles.end(); it++)
		{
			float dot = abs(glm::dot(it->normal, it->A.v));
			DBG_ASSERT(dot != 0.0f);
			if (dot < minDot)
			{
				minDot = dot;
				closest = *it;
			}
		}

		// Searching the furthes point along triangles negative nomral (=> point lays in bounds on minkowski sum, negative bc positive points towards center of shape)
		SupportPoint a;
		a.A = support(-closest.normal, A);
		a.B = support(closest.normal, B);
		a.v = a.A - a.B;

		for (auto it = triangles.begin(); it != triangles.end(); it++)
		{
			if (it->A.v == a.v || it->B.v == a.v || it->C.v == a.v) 
			{
				goto generate_contact_data; // Closest triangle had been found
			}
				
		}

		// Searching for triangles to be removed
		for (auto it = triangles.begin(); it != triangles.end();)
		{
			if (glm::dot(it->normal, a.v - it->A.v) > 0)
			{
				addEdge(it->A, it->B, edges);
				addEdge(it->B, it->C, edges);
				addEdge(it->C, it->A, edges);
				it = triangles.erase(it);
			}else
				it++;
		}

		// Create new triangle(s) with new point
		for (auto it = edges.begin(); it != edges.end(); it++)
		{
			triangles.emplace_back(a, it->A, it->B);
		}
	}
#pragma endregion

#pragma region Generate contact data
	generate_contact_data :
		float bary_u, bary_v, bary_w;
		barycentric(closest.normal, closest.A.v, closest.B.v, closest.C.v, &bary_u, &bary_v, &bary_w);

		// collision point onobject in world space
		contact.m_contactPoint = glm::vec3((bary_u * closest.A.A) + (bary_v * closest.B.A) + bary_w * closest.C.A);

		// collision normal
		contact.m_contactNormal = -closest.normal;

		// penetration depth
		contact.m_penetration = minDot;
#pragma endregion
}
#pragma endregion

void PhysicsManager::collisionResponse(const Collision:: ContactData& contact)
{
	RigidBody& rb1 = *contact.m_bodies[0];
	RigidBody& rb2 = *contact.m_bodies[1];
	
	// Both objects are static and do not react to any forces or impusles.
	// No need for further calculation, because they will have no affect on the objects.
	// Just resolve the interpenetration.
	if (rb1.IsStatic() && rb2.IsStatic())
		goto interpenetration;

	glm::vec3& v1 = rb1.GetVelocity();
	glm::vec3& v2 = rb2.GetVelocity();
	glm::vec3& w1 = rb1.GetAngularVelocity();
	glm::vec3& w2 = rb1.GetAngularVelocity();
	
	float im1 = rb1.GetInverseMass(); // inverse mass
	float im2 = rb2.GetInverseMass(); // inverse mass
	glm::vec3& pos1 = rb1.GetPosition();
	glm::vec3& pos2 = rb2.GetPosition();
	glm::mat3& I1 = rb1.GetInertiaTensor();
	glm::mat3& I2 = rb2.GetInertiaTensor();

	/*
	//glm::vec3 collisionNormal = glm::normalize(pos1 - pos2);

	// checking if collision response is needed
	// if objects move towards each other
	//glm::vec3 relativeVelocity = v1 - v2;
	//float separatingVelocity = glm::dot(relativeVelocity, collisionNormal);
	//if (separatingVelocity > 0.0f)
	//	return;

	//// involve bounciness / restitution
	//float newSeparatingVelocity = -separatingVelocity * 1.0f;
	//float deltaVelocity = newSeparatingVelocity - separatingVelocity;
	*/
	
#pragma region Linear impuls
	// Checking if collision response is needed,
	// If objects move towards each other.
	glm::vec3 relativeVelcotiy = v2 - v1;
	float closingVelocity = glm::dot(relativeVelcotiy, contact.m_contactNormal);
	if (closingVelocity <= 0.0f)
		return;
	
	// Applying factor of bounciness. In this case 100% => perfectly elastic collision
	float restitution = 0.0f;
	float deltaVelocity = closingVelocity + closingVelocity * restitution;

	// Calculating the impuls magnitude (p). p = m * v
	float p = deltaVelocity / (im1 + im2);

	// Calculating the impuls direction with the collision normal, calculating the impulse vector
	glm::vec3 j = p * contact.m_contactNormal;

	// Set new velocity / apply impulse to objects
	rb1.SetVelocity(v1 + j * im1);
	rb2.SetVelocity(v2 + j * -im2);

	// Push objects apart in relation to thier velocity
	float speed1 = glm::length(v1);
	float speed2 = glm::length(v2);
	glm::vec3 direction1 = glm::normalize(v1);
	glm::vec3 direction2 = glm::normalize(v2);
#pragma endregion

#pragma region Angular Impuls

	// calculate new rotation speed of the contact point and translating it into an angular velocity (rad/s)
	// Calculate distance from contact point and centre of mass.
	glm::vec3 r1 = contact.m_contactPoint - rb1.GetCentreOfMass();
	//glm::vec3 av1 = j * I1 * glm::cross(r1, contact.m_contactNormal);
	glm::vec3 av1 = glm::inverse(I1) * glm::cross(r1, j);
	rb1.SetAngularVelocity(w1 + av1);

	glm::vec3 r2 = contact.m_contactPoint - rb2.GetCentreOfMass();
	//glm::vec3 av2 = j * I2 * glm::cross(r2, contact.m_contactNormal);
	glm::vec3 av2 = glm::inverse(I2) * glm::cross(r2, -j);
	rb2.SetAngularVelocity(w2 + av2);

#pragma endregion

	//glm::vec3 relativeVelcotiy = v2 - v1;
	//float closingVelocity = glm::dot(relativeVelcotiy, contact.m_contactNormal);
	//if (closingVelocity <= 0.0f)
	//	return;

	//// Applying factor of bounciness. In this case 100% => perfectly elastic collision
	//float restitution = 1.0f;
	//float deltaVelocity = closingVelocity + closingVelocity * restitution;

	//glm::vec3 r1 = contact.m_contactPoint - rb1.GetCentreOfMass();
	//glm::vec3 r2 = contact.m_contactPoint - rb2.GetCentreOfMass();

	//float j = glm::dot(relativeVelcotiy, contact.m_contactNormal) / (im1 + im2 + glm::dot(glm::cross((1.0f / I1) * glm::cross(r1, contact.m_contactNormal), r1) + glm::cross((1.0f / I2) * glm::cross(r2, contact.m_contactNormal), r2), contact.m_contactNormal));
	//
	//rb1.SetVelocity(v1 + j * im1);
	//rb1.SetVelocity(v2 + j * im2);


#pragma region Interpenetration
	interpenetration:

	if (contact.m_penetration > 0.0f)
	{
		glm::vec3 s = contact.m_contactNormal * (contact.m_penetration / (im1 + im2));

		rb1.SetPosition(pos1 + s * im1);
		rb2.SetPosition(pos2 + s * -im2);
	}
	
	/*float s = contact.m_penetration / speed1 + speed2;
	if (isinf(s))
		return;
	if(!isnan(direction1.x))
		rb1.SetPosition(pos1 + speed1 * s * direction1);
	if (!isnan(direction2.x))
		rb2.SetPosition(pos2 + speed2 * s * direction2);*/
#pragma endregion
}