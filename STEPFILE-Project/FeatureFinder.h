#pragma once
#include "STEP.h"
class FeatureFinder
{
private:
	long double minX, maxX;
	long double minY, maxY;
	long double minZ, maxZ;

	void findMinMax(STEP stepDataObj);
	void createCubeToFit(FeatureFinder mainObj, FeatureFinder cubeFinder, STEP cubeObj);
	void facesThatTouch(STEP stepDataObj);
	
public:
	void featureFinderController(STEP stepDataObj);

};

