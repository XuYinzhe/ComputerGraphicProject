#include "Node.h"

#include <ctime>
#define VAL(x) (uiObj->x->value())


#include <time.h>

#include <ctime>



static vector<Node*> nodeList;
static int maxDepth = 10;
static ModelerUserInterface* uiObj;

std::default_random_engine generator;

std::uniform_real<float> distribution(0.7, 1.3);
std::uniform_real<float> distribution2(0, 360);

static std::vector<GLuint*>* textures;

void Node::setTexture(std::vector<GLuint*>* tex) {
    textures = tex;
}

void Node::setUI(ModelerUserInterface* UI)
{
    uiObj = UI;
}

Node* Node::generateTree(int Depth)
{
    //Clean up
    if (nodeList.size() != 0) {
        delete nodeList.back();
        nodeList.clear();
    }


    maxDepth = uiObj->m_depthSlider->value();

    maxDepth = Depth;

    generator.seed(std::time(NULL));
    Node* root = new Node(nullptr);
    return root;
}

void Node::drawTree(Node* root)
{
    glPushMatrix();
    glTranslated(root->scale[0] / 2 + root->scale[0]*0.2, root->parent == nullptr?0: (root->parent->scale[1] - root->scale[0] / 2), root->scale[2] / 2 + root->scale[2]*0.2);
    glRotated(root->rotate[0], 1, 0, 0);
    glRotated(root->rotate[1], 0, 1, 0);
    glRotated(root->rotate[2], 0, 0, 1);
    //if(root->parent == nullptr)

    //make it rotate w.r.t center of box
    glTranslated(-root->scale[0]/2,0,-root->scale[2]/2);

    //glScaled(root->scale[0], root->scale[1], root->scale[2]);
    //
    setDiffuseColor(155 / 255.0, 115 / 255.0, 58 / 255.0);
    glBindTexture(GL_TEXTURE_2D, *(*textures)[0]);
    drawBox(root->scale[0], root->scale[1], root->scale[2]);

    if(root->scale[1]>=0.05)//stop if already too short
        for (Node* child : root->childrens) {
            drawTree(child);
        }

    if (VAL(m_LeavesCheckbox) && (root->childrens.size() == 0 || root->scale[1] < 0.05)) {
        glRotated(root->leavesRot, 0, 1, 0);
        glTranslated(-root->scale[0]*1.8, root->scale[1] + 0.01,- root->scale[2]*1.5);
        drawleaves(1, 1, 1,(*textures)[2]);
    }
    glPopMatrix();
}

Vec3f dir[4] = {
    Vec3f(-50,0,0),
    Vec3f(0,0,-50),
    Vec3f(50,0,0),
    Vec3f(0,0,50)
};
Node::Node(Node* parent = nullptr) {
    this->parent = parent;
    if (parent == nullptr) {
        depth = 0;
        scale = Vec3f(1,uiObj->TrunkLength,1);
    }
    else
    {
        depth = parent->depth+1;
        this->scale = Vec3f(0.8 * parent->scale);

        std::uniform_real<float> sizeDistribution(VAL(m_branchMinSlider), VAL(m_branchMaxSlider));
        this->scale[1] = sizeDistribution(generator) * parent->scale[1];

        int num = parent->childrens.size();
        float x, z;
        if (VAL(m_RandomXrotateCheckbox)) {
            std::uniform_real<float> xDistribution(-VAL(m_XRotateSlider), VAL(m_XRotateSlider));
            x = xDistribution(generator);
        }
        else
        {
            switch (num+1)
            {
            case 1:
                x = 1 * VAL(m_XRotateSlider);
                break;
            case 4:
                x = -1 * VAL(m_XRotateSlider);
                break;
            default:
                x = 0 * VAL(m_XRotateSlider);
                break;
            }
            
        }

        if (VAL(m_RandomZrotateCheckbox)) {
            std::uniform_real<float> zDistribution(-VAL(m_ZRotateSlider), VAL(m_ZRotateSlider));
            z = zDistribution(generator);
        }
        else {
            switch (num+1)
            {
            case 2:
                z = 1 * VAL(m_ZRotateSlider);
                break;
            case 3:
                z = -1 * VAL(m_ZRotateSlider);
                break;
            default:
                z = 0 * VAL(m_ZRotateSlider);
                break;
            }
        }
        this->rotate = Vec3f(x, 0, z);

    }

    if (depth <= maxDepth) {
        for (int i = 0; i < uiObj->m_nSubTreeMax; i++) {

            if (VAL(m_RandomCheckbox) && i > VAL(m_SubTreeMinSlider)) {
                if (distribution(generator) > 1.0) {
                    continue;
                }
            }


            Node* tempChild = new Node(this);
            nodeList.push_back(tempChild);
            this->childrens.push_back(tempChild);
        }
    }
    else {
        leavesRot = distribution2(generator);
    }
    nodeList.push_back(this);
}

