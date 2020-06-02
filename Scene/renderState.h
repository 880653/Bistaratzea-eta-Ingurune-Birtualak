// -*-C++-*-

#pragma once

/**
 * @file   renderState.h
 *
 * @brief Rendering state, including matrices, materials, textures and
 * shaders. It replaces the openGL fixing pipeline. The Rendering state is a
 * singleton, and can be retrieved using RenderState::instace() function.
 *
 */

#include <string>
#include <list>
#include "trfmStack.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include "textureManager.h"
#include "shader.h"
#include "skybox.h"

// Possible matrix stacks

#define MG_RT_FRMEBUFFER            0
#define MG_RT_TEXTURE               1


class RenderState {

public:
	static RenderState *instance();

	// stack type
	enum stack_t {
		modelview,
		model,
		projection,
		texture,
		modelview_projection
	};

	///////////////////////////////////////////
	// Matrix stuff

	/**
	 * Push to a matrix in the stack (the stack is selected by matrixMode)
	 *
	 * @param matrixMode the mode (see stack_t).
	 */
	void push(stack_t matrixMode);

	/**
	 * Pop from a matrix in the stack (the stack is selected by matrixMode)
	 *
	 * @param matrixMode the mode (see stack_t)
	 */
	Trfm3D *pop(stack_t matrixMode);

	/**
	 * Get top Trfm from a matrix in the stack (the stack is selected by matrixMode)
	 *
	 * @param matrixMode the mode (see stack_t)
	 */
	Trfm3D &top(stack_t matrixMode);

	/**
	 * Load identity Trfm into the top of a matrix in the stack (the stack is selected by matrixMode)
	 *
	 * @param matrixMode the mode (see stack_t)
	 */
	void loadIdentity(stack_t matrixMode);

	/**
	 * Load a Trfm into the top of a matrix in the stack (the stack is selected by matrixMode)
	 *
	 * @param matrixMode the mode (see stack_t)
	 */
	void loadTrfm(stack_t matrixMode, const Trfm3D * T);

	/**
	 * Add (compose) a Trfm into the top of a matrix in the stack (the stack is selected by matrixMode)
	 *
	 * @param matrixMode the mode (see stack_t)
	 */
	void addTrfm(stack_t matrixMode, const Trfm3D * T);

	/**
	 * Get openGL matrix of the top of a matrix in the stack (the stack is selected by matrixMode)
	 *
	 * @param matrixMode the mode (see stack_t)
	 */
	const GLfloat *getGLMatrix(stack_t matrixMode);

	/**
	 * Print the top of a matrix in the stack (the stack is selected by matrixMode)
	 *
	 * @param matrixMode the mode (see stack_t)
	 */
	void printTop(stack_t matrixMode);

	///////////////////////////////////////////
	// Shaders

	/**
	 * Set the Shader program as current, and activate it
	 *
	 * @param p the ShaderProgram object
	 */
	void setShader(ShaderProgram *p);

	/**
	 * Get the current Shader program
	 *
	 *
	 * @return the current ShaderProgram object (0 if none)
	 */
	ShaderProgram *getShader();

	///////////////////////////////////////////
	// Camera

	void setCamera(Camera *cam);
	Camera *getCamera() const;

	///////////////////////////////////////////
	// Lights

	/**
	 * Add a light into the rendering state
	 *
	 * @param l a light
	 */
	void addLight(Light *l);

	/**
	 * Remove a light from the rendering state
	 *
	 * @param l a light
	 */
	void removeLight(Light *l);

	/**
	 * Get a list with all active lights
	 *
	 *
	 * @return a list with the active lights
	 */
	const std::list<Light *> *lightList() const;

	///////////////////////////////////////////
	// Scene ambient light
	void setSceneAmbient(const Vector3 &rgb);
	const Vector3 &getSceneAmbient() const;

	///////////////////////////////////////////
	// Material

	/**
	 * Set current front material
	 *
	 * @param mat the material
	 */
	void setFrontMaterial(Material *mat);

	/**
	 * Get current front material
	 *
	 * @param mat the material
	 */
	Material *getFrontMaterial();


	/**
	 * Set current back material
	 *
	 * @param mat the material
	 */
	void setBackMaterial(Material *mat);


	Node *getSkybox();
	void setSkybox(Node *sky);

	///////////////////////////////////////////
	// Misc

	/**
	 * Specify whether to draw BBox-es
	 *
	 * @param draw 1 draw BBoxes, 0 don't draw BBoxes
	 */
	void drawBBoxes(bool draw);

	/**
	 * Get draw BBox-es
	 *
	 */
	bool getBBoxDraw() const;

	void print() const;

	float getSc() const;
	void setSc(float sc);

private:
	RenderState();
	~RenderState();
	RenderState(const RenderState &);
	RenderState operator=(const RenderState &);

	TrfmStack *chooseStack(stack_t matrixMode);
	void afterTopUpdate(stack_t matrixMode);

	// Matrix stacks
	TrfmStack m_modelViewStack;           // model space to clip space
	TrfmStack m_modelStack;               // model space to world space
	TrfmStack m_projectionStack;          // clip space to NDC space
	TrfmStack m_textureStack;             // tex. coordinates: object space to texture space
	TrfmStack m_modelViewProjectionStack; // model space to NDC space

	// Lights

	Camera *m_camera; // The camera we are rendering from

	// Lights

	std::list<Light *> m_lights;

	// Materials

	Material *m_frontMaterial;
	Material *m_backMaterial;

	// Scene ambient
	Vector3  m_ambient;               // default (0.05, 0.05, 0.05)

	// Shaders

	ShaderProgram *m_activeShader;
	bool m_drawBBox;

	// SkyBox

	Node *m_skybox;

	float m_sc;

};
