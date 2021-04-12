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

	void extractFeatures(string inputFile);
	void checkDifference();

public:	
	vector<string> headerLines;
	set<string> diffLines;
	map<string, vector<string>> stepFeatureList;
	map<string, vector<string>> vertexPoints;
	map<string, vector<string>> cartesianPoints;

	void stepController(string inputFile);
	
};

