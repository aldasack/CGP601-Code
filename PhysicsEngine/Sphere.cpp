////////////////////////////////
// Name: Sphere.cpp			  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include "Sphere.h"

//Sphere::Sphere()
//{
//	
//	
//}

Sphere::Sphere(const Math::Vector3 position) : RigidBody()
{
	m_radius = 0.0f;
	m_color[0] = 1.0f;
	m_color[1] = 1.0f;
	m_color[2] = 1.0f;

	m_radius = 0.5f;

	RigidBody::SetPosition(position);
}

Sphere::~Sphere(){}

//void Sphere::Initialize()
//{
//	Initialize(0.5f, 1.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
//}
//
//void Sphere::Initialize(float radius, float r, float g, float b, float posX, float posY, float posZ)
//{
//	m_radius = radius;
//
//	m_color[0] = r;
//	m_color[1] = g;
//	m_color[2] = b;
//}

void Sphere::Draw()
{
	Math::Vector3 pos = RigidBody::GetPosition();

	glColor3f(m_color[0], m_color[1], m_color[2]);
	
	glLoadIdentity();
	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidSphere(m_radius, m_segments, m_segments);
}

void Sphere::Update(float dt)
{
	RigidBody::Update(dt);
}