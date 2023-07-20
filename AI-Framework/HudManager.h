#pragma once
#include "BaseEntity.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "GridManager.h"
#include <string>


using namespace std;

class HudManager
{
private:
	vector<BaseEntity*> agents;
	int agentCounter;
	sf::RectangleShape textBackground;
	string outputText;
	sf::RenderWindow* window;
	GridManager* backgroundGrid_;
	sf::Font font;
	sf::Text text;

public:
	HudManager(vector<BaseEntity*> agents_, int boxPosX, int boxPosY, int boxWidth, int boxHeight, sf::Color colour, sf::RenderWindow* window_, string fontFileDirectory, int textCharacterSize, sf::Color textColour, int textX, int textY);
	void IterateAgent(bool right);
	void DrawGrid();
	void DrawHUD();
};