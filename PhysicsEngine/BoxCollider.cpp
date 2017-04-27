////////////////////////////////
// Name: BoundingBox.cpp      //
// Author: Alexander Kocourek //
// Date: 27/03/2017			  //
////////////////////////////////

#include "BoxCollider.h"

#include "GameObject.h"

using namespace Collision;

BoxCollider::BoxCollider(RigidBody& rigidbody/*, const glm::vec3& scale*/) : Collider(rigidbody)
{
	m_center = m_pRigidBody->GetPosition();
	m_offset = { 0.0f, 0.0f, 0.0f };
	m_extent = { 0.5f, 0.5f, 0.5f };
	m_axes[0] = { 1.0f, 0.0f, 0.0f };
	m_axes[1] = { 0.0f, 1.0f, 0.0f };
	m_axes[2] = { 0.0f, 0.0f, 1.0f };
}

BoxCollider::~BoxCollider(){}

void BoxCollider::Update()
{
	// Translating the box collider.
	m_center = m_pRigidBody->GetPosition();

	// Rotating the box collider / rotating the axis
	glm::quat m_rotation = m_pRigidBody->GetQuaternionRotation();
	// x axis (1,0,0)
	m_axes[0] = Math::rotateVector(glm::vec3(1.0f, 0.0f, 0.0f), m_rotation);
	m_axes[0] = glm::normalize(m_axes[0]);
	// y axis (0,1,0)
	m_axes[1] = Math::rotateVector(glm::vec3(0.0f, 1.0f, 0.0f), m_rotation);
	m_axes[1] = glm::normalize(m_axes[1]);
	// z axis (0,0,1)
	m_axes[2] = Math::rotateVector(glm::vec3(0.0f, 0.0f, 1.0f), m_rotation);
	m_axes[2] = glm::normalize(m_axes[2]);

	// Scaling the box collider / scaling the extends
	glm::vec3& scale = m_pRigidBody->GetGameObject().GetScale();
	m_extent *= scale;

}

void BoxCollider::AdjustCollider()
{
	std::vector<glm::vec3>& vertices = m_pRigidBody->GetGameObject().GetVertices();
	
	// Calculating center / offset from GameObject origin
	glm::vec3 minBounds = vertices[0];
	glm::vec3 maxBounds = vertices[0];
	for (size_t i = 1; i < vertices.size(); i++)
	{
		glm::vec3 vertex = vertices[i];
		if (minBounds.x > vertex.x || minBounds.y > vertex.y || minBounds.z > vertex.z)
			minBounds = vertex;

		if (maxBounds.x < vertex.x || maxBounds.y < vertex.y || maxBounds.z < vertex.z)
			maxBounds = vertex;
	}
	m_offset.x = minBounds.x + ((maxBounds.x - minBounds.x) / 2);
	m_offset.y = minBounds.y + ((maxBounds.y - minBounds.y) / 2);
	m_offset.z = minBounds.z + ((maxBounds.z - minBounds.z) / 2);

	// Calculating extents of the box along the axes
	m_extent.x = maxBounds.x - m_offset.x;
	m_extent.y = maxBounds.y - m_offset.y;
	m_extent.z = maxBounds.z - m_offset.z;

	// Apply transformation
	Update();
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

