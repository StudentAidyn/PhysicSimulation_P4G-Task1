#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

class PhysicsSceneApp : public aie::Application {
public:

	PhysicsScene* m_physicsScene;

	PhysicsSceneApp();
	virtual ~PhysicsSceneApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
};