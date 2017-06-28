////////////////////////////////
// Name: RigidBody.cpp		  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include "RigidBody.h"

#include "Collider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "MeshCollider.h"
#include "PhysicsManager.h"

RigidBody::RigidBody(GameObject& gameObject)
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
	m_inverseMass = 1.0f / m_mass;

	m_centreOfMass.x = 0.0f;
	m_centreOfMass.y = 0.0f;
	m_centreOfMass.z = 0.0f;

	m_inertiaTensor = glm::mat3(1.0);
	m_inverseInertiaTensor = glm::inverse(m_inertiaTensor);

	m_isStatic = false;
	m_useGravity = true;

	m_linearDamping = 0.99f;
	m_angularDamping = 0.99f;

	DBG_ASSERT(&gameObject != nullptr);
	m_pGameObject = &gameObject;

	m_pSphereCollider = new Collision::SphereCollider(*this);
	//m_pSphereCollider->SetPosition(m_position);
	//m_pSphereCollider->SetRadius(0.5f);
	m_pBoxCollider = new Collision::BoxCollider(*this);
	m_pMeshCollider = new Collision::MeshCollider(*this);

	// register RigidBody with the PhysicsManager
	PhysicsManager::GetInstance().AddRigidBody(*this);
}

RigidBody::~RigidBody(){}

void RigidBody::Update(float dt)
{
	// if object is static
	if (!m_isStatic)
	{
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
		m_acceleration = m_force / m_mass;

		if (m_useGravity)
		{
			// Adding acceleration of gravity. Because this acceleration is constant, it is unecessary to calculate the Force first
			m_acceleration.y += Constants::G * -1.0f; // acceleration goes down on the y-axis and needs to be negative.
		}

		// 2. Step: Calculate velocity (v) at this moment. v = a * t
		m_velocity += m_acceleration * dt;
		//if (m_velocity.y <= 0.000001f && m_velocity.y >= -0.000001f) m_velocity.y = 0.0f;

		// 3. Step: Calcuate travelled distance (s) on each axis during the time step. s = v * t
		glm::vec3 s = m_velocity * dt;

		// 4. Step: Adding resulting distances to the position.
		m_position += s;

		//if (m_position.y <= -0.5f) // -1.0 is the height of the plane, radius is 0.5m
		//{
		//	m_velocity.y *= -1.0f; // reverse velocity on collision, no energy is lost and is completly accelerating the object
		//	m_position.y = -0.5; // pushing sphere out of the ground
		//}

#pragma endregion

#pragma region Angular Movement
		// 1. Step: Calculate acceleration (a). a = M/I
		m_angularAcceleration = m_inverseInertiaTensor * m_torque; // multiplication because of inverted tensor; interia  tensor needs to be rotated

																   // 2. Step: Calculate velocity (v) at this moment. v = a * t
		m_angularVelocity += m_angularAcceleration * dt;

		// 3. Step: Calcuate travelled distance / angle (s) on each axis during the time step. s = v * t
		glm::vec3 rotation = m_angularVelocity * dt;

		// 4. Adding resulting rotation to objects rotation / orientation
		Rotate(rotation);

#pragma endregion

		resetAccumulators();
	}
		
	// Updating Colliders to transform them the same way as the rigidbody
	m_pSphereCollider->Update();
	m_pBoxCollider->Update();
	m_pMeshCollider->Update();
}

void RigidBody::Shutdown()
{
	if (m_pSphereCollider)
	{
		m_pSphereCollider->Shutdown();
		delete m_pSphereCollider;
		m_pSphereCollider = nullptr;
	}
	if (m_pBoxCollider)
	{
		m_pBoxCollider->Shutdown();
		delete m_pBoxCollider;
		m_pBoxCollider = nullptr;
	}
	if (m_pMeshCollider)
	{
		m_pMeshCollider->Shutdown();
		delete m_pMeshCollider;
		m_pMeshCollider = nullptr;
	}
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

glm::vec3 RigidBody::GetAngularVelocity() const
{
	return m_angularVelocity;
}

void RigidBody::SetAngularVelocity(const glm::vec3& velocity)
{
	m_angularVelocity = velocity;
}

float RigidBody::GetMass() const
{
	DBG_ASSERT(m_mass > 0.0f);
	return m_mass;
}

float RigidBody::GetInverseMass() const
{
	return m_inverseMass;
}

void RigidBody::SetMass(const float mass)
{
	// mass can not be zero or less
	DBG_ASSERT(mass > 0.0f);

	m_mass = mass;
	m_inverseMass = 1.0f / mass;
}

glm::vec3 RigidBody::GetCentreOfMass() const
{
	return m_position + m_centreOfMass;
}

void RigidBody::SetCentreOfMass(const glm::vec3& centre)
{
	m_centreOfMass = centre;
}

bool RigidBody::IsStatic() const
{
	return m_isStatic;
}

void RigidBody::IsStatic(const bool isStatic)
{
	// setting mass to "infinity" for collision response
	if (isStatic)
		m_inverseMass = 0.0f;
	else
		m_inverseMass = 1.0f / m_mass;
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

glm::vec3 RigidBody::GetEulerRotation() const
{
	return glm::eulerAngles(m_rotation);
}

glm::vec4 RigidBody::GetAxisAngleRotation()
{
	m_rotation = glm::normalize(m_rotation);
	glm::vec4 rotation;
	float s = sqrt(1 - m_rotation.w * m_rotation.w); // error when w is 1 !?
	if (abs(s) > Constants::Precision)
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
	DBG_ASSERT(abs(tmp - 1.0f) < Constants::Precision);
	return rotation;
	// source: http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/
}

glm::quat RigidBody::GetQuaternionRotation() const
{
	// check if quaternion is normalized
	float length = glm::length(m_rotation);
	DBG_ASSERT(abs(length - 1.0f) < Constants::Precision);
	return m_rotation;
}

void RigidBody::SetEulerRotation(const glm::vec3& rotation)
{
	m_rotation = glm::quat(rotation);
	m_rotation = glm::normalize(m_rotation);
	
	float length = glm::length(m_rotation);
	DBG_ASSERT(abs(length - 1.0f) < Constants::Precision);
}

void RigidBody::SetQuaterionRotation(const glm::quat& rotation)
{
	m_rotation = glm::normalize(rotation);
}

glm::mat3 RigidBody::GetInertiaTensor() const
{
	return m_inertiaTensor;
}

void RigidBody::SetInertiaTensor(const glm::mat3& tensor)
{
	m_inertiaTensor = tensor;
	m_inverseInertiaTensor = glm::inverse(m_inertiaTensor);
}

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

	// Calculating distance from centre of mass to applied point
	glm::vec3 distance = position - (m_position - m_centreOfMass);
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
	DBG_ASSERT(abs(length - 1.0f) < Constants::Precision);
}

Collision::SphereCollider& RigidBody::GetSphereCollider() const
{
	DBG_ASSERT(m_pSphereCollider != nullptr);
	return *m_pSphereCollider;
}

Collision::BoxCollider& RigidBody::GetBoxCollider() const
{
	DBG_ASSERT(m_pBoxCollider != nullptr);
	return *m_pBoxCollider;
}

Collision::MeshCollider& RigidBody::GetMeshCollider() const
{
	DBG_ASSERT(m_pMeshCollider != nullptr);
	return *m_pMeshCollider;
}

GameObject& RigidBody::GetGameObject() const
{
	DBG_ASSERT(m_pGameObject != nullptr);
	return *m_pGameObject;
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