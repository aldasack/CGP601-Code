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
#include "GameObject.h"

class GameManager
{
public:
	GameManager();
	~GameManager();
	void Shutdown();
	void Loop();
	void InitEngine();
	void KeyboardHandle(unsigned char key, int x, int y);
	//int Run();
	void Exit(int);

private:
	int m_exitCode;
	bool m_isRunning;
	bool m_isRendering;
	bool m_isExiting;
	bool m_keyState[256];
	float m_deltaTime;
	float m_targetFrameTime;
	unsigned int m_windowHeight;
	unsigned int m_windowWidth;

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
	void drawText(Math::Vector2 position, Math::Vector3 color, std::string text);
	void update();
};

#endif // !_GAMEMANAGER_H