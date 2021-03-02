#include "STEP.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>

using namespace std;

map<string, string> STEP::extractFeatures()
{
	cout << "Extracting Features... \n";
	string currentLine;
	string stepNumber;
	map<string, string> dataList;
	map<string, string> featureList;
	set<string> faces;
	ifstream StepFile;

	set<string> foundlines;
	set<string> nextlines;
	bool numberFound = false;
	bool lastNumberFound = false;

	StepFile.open("C:\\work\\STEP\\Cube.STEP"); // Read STEP File
	if (!StepFile)
	{
		cout << "Unable to open STEP File\n";
		exit(1);
	}
	else 
	{
		cout << "STEP File Opened\n";
		while (getline (StepFile, currentLine)) // Cycle through each line
		{
				if (currentLine[0] == '#')
				{
					stepNumber = currentLine.substr(0, currentLine.find(" "));
					dataList.insert({ stepNumber, currentLine }); // insert data section into map
					if (currentLine.find(" ADVANCED_FACE ") != string::npos)
					{
						faces.insert(currentLine); // insert adv face locations into a set for use later
					}
				}
		}
		stepNumber = "";
		StepFile.close();
		cout << "STEP File Closed\n";
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
		*/
		// Group features with sub features next
		for (auto item : faces) // Iterate through adv faces
		{
			nextlines.insert(item);
			while (lastNumberFound != true) // runs until last number is found
			{
				for (auto nLine : nextlines) // cycles through set nextlines
				{
					cout << nLine << "\n";
					string subnLine = nLine.substr(nLine.find(" "), nLine.size()); // get sub string to avoid reading step's own ID
					for (char& ch : subnLine) // Cycles through each character in nLine
					{
						if (numberFound == true)
						{
							if (isdigit(ch))
							{
								stepNumber += ch;
							}
							else 
							{
								cout << "Found : " + stepNumber << "\n";
								numberFound = false;
								for (auto& it : dataList) {
									if (it.first == "#" + stepNumber)
									{
										foundlines.insert(it.second);
									}
								}
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
				nextlines.insert(foundlines.begin(), foundlines.end());
				foundlines.clear();
			}
			lastNumberFound = false;
		}
	}
	cout << "\nAdvanced Faces Found: " << faces.size() << "\n";
	return featureList;
}

map<string, string> STEP::stepController()
{
	cout << "Welcome to the STEP Class\n";
	map<string, string> featureList = extractFeatures();
	return featureList;
}