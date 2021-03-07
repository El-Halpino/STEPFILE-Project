#include "STEP.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

map<string, vector<string>> STEP::extractFeatures()
{
	std::cout << "Extracting Features... \n";
	string currentLine;
	string stepNumber;
	map<string, string> dataList;
	vector<string> faces;
	ifstream StepFile;

	map<string, vector<string>> featureList;
	vector<string> foundlines;
	vector<string> nextlines;
	bool numberFound = false;
	bool lastNumberFound = false;

	StepFile.open("C:\\work\\STEP\\Cube.STEP"); // Read STEP File
	if (!StepFile)
	{
		std::cout << "Unable to open STEP File\n";
		exit(1);
	}
	else 
	{
		std::cout << "STEP File Opened\n";
		while (getline (StepFile, currentLine)) // Cycle through each line
		{
				if (currentLine[0] == '#')
				{
					stepNumber = currentLine.substr(0, currentLine.find(" "));
					dataList.insert({ stepNumber, currentLine }); // insert data section into map
					if (currentLine.find(" ADVANCED_FACE ") != string::npos)
					{
						faces.push_back(currentLine); // insert adv face locations into a set for use later
						featureList[stepNumber].push_back(currentLine); // insert adv faces into feature list
					}
				}
		}
		stepNumber = "";
		currentLine = "";
		StepFile.close();
		std::cout << "STEP File Closed\n";

		/*
		cout << "Adv Faces Found\n";
		for (auto it = faces.begin(); it != faces.end(); it++)
		{
			std::cout << (*it) << "\n";
		}
		cout << "Data List; ";
		for (auto item : dataList)
		{
			cout << item.first << " " << item.second << "\n";
		}
		auto it = dataList.find("#36");
		std::cout << it->second;
		*/#

		// Group features with sub features next
		for (auto item : faces) // Iterate through adv faces
		{
			currentLine = item.substr(0, item.find(" ")); // CurrentLine used to identify the current adv face
			std::cout << "\n\n\nNEW FACE " << " " << item << "\n\n";
			nextlines.insert(nextlines.begin(), item); // Must insert the adv face at the start of the list

			while (lastNumberFound == false) // runs until last number is found
			{
				for (auto nLine : nextlines) // cycles through set nextlines
				{
					//std::cout << "\n" << nLine;
					string subnLine = nLine.substr(nLine.find(" "), nLine.size()); // get sub string to avoid reading step's own ID
					for (char& ch : subnLine) // Cycles through each character in subnLine
					{
						
						if (numberFound == true)
						{
							if (isdigit(ch))
							{
								stepNumber += ch;
							}
							else 
							{
								std::cout << "\nFound : #" + stepNumber;
								foundlines.push_back(dataList["#" + stepNumber]); // add found numbers to foundlines for next iteration
								featureList[currentLine].push_back(dataList["#" + stepNumber]); // Add found step into currentline's list of steps
								numberFound = false;
								stepNumber = "";
							}
						}
						else if (ch == '#')
						{
							numberFound = true;
						}	
					}
					if (foundlines.size() == 0)
					{
						lastNumberFound = true;
					}
				}
				nextlines.clear();
				for (int i = 0; i < foundlines.size(); i++)
					nextlines.push_back(foundlines[i]);
				foundlines.clear();
			} // end while
			lastNumberFound = false;
		}
	}
	std::cout << "\nAdvanced Faces Found: " << faces.size() << "\n";
	return featureList;
}

map<string, vector<string>> STEP::stepController()
{
	std::cout << "Welcome to the STEP Class\n";
	map<string, vector<string>> featureList = extractFeatures();
	return featureList;
}