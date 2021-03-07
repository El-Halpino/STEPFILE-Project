// STEPFILE-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
// SYSTEM CONTROLLER
#include <iostream>
#include <chrono>
#include <fstream>
#include <set>
#include <algorithm>
#include "STEP.h"
using namespace std;
/*TO DO*/
// Gather header section of STEP file for file writing
// Pass differences from checkDifference() for file writing
// Edit writeToFile() to allow any file to be passed (header input needs automation)

void writeToFile(map<string, vector<string>> featureList)
{
    set<string> featureLines;
    for (auto key : featureList)
    {
        for (auto it = key.second.begin(); it != key.second.end(); ++it)
        {
            featureLines.insert(*it);
        }
    }

    ofstream TestFile("C:\\Users\\alanh\\source\\repos\\STEPFILE-Project\\WriteTests\\testfile.step");

    TestFile << "ISO-10303-21;\nHEADER;\nFILE_DESCRIPTION (( 'STEP AP203' ),\n    '1' );\nFILE_NAME ('Cube.STEP',\n    '2020-10-26T15:18:52',\n(''),\n(''),\n'SwSTEP 2.0',\n'SolidWorks 2020',\n'' );\nFILE_SCHEMA(('CONFIG_CONTROL_DESIGN'));\nENDSEC;\n\nDATA;\n";
    for (auto line : featureLines)
    {
            TestFile << line << "\n";
    }
    TestFile << "ENDSEC;\nEND - ISO - 10303 - 21;";
    TestFile.close();
}

int main()
{
    STEP stepDataObj;
    auto start = chrono::steady_clock::now();// Start Clock
    map<string, vector<string>> featureList = stepDataObj.stepController();
    auto end = chrono::steady_clock::now();// End Clock
    std::cout << "\nTime Taken: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n\n";

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
    writeToFile(featureList);
    return 0;
}
