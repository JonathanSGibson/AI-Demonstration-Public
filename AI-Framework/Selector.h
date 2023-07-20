#pragma once
#include "Composite.h"
class Selector :
    public Composite
{
protected:
    Behaviours::iterator currentChild;
    virtual void onInit(map<const char*, BlackboardBaseType*> blackboard) override { currentChild = children.begin(); }
    virtual void onExit(Status) override { currentChild = children.begin(); }
    virtual Status Update(map<const char*, BlackboardBaseType*> blackboard) override;
};

