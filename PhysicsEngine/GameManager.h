#pragma once

#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H

////////////////////////////////
// Name: GameManager.h	      //
// Author: Alexander Kocourek //
// Date: 15/02/2017			  //
////////////////////////////////

#include <vector>
#include <chrono>
#include <Windows.h>

#include "Defs.h"
#include "Sphere.h";
#include "Plane.h"
#include "GameObject.h"

class GameManager
{
public:
	GameManager();
	~GameManager();
	void Shutdown();
	void Loop();
	void InitScene();
	void KeyboardHandle(unsigned char key, int x, int y);
	//int Run();
	void Exit(int);

private:
	const unsigned int m_windowHeight = 768;
	const unsigned int m_windowWidth = 1024;
	
	int m_exitCode;
	bool m_isRunning;
	bool m_isRendering;
	bool m_isExiting;
	bool m_keyState[256];
	float m_deltaTime;
	float m_targetFrameTime;

	std::chrono::high_resolution_clock g_timer;
	using ms = std::chrono::duration<float, std::milli>;
	
	std::chrono::time_point<std::chrono::steady_clock> oldTime;
	std::chrono::time_point<std::chrono::steady_clock> newTime;
	std::chrono::time_point<std::chrono::steady_clock> loopOldTime;
	std::chrono::time_point<std::chrono::steady_clock> loopNewTime;

	//std::vector<RigidBody*> m_rigidBodys;
	std::vector<GameObject*> m_gameObjects;
	//std::vector<Sphere*> m_spheres;

	void clearKeyState();
	void draw();
	void drawPlane();
	void drawText(glm::vec2 position, glm::vec3 color, std::string text);
	void update();
	float collisionDetection(const Collision::SphereCollider &col1, const Collision::SphereCollider &col2);
	void collisionResponse(GameObject& g1, GameObject& g2, float intersection);
};

#endif // !_GAMEMANAGER_H