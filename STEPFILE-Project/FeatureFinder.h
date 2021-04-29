/*
This work is licensed under the  Creative Commons Attribution-ShareAlike 4.0 International Licence.
To view of this licence, visit http://creativecommons.org/licenses/by-sa/4.0/.
*/
/*! \class FeatureFinder.h
	\brief
	\author Alan Halpin
	\date 29/04/2021
	\copyright Creative Commons Attribution-ShareAlike 4.0 International Licence
*/
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

