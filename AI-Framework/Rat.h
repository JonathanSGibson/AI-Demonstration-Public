#pragma once
#include "BaseEntity.h"
#include "Steering.h"
class Rat :
    public BaseEntity
{
public:
    Rat(BaseEntity* miner_, BaseEntity* ghost_, sf::RenderWindow* window);
    void Think();
private:
    float GetWeighting(BaseEntity*);
    BaseEntity* miner;
    BaseEntity* ghost;
    std::vector<std::pair<Steering*, float>> steerings;
};

