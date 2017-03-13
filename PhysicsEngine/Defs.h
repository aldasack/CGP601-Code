#pragma once

#ifndef _DEFS_H
#define _DEFS_H

////////////////////////////////
// Name: RigidBody.h		  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include <cmath>
#include <glut.h>
#include <string>

#include "glm\glm.hpp"

namespace Constants
{
	const float G = 9.81f;
	const float Pi = 4.0f * atan(1.0f);
}

//namespace Time
//{
//	// Time since application start, currently not working
//	float TotalTime = 0.0f;
//}

namespace Math
{
	struct Vector3
	{
		float x, y, z;
	};

	struct Vector2
	{
		float x, y;
	};
}

namespace Collider
{
	struct SphereCollider
	{
		glm::vec3 position;
		float radius;
	};
}

namespace Colors
{
	const glm::vec3 White = { 1.0f, 1.0f, 1.0f };
	const glm::vec3 Black = { 0.0f, 0.0f, 0.0f };
	const glm::vec3 Red = { 1.0f, 0.0f, 0.0f };
	const glm::vec3 Blue = { 0.0f, 0.0f, 1.0f };
}

//class RigidBody;
//class GameManager;
//class Sphere;
//class GameObject;

#endif // !_DEFS_H