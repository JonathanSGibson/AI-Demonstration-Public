#include "Selector.h"
Status Selector::Update(map<const char*, BlackboardBaseType*> blackboard)
{
	// keep going until child says it is running
	while (true)
	{
		Status s = (*currentChild)->Tick(blackboard);
		//if a child is running or has failed, return the same
		if (s != BH_FAIL)
			return s;
		//keep going until we hit the end of the children
		if (++currentChild == children.end())
			return BH_FAIL;
	}
}
