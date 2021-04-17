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
	void findEdgeCurves();

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

