#pragma once
#include "Singleton.h"
#include "Graphics.h"
#include <algorithm>
class InputManager : public Singleton<InputManager>
{
public:
	enum
	{
		W_KEY,
		A_KEY,
		S_KEY,
		D_KEY,
		Q_KEY,
		E_KEY,
		LEFT_MOUSE,
		RIGHT_MOUSE,
		KeyCount
	};
	bool state[KeyCount];
	bool lastState[KeyCount];
	glm::dvec2 cursorPos;
	glm::dvec2 lastCursorPos;
	void HadnleInput(GLFWwindow* window);
	bool GetKey(int key);
};