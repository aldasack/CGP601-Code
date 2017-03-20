////////////////////////////////
// Name: RigidBody.cpp		  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include "RigidBody.h"

RigidBody::RigidBody()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	// creating quaternion of euler angles
	m_rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	m_rotation = glm::normalize(m_rotation);

	m_velocity.x = 0.0f;
	m_velocity.y = 0.0f;
	m_velocity.z = 0.0f;

	m_angularVelocity.x = 0.0f;
	m_angularVelocity.y = 0.0f;
	m_angularVelocity.z = 0.0f;

	m_acceleration.x = 0.0f;
	m_acceleration.y = 0.0f;
	m_acceleration.z = 0.0f;

	m_angularAcceleration.x = 0.0f;
	m_angularAcceleration.y = 0.0f;
	m_angularAcceleration.z = 0.0f;

	m_force.x = 0.0f;
	m_force.y = 0.0f;
	m_force.z = 0.0f;

	m_torque.x = 0.0f;
	m_torque.y = 0.0f;
	m_torque.z = 0.0f;

	m_mass = 1.0f;

	m_centreOfMass.x = 0.0f;
	m_centreOfMass.y = 0.0f;
	m_centreOfMass.z = 0.0f;

	m_inertiaTensor = glm::mat3(1.0);
	m_inverseInertiaTensor = glm::inverse(m_inertiaTensor);

	m_isStatic = false;
	m_useGravity = true;

	m_linearDamping = 0.99f;
	m_angularDamping = 0.99f;

	/*m_collider.position = m_position;
	m_collider.radius = 0.5f;*/
}

RigidBody::~RigidBody(){}

void RigidBody::Update(float dt)
{
	// if object is static
	if (m_isStatic)
		return;

	/*
	*	a : Acceleration
	*	M : Torque
	*	I : Moment of Inertia
	*	F : Force
	*	v : Velocity
	*	m : Mass
	*/

#pragma region Linear Movement
	// 1. Step: Calculate acceleration (a). a = F/m
	m_acceleration.x = m_force.x / m_mass;
	m_acceleration.y = m_force.y / m_mass;
	m_acceleration.z = m_force.z / m_mass;
	if (m_useGravity)
	{
		// adding acceleration of gravity. Because this acceleration is constant, it is unecessary to calculate the Force first
		m_acceleration.y += Constants::G * -1.0f; // acceleration goes down on the y-axis and needs to be negative.
	}

	// 2. Step: Calculate velocity (v) at this moment. v = a * t
	m_velocity.x += m_acceleration.x * dt;
	m_velocity.y += m_acceleration.y * dt;
	m_velocity.z += m_acceleration.z * dt;
	//if (m_velocity.y <= 0.000001f && m_velocity.y >= -0.000001f) m_velocity.y = 0.0f;

	// 3. Step: Calcuate travelled distance (s) on each axis during the time step. s = v * t
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

#pragma endregion

#pragma region Angular Movement
	// 1. Step: Calculate acceleration (a). a = M/I
	m_angularAcceleration = m_inverseInertiaTensor * m_torque; // multiplication because of inverted tensor

	// 2. Step: Calculate velocity (v) at this moment. v = a * t
	m_angularVelocity += m_angularAcceleration * dt;

	// 3. Step: Calcuate travelled distance / angle (s) on each axis during the time step. s = v * t
	glm::vec3 rotation = m_angularVelocity * dt;

	// 4. Adding resulting rotation to objects rotation / orientation
	Rotate(rotation);

#pragma endregion

	resetAccumulators();

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

void RigidBody::SetPosition(const glm::vec3& position)
{
	m_position = position;
}

glm::vec3 RigidBody::GetVelocity() const
{
	return m_velocity;
}

void RigidBody::SetVelocity(const glm::vec3& velocity)
{
	m_velocity = velocity;
}

float RigidBody::GetMass() const
{
	return m_mass;
}

float RigidBody::GetInverseMass() const
{
	return m_inverseMass;
}

void RigidBody::SetMass(const float mass)
{
	// mass can not be zero or less
	assert(mass > 0.0f);
	if (mass <= 0.0f)
	{
		throw  std::exception("Mass can't be negative or zero!");
		return;
	}
	m_mass = mass;
	m_inverseMass = 1 / mass;
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

glm::vec3 RigidBody::GetRotation() const
{
	return glm::eulerAngles(m_rotation);
}

glm::vec4 RigidBody::GetAxisAngleRotation()
{
	m_rotation = glm::normalize(m_rotation);
	glm::vec4 rotation;
	float s = sqrt(1 - m_rotation.w * m_rotation.w); // error when w is 1 !?
	if (abs(s) > 0.001f)
	{
		rotation.x = m_rotation.x / s;
		rotation.y = m_rotation.y / s;
		rotation.z = m_rotation.z / s;
		rotation = glm::normalize(rotation);
		rotation.w = 2 * acosf(m_rotation.w);
	}
	else
	{
		// no rotation, leading to devide be zero. Solved by setting rotation axis manually.
		rotation.x = 1.0f;
		rotation.y = 0.0f;
		rotation.z = 0.0f;
		rotation = glm::normalize(rotation);
		rotation.w = 0.0f;
	}

	float tmp = glm::length(m_rotation);
	assert(abs(tmp - 1.0f) < 0.001f);
	return rotation;
	// source: http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/
}

void RigidBody::SetRotation(const glm::vec3& rotation)
{
	m_rotation = glm::quat(rotation);
	m_rotation = glm::normalize(m_rotation);
}

void RigidBody::SetInertiaTensor(const glm::mat3& tensor)
{
	m_inertiaTensor = tensor;
	m_inverseInertiaTensor = glm::inverse(m_inertiaTensor);
}

//void RigidBody::SetForce(const glm::vec3& force)
//{
//	m_force = force;
//}

void RigidBody::AddForce(const glm::vec3& force)
{
	m_force.x += force.x;
	m_force.y += force.y;
	m_force.z += force.z;
}

void RigidBody::AddForce(const glm::vec3& force, const glm::vec3& position)
{
	// Adding force
	m_force.x += force.x;
	m_force.y += force.y;
	m_force.z += force.z;

	// Calculating distance from centre of mass 
	glm::vec3 distance = position - m_centreOfMass;
	// Calculating resulting torque
	glm::vec3 torque = glm::cross(distance, force);
	// Adding torque
	AddTorque(torque);
}

void RigidBody::AddTorque(const glm::vec3& torque)
{
	// Adding torque
	m_torque.x += torque.x;
	m_torque.y += torque.y;
	m_torque.z += torque.z;
}

void RigidBody::Rotate(const glm::vec3& rotation)
{
	glm::quat tmp = glm::quat(rotation) * m_rotation;
	glm::normalize(tmp);
	m_rotation = tmp;
	
	// check if quaternion is normalized
	float length = glm::length(m_rotation);
	assert(abs(length - 1.0f) < 0.001f);
}

void RigidBody::resetAccumulators()
{
	m_force.x = 0.0f;
	m_force.y = 0.0f;
	m_force.z = 0.0f;

	m_torque.x = 0.0f;
	m_torque.y = 0.0f;
	m_torque.z = 0.0f;
}