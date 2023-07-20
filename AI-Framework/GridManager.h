#pragma once
#include "Constants.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "MapObject.h"

struct Cell
{
	//Set up a new cell
	Cell(int u, int v) : xPos(u), yPos(v) {}

	//Get the neighbour of this cell in a given direction
	Cell* GetNeighbour(int dir) {return neighbours[dir];}

	//Checks given coordinates compared to cell postition
	bool isMatch(int x, int y) { return x == xPos && y == yPos; }

	//Get the centre of this cell
	sf::Vector2f GetOrigin() { return sf::Vector2f{ (float)(xPos * Width) + HalfWidth, (float)(yPos * Height) + HalfHeight }; }

	const int Width = MaxCellWidth;
	const int Height = MaxCellHeight;
	const float HalfWidth = Width / 2;
	const float HalfHeight = Height / 2;

	bool visited = false;
	bool checked = false;

	int xPos;
	int yPos;

	bool highlighted = false;
	bool endPoint = false;
	bool startPoint = false;
	bool path = false;

	//The cost within the current path to get to this cell
	double cost = 0.0;

	//The cost to get to that cell
	float weighting = 1.0f;

	std::vector<Cell*> neighbours;
	Cell* parent = nullptr;
};


class GridManager
{
public:
	GridManager(bool backgroundGrid);
	sf::Vector2f GetNearestCellPos(sf::Vector2f);
	Cell* GetNearestCell(sf::Vector2f);
	std::vector<Cell*> AStar(float startX, float startY, float endX, float endY);

	int Manhattan(Cell* start, Cell* end);

	Cell* FindGridCell(int x, int y) {
		return gridCells[x][y];
	}

	int RowSize;
	int ColSize;

	void Draw(sf::RenderWindow& window);

	void ResetCells();

	bool AllCollected();
	bool AllPlaced();

	std::vector<MapObject*> GetHauntedObject() { return hauntedObjects; }
	BaseEntity* GetMinecart() { return mineCart; };



private:
	std::vector<std::vector<Cell*>> gridCells;
	sf::RectangleShape cellShape;
	std::vector<MapObject*> hauntedObjects;
	MapObject* mineCart;
};

