////////////////////////////////
// Name: GameObject.cpp		  //
// Author: Alexander Kocourek //
// Date: 16/02/2017			  //
////////////////////////////////

#include "GameObject.h"

GameObject::GameObject()
{
	m_color = { 1.0f, 1.0f, 1.0f };
	m_rigidbody = new RigidBody();
}

GameObject::GameObject(Math::Vector3 position) : GameObject()
{
	m_rigidbody->SetPosition(position);
}

GameObject::~GameObject(){}

void GameObject::Shutdown()
{
	// shoutdown()
	m_rigidbody = nullptr;
	delete m_rigidbody;
}

void GameObject::Draw()
{
	glColor3f(m_color.x, m_color.y, m_color.z);
	
	Math::Vector3 pos = m_rigidbody->GetPosition();
	glLoadIdentity();
	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidSphere(0.5f, 25, 25);
}

void GameObject::Update(float dt)
{
	m_rigidbody->Update(dt);
}

void GameObject::SetPosition(const Math::Vector3 position)
{
	m_rigidbody->SetPosition(position);
}

void GameObject::SetColor(const Math::Vector3 color)
{
	m_color = color;
}

Math::Vector3 GameObject::GetPosition() const
{
	return m_rigidbody->GetPosition();
}

//Math::Vector3 GameObject::GetAcceleration() const
//{
//	return;
//}
//Math::Vector3 GameObject::GetVelocity() const
//{
//
//}