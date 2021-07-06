#pragma once
#include "linearcurveevaluator.h"

class C2Hermite :
    public LinearCurveEvaluator
{
public:
	Point calculateY(Point p[], float t) const;
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
};

