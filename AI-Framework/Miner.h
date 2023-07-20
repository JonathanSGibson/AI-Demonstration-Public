#pragma once
#include "BaseEntity.h"
#include "Blackboard.h"
#include "GridManager.h"
#include <map>
#include <string>
#include <vector>
#include "MinerBehaviours.h"
#include "Selector.h"
#include "Sequence.h"

using namespace std;

class Miner :
    public BaseEntity
{
private:
    map<const char*, BlackboardBaseType*> blackboard;
    Selector* root;
    GridManager* grid;
public:
    Miner(GridManager* grid_, sf::RenderWindow* window_);
    void Think() { root->Tick(blackboard); CreateString(); }
    void SetGhost(BaseEntity* ghost) {blackboard.insert(std::pair<const char*, BlackboardBaseType*>(GhostKey, new BlackboardBaseEntityType(ghost)));}
    GridManager* GetGrid() { return grid; }
    void CreateString();
};