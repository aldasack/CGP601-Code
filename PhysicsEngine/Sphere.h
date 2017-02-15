#pragma once

#ifndef _SPHERE_H
#define _SPHERE_H

////////////////////////////////
// Name: Sphere.h			  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include "Defs.h"
#include "RigidBody.h"

class Sphere : public RigidBody
{
public:
	//Sphere();
	Sphere(const Math::Vector3 position);
	~Sphere();
	//void Initialize(float radius, float r, float g, float b, float posX, float posY, float posZ);
	//void Initialize();
	void Draw();
	void Update(float dt);

private:
	float m_radius;
	float m_color[3];
	const int m_segments = 25;
};

#endif // !_SPHERE_H_