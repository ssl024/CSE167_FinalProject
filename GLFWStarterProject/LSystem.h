#ifndef _LSYSTEM_H_
#define _LSYSTEM_H_
#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <vector>

#include "Window.h"
#include "ShaderProgram.h"
using namespace std;


class LSystem
{
private:
public:
	vector<glm::vec3> points;
	string pa, pb;
	stack<glm::vec3> spos;
	float global;
	stack<float> sdir;
	stack<float> sdirz;
	float angle;
	float anglez;
	glm::vec3 pos;
	float offx, offy, offz;
	float am;
	float brnw;
	float grnw;
	float bm;
	void LSystem::turnz(int pos);
	vector<glm::vec3> brnpoints;
	static int width;
	LSystem(string ax, string rule1, string rule2,float itr, float ang, float x, float y, float z,float mag,float brn,float grn);
	void LSystem::drawA(int z);
	void LSystem::adjust(float x, float y, float z);
	void LSystem::drawB(int z);
	void LSystem::forward(float x, int z,int y);
	void LSystem::turn();
	void LSystem::turn(int pos);
	void LSystem::draw();
	void LSystem::redraw();
};

#endif
