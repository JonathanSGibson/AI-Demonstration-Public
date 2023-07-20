#pragma once
#include <map>

class BehaviourTree;
class BlackboardBaseType;

using namespace std;

enum Status
{
	BH_INVALID,
	BH_FAIL,
	BH_SUCCESS,
	BH_RUNNING
};

class Behaviour
{
protected:
	virtual void onInit(map<const char*, BlackboardBaseType*> blackboard) {}
	virtual Status Update(map<const char*, BlackboardBaseType*> blackboard) = 0;
	virtual void onExit(Status) {}
	BlackboardBaseType* SearchBlackboard(map<const char*, BlackboardBaseType*> blackboard, const char*);
private:
	Status currentStatus;
public:
	Status Tick(map<const char*, BlackboardBaseType*> blackboard);
	Behaviour() : currentStatus(BH_INVALID) {  }
	virtual ~Behaviour() {}
};

