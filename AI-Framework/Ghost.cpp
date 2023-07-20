#include "Ghost.h"
#include "Constants.h"
#include "GridManager.h"
#include "Miner.h"

Ghost::Ghost(sf::RenderWindow* window, BaseEntity* miner_, GridManager* grid_)
{
	miner = miner_;
	grid = grid_;
	wander = new Wander(window);
	flee = new Flee(miner, window);
	seek = new Seek(miner, window);
	currentState = new WanderState(this);
	((Miner*)miner)->SetGhost(this);

	// Runs enter for initial state
	currentState->Enter();
}

bool Ghost::GetCollected()
{
	return grid->AllCollected();
}

bool Ghost::GetAllPlaced()
{
	return grid->AllPlaced();
}

bool Ghost::IsInPosessionRange()
{
	return CloseEnough(this->getPosition(), miner->getPosition(), PosessionRange);
}

void Ghost::Think()
{
	if (currentState != nullptr)
	{
		IState* newState = currentState->Update();
		if (newState != nullptr)
		{
			currentState->Exit();
			newState->Enter();
			currentState = newState;
		}
	}
}

void WanderState::Enter()
{
	std::string output = "Selected Agent: Ghost\nCurrent State: Wandering\nWeightings:\n   Wander: " + std::to_string(WanderWeighting) + "\n   Flee: " + std::to_string(FleeWeighting);
	ghost->SetHUDString(output);
}

WanderState::WanderState(BaseEntity* ghost_)
{
	ghost = (Ghost*)ghost_;
}

IState* WanderState::Update()
{
	if (ghost->GetCollected())
		return new HuntState(ghost);
	ghost->GetWander()->Steer(ghost, WanderWeighting, ghost->GetOnHUD());
	ghost->GetFlee()->Steer(ghost, FleeWeighting, ghost->GetOnHUD());
	return nullptr;
}

void WanderState::Exit()
{
	delete(this);
}

HuntState::HuntState(BaseEntity* ghost_)
{
	ghost = (Ghost*)ghost_;
}

void HuntState::Enter()
{
	std::string output = "Selected Agent: Ghost\nCurrent State: Hunting";
	ghost->SetHUDString(output);
}

IState* HuntState::Update()
{
	if (ghost->IsInPosessionRange())
		return new PossessionState(ghost);
	ghost->GetSeek()->Steer(ghost, 1.0f, ghost->GetOnHUD());
	return nullptr;
}

void HuntState::Exit()
{
	delete(this);
}

PossessionState::PossessionState(BaseEntity* ghost_)
{
	ghost = (Ghost*)ghost_;
}

void PossessionState::Enter()
{
	std::string output = "Selected Agent: Ghost\nCurrent State: Possessing";
	ghost->SetHUDString(output);
}

IState* PossessionState::Update()
{
	if (ghost->GetAllPlaced())
		return new WanderState(ghost);
	ghost->setPosition(ghost->getMiner()->getPosition());
	return nullptr;
}

void PossessionState::Exit()
{
	delete (this);
}
