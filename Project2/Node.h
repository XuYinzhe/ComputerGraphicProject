#pragma once
#include <vector>
#include "vec.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include "modelerui.h"
#include <random >

class Node
{
public:
	Node* parent;
	std::vector<Node*> childrens;

	Vec3f rotate;
	Vec3f scale;

	static Node* generateTree(int maxDepth);
	static void drawTree(Node* root);
	static void setTexture(std::vector<GLuint*>* textures);
	static void setUI(ModelerUserInterface* UI);

	~Node() {
		while (!childrens.empty())
		{
			Node* temp = childrens.back();
			childrens.pop_back();
			if (temp != nullptr)
				delete (temp);
		}
	}

private:
	int depth = 0;
	float leavesRot = 0;
	Node(Node* parent);
};

