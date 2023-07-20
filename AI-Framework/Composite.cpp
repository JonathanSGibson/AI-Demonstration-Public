#include "Composite.h"

void Composite::removeChild(Behaviour* child)
{
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i] == child)
		{
			children.erase(children.begin() + i);
		}
	}
}