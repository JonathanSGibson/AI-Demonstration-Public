#pragma once
#include "Behaviour.h"
#include <vector>
class Composite :
    public Behaviour
{
public:
    void addChild(Behaviour* child) { children.push_back(child); };
    void removeChild(Behaviour*);
    void ClearChildren() { children.clear(); };
protected:
    typedef std::vector<Behaviour*> Behaviours;
    Behaviours children;

};

