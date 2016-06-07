#include "LSystem.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <time.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <vector>
using namespace std;
string start;
glm::mat4 dum(1.0f);
GLuint turt, turtVBO;
GLuint brnVAO, brnVBO;
GLint s, s2, s3, s4, s5, s6;
string pattern;
int counter = 0;
float inc = 30;
#define PI 3.14159265
LSystem::LSystem(string ax, string rule1, string rule2, float itr, float ang, float x, float y, float z, float mag, float brn, float grn)
{
	start = ax;
	pa = rule1;
	am = 0.5 * mag;
	bm = mag;
	offx = x;
	offy = y;
	offz = z;
	brnw = brn;
	grnw = grn;
	angle = 90;
	anglez = 90;
	global = itr;
	inc = ang;
#ifdef _WIN32 // Windows (both 32 and 64 bit versions)
	s3 = (new ShaderProgram("shader3.vert", "shader3.frag"))->getShaderProgram();
	s4 = (new ShaderProgram("shader4.vert", "shader4.frag"))->getShaderProgram();
#else // Not windows
	s = LoadShaders("shader.vert", "shader.frag");
	s2 = LoadShaders("shader2.vert", "shader2.frag");
	s3 = LoadShaders("shader3.vert", "shader3.frag");
	s4 = LoadShaders("../shader4.vert", "../shader4.frag");
	s5 = LoadShaders("../shader5.vert", "../shader5.frag");
	s6 = LoadShaders("../shader6.vert", "../shader6.frag");
#endif
	pb = rule2;
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &turt);
	glGenBuffers(1, &turtVBO);
	glBindVertexArray(turt);
	glBindBuffer(GL_ARRAY_BUFFER, turtVBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]), &points[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &brnVAO);
	glGenBuffers(1, &brnVBO);
	glBindVertexArray(brnVAO);
	glBindBuffer(GL_ARRAY_BUFFER, brnVBO);
	glBufferData(GL_ARRAY_BUFFER, brnpoints.size() * sizeof(brnpoints[0]), &brnpoints[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	char tt;
	for (int c = 0; c < start.size(); c++)
	{
		tt = start[c];
		if (tt == 'A')
		{
			drawA(0);
		}
		if (tt == 'B')
		{
			drawB(0);
		}
	}
	adjust(x, y, z);
}
void LSystem::adjust(float xx, float yy, float zz)
{
	for (int i = 0; i < points.size(); i++)
	{
		points[i].x += xx;
		points[i].y += yy;
		points[i].z += zz;
	}
	for (int i = 0; i < brnpoints.size(); i++)
	{
		brnpoints[i].x += xx;
		brnpoints[i].y += yy;
		brnpoints[i].z += zz;
	}
}
void LSystem::redraw()
{
	angle = 90;
	anglez = 90;
	points.clear();
	brnpoints.clear();
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	drawA(0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &turt);
	glGenBuffers(1, &turtVBO);
	glBindVertexArray(turt);
	glBindBuffer(GL_ARRAY_BUFFER, turtVBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]), &points[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &brnVAO);
	glGenBuffers(1, &brnVBO);
	glBindVertexArray(brnVAO);
	glBindBuffer(GL_ARRAY_BUFFER, brnVBO);
	glBufferData(GL_ARRAY_BUFFER, brnpoints.size() * sizeof(brnpoints[0]), &brnpoints[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	char tt;
	for (int c = 0; c < start.size(); c++)
	{
		tt = start[c];
		if (tt == 'A')
		{
			drawA(0);
		}
		if (tt == 'B')
		{
			drawB(0);
		}
	}
	adjust(offx, offy, offz);
}
void LSystem::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, brnVBO);
	glBufferData(GL_ARRAY_BUFFER, brnpoints.size() * sizeof(brnpoints[0]), &brnpoints[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(s4);
	glm::mat4 MVP = Window::P * Window::V * dum;
	GLint modelLoc = glGetUniformLocation(s3, "model");
	GLuint MatrixID = glGetUniformLocation(s3, "MVP");

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glBindVertexArray(brnVAO);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &dum[0][0]);
	glLineWidth(brnw);
	glDrawArrays(GL_LINES, 0, brnpoints.size());
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, turtVBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]), &points[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(s3);
	MVP = Window::P * Window::V * dum;
	modelLoc = glGetUniformLocation(s4, "model");
	MatrixID = glGetUniformLocation(s4, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glBindVertexArray(turt);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &dum[0][0]);
	glLineWidth(grnw);
	glDrawArrays(GL_LINES, 0, points.size());
	glBindVertexArray(0);
}
void LSystem::drawA(int z)
{
	char t;
	for (int i = 0; i < pa.size(); i++)
	{
		t = pa[i];
		if (t == 'A')
		{
			if (z < global)
			{
				z += 1;
				//cout << "Az = " << z << endl;
				drawA(z + 1);
			}
			else
			{
				//cout << "Aa" << endl;
				forward(am / global, 1, z);
			}
		}
		if (t == 'B')
		{
			if (z < global)
			{
				z += 1;
				//cout << "Bz = " << z << endl;
				drawB(z + 1);
			}
			else
			{
				//cout << "Ba" << endl;
				forward(bm / global, 0, z);
			}
		}
		if (t == '-')
		{
			turn(0);
		}
		if (t == '+')
		{
			turn(1);
		}
		if (t == '[')
		{
			spos.push(pos);
			sdirz.push(anglez);
			sdir.push(angle);
		}
		if (t == ']')
		{
			pos = spos.top();
			angle = sdir.top();
			anglez = sdirz.top();
			spos.pop();
			sdirz.pop();
			sdir.pop();
			//points.push_back(pos);
			//cout << "adding(popped) " << pos.x << " " << pos.y << " " << pos.z << endl;
		}
	}
}
void LSystem::drawB(int z)
{
	char t;
	for (int i = 0; i < pb.size(); i++)
	{
		t = pb[i];
		if (t == 'A')
		{
			if (z < global)
			{
				//cout << "z = " << z << endl;
				drawA(z + 1);
			}
			else
			{
				//cout << "Ab" << endl;
				forward(am / global, 1, z);
			}
		}
		if (t == 'B')
		{
			if (z < global)
			{
				drawB(z + 1);
			}
			else
			{
				//cout << "Bb" << endl;
				forward(bm / global, 0, z);
			}
		}
		if (t == '-')
		{
			turn(0);
		}
		if (t == 'x')
		{
			turnz(1);
		}
		if (t == '/')
		{
			turnz(0);
		}
		if (t == '+')
		{
			turn(1);
		}
		if (t == '[')
		{
			spos.push(pos);
			sdir.push(angle);
		}
		if (t == ']')
		{
			pos = spos.top();
			angle = sdir.top();
			spos.pop();
			sdir.pop();
			//points.push_back(pos);
			//cout << "pushed" << endl;
		}
	}
}
void LSystem::forward(float x, int q, int z)
{
	if (q == 0)
	{
		points.push_back(pos);
		//cout << "adding " << pos.x << " " << pos.y << " " << pos.z << endl;
		pos.x += cos(angle * PI / 180.0)*x;
		pos.y += sin(angle * PI / 180.0)*x;
		pos.z += cos(anglez * PI / 180.0) * x;
		points.push_back(pos);
		//cout << "adding " << pos.x << " " << pos.y << " " << pos.z << endl;
		//draw();
	}
	else
	{
		brnpoints.push_back(pos);
		//cout << "adding " << pos.x << " " << pos.y << " " << pos.z << endl;
		pos.x += cos(angle * PI / 180.0)*x;
		pos.y += sin(angle * PI / 180.0)*x;
		pos.z += cos(anglez * PI / 180.0) * x;
		brnpoints.push_back(pos);
		//cout << "adding " << pos.x << " " << pos.y << " " << pos.z << endl;
		//draw();
	}
}
void LSystem::turn(int pos)
{
	srand(time(NULL));
	int v1 = rand() % 60;
	if (pos == 1)
		angle += inc + v1 - 30;
	else
		angle -= inc + v1 - 30;
	if (angle >= 360)
	{
		angle = angle - 360;
	}
	else if (angle < 0)
	{
		angle += 360;
	}
}
void LSystem::turnz(int pos)
{
	srand(time(NULL));
	int v1 = rand() % 60;
	if (pos == 1)
		anglez += inc + v1 - 30;
	else
		anglez -= inc + v1 - 30;
	if (anglez >= 360)
	{
		anglez = anglez - 360;
	}
	else if (angle < 0)
	{
		anglez += 360;
	}
}