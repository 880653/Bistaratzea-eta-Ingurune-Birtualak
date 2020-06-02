#include <cstdio>
#include "tools.h"
#include "renderState.h"


RenderState *RenderState::instance() {
	static RenderState obj;
	return &obj;
}

RenderState::RenderState() :
	m_frontMaterial(0),
	m_backMaterial(0),
	m_camera(0),
	m_ambient(Vector3(0.05f, 0.05f, 0.05)),
	m_activeShader(0),
	m_sc(1.0f),
	m_drawBBox(false) {}

RenderState::~RenderState() {}

///////////////////////////////////////////////////////////////
// Matrix stack stuff

TrfmStack *RenderState::chooseStack(stack_t matrixMode) {

	TrfmStack *res;

	res = 0;

	switch (matrixMode) {
	case modelview:
		res = &m_modelViewStack;
		break;
	case model:
		res = &m_modelStack;
		break;
	case projection:
		res  = &m_projectionStack;
		break;
	case texture:
		res = &m_textureStack;
		break;
	case modelview_projection:
		res = &m_modelViewProjectionStack;
		break;
	default:
		fprintf(stderr, "[E] RenderState::chooseStack: invalid matrix mode %d\n", matrixMode);
		exit(1);
	}
	return res;
}

// run after stack top changes

void RenderState::afterTopUpdate(stack_t matrixMode) {
	if(matrixMode == modelview ||
	   matrixMode == projection) {
		// update modelViewProjection matrix
		Trfm3D *mvp = m_modelViewProjectionStack.top();
		mvp->clone(m_projectionStack.top());
		mvp->add(m_modelViewStack.top());
	}
}

void RenderState::push(stack_t matrixMode) {
	chooseStack(matrixMode)->push();
}

Trfm3D *RenderState::pop(stack_t matrixMode) {
	Trfm3D *aux;
	TrfmStack *st = chooseStack(matrixMode);
	aux = st->pop();
	afterTopUpdate(matrixMode);
	return aux;
}

Trfm3D &RenderState::top(stack_t matrixMode) {
	return *chooseStack(matrixMode)->top();
}

void RenderState::loadIdentity(stack_t matrixMode) {
	chooseStack(matrixMode)->loadIdentity();
	afterTopUpdate(matrixMode);
}

void RenderState::loadTrfm(stack_t matrixMode, const Trfm3D * T) {
	chooseStack(matrixMode)->load(T);
	afterTopUpdate(matrixMode);
}

void RenderState::addTrfm(stack_t matrixMode, const Trfm3D * T) {
	chooseStack(matrixMode)->add(T);
	afterTopUpdate(matrixMode);
}

const GLfloat *RenderState::getGLMatrix(stack_t matrixMode) {
	return chooseStack(matrixMode)->getGLMatrix();
}

void RenderState::printTop(stack_t matrixMode) {
	chooseStack(matrixMode)->print();
}

///////////////////////////////////////////
// Shaders

void RenderState::setShader(ShaderProgram *program) {
	if (m_activeShader != 0)
		m_activeShader->deactivate();
	m_activeShader = program;
	if (m_activeShader != 0)
		m_activeShader->activate();
}

ShaderProgram *RenderState::getShader() {
	return m_activeShader;
}

///////////////////////////////////////////
// Camera

void RenderState::setCamera(Camera *cam) { m_camera = cam; }
Camera *RenderState::getCamera() const { return m_camera; }

///////////////////////////////////////////
// Lights

void RenderState::addLight(Light *l) {
	m_lights.push_back(l);
}

void RenderState::removeLight(Light *l) {
	m_lights.remove(l);
}

const std::list<Light *> *RenderState::lightList() const {
	return &m_lights;
}

///////////////////////////////////////////
// Materials

void RenderState::setFrontMaterial(Material *mat) {
	m_frontMaterial = mat;
}

Material *RenderState::getFrontMaterial() {
	return m_frontMaterial;
}

void RenderState::setBackMaterial(Material *mat) {
	m_backMaterial = mat;
}


void RenderState::setSkybox(Node *sky) {m_skybox = sky;}
Node *RenderState::getSkybox() { return m_skybox; }

void RenderState::drawBBoxes(bool draw) {
	m_drawBBox = draw;
}

bool RenderState::getBBoxDraw() const {
	return m_drawBBox;
}

///////////////////////////////////////////
// Scene ambient light
void RenderState::setSceneAmbient(const Vector3 &rgb) {
	m_ambient = rgb;
}
const Vector3 &RenderState::getSceneAmbient() const {
	return m_ambient;
}

float RenderState::getSc() const { return m_sc; }
void RenderState::setSc(float sc) { m_sc = sc;}

void RenderState::print() const {
	printf("* Stacks\n");
	printf("texture:\n");
	m_textureStack.top()->print();
	printf("model:\n");
	m_modelStack.top()->print();
	printf("modelView:\n");
	m_modelViewStack.top()->print();
	printf("projection:\n");
	m_projectionStack.top()->print();
	printf("modelViewProjection:\n");
	m_modelViewProjectionStack.top()->print();
	// printf("* Lights\n");
	// for(std::list<Light *>::const_iterator it =  m_lights.begin();
	//	it != m_lights.end(); ++it) {
	// }

	// Materials
	printf("frontMaterial\n");
	m_frontMaterial->print();

	// Shaders

	printf("Shader: %s\n", m_activeShader ? m_activeShader->getName() : "NULL");
	bool m_drawBBox;
}
