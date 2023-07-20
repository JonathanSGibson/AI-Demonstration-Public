#include "Miner.h"


Miner::Miner(GridManager* grid_, sf::RenderWindow* window_)
{
	grid = grid_;
	isMiner = true;

	vector<BaseEntity*> temp;
	for (BaseEntity* object : grid_->GetHauntedObject())
		temp.push_back(object);


	//Creating blackboards for my behaviour tree to use
	blackboard.insert(std::pair<const char*, BlackboardBaseType*>(GridKey, new BlackboardGridManagerType(grid_)));
	blackboard.insert(std::pair<const char*, BlackboardBaseType*>(AgentKey, new BlackboardBaseEntityType(this)));
	blackboard.insert(std::pair<const char*, BlackboardBaseType*>(PathKey, new BlackboardVectorCellType(vector<Cell*>())));
	blackboard.insert(std::pair<const char*, BlackboardBaseType*>(TargetPosKey, new BlackboardVector2fType(new Vector2f())));
	blackboard.insert(std::pair<const char*, BlackboardBaseType*>(HandFullKey, new BlackboardBoolType(false)));
	blackboard.insert(std::pair<const char*, BlackboardBaseType*>(CollectiblesKey, new BlackboardVectorBaseEntityType(temp)));
	blackboard.insert(std::pair<const char*, BlackboardBaseType*>(CurrentCollectibleKey, new BlackboardBaseEntityType(new BaseEntity())));
	blackboard.insert(std::pair<const char*, BlackboardBaseType*>(PossessableKey, new BlackboardBoolType(false)));
	blackboard.insert(std::pair<const char*, BlackboardBaseType*>(CounterKey, new BlackboardIntType(0)));
	blackboard.insert(std::pair<const char*, BlackboardBaseType*>(BehaviourNameKey, new BlackboardStringType("N/A")));
	blackboard.insert(std::pair<const char*, BlackboardBaseType*>(WindowKey, new BlackboardWindowType(window_)));


	//Creating the behaviour tree itself (Please see the diagram in the "design slash planning" folder)
	root = new Selector();
	
	Selector* mine = new Selector();
	root->addChild(mine);
		
	
	Sequence* sequence1 = new Sequence();
	mine->addChild(sequence1);
			
	sequence1->addChild(new PossessableFalse());
	sequence1->addChild(new HandEmptyQuery());
	Sequence* search = new Sequence();
	sequence1->addChild(search);

				
	search->addChild(new GetRandomPosition());
	search->addChild(new FindPath());
	search->addChild(new FollowPathSearching());
				
	sequence1->addChild(new FindPath());
	sequence1->addChild(new FollowPath());
	sequence1->addChild(new Collect());
			
	Sequence* sequence2 = new Sequence();
	mine->addChild(sequence2);
	
	sequence2->addChild(new PossessableFalse());
	sequence2->addChild(new HandFullQuery());
	sequence2->addChild(new GetStartPosition());
	sequence2->addChild(new FindPath());
	sequence2->addChild(new FollowPath());
	sequence2->addChild(new Place());
			
		

	Selector* possessed = new Selector();
	root->addChild(possessed);

	possessed->addChild(new PossessableFalse());
	possessed->addChild(new GhostTooFar());

	Sequence* sequence4 = new Sequence();
	possessed->addChild(sequence4);
			
	sequence4->addChild(new HandFullQuery());
	sequence4->addChild(new GetRandomPosition());
	sequence4->addChild(new FindPath());
	sequence4->addChild(new FollowPath());
	sequence4->addChild(new PlacePossessed());
			
	Sequence* sequence5 = new Sequence();
	possessed->addChild(sequence5);
			
	sequence5->addChild(new HandEmptyQuery());
	sequence5->addChild(new GetStartPosition());
	sequence5->addChild(new FindPath());
	sequence5->addChild(new FollowPath());
	sequence5->addChild(new Collect());
			
		
	
}

void Miner::CreateString()
{
	//Creating the string to be displayed while this agent is selected
	string behaviour = ((BlackboardStringType*)(blackboard.find(BehaviourNameKey)->second))->GetValue();
	bool handFull = ((BlackboardBoolType*)(blackboard.find(HandFullKey)->second))->GetValue();
	bool possessable = ((BlackboardBoolType*)(blackboard.find(PossessableKey)->second))->GetValue();

	string handFullString;
	string possessableString;
	if (handFull)
		handFullString = "True";
	else
		handFullString = "False";

	if (possessable)
		possessableString = "True";
	else
		possessableString = "False";


	string output = "Selected Agent: Miner \nCurrent Behaviour: " + behaviour + "\nHand Full: " + handFullString + "\nIs Possessable: " + possessableString;
	HUDInformation = output;
}

