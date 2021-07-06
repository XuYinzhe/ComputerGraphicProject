#include "C2Hermite.h"
#include "vec.h"
#include "mat.h"

Point C2Hermite::calculateY(Point p[], float t) const
{
	float t2 = pow(t, 2);
	float t3 = pow(t, 3);
	Point Qt = p[2]*t+p[0]*(-3*t2+2*t3+1)+p[1]*(3*t2-2*t3)-p[2]*(2*t2-t3)-p[3]*(t2-t3);
	return Qt;
}

void C2Hermite::evaluateCurve(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, const float& fAniLength, const bool& bWrap) const
{
	int iCtrlPtCount = ptvCtrlPts.size();
	ptvEvaluatedCurvePts.clear();

	if (iCtrlPtCount > 3) {
	std::vector<Point> RHS;
		std::vector<Vec3f> LHS;
		std::vector<Point> D(iCtrlPtCount);

		RHS.reserve(iCtrlPtCount + 2);
		LHS.reserve(iCtrlPtCount + 2);
		//D.reserve(iCtrlPtCount + 2);

		// "Matrix" init
		RHS.push_back((Point(ptvCtrlPts[1]) - ptvCtrlPts[0]) * 3);
		LHS.push_back(Vec3f(0, 2, 1));
		//RHS.push_back((Point(ptvCtrlPts[2]) - ptvCtrlPts[0]) * 3);
		//LHS.push_back(Vec3f(1, 4, 1));

		for (int i = 2; i < iCtrlPtCount; i++) {
			RHS.push_back((Point(ptvCtrlPts[i]) - ptvCtrlPts[i-2]) * 3);
			LHS.push_back(Vec3f(1, 4, 1));
		}

		if (iCtrlPtCount > 3) {
			RHS.push_back((Point(ptvCtrlPts[iCtrlPtCount-1]) - ptvCtrlPts[iCtrlPtCount-2]) * 3);
			LHS.push_back(Vec3f(1, 2, 0));
		}

		//Forward Elimination
		for (int i = 1; i < iCtrlPtCount ;i++) {
			LHS[i][1] = LHS[i][1] - (LHS[i][0] / LHS[i-1][1]) * LHS[i-1][1];
			LHS[i][0] = 0;
			RHS[i] = RHS[i] - RHS[i-1] * (LHS[i][0] / LHS[i - 1][1]);
		}

		//Backward subsititution
		D[iCtrlPtCount-1] = RHS[iCtrlPtCount-1] / LHS[iCtrlPtCount-1][1];
		for (int i = iCtrlPtCount-2; i>=0; i--) {
			D[i] = (RHS[i]-D[i+1]* LHS[i][2]) / LHS[i][1];
		}

		//Curve compute

		float maxX = -1;
		for (int subBegin = 0; subBegin < iCtrlPtCount-1 ; subBegin ++) {

			Point p[] = { ptvCtrlPts[subBegin] ,ptvCtrlPts[subBegin + 1] ,D[subBegin ] ,D[subBegin + 1] };
			float sample = 1 / ((p[1].x - p[0].x) * 30);
			for (float t = 0; t <= 1; t += sample) {

				Point Qt = calculateY(p, t);
				if (Qt.x >= maxX) {
					ptvEvaluatedCurvePts.push_back(Qt);
					maxX = Qt.x;
				}

			}
			//ptvEvaluatedCurvePts.push_back(ptvCtrlPts[subBegin + 1]);
		}

		if (bWrap) {
			Point p[] = { ptvCtrlPts[iCtrlPtCount - 1] ,Point(ptvCtrlPts[0].x + fAniLength,ptvCtrlPts[0].y) ,D[iCtrlPtCount - 1] ,D[0] };
			float sample = 1 / ((p[1].x - p[0].x) * 10);
			for (float t = 0; t <= 1; t += sample) {

				Point Qt = calculateY(p, t);

				if (Qt.x > fAniLength) {
					Qt.x -= fAniLength;

				}
				ptvEvaluatedCurvePts.push_back(Qt);


			}
		}
		else
		{
			Point e = Point(ptvEvaluatedCurvePts[ptvEvaluatedCurvePts.size() - 1]);
			Point s = Point(ptvEvaluatedCurvePts[0]);
			e.x = fAniLength;
			s.x = 0;
			ptvEvaluatedCurvePts.push_back(e);
			ptvEvaluatedCurvePts.push_back(s);
		}
	}
	else {
		LinearCurveEvaluator::evaluateCurve(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength, bWrap);
	}
	

}
