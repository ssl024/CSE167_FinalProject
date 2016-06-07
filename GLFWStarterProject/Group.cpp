#include "Group.h"

Group::Group() {}

Group::~Group() {}

void Group::addChild(Node* child)
{
	children.push_back(child);
}

void Group::removeChild(Node* targetChild)
{
	// Iterate through the children and remove child if found
	std::vector<Node*>::iterator it = children.begin();
	for (; it != children.end(); ++it)
	{
		if (*it == targetChild)
		{
			// Remove target child from children list
			children.erase(it);

			// Stop looping
			break;
		}
	}
}

void Group::update(glm::mat4 C)
{
	// Loop through all children and call update(C)
	for each (Node* child in children)
	{
		child->update(C);
	}
}

void Group::draw()
{
	for each (Node* child in children)
	{
		child->draw();
	}
}