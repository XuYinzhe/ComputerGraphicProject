#include "GaussianBlur.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "pointbrush.h"

/*
1 2 4 2 1
2 4 8 4 2
4 8 16 8 4
2 4 8 4 2 
1 2 4 2 1
*/

GaussianBlurBrush::GaussianBlurBrush(ImpressionistDoc* pDoc, char* name):KernalBruah(pDoc,name) {
	kernal = {1,2,4,2,1,2,4,8,4,2,4,8,16,8,4,2,4,8,4,2,1,2,4,2,1};


	sum = 1+ 2+ 4+ 2+ 1+ 2+ 4+ 8+ 4+ 2+ 4+ 8+ 16+ 8+ 4+ 2+ 4+ 8+ 4+ 2+ 1+ 2+ 4+ 2+ 1;
	size = 5;
}

void GaussianBlurBrush::BrushMove(const Point source, const Point target)
{



	int range = GetDocument()->getSize()/2;
	for (int i = -range; i <= range; i++) {
		for (int j = -range; j <= range; j++) {
			Point realTarget(target.x + i, target.y + j);
			KernalBruah::BrushMove(realTarget, realTarget);
		}
	}
}
