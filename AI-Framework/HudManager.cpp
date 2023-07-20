#include "HudManager.h"
#include "Miner.h"

HudManager::HudManager(vector<BaseEntity*> agents_, int boxPosX, int boxPosY, int boxWidth, int boxHeight, sf::Color colour, sf::RenderWindow* window_, string fontFileDirectory, int textCharacterSize, sf::Color textColour, int textX, int textY)
{
	window = window_;

	agents = agents_;

	textBackground.setOrigin(0, 0);
	textBackground.setPosition(boxPosX, boxPosY);
	textBackground.setSize(sf::Vector2f(boxWidth, boxHeight));
	textBackground.setFillColor(colour);

	backgroundGrid_ = new GridManager(true);

	font.loadFromFile(fontFileDirectory);

	text.setFont(font);
	text.setCharacterSize(textCharacterSize);
	text.setFillColor(textColour);
	text.setOrigin(0, 0);
	text.setPosition(textX, textY);
}

void HudManager::IterateAgent(bool right)
{
	agents[agentCounter]->SetOnHUD(false);
	if (right)
	{
		agentCounter++;
		if (agentCounter >= agents.size())
			agentCounter = 0;
	}
	else
	{
		agentCounter--;
		if (agentCounter < 0)
			agentCounter = agents.size() - 1;
	}
	agents[agentCounter]->SetOnHUD(true);
}

void HudManager::DrawGrid()
{
	if (!agents[agentCounter]->GetIsMiner())
		backgroundGrid_->Draw(*window);
	else
		((Miner*)agents[agentCounter])->GetGrid()->Draw(*window);
}

void HudManager::DrawHUD()
{
	window->draw(textBackground);

	text.setString(agents[agentCounter]->GetHUDInformation());
	window->draw(text);
}
