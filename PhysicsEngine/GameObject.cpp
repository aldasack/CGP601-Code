////////////////////////////////
// Name: GameObject.cpp		  //
// Author: Alexander Kocourek //
// Date: 16/02/2017			  //
////////////////////////////////

#include "GameObject.h"

#include "RigidBody.h"
#include "SphereCollider.h"

GameObject::GameObject()
{
	m_color = { 1.0f, 1.0f, 1.0f };
	m_scale = { 1.0f, 1.0f, 1.0f };
	m_pRigidbody = new RigidBody(*this);
}

GameObject::GameObject(glm::vec3 position) : GameObject()
{
	m_pRigidbody->SetPosition(position);
}

GameObject::GameObject(glm::vec3 position, glm::vec3 rotation) : GameObject()
{
	m_pRigidbody->SetEulerRotation(rotation);
}

GameObject::~GameObject(){}

void GameObject::SetPosition(const glm::vec3& position)
{
	m_pRigidbody->SetPosition(position);
}

void GameObject::SetScale(const glm::vec3& scale)
{
	// scale cant be negative or zero
	DBG_ASSERT(scale.x > 0.0f && scale.y > 0.0f && scale.z > 0.0f);
	m_scale = scale;
}

void GameObject::SetRotation(const glm::vec3& rotation)
{
	m_pRigidbody->SetEulerRotation(rotation);
}

void GameObject::SetVelocity(const glm::vec3& velocity)
{
	m_pRigidbody->SetVelocity(velocity);
}

void GameObject::SetMass(const float mass)
{
	m_pRigidbody->SetMass(mass);
}

void GameObject::SetColor(const glm::vec3& color)
{
	m_color = color;
}

glm::vec3 GameObject::GetPosition() const
{
	return m_pRigidbody->GetPosition();
}

glm::vec3 GameObject::GetScale() const
{
	return m_scale;
}

glm::vec3 GameObject::GetVelocity() const
{
	return m_pRigidbody->GetVelocity();
}

float GameObject::GetMass() const
{
	return m_pRigidbody->GetMass();
}

std::vector<glm::vec3> GameObject::GetVertices() const
{
	DBG_ASSERT(m_vertices.size() > 0);
	return m_vertices;
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

void GameObject::AddForce(const glm::vec3& force)
{
	m_pRigidbody->AddForce(force);
}

void GameObject::AddForce(const glm::vec3& force, const glm::vec3& position)
{
	m_pRigidbody->AddForce(force, position);
}

void GameObject::AddTorque(const glm::vec3& torque)
{
	m_pRigidbody->AddTorque(torque);
}

void GameObject::shutdown()
{
	// m_pRigidbody->Shoutdown()
	if (m_pRigidbody)
	{
		m_pRigidbody->Shutdown();
		delete m_pRigidbody;
		m_pRigidbody = nullptr;
	}
}

void GameObject::prepareDraw()
{
	glPushMatrix();
	glColor3f(m_color.x, m_color.y, m_color.z);

	glm::vec3 pos = m_pRigidbody->GetPosition();
	glm::vec4 rotation = m_pRigidbody->GetAxisAngleRotation();

	// check if axis is normalized
	// don't think it's required!!!!!
	glm::vec3 rr = glm::vec3(rotation.x, rotation.y, rotation.z);
	float ll = glm::length(rr);
	assert(abs(ll - 1) < 0.0001);
	
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(rotation.w * Constants::Rad2Deg, rotation.x, rotation.y, rotation.z);
	glScalef(m_scale.x, m_scale.y, m_scale.z);
}