#pragma once
#include "Bspline.h"
#include "modelerapp.h"

class CatmullRom :
    public Bspline
{
    virtual Point calculateY(Point p[], float t) const;
    virtual void evaluateCurve(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, const float& fAniLength, const bool& bWrap) const;
};

