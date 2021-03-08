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

	void extractFeatures();
	void checkDifference();

public:	
	vector<string> headerLines;
	set<string> diffLines;
	map<string, vector<string>> stepFeatureList;

	void stepController();
	
};

