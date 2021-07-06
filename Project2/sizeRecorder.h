#pragma once
#include  <limits.h> 
#include <FL/gl.h>
#include "mat.h"

class SizeRecorder
{
public:
	static SizeRecorder* getInstance();
	double maxCoord[3] = {INT_MIN,INT_MIN ,INT_MIN };
	double minCoord[3] = { INT_MAX ,INT_MAX ,INT_MAX };
	void record();
private:
	SizeRecorder();
};

