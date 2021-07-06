#pragma once
#include "KernalBruah.h"

class GaussianBlurBrush : public KernalBruah {
public:
	GaussianBlurBrush(ImpressionistDoc* pDoc = NULL, char* name = "Gaussian Blur Brush");


	virtual void BrushMove(const Point source, const Point target) override;

};