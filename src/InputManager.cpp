#include "InputManager.h"

void InputManager::HadnleInput(GLFWwindow* window)
{
    memcpy(&lastState, state, KeyCount);
    lastCursorPos = cursorPos;
    glfwPollEvents();
    std::fill(std::begin(state), std::end(state), 0);
    glfwGetCursorPos(window, &cursorPos.x, &cursorPos.y);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) state[W_KEY] = true;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) state[A_KEY] = true;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) state[S_KEY] = true;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) state[D_KEY] = true;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) state[Q_KEY] = true;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) state[E_KEY] = true;
    if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)) state[RIGHT_MOUSE] = true;
    if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)) state[LEFT_MOUSE] = true;
}

bool InputManager::GetKey(int key)
{
    return state[key];
}
