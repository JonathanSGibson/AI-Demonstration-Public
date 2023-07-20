#include <iostream>
#include <SFML/Graphics.hpp>

#include "UtilRandom.h"
#include "Constants.h"
#include "GridManager.h"
#include "Rat.h"
#include "Ghost.h"
#include "Miner.h"
#include "HudManager.h"
#include <iostream>

int main()
{
	// construct a window
	sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "AI Framework");

	GridManager grid(false);

	//Creating my agents
	Miner* mineMiner = new Miner(&grid, &window);
	mineMiner->SetTextureFile("Assets\\Miner.png");
	mineMiner->Initialize(false);


	Ghost* mineGhost = new Ghost(&window, mineMiner, &grid);
	mineGhost->SetTextureFile("Assets\\Ghost.png");
	mineGhost->SetColour(sf::Color(255, 255, 255, 150));
	mineGhost->Initialize(false);

	Rat* rat = new Rat(mineMiner, mineGhost, &window);
	rat->SetTextureFile("Assets\\Rat.png");
	rat->Initialize(false);


	HudManager* HUD = new HudManager(vector<BaseEntity*>{mineMiner, mineGhost, rat}, 0, 0, ScreenWidth, 32*5, sf::Color(0,0,0, 100), &window, "Assets\\comic.ttf", 24, sf::Color::White, 0,0);

	//This is to set the starting agent as currently selected
	HUD->IterateAgent(true);
	HUD->IterateAgent(false);

	while (window.isOpen())
	{
		// poll for events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Right)
				{
					HUD->IterateAgent(true);
				}
				else if (event.key.code == sf::Keyboard::Left)
				{
					HUD->IterateAgent(false);
				}
			}
		}

		// clear the window before drawing anything to it
		window.clear();

		HUD->DrawGrid();

		// update all our entities
		for ( auto entity : BaseEntity::Renderables )
		{
			entity->Think();
		}

		for (auto entity : BaseEntity::Renderables)
		{
			entity->Update();
		}

		// and then draw them
		for (auto entity : BaseEntity::Renderables)
		{			
			window.draw(entity->GetSprite(), entity->getTransform());
		}

		HUD->DrawHUD();
		window.display();

	}

	return 0;
}
