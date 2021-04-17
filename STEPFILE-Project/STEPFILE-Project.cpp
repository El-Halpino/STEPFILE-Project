// STEPFILE-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
// SYSTEM CONTROLLER
#include <iostream>
#include <chrono>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <type_traits>

#include "FeatureFinder.h"
#include "STEP.h"
using namespace std;
/*TO DO*/
// Edit writeToFile() to allow any file to be passed with specified faces

void writeToFile(STEP stepDataObj)
{
    map<string, vector<string>> featureList = stepDataObj.stepFeatureList;
    vector<string> header = stepDataObj.headerLines;
    set<string> compileLines = stepDataObj.diffLines;
    set<string> featureLines;
    
    for (auto key : featureList)
    {
        for (auto it = key.second.begin(); it != key.second.end(); ++it)
        {
            featureLines.insert(*it);
        }
    } 
    //ofstream TestFile("C:\\Users\\alanh\\source\\repos\\STEPFILE-Project\\WriteTests\\testfile3.step"); // File created and opened
    // Writes each face into separate files
    /*for (auto key : featureList)
    {
        cout << "\nWriting File: " << key.first << "\n";
        string FilePath = ("C:\\Users\\alanh\\source\\repos\\STEPFILE-Project\\WriteTests\\" + key.first + ".step");
        ofstream AdvFace(FilePath.c_str());
        if (!AdvFace)
        {
            cout << "Failed to create file\n";
        }
        else
        {
            for (auto line1 : header)
            {
                AdvFace << line1 << "\n";
            }
            for (auto it = key.second.begin(); it != key.second.end(); ++it)
            {
                AdvFace << *it << "\n";
            }
            for (auto line3 : compileLines)
            {
                AdvFace << line3 << "\n";
            }
            AdvFace << "ENDSEC;\nEND - ISO - 10303 - 21;";
            AdvFace.close();
        }
    } */
    /* // Writes whole object into one file
    cout << "\nCurrent File: \n\n";
    for (auto line1 : header)
    {
        cout << line1 << "\n";
        TestFile << line1 << "\n";
    }
    for (auto line2 : featureLines)
    {
            TestFile << line2 << "\n";
    }
    for (auto line3 : compileLines)
    {
        TestFile << line3 << "\n";
    }
    TestFile << "ENDSEC;\nEND - ISO - 10303 - 21;";
    TestFile.close(); // file closed
    */  
}

int main()
{
    string inputFile;
    STEP stepDataObj;
    cout << "STEP File Location: C:\\work\\STEP\\\n";
    cout << "Enter the name of the STEP file\n";
    //cin >> inputFile;
    inputFile = "CubeSlot";
    auto start = chrono::steady_clock::now();// Start Clock
    stepDataObj.stepController(inputFile);
    auto end = chrono::steady_clock::now();// End Clock
    cout << "\nTime taken to read STEP: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n\n";
    system("pause");
    string currentLine;
    map<string, vector<string>> featureList = stepDataObj.stepFeatureList;
   /*
    for (auto key : featureList) 
    {
        cout << "***********************************************************";
        cout << "\n" << "Key: " << key.first << " Results\n\n";
        for (auto it = key.second.begin(); it != key.second.end(); ++it)
        {
            currentLine = *it;
            if (currentLine.find(" EDGE_CURVE ") != string::npos)
            {
                cout << currentLine << "\n";
            }
        }
        cout << "***********************************************************\n";
    }
    */
   //writeToFile(stepDataObj);
    FeatureFinder highLevelFeatureObj;
    highLevelFeatureObj.featureFinderController(stepDataObj);
    return 0;
}
