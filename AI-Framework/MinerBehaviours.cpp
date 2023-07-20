#include "MinerBehaviours.h"
#include "Blackboard.h"
#include "UtilRandom.h"
#include "Constants.h"
#include "BaseEntity.h"

Status HandEmptyQuery::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	BlackboardBoolType* handFull = (BlackboardBoolType*)SearchBlackboard(blackboard, HandFullKey);
	if (handFull == nullptr || handFull->GetValue())
		return BH_FAIL;
	return BH_SUCCESS;
}

Status HandFullQuery::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	BlackboardBoolType* handFull = (BlackboardBoolType*)SearchBlackboard(blackboard, HandFullKey);
	if (handFull == nullptr || !handFull->GetValue())
		return BH_FAIL;
	return BH_SUCCESS;
}

Status GetRandomPosition::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	//Generates and stores a random position within the limits of the screen
	Vector2f* temp = new sf::Vector2f();
	temp->x = UtilRandom::instance()->GetRange(0, ScreenWidth);
	temp->y = UtilRandom::instance()->GetRange(0, ScreenHeight);

	BlackboardVector2fType* targetPos = (BlackboardVector2fType*)SearchBlackboard(blackboard, TargetPosKey);
	if (targetPos == nullptr)
		blackboard.insert(std::pair<const char*, BlackboardBaseType*>(TargetPosKey, new BlackboardVector2fType(temp)));
	else
		targetPos->SetValue(temp);

	return BH_SUCCESS;
}

void Collect::onInit(map<const char*, BlackboardBaseType*> blackboard)
{
	vector<BaseEntity*> temp = ((BlackboardVectorBaseEntityType*)SearchBlackboard(blackboard, CollectiblesKey))->GetValue();
	for (BaseEntity* object : temp)
	{
		collectibles.push_back((MapObject*)object);
	}
	agent = ((BlackboardBaseEntityType*)SearchBlackboard(blackboard, AgentKey))->GetValue();
}

Status Collect::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	//Checks all the objects to see if any of them are within range to be collected, if so it collects that one then returns success
	for (MapObject* object : collectibles)
	{
		if (CloseEnough(agent->getPosition(), object->getPosition(), PathfindingTolerance))
		{
			object->ToggleVisible(false);

			BlackboardBaseEntityType* currentObject = (BlackboardBaseEntityType*)SearchBlackboard(blackboard, CurrentCollectibleKey);
			if (currentObject == nullptr)
				blackboard.insert(std::pair<const char*, BlackboardBaseEntityType*>(TargetPosKey, new BlackboardBaseEntityType(object)));
			else
			{
				currentObject->SetValue(object);
			}
			BlackboardBoolType* temp = ((BlackboardBoolType*)SearchBlackboard(blackboard, HandFullKey));
			if (temp != nullptr)
				temp->SetValue(true);
			return BH_SUCCESS;
		}
	}
	return BH_FAIL;
}

void Place::onInit(map<const char*, BlackboardBaseType*> blackboard)
{
	agent = ((BlackboardBaseEntityType*)SearchBlackboard(blackboard, AgentKey))->GetValue();

	BlackboardIntType* counter = (BlackboardIntType*)SearchBlackboard(blackboard, CounterKey);
	if (counter == nullptr)
		blackboard.insert(std::pair<const char*, BlackboardBaseType*>(CounterKey, new BlackboardIntType(0)));

	BlackboardBoolType* possessable = (BlackboardBoolType*)SearchBlackboard(blackboard, PossessableKey);
	if (possessable == nullptr)
		blackboard.insert(std::pair<const char*, BlackboardBaseType*>(PossessableKey, new BlackboardBoolType(false)));
}

Status Place::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	//Place the currently held object at the position of the agent

	MapObject* currentObject = (MapObject*)((BlackboardBaseEntityType*)SearchBlackboard(blackboard, CurrentCollectibleKey))->GetValue();
	if (currentObject == nullptr)
		return BH_FAIL;

	currentObject->setPosition(agent->getPosition());
	currentObject->SetPosToNearestCell();
	currentObject->ToggleVisible(true);
	currentObject->SetCollected(true);
	((BlackboardBaseEntityType*)SearchBlackboard(blackboard, CurrentCollectibleKey))->SetValue(nullptr);
	BlackboardBoolType* temp = ((BlackboardBoolType*)SearchBlackboard(blackboard, HandFullKey));
	if (temp != nullptr)
		temp->SetValue(false);

	BlackboardIntType* counter = (BlackboardIntType*)SearchBlackboard(blackboard, CounterKey);
	counter->SetValue(counter->GetValue() + 1);

	if (counter->GetValue() >= HauntedObjectTotal)
	{
		BlackboardBoolType* possessable = ((BlackboardBoolType*)SearchBlackboard(blackboard, PossessableKey));
	if (possessable != nullptr)
		possessable->SetValue(true);
	}
		return BH_SUCCESS;
}


void PlacePossessed::onInit(map<const char*, BlackboardBaseType*> blackboard)
{
	agent = ((BlackboardBaseEntityType*)SearchBlackboard(blackboard, AgentKey))->GetValue();
}

Status PlacePossessed::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	//Same as place except counter goes down not up
	MapObject* currentObject = (MapObject*)((BlackboardBaseEntityType*)SearchBlackboard(blackboard, CurrentCollectibleKey))->GetValue();
	if (currentObject == nullptr)
		return BH_FAIL;

	currentObject->setPosition(agent->getPosition());
	currentObject->SetPosToNearestCell();
	currentObject->ToggleVisible(true);
	currentObject->SetCollected(false);
		((BlackboardBaseEntityType*)SearchBlackboard(blackboard, CurrentCollectibleKey))->SetValue(nullptr);
	BlackboardBoolType* temp = ((BlackboardBoolType*)SearchBlackboard(blackboard, HandFullKey));
	if (temp != nullptr)
		temp->SetValue(false);

	BlackboardIntType* counter = (BlackboardIntType*)SearchBlackboard(blackboard, CounterKey);
	counter->SetValue(counter->GetValue() - 1);

	if (counter->GetValue() <= 0)
	{
		BlackboardBoolType* possessable = ((BlackboardBoolType*)SearchBlackboard(blackboard, PossessableKey));
		if (possessable != nullptr)
			possessable->SetValue(false);
	}

	return BH_SUCCESS;
}

void FollowPath::onInit(map<const char*, BlackboardBaseType*> blackboard)
{
	agent = ((BlackboardBaseEntityType*)SearchBlackboard(blackboard, AgentKey))->GetValue();
}

Status FollowPath::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	//Follows the path generated by find path
	BlackboardStringType* behaviourName = (BlackboardStringType*)SearchBlackboard(blackboard, BehaviourNameKey);
	if (behaviourName == nullptr)
		blackboard.insert(std::pair<const char*, BlackboardBaseType*>(TargetPosKey, new BlackboardStringType("Follow Path")));
	else
		behaviourName->SetValue("Follow Path");

	if (path.empty())
	{
		path = ((BlackboardVectorCellType*)SearchBlackboard(blackboard, PathKey))->GetValue();
		if (path.empty())
			return BH_FAIL;
	}

	Cell* targetCell = path.front();
	
	Vector2f direction = agent->getPosition() - targetCell->GetOrigin();
	Vector2f force;


	float distance = Magnitude(direction);
	direction /= Magnitude(direction);

	force = agent->getVelocity() - direction;

	agent->applyForce(force);
	
	if (CloseEnough(targetCell->GetOrigin(), agent->getPosition(), PathfindingTolerance))
	{
		path.erase(path.begin());
		if (path.empty())
		{
			agent->Stop();
			return BH_SUCCESS;
		}
	}

	return BH_RUNNING;
}

void FollowPath::onExit(Status status)
{
	path.clear();
	agent->Stop();
}

void FollowPathSearching::onInit(map<const char*, BlackboardBaseType*> blackboard)
{
	
	agent = ((BlackboardBaseEntityType*)SearchBlackboard(blackboard, AgentKey))->GetValue();
	collectibles = ((BlackboardVectorBaseEntityType*)SearchBlackboard(blackboard, CollectiblesKey))->GetValue();
	circle.setOrigin(ObjectSearchRadius, ObjectSearchRadius);
	circle.setOutlineColor(sf::Color::Cyan);
	circle.setOutlineThickness(1);
	circle.setFillColor(sf::Color::Transparent);
}

Status FollowPathSearching::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	//Same as find path except it is interrupted if an object comes within a set radius of the agent
	BlackboardStringType* behaviourName = (BlackboardStringType*)SearchBlackboard(blackboard, BehaviourNameKey);
	if (behaviourName == nullptr)
		blackboard.insert(std::pair<const char*, BlackboardBaseType*>(TargetPosKey, new BlackboardStringType("Follow Path While Searching")));
	else
		behaviourName->SetValue("Follow Path While Searching");

	BaseEntity* agent = ((BlackboardBaseEntityType*)SearchBlackboard(blackboard, AgentKey))->GetValue();
	if (agent->GetOnHUD())
	{
		Vector2f agentPos = agent->getPosition();
		circle.setPosition(agentPos);
		sf::RenderWindow* window = ((BlackboardWindowType*)SearchBlackboard(blackboard, WindowKey))->GetValue();
		if (window != nullptr)
			window->draw(circle);
	}


	if (collectibles.empty())
		return BH_FAIL;
	if (path.empty())
	{
		path = ((BlackboardVectorCellType*)SearchBlackboard(blackboard, PathKey))->GetValue();
		if (path.empty())
			return BH_FAIL;
	}

	Cell* targetCell = path.front();

	Vector2f direction = agent->getPosition() - targetCell->GetOrigin();
	Vector2f force;


	float distance = Magnitude(direction);
	direction /= Magnitude(direction);

	force = agent->getVelocity() - direction;

	agent->applyForce(force);

	for (BaseEntity* object : collectibles)
	{
		if (Distance(object->getPosition(), agent->getPosition()) <= ObjectSearchRadius && !((MapObject*)object)->getCollected())
		{
			//Set the target to be the position of the first object in the radius
			Vector2f* temp = new Vector2f();
			temp->x = object->getPosition().x;
			temp->y = object->getPosition().y;
			BlackboardVector2fType* targetPos = (BlackboardVector2fType*)SearchBlackboard(blackboard, TargetPosKey);
			if (targetPos == nullptr)
				blackboard.insert(std::pair<const char*, BlackboardBaseType*>(TargetPosKey, new BlackboardVector2fType(temp)));
			else
				targetPos->SetValue(temp);
			return BH_SUCCESS;
		}
	}

	if (CloseEnough(targetCell->GetOrigin(), agent->getPosition(), PathfindingTolerance))
	{
		path.erase(path.begin());
		if (path.empty())
		{
			agent->Stop();
			return BH_FAIL;
		}
	}

	return BH_RUNNING;
}

void FollowPathSearching::onExit(Status status)
{
	path.clear();
	agent->Stop();
}

void FindPath::onInit(map<const char*, BlackboardBaseType*> blackboard)
{

	grid = ((BlackboardGridManagerType*)SearchBlackboard(blackboard, GridKey))->GetValue();
	agent = ((BlackboardBaseEntityType*)SearchBlackboard(blackboard, AgentKey))->GetValue();

}

Status FindPath::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	//Uses Astar pathfinding to create a path to the target in the blackboard
	Vector2f agentPos = agent->getPosition();
	Vector2f* targetPos = ((BlackboardVector2fType*)SearchBlackboard(blackboard, TargetPosKey))->GetValue();

	if (targetPos == nullptr)
		return BH_FAIL;

	BlackboardVectorCellType* path = (BlackboardVectorCellType*)SearchBlackboard(blackboard, PathKey);
	vector<Cell*> tempPath = grid->AStar(agentPos.x, agentPos.y, targetPos->x, targetPos->y);

	if (tempPath.size() > 0)
	{
		if (path == nullptr)
			blackboard.insert(std::pair<const char*, BlackboardBaseType*>(PathKey, new BlackboardVectorCellType(tempPath)));
		else
			path->SetValue(tempPath);

		return BH_SUCCESS;
	}
	return BH_FAIL;
}

Status GetStartPosition::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	Vector2f startPos = ((BlackboardGridManagerType*)SearchBlackboard(blackboard, GridKey))->GetValue()->GetMinecart()->getPosition();

	Vector2f* temp = new sf::Vector2f();
	temp->x = startPos.x;
	temp->y = startPos.y;

	BlackboardVector2fType* targetPos = (BlackboardVector2fType*)SearchBlackboard(blackboard, TargetPosKey);
	if (targetPos == nullptr)
		blackboard.insert(std::pair<const char*, BlackboardBaseType*>(TargetPosKey, new BlackboardVector2fType(temp)));
	else
		targetPos->SetValue(temp);

	return BH_SUCCESS;
}

Status AllCollected::Update(map<const char*, BlackboardBaseType*> blackboard)
{

	if (((BlackboardGridManagerType*)SearchBlackboard(blackboard, GridKey))->GetValue()->AllCollected())
		return BH_SUCCESS;
	return BH_FAIL;
}

Status AllNotCollected::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	if (((BlackboardGridManagerType*)SearchBlackboard(blackboard, GridKey))->GetValue()->AllPlaced())
		return BH_SUCCESS;
	return BH_FAIL;
}


void GhostTooFar::onInit(map<const char*, BlackboardBaseType*> blackboard)
{
	agent = ((BlackboardBaseEntityType*)SearchBlackboard(blackboard, AgentKey))->GetValue();
	ghost = ((BlackboardBaseEntityType*)SearchBlackboard(blackboard, GhostKey))->GetValue();
}

Status GhostTooFar::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	if (CloseEnough(ghost->getPosition(), agent->getPosition(), PosessionRange))
		return BH_FAIL;

	return BH_SUCCESS;
}

void PossessableFalse::onInit(map<const char*, BlackboardBaseType*> blackboard)
{
	BlackboardBoolType* possessable = (BlackboardBoolType*)SearchBlackboard(blackboard, PossessableKey);
	if (possessable == nullptr)
		blackboard.insert(std::pair<const char*, BlackboardBaseType*>(PossessableKey, new BlackboardBoolType(false)));
}

Status PossessableFalse::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	BlackboardBoolType* possessable = (BlackboardBoolType*)SearchBlackboard(blackboard, PossessableKey);
	if (possessable->GetValue())
		return BH_FAIL;

	return BH_SUCCESS;
}

Status Idle::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	BlackboardStringType* behaviourName = (BlackboardStringType*)SearchBlackboard(blackboard, BehaviourNameKey);
	if (behaviourName == nullptr)
		blackboard.insert(std::pair<const char*, BlackboardBaseType*>(TargetPosKey, new BlackboardStringType("Idle")));
	else
		behaviourName->SetValue("Idle");

	return BH_SUCCESS;
}
