#pragma once
#include "curveevaluator.h"
#include "linearcurveevaluator.h"
#include<algorithm>

class C0Bezier :
    public LinearCurveEvaluator
{
protected:
    void evaluateCurve(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, const float& fAniLength, const bool& bWrap) const;
    virtual Point calculateY(Point p[], float t) const;
};

