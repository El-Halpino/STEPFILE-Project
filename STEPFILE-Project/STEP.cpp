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
	string advFaceNum;
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
						advFaceNum = currentLine.substr(0, currentLine.find(" "));
						faces.insert(advFaceNum); // insert adv face locations into a set for use later
					}
				}
		}
		StepFile.close();
		cout << "STEP File Closed\n";
		
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
		// Group features with sub features next
		for (auto item : faces) // Iterate through adv faces
		{
			while (lastNumberFound != true)
			{

			}
		}
	}
	return featureList;
}

map<string, string> STEP::stepController()
{
	cout << "Welcome to the STEP Class\n";
	map<string, string> featureList = extractFeatures();
	return featureList;
}