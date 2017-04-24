////////////////////////////////
// Name: PhysicsManager.h     //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include <chrono>
#include <iostream>

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
			//if (&m_rigidBodies[i] != &m_rigidBodies[j])
			if(i != j)
			{
				//float length = m_rigidBodies[i]->GetPosition().z - m_rigidBodies[j]->GetPosition().z;  //glm::length(m_rigidBodies[i]->GetPosition() - m_rigidBodies[j]->GetPosition());
				//if (abs(length) <= (0.5f + sqrtf(2) / 2))
				//{
				//	length += 0;
				//}
				if (spheresIntersect(m_rigidBodies[i]->GetSphereCollider(), m_rigidBodies[j]->GetSphereCollider()))
				{
					if (boxIntersects(m_rigidBodies[i]->GetBoxCollider(), m_rigidBodies[j]->GetBoxCollider()))
					{
						if (meshIntersects(m_rigidBodies[i]->GetMeshCollider(), m_rigidBodies[j]->GetMeshCollider(), m_rigidBodies[i]->GetQuaternionRotation(), m_rigidBodies[j]->GetQuaternionRotation()))
						{
							collisionResponse(*m_rigidBodies[i], *m_rigidBodies[j]);
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
bool PhysicsManager::meshIntersects(const Collision::MeshCollider& col1, const Collision::MeshCollider& col2, const glm::quat& rot1, const glm::quat& rot2)
{
	std::vector<glm::vec3>& A = col1.GetVertices();
	std::vector<glm::vec3>& B = col2.GetVertices();

	DBG_ASSERT(A.size() != 0);
	DBG_ASSERT(B.size() != 0);

	std::vector<glm::vec3> simplex;
	//std::array<glm::vec3, 4> simplex;
	glm::vec3 a = A[0] - B[0];
	simplex.push_back(a);
	glm::vec3 direction = -a; // from point a to the origin

	int i = 0;
	while (1)
	{
		//a = support(Math::rotateVector(direction, rot1), A) - support(Math::rotateVector(-direction, rot2), B);
		a = support(direction, A) - support(-direction, B);
		if (glm::dot(a, direction) < 0)
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

bool PhysicsManager::doSimplex(std::vector<glm::vec3>& simplex, glm::vec3& direction)
{
	// a x b x a = cross(cross(a,b),a)

	size_t size = simplex.size();
	// simplex can not be empty
	DBG_ASSERT(size != 0);

	// get the last added point
	glm::vec3& A = simplex[size - 1];

	switch (size)
	{
		// Simpex is a point (A)
		// case 1 wont appear, because doSimplex will always be called after atleast 2 points had been added to the simplex list
	case 1:
	{
		direction = -A;
		return false;
	} // Case 1

	  // Simplex is a line (AB)
	case 2:
	{
		glm::vec3& B = simplex[0];
		//DBG_ASSERT(A != B);
		if (A == B)
		{
			simplex.pop_back();
			direction *= -1.0f;
			return false;
		}
		glm::vec3& AB = B - A;

		if (glm::dot(AB, -A) > 0)
		{
			// is between A and B
			direction = glm::cross(glm::cross(AB, -A), AB);
		}
		else
		{
			// is behind A
			direction = -A;
		}
		return false;
	} // Case 2

	  // Simplex is a triangle (ABC)
	case 3:
	{
	case3:
		glm::vec3& B = simplex[1];
		glm::vec3& C = simplex[0];
		glm::vec3 AB = B - A;
		glm::vec3 AC = C - A;

		DBG_ASSERT(A != B);
		DBG_ASSERT(A != C);
		DBG_ASSERT(B != C);

		// cross-product / normal of the triangle ABC
		glm::vec3 n = glm::cross(AB, AC);

		if (glm::dot(glm::cross(n, AC), -A) > 0)
		{
			if (glm::dot(AC, -A) > 0)
			{
				direction = glm::cross(glm::cross(AC, -A), AC);
				return false;
			}

			else
			{
#pragma region star
				if (glm::dot(B - A, -A) > 0)
				{
					direction = glm::cross(glm::cross(AB, -A), AB);
				}
				else
				{
					direction = -A;
				}
#pragma endregion
			}
		}
		else
		{
			if (glm::dot(glm::cross(AB, n), -A) > 0)
			{
#pragma region star
				if (glm::dot(AB, -A) > 0)
				{
					direction = glm::cross(glm::cross(AB, -A), AB);
					return false;
				}

				else
				{
					direction = -A;
				}
#pragma endregion
			}
			else
			{
				// above triangle
				if (glm::dot(n, -A) > 0)
				{
					direction = -n; // vorzeichen geändert
					return false;
				}
				// under triangle
				else
				{
					direction = n; // vorzeichen geändert
					return false;
				}
			}
		}
	} // Case 3

	  //// Simplex is a tetrahedron (ABCD)
	case 4:
	{
		// Points of simples
		//glm::vec3& A = simplex[3]; already know it, dunno why no compile error
		glm::vec3& B = simplex[2];
		glm::vec3& C = simplex[1];
		glm::vec3& D = simplex[0];

		DBG_ASSERT(A != B);
		DBG_ASSERT(A != C);
		DBG_ASSERT(A != D);
		DBG_ASSERT(B != C);
		DBG_ASSERT(B != D);
		DBG_ASSERT(C != D);

		// Line segments
		glm::vec3 AB = B - A;
		glm::vec3 AC = C - A;
		glm::vec3 AD = D - A;

		// Normals of triangles
		glm::vec3 nABC = glm::cross(AB, AC);
		glm::vec3 nABD = glm::cross(AD, AB);
		glm::vec3 nACD = glm::cross(AC, AD);
		// probably unnecessary
		glm::vec3 nBCD = glm::cross(C - B, D - B);


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
		float dot = glm::dot(nABC, -A);
		dot = glm::dot(nABD, -A);
		dot = glm::dot(nACD, -A);
		if (glm::dot(nABC, -A) < 0)
		{
			// ABD
			dot = glm::dot(nABD, -A);
			if (glm::dot(nABD, -A) < 0)
			{
				// ACD
				dot = glm::dot(nACD, -A);
				if (glm::dot(nACD, -A) < 0)
				{
					return true;
					////BCD
					//if (glm::dot(nBCD, -B) > 0)
					//{
					//	return true;
					//}
				}
			}
		}

		// ABC
		dot = glm::dot(nABC, -A);
		if (glm::dot(nABC, -A) > 0)
		{
			simplex[0] = C;
			simplex[1] = B;
			simplex[2] = A;
			simplex.pop_back();
			direction = nABC;

			return false;
		}

		// ABD
		dot = glm::dot(nABD, -A);
		if (glm::dot(nABD, -A) > 0)
		{
			simplex[0] = B;
			simplex[1] = D; // because counter clockwise
			simplex[2] = A;
			simplex.pop_back();
			direction = nABD;
			return false;
		}

		// ACD
		dot = glm::dot(nACD, -A);
		if (glm::dot(nACD, -A) > 0)
		{
			simplex[0] = D;
			simplex[1] = C;
			simplex[2] = A;
			simplex.pop_back();
			direction = nACD;
			return false;
		}

		// BCD
		dot = glm::dot(nBCD, -A);
		if (glm::dot(nBCD, -A) > 0)
		{
			__debugbreak();
		}

		direction = { 0.0f, 0.0f, 0.0f };
		return false;
		break;
	} // Case 4
	} // Switch

	  //DBG_ASSERT(direction != tmpDirection);
	return false;
}

bool PhysicsManager::minkowskiSum(const Collision::MeshCollider& col1, const Collision::MeshCollider& col2)
{
	std::vector<glm::vec3>& v1 = col1.GetVertices();
	std::vector<glm::vec3>& v2 = col2.GetVertices();
	std::vector<glm::vec3> v3;

	// subtracting each point (minkowskik difference)
	for (size_t i = 0; i < v1.size(); i++)
	{
		for (size_t j = 0; j < v2.size(); j++)
		{
			bool b = false;
			glm::vec3 tmp = v1[i] - v2[j];
			for (size_t k = 0; k < v3.size(); k++)
			{
				if (tmp == v3[k])
				{
					b = true;
					break;
				}

			}
			if (!b)
				v3.push_back(v1[i] - v2[j]);
		}
	}

	// checking if new shape contains the origin (0,0,0)


	return false;
}

#pragma endregion


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