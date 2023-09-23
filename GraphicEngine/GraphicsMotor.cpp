#include "GraphicsMotor.h"


GraphicsMotor::GraphicsMotor() : window(800, 600, "Test"),
								 vulkanHandler(window) 
								 /*imguiHandler(window.GetWindow(), vulkanHandler) */{};

GraphicsMotor::~GraphicsMotor()
{
}

void GraphicsMotor::Update(float frameTime)
{
	window.Update();
	vulkanHandler.Update(frameTime);
}

void GraphicsMotor::Render()
{
	vulkanHandler.Render();
	//imguiHandler.Render();
}

void GraphicsMotor::Shutdown()
{
	vulkanHandler.Shutdown();
	window.Shutdown();
}

bool GraphicsMotor::ExitCondition()
{
	return window.ShouldClose();
}
