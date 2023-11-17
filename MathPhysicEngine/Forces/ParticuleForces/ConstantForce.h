#pragma once
#include "../ForceGenerator.h"
class ConstantForce :
    public ForceGenerator
{
private:
    Vector3D m_constantForce;
public:
    void UpdateForce(Particle* particle, float duration);
    ConstantForce(const Vector3D m_constantForce);
};
