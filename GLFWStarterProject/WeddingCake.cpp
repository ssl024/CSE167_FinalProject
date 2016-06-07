#include "WeddingCake.h"

WeddingCake::WeddingCake(OBJObject* cylinderObj, OBJObject* podObj, OBJObject* bearObj, GLuint shaderProgram)
{
	this->cylinderObj = cylinderObj;
	this->podObj = podObj;
	this->bearObj = bearObj;
	this->shaderProgram = shaderProgram;

	this->initialize();
}

WeddingCake::~WeddingCake()
{

}

Group* WeddingCake::getRoot()
{
	return this->root;
}

glm::mat4 WeddingCake::getBearCam()
{
	glm::vec3 bearEyePos = glm::vec3(bear->getM()[3]) + glm::vec3(0.0f, 2.0f, 0.0f);
	glm::vec3 bearEyeUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 bearDirection = glm::vec3(bear->getM() * glm::vec4(1.0f));

	return glm::lookAt(bearEyePos, bearDirection, bearEyeUp);
}

void WeddingCake::update()
{
	// Update since last frame
	this->root->update(glm::mat4(1.0f));

	// Set next frame rotations and translations
	// Spinning pods
	podAngle += 1.0f;
	if (podAngle > 360.0f)
		podAngle = 0.0f;
	for (int i = 0; i < NUM_PODS; ++i)
	{
		podRotate[i]->setUpdateMatrix(glm::rotate(glm::mat4(1.0f), podAngle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)));
	}

	// Spinning hand
	handAngle += 1.0f;
	if (handAngle > 360.0f)
		handAngle = 0.0f;
	for (int i = 0; i < NUM_HANDS; ++i)
	{
		handRotate[i]->setUpdateMatrix(glm::rotate(glm::mat4(1.0f), handAngle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)));
	}

	// Spinning level
	levelAngle -= 0.5f;
	if (levelAngle < 0.0f)
		levelAngle = 360.0f;
	for (int i = 0; i < NUM_LEVELS; ++i)
	{
		levelRotate[i]->setUpdateMatrix(glm::rotate(glm::mat4(1.0f), levelAngle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)));
	}

	// Extending levels
	levelDistance += 0.01f * levelDirection;
	if (levelDistance > 8.0f || levelDistance < 0.0f)
		levelDirection *= -1.0f;
	levelTranslate[1]->setUpdateMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, levelDistance * 0.2f, 0.0f)));
	levelTranslate[2]->setUpdateMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, levelDistance * 0.4f, 0.0f)));

	// Extending fingers
	fingerDistance += 0.01f * fingerDirection;
	if (fingerDistance > 2.0f || fingerDistance < 0.0f)
		fingerDirection *= -1.0f;
	for (int i = 0; i < NUM_FINGER_AND_POD; ++i)
	{
		extFingerAndPodTranslate[i]->setUpdateMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(fingerDistance, 0.0f, 0.0f)));
	}

	// Extending arm
	armDistance += 0.01f * armDirection;
	if (armDistance > 5.0f || armDistance < -1.0f)
		armDirection *= -1.0f;
	for (int i = 0; i < NUM_FINGERS; ++i)
	{
		handAndExtArm[i]->setUpdateMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(armDistance, 0.0f, 0.0f)));
	}


}

void WeddingCake::draw()
{
	this->root->draw();
}

void WeddingCake::initialize()
{
	// Bear
	bear = new Geode(bearObj, shaderProgram);

	// Poles
	for (int i = 0; i < NUM_POLES; ++i)
	{
		pole[i] = new Geode(cylinderObj, shaderProgram);
	}

	// Arms and Ext Arms
	for (int i = 0; i < NUM_ARMS; ++i)
	{
		arm[i] = new Geode(cylinderObj, shaderProgram);
	}
	for (int i = 0; i < NUM_EXT_ARMS; ++i)
	{
		extArm[i] = new Geode(cylinderObj, shaderProgram);
	}

	// Fingers, Ext Fingers, and Pods
	for (int i = 0; i < NUM_FINGERS; ++i)
	{
		finger[i] = new Geode(cylinderObj, shaderProgram);
	}
	for (int i = 0; i < NUM_EXT_FINGERS_AND_PODS; ++i)
	{
		extFinger[i] = new Geode(cylinderObj, shaderProgram);
	}
	for (int i = 0; i < NUM_PODS; ++i)
	{
		pod[i] = new Geode(podObj, shaderProgram);
	}

	// Bear Transform
	bearTranslate = new MatrixTransform(translate_bear);
	bearTranslate->addChild(bear);

	// Finger Transform
	for (int i = 0; i < NUM_FINGERS; ++i)
	{
		fingerTranslate[i] = new MatrixTransform(translate_finger_cylinder); // Move finger out of originn
		fingerScale[i] = new MatrixTransform(scale_stretch_finger_cylinder); // Stretch finger
		fingerRotate[i] = new MatrixTransform(rotate_finger_CWZ); // Turn finger on its side

		fingerRotate[i]->addChild(finger[i]);
		fingerScale[i]->addChild(fingerRotate[i]);
		fingerTranslate[i]->addChild(fingerScale[i]);
	}

	// Ext Finger Transform
	for (int i = 0; i < NUM_EXT_FINGERS; ++i)
	{
		extFingerTranslate[i] = new MatrixTransform(translate_finger_cylinder);
		extFingerScale[i] = new MatrixTransform(scale_stretch_finger_cylinder);
		extFingerRotate[i] = new MatrixTransform(rotate_finger_CWZ);

		extFingerRotate[i]->addChild(extFinger[i]);
		extFingerScale[i]->addChild(extFingerRotate[i]);
		extFingerTranslate[i]->addChild(extFingerScale[i]);
	}

	// Pod Transform
	for (int i = 0; i < NUM_PODS; ++i)
	{
		podTranslate[i] = new MatrixTransform(translate_pod); // Move pod out of origin to end of finger 
		podRotate[i] = new MatrixTransform(glm::mat4(1.0f)); // Change this to spin pod

		podRotate[i]->addChild(pod[i]);
		podTranslate[i]->addChild(podRotate[i]);
	}
	// Attach bear to one of the pods
	podRotate[10]->addChild(bearTranslate);

	// Ext Finger and Pod
	for (int i = 0; i < NUM_EXT_FINGERS_AND_PODS; ++i)
	{
		extFingerAndPodTranslate[i] = new MatrixTransform(glm::mat4(1.0f)); // Change this to extend finger and pod

		extFingerAndPodTranslate[i]->addChild(extFingerTranslate[i]);
		extFingerAndPodTranslate[i]->addChild(podTranslate[i]);
	}

	// Finger and Pod Transform
	for (int i = 0; i < NUM_FINGER_AND_POD; ++i)
	{
		rotate_third_CCWY = glm::rotate(glm::mat4(1.0f), ((float)i * 120.0f) / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
		fingersAndPodRotate[i] = new MatrixTransform(rotate_third_CCWY);

		fingersAndPodRotate[i]->addChild(fingerTranslate[i]);
		fingersAndPodRotate[i]->addChild(extFingerAndPodTranslate[i]);
	}

	// Hand Transform
	for (int i = 0; i < NUM_HANDS; ++i)
	{
		handTranslate[i] = new MatrixTransform(translate_hand); // Move hand to end of ext arm
		handRotate[i] = new MatrixTransform(glm::mat4(1.0f)); // Change this to spin hand

		int stride = i * 3;

		handRotate[i]->addChild(fingersAndPodRotate[stride]);
		handRotate[i]->addChild(fingersAndPodRotate[stride + 1]);
		handRotate[i]->addChild(fingersAndPodRotate[stride + 2]);

		handTranslate[i]->addChild(handRotate[i]);
	}

	// Arm Transform
	for (int i = 0; i < NUM_ARMS; ++i)
	{
		armTranslate[i] = new MatrixTransform(translate_arm_cylinder); // Move arm out of origin
		armScale[i] = new MatrixTransform(scale_stretch_arm_cylinder); // Stretch arm
		armRotate[i] = new MatrixTransform(rotate_90_CWZ); // Move arm cylinder on its side

		armRotate[i]->addChild(arm[i]);
		armScale[i]->addChild(armRotate[i]);
		armTranslate[i]->addChild(armScale[i]);
	}

	// Ext Arm Transform
	for (int i = 0; i < NUM_EXT_ARMS; ++i)
	{
		extArmTranslate[i] = new MatrixTransform(translate_arm_cylinder); // Move ext arm out of origin
		extArmScale[i] = new MatrixTransform(scale_stretch_arm_cylinder); // Stretch ext arm
		extArmRotate[i] = new MatrixTransform(rotate_90_CWZ); // Move arm cylinder on its side

		extArmRotate[i]->addChild(extArm[i]);
		extArmScale[i]->addChild(extArmRotate[i]);
		extArmTranslate[i]->addChild(extArmScale[i]);
	}

	// Hand and Ext Arm Transform
	for (int i = 0; i < NUM_HAND_AND_EXT_ARM; ++i)
	{
		handAndExtArm[i] = new MatrixTransform(glm::mat4(1.0f)); // Translate this to extend

		handAndExtArm[i]->addChild(handTranslate[i]);
		handAndExtArm[i]->addChild(extArmTranslate[i]);
	}

	// Entire Arm Transform
	for (int i = 0; i < NUM_ENTIRE_ARMS; ++i)
	{
		rotate_third_CCWY = glm::rotate(glm::mat4(1.0f), ((float)i * 120.0f) / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
		entireArm[i] = new MatrixTransform(rotate_third_CCWY);

		entireArm[i]->addChild(handAndExtArm[i]);
		entireArm[i]->addChild(armTranslate[i]);
	}

	// Pole Transform
	for (int i = 0; i < NUM_POLES; ++i)
	{
		poleTranslate[i] = new MatrixTransform(glm::mat4(1.0f)); // Change this to make pole go up or down
		poleScale[i] = new MatrixTransform(scale_pole_cylinder);

		poleScale[i]->addChild(pole[i]);
		poleTranslate[i]->addChild(poleScale[i]);
	}

	// Levels
	for (int i = 0; i < NUM_LEVELS; ++i)
	{
		levelTranslate[i] = new MatrixTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, (float)i * 8.0f + -10.0f, 0.0f)));
		levelRotate[i] = new MatrixTransform(glm::mat4(1.0f)); // Change this to make entire level rotate

		levelRotate[i]->addChild(poleTranslate[i]);

		int stride = i * 3;

		levelRotate[i]->addChild(entireArm[stride]);
		levelRotate[i]->addChild(entireArm[stride + 1]);
		levelRotate[i]->addChild(entireArm[stride + 2]);

		levelTranslate[i]->addChild(levelRotate[i]);
	}

	// Root
	root = new Group();
	rootTranslate = new MatrixTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 17.0f, 0.0f)));
	for (int i = 0; i < NUM_LEVELS; ++i)
	{
		//root->addChild(levelTranslate[i]);
		rootTranslate->addChild(levelTranslate[i]);
	}

	root->addChild(rootTranslate);

}