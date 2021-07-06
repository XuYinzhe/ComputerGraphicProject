#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointBrush.h"
#include <iostream>
extern float frand();

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredPointBrush::BrushBegin(const Point source, const Point target)
{

	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	int size = pDoc->getSize();

	glPointSize(1);
	glBegin(GL_POINTS);
	for (int i = 0; i < size * 5; i++)
	{
		int curX = rand() % size;
		int curY = rand() % (2*size-1)%size;	
		Point curP(source.x + curX - size / 2, source.y + curY - size / 2); //std::cout << curP.x << ' ' << curP.y << std::endl;
		SetColor(curP);
		glVertex2i(curP.x, curP.y);
	}
	glEnd();

	//glPointSize((float)size);

	//BrushMove(source, target);
}

void ScatteredPointBrush::BrushMove(const Point source, const Point target)
{/*
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredPointBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_LINES);
	SetColor(source);

	glVertex2d(target.x, target.y);

	glEnd();*/
	BrushBegin(source, target);
}

void ScatteredPointBrush::BrushEnd(const Point source, const Point target) {}

