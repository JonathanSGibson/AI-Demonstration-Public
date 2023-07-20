#pragma once
#include <SFML/Graphics.hpp>
#include <vector>



class GridManager;
class Cell;
class BaseEntity;

using namespace sf;
using namespace std;


//The blackboard base type acts as a generic type to pass data into the blackboard, the other types inherit from it and store data of a specific value

class BlackboardBaseType
{
public:
	BlackboardBaseType() {}
	virtual ~BlackboardBaseType() {}
};

class BlackboardBoolType : public BlackboardBaseType
{
public:
	BlackboardBoolType(bool val) : value(val) {}
	bool GetValue() { return value; }
	void SetValue(bool val) { value = val; }
private:
	bool value;
};

class BlackboardGridManagerType : public BlackboardBaseType
{
public:
	BlackboardGridManagerType(GridManager* val) : value(val) {}
	GridManager* GetValue() { return value; }
	void SetValue(GridManager* val) { value = val; }
private:
	GridManager* value;
};


class BlackboardIntType : public BlackboardBaseType
{
public:
	BlackboardIntType(int val) : value(val) {}
	int GetValue() { return value; }
	void SetValue(int val) { value = val; }
private:
	int value;
};

class BlackboardFloatType : public BlackboardBaseType
{
public:
	BlackboardFloatType(float val) : value(val) {}
	float GetValue() { return value; }
	void SetValue(float val) { value = val; }
private:
	float value;
};

class BlackboardVector2fType : public BlackboardBaseType
{
public:
	BlackboardVector2fType(Vector2f* val) : value(val) {}
	Vector2f* GetValue() { return value; }
	void SetValue(Vector2f* val) { value = val; }
private:
	Vector2f* value;
};

class BlackboardVectorCellType : public BlackboardBaseType
{
public:
	BlackboardVectorCellType(vector<Cell*> val) : value(val) {}
	vector<Cell*> GetValue() { return value; }
	void SetValue(vector<Cell*> val) { value = val; }
private:
	vector<Cell*> value;
};

class BlackboardBaseEntityType : public BlackboardBaseType
{
public:
	BlackboardBaseEntityType(BaseEntity* val) : value(val) {};
	BaseEntity* GetValue() { return value; }
	void SetValue(BaseEntity* val) { value = val; }
private:
	BaseEntity* value;
};

class BlackboardVectorBaseEntityType : public BlackboardBaseType
{
public:
	BlackboardVectorBaseEntityType(vector<BaseEntity*> val) : value(val) {};
	vector<BaseEntity*> GetValue() { return value; }
	void SetValue(vector<BaseEntity*> val) { value = val; }
private:
	vector<BaseEntity*> value;
};

class BlackboardStringType : public BlackboardBaseType
{
public:
	BlackboardStringType(const char* val) : value(val) {};
	const char* GetValue() { return value; }
	void SetValue(const char* val) { value = val; }
private:
	const char* value;
};

class BlackboardWindowType : public BlackboardBaseType
{
public:
	BlackboardWindowType(sf::RenderWindow* val) : value(val) {};
	sf::RenderWindow* GetValue() { return value; }
	void SetValue(sf::RenderWindow* val) { value = val; }
private:
	sf::RenderWindow* value;
};
