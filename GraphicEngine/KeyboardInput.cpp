#include "KeyboardInput.h"

// std
#include <limits>

void KeyboardInput::MoveInPlaneXZ(
    GLFWwindow* window, float dt, VisualGameObject& gameObject) {
    glm::vec3 rotate{ 0 };
    if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
    if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
    if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
    if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;

    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
        gameObject.transformVisual.rotation += lookSpeed * dt * glm::normalize(rotate);
    }

    // limit pitch values between about +/- 85ish degrees
    gameObject.transformVisual.rotation.x = glm::clamp(gameObject.transformVisual.rotation.x, -1.5f, 1.5f);
    gameObject.transformVisual.rotation.y = glm::mod(gameObject.transformVisual.rotation.y, glm::two_pi<float>());

    float yaw = gameObject.transformVisual.rotation.y;
    const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
    const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
    const glm::vec3 upDir{ 0.f, -1.f, 0.f };

    glm::vec3 moveDir{ 0.f };
    if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
    if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
    if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
    if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
    if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
    if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        gameObject.transformVisual.translation += moveSpeed * dt * glm::normalize(moveDir);
    }
}

void KeyboardInput::ImGuiControls(
    GLFWwindow* window, bool &visibilityTrigger) {

    if (glfwGetKey(window, keys.closeWindow) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    
    int nextState = glfwGetKey(window, keys.imGuiVisibilityTrigger);
    visibilityTrigger = lastStateImGuiVisibilityTrigger != nextState && nextState == GLFW_PRESS;
    lastStateImGuiVisibilityTrigger = nextState;
}


void KeyboardInput::BlobControls(GLFWwindow* window, Vector3D& inputForce) {
    if (glfwGetKey(window, keys.closeWindow) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    
    if (glfwGetKey(window, keys.moveLeftBlob) == GLFW_PRESS && glfwGetKey(window, keys.moveRightBlob)) {
        inputForce = 0.f;
        return;
    }
    if (glfwGetKey(window, keys.moveLeftBlob) == GLFW_PRESS) {
        inputForce = {-4.f,0.f,0.f };
        return;
    }
    if (glfwGetKey(window, keys.moveRightBlob) == GLFW_PRESS) {
        inputForce = { 4.f,0.f,0.f };
        return;
    }
    inputForce = { 0.f,0.f,0.f };
}