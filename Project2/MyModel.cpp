

// The sample model.  You should build a file
// very similar to this for when you make your model.
#include <gl/glew.h>
#include <math.h>
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "FBXManager.h"
#include "modelerui.h"

#include "modelerglobals.h"
//#include "Shader.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <FL/gl.h>
#include <vector>
#include <iostream>



#include "MyModel.h"

int aniCount = 0;
const int aniMax = 100;
int moodCount = 0;
const int moodMax = 200;
bool moodType = true;
bool ANI = false;
double range8 = 0, range15 = 0, range12 = 0;
double CountD = aniCount;
double maxD = aniMax;
int preMood = 0;



// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createMyModel(int x, int y, int w, int h, char* label)
{
	
	return new MyModel(x, y, w, h, label);
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out MyModel
void drawTorus(double radius=3, double radius_d=1, double face=8, double section=16)
{
	double preTheta = 0, preCos = 1.0, preSin = 0.0;
	double faceTheta, length;

	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(1, 1, 1);

	for (int i = 0; i < section; i++)
	{
		double nowTheta = preTheta + 2.0 * M_PI / section;
		glBegin(GL_QUAD_STRIP);
		faceTheta = 0;
		for (int j = 0; j <= face; j++)
		{
			faceTheta += 2.0 * M_PI / face;
			length = radius + radius_d * cos(faceTheta);

			glNormal3d(preCos * cos(faceTheta), preSin * cos(faceTheta), sin(faceTheta));
			glVertex3d(preCos * length, preSin * length, radius_d * sin(faceTheta));

			glNormal3d(cos(nowTheta) * cos(faceTheta), sin(nowTheta) * cos(faceTheta), sin(faceTheta));
			glVertex3d(cos(nowTheta) * length, sin(nowTheta) * length, radius_d * sin(faceTheta));
		}
		glEnd();
		preTheta = nowTheta;
		preCos = cos(nowTheta);
		preSin = sin(nowTheta);
	}
}


void MyModel::draw()
{
	// init texture here
	if (!init) {
		init = true;
		//testShader = new Shader("shader.vs", "shader.fs");
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.9);//0.5可以换成任何在0~1之间的数 
		
		// 为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// 加载并生成纹理
		int width, height, nrChannels;
		unsigned char* data = stbi_load("texture1.jpg", &width, &height, &nrChannels, 0);
		unsigned char* data2;
		unsigned char* data3;
		if (data)
		{
			texture.push_back( new GLuint);
			glGenTextures(1, texture[0]);
			glBindTexture(GL_TEXTURE_2D, *texture[0]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			data2 = stbi_load("testTexture.jpg", &width, &height, &nrChannels, 0);

			texture.push_back(new GLuint);
			glGenTextures(1, texture[1]);
			glBindTexture(GL_TEXTURE_2D, *texture[1]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
			glGenerateMipmap(GL_TEXTURE_2D);

			texture.push_back(new GLuint);
			data3 = stbi_load("leaves.png", &width, &height, &nrChannels, 0);
			glGenTextures(1, texture[2]);
			glBindTexture(GL_TEXTURE_2D, *texture[2]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data3);
			glGenerateMipmap(GL_TEXTURE_2D);


		}
		else
		{
			printf("Failed to load texture");
		}
			stbi_image_free(data);
			stbi_image_free(data2);
			stbi_image_free(data3);
			glBindTexture(GL_TEXTURE_2D, *texture[0]);
	}


	// This call takes care of a lot of the nasty projection 
	// matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
	ModelerView::draw();
	
	if (root != nullptr) {
		Node::drawTree(root);
	}else if (FBXManager::getFbxManager()->loaded) {
		FBXManager* fbxManager = FBXManager::getFbxManager();
		fbxManager->drawSceneGL();
	}

	else if (VAL(TORUS) == 1)
	{
		drawTorus(VAL(RADIUS), VAL(RADIUS_D), VAL(FACES), VAL(SECTIONS));
		/*
		double preTheta = 0, preCos = 1.0, preSin = 0.0;
		double faceTheta, length;

		setAmbientColor(.1f, .1f, .1f);
		setDiffuseColor(1,1,1);

		for (int i = 0; i < VAL(SECTIONS); i++)
		{
			double nowTheta = preTheta + 2.0 * M_PI / VAL(SECTIONS);
			glBegin(GL_QUAD_STRIP);
			faceTheta = 0;
			for (int j = 0; j <= VAL(FACES); j++)
			{
				faceTheta += 2.0 * M_PI / VAL(FACES);
				length = VAL(RADIUS) + VAL(RADIUS_D) * cos(faceTheta);

				glNormal3d(preCos * cos(faceTheta), preSin * cos(faceTheta), sin(faceTheta));
				glVertex3d(preCos * length, preSin * length, VAL(RADIUS_D) * sin(faceTheta));

				glNormal3d(cos(nowTheta) * cos(faceTheta), sin(nowTheta) * cos(faceTheta), sin(faceTheta));
				glVertex3d(cos(nowTheta) * length, sin(nowTheta) * length, VAL(RADIUS_D)* sin(faceTheta));
			}
			glEnd();
			preTheta = nowTheta;
			preCos = cos(nowTheta);
			preSin = sin(nowTheta);
		}*/
	}



	else {
		//std::cout << ModelerUserInterface::m_controlsAnimOnMenu->value() << std::endl;
		//std::cout << ModelerUserInterface::menu_m_controlsMenuBar->value() << std::endl;
		if (ModelerUserInterface::m_controlsAnimOnMenu->value()==0)ANI = false;
		else ANI = true;


		if (!ANI)
		{
			aniCount = 0;
		}
		
		// draw the floor
		setAmbientColor(.1f, .1f, .1f);
		setDiffuseColor(COLOR_RED);
		/*
		glPushMatrix();
		glTranslated(-5, 0, -5);
		drawBox(10, 0.01f, 10);
		glPopMatrix();
	*/
	//Draw some test obj
		glBindTexture(GL_TEXTURE_2D, *texture[1]);

		glPushMatrix();
		setDiffuseColor(0.3, 0, 0);
		glTranslated(-5, 0, 0);
		drawTextureBox(1, 1, 1);
		glPopMatrix();


		glPushMatrix();
		setDiffuseColor(COLOR_RED);
		glTranslated(5, 0, 0);
		drawTextureBox(1, 1, 1);
		glPopMatrix();

		glPushMatrix();
		setDiffuseColor(0, 0.3, 0);
		glTranslated(0, -5, 0);
		drawTextureBox(1, 1, 1);
		glPopMatrix();


		glPushMatrix();
		setDiffuseColor(COLOR_GREEN);
		glTranslated(0, 5, 0);
		drawTextureBox(1, 1, 1);
		glPopMatrix();

		glPushMatrix();
		setDiffuseColor(0, 0, 0.3);
		glTranslated(0, 0, -5);
		drawTextureBox(1, 1, 1);
		glPopMatrix();


		glPushMatrix();
		setDiffuseColor(COLOR_BLUE);
		glTranslated(0, 0, 5);
		drawTextureBox(1, 1, 1);
		glPopMatrix();

		//glDeleteTextures(1, &texture);
		//test obj end

		//change testure
		glBindTexture(GL_TEXTURE_2D, *texture[0]);

		// draw the sample model
		setAmbientColor(.1f, .1f, .1f);
		setDiffuseColor(.4f, 0, .2f);

		glPushMatrix();
		if (!ANI)
			glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
		else
			moodType? glTranslated(VAL(XPOS), VAL(YPOS), range15/50): glTranslated(VAL(XPOS), range15/100, VAL(ZPOS));

		glPushMatrix();
		if (!ANI)
			glRotated(VAL(FRONT_BODY), 0.0, 1.0, 0.0);
		else
			moodType ? glRotated(range8, 0.0, 1.0, 0.0) : glRotated(0, 0.0, 1.0, 0.0);
		glTranslated(-1, 0, .1);
		drawTextureBox(2, 1.2, 1);
		glTranslated(0.4, 0.1, 1);
		drawTextureBox(1.2, 1, 0.8);
		glPushMatrix();
		glTranslated(-.4, -.4, -.8);//2.9 -1.7
		//glRotated(-80 + VAL(LEG_RIGHT1), 0.0, 1.0, 0.0);
		if (!ANI)
			glRotated(-80 + VAL(LEG_RIGHT1), 0.0, 1.0, 0.0);
		else
			//glRotated(-80 - range15, 0.0, 1.0, 0.0);
			moodType ? glRotated(-80 - range15, 0.0, 1.0, 0.0) : glRotated(-80 + range15, 0.0, 1.0, 0.0);
		glRotated(30, 1.0, 0.0, 0.0);
		drawTextureBox(.3, .3, 1.5);
		glPopMatrix();

		glPushMatrix();
		glTranslated(1.6, -.4, -.8);
		//glRotated(80 + VAL(LEG_LEFT1), 0.0, 1.0, 0.0);
		if (!ANI)
			glRotated(80 + VAL(LEG_LEFT1), 0.0, 1.0, 0.0);
		else
			glRotated(80 + range15, 0.0, 1.0, 0.0);
		glRotated(30, 1.0, 0.0, 0.0);
		drawTextureBox(-.3, .3, 1.5);
		glPopMatrix();


			glPushMatrix();
			glTranslated(1.8, .4, -.4);
			//glRotated(75 + VAL(ARM_LEFT2_1), 0.0, 1.0, 0.0);
			if (!ANI)
				glRotated(75 + VAL(ARM_LEFT2_1), 0.0, 1.0, 0.0);
			else
				glRotated(75 + range15, 0.0, 1.0, 0.0);
			//glRotated(VAL(ARM_LEFT2_2), 1.0, 0.0, 0.0);
			if (!ANI)
				glRotated(VAL(ARM_LEFT2_2), 1.0, 0.0, 0.0);
			else
				glRotated(range8, 1.0, 0.0, 0.0);
			glRotated(10, 1.0, 0.0, 0.0);
			drawTextureBox(-.3, .3, 1.4);
				glPushMatrix();
				glTranslated(-.2, .15, 1.2);
				//glRotated(-90 + VAL(ARM_LEFT1), 0.0, 1.0, 0.0);
				if (!ANI)
					glRotated(-90 + VAL(ARM_LEFT1), 0.0, 1.0, 0.0);
				else
					glRotated(-90 + range15, 0.0, 1.0, 0.0);
				drawCylinder(1.5, 0.15, 0.15);
					glPushMatrix();
					glTranslated(0, 0, 1.4);
					glRotated(30, 0.0, 1.0, 0.0);
					drawCylinder(0.8, 0.15, 0.1);
						glPushMatrix();
						glRotated(-40, 0.0, 1.0, 0.0);
						glTranslated(0.5, 0, 0.6);
						drawCylinder(0.8, 0.08, 0.08);
							glPushMatrix();
							glTranslated(0, 0, 0.8);
							glRotated(-45, 0.0, 1.0, 0.0);
							drawCylinder(0.6, 0.08, 0.001);
							glPopMatrix();
						glPopMatrix();
					glPopMatrix();
					glPushMatrix();
					glTranslated(-0.1, 0, 1.6);
					//glRotated(-10 + VAL(JAW_LEFT), 0.0, 1.0, 0.0);
					if (!ANI)
						glRotated(-10 + VAL(JAW_LEFT), 0.0, 1.0, 0.0);
					else
						glRotated(-10 + range12, 0.0, 1.0, 0.0);
					drawCylinder(.9, 0.1, 0.06);
						glPushMatrix();
						glTranslated(0, 0, .9);
						glRotated(30, 0.0, 1.0, 0.0);
						drawCylinder(.6, 0.06, 0.001);
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();

			glPushMatrix();
			glTranslated(-0.6, .4, -.4);
			//glRotated(-75 + VAL(ARM_RIGHT2_1), 0.0, 1.0, 0.0);
			if (!ANI)
				glRotated(-75 + VAL(ARM_RIGHT2_1), 0.0, 1.0, 0.0);
			else
				moodType ? glRotated(-75 + range15, 0.0, 1.0, 0.0) : glRotated(-75 - range15, 0.0, 1.0, 0.0);
			//glRotated(VAL(ARM_RIGHT2_2), 1.0, 0.0, 0.0);
			if (!ANI)
				glRotated(VAL(ARM_RIGHT2_2), 1.0, 0.0, 0.0);
			else
				glRotated(range8, 1.0, 0.0, 0.0);
			glRotated(10, 1.0, 0.0, 0.0);
			drawTextureBox(.3, .3, 1.4);
				glPushMatrix();
				glTranslated(.2, .15, 1.2);
				//glRotated(90 + VAL(ARM_RIGHT1), 0.0, 1.0, 0.0);
				if (!ANI)
					glRotated(90 + VAL(ARM_RIGHT1), 0.0, 1.0, 0.0);
				else
					moodType ? glRotated(90 + range15, 0.0, 1.0, 0.0) : glRotated(90 - range15, 0.0, 1.0, 0.0);
				drawCylinder(1.5, 0.15, 0.15);
					glPushMatrix();
					glTranslated(0, 0, 1.5);
					glRotated(-30, 0.0, 1.0, 0.0);
					drawCylinder(0.8, 0.15, 0.1);
						glPushMatrix();
						glRotated(40, 0.0, 1.0, 0.0);
						glTranslated(-0.5, 0, 0.6);
						drawCylinder(0.8, 0.08, 0.08);
							glPushMatrix();
							glTranslated(0, 0, 0.8);
							glRotated(45, 0.0, 1.0, 0.0);
							drawCylinder(0.6, 0.08, 0.001);
							glPopMatrix();
						glPopMatrix();
					glPopMatrix();
					glPushMatrix();
					glTranslated(0.1, 0, 1.6);
					//glRotated(10 + VAL(JAW_RIGHT), 0.0, 1.0, 0.0);
					if (!ANI)
						glRotated(10 + VAL(JAW_RIGHT), 0.0, 1.0, 0.0);
					else
						glRotated(10 - range12, 0.0, 1.0, 0.0);
					drawCylinder(.9, 0.1, 0.06);
						glPushMatrix();
						glTranslated(0, 0, .9);
						glRotated(-30, 0.0, 1.0, 0.0);
						drawCylinder(.6, 0.06, 0.001);
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
		//glRotated(VAL(BACK_BODY), 0.0, 1.0, 0.0);
		if (!ANI)
			glRotated(VAL(BACK_BODY), 0.0, 1.0, 0.0);
		else
			moodType ? glRotated(-range8, 0.0, 1.0, 0.0) : glRotated(0, 0.0, 1.0, 0.0);
		glTranslated(-1, 0, -1.1);
		drawTextureBox(2, 1.2, 1);
		glTranslated(0.4, 0.1, -.8);
		drawTextureBox(1.2, 1, 0.8);
			glPushMatrix();
			glTranslated(1.5, -.4, .7);
			//glRotated(-45 + VAL(LEG_LEFT3), 0.0, 1.0, 0.0);
			if (!ANI)
				glRotated(-45 + VAL(LEG_LEFT3), 0.0, 1.0, 0.0);
			else
				glRotated(-45 - range15, 0.0, 1.0, 0.0);
			glRotated(-30, 1.0, 0.0, 0.0);
			drawTextureBox(.3, .3, -1.5);
			glPopMatrix();

			glPushMatrix();
			glTranslated(-.3, -.4, .7);
			//glRotated(45 + VAL(LEG_RIGHT3), 0.0, 1.0, 0.0);
			if (!ANI)
				glRotated(45 + VAL(LEG_RIGHT3), 0.0, 1.0, 0.0);
			else
				//glRotated(45 + range15, 0.0, 1.0, 0.0);
				moodType ? glRotated(45 + range15, 0.0, 1.0, 0.0) : glRotated(45 - range15, 0.0, 1.0, 0.0);
			glRotated(-30, 1.0, 0.0, 0.0);
			drawTextureBox(-.3, .3, -1.5);
			glPopMatrix();

			glPushMatrix();
			glTranslated(1.6, -.4, 1.4);
			//glRotated(-80 + VAL(LEG_LEFT2), 0.0, 1.0, 0.0);
			if (!ANI)
				glRotated(-80 + VAL(LEG_LEFT2), 0.0, 1.0, 0.0);
			else
				glRotated(-80 + range15, 0.0, 1.0, 0.0);
			glRotated(-30, 1.0, 0.0, 0.0);
			drawTextureBox(.3, .3, -1.5);
			glPopMatrix();

			glPushMatrix();
			glTranslated(-.4, -.4, 1.4);
			//glRotated(80 + VAL(LEG_RIGHT2), 0.0, 1.0, 0.0);
			if (!ANI)
				glRotated(80 + VAL(LEG_RIGHT2), 0.0, 1.0, 0.0);
			else
				//glRotated(80 - range15, 0.0, 1.0, 0.0);
				moodType ? glRotated(80 - range15, 0.0, 1.0, 0.0) : glRotated(80 + range15, 0.0, 1.0, 0.0);
			glRotated(-30, 1.0, 0.0, 0.0);
			drawTextureBox(-.3, .3, -1.5);
			glPopMatrix();
		
		
			glPushMatrix();
			//glRotated(30 + VAL(TAIL1), 1.0, 0.0, 0.0);
			if (!ANI)
				glRotated(30 + VAL(TAIL1), 1.0, 0.0, 0.0);
			else
				moodType ? glRotated(30 + range15, 1.0, 0.0, 0.0) : glRotated(30 - range15, 1.0, 0.0, 0.0);
			glTranslated(0.3, 0.1, 0);
			drawTextureBox(.6, .6, -1.2);
				glPushMatrix();
				glTranslated(.05, 0.06, -1.2);
				//glRotated(30 + VAL(TAIL2), 1.0, 0.0, 0.0);
				if (!ANI)
					glRotated(30 + VAL(TAIL2), 1.0, 0.0, 0.0);
				else
					moodType ? glRotated(30 + range15, 1.0, 0.0, 0.0) : glRotated(30 - range15, 1.0, 0.0, 0.0);
				drawTextureBox(.5, .5, -1.2);
					glPushMatrix();
					glTranslated(.05, 0.06, -1.2);
					//glRotated(30 + VAL(TAIL3), 1.0, 0.0, 0.0);
					if (!ANI)
						glRotated(30 + VAL(TAIL3), 1.0, 0.0, 0.0);
					else
						moodType ? glRotated(30 + range15, 1.0, 0.0, 0.0) : glRotated(30 - range15, 1.0, 0.0, 0.0);
					drawTextureBox(.4, .4, -1.2);
						glPushMatrix();
						glTranslated(.05, 0.06, -1.2);
						//glRotated(30 + VAL(TAIL4), 1.0, 0.0, 0.0);
						if (!ANI)
							glRotated(30 + VAL(TAIL4), 1.0, 0.0, 0.0);
						else
							moodType ? glRotated(30 + range15, 1.0, 0.0, 0.0) : glRotated(30 - range15, 1.0, 0.0, 0.0);
						drawTextureBox(.3, .3, -1);
							glPushMatrix();
							glTranslated(.15, .15, -.9);
							//glRotated(VAL(HOOK1), 0.0, 1.0, 0.0);
							if (!ANI)
								glRotated(VAL(HOOK1), 0.0, 1.0, 0.0);
							else
								glRotated(range15, 0.0, 1.0, 0.0);
							//glRotated(180 + VAL(HOOK2), 1.0, 0.0, 0.0);
							if (!ANI)
								glRotated(180 + VAL(HOOK2), 1.0, 0.0, 0.0);
							else
								moodType ? glRotated(180 + range15, 1.0, 0.0, 0.0) : glRotated(180 - range15, 1.0, 0.0, 0.0);
							drawCylinder(1, 0.1, 0.001);
							glPopMatrix();
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	/*
		glPushMatrix();
		glTranslated(-1.5, 0, -2);
		glScaled(3, 1, 4);
		drawBox(1, 1, 1);
		glPopMatrix();
			glRotated(VAL(ROTATE), 0.0, 1.0, 0.0);
			glRotated(-90, 1.0, 0.0, 0.0);
			drawCylinder(VAL(HEIGHT), 0.1, 0.1);

			glTranslated(0.0, 0.0, VAL(HEIGHT));
			drawCylinder(1, 1.0, 0.9);

			glTranslated(0.0, 0.0, 0.5);
			glRotated(90, 1.0, 0.0, 0.0);
			drawCylinder(4, 0.1, 0.2);
			glPopMatrix();
			*/

		glPopMatrix();

		//std::cout << range8 << std::endl;

		if (ANI)
		{
			//std::cout << range8 << std::endl;
			aniCount %= aniMax;
			aniCount++;

			if (VAL(MOOD) == 1)
			{
				if (preMood != VAL(MOOD))aniCount = 0;
				moodCount++;
				if (moodCount > aniMax && moodType)moodType = !moodType;
				if (moodCount == moodMax)
				{
					moodType = true;
					moodCount = 0;
				}
			}
			else
			{
				if (preMood != VAL(MOOD))aniCount = 0;
				moodType = true;
				//aniCount = 0;
			}

			preMood = VAL(MOOD);

			CountD = aniCount;

			if (aniCount <= aniMax / 4)
				range8 = 8 * CountD / (maxD / 4);
			else if (aniCount <= aniMax * 3 / 4)
				range8 = 8 - 16 * (CountD - maxD / 4) / (maxD / 2);
			else
				range8 = -8 + 8*(CountD - maxD * 3 / 4) / (maxD / 4);

			if (aniCount <= aniMax / 4)
				range15 = 15 * CountD / (maxD / 4);
			else if (aniCount <= aniMax * 3 / 4)
				range15 = 15 - 30 * (CountD - maxD / 4) / (maxD / 2);
			else
				range15 = -15 + 15 * (CountD - maxD * 3 / 4) / (maxD / 4);

			if (aniCount <= aniMax / 4)
				range12 = 12 * CountD / (maxD / 4);
			else if (aniCount <= aniMax * 3 / 4)
				range12 = 12 - 24 * (CountD - maxD / 4) / (maxD / 2);
			else
				range12 = -12 + 12 * (CountD - maxD * 3 / 4) / (maxD / 4);
		}
	}
}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
	ModelerControl controls[NUMCONTROLS];
	controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
	controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
	controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
	//controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1f, 1);
	//controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);
	controls[FRONT_BODY] = ModelerControl("Front Body Angle", -8, 8, 0.1f, 0);
	controls[BACK_BODY] = ModelerControl("Back Body Angle", -8, 8, 0.1f, 0);
	controls[LEG_LEFT1] = ModelerControl("Left Leg-1 Angle", -8, 8, 0.1f, 0);
	controls[LEG_LEFT2] = ModelerControl("Left Leg-2 Angle", -8, 8, 0.1f, 0);
	controls[LEG_LEFT3] = ModelerControl("Left Leg-3 Angle", -8, 8, 0.1f, 0);
	controls[LEG_RIGHT1] = ModelerControl("Right Leg-1 Angle", -8, 8, 0.1f, 0);
	controls[LEG_RIGHT2] = ModelerControl("Right Leg-2 Angle", -8, 8, 0.1f, 0);
	controls[LEG_RIGHT3] = ModelerControl("Right Leg-3 Angle", -8, 8, 0.1f, 0);
	controls[ARM_LEFT1] = ModelerControl("Left Arm-1 Angle", -15, 15, 0.1f, 0);
	controls[ARM_RIGHT1] = ModelerControl("Right Arm-1 Angle", -15, 15, 0.1f, 0);
	controls[ARM_LEFT2_1] = ModelerControl("Left Arm-2 Angle-1", -15, 15, 0.1f, 0);
	controls[ARM_RIGHT2_1] = ModelerControl("Right Arm-2 Angle-1", -15, 15, 0.1f, 0);
	controls[ARM_LEFT2_2] = ModelerControl("Left Arm-2 Angle-2", -8, 8, 0.1f, 0);
	controls[ARM_RIGHT2_2] = ModelerControl("Right Arm-2 Angle-2", -8, 8, 0.1f, 0);
	controls[JAW_LEFT] = ModelerControl("Left Jaw Angle", -12, 12, 0.1f, 0);
	controls[JAW_RIGHT] = ModelerControl("Right Jaw Angle", -12, 12, 0.1f, 0);
	controls[TAIL1] = ModelerControl("Tail-1 Angle", -15, 15, 0.1f, 0);
	controls[TAIL2] = ModelerControl("Tail-2 Angle", -15, 15, 0.1f, 0);
	controls[TAIL3] = ModelerControl("Tail-3 Angle", -15, 15, 0.1f, 0);
	controls[TAIL4] = ModelerControl("Tail-4 Angle", -15, 15, 0.1f, 0);
	controls[HOOK1] = ModelerControl("Hook Angle-1", -15, 15, 0.1f, 0);
	controls[HOOK2] = ModelerControl("Hook Angle-2", -15, 15, 0.1f, 0);
	controls[MOOD] = ModelerControl("Open Mood", 0, 1, 1, 0);
	controls[TORUS] = ModelerControl("Draw Torus", 0, 1, 1, 0);
	controls[RADIUS] = ModelerControl("Torus Radius", 0, 10, 0.1f, 3);
	controls[RADIUS_D] = ModelerControl("Torus Radius Thick", 0, 3, 0.1f, 1);
	controls[FACES] = ModelerControl("Torus Faces", 3, 16, 1, 8);
	controls[SECTIONS] = ModelerControl("Torus Sections", 3, 32, 1, 16);

	controls[LIGHT_X] = ModelerControl("light X", -100, 100, 1, 0);
	controls[LIGHT_Y] = ModelerControl("light Y", -100, 100, 1, 0);
	controls[LIGHT_Z] = ModelerControl("light Z", -100, 100, 1, 0);
	controls[CAM_ANG] = ModelerControl("Camera angle", -180, 180, 1, 0);

	

	ModelerApplication::Instance()->Init(&createMyModel, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}

void MyModel::generateTree() {
	Node::setUI(ModelerApplication::Instance()->m_ui);
	root = Node::generateTree(ModelerApplication::Instance()->m_ui->m_nSize);
	redraw();
}