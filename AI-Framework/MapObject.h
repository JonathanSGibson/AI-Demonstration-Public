#pragma once
#include "BaseEntity.h"

class GridManager;


class MapObject :
    public BaseEntity
{
public:
    MapObject(std::string fileLocation, GridManager* grid_);
    void SetPosToNearestCell();
    void ToggleVisible(bool);
    void SetCollected(bool collected_) { collected = collected_; }
    bool getCollected() { return collected; }
private:
    GridManager* grid;
    bool collected = false;
};

