#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circlebrush.h"
#include "math.h"

extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();
	GLdouble PI = 3.14;
	GLdouble R = size / 2;
	GLint n = 32;

	glBegin(GL_POLYGON);
	SetColor(source);
	for (int i = 0; i < n; i++)
		glVertex2d(target.x+R*cos(2*PI*i/n),target.y+R*sin(2*PI*i/n));
	glEnd();

	//glPointSize((float)size);

	//BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{/*
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("CircleBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_LINES);
	SetColor(source);

	glVertex2d(target.x, target.y);

	glEnd();*/
	BrushBegin(source, target);
}

void CircleBrush::BrushEnd(const Point source, const Point target) {}

