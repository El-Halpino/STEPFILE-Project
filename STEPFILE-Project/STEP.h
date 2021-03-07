#pragma once
#include <map>
#include <string>
#include <vector>

class STEP
{
private:
	std::map<std::string, std::vector<std::string>> extractFeatures();

public:	
	std::map<std::string, std::vector<std::string>> stepController();

};

