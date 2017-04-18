#pragma once

#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H

////////////////////////////////
// Name: GameManager.h	      //
// Author: Alexander Kocourek //
// Date: 15/02/2017			  //
////////////////////////////////

#include <time.h>
#include <chrono>
#include <Windows.h>

#include "Defs.h"

// This class is a singleton
class GameManager
{
public:
	static GameManager& GetInstance()
	{
		if (!s_instance)
			s_instance = new GameManager();

		return *s_instance;
	}
	~GameManager();
	void Shutdown();
	void Loop();
	void InitScene();
	void KeyboardHandle(unsigned char key, int x, int y);
	//int Run();
	void Exit(int);

	void AddRigidBody(RigidBody& rigidbody);

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

	// stores all RigidBodys to update them
	std::vector<RigidBody*> m_rigidBodys;
	// stores all GameObjects to draw them
	std::vector<GameObject*> m_gameObjects;
	PhysicsManager* m_pPhysicsManager;

	GameManager();
	void clearKeyState();
	void draw();
	void drawText(glm::vec2 position, glm::vec3 color, std::string text);

	static GameManager* s_instance;
};

#endif // !_GAMEMANAGER_H