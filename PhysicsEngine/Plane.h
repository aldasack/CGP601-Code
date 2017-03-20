#pragma once

#ifndef _PLANE_H
#define _PLANE_H

////////////////////////////////
// Name: Plane.h			  //
// Author: Alexander Kocourek //
// Date: 14/03/2017			  //
////////////////////////////////

#include "Defs.h"
#include "GameObject.h"

class Plane : public GameObject
{
public:
	Plane();
	Plane(const glm::vec3 position);
	Plane(const glm::vec3 position, const glm::vec3 scale);
	~Plane();
	void Shutdown() override;

	void Draw() override;
private:
	glm::vec3 m_normal;
};

#endif // !_PLANE_H