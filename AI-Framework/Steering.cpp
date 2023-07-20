#include "Steering.h"
#include "BaseEntity.h"
#include <SFML/Graphics.hpp>

using namespace sf;

bool Seek::Steer(BaseEntity* agent, float weighting, bool onHUD)
{
    Vector2f force;

    if (counter >= 10)
    {
        counter = 0;
        //checks there is a target to seek
        if (target == nullptr)
            return false;

        //gets the direction towards the target
        Vector2f direction = agent->getPosition() - target->getPosition();


        float distance = Magnitude(direction);
        direction /= Magnitude(direction);

        force = agent->getVelocity() - direction;

        agent->applyForce(force * weighting);
    }
    counter++;

    if (onHUD)
    {
        //Visualises the force with a line
        sf::Vertex seekForce[] =
        {
            sf::Vertex(agent->getPosition(), sf::Color::Red),
            sf::Vertex(agent->getPosition() + (force * weighting * ForceLineMultiplier), sf::Color::Red)
        };

        window->draw(seekForce, 2, sf::Lines);
    }
    return true;
}


bool Flee::Steer(BaseEntity* agent, float weighting, bool onHUD)
{
    Vector2f force;
    if (counter >= 10)
    {
        counter = 0;
        //checks there is a target to seek
        if (target == nullptr)
            return false;

        //gets the direction towards the target
        Vector2f direction = agent->getPosition() - target->getPosition();


        float distance = Magnitude(direction);
        direction /= Magnitude(direction);

        if (distance < FleeDetectionRadius)//checks that the flee boid is within range of the one it is fleeing from
            force = direction - agent->getVelocity();

        agent->applyForce(force * weighting);
    }
    counter++;

    if (onHUD)
    {
        //Visualise the flee radius with a circle and the force with a line

        fleeCircle.setPosition(agent->getPosition());

        sf::Vertex fleeForce[] =
        {
            sf::Vertex(agent->getPosition(), sf::Color::Cyan),
            sf::Vertex(agent->getPosition() + (force * weighting * ForceLineMultiplier), sf::Color::Cyan)
        };

        window->draw(fleeForce, 2, sf::Lines);
        window->draw(fleeCircle);
    }
    return true;
}

Flee::Flee(BaseEntity* target_, sf::RenderWindow* window_)
{
    target = target_; 
    window = window_;

    fleeCircle.setOrigin(FleeDetectionRadius, FleeDetectionRadius);
    fleeCircle.setOutlineColor(sf::Color::Cyan);
    fleeCircle.setOutlineThickness(1);
    fleeCircle.setFillColor(sf::Color::Transparent);
}

Wander::Wander(sf::RenderWindow* window_)
{
    window = window_;

    circle.setOutlineColor(sf::Color::Red);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(1.0f);
    circle.setOrigin(CircleRadius, CircleRadius);

    target.setOutlineColor(sf::Color::Yellow);
    target.setFillColor(sf::Color::Yellow);
    target.setOutlineThickness(1.0f);
    target.setOrigin(5.0f, 5.0f);

}


//Get the centrepoint of a circle in front of the agent
//Randomly generate angle in circle
//Convert the angle to a vector, multiply by radius to make it a point on the circle
//Make a force from the agent to the target
//multiply by weighting
bool Wander::Steer(BaseEntity* agent, float weighting, bool onHUD)
{
    Vector2f circlePos;
    Vector2f targetPos;
    Vector2f force;
    
    //get the position of the circle relative to the agent
    circlePos.x = cos(agent->getRotation() * DegToRad);
    circlePos.y = sin(agent->getRotation() * DegToRad);
    circlePos /= Magnitude(circlePos);
    circlePos *= AgentToCentre;
    circlePos += agent->getPosition();

    if (counter >= 10) {
        counter = 0;

        //randomly generate a new angle on that circle
        float angle = UtilRandom::instance()->GetRange(0, 359);
        angle *= DegToRad;

        targetPos.x = cos(angle);
        targetPos.y = sin(angle);

        targetPos *= CircleRadius;

        targetPos += circlePos;

        targetPosStore.first = targetPos.x;
        targetPosStore.second = targetPos.y;

        //make the force towards the target point
        force = targetPos - agent->getPosition();
        force /= Magnitude(force);

        agent->applyForce(force * weighting);
    }
    counter++;

    if (onHUD)
    {
        // Visualises the wandering circle with a circle and the point on that circle with another, smaller circle. Also visualises the force with a line

        circle.setPosition(circlePos);
        window->draw(circle);

        target.setPosition(targetPosStore.first, targetPosStore.second);
        window->draw(target);

        sf::Vertex wanderForce[] =
        {
            sf::Vertex(agent->getPosition(), sf::Color::Yellow),
            sf::Vertex(agent->getPosition() + (force * weighting * ForceLineMultiplier), sf::Color::Yellow)
        };

        window->draw(wanderForce, 2, sf::Lines);
    }
    return true;
}

bool Steering::Steer(BaseEntity* agent, float weighting, bool onHUD)
{
    return false;
}
