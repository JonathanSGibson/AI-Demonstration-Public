#include "Rat.h"
#include <string>;

Rat::Rat(BaseEntity* miner_, BaseEntity* ghost_, sf::RenderWindow* window)
{
	miner = miner_;
	ghost = ghost_;
	Wander* wander = new Wander(window);
	Flee* minerFlee = new Flee(miner, window);
	Flee* ghostFlee = new Flee(ghost, window);
	steerings.push_back(std::pair<Steering*, float >(wander, 4.0f));
	steerings.push_back(std::pair<Steering*, float >(minerFlee, 1.0f));
	steerings.push_back(std::pair<Steering*, float >(ghostFlee, 1.0f));
}

void Rat::Think()
{
	//Update the weightings of the 2 flees as they are dynamic
	steerings[1].second = GetWeighting(miner);
	steerings[2].second = GetWeighting(ghost);

	//Go through each of the rat's steerings
	for (int i = 0; i < steerings.size(); i++)
	{
		steerings[i].first->Steer(this, steerings[i].second, onHUD);
	}

	std::string output;
	output = "Selected Agent: Rat\n Weightings:\n   Wander: " + std::to_string(steerings[0].second) + "\n   Flee (Miner): " + std::to_string(steerings[1].second) + "\n   Flee (Ghost): " + std::to_string(steerings[2].second);
	HUDInformation = output;

}

float Rat::GetWeighting(BaseEntity* agent)
{
	float dist = Distance(this->getPosition(), agent->getPosition());
	if (dist <= FleeDetectionRadius)
	{
		//Invert the value so closer values are higher
		float newWeight = FleeDetectionRadius - dist;

		//Divide by RatFleeRadius to get a value from 0 to 1
		newWeight /= FleeDetectionRadius;

		//Multiply that by an arbitrary multiplier to make it larger
		newWeight *= FleeDetectionRadius;
		return newWeight;
	}

	return 0.0f;
}
