#include "Bspline.h"
#include "vec.h"
#include "linearcurveevaluator.h"
Point Bspline::calculateY(Point p[], float t) const
{
	float t2 = pow(t, 2);
	float t3 = pow(t, 3);
	Point Qt = ((p[0]-p[2]) * (-3.0)*t + p[0]*(3*t2-t3+1)-p[1]*(6*t2-3*t3-4)+p[2]*(3*t2-3*t3+1)+p[3]*t3)/6.0;
	return Qt;
}

void Bspline::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	int iCtrlPtCount = ptvCtrlPts.size();
	for (int subBegin = 0; subBegin < iCtrlPtCount - 3; subBegin += 1) {

		Point p[] = { ptvCtrlPts[subBegin] ,ptvCtrlPts[subBegin + 1] ,ptvCtrlPts[subBegin + 2] ,ptvCtrlPts[subBegin + 3] };
		float sample = 1.0 / (p[3].x > p[0].x ? (p[3].x - p[0].x) * 10 : (fAniLength + p[3].x - p[0].x) * 10);
		for (float t = 0; t <= 1; t += sample) {

			Point Qt = calculateY(p, t);
			ptvEvaluatedCurvePts.push_back(Qt);


		}
	}
	
	int iEvalCount = ptvEvaluatedCurvePts.size();
	if (bWrap) {
		
		if (iEvalCount < 3) {
			LinearCurveEvaluator::evaluateCurve(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength, bWrap);
		}
		else {
			Point p[] = { ptvEvaluatedCurvePts[iEvalCount -1] ,ptvEvaluatedCurvePts[(iEvalCount + 1) % iEvalCount] ,ptvEvaluatedCurvePts[(iEvalCount + 2) % iEvalCount] ,ptvEvaluatedCurvePts[(iEvalCount + 3) % iEvalCount] };
			p[1] = Point(p[1].x + fAniLength, p[1].y);
			p[2] = Point(p[2].x + fAniLength, p[2].y);
			p[3] = Point(p[3].x + fAniLength, p[3].y);

			float sample = 1 / ((fAniLength + p[3].x - p[0].x) * 10);
			for (float t = 0; t <= 1; t += sample) {

				Point Qt = C0Bezier::calculateY(p, t);
				ptvEvaluatedCurvePts.push_back(Qt);

			
				if (Qt.x > fAniLength) {
					Qt.x -= fAniLength;
				}
				ptvEvaluatedCurvePts.push_back(Qt);



			}
		}


	}
	else {
		if (iEvalCount < 4) {
			LinearCurveEvaluator::evaluateCurve(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength, bWrap);
		}
		else {
			Point e = Point(ptvEvaluatedCurvePts[iEvalCount - 1]);
			Point s = Point(ptvEvaluatedCurvePts[0]);
			e.x = fAniLength;
			s.x = 0;
			ptvEvaluatedCurvePts.push_back(e);
			ptvEvaluatedCurvePts.push_back(s);
		}

	}

	// connect end to begin
	/*if (iCtrlPtCount % 3 != 0 || (iCtrlPtCount <= 3 && !bWrap))
		for (int i = iCtrlPtCount < 3 ? 0 : (iCtrlPtCount - (iCtrlPtCount % 3)); i < iCtrlPtCount - 1; i++) {
			float dx = ptvCtrlPts[i + 1].x - ptvCtrlPts[i].x;
			float dy = (ptvCtrlPts[i + 1].y - ptvCtrlPts[i].y);
			float sample = 1 / (dx * 10);
			for (float t = 0; t <= 1; t += sample) {

				Point temp(dx * t + ptvCtrlPts[i].x, ptvCtrlPts[i].y + dy * t);
				if (temp.x > fAniLength) {
					temp.x -= fAniLength;
				}
				ptvEvaluatedCurvePts.push_back(temp);

			}
		}


	if (iCtrlPtCount % 3 != 0) {
		if (bWrap) {
			float dx = fAniLength + ptvCtrlPts[0].x - ptvCtrlPts[iCtrlPtCount - 1].x;
			float dy = (ptvCtrlPts[0].y - ptvCtrlPts[iCtrlPtCount - 1].y);
			float sample = 1 / (dx * 10);
			for (float t = 0; t <= 1; t += sample) {

				Point temp(dx * t + ptvCtrlPts[iCtrlPtCount - 1].x, ptvCtrlPts[iCtrlPtCount - 1].y + dy * t);
				if (temp.x > fAniLength) {
					temp.x -= fAniLength;
				}
				ptvEvaluatedCurvePts.push_back(temp);

			}
		}
		else {
			ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
			ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));

		}

	}*/



}



