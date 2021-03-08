// STEPFILE-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
// SYSTEM CONTROLLER
#include <iostream>
#include <chrono>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <type_traits>

#include "STEP.h"
using namespace std;
/*TO DO*/
// Edit writeToFile() to allow any file to be passed with specified faces too!

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

    ofstream TestFile("C:\\Users\\alanh\\source\\repos\\STEPFILE-Project\\WriteTests\\testfile2.step"); // File created and opened
    /*
    TestFile.seekp(0, ios::end);
    if (!TestFile.tellp() == 0) // If the file is not empty then delete it
    { 
       remove("C:\\Users\\alanh\\source\\repos\\STEPFILE - Project\\WriteTests\\testfile2.step");
       cout << "File Deleted\n";
    }
    */
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
}

int main()
{
    STEP stepDataObj;
    auto start = chrono::steady_clock::now();// Start Clock
    stepDataObj.stepController();
    auto end = chrono::steady_clock::now();// End Clock
    std::cout << "\nTime Taken: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n\n";
    map<string, vector<string>> featureList = stepDataObj.stepFeatureList;

    for (auto key : featureList)
    {
        cout << "***********************************************************";
        cout << "\n" << "Key: "<< key.first << " Results\n\n";
        for (auto it = key.second.begin(); it != key.second.end(); ++it)
        {
            cout << *it << "\n";
        }
        cout << "***********************************************************\n";
    }

    writeToFile(stepDataObj);
    return 0;
}
