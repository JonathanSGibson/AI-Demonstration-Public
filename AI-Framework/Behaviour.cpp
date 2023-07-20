#include "Behaviour.h"
#include "Blackboard.h"
#include "GridManager.h"

BlackboardBaseType* Behaviour::SearchBlackboard(map<const char*, BlackboardBaseType*> blackboard, const char* key)
{
	if (blackboard.find(key) == blackboard.end())
	{
		return nullptr;
	}
	else
	{
		return blackboard.find(key)->second;
	}
}

Status Behaviour::Tick(map<const char*, BlackboardBaseType*> blackboard)
{
	if (currentStatus == BH_INVALID) 
		onInit(blackboard);

	currentStatus = Update(blackboard);

	if (currentStatus != BH_RUNNING) 
		onExit(currentStatus);

	return currentStatus;
}