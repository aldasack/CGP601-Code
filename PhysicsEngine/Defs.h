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

namespace Constants
{
	const float G = 9.81f;
	const float Pi = 4.0f * atan(1.0f);
}

namespace Time
{
	static float TotalTime = 0.0f; // time in seconds
}

namespace Math
{
	struct Vector3
	{
		float x, y, z;
		/*void Zero()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}*/
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
		Math::Vector3 position;
		float radius;
	};
}

//class RigidBody;
//class GameManager;
//class Sphere;
//class GameObject;

#endif // !_DEFS_H