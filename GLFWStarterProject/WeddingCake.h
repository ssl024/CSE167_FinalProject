#ifndef _WEDDINGCAKE_H_
#define _WEDDINGCAKE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "Geode.h"
#include "MatrixTransform.h"

class WeddingCake {
public:
	static const int NUM_LEVELS = 3;
	static const int NUM_POLES = 3;
	static const int NUM_ARMS = 9;
	static const int NUM_EXT_ARMS = 9;
	static const int NUM_PODS = 27;
	static const int NUM_FINGERS = 27;
	static const int NUM_EXT_FINGERS = 27;
	static const int NUM_EXT_FINGERS_AND_PODS = 27;
	static const int NUM_HAND_AND_EXT_ARM = 27;
	static const int NUM_HANDS = 9;
	static const int NUM_FINGER_AND_POD = 27;
	static const int NUM_ENTIRE_ARMS = 9;

	WeddingCake(OBJObject* cylinderObj, OBJObject* podObj, OBJObject* bearObj, GLuint shaderProgram);
	~WeddingCake();

	void update();
	void draw();

	glm::mat4 getBearCam();

	Group* getRoot();

private:
	// Objobjects
	OBJObject* cylinderObj;
	OBJObject* podObj;
	OBJObject* bearObj;

	// Which shader program to use
	GLuint shaderProgram;

	// Animation fields
	float podAngle = 0.0f;
	float handAngle = 0.0f; // Spinning hand
	float levelAngle = 360.0f; // Spinning levels
	float fingerDistance = 0.0f; // Extending fingers
	float fingerDirection = 1.0f;
	float armDistance = 0.0f; // Extending arms
	float armDirection = 1.0f;
	float levelDistance = 0.0f; // Extending levels
	float levelDirection = 1.0f;

	// Rotate matrices
	// Rotate matrices
	glm::mat4 rotate_90_CWZ = glm::rotate(glm::mat4(1.0f), -90.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)); // Turn on side
	glm::mat4 rotate_finger_CWZ = glm::rotate(glm::mat4(1.0f), -90.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)); // (Slight turn on side)
	glm::mat4 rotate_90_CCWY = glm::rotate(glm::mat4(1.0f), 90.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around pole
	glm::mat4 rotate_third_CCWY; // Rotate 0, 120, or 240 around y

								 // Scale matrices
	glm::mat4 scale_stretch_arm_cylinder = glm::scale(glm::mat4(1.0f), glm::vec3(2.5f, 0.25f, 0.25f)); // Stretch
	glm::mat4 scale_stretch_finger_cylinder = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.2f, 0.2f)); // Stretch small
	glm::mat4 scale_pole_cylinder = glm::scale(glm::mat4(1.0f), glm::vec3(0.7f, 5.0f, 0.7f)); // Stretch pole

																							  // Translate matrices
	glm::mat4 translate_arm_cylinder = glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 0.0f, 0.0f)); // Move arm cylinder out of origin
	glm::mat4 translate_finger_cylinder = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Move finger out of origin (and slightly up)
	glm::mat4 translate_pod = glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 0.0f, 0.0f)); // Move pod to tip of finger (and slightly up)
	glm::mat4 translate_hand = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f)); // Move hand to tip of ext arm
	glm::mat4 translate_pole_cylinder; // Move pole up one level
	glm::mat4 translate_bear = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f)); // Change how bear sits in pod

	Geode* bear;
	Geode* pole[NUM_POLES] = { 0 };
	Geode* arm[NUM_ARMS] = { 0 };
	Geode* extArm[NUM_EXT_ARMS] = { 0 };
	Geode* finger[NUM_FINGERS] = { 0 };
	Geode* extFinger[NUM_EXT_FINGERS] = { 0 };
	Geode* pod[NUM_PODS] = { 0 };

	// Finger Transform
	MatrixTransform* fingerTranslate[NUM_FINGERS] = { 0 };
	MatrixTransform* fingerScale[NUM_FINGERS] = { 0 };
	MatrixTransform* fingerRotate[NUM_FINGERS] = { 0 };
	// Ext Finger Transform
	MatrixTransform* extFingerTranslate[NUM_EXT_FINGERS] = { 0 };
	MatrixTransform* extFingerScale[NUM_EXT_FINGERS] = { 0 };
	MatrixTransform* extFingerRotate[NUM_EXT_FINGERS] = { 0 };
	// Bear Transform
	MatrixTransform* bearTranslate;
	// Pod Transform
	MatrixTransform* podTranslate[NUM_PODS] = { 0 };
	MatrixTransform* podRotate[NUM_PODS] = { 0 };
	// Ext Finger and Pod Transform
	MatrixTransform* extFingerAndPodTranslate[NUM_EXT_FINGERS_AND_PODS];
	// Finger and Pod Transform
	MatrixTransform* fingersAndPodRotate[NUM_FINGER_AND_POD] = { 0 };
	// Hand Transform
	MatrixTransform* handTranslate[NUM_HANDS] = { 0 };
	MatrixTransform* handRotate[NUM_HANDS] = { 0 };
	// Arm Transform
	MatrixTransform* armTranslate[NUM_ARMS] = { 0 };
	MatrixTransform* armScale[NUM_ARMS] = { 0 };
	MatrixTransform* armRotate[NUM_ARMS] = { 0 };
	// Ext Arm Transform
	MatrixTransform* extArmTranslate[NUM_EXT_ARMS] = { 0 };
	MatrixTransform* extArmScale[NUM_EXT_ARMS] = { 0 };
	MatrixTransform* extArmRotate[NUM_EXT_ARMS] = { 0 };
	// Hand and Ext Arm Transform
	MatrixTransform* handAndExtArm[NUM_HAND_AND_EXT_ARM] = { 0 };
	// Entire Arm Transform
	MatrixTransform* entireArm[NUM_ENTIRE_ARMS] = { 0 };
	// Pole Transform
	MatrixTransform* poleTranslate[NUM_POLES] = { 0 };
	MatrixTransform* poleScale[NUM_POLES] = { 0 };
	// Level Transform
	MatrixTransform* levelTranslate[NUM_LEVELS] = { 0 };
	MatrixTransform* levelRotate[NUM_LEVELS] = { 0 };

	// Root Group
	Group* root;
	// Root transform
	MatrixTransform* rootTranslate;

	// Builds the Wedding Cake
	void initialize();

};

#endif