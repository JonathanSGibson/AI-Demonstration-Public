#pragma once
# include "Behaviour.h"
#include "GridManager.h"
#include <SFML/Graphics.hpp>

const char* const GridKey = "grid";
const char* const AgentKey = "agent";
const char* const PathKey = "path";
const char* const TargetPosKey = "targetPos";
const char* const HandFullKey = "handFull";
const char* const CollectiblesKey = "collectibles";
const char* const CurrentCollectibleKey = "currentCollectible";
const char* const GhostKey = "ghost";
const char* const PossessableKey = "possessable";
const char* const CounterKey = "counter";
const char* const BehaviourNameKey = "behaviourName";
const char* const WindowKey = "window";


class HandEmptyQuery: public Behaviour{
protected:
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
};

class FindPath : public Behaviour {
protected:
	void onInit(map<const char*, BlackboardBaseType*> blackboard);
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
	GridManager* grid;
	BaseEntity* agent;
};

class FollowPath : public Behaviour {
protected:
	void onInit(map<const char*, BlackboardBaseType*> blackboard);
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
	void onExit(Status);
	vector<Cell*> path;
	BaseEntity* agent;
};

class FollowPathSearching : public Behaviour {
protected:
	void onInit(map<const char*, BlackboardBaseType*> blackboard);
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
	void onExit(Status);
	vector<BaseEntity*> collectibles;
	vector<Cell*> path;
	BaseEntity* agent;
	sf::CircleShape circle{ ObjectSearchRadius };
};

class Collect : public Behaviour {
protected:
	void onInit(map<const char*, BlackboardBaseType*> blackboard);
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
	vector<MapObject*> collectibles;
	BaseEntity* agent;
};

class Place : public Behaviour {
protected:
	void onInit(map<const char*, BlackboardBaseType*> blackboard);
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
	BaseEntity* agent;
};

class PlacePossessed : public Behaviour {
protected:
	void onInit(map<const char*, BlackboardBaseType*> blackboard);
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
	BaseEntity* agent;
};

class HandFullQuery : public Behaviour {
protected:
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
};

class GetRandomPosition : public Behaviour {
protected:
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
};

class GetStartPosition : public Behaviour {
protected:
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
};

class AllCollected :public Behaviour {
protected:
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
};

class AllNotCollected :public Behaviour {
protected:
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
};

class GhostTooFar : public Behaviour {
protected:
	void onInit(map<const char*, BlackboardBaseType*> blackboard);
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
	BaseEntity* agent;
	BaseEntity* ghost;
};

class PossessableFalse : public Behaviour {
protected:
	void onInit(map<const char*, BlackboardBaseType*> blackboard);
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
};

class Idle :public Behaviour
{
protected:
	Status Update(map<const char*, BlackboardBaseType*> blackboard);
};
