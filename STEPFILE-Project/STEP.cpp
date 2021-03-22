#include "STEP.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;
// This method is used to identify the lines in the step file that are unrelated to the adv face features
// These lines are essential for building the STEP file
void STEP::checkDifference()
{
	map<string, vector<string>> featureList = STEP::stepFeatureList;
	map<string, string> dataList = STEP::stepDataList;
	set<string> featureLines; 
	set<string> dataLines;
	set<string> result;

	for (auto key : featureList)
	{
		for (auto it = key.second.begin(); it != key.second.end(); ++it)
		{
			featureLines.insert(*it);
		}
	}
	for (auto key2 : dataList)
	{
		dataLines.insert(key2.second);
	}
	// Compare features to data to see what's missing
	set_difference(dataLines.begin(), dataLines.end(), featureLines.begin(), featureLines.end(),inserter(result, result.begin()));
	/*
	cout << "\n\nDIFFERENCES\n\n";
	for (auto diff : result)
	{
		cout << diff << "\n"; // If these lines are missing from STEP, it won't compile in STEP readers
	}
	*/
	STEP::diffLines = result;
}

// This method reads a step file and creates a datalist and feature list
void STEP::extractFeatures(string inputFile)
{
	// Declare Variables and DS
	string currentLine;
	string stepNumber;
	string multipleLineSTEP;
	map<string, string> dataList;
	vector<string> faces;
	ifstream StepFile;

	map<string, vector<string>> featureList;
	map<string, vector<string>> vPoints;
	vector<string> foundlines;
	vector<string> nextlines;
	set<string> subFeatures;
	bool numberFound = false;
	bool lastNumberFound = false;
	bool header = true;
	bool vPoint = false;

	string FilePath = ("C:\\work\\STEP\\" + inputFile + ".step");

	std::cout << "Extracting Features... \n";
	StepFile.open(FilePath.c_str()); // Read STEP File
	if (!StepFile)
	{
		std::cout << "Unable to open STEP File\n";
		exit(1);
	}
	else 
	{
		// Read STEP file's contents into memory
		std::cout << "STEP File Opened\n";
		while (getline (StepFile, currentLine)) // Cycle through each line
		{
			if (currentLine[0] == '#')
			{
				header = false;
				if (currentLine.find(";") != string::npos) // if the line ends
				{
					stepNumber = currentLine.substr(0, currentLine.find(" "));
					dataList.insert({ stepNumber, currentLine }); // insert data section into map
					if (currentLine.find(" ADVANCED_FACE ") != string::npos)
					{
						faces.push_back(currentLine); // insert adv face locations into a set for use later
						featureList[stepNumber].push_back(currentLine); // insert adv faces into feature list
					}
				}
				else // line doesn't end, it is a multiple line step
				{
					stepNumber = currentLine.substr(0, currentLine.find(" "));
					multipleLineSTEP = currentLine;
				}
				continue;
			}
			else if (header == true) // add to header
			{
				STEP::headerLines.push_back(currentLine);
				continue;
			}
			else if (multipleLineSTEP != "") // If the step has multiple lines, append additional lines onto string
			{
				if (currentLine.find(";") != string::npos) // If it is the last line (end is denoted by ";")
				{
					multipleLineSTEP.append(currentLine);
					dataList.insert({ stepNumber, multipleLineSTEP });
					multipleLineSTEP = "";
				}
				else // Not end, so append 
				{
					multipleLineSTEP.append(currentLine);
				}
				continue;
			}
			else // Current line is not data or header, so continue
			{
				continue;
			}
		}
		STEP::stepDataList = dataList;
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
		*/

		// Store features with sub features next
		for (auto item : faces) // Iterate through adv faces
		{
			currentLine = item.substr(0, item.find(" ")); // CurrentLine used to identify the current adv face
			//std::cout << "\nNEW FACE " << " " << item << "\n";
			nextlines.insert(nextlines.begin(), item); // Must insert the adv face at the start of the list

			while (lastNumberFound == false) // runs until last number is found
			{
				for (auto nLine : nextlines) // cycles through set nextlines
				{

					if (nLine.find(" VERTEX_POINT ") != string::npos)
					{
						vPoint = true;
					}

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
								//std::cout << "\nFound : #" + stepNumber;
								foundlines.push_back(dataList["#" + stepNumber]); // add found numbers to foundlines for next iteration
								featureList[currentLine].push_back(dataList["#" + stepNumber]); // Add found step into currentline's list of steps

								if (vPoint == true)
								{
									vPoints[currentLine].push_back(dataList["#" + stepNumber]);
									vPoint = false;
								}

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

	// Remove Duplicates
	vector<string>::iterator rd;
	vector<string> v;
	for(auto item : featureList)
	{
		v = item.second;
		sort(v.begin(), v.end());
		rd = unique(v.begin(), v.end());
		v.resize(distance(v.begin(), rd));
		featureList[item.first] = v;
	}
	for (auto item : vPoints)
	{
		v = item.second;
		sort(v.begin(), v.end());
		rd = unique(v.begin(), v.end());
		v.resize(distance(v.begin(), rd));
		vPoints[item.first] = v;
	}
	STEP::stepFeatureList = featureList;
	STEP::vertexPoints = vPoints;
	std::cout << "\nAdvanced Faces Found: " << faces.size() << "\n";
	checkDifference();
}

// This method controls the step class
void STEP::stepController(string inputFile)
{
	extractFeatures(inputFile);
}