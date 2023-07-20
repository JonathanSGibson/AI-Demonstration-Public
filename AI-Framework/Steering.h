#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class BaseEntity;

class Steering
{
protected:
	int counter = 0;
public:
	virtual bool Steer(BaseEntity* agent, float weighting = 1.0f, bool onHUD = false);
};

class Wander :public Steering {
private:
	std::pair<float, float> targetPosStore;
	sf::RenderWindow* window;
	sf::CircleShape circle{ CircleRadius };
	sf::CircleShape target{ 5.0f };

public:
	Wander(sf::RenderWindow* window_);
	bool Steer(BaseEntity* agent, float weighting = 1.0f, bool onHUD = false);

};

class Seek :public Steering {
public:
	bool Steer(BaseEntity* agent, float weighting = 1.0f, bool onHUD = false);
	Seek(BaseEntity* target_, sf::RenderWindow* window_) { target = target_; window = window_; }
private:
	BaseEntity* target;
	sf::RenderWindow* window;
};


class Flee :public Steering {
public:
	bool Steer(BaseEntity* agent, float weighting = 1.0f, bool onHUD = false);
	Flee(BaseEntity* target_, sf::RenderWindow* window_);
private:
	BaseEntity* target;
	sf::RenderWindow* window;
	sf::CircleShape fleeCircle{ FleeDetectionRadius };
};







//Steering* behaviour;
//behaviour.steer();
//use vector of pairs to include weightings