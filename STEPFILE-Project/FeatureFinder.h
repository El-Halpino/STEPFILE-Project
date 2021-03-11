#pragma once
#include "STEP.h"
class FeatureFinder
{
private:
	void findMinMax(STEP stepDataObj);
	long double minX, maxX;
	long double minY, maxY;
	long double minZ, maxZ;


public:
	void featureFinderController(STEP stepDataObj);

};

