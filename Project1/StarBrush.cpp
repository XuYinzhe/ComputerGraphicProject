#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "starbrush.h"

extern float frand();

StarBrush::StarBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void StarBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	



	//glPointSize((float)size);

	BrushMove(source, target);
}

void StarBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	int size = pDoc->getSize();

	if (pDoc == NULL) {
		printf("StarBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_TRIANGLES);
	SetColor(source);
	glVertex2d(target.x + size / 2, target.y + size / sqrt(12));
	glVertex2d(target.x - size / 2, target.y + size / sqrt(12));
	glVertex2d(target.x, target.y);
	glEnd();

	glBegin(GL_TRIANGLES);
	SetColor(source);
	glVertex2d(target.x, target.y + size / sqrt(3));
	glVertex2d(target.x - size / 2, target.y - size / sqrt(12));
	glVertex2d(target.x, target.y);
	glEnd();

	glBegin(GL_TRIANGLES);
	SetColor(source);
	glVertex2d(target.x, target.y + size / sqrt(3) );
	glVertex2d(target.x + size / 2 , target.y - size / sqrt(12) );
	glVertex2d(target.x , target.y );
	glEnd();
}

void StarBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}