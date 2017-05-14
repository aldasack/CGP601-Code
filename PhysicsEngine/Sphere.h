////////////////////////////////
// Name: Sphere.h			  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#pragma once

#ifndef _SPHERE_H
#define _SPHERE_H


#include "Defs.h"
#include "GameObject.h"

class Sphere : public GameObject
{
public:
	Sphere();
	/*
	*	Creates a Sphere at 'position'.
	*/
	Sphere(const glm::vec3 position);
	/*
	*	Creates a Sphere at 'position' with 'radius'.
	*/
	Sphere(const glm::vec3 position, const float radius);
	~Sphere();
	/*
	*	Destructor replacement.
	*/
	void Shutdown() override;
	/*
	*	Draws Sphere in the scene.
	*/
	void Draw() override;
	/*
	*	Sets radius of the Sphere.
	*/
	void SetRadius(const float radius);

private:
	/*
	*	Radius of sphere
	*/
	float m_radius;
	/*
	*	Number of segments. The more segments the less boxy.
	*/
	const int m_segments = 25;
	/*
	*	Calculates the inertia tensor depending on radius and mass;
	*/
	void setInertiaTensor();
	/*
	*	Fills the vertices list with vectors, which represent a box.
	*/
	void setVertices();
};

#endif // !_SPHERE_H_