#include "MapObject.h"
#include "GridManager.h"


MapObject::MapObject(std::string fileLocation, GridManager* grid_)
{
	grid = grid_;
	SetTextureFile(fileLocation);
	this->Initialize(true);
	this->SetPosToNearestCell();

}

void MapObject::SetPosToNearestCell()
{
	this->setPosition(grid->GetNearestCellPos(this->getPosition()));
}
///<summary> Does not work if default colour isn't white </summary>
/// <param name="visible"> - true means visible, false means transparent</param>
void MapObject::ToggleVisible(bool visible)
{
	if (visible)
		this->SetColour(sf::Color(255, 255, 255, 255));
	else
		this->SetColour(sf::Color(255, 255, 255, 0));
}
