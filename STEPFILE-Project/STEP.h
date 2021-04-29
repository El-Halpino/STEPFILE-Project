/*
This work is licensed under the  Creative Commons Attribution-ShareAlike 4.0 International Licence.
To view of this licence, visit http://creativecommons.org/licenses/by-sa/4.0/.
*/
/*! \class STEP.h
	\brief
	\author Alan Halpin
	\date 29/04/2021
	\copyright Creative Commons Attribution-ShareAlike 4.0 International Licence
*/
#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;
class STEP
{
private:
	map<string, string> stepDataList;
	map<string, vector<string>> edgeCurves;

	void extractFeatures(string inputFile);
	void checkDifference();
	void checkFacesThatTouch();
	//void findEdgeCurves();
	 
public:	
	vector<string> headerLines;
	set<string> diffLines;
	map<string, vector<string>> stepFeatureList;
	map<string, vector<string>> vertexPoints;
	map<string, vector<string>> cartesianPoints;
	map<string, vector<string>> touchingFaces;
	map<string, map<string, vector<string>>> edgeCurveGeometry;

	void stepController(string inputFile);	
};