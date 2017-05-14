////////////////////////////////
// Name: Plane.h			  //
// Author: Alexander Kocourek //
// Date: 14/03/2017			  //
////////////////////////////////

#pragma once

#ifndef _PLANE_H
#define _PLANE_H

#include "Defs.h"
#include "GameObject.h"

class Plane : public GameObject
{
public:
	Plane();
	/*
	*	Constructor positioning Plane at 'position'.
	*/
	Plane(const glm::vec3 position);
	/*
	*	Constructor positioning Plane at 'position' and scaling it whith 'scale'.
	*/
	Plane(const glm::vec3 position, const glm::vec3 scale);
	~Plane();
	/*
	*	Destructor replacement.
	*/
	void Shutdown() override;
	/*
	*	Draws Box in the scene.
	*/
	void Draw() override;

private:
	/*
	*	Normal vecor of the plane. Is normalized.
	*/
	glm::vec3 m_normal;

	/*
	*	Fills the vertices list with vectors, which represent a box.
	*/
	void setVertices();
};

#endif // !_PLANE_H