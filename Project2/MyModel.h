#pragma once

// The sample model.  You should build a file
// very similar to this for when you make your model.

#include <gl/glew.h>
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "FBXManager.h"


#include "modelerglobals.h"

#include <FL/gl.h>
#include <vector>
#include <iostream>
#include "Node.h"



// To make a MyModel, we inherit off of ModelerView
class MyModel : public ModelerView
{
public:
	MyModel(int x, int y, int w, int h, char* label)
		: ModelerView(x, y, w, h, label) {
		Node::setTexture(&texture);
	}

	virtual void draw();
	void generateTree();
	//Shader* testShader = NULL;
	bool init = false;
	std::vector<GLuint*> texture;
	Node* root = nullptr;


};