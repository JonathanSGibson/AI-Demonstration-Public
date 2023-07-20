#include "GridManager.h"
#include <iostream>
#include <stack>
#include <queue>
#include "Constants.h"

GridManager::GridManager(bool backgroundGrid)
{
	//Create the grid of cells

	int rowWidth = (ScreenWidth / MaxCellWidth);
	int rowHeight = (ScreenHeight / MaxCellHeight);

	RowSize = rowWidth;
	ColSize = rowHeight;

	for (int i = 0; i < rowHeight; ++i)
	{
		gridCells.push_back(std::vector<Cell*>());
		for (int j = 0; j < rowWidth; ++j) {
			gridCells[i].push_back(new Cell(j, i));
		}
	}

	for (int i = 0; i < rowHeight; ++i)
	{
		for (int j = 0; j < rowWidth; ++j) {

			if (i > 0) // top neighbour
				gridCells[i][j]->neighbours.push_back(gridCells[i - 1][j]);
			else
				gridCells[i][j]->neighbours.push_back(nullptr);

			if (i + 1 <= rowHeight - 1) // bottom neighbour
				gridCells[i][j]->neighbours.push_back(gridCells[i + 1][j]);
			else
				gridCells[i][j]->neighbours.push_back(nullptr);

			if (j + 1 <= rowWidth - 1) // right neighbour
				gridCells[i][j]->neighbours.push_back(gridCells[i][j + 1]);
			else
				gridCells[i][j]->neighbours.push_back(nullptr);

			if (j > 0) // left neighbour
				gridCells[i][j]->neighbours.push_back(gridCells[i][j - 1]);
			else
				gridCells[i][j]->neighbours.push_back(nullptr);
		}
	}

	cellShape.setOutlineColor(sf::Color(50, 50, 50));
	cellShape.setOutlineThickness(1.0f);
	cellShape.setFillColor(sf::Color::Transparent);
	cellShape.setSize(sf::Vector2f(MaxCellWidth, MaxCellHeight));

	if (!backgroundGrid)
	{
		mineCart = new MapObject("Assets\\Minecart.png", this);

		for (int i = 0; i < HauntedObjectTotal; i++)
		{
			hauntedObjects.push_back(new MapObject("Assets\\HauntedObject.png", this));
		}
	}

}

sf::Vector2f GridManager::GetNearestCellPos(sf::Vector2f location)
{
	int column = floor(location.x / MaxCellWidth);
	int row = floor(location.y / MaxCellHeight);

	Cell* nearestCell = gridCells[row][column];
	return nearestCell->GetOrigin();
}

Cell* GridManager::GetNearestCell(sf::Vector2f location)
{
	int column = floor(location.x / MaxCellWidth);
	int row = floor(location.y / MaxCellHeight);

	Cell* nearestCell = gridCells[row][column];
	return nearestCell;
}

std::vector<Cell*> GridManager::AStar(float startX, float startY, float endX, float endY)
{
	ResetCells();

	Cell* startCell = GetNearestCell(sf::Vector2f(startX, startY));
	Cell* endCell = GetNearestCell(sf::Vector2f(endX, endY));

	std::vector<Cell*> path;

	//Setting the values of the start and end cell, then returning a path of the single cell if the start and end are the same cell
	startCell->startPoint = true;
	endCell->endPoint = true;
	startCell->cost = 0;
	if (startCell == endCell)
	{
		path.push_back(startCell);
		return path;
	}

	//Create a priority queue based on the cost of the cells
	auto cmp = [](Cell* left, Cell* right) {return left->cost > right->cost; };
	std::priority_queue<Cell*, std::vector<Cell*>, decltype(cmp)> cellQueue(cmp);

	cellQueue.push(startCell);

	bool found = false;

	while (!cellQueue.empty())
	{
		//Remove the lowest cost cell (the cell we're currently using) from the queue
		Cell* currentCell = cellQueue.top();
		cellQueue.pop();

		//Cycle through every neighbour, calculate the cost, set the parent and add it to the queue
		for (Cell* currentNeighbour : currentCell->neighbours)
		{
			if (currentNeighbour == nullptr)
				continue;
			else if (currentNeighbour == endCell)
			{
				//If the current neighbour is the end we no longer need to keep searching
				endCell->parent = currentCell;
				found = true;
				break;
			}

			// f(n) = g(n) + h(n)
			// f(n) is this cells cost
			// g(n) is the cost to get to this cell
			// h(n) is the manhatten distance from this cell to the end
			double tempCost = (currentCell->cost + currentNeighbour->weighting) + Manhattan(currentNeighbour, endCell);

			if (tempCost < currentNeighbour->cost)
			{
				currentNeighbour->cost = tempCost;
				currentNeighbour->parent = currentCell;
			}

			if (!currentNeighbour->visited && !currentNeighbour->checked)
			{
				currentNeighbour->checked = true;
				cellQueue.push(currentNeighbour);
			}
		}

		currentCell->visited = true;

		if (found)
			break;
	}

	if (!found)
		return path;
	
	Cell* currentCell = endCell;

	//Add all cells to our path
	while (currentCell != nullptr)
	{
		currentCell->path = true;
		path.push_back(currentCell);
		currentCell = currentCell->parent;
	}

	startCell->path = false;
	endCell->path = false;

	//Reverse path (as it currently starts at the end)
	std::reverse(path.begin(), path.end());
	return path;
}


int GridManager::Manhattan(Cell* start, Cell* end)
{
	int x = abs(start->xPos - end->xPos);
	int y = abs(start->yPos - end->yPos);
	return x + y;
}

void GridManager::Draw(sf::RenderWindow& window)
{
	for (std::vector<Cell*> row : gridCells)
	{
		for (Cell* cell : row)
		{
			cell->highlighted = false;
			cellShape.setFillColor(sf::Color::Transparent);

			for (Cell* neighbour : cell->neighbours) {
				if (neighbour == nullptr) continue;
				if (neighbour->highlighted)
				{
					cellShape.setFillColor(sf::Color::Yellow);
					break;
				}
			}

			if (cell->visited)
			{
				cellShape.setFillColor(sf::Color{0, 0, 255, 150});
			}

			if (cell->endPoint)
			{
				cellShape.setFillColor(sf::Color{ 0, 255, 0, 150 });
			}

			if (cell->startPoint)
			{
				cellShape.setFillColor( sf::Color{255, 0, 255, 150 });
			}

			if (cell->path)
			{
				cellShape.setFillColor(sf::Color{ 255, 255, 0, 150 });
			}
			cellShape.setPosition(cell->xPos * cell->Width, cell->yPos * cell->Height);
			window.draw(cellShape);
		}
	}
}

void GridManager::ResetCells()
{
	for (std::vector<Cell*> row : gridCells)
	{
		for (Cell* cell : row)
		{
			cell->visited = false;
			cell->checked = false;
			cell->highlighted = false;
			cell->endPoint = false;
			cell->startPoint = false;
			cell->path = false;
			cell->parent = nullptr;
			cell->cost = DBL_MAX;
		}
	}
}

bool GridManager::AllCollected()
{
	bool temp = true;
	for (MapObject* object : hauntedObjects)
	{
		if (!object->getCollected())
		{
			temp = false;
			break;
		}
	}
	return temp;
}

bool GridManager::AllPlaced()
{
	bool temp = true;
	for (MapObject* object : hauntedObjects)
	{
		if (object->getCollected())
		{
			temp = false;
			break;
		}
	}
	return temp;
}
