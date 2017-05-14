////////////////////////////////
// Name: RigidBody.h		  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////


// TODO:
// mehtoden namen convention, klein oder groﬂ
#pragma once

#ifndef _DEFS_H
#define _DEFS_H


#include <cmath>
#include <glut.h>
#include <string>
#include <exception>
#include <array>
#include <vector>
#include <memory>

#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"

// all angles in glm are represented in radians
#define GLM_FORCE_RADIANS

// Basic assert function to do some checking - if the check is violate
// i.e. false, then this line will cause the debugger to stop on the line
#define DBG_ASSERT(condition) { if(!(condition)){ __debugbreak(); }; }


namespace Constants
{
	// Value for gravity = 9.81 m/s^2.
	const float G = 9.81f;
	// Precalculated value for pi.
	const float Pi = 4.0f * atan(1.0f);
	// Factor to convert degree to radians.
	const float Deg2Rad = Pi / 180.0f;
	// Factor to convert radians to degree.
	const float Rad2Deg = 180.0f / Pi;
	// Precision value for precision tests.
	const float Precision = 0.0001f;
}

namespace Math
{
	// Rotates vector by normalized quaternion
	static glm::vec3 RotateVector(const glm::vec3& vector, const glm::quat& quat)
	{
		// check if quaternion is normalized
		float length = glm::length(quat);
		DBG_ASSERT(abs(length - 1.0f) < Constants::Precision);

		glm::vec3 result;
		
		// x = cross product
		// p' = q x p x q^-1
		// p' = (2w^2 - 1)p + 2(v dot p)v + 2w(v x p)

		float vMult = 2.0f * (quat.x * vector.x + quat.y * vector.y + quat.z * vector.z);
		float crossMult = 2.0f * quat.w;
		float pMult = crossMult * quat.w - 1.0f;
		
		result.x = pMult * vector.x + vMult * quat.x + crossMult * (quat.y * vector.z - quat.z * vector.y);
		result.y = pMult * vector.y + vMult * quat.y + crossMult * (quat.z * vector.x - quat.x * vector.z);
		result.z = pMult * vector.z + vMult * quat.z + crossMult * (quat.x * vector.y - quat.y * vector.x);

		return result;
	}

	// Predefined vector values.
	namespace vec3
	{
		const glm::vec3 Zero{ 0.0f, 0.0f, 0.0f };
		const glm::vec3 Up{ 0.0f, 1.0f, 0.0f };
		const glm::vec3 Down{ 0.0f, -1.0f, 0.0f };
		const glm::vec3 Left{ -1.0f, 0.0f, 0.0f };
		const glm::vec3 Right{ 1.0f, 0.0f, 0.0f };
		const glm::vec3 Forward{ 0.0f, 0.0f, -1.0f };
		const glm::vec3 Backward{ 0.0f, 0.0f, -1.0f };
	}
}

//TODO: methods calculating inertia tensor for different shapes

class RigidBody;
class GameManager;
class PhysicsManager;
class GameObject;
class Sphere;
class Box;
class Plane;

namespace Collision
{
	class Collider;
	class SphereCollider;
	class BoxCollider;
	class MeshCollider;

	// Represents a contact between two objects.
	struct ContactData
	{
		RigidBody* m_bodies[2];
		glm::vec3 m_contactNormal;
		glm::vec3 m_contactPoint;
		float m_penetration;
	};
}

// Vectors representing colors.
namespace Colors
{
	const glm::vec3 White = { 1.0f, 1.0f, 1.0f };
	const glm::vec3 Black = { 0.0f, 0.0f, 0.0f };
	const glm::vec3 Red = { 1.0f, 0.0f, 0.0f };
	const glm::vec3 Blue = { 0.0f, 0.0f, 1.0f };
}

#endif // !_DEFS_H