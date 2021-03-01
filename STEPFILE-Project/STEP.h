#pragma once
#include <map>
#include <string>

class STEP
{
private:
	std::map<std::string, std::string> extractFeatures();

public:	
	std::map<std::string, std::string> stepController();
};

