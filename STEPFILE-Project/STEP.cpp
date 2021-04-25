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
	map<string, vector<string>> cartPoints;
	vector<string> foundlines;
	vector<string> nextlines;
	set<string> subFeatures;
	bool numberFound = false;
	bool lastNumberFound = false;
	bool header = true;
	bool vPoint = false;

	string FilePath = ("STEPFILES/" + inputFile + ".step");
	//string FilePath = ("C:/work/STEP/" + inputFile + ".step");
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
					if (nLine.find(" EDGE_CURVE ") != string::npos)
					{
						STEP::edgeCurves[currentLine].push_back(nLine);
					}
					if (nLine.find(" VERTEX_POINT ") != string::npos)
					{
						vPoint = true;
					}
					else if (nLine.find(" CARTESIAN_POINT ") != string::npos)
					{
						cartPoints[currentLine].push_back(nLine);
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
	for (auto item : cartPoints)
	{
		v = item.second;
		sort(v.begin(), v.end());
		rd = unique(v.begin(), v.end());
		v.resize(distance(v.begin(), rd));
		cartPoints[item.first] = v;
	}
	STEP::stepFeatureList = featureList;
	STEP::vertexPoints = vPoints;
	STEP::cartesianPoints = cartPoints;
	std::cout << "\nAdvanced Faces Found: " << faces.size() << "\n";
	checkDifference();
}

void STEP::checkFacesThatTouch()
{ // 2 faces touch if they share a vertex point with the same geometrical location (X,Y,Z)
	map<string, vector<string>> touchingFaces;
	string subLine, subLine1;
	for (auto step : STEP::vertexPoints)
	{
		for (auto step2 : STEP::vertexPoints)
		{
			if (step != step2)
			{
				for (auto item : step.second)
				{
					subLine = item.substr(item.find(" "), item.size());
					for (auto item2 : step2.second)
					{
						subLine1 = item2.substr(item2.find(" "), item2.size());
						if (subLine == subLine1)
						{
							touchingFaces[step.first].push_back(step2.first); 
							goto next;
						}
					}
				}
			}
		next:
			continue;
		}
	}
	STEP::touchingFaces = touchingFaces;
	/*
	for (auto item : touchingFaces)
	{
		cout << "Face: " << item.first	 << "\n";
		for (auto item2 : item.second)
		{
			cout << item2 << "\n";
		}
	}
	*/
}

void STEP::findEdgeCurves() // could be used to identify more complex objects.
{
	string subLine, stepNumber, currentLine, edgeNumber;
	vector<string> foundlines, nextLines;
	bool numberFound, lastNumberFound;
	numberFound = false;
	lastNumberFound = false;

	for (auto key : STEP::edgeCurves) // Cycle through faces
	{
		for (auto item : key.second) //  Cycle through edge curves in current face
		{
			edgeNumber = item.substr(0 , item.find(" "));
			//cout <<"Edge : " << STEP::stepDataList[edgeNumber] << "\n";
			nextLines.insert(nextLines.begin(), item); // Must be inserted at the beginning
			while (lastNumberFound == false)
			{
				for (auto nextL : nextLines)
				{
					subLine = nextL.substr(nextL.find(" "), nextL.size()); // sub string to avoid reading current step number
					for (char& ch : subLine) // cycle through chars in current line
					{
						//
						if (numberFound == true)
						{
							if (isdigit(ch))
							{
								stepNumber += ch;
							}
							else
							{
								foundlines.push_back(STEP::stepDataList["#" + stepNumber]);
								STEP::edgeCurveGeometry[key.first][edgeNumber].push_back(STEP::stepDataList["#" + stepNumber]);
								numberFound = false;
								stepNumber = "";
							}
						}
						else if (ch == '#')
						{
							numberFound = true;
						}
						//
					}
					if (foundlines.size() == 0)
					{
						lastNumberFound = true;
					}
				}
				nextLines.clear();
				for (int i = 0; i < foundlines.size(); i++)
					nextLines.push_back(foundlines[i]);
				foundlines.clear();
			}
			lastNumberFound = false;
		}
	}
	
	/*
	for (auto key : edgeCurveGeometry)
	{
		cout << "\n\n\nFace : " << key.first << "\n";
		for (auto edge : edgeCurveGeometry[key.first])
		{
			cout << "Edge : \n" << stepDataList[edge.first] << "\n";
			for (auto item : edgeCurveGeometry[key.first][edge.first])
			{
				cout << item << "\n";
			}
		}
	}
	*/
}

// This method controls the step class
void STEP::stepController(string inputFile)
{
	cout << "File name: " << inputFile << "\n";
	extractFeatures(inputFile);
	checkFacesThatTouch();
	//findEdgeCurves();
}