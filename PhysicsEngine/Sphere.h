#pragma once

#ifndef _SPHERE_H
#define _SPHERE_H

////////////////////////////////
// Name: Sphere.h			  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include "Defs.h"
#include "GameObject.h"

class Sphere : public GameObject
{
public:
	Sphere();
	Sphere(const glm::vec3 position);
	Sphere(const glm::vec3 position, const float radius);
	~Sphere();
	void Shutdown() override;

	void Draw() override;
	void SetRadius(const float radius);

private:
	float m_radius;
	const int m_segments = 25;
};

#endif // !_SPHERE_H_