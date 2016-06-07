#ifndef _GROUP_H
#define _GROUP_H

#include "Node.h"

class Group : public Node {

private:
	std::vector<Node *> children;

public:
	Group();
	~Group();

	void addChild(Node* child);
	void removeChild(Node* child);

	virtual void update(glm::mat4 C);

	virtual void draw();
};

#endif