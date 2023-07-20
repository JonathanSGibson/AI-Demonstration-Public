#pragma once
#include "Behaviour.h"
class Decorator :
    public Behaviour
{
protected:
    Behaviour* child;
public:
    Decorator(Behaviour* _child) : child(_child) {}
};

