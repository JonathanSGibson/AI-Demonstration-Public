#pragma once
#include "BaseEntity.h"
#include "Steering.h"
#include <string>

class GridManager;
class IState;

class Ghost :
    public BaseEntity
{
private:
    Wander* wander;
    Flee* flee;
    Seek* seek;
    GridManager* grid;
    BaseEntity* miner;
    IState* currentState;
public:
    Ghost(sf::RenderWindow* window, BaseEntity* miner_, GridManager* grid_);
    Wander* GetWander() { return wander; };
    Flee* GetFlee() { return flee; };
    Seek* GetSeek() { return seek; };
    bool GetCollected();
    bool GetAllPlaced();
    bool IsInPosessionRange();
    BaseEntity* getMiner() { return miner; }
    void Think();
    void SetHUDString(std::string string_) { HUDInformation = string_; }
};


class IState {
public:
    virtual void Enter() = 0;
    virtual IState* Update() = 0;
    virtual void Exit() = 0;
protected:
    Ghost* ghost;
};

class WanderState : public IState {
public:
     void Enter();
     WanderState(BaseEntity* ghost_);
     IState* Update();
     void Exit();
};

class HuntState : public IState {
public:
    HuntState(BaseEntity* ghost_);
    void Enter();
    IState* Update();
    void Exit();
};

class PossessionState : public IState {
public:
    PossessionState(BaseEntity* ghost_);
    void Enter();
    IState* Update();
    void Exit();
};