// STEPFILE-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
// SYSTEM CONTROLLER
#include <iostream>
#include <chrono>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <filesystem>
#include "FeatureFinder.h"
#include "STEP.h"

using namespace std;
namespace fs = filesystem;

void printFeatureCoords(FeatureFinder featureObj, STEP stepDataObj)
{
    cout << "\n\nThe following high level features have been identified. Cuts need to be made at the vertex points displayed below." << "\n\n";
    set<string> holder;
    for (auto hLFeature : featureObj.highLevelFeatures)
    {
        cout << "High Level Feature : " << hLFeature.first << "\n";
        for (auto face : hLFeature.second)
        {
            for (auto point : stepDataObj.vertexPoints[face])
            {
                holder.insert(point);
            }
        }
        for (auto item : holder)
        {
            cout << item << "\n";
        }  
        holder.clear();
        system("pause");
    }
}

int main()
{
    string inputFile;
    STEP stepDataObj;
    // Read in STEP file names and display for user to choose from
    cout << "STEP File Location: STEPFILE-Project\\STEPFILES\\\n";
    string path = "STEPFILES/";
    for (const auto& entry : fs::directory_iterator(path))
        cout << entry.path() << endl;
    cout << "Enter the name of the STEP file (sans file extension)" << endl;
    cin >> inputFile;
    //inputFile = "CubeCUTS";
    auto start = chrono::steady_clock::now();// Start Clock
    stepDataObj.stepController(inputFile);
    FeatureFinder highLevelFeatureObj;
    highLevelFeatureObj.featureFinderController(stepDataObj);
    auto end = chrono::steady_clock::now();// End Clock
    cout << "\nTime taken to read STEP: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n\n";
    system("pause");
    printFeatureCoords(highLevelFeatureObj, stepDataObj);
    return 0;
}
