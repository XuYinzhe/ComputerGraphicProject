#ifndef SCATTEREDLINEBRUSH_H
#define SCATTEREDLINEBRUSH_H

#include "ImpBrush.h"

class ScatteredLineBrush : public ImpBrush
{
public:
	ScatteredLineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void DirectionBegin(const Point source, const Point target);
	void DirectionMove(const Point source, const Point target);
	void DirectionEnd(const Point source, const Point target);
	char* BrushName(void);
private:
	double prePointX, prePointY;
	double directionPointX, directionPointY;
};


#endif
