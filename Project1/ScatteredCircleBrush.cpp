#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCircleBrush.h"
#include "math.h"

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();
	GLdouble PI = 3.14;
	GLdouble R = size / 2;
	GLint n = 32;

	
	
	for (int c = 0; c < 4; c++)
	{
		int curX = rand() % size;
		int curY = rand() % (2 * size - 1) % size;
		Point curP(source.x + curX - size / 2, source.y + curY - size / 2);
		glBegin(GL_POLYGON);
		SetColor(curP);
		for (int i = 0; i < n; i++)
			glVertex2i(curP.x + R * cos(2 * PI * i / n), curP.y + R * sin(2 * PI * i / n));
		glEnd();
	}
	

	//glPointSize((float)size);

	//BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
{/*
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredCircleBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_LINES);
	SetColor(source);

	glVertex2d(target.x, target.y);

	glEnd();*/
	BrushBegin(source, target);
}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target) {}

