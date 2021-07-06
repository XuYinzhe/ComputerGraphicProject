#include "KernalBruah.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

KernalBruah::KernalBruah(ImpressionistDoc* pDoc, char* name)
	:ImpBrush(pDoc, name)
{
	size = 0;
	sum = 0;
}
//-1,-1,-1,0,0,0,1,1,1
void KernalBruah::BrushBegin(const Point source, const Point target)
{
	glPointSize(1);

	
	BrushMove(source, target);
}

void KernalBruah::BrushMove(const Point source, const Point target)
{
	glBegin(GL_POINTS);
	int range = size / 2;
	int kernalPos = 0;
	float result[3] = { 0,0,0 };//RGB
	for (int y = -range; y <= range; y++) {
		for (int x = -range; x <= range; x++) {
			result[0] += kernal[kernalPos] * GetDocument()->GetFileCopyPixel(source.x + x, source.y + y)[0];
			result[1] += kernal[kernalPos] * GetDocument()->GetFileCopyPixel(source.x + x, source.y + y)[1];
			result[2] += kernal[kernalPos] * GetDocument()->GetFileCopyPixel(source.x + x, source.y + y)[2];
			kernalPos++;
		}
	}
	result[0] /= sum;
	result[1] /= sum;
	result[2] /= sum;

	GLubyte resultByte[3] = { result[0] ,result[1] ,result[2] };

	glColor4ubv(resultByte);

	glVertex2d(target.x, target.y);

	glEnd();
}

void KernalBruah::BrushEnd(const Point source, const Point target)
{
}

KernalBruah* KernalBruah::GenerateConverlution(ImpressionistDoc* pDoc, char* weights,bool normalize) {

	//try parse input kernal string

	std::vector<float> i_weight;
	char* token = strtok(weights, ",");
	while (token != NULL) {
		i_weight.push_back(atof(token));
		token = strtok(NULL, ",");
	};

	double temp = sqrt(i_weight.size());

	delete[] weights;
	if (int(temp) != temp || temp == 1 || int(temp) % 2 == 0) {
		//failed
		return nullptr;
	}
	else {
		KernalBruah* kernalBrush = new KernalBruah(pDoc);

		kernalBrush->size = int(temp);

		float sum = 0;
		for (int i : i_weight) {
			sum += i;
		}


		if (normalize) {
			kernalBrush->sum = sum ? sum:1;
		}
		else {
			kernalBrush->sum = 1;
		}
		
		kernalBrush->kernal = i_weight;

		return kernalBrush;
	}

}
