#pragma once
#include "STEP.h"
class FeatureFinder
{
private:
	long double minX, maxX;
	long double minY, maxY;
	long double minZ, maxZ;

	void findMinMax(STEP stepDataObj);
	void createCubeToFit(FeatureFinder mainObj);
	
public:
	void featureFinderController(STEP stepDataObj);
	map<string, vector<string>> touchingFaces;
};

