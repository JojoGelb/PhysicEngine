#include <GraphicsMotor.h>
#include <chrono>
#include "../MathPhysicEngine/MathPhysicEngine.h"
#include "../Sources/ImGuiEngine.h"

int main() {

    ObjectData* objectData = new ObjectData();

    GraphicsMotor graphicsMotor = GraphicsMotor(objectData);
    MathPhysicsEngine mathPhysics = MathPhysicsEngine(objectData);
    ImGuiEngine imGuiEngine = ImGuiEngine(graphicsMotor.GetGLFWWindow(),objectData);
    mathPhysics.Init();

    double t = 0.0f;

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!graphicsMotor.ExitCondition()) {

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        imGuiEngine.Update();
        mathPhysics.Update(t, frameTime);
        graphicsMotor.Update(frameTime);
        graphicsMotor.Render();

        t += frameTime;
    }

    graphicsMotor.Shutdown();

    return EXIT_SUCCESS;
}