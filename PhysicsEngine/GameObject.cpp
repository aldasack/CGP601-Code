////////////////////////////////
// Name: GameObject.cpp		  //
// Author: Alexander Kocourek //
// Date: 16/02/2017			  //
////////////////////////////////

#include "GameObject.h"

GameObject::GameObject()
{
	m_color = { 1.0f, 1.0f, 1.0f };
	m_pRigidbody = new RigidBody();
}

GameObject::GameObject(glm::vec3 position) : GameObject()
{
	m_pRigidbody->SetPosition(position);
}

GameObject::~GameObject(){}

void GameObject::Update(float dt)
{
	//m_color = Colors::White;
	m_pRigidbody->Update(dt);
	m_collider.position = m_pRigidbody->GetPosition();
}

void GameObject::SetPosition(const glm::vec3 position)
{
	m_pRigidbody->SetPosition(position);
}

void GameObject::SetVelocity(const glm::vec3 velocity)
{
	m_pRigidbody->SetVelocity(velocity);
}

void GameObject::SetColor(const glm::vec3 color)
{
	m_color = color;
}

glm::vec3 GameObject::GetPosition() const
{
	return m_pRigidbody->GetPosition();
}

glm::vec3 GameObject::GetVelocity() const
{
	return m_pRigidbody->GetVelocity();
}

float GameObject::GetMass() const
{
	return m_pRigidbody->GetMasss();
}

Collider::SphereCollider GameObject::GetCollider() const
{
	return m_collider;
}

bool GameObject::IsStatic() const
{
	return m_pRigidbody->IsStatic();
}

void GameObject::IsStatic(const bool isStatic)
{
	m_pRigidbody->IsStatic(isStatic);
}

bool GameObject::UseGravity() const
{
	return m_pRigidbody->UseGravity();
}

void GameObject::UseGravity(const bool useGravity)
{
	m_pRigidbody->UseGravity(useGravity);
}

//glm::vec3 GameObject::GetAcceleration() const
//{
//	return;
//}
//glm::vec3 GameObject::GetVelocity() const
//{
//
//}

void GameObject::AddForce(const glm::vec3 force)
{
	m_pRigidbody->AddForce(force);
}

void GameObject::shutdown()
{
	// m_pRigidbody->Shoutdown()
	m_pRigidbody = nullptr;
	delete m_pRigidbody;
}

void GameObject::prepareDraw()
{
	glPushMatrix();
	glColor3f(m_color.x, m_color.y, m_color.z);

	glm::vec3 pos = m_pRigidbody->GetPosition();
	//glLoadIdentity();
	glTranslatef(pos.x, pos.y, pos.z);
}