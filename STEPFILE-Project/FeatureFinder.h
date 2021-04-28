#pragma once
#include "STEP.h"
class FeatureFinder
{
private:
	long double minX, maxX;
	long double minY, maxY;
	long double minZ, maxZ;
	string minXorg, maxXorg, minYorg, maxYorg, minZorg, maxZorg;

	void findMinMax(STEP stepDataObj);
	void createCubeToFit(STEP cubeObj, STEP stepDataObj);
	void identifyHighLevelFeatures(STEP stepDataObj, STEP cubeObj);
	
public:
	void featureFinderController(STEP stepDataObj);
	map<int, set<string>> highLevelFeatures;

};

