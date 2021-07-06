#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "linebrush.h"
#include <iostream>
extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	
	prePointX = source.x;
	prePointY = source.y;
	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();
	int width = pDoc->getWidth();
	int angle = pDoc->getAngle();

	glLineWidth((float)width);
	glBegin(GL_LINES);

	double p1X = 0, p1Y = 0, p2X = 0, p2Y = 0;

	if (ImpBrush::m_DirectionType == RIGHT_MOUSE)
	{
		double theta = angle * 3.14159 / 180;
		p1X = target.x + size / 2.0 * cos(theta);
		p1Y = target.y + size / 2.0 * sin(theta);
		p2X = target.x - size / 2.0 * cos(theta);
		p2Y = target.y - size / 2.0 * sin(theta);
	}
	else if (ImpBrush::m_DirectionType == GRADIENT)
	{
		GLubyte* upPoint = pDoc->GetOriginalPixel(source.x, source.y - 1);
		GLubyte* lowPoint = pDoc->GetOriginalPixel(source.x, source.y + 1);
		GLubyte* leftPoint = pDoc->GetOriginalPixel(source.x - 1, source.y);
		GLubyte* rightPoint = pDoc->GetOriginalPixel(source.x + 1, source.y);
		double upPointGray = upPoint[0] * 0.299 + upPoint[1] * 0.587 + upPoint[2] * 0.114;
		double lowPointGray = lowPoint[0] * 0.299 + lowPoint[1] * 0.587 + lowPoint[2] * 0.114;
		double leftPointGray = leftPoint[0] * 0.299 + leftPoint[1] * 0.587 + leftPoint[2] * 0.114;
		double rightPointGray = rightPoint[0] * 0.299 + rightPoint[1] * 0.587 + rightPoint[2] * 0.114;
		double Gx = rightPointGray - leftPointGray;
		double Gy = upPointGray - lowPointGray;
		double theta = atan2(Gx, Gy);
		p1X = target.x + size / 2.0 * cos(theta);
		p1Y = target.y + size / 2.0 * sin(theta);
		p2X = target.x - size / 2.0 * cos(theta);
		p2Y = target.y - size / 2.0 * sin(theta);
	}
	else if (ImpBrush::m_DirectionType == BRUSH_DIRECTION)
	{
		double theta = atan2(prePointY-source.y,prePointX-source.x);
		p1X = target.x + size / 2.0 * cos(theta);
		p1Y = target.y + size / 2.0 * sin(theta);
		p2X = target.x - size / 2.0 * cos(theta);
		p2Y = target.y - size / 2.0 * sin(theta);
	}
	else {}
	SetColor(source);
	glVertex2d(p1X, p1Y);
	glVertex2d(p2X, p2Y);
	glEnd();
	prePointX = source.x;
	prePointY = source.y;
}

void LineBrush::BrushEnd(const Point source, const Point target){}

void LineBrush::DirectionBegin(const Point source, const Point target)
{
	directionPointX = target.x;
	directionPointY = target.y;
	//std::cout << directionPointX << std::endl;
	//std::cout << directionPointY << std::endl;
}
void LineBrush::DirectionMove(const Point source, const Point target)
{
	glLineWidth(2);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2d(directionPointX, directionPointY);
	glVertex2d(target.x, target.y);
	glEnd();
	//glFlush();
	//std::cout << target.x << std::endl;
	//std::cout << target.y << std::endl;
}
void LineBrush::DirectionEnd(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	int theta = 180 * atan2(target.y - directionPointY, target.x - directionPointX) / 3.14159;
	dlg->setAngle(theta); //std::cout << theta << std::endl;
	if(sqrt((target.y - directionPointY)*(target.y - directionPointY)+(target.x - directionPointX)*(target.x - directionPointX))<=40)
		dlg->setSize(sqrt((target.y - directionPointY) * (target.y - directionPointY) + (target.x - directionPointX) * (target.x - directionPointX)));
}