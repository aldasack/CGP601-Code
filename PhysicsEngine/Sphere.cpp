////////////////////////////////
// Name: Sphere.cpp			  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include "Sphere.h"

Sphere::Sphere() : GameObject()
{
	m_radius = 0.5f;
}

Sphere::Sphere(const Math::Vector3 position) : GameObject(position)
{
	m_radius = 0.5f;
}

Sphere::Sphere(const Math::Vector3 position, const float radius) : GameObject(position)
{
	m_radius = radius;
}

Sphere::~Sphere(){}

void Sphere::Draw()
{
	GameObject::Draw();
	glutSolidSphere(m_radius, m_segments, m_segments);
}

void Sphere::SetRadius(const float radius)
{
	m_radius = radius;
}