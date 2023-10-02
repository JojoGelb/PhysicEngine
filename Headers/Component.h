#pragma once

class Component {

public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Shutdown() = 0;
};