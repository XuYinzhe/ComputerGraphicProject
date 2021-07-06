#include "C0Bezier.h"
#include "vec.h"

void C0Bezier::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	int iCtrlPtCount = ptvCtrlPts.size();
	for (int subBegin = 0; subBegin <= iCtrlPtCount - 3; subBegin += 3) {

		Point p[] = { ptvCtrlPts[subBegin] ,ptvCtrlPts[subBegin + 1] ,ptvCtrlPts[subBegin + 2] ,ptvCtrlPts[(subBegin + 3) % iCtrlPtCount] };
		float sample = 1 / (p[3].x > p[0].x ? (p[3].x - p[0].x) * 10 : (fAniLength + p[3].x - p[0].x) * 10);
		for (float t = 0; t <= 1; t += sample) {

			float y = calculateY(p, t).y;
			if (p[3].x > p[0].x)
				ptvEvaluatedCurvePts.push_back(Point((p[3].x - p[0].x) * t + p[0].x, y));
			else {
				if (bWrap) {
					Point temp((fAniLength + p[3].x - p[0].x) * t + p[0].x, y);
					if (temp.x > fAniLength) {
						temp.x -= fAniLength;
						if (temp.x > min(p[1].x, min(p[2].x, p[3].x))) {
							return;
						}
					}
					ptvEvaluatedCurvePts.push_back(temp);
				}
				else {
					ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
					ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
				}


			}

		}
	}


	if (iCtrlPtCount % 3 != 0 || (iCtrlPtCount <= 3 && !bWrap))//Connect points left
		for (int i = iCtrlPtCount <= 3?0:(iCtrlPtCount - (iCtrlPtCount %3)); i < iCtrlPtCount - 1; i++) {
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


	if (iCtrlPtCount % 3 != 0) {//connect begin and end
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

	}







}

Point C0Bezier::calculateY(Point p[], float t) const
{
	float t2 = pow(t, 2);
	float t3 = pow(t, 3);
	Point Qt = (p[0] * (-3 * t + 3 * t2 - t3 + 1) + p[1] * (3 * t - 6 * t2 + 3 * t3) + p[2] * (3 * t2 - 3 * t3) + p[3] * t3);
	return Qt;
}
