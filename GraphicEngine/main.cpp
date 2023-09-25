#include <GraphicsMotor.h>
#include <chrono>

int main() {
   
    GraphicsMotor graphicsMotor = GraphicsMotor();

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!graphicsMotor.ExitCondition()) {

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        graphicsMotor.Update(frameTime);
        graphicsMotor.Render();

    }

    graphicsMotor.Shutdown();

    return EXIT_SUCCESS;
}