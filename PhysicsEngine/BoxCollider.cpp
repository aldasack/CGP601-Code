////////////////////////////////
// Name: BoundingBox.cpp      //
// Author: Alexander Kocourek //
// Date: 27/03/2017			  //
////////////////////////////////

#include "BoxCollider.h"

using namespace Collision;

BoxCollider::BoxCollider(RigidBody& rigidbody/*, const glm::vec3& scale*/) : Collider(rigidbody)
{
	// TODO: scale edges according to the GameObject.
	// Changing the scale later will have no affect to the Collider.

	// cube with a = 1m standing in the ground y = 0.
	m_edges[0] = glm::vec3(-0.5f, 1.0f, -0.5f);
	m_edges[1] = glm::vec3(0.5f, 1.0f, -0.5f);
	m_edges[2] = glm::vec3(0.5f, 1.0f, 0.5f);
	m_edges[3] = glm::vec3(-0.5f, 1.0f, 0.5f);
	m_edges[4] = glm::vec3(-0.5f, 0.0f, -0.5f);
	m_edges[5] = glm::vec3(0.5f, 0.0f, -0.5f);
	m_edges[6] = glm::vec3(0.5f, 0.0f, 0.5f);
	m_edges[7] = glm::vec3(-0.5f, 0.0f, 0.5f);

	m_center = glm::vec3(0.0f, 0.0f, 0.0f);
	m_extent[0] = 0.5f;
	m_extent[1] = 0.5f;
	m_extent[2] = 0.5f;
	m_axes[0] = glm::vec3(1.0f, 0.0f, 0.0f);
	m_axes[1] = glm::vec3(0.0f, 1.0f, 0.0f);
	m_axes[2] = glm::vec3(0.0f, 0.0f, 1.0f);
}

BoxCollider::~BoxCollider(){}

void BoxCollider::Update()
{
	// Translating the box collider.
	glm::vec3& position = m_pRigidBody->GetPosition();
	for (unsigned int i = 0; i < m_edges.size(); i++)
	{
		m_transformedEdges[i] = m_edges[i] + position;
	}
	m_center = position;


	// Rotating the box collider / rotating the axis
	m_rotation = m_pRigidBody->GetQuaternionRotation();
	// x axis (1,0,0)
	m_axes[0] = Math::rotateVector(glm::vec3(1.0f, 0.0f, 0.0f), m_rotation);
	m_axes[0] = glm::normalize(m_axes[0]);
	// y axis (0,1,0)
	m_axes[1] = Math::rotateVector(glm::vec3(0.0f, 1.0f, 0.0f), m_rotation);
	m_axes[1] = glm::normalize(m_axes[1]);
	// z axis (0,0,1)
	m_axes[2] = Math::rotateVector(glm::vec3(0.0f, 0.0f, 1.0f), m_rotation);
	m_axes[2] = glm::normalize(m_axes[2]);

	// TODO: Scale
}

std::array<glm::vec3, 8> BoxCollider::GetEdges() const
{
	return m_transformedEdges;
}

glm::vec3 BoxCollider::GetCenter() const
{
	return m_center;
}

std::array<glm::vec3, 3> BoxCollider::GetAxes() const
{
	return m_axes;
}

glm::vec3 BoxCollider::GetExtents() const
{
	return m_extent;
}

glm::quat BoxCollider::GetRotation() const
{
	// check if quaternion is normalized
	float length = glm::length(m_rotation);
	DBG_ASSERT(abs(length - 1.0f) < 0.001f);

	return m_rotation;
}
