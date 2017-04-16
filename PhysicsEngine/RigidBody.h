#pragma once

#ifndef _RIGIDBODY_H
#define _RIGIDBODY_H

////////////////////////////////
// Name: RigidBody.h		  //
// Author: Alexander Kocourek //
// Date: 06/02/2017			  //
////////////////////////////////

#include <iostream>

#include "Defs.h"

class RigidBody
{
public:
	RigidBody();
	//RigidBody(const glm::vec3 position);
	~RigidBody();
	void Update(float dt);
	void Shutdown();

	glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& position);
	//void SetForce(const glm::vec3& force);

	glm::vec3 GetVelocity() const;
	void SetVelocity(const glm::vec3& velocity);
	
	float GetMass() const;
	float GetInverseMass() const;
	void SetMass(const float mass);

	bool IsStatic() const;
	void IsStatic(const bool isStatic);

	bool UseGravity() const;
	void UseGravity(const bool useGravity);
	// Returns rotation as euler angles in radians
	glm::vec3 GetEulerRotation() const;
	// Returns rotation in angle-axis representation. 
	glm::vec4 GetAxisAngleRotation();
	// Returns rotation as a queternion
	glm::quat GetQuaternionRotation() const;
	// Rotation is represented as euler angles in radians
	void SetEulerRotation(const glm::vec3& rotation);
	// Setting the Inertia Tensor
	void SetInertiaTensor(const glm::mat3& tensor);

	// Adds a force to the rigidbody at its centre of mass and calculates the resulting force of all applied forces
	void AddForce(const glm::vec3& force);
	// Adds a force at the given position, also results in applying a torque
	void AddForce(const glm::vec3& force, const glm::vec3& position);
	// Adds torque to the rigidbody and calculates the resulting force of all applied forces
	// curently at the centre of the object, not the center of mass
	void AddTorque(const glm::vec3& torque);
	// rotation is represented as euler angles in radians
	void Rotate(const glm::vec3& rotation);

	Collision::SphereCollider& GetSphereCollider() const;
	Collision::BoxCollider& GetBoxCollider() const;
	Collision::MeshCollider& GetMeshCollider() const;

private:
	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_velocity;
	float m_linearDamping;
	// Angular velocity in radians per second
	glm::vec3 m_angularVelocity;
	float m_angularDamping;
	glm::vec3 m_acceleration;
	glm::vec3 m_angularAcceleration;
	glm::vec3 m_force;
	glm::vec3 m_torque;

	float m_mass;
	// Inverse mass is needed for collision response calculations
	float m_inverseMass;
	glm::vec3 m_centreOfMass;
	// 
	float m_bounciness;
	
	// description needed
	glm::mat3x3 m_inertiaTensor;
	glm::mat3x3 m_inverseInertiaTensor;

	bool m_isStatic;
	bool m_useGravity;

	Collision::SphereCollider* m_pSphereCollider;
	Collision::BoxCollider* m_pBoxCollider;
	Collision::MeshCollider* m_pMeshCollider;

	// Resets the force and torque accumulators after each iteration
	void resetAccumulators();
};

#endif // !_RIGIDBODY_H