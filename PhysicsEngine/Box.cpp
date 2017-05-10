////////////////////////////////
// Name: Box.cpp		      //
// Author: Alexander Kocourek //
// Date: 27/03/2017			  //
////////////////////////////////

#include "Box.h"

#include "RigidBody.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "MeshCollider.h"

Box::Box() : GameObject()
{	
	// Set the vertices
	setVertices();
	
	// Adjust Colliders to the mesh
	m_pRigidbody->GetSphereCollider().AdjustCollider(); //SetRadius(sqrt(3) / 2); // d = sqrt(3) * a; r = d / 2 => sqrt(3) / 2; a = 1
	m_pRigidbody->GetBoxCollider().AdjustCollider();
	m_pRigidbody->GetMeshCollider().AdjustCollider(m_vertices);

	// Adjust rigid body settings
	m_pRigidbody->SetCentreOfMass(glm::vec3(0.0f, 0.5f, 0.0f));
	glm::mat3 inertiaTensor;
	glm::vec3& scale = GetScale();
	inertiaTensor[0][0] = 1 / 12 * GetMass() * (scale.y * scale.y + scale.z * scale.z);
	inertiaTensor[1][1] = 1 / 12 * GetMass() * (scale.x * scale.x + scale.z * scale.z);
	inertiaTensor[2][2] = 1 / 12 * GetMass() * (scale.x * scale.x + scale.y * scale.y);
	m_pRigidbody->SetInertiaTensor(inertiaTensor);
}

Box::Box(const glm::vec3& position) : GameObject(position)
{
	setVertices();
	// Adjust Colliders to the mesh
	m_pRigidbody->GetSphereCollider().AdjustCollider(); //SetRadius(sqrt(3) / 2); // d = sqrt(3) * a; r = d / 2 => sqrt(3) / 2; a = 1
	m_pRigidbody->GetBoxCollider().AdjustCollider();
	m_pRigidbody->GetMeshCollider().AdjustCollider(m_vertices);

	// Adjust rigid body settings
	m_pRigidbody->SetCentreOfMass(glm::vec3(0.0f, 0.5f, 0.0f));
	glm::mat3 inertiaTensor;
	glm::vec3& scale = GetScale();
	/*inertiaTensor[0][0] = 1 / 12 * GetMass() * (scale.y * scale.y + scale.z * scale.z);
	inertiaTensor[1][1] = 1 / 12 * GetMass() * (scale.x * scale.x + scale.z * scale.z);
	inertiaTensor[2][2] = 1 / 12 * GetMass() * (scale.x * scale.x + scale.y * scale.y);*/
	m_pRigidbody->SetInertiaTensor(inertiaTensor);
}

Box::~Box(){}

void Box::Shutdown()
{
	shutdown();
}

void Box::Draw()
{
	prepareDraw();

	glBegin(GL_QUADS);

	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		glVertex3f(m_vertices[i].x, m_vertices[i].y, m_vertices[i].z);
	}

	glEnd();

	glPopMatrix();
}

void Box::setVertices()
{
	 //frontface (normal pointing +z)
	m_vertices.push_back(glm::vec3(-0.5f, 1.0f, 0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 1.0f, 0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 0.0f, 0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 0.0f, 0.5f));

	// rightface (normal pointing +x)
	m_vertices.push_back(glm::vec3(0.5f, 1.0f, 0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 1.0f, -0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 0.0f, -0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 0.0f, 0.5f));

	// leftface (normal pointing -x)
	m_vertices.push_back(glm::vec3(-0.5f, 1.0f, 0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 1.0f, -0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 0.0f, -0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 0.0f, 0.5f));

	//backface (normal pointing -z)
	m_vertices.push_back(glm::vec3(0.5f, 1.0f, -0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 1.0f, -0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 0.0f, -0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 0.0f, -0.5f));

	// topface (normal pointing +y)
	m_vertices.push_back(glm::vec3(-0.5f, 1.0f, -0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 1.0f, -0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 1.0f, 0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 1.0f, 0.5f));

	// bottomface (normal pointing -y)
	m_vertices.push_back(glm::vec3(0.5f, 0.0f, 0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 0.0f, 0.5f));
	m_vertices.push_back(glm::vec3(-0.5f, 0.0f, -0.5f));
	m_vertices.push_back(glm::vec3(0.5f, 0.0f, -0.5f));


	/*m_vertices.push_back(glm::vec3(0.000000f, 2.933333f, -0.000000f));
	m_vertices.push_back(glm::vec3(0.000000f, -2.933333f, -0.000000f));
	m_vertices.push_back(glm::vec3(0.382877f, - 2.908238f, - 0.000000f));
		m_vertices.push_back(glm::vec3(0.369831f, - 2.908238f, 0.099096));
		m_vertices.push_back(glm::vec3(0.331581f, - 2.908238f, 0.191438));
		m_vertices.push_back(glm::vec3(0.270735f, - 2.908238f, 0.270735));
		m_vertices.push_back(glm::vec3(0.191438f, - 2.908238f, 0.331581));
		m_vertices.push_back(glm::vec3(0.099096f, - 2.908238f, 0.369831));
		m_vertices.push_back(glm::vec3(0.000000f, - 2.908238f, 0.382877));
		m_vertices.push_back(glm::vec3(- 0.099096f, - 2.908238f, 0.369831));
		m_vertices.push_back(glm::vec3(- 0.191438f, - 2.908238f, 0.331581));
		m_vertices.push_back(glm::vec3(- 0.270735f, - 2.908238f, 0.270735));
		m_vertices.push_back(glm::vec3(- 0.331581f, - 2.908238f, 0.191438));
		m_vertices.push_back(glm::vec3(- 0.369831f, - 2.908238f, 0.099096));
		m_vertices.push_back(glm::vec3(- 0.382877f, - 2.908238f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 0.369831f, - 2.908238f, - 0.099096f));
		m_vertices.push_back(glm::vec3(- 0.331581f, - 2.908238f, - 0.191438f));
		m_vertices.push_back(glm::vec3(- 0.270735f, - 2.908238f, - 0.270735f));
		m_vertices.push_back(glm::vec3(- 0.191438f, - 2.908238f, - 0.331581f));
		m_vertices.push_back(glm::vec3(- 0.099096f, - 2.908238f, - 0.369831f));
		m_vertices.push_back(glm::vec3(0.000000f, - 2.908238f, - 0.382877f));
		m_vertices.push_back(glm::vec3(0.099096f, - 2.908238f, - 0.369831f));
		m_vertices.push_back(glm::vec3(0.191438f, - 2.908238f, - 0.331581f));
		m_vertices.push_back(glm::vec3(0.270735f, - 2.908238f, - 0.270735f));
		m_vertices.push_back(glm::vec3(0.331581f, - 2.908238f, - 0.191438f));
		m_vertices.push_back(glm::vec3(0.369831f, - 2.908238f, - 0.099096f));
		m_vertices.push_back(glm::vec3(0.759203f, - 2.833382f, - 0.000000f));
		m_vertices.push_back(glm::vec3(0.733333f, - 2.833382f, 0.196496f));
		m_vertices.push_back(glm::vec3(0.657489f, - 2.833382f, 0.379601f));
		m_vertices.push_back(glm::vec3(0.536837f, - 2.833382f, 0.536837f));
		m_vertices.push_back(glm::vec3(0.379601f, - 2.833382f, 0.657489f));
		m_vertices.push_back(glm::vec3(0.196496f, - 2.833382f, 0.733333f));
		m_vertices.push_back(glm::vec3(0.000000f, - 2.833382f, 0.759202f));
		m_vertices.push_back(glm::vec3(- 0.196496f, - 2.833382f, 0.733333f));
		m_vertices.push_back(glm::vec3(- 0.379601f, - 2.833382f, 0.657489f));
		m_vertices.push_back(glm::vec3(- 0.536837f, - 2.833382f, 0.536837f));
		m_vertices.push_back(glm::vec3(- 0.657489f, - 2.833382f, 0.379601f));
		m_vertices.push_back(glm::vec3(- 0.733333f, - 2.833382f, 0.196496f));
		m_vertices.push_back(glm::vec3(- 0.759203f, - 2.833382f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 0.733333f, - 2.833382f, - 0.196496f));
		m_vertices.push_back(glm::vec3(- 0.657489f, - 2.833382f, - 0.379601f));
		m_vertices.push_back(glm::vec3(- 0.536837f, - 2.833382f, - 0.536837f));
		m_vertices.push_back(glm::vec3(- 0.379601f, - 2.833382f, - 0.657489f));
		m_vertices.push_back(glm::vec3(- 0.196496f, - 2.833382f, - 0.733333f));
		m_vertices.push_back(glm::vec3(0.000000f, - 2.833382f, - 0.759203f));
		m_vertices.push_back(glm::vec3(0.196496f, - 2.833382f, - 0.733333f));
		m_vertices.push_back(glm::vec3(0.379601f, - 2.833382f, - 0.657489f));
		m_vertices.push_back(glm::vec3(0.536837f, - 2.833382f, - 0.536837f));
		m_vertices.push_back(glm::vec3(0.657489f, - 2.833382f, - 0.379601f));
		m_vertices.push_back(glm::vec3(0.733333f, - 2.833382f, - 0.196496f));
		m_vertices.push_back(glm::vec3(1.122538f, - 2.710047f, - 0.000000f));
		m_vertices.push_back(glm::vec3(1.084289f, - 2.710047f, 0.290534f));
		m_vertices.push_back(glm::vec3(0.972147f, - 2.710047f, 0.561269f));
		m_vertices.push_back(glm::vec3(0.793754f, - 2.710047f, 0.793754f));
		m_vertices.push_back(glm::vec3(0.561269f, - 2.710047f, 0.972147f));
		m_vertices.push_back(glm::vec3(0.290534f, - 2.710047f, 1.084288f));
		m_vertices.push_back(glm::vec3(0.000000f, - 2.710047f, 1.122538f));
		m_vertices.push_back(glm::vec3(- 0.290534f, - 2.710047f, 1.084288f));
		m_vertices.push_back(glm::vec3(- 0.561269f, - 2.710047f, 0.972147f));
		m_vertices.push_back(glm::vec3(- 0.793754f, - 2.710047f, 0.793754f));
		m_vertices.push_back(glm::vec3(- 0.972147f, - 2.710047f, 0.561269f));
		m_vertices.push_back(glm::vec3(- 1.084289f, - 2.710047f, 0.290534f));
		m_vertices.push_back(glm::vec3(- 1.122538f, - 2.710047f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 1.084289f, - 2.710047f, - 0.290534f));
		m_vertices.push_back(glm::vec3(- 0.972147f, - 2.710047f, - 0.561269f));
		m_vertices.push_back(glm::vec3(- 0.793754f, - 2.710047f, - 0.793754f));
		m_vertices.push_back(glm::vec3(- 0.561269f, - 2.710047f, - 0.972147f));
		m_vertices.push_back(glm::vec3(- 0.290534f, - 2.710047f, - 1.084289f));
		m_vertices.push_back(glm::vec3(0.000000f, - 2.710047f, - 1.122538f));
		m_vertices.push_back(glm::vec3(0.290534f, - 2.710047f, - 1.084289f));
		m_vertices.push_back(glm::vec3(0.561269f, - 2.710047f, - 0.972147f));
		m_vertices.push_back(glm::vec3(0.793754f, - 2.710047f, - 0.793754f));
		m_vertices.push_back(glm::vec3(0.972147f, - 2.710047f, - 0.561269f));
		m_vertices.push_back(glm::vec3(1.084289f, - 2.710047f, - 0.290534f));
		m_vertices.push_back(glm::vec3(1.466667f, - 2.540341f, - 0.000000f));
		m_vertices.push_back(glm::vec3(1.416691f, - 2.540341f, 0.379601f));
		m_vertices.push_back(glm::vec3(1.270171f, - 2.540341f, 0.733333f));
		m_vertices.push_back(glm::vec3(1.037090f, - 2.540341f, 1.037090f));
		m_vertices.push_back(glm::vec3(0.733333f, - 2.540341f, 1.270171f));
		m_vertices.push_back(glm::vec3(0.379601f, - 2.540341f, 1.416691f));
		m_vertices.push_back(glm::vec3(0.000000f, - 2.540341f, 1.466667f));
		m_vertices.push_back(glm::vec3(- 0.379601f, - 2.540341f, 1.416691f));
		m_vertices.push_back(glm::vec3(- 0.733333f, - 2.540341f, 1.270171f));
		m_vertices.push_back(glm::vec3(- 1.037090f, - 2.540341f, 1.037090f));
		m_vertices.push_back(glm::vec3(- 1.270171f, - 2.540341f, 0.733333f));
		m_vertices.push_back(glm::vec3(- 1.416691f, - 2.540341f, 0.379601f));
		m_vertices.push_back(glm::vec3(- 1.466667f, - 2.540341f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 1.416691f, - 2.540341f, - 0.379601f));
		m_vertices.push_back(glm::vec3(- 1.270171f, - 2.540341f, - 0.733333f));
		m_vertices.push_back(glm::vec3(- 1.037090f, - 2.540341f, - 1.037090f));
		m_vertices.push_back(glm::vec3(- 0.733333f, - 2.540341f, - 1.270171f));
		m_vertices.push_back(glm::vec3(- 0.379601f, - 2.540341f, - 1.416691f));
		m_vertices.push_back(glm::vec3(0.000000f, - 2.540341f, - 1.466667f));
		m_vertices.push_back(glm::vec3(0.379601f, - 2.540341f, - 1.416691f));
		m_vertices.push_back(glm::vec3(0.733333f, - 2.540341f, - 1.270171f));
		m_vertices.push_back(glm::vec3(1.037090f, - 2.540341f, - 1.037090f));
		m_vertices.push_back(glm::vec3(1.270171f, - 2.540341f, - 0.733333f));
		m_vertices.push_back(glm::vec3(1.416691f, - 2.540341f, - 0.379601f));
		m_vertices.push_back(glm::vec3(1.785700f, - 2.327170f, - 0.000000f));
		m_vertices.push_back(glm::vec3(1.724854f, - 2.327170f, 0.462173f));
		m_vertices.push_back(glm::vec3(1.546462f, - 2.327170f, 0.892850f));
		m_vertices.push_back(glm::vec3(1.262681f, - 2.327170f, 1.262681f));
		m_vertices.push_back(glm::vec3(0.892850f, - 2.327170f, 1.546462f));
		m_vertices.push_back(glm::vec3(0.462173f, - 2.327170f, 1.724854f));
		m_vertices.push_back(glm::vec3(0.000000f, - 2.327170f, 1.785700f));
		m_vertices.push_back(glm::vec3(- 0.462173f, - 2.327170f, 1.724854f));
		m_vertices.push_back(glm::vec3(- 0.892850f, - 2.327170f, 1.546462f));
		m_vertices.push_back(glm::vec3(- 1.262681f, - 2.327170f, 1.262681f));
		m_vertices.push_back(glm::vec3(- 1.546462f, - 2.327170f, 0.892850f));
		m_vertices.push_back(glm::vec3(- 1.724854f, - 2.327170f, 0.462173f));
		m_vertices.push_back(glm::vec3(- 1.785700f, - 2.327170f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 1.724854f, - 2.327170f, - 0.462173f));
		m_vertices.push_back(glm::vec3(- 1.546462f, - 2.327170f, - 0.892850f));
		m_vertices.push_back(glm::vec3(- 1.262681f, - 2.327170f, - 1.262681f));
		m_vertices.push_back(glm::vec3(- 0.892850f, - 2.327170f, - 1.546462f));
		m_vertices.push_back(glm::vec3(- 0.462173f, - 2.327170f, - 1.724854f));
		m_vertices.push_back(glm::vec3(0.000000f, - 2.327170f, - 1.785700f));
		m_vertices.push_back(glm::vec3(0.462173f, - 2.327170f, - 1.724854f));
		m_vertices.push_back(glm::vec3(0.892850f, - 2.327170f, - 1.546462f));
		m_vertices.push_back(glm::vec3(1.262681f, - 2.327170f, - 1.262681f));
		m_vertices.push_back(glm::vec3(1.546462f, - 2.327170f, - 0.892850f));
		m_vertices.push_back(glm::vec3(1.724854f, - 2.327170f, - 0.462173f));
		m_vertices.push_back(glm::vec3(2.074180f, - 2.074180f, - 0.000000f));
		m_vertices.push_back(glm::vec3(2.003504f, - 2.074180f, 0.536837f));
		m_vertices.push_back(glm::vec3(1.796292f, - 2.074180f, 1.037090f));
		m_vertices.push_back(glm::vec3(1.466667f, - 2.074180f, 1.466667f));
		m_vertices.push_back(glm::vec3(1.037090f, - 2.074180f, 1.796292f));
		m_vertices.push_back(glm::vec3(0.536837f, - 2.074180f, 2.003504f));
		m_vertices.push_back(glm::vec3(0.000000f, - 2.074180f, 2.074180f));
		m_vertices.push_back(glm::vec3(- 0.536837f, - 2.074180f, 2.003504f));
		m_vertices.push_back(glm::vec3(- 1.037090f, - 2.074180f, 1.796292f));
		m_vertices.push_back(glm::vec3(- 1.466667f, - 2.074180f, 1.466667f));
		m_vertices.push_back(glm::vec3(- 1.796292f, - 2.074180f, 1.037090f));
		m_vertices.push_back(glm::vec3(- 2.003504f, - 2.074180f, 0.536837f));
		m_vertices.push_back(glm::vec3(- 2.074180f, - 2.074180f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 2.003504f, - 2.074180f, - 0.536837f));
		m_vertices.push_back(glm::vec3(- 1.796292f, - 2.074180f, - 1.037090f));
		m_vertices.push_back(glm::vec3(- 1.466667f, - 2.074180f, - 1.466667f));
		m_vertices.push_back(glm::vec3(- 1.037090f, - 2.074180f, - 1.796293f));
		m_vertices.push_back(glm::vec3(- 0.536837f, - 2.074180f, - 2.003504f));
		m_vertices.push_back(glm::vec3(0.000000f, - 2.074180f, - 2.074180f));
		m_vertices.push_back(glm::vec3(0.536837f, - 2.074180f, - 2.003504f));
		m_vertices.push_back(glm::vec3(1.037090f, - 2.074180f, - 1.796293f));
		m_vertices.push_back(glm::vec3(1.466667f, - 2.074180f, - 1.466667f));
		m_vertices.push_back(glm::vec3(1.796292f, - 2.074180f, - 1.037090f));
		m_vertices.push_back(glm::vec3(2.003504f, - 2.074180f, - 0.536837f));
		m_vertices.push_back(glm::vec3(2.327170f, - 1.785700f, - 0.000000f));
		m_vertices.push_back(glm::vec3(2.247873f, - 1.785700f, 0.602316f));
		m_vertices.push_back(glm::vec3(2.015388f, - 1.785700f, 1.163585f));
		m_vertices.push_back(glm::vec3(1.645558f, - 1.785700f, 1.645557f));
		m_vertices.push_back(glm::vec3(1.163585f, - 1.785700f, 2.015388f));
		m_vertices.push_back(glm::vec3(0.602316f, - 1.785700f, 2.247873f));
		m_vertices.push_back(glm::vec3(0.000000f, - 1.785700f, 2.327170f));
		m_vertices.push_back(glm::vec3(- 0.602316f, - 1.785700f, 2.247873f));
		m_vertices.push_back(glm::vec3(- 1.163585f, - 1.785700f, 2.015388f));
		m_vertices.push_back(glm::vec3(- 1.645558f, - 1.785700f, 1.645557f));
		m_vertices.push_back(glm::vec3(- 2.015388f, - 1.785700f, 1.163585f));
		m_vertices.push_back(glm::vec3(- 2.247873f, - 1.785700f, 0.602316f));
		m_vertices.push_back(glm::vec3(- 2.327170f, - 1.785700f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 2.247873f, - 1.785700f, - 0.602316f));
		m_vertices.push_back(glm::vec3(- 2.015388f, - 1.785700f, - 1.163585f));
		m_vertices.push_back(glm::vec3(- 1.645558f, - 1.785700f, - 1.645558f));
		m_vertices.push_back(glm::vec3(- 1.163585f, - 1.785700f, - 2.015388f));
		m_vertices.push_back(glm::vec3(- 0.602316f, - 1.785700f, - 2.247874f));
		m_vertices.push_back(glm::vec3(0.000000f, - 1.785700f, - 2.327170f));
		m_vertices.push_back(glm::vec3(0.602316f, - 1.785700f, - 2.247874f));
		m_vertices.push_back(glm::vec3(1.163585f, - 1.785700f, - 2.015388f));
		m_vertices.push_back(glm::vec3(1.645558f, - 1.785700f, - 1.645558f));
		m_vertices.push_back(glm::vec3(2.015388f, - 1.785700f, - 1.163585f));
		m_vertices.push_back(glm::vec3(2.247873f, - 1.785700f, - 0.602316f));
		m_vertices.push_back(glm::vec3(2.540341f, - 1.466667f, - 0.000000f));
		m_vertices.push_back(glm::vec3(2.453781f, - 1.466667f, 0.657489f));
		m_vertices.push_back(glm::vec3(2.200000f, - 1.466667f, 1.270171f));
		m_vertices.push_back(glm::vec3(1.796293f, - 1.466667f, 1.796293f));
		m_vertices.push_back(glm::vec3(1.270171f, - 1.466667f, 2.200000f));
		m_vertices.push_back(glm::vec3(0.657489f, - 1.466667f, 2.453781f));
		m_vertices.push_back(glm::vec3(0.000000f, - 1.466667f, 2.540341f));
		m_vertices.push_back(glm::vec3(- 0.657489f, - 1.466667f, 2.453781f));
		m_vertices.push_back(glm::vec3(- 1.270171f, - 1.466667f, 2.200000f));
		m_vertices.push_back(glm::vec3(- 1.796293f, - 1.466667f, 1.796293f));
		m_vertices.push_back(glm::vec3(- 2.200000f, - 1.466667f, 1.270171f));
		m_vertices.push_back(glm::vec3(- 2.453781f, - 1.466667f, 0.657489f));
		m_vertices.push_back(glm::vec3(- 2.540341f, - 1.466667f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 2.453781f, - 1.466667f, - 0.657489f));
		m_vertices.push_back(glm::vec3(- 2.200000f, - 1.466667f, - 1.270171f));
		m_vertices.push_back(glm::vec3(- 1.796293f, - 1.466667f, - 1.796293f));
		m_vertices.push_back(glm::vec3(- 1.270171f, - 1.466667f, - 2.200000f));
		m_vertices.push_back(glm::vec3(- 0.657489f, - 1.466667f, - 2.453781f));
		m_vertices.push_back(glm::vec3(0.000000f, - 1.466667f, - 2.540341f));
		m_vertices.push_back(glm::vec3(0.657489f, - 1.466667f, - 2.453781f));
		m_vertices.push_back(glm::vec3(1.270171f, - 1.466667f, - 2.200000f));
		m_vertices.push_back(glm::vec3(1.796293f, - 1.466667f, - 1.796293f));
		m_vertices.push_back(glm::vec3(2.200000f, - 1.466667f, - 1.270171f));
		m_vertices.push_back(glm::vec3(2.453781f, - 1.466667f, - 0.657489f));
		m_vertices.push_back(glm::vec3(2.710047f, - 1.122538f, - 0.000000f));
		m_vertices.push_back(glm::vec3(2.617704f, - 1.122538f, 0.701412f));
		m_vertices.push_back(glm::vec3(2.346969f, - 1.122538f, 1.355023f));
		m_vertices.push_back(glm::vec3(1.916292f, - 1.122538f, 1.916292f));
		m_vertices.push_back(glm::vec3(1.355023f, - 1.122538f, 2.346969f));
		m_vertices.push_back(glm::vec3(0.701412f, - 1.122538f, 2.617704f));
		m_vertices.push_back(glm::vec3(0.000000f, - 1.122538f, 2.710047f));
		m_vertices.push_back(glm::vec3(- 0.701412f, - 1.122538f, 2.617704f));
		m_vertices.push_back(glm::vec3(- 1.355023f, - 1.122538f, 2.346969f));
		m_vertices.push_back(glm::vec3(- 1.916292f, - 1.122538f, 1.916292f));
		m_vertices.push_back(glm::vec3(- 2.346969f, - 1.122538f, 1.355023f));
		m_vertices.push_back(glm::vec3(- 2.617704f, - 1.122538f, 0.701412f));
		m_vertices.push_back(glm::vec3(- 2.710047f, - 1.122538f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 2.617704f, - 1.122538f, - 0.701412f));
		m_vertices.push_back(glm::vec3(- 2.346969f, - 1.122538f, - 1.355023f));
		m_vertices.push_back(glm::vec3(- 1.916292f, - 1.122538f, - 1.916292f));
		m_vertices.push_back(glm::vec3(- 1.355023f, - 1.122538f, - 2.346969f));
		m_vertices.push_back(glm::vec3(- 0.701412f, - 1.122538f, - 2.617704f));
		m_vertices.push_back(glm::vec3(0.000000f, - 1.122538f, - 2.710047f));
		m_vertices.push_back(glm::vec3(0.701412f, - 1.122538f, - 2.617704f));
		m_vertices.push_back(glm::vec3(1.355023f, - 1.122538f, - 2.346969f));
		m_vertices.push_back(glm::vec3(1.916292f, - 1.122538f, - 1.916292f));
		m_vertices.push_back(glm::vec3(2.346969f, - 1.122538f, - 1.355023f));
		m_vertices.push_back(glm::vec3(2.617704f, - 1.122538f, - 0.701412f));
		m_vertices.push_back(glm::vec3(2.833382f, - 0.759203f, - 0.000000f));
		m_vertices.push_back(glm::vec3(2.736837f, - 0.759203f, 0.733333f));
		m_vertices.push_back(glm::vec3(2.453781f, - 0.759203f, 1.416691f));
		m_vertices.push_back(glm::vec3(2.003504f, - 0.759203f, 2.003504f));
		m_vertices.push_back(glm::vec3(1.416691f, - 0.759203f, 2.453781f));
		m_vertices.push_back(glm::vec3(0.733333f, - 0.759203f, 2.736837f));
		m_vertices.push_back(glm::vec3(0.000000f, - 0.759203f, 2.833382f));
		m_vertices.push_back(glm::vec3(- 0.733333f, - 0.759203f, 2.736837f));
		m_vertices.push_back(glm::vec3(- 1.416691f, - 0.759203f, 2.453781f));
		m_vertices.push_back(glm::vec3(- 2.003504f, - 0.759203f, 2.003504f));
		m_vertices.push_back(glm::vec3(- 2.453781f, - 0.759203f, 1.416691f));
		m_vertices.push_back(glm::vec3(- 2.736837f, - 0.759203f, 0.733333f));
		m_vertices.push_back(glm::vec3(- 2.833382f, - 0.759203f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 2.736837f, - 0.759203f, - 0.733333f));
		m_vertices.push_back(glm::vec3(- 2.453781f, - 0.759203f, - 1.416691f));
		m_vertices.push_back(glm::vec3(- 2.003504f, - 0.759203f, - 2.003504f));
		m_vertices.push_back(glm::vec3(- 1.416691f, - 0.759203f, - 2.453781f));
		m_vertices.push_back(glm::vec3(- 0.733333f, - 0.759203f, - 2.736837f));
		m_vertices.push_back(glm::vec3(0.000000f, - 0.759203f, - 2.833383f));
		m_vertices.push_back(glm::vec3(0.733333f, - 0.759203f, - 2.736837f));
		m_vertices.push_back(glm::vec3(1.416691f, - 0.759203f, - 2.453781f));
		m_vertices.push_back(glm::vec3(2.003504f, - 0.759203f, - 2.003504f));
		m_vertices.push_back(glm::vec3(2.453781f, - 0.759203f, - 1.416691f));
		m_vertices.push_back(glm::vec3(2.736837f, - 0.759203f, - 0.733333f));
		m_vertices.push_back(glm::vec3(2.908238f, - 0.382877f, - 0.000000f));
		m_vertices.push_back(glm::vec3(2.809142f, - 0.382877f, 0.752707f));
		m_vertices.push_back(glm::vec3(2.518608f, - 0.382877f, 1.454119f));
		m_vertices.push_back(glm::vec3(2.056435f, - 0.382877f, 2.056435f));
		m_vertices.push_back(glm::vec3(1.454119f, - 0.382877f, 2.518608f));
		m_vertices.push_back(glm::vec3(0.752707f, - 0.382877f, 2.809142f));
		m_vertices.push_back(glm::vec3(0.000000f, - 0.382877f, 2.908238f));
		m_vertices.push_back(glm::vec3(- 0.752707f, - 0.382877f, 2.809142f));
		m_vertices.push_back(glm::vec3(- 1.454119f, - 0.382877f, 2.518608f));
		m_vertices.push_back(glm::vec3(- 2.056435f, - 0.382877f, 2.056435f));
		m_vertices.push_back(glm::vec3(- 2.518608f, - 0.382877f, 1.454119f));
		m_vertices.push_back(glm::vec3(- 2.809142f, - 0.382877f, 0.752707f));
		m_vertices.push_back(glm::vec3(- 2.908238f, - 0.382877f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 2.809142f, - 0.382877f, - 0.752707f));
		m_vertices.push_back(glm::vec3(- 2.518608f, - 0.382877f, - 1.454119f));
		m_vertices.push_back(glm::vec3(- 2.056435f, - 0.382877f, - 2.056435f));
		m_vertices.push_back(glm::vec3(- 1.454119f, - 0.382877f, - 2.518608f));
		m_vertices.push_back(glm::vec3(- 0.752707f, - 0.382877f, - 2.809142f));
		m_vertices.push_back(glm::vec3(0.000000f, - 0.382877f, - 2.908238f));
		m_vertices.push_back(glm::vec3(0.752707f, - 0.382877f, - 2.809142f));
		m_vertices.push_back(glm::vec3(1.454119f, - 0.382877f, - 2.518608f));
		m_vertices.push_back(glm::vec3(2.056435f, - 0.382877f, - 2.056435f));
		m_vertices.push_back(glm::vec3(2.518608f, - 0.382877f, - 1.454119f));
		m_vertices.push_back(glm::vec3(2.809142f, - 0.382877f, - 0.752707f));
		m_vertices.push_back(glm::vec3(2.933333f, 0.000000f, - 0.000000f));
		m_vertices.push_back(glm::vec3(2.833382f, 0.000000f, 0.759202f));
		m_vertices.push_back(glm::vec3(2.540341f, 0.000000f, 1.466667f));
		m_vertices.push_back(glm::vec3(2.074180f, 0.000000f, 2.074180f));
		m_vertices.push_back(glm::vec3(1.466667f, 0.000000f, 2.540341f));
		m_vertices.push_back(glm::vec3(0.759203f, 0.000000f, 2.833382f));
		m_vertices.push_back(glm::vec3(0.000000f, 0.000000f, 2.933333f));
		m_vertices.push_back(glm::vec3(- 0.759203f, 0.000000f, 2.833382f));
		m_vertices.push_back(glm::vec3(- 1.466667f, 0.000000f, 2.540341f));
		m_vertices.push_back(glm::vec3(- 2.074180f, 0.000000f, 2.074180f));
		m_vertices.push_back(glm::vec3(- 2.540341f, 0.000000f, 1.466667f));
		m_vertices.push_back(glm::vec3(- 2.833382f, 0.000000f, 0.759202f));
		m_vertices.push_back(glm::vec3(- 2.933333f, 0.000000f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 2.833382f, 0.000000f, - 0.759203f));
		m_vertices.push_back(glm::vec3(- 2.540341f, 0.000000f, - 1.466667f));
		m_vertices.push_back(glm::vec3(- 2.074180f, 0.000000f, - 2.074180f));
		m_vertices.push_back(glm::vec3(- 1.466667f, 0.000000f, - 2.540341f));
		m_vertices.push_back(glm::vec3(- 0.759203f, 0.000000f, - 2.833383f));
		m_vertices.push_back(glm::vec3(0.000000f, 0.000000f, - 2.933333f));
		m_vertices.push_back(glm::vec3(0.759203f, 0.000000f, - 2.833383f));
		m_vertices.push_back(glm::vec3(1.466667f, 0.000000f, - 2.540341f));
		m_vertices.push_back(glm::vec3(2.074180f, 0.000000f, - 2.074180f));
		m_vertices.push_back(glm::vec3(2.540341f, 0.000000f, - 1.466667f));
		m_vertices.push_back(glm::vec3(2.833382f, 0.000000f, - 0.759203f));
		m_vertices.push_back(glm::vec3(2.908238f, 0.382877f, - 0.000000f));
		m_vertices.push_back(glm::vec3(2.809143f, 0.382877f, 0.752707f));
		m_vertices.push_back(glm::vec3(2.518608f, 0.382877f, 1.454119f));
		m_vertices.push_back(glm::vec3(2.056435f, 0.382877f, 2.056435f));
		m_vertices.push_back(glm::vec3(1.454119f, 0.382877f, 2.518608f));
		m_vertices.push_back(glm::vec3(0.752707f, 0.382877f, 2.809143f));
		m_vertices.push_back(glm::vec3(0.000000f, 0.382877f, 2.908238f));
		m_vertices.push_back(glm::vec3(- 0.752707f, 0.382877f, 2.809143f));
		m_vertices.push_back(glm::vec3(- 1.454119f, 0.382877f, 2.518608f));
		m_vertices.push_back(glm::vec3(- 2.056435f, 0.382877f, 2.056435f));
		m_vertices.push_back(glm::vec3(- 2.518608f, 0.382877f, 1.454119f));
		m_vertices.push_back(glm::vec3(- 2.809143f, 0.382877f, 0.752707f));
		m_vertices.push_back(glm::vec3(- 2.908238f, 0.382877f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 2.809143f, 0.382877f, - 0.752708f));
		m_vertices.push_back(glm::vec3(- 2.518608f, 0.382877f, - 1.454119f));
		m_vertices.push_back(glm::vec3(- 2.056435f, 0.382877f, - 2.056435f));
		m_vertices.push_back(glm::vec3(- 1.454119f, 0.382877f, - 2.518608f));
		m_vertices.push_back(glm::vec3(- 0.752707f, 0.382877f, - 2.809143f));
		m_vertices.push_back(glm::vec3(0.000000f, 0.382877f, - 2.908238f));
		m_vertices.push_back(glm::vec3(0.752707f, 0.382877f, - 2.809143f));
		m_vertices.push_back(glm::vec3(1.454119f, 0.382877f, - 2.518608f));
		m_vertices.push_back(glm::vec3(2.056435f, 0.382877f, - 2.056435f));
		m_vertices.push_back(glm::vec3(2.518608f, 0.382877f, - 1.454119f));
		m_vertices.push_back(glm::vec3(2.809143f, 0.382877f, - 0.752708f));
		m_vertices.push_back(glm::vec3(2.833382f, 0.759203f, - 0.000000f));
		m_vertices.push_back(glm::vec3(2.736837f, 0.759203f, 0.733333f));
		m_vertices.push_back(glm::vec3(2.453781f, 0.759203f, 1.416691f));
		m_vertices.push_back(glm::vec3(2.003504f, 0.759203f, 2.003504f));
		m_vertices.push_back(glm::vec3(1.416691f, 0.759203f, 2.453781f));
		m_vertices.push_back(glm::vec3(0.733333f, 0.759203f, 2.736837f));
		m_vertices.push_back(glm::vec3(0.000000f, 0.759203f, 2.833382f));
		m_vertices.push_back(glm::vec3(- 0.733333f, 0.759203f, 2.736837f));
		m_vertices.push_back(glm::vec3(- 1.416691f, 0.759203f, 2.453781f));
		m_vertices.push_back(glm::vec3(- 2.003504f, 0.759203f, 2.003504f));
		m_vertices.push_back(glm::vec3(- 2.453781f, 0.759203f, 1.416691f));
		m_vertices.push_back(glm::vec3(- 2.736837f, 0.759203f, 0.733333f));
		m_vertices.push_back(glm::vec3(- 2.833382f, 0.759203f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 2.736837f, 0.759203f, - 0.733333f));
		m_vertices.push_back(glm::vec3(- 2.453781f, 0.759203f, - 1.416691f));
		m_vertices.push_back(glm::vec3(- 2.003504f, 0.759203f, - 2.003504f));
		m_vertices.push_back(glm::vec3(- 1.416691f, 0.759203f, - 2.453781f));
		m_vertices.push_back(glm::vec3(- 0.733333f, 0.759203f, - 2.736837f));
		m_vertices.push_back(glm::vec3(0.000000f, 0.759203f, - 2.833383f));
		m_vertices.push_back(glm::vec3(0.733333f, 0.759203f, - 2.736837f));
		m_vertices.push_back(glm::vec3(1.416691f, 0.759203f, - 2.453781f));
		m_vertices.push_back(glm::vec3(2.003504f, 0.759203f, - 2.003504f));
		m_vertices.push_back(glm::vec3(2.453781f, 0.759203f, - 1.416691f));
		m_vertices.push_back(glm::vec3(2.736837f, 0.759203f, - 0.733333f));
		m_vertices.push_back(glm::vec3(2.710047f, 1.122538f, - 0.000000f));
		m_vertices.push_back(glm::vec3(2.617704f, 1.122538f, 0.701412f));
		m_vertices.push_back(glm::vec3(2.346969f, 1.122538f, 1.355023f));
		m_vertices.push_back(glm::vec3(1.916292f, 1.122538f, 1.916292f));
		m_vertices.push_back(glm::vec3(1.355023f, 1.122538f, 2.346969f));
		m_vertices.push_back(glm::vec3(0.701412f, 1.122538f, 2.617704f));
		m_vertices.push_back(glm::vec3(0.000000f, 1.122538f, 2.710047f));
		m_vertices.push_back(glm::vec3(- 0.701412f, 1.122538f, 2.617704f));
		m_vertices.push_back(glm::vec3(- 1.355023f, 1.122538f, 2.346969f));
		m_vertices.push_back(glm::vec3(- 1.916292f, 1.122538f, 1.916292f));
		m_vertices.push_back(glm::vec3(- 2.346969f, 1.122538f, 1.355023f));
		m_vertices.push_back(glm::vec3(- 2.617704f, 1.122538f, 0.701412f));
		m_vertices.push_back(glm::vec3(- 2.710047f, 1.122538f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 2.617704f, 1.122538f, - 0.701412f));
		m_vertices.push_back(glm::vec3(- 2.346969f, 1.122538f, - 1.355024f));
		m_vertices.push_back(glm::vec3(- 1.916292f, 1.122538f, - 1.916292f));
		m_vertices.push_back(glm::vec3(- 1.355023f, 1.122538f, - 2.346969f));
		m_vertices.push_back(glm::vec3(- 0.701412f, 1.122538f, - 2.617704f));
		m_vertices.push_back(glm::vec3(0.000000f, 1.122538f, - 2.710047f));
		m_vertices.push_back(glm::vec3(0.701412f, 1.122538f, - 2.617704f));
		m_vertices.push_back(glm::vec3(1.355023f, 1.122538f, - 2.346969f));
		m_vertices.push_back(glm::vec3(1.916292f, 1.122538f, - 1.916292f));
		m_vertices.push_back(glm::vec3(2.346969f, 1.122538f, - 1.355024f));
		m_vertices.push_back(glm::vec3(2.617704f, 1.122538f, - 0.701412f));
		m_vertices.push_back(glm::vec3(2.540341f, 1.466667f, - 0.000000f));
		m_vertices.push_back(glm::vec3(2.453781f, 1.466667f, 0.657489f));
		m_vertices.push_back(glm::vec3(2.200000f, 1.466667f, 1.270171f));
		m_vertices.push_back(glm::vec3(1.796292f, 1.466667f, 1.796292f));
		m_vertices.push_back(glm::vec3(1.270171f, 1.466667f, 2.200000f));
		m_vertices.push_back(glm::vec3(0.657489f, 1.466667f, 2.453781f));
		m_vertices.push_back(glm::vec3(0.000000f, 1.466667f, 2.540341f));
		m_vertices.push_back(glm::vec3(- 0.657489f, 1.466667f, 2.453781f));
		m_vertices.push_back(glm::vec3(- 1.270171f, 1.466667f, 2.200000f));
		m_vertices.push_back(glm::vec3(- 1.796292f, 1.466667f, 1.796292f));
		m_vertices.push_back(glm::vec3(- 2.200000f, 1.466667f, 1.270171f));
		m_vertices.push_back(glm::vec3(- 2.453781f, 1.466667f, 0.657489f));
		m_vertices.push_back(glm::vec3(- 2.540341f, 1.466667f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 2.453781f, 1.466667f, - 0.657489f));
		m_vertices.push_back(glm::vec3(- 2.200000f, 1.466667f, - 1.270171f));
		m_vertices.push_back(glm::vec3(- 1.796292f, 1.466667f, - 1.796293f));
		m_vertices.push_back(glm::vec3(- 1.270171f, 1.466667f, - 2.200000f));
		m_vertices.push_back(glm::vec3(- 0.657489f, 1.466667f, - 2.453781f));
		m_vertices.push_back(glm::vec3(0.000000f, 1.466667f, - 2.540341f));
		m_vertices.push_back(glm::vec3(0.657489f, 1.466667f, - 2.453781f));
		m_vertices.push_back(glm::vec3(1.270171f, 1.466667f, - 2.200000f));
		m_vertices.push_back(glm::vec3(1.796292f, 1.466667f, - 1.796293f));
		m_vertices.push_back(glm::vec3(2.200000f, 1.466667f, - 1.270171f));
		m_vertices.push_back(glm::vec3(2.453781f, 1.466667f, - 0.657489f));
		m_vertices.push_back(glm::vec3(2.327170f, 1.785700f, - 0.000000f));
		m_vertices.push_back(glm::vec3(2.247873f, 1.785700f, 0.602316f));
		m_vertices.push_back(glm::vec3(2.015388f, 1.785700f, 1.163585f));
		m_vertices.push_back(glm::vec3(1.645558f, 1.785700f, 1.645557f));
		m_vertices.push_back(glm::vec3(1.163585f, 1.785700f, 2.015388f));
		m_vertices.push_back(glm::vec3(0.602316f, 1.785700f, 2.247873f));
		m_vertices.push_back(glm::vec3(0.000000f, 1.785700f, 2.327170f));
		m_vertices.push_back(glm::vec3(- 0.602316f, 1.785700f, 2.247873f));
		m_vertices.push_back(glm::vec3(- 1.163585f, 1.785700f, 2.015388f));
		m_vertices.push_back(glm::vec3(- 1.645558f, 1.785700f, 1.645557f));
		m_vertices.push_back(glm::vec3(- 2.015388f, 1.785700f, 1.163585f));
		m_vertices.push_back(glm::vec3(- 2.247873f, 1.785700f, 0.602316f));
		m_vertices.push_back(glm::vec3(- 2.327170f, 1.785700f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 2.247873f, 1.785700f, - 0.602316f));
		m_vertices.push_back(glm::vec3(- 2.015388f, 1.785700f, - 1.163585f));
		m_vertices.push_back(glm::vec3(- 1.645558f, 1.785700f, - 1.645558f));
		m_vertices.push_back(glm::vec3(- 1.163585f, 1.785700f, - 2.015388f));
		m_vertices.push_back(glm::vec3(- 0.602316f, 1.785700f, - 2.247874f));
		m_vertices.push_back(glm::vec3(0.000000f, 1.785700f, - 2.327170f));
		m_vertices.push_back(glm::vec3(0.602316f, 1.785700f, - 2.247874f));
		m_vertices.push_back(glm::vec3(1.163585f, 1.785700f, - 2.015388f));
		m_vertices.push_back(glm::vec3(1.645558f, 1.785700f, - 1.645558f));
		m_vertices.push_back(glm::vec3(2.015388f, 1.785700f, - 1.163585f));
		m_vertices.push_back(glm::vec3(2.247873f, 1.785700f, - 0.602316f));
		m_vertices.push_back(glm::vec3(2.074180f, 2.074180f, - 0.000000f));
		m_vertices.push_back(glm::vec3(2.003504f, 2.074180f, 0.536837f));
		m_vertices.push_back(glm::vec3(1.796292f, 2.074180f, 1.037090f));
		m_vertices.push_back(glm::vec3(1.466667f, 2.074180f, 1.466667f));
		m_vertices.push_back(glm::vec3(1.037090f, 2.074180f, 1.796292f));
		m_vertices.push_back(glm::vec3(0.536837f, 2.074180f, 2.003504f));
		m_vertices.push_back(glm::vec3(0.000000f, 2.074180f, 2.074180f));
		m_vertices.push_back(glm::vec3(- 0.536837f, 2.074180f, 2.003504f));
		m_vertices.push_back(glm::vec3(- 1.037090f, 2.074180f, 1.796292f));
		m_vertices.push_back(glm::vec3(- 1.466667f, 2.074180f, 1.466667f));
		m_vertices.push_back(glm::vec3(- 1.796292f, 2.074180f, 1.037090f));
		m_vertices.push_back(glm::vec3(- 2.003504f, 2.074180f, 0.536837f));
		m_vertices.push_back(glm::vec3(- 2.074180f, 2.074180f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 2.003504f, 2.074180f, - 0.536837f));
		m_vertices.push_back(glm::vec3(- 1.796292f, 2.074180f, - 1.037090f));
		m_vertices.push_back(glm::vec3(- 1.466667f, 2.074180f, - 1.466667f));
		m_vertices.push_back(glm::vec3(- 1.037090f, 2.074180f, - 1.796293f));
		m_vertices.push_back(glm::vec3(- 0.536837f, 2.074180f, - 2.003504f));
		m_vertices.push_back(glm::vec3(0.000000f, 2.074180f, - 2.074180f));
		m_vertices.push_back(glm::vec3(0.536837f, 2.074180f, - 2.003504f));
		m_vertices.push_back(glm::vec3(1.037090f, 2.074180f, - 1.796293f));
		m_vertices.push_back(glm::vec3(1.466667f, 2.074180f, - 1.466667f));
		m_vertices.push_back(glm::vec3(1.796292f, 2.074180f, - 1.037090f));
		m_vertices.push_back(glm::vec3(2.003504f, 2.074180f, - 0.536837f));
		m_vertices.push_back(glm::vec3(1.785700f, 2.327170f, - 0.000000f));
		m_vertices.push_back(glm::vec3(1.724854f, 2.327170f, 0.462173f));
		m_vertices.push_back(glm::vec3(1.546462f, 2.327170f, 0.892850f));
		m_vertices.push_back(glm::vec3(1.262681f, 2.327170f, 1.262681f));
		m_vertices.push_back(glm::vec3(0.892850f, 2.327170f, 1.546462f));
		m_vertices.push_back(glm::vec3(0.462173f, 2.327170f, 1.724854f));
		m_vertices.push_back(glm::vec3(0.000000f, 2.327170f, 1.785700f));
		m_vertices.push_back(glm::vec3(- 0.462173f, 2.327170f, 1.724854f));
		m_vertices.push_back(glm::vec3(- 0.892850f, 2.327170f, 1.546462f));
		m_vertices.push_back(glm::vec3(- 1.262681f, 2.327170f, 1.262681f));
		m_vertices.push_back(glm::vec3(- 1.546462f, 2.327170f, 0.892850f));
		m_vertices.push_back(glm::vec3(- 1.724854f, 2.327170f, 0.462173f));
		m_vertices.push_back(glm::vec3(- 1.785700f, 2.327170f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 1.724854f, 2.327170f, - 0.462173f));
		m_vertices.push_back(glm::vec3(- 1.546462f, 2.327170f, - 0.892850f));
		m_vertices.push_back(glm::vec3(- 1.262681f, 2.327170f, - 1.262681f));
		m_vertices.push_back(glm::vec3(- 0.892850f, 2.327170f, - 1.546462f));
		m_vertices.push_back(glm::vec3(- 0.462173f, 2.327170f, - 1.724854f));
		m_vertices.push_back(glm::vec3(0.000000f, 2.327170f, - 1.785700f));
		m_vertices.push_back(glm::vec3(0.462173f, 2.327170f, - 1.724854f));
		m_vertices.push_back(glm::vec3(0.892850f, 2.327170f, - 1.546462f));
		m_vertices.push_back(glm::vec3(1.262681f, 2.327170f, - 1.262681f));
		m_vertices.push_back(glm::vec3(1.546462f, 2.327170f, - 0.892850f));
		m_vertices.push_back(glm::vec3(1.724854f, 2.327170f, - 0.462173f));
		m_vertices.push_back(glm::vec3(1.466667f, 2.540341f, - 0.000000f));
		m_vertices.push_back(glm::vec3(1.416691f, 2.540341f, 0.379601f));
		m_vertices.push_back(glm::vec3(1.270171f, 2.540341f, 0.733333f));
		m_vertices.push_back(glm::vec3(1.037090f, 2.540341f, 1.037090f));
		m_vertices.push_back(glm::vec3(0.733333f, 2.540341f, 1.270171f));
		m_vertices.push_back(glm::vec3(0.379601f, 2.540341f, 1.416691f));
		m_vertices.push_back(glm::vec3(0.000000f, 2.540341f, 1.466667f));
		m_vertices.push_back(glm::vec3(- 0.379601f, 2.540341f, 1.416691f));
		m_vertices.push_back(glm::vec3(- 0.733333f, 2.540341f, 1.270171f));
		m_vertices.push_back(glm::vec3(- 1.037090f, 2.540341f, 1.037090f));
		m_vertices.push_back(glm::vec3(- 1.270171f, 2.540341f, 0.733333f));
		m_vertices.push_back(glm::vec3(- 1.416691f, 2.540341f, 0.379601f));
		m_vertices.push_back(glm::vec3(- 1.466667f, 2.540341f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 1.416691f, 2.540341f, - 0.379601f));
		m_vertices.push_back(glm::vec3(- 1.270171f, 2.540341f, - 0.733333f));
		m_vertices.push_back(glm::vec3(- 1.037090f, 2.540341f, - 1.037090f));
		m_vertices.push_back(glm::vec3(- 0.733333f, 2.540341f, - 1.270171f));
		m_vertices.push_back(glm::vec3(- 0.379601f, 2.540341f, - 1.416691f));
		m_vertices.push_back(glm::vec3(0.000000f, 2.540341f, - 1.466667f));
		m_vertices.push_back(glm::vec3(0.379601f, 2.540341f, - 1.416691f));
		m_vertices.push_back(glm::vec3(0.733333f, 2.540341f, - 1.270171f));
		m_vertices.push_back(glm::vec3(1.037090f, 2.540341f, - 1.037090f));
		m_vertices.push_back(glm::vec3(1.270171f, 2.540341f, - 0.733333f));
		m_vertices.push_back(glm::vec3(1.416691f, 2.540341f, - 0.379601f));
		m_vertices.push_back(glm::vec3(1.122538f, 2.710047f, - 0.000000f));
		m_vertices.push_back(glm::vec3(1.084289f, 2.710047f, 0.290534f));
		m_vertices.push_back(glm::vec3(0.972147f, 2.710047f, 0.561269f));
		m_vertices.push_back(glm::vec3(0.793754f, 2.710047f, 0.793754f));
		m_vertices.push_back(glm::vec3(0.561269f, 2.710047f, 0.972147f));
		m_vertices.push_back(glm::vec3(0.290534f, 2.710047f, 1.084289f));
		m_vertices.push_back(glm::vec3(0.000000f, 2.710047f, 1.122538f));
		m_vertices.push_back(glm::vec3(- 0.290534f, 2.710047f, 1.084289f));
		m_vertices.push_back(glm::vec3(- 0.561269f, 2.710047f, 0.972147f));
		m_vertices.push_back(glm::vec3(- 0.793754f, 2.710047f, 0.793754f));
		m_vertices.push_back(glm::vec3(- 0.972147f, 2.710047f, 0.561269f));
		m_vertices.push_back(glm::vec3(- 1.084289f, 2.710047f, 0.290534f));
		m_vertices.push_back(glm::vec3(- 1.122538f, 2.710047f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 1.084289f, 2.710047f, - 0.290534f));
		m_vertices.push_back(glm::vec3(- 0.972147f, 2.710047f, - 0.561269f));
		m_vertices.push_back(glm::vec3(- 0.793754f, 2.710047f, - 0.793754f));
		m_vertices.push_back(glm::vec3(- 0.561269f, 2.710047f, - 0.972147f));
		m_vertices.push_back(glm::vec3(- 0.290534f, 2.710047f, - 1.084289f));
		m_vertices.push_back(glm::vec3(0.000000f, 2.710047f, - 1.122538f));
		m_vertices.push_back(glm::vec3(0.290534f, 2.710047f, - 1.084289f));
		m_vertices.push_back(glm::vec3(0.561269f, 2.710047f, - 0.972147f));
		m_vertices.push_back(glm::vec3(0.793754f, 2.710047f, - 0.793754f));
		m_vertices.push_back(glm::vec3(0.972147f, 2.710047f, - 0.561269f));
		m_vertices.push_back(glm::vec3(1.084289f, 2.710047f, - 0.290534f));
		m_vertices.push_back(glm::vec3(0.759203f, 2.833382f, - 0.000000f));
		m_vertices.push_back(glm::vec3(0.733334f, 2.833382f, 0.196496f));
		m_vertices.push_back(glm::vec3(0.657489f, 2.833382f, 0.379601f));
		m_vertices.push_back(glm::vec3(0.536837f, 2.833382f, 0.536837f));
		m_vertices.push_back(glm::vec3(0.379601f, 2.833382f, 0.657489f));
		m_vertices.push_back(glm::vec3(0.196496f, 2.833382f, 0.733334f));
		m_vertices.push_back(glm::vec3(0.000000f, 2.833382f, 0.759203f));
		m_vertices.push_back(glm::vec3(- 0.196496f, 2.833382f, 0.733334f));
		m_vertices.push_back(glm::vec3(- 0.379601f, 2.833382f, 0.657489f));
		m_vertices.push_back(glm::vec3(- 0.536837f, 2.833382f, 0.536837f));
		m_vertices.push_back(glm::vec3(- 0.657489f, 2.833382f, 0.379601f));
		m_vertices.push_back(glm::vec3(- 0.733334f, 2.833382f, 0.196496f));
		m_vertices.push_back(glm::vec3(- 0.759203f, 2.833382f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 0.733334f, 2.833382f, - 0.196496f));
		m_vertices.push_back(glm::vec3(- 0.657489f, 2.833382f, - 0.379601f));
		m_vertices.push_back(glm::vec3(- 0.536837f, 2.833382f, - 0.536838f));
		m_vertices.push_back(glm::vec3(- 0.379601f, 2.833382f, - 0.657489f));
		m_vertices.push_back(glm::vec3(- 0.196496f, 2.833382f, - 0.733334f));
		m_vertices.push_back(glm::vec3(0.000000f, 2.833382f, - 0.759203));
		m_vertices.push_back(glm::vec3(0.196496f, 2.833382f, - 0.733334));
		m_vertices.push_back(glm::vec3(0.379601f, 2.833382f, - 0.657489f));
		m_vertices.push_back(glm::vec3(0.536837f, 2.833382f, - 0.536838f));
		m_vertices.push_back(glm::vec3(0.657489f, 2.833382f, - 0.379601f));
		m_vertices.push_back(glm::vec3(0.733334f, 2.833382f, - 0.196496f));
		m_vertices.push_back(glm::vec3(0.382877f, 2.908238f, - 0.000000f));
		m_vertices.push_back(glm::vec3(0.369830f, 2.908238f, 0.099096f));
		m_vertices.push_back(glm::vec3(0.331581f, 2.908238f, 0.191438f));
		m_vertices.push_back(glm::vec3(0.270735f, 2.908238f, 0.270735f));
		m_vertices.push_back(glm::vec3(0.191438f, 2.908238f, 0.331581f));
		m_vertices.push_back(glm::vec3(0.099096f, 2.908238f, 0.369830f));
		m_vertices.push_back(glm::vec3(0.000000f, 2.908238f, 0.382876f));
		m_vertices.push_back(glm::vec3(- 0.099096f, 2.908238f, 0.369830f));
		m_vertices.push_back(glm::vec3(- 0.191438f, 2.908238f, 0.331581f));
		m_vertices.push_back(glm::vec3(- 0.270735f, 2.908238f, 0.270735f));
		m_vertices.push_back(glm::vec3(- 0.331581f, 2.908238f, 0.191438f));
		m_vertices.push_back(glm::vec3(- 0.369830f, 2.908238f, 0.099096f));
		m_vertices.push_back(glm::vec3(- 0.382877f, 2.908238f, - 0.000000f));
		m_vertices.push_back(glm::vec3(- 0.369830f, 2.908238f, - 0.099096f));
		m_vertices.push_back(glm::vec3(- 0.331581f, 2.908238f, - 0.191438f));
		m_vertices.push_back(glm::vec3(- 0.270735f, 2.908238f, - 0.270735f));
		m_vertices.push_back(glm::vec3(- 0.191438f, 2.908238f, - 0.331581f));
		m_vertices.push_back(glm::vec3(- 0.099096f, 2.908238f, - 0.369830f));
		m_vertices.push_back(glm::vec3(0.000000f,2.908238f, - 0.382877f));
		m_vertices.push_back(glm::vec3(0.099096f,2.908238f, - 0.369830f));
		m_vertices.push_back(glm::vec3(0.191438f,2.908238f,- 0.331581f));
		m_vertices.push_back(glm::vec3(0.270735f,2.908238f,- 0.270735f));
		m_vertices.push_back(glm::vec3(0.331581f,2.908238f,- 0.191438f));
		m_vertices.push_back(glm::vec3(0.369830f,2.908238f,- 0.099096f));*/
}