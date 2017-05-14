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
	
	/*
	*	Singleton implementation.
	*	Retruns instance of the GameManger.
	*	If the instance is nullptr, a new GameManager is created
	*/
	static GameManager& GetInstance()
	{
		if (!s_instance)
			s_instance = new GameManager();

		return *s_instance;
	}
	~GameManager();
	/*
	*	Destructor replacement.
	*/
	void Shutdown();
	/*
	*	Main loop. Calls draw and update of objects.
	*	Creates a fixed fps.
	*/
	void Loop();
	/*
	*	Initializes the scene  with all gameobjects
	*/
	void InitScene();
	/*
	*	Recives Keyboard input and saves it.
	*/
	void KeyboardHandle(unsigned char key, int x, int y);

private:
	const unsigned int m_windowHeight = 1080;
	const unsigned int m_windowWidth = 1920;

	
	bool m_isRunning;
	bool m_isRendering;
	bool m_keyState[256];
	float m_deltaTime;
	float m_targetFrameTime;

	std::chrono::high_resolution_clock g_timer;
	using ms = std::chrono::duration<float, std::milli>;
	
	// times to create a fixed time step and calculate current fps.
	std::chrono::time_point<std::chrono::steady_clock> oldTime;
	std::chrono::time_point<std::chrono::steady_clock> newTime;
	std::chrono::time_point<std::chrono::steady_clock> loopOldTime;
	std::chrono::time_point<std::chrono::steady_clock> loopNewTime;

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