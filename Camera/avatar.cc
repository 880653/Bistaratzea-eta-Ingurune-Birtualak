#include "tools.h"
#include "avatar.h"
#include "scene.h"
#include "intersect.h"
#include "node.h"

Avatar::Avatar(const std::string &name, Camera * cam, float radius) :
	m_name(name), m_cam(cam), m_walk(false) {
	Vector3 P = cam->getPosition();
	m_bsph = new BSphere(P, radius);
}

Avatar::~Avatar() {
	delete m_bsph;
}

bool Avatar::walkOrFly(bool walkOrFly) {
	bool walk = m_walk;
	m_walk = walkOrFly;
	return walk;
}

//
// AdvanceAvatar: advance 'step' units
//
// @@ TODO: Change function to check for collisions. If the destination of
// avatar collides with scene, do nothing.
//
// Return: true if the avatar moved, false if not.

bool Avatar::advance(float step) {

	Node *rootNode = Scene::instance()->rootNode(); // root node of scene

	m_bsph->setPosition(m_cam->getPosition());

	if(step>0){
		if(rootNode->checkCollision(m_bsph) == 0){
			if (m_walk)
				m_cam->walk(step);
			else
				m_cam->fly(step);
			return true;
		}
		else{
			if (m_walk)
				m_cam->walk(-step);
			else
				m_cam->fly(-step);
			return false;	
		}
	}
	else{
		if(rootNode->checkCollision(m_bsph) == 0){
			if (m_walk)
				m_cam->walk(step);
			else
				m_cam->fly(step);
			return true;
		}
		else{
			// if (m_walk)
			// 	m_cam->walk(-step);
			// else
			// 	m_cam->fly(-step);
			return false;	
		}
	}



}

void Avatar::leftRight(float angle) {
	if (m_walk)
		m_cam->viewYWorld(angle);
	else
		m_cam->yaw(angle);
}

void Avatar::upDown(float angle) {
	m_cam->pitch(angle);
}

void Avatar::print() const { }
