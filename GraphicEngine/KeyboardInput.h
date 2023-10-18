#pragma once

#include "VisualGameObject.h"
#include "Window.h"

class KeyboardInput {
public:
    struct KeyMappings {
        int moveLeft = GLFW_KEY_A;
        int moveRight = GLFW_KEY_D;
        int moveForward = GLFW_KEY_W;
        int moveBackward = GLFW_KEY_S;
        int moveUp = GLFW_KEY_SPACE;
        int moveDown = GLFW_KEY_LEFT_SHIFT;
        int lookLeft = GLFW_KEY_LEFT;
        int lookRight = GLFW_KEY_RIGHT;
        int lookUp = GLFW_KEY_UP;
        int lookDown = GLFW_KEY_DOWN;
        int closeWindow = GLFW_KEY_ESCAPE;
        int imGuiVisibilityTrigger = GLFW_KEY_I;
    };

    void MoveInPlaneXZ(GLFWwindow* window, float dt, VisualGameObject& gameObject);

    void ImGuiControls(GLFWwindow* window,bool& visibilityTrigger);

    KeyMappings keys{};
    float moveSpeed{ 3.f };
    float lookSpeed{ 1.5f };
    int lastStateImGuiVisibilityTrigger = -1;
};
