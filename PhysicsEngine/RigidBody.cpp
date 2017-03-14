////////////////////////////////
// Name: RigidBody.cpp		  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include "RigidBody.h"

float time = 0.0f;
float threshold = 0.1f;

bool bo = false;

RigidBody::RigidBody()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_force.x = 0.0f;
	m_force.y = 0.0f;
	m_force.z = 0.0f;

	m_acceleration.x = 0.0f;
	m_acceleration.y = 0.0f;
	m_acceleration.z = 0.0f;

	m_velocity.x = 0.0f;
	m_velocity.y = 0.0f;
	m_velocity.z = 0.0f;

	m_mass = 1.0f;

	m_isStatic = false;
	m_useGravity = true;

	/*m_collider.position = m_position;
	m_collider.radius = 0.5f;*/
}

RigidBody::~RigidBody(){}

void RigidBody::Update(float dt)
{
	// if object is static
	if (m_isStatic)
		return;

	// 1. Step: Calculate acceleration (a). a = F/m
	m_acceleration.x = m_force.x / m_mass;
	m_acceleration.y = m_force.y / m_mass;
	m_acceleration.z = m_force.z / m_mass;
	if (m_useGravity)
	{
		// adding acceleration of gravity. Because this acceleration is constant, it is unecessary to calculate the Force first
		m_acceleration.y += Constants::G * -1.0f; // acceleration goes down on the y-axis and needs to be negative.
	}

	// 2. Step: Calculate velocity (v) in this moment.
	m_velocity.x += m_acceleration.x * dt;
	m_velocity.y += m_acceleration.y * dt;
	m_velocity.z += m_acceleration.z * dt;
	//if (m_velocity.y <= 0.000001f && m_velocity.y >= -0.000001f) m_velocity.y = 0.0f;
	
	// 3. Step: Calcuate travelled distance (s) on each axis during the time step.
	glm::vec3 s;
	s.x = m_velocity.x * dt;
	s.y = m_velocity.y * dt;
	s.z = m_velocity.z * dt;

	// 4. Step: Adding resulting distances to the position.
	m_position.x += s.x;
	m_position.y += s.y;
	m_position.z += s.z;


	if (m_position.y <= -0.5f) // -1.0 is the height of the plane, radius is 0.5m
	{
		m_velocity.y *= -1.0f; // reverse velocity on collision, no energy is lost and is completly accelerating the object
		m_position.y = -0.5; // pushing sphere out of the ground
	}

	resetForce();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Testcase for Objective 1: Gravity on Sphere
	// get time of fall.
	// s = 6m; a = 9.81 m/s^2 => t ~ 1.1s
	//time += dt;
	//if (m_position.y <= -0.5f && bo == false) // -1.0 is the height of the plane, radius is 0.5m
	//{
	//	bo = true;
	//	//std::cout << glutGet(GLUT_ELAPSED_TIME) * 0.001f << std::endl;
	//	std::cout << "Time of fall: " << time << std::endl;
	//	std::cout << "Velcoity: " << m_velocity.y << std::endl;
	//}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

glm::vec3 RigidBody::GetPosition() const
{
	return m_position;
}

void RigidBody::SetPosition(const glm::vec3 position)
{
	m_position = position;
}

glm::vec3 RigidBody::GetVelocity() const
{
	return m_velocity;
}

void RigidBody::SetVelocity(const glm::vec3 velocity)
{
	m_velocity = velocity;
}

float RigidBody::GetMasss() const
{
	return m_mass;
}

bool RigidBody::IsStatic() const
{
	return m_isStatic;
}

void RigidBody::IsStatic(const bool isStatic)
{
	m_isStatic = isStatic;
}

bool RigidBody::UseGravity() const
{
	return m_useGravity;
}

void RigidBody::UseGravity(const bool useGravity)
{
	m_useGravity = useGravity;
}

//void RigidBody::SetForce(const glm::vec3 force)
//{
//	m_force = force;
//}

void RigidBody::AddForce(const glm::vec3 force)
{
	m_force.x += force.x;
	m_force.y += force.y;
	m_force.z += force.z;
}

void RigidBody::resetForce()
{
	m_force.x = 0.0f;
	m_force.y = 0.0f;
	m_force.z = 0.0f;
}