// This class will be used to identify high level features

// make list of faces that touch

#include <iostream>
#include <iomanip>
#include <fstream>
#include "FeatureFinder.h"
#include "STEP.h"

using namespace std;

void FeatureFinder::findMinMax(STEP stepDataObj)
{                                       //          x                       y                    z
    // #796 = CARTESIAN_POINT ( 'NONE',  ( 28.20906519726944239, 20.00000000000000000, 13.79214587795902425 ) ) ;
    FeatureFinder::minX = 0;
    FeatureFinder::maxX = 0;
    FeatureFinder::minY = 0;
    FeatureFinder::maxY = 0;
    FeatureFinder::minZ = 0;
    FeatureFinder::maxZ = 0;
    string currentLine;
    string number;
    long double placeHolder;
    bool numberFound = false;
    int count = 0; // When count = 0, X && count = 1, Y && count = 2, Z

    for (auto key : stepDataObj.vertexPoints) // cycles through each feature
    {
        //cout << "\n" << "Key: " << key.first << " Results\n\n";
        for (auto it = key.second.begin(); it != key.second.end(); ++it) // Cycles through each sub feature
        {
                currentLine = *it;
                string subnLine = currentLine.substr(currentLine.find("="), currentLine.size()); // get sub string
                for (char& ch : subnLine) // Cycles through each character in subnLine
                {
                    if (numberFound == true)
                    {
                        if (ch == ',' || ch == ' ') // These charcters signal the end of the current number
                        {
                            char* char_arr;
                            char* end;
                            char_arr = &number[0];                           
                            placeHolder = strtod(char_arr, &end); // string to double 
                            switch (count) 
                            {
                            case 0: // X
                                if (placeHolder < minX)
                                {
                                    minX = placeHolder;
                                }
                                else if (placeHolder > maxX)
                                {
                                    maxX = placeHolder;
                                }
                                break;
                            case 1: // Y
                                if (placeHolder < minY) 
                                {
                                    minY = placeHolder;
                                }
                                else if (placeHolder > maxY) 
                                {
                                    maxY = placeHolder;
                                }
                                break;
                            case 2: // Z
                                if (placeHolder < minZ) 
                                {
                                    minZ = placeHolder;
                                }
                                else if (placeHolder > maxZ) 
                                {
                                    maxZ = placeHolder;
                                }
                                break;
                            }
                            count++;
                            number = "";
                            numberFound = false;
                        }
                        else 
                        {
                            number += ch;
                        }
                    }  
                    else if (ch == '-' || isdigit(ch))
                    {
                        number += ch;
                        numberFound = true;
                    }
                }
                count = 0;
        }
    }
    cout << "\n\n";
    cout << setprecision(24);
    cout << "Max X Value: " << maxX << "\n";
    cout << "Min X Value: " << minX << "\n";
    cout << "Max Y Value: " << maxY << "\n";
    cout << "Min Y Value: " << minY << "\n";
    cout << "Max Z Value: " << maxZ << "\n";
    cout << "Min Z Value: " << minZ << "\n";
}

void FeatureFinder::facesThatTouch(STEP stepDataObj)
{

}

void writeFile(STEP cubeObj)
{

    vector<string> header = cubeObj.headerLines;
    set<string> compileLines = cubeObj.diffLines;
    set<string> featureLines;
    ofstream TestFile("C:\\Users\\alanh\\source\\repos\\STEPFILE-Project\\WriteTests\\cubextended.step");
    
    for (auto key : cubeObj.stepFeatureList)
    {
        for (auto it = key.second.begin(); it != key.second.end(); ++it)
        {
            featureLines.insert(*it);
        }
    }
    for (auto line1 : header)
    {
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

void FeatureFinder::createCubeToFit(FeatureFinder mainObjFinder, FeatureFinder cubeFinder, STEP cubeObj)
{
    map < string, vector<string>> features = cubeObj.stepFeatureList;
    string number = "";
    string maxX = to_string(mainObjFinder.maxX);
    string maxY = to_string(mainObjFinder.maxY);
    string maxZ = to_string(mainObjFinder.maxZ);
    string minX = to_string(mainObjFinder.minX);
    string minY = to_string(mainObjFinder.minY);
    string minZ = to_string(mainObjFinder.minZ);
    bool numberFound = false;
    long double placeHolder;
    int count = 0;
    char start;
    string stepNumber;
    string stepNumber1;
    string newValue;

    // #796 = CARTESIAN_POINT ( 'NONE',  ( 28.20906519726944239, 20.00000000000000000, 13.79214587795902425 ) ) ;

    for (auto &item : cubeObj.stepFeatureList) // cycles through each set of vectors
    {
        std::cout << "\n\n" << item.first << "\n\n";
        for (auto &item2 : item.second) // cycles through each element of the vector
        {
            for (auto searchItem : cubeObj.cartesianPoints) // cycles through each set of vectors
            {
                for (auto searchItem2 : searchItem.second) // cycles through each element of the vector
                {
                    if (item2 == searchItem2) // If the current item === the search item. Then replace XYZ
                    {
                        string subnLine = item2.substr(item2.find("="), item2.size());
                        //cout << "BEFORE: " << item2 << "\n";
                        for (char& ch : subnLine) // Cycles through each character in item2
                        {
                            if (numberFound == true)
                            {
                                if (ch == ',' || ch == ' ') // These charcters signal the end of the current number
                                {
                                    char* char_arr;
                                    char* end;
                                    char_arr = &number[0];
                                    placeHolder = strtod(char_arr, &end); // string to double                        
                                    switch (count)
                                    {
                                    case 0: // X
                                        if (placeHolder == cubeFinder.minX)
                                        {
                                            newValue = item2.replace(item2.find(start, item2.find(number)), number.size(), minX);
                                            item2 = newValue;
                                        }
                                        else if (placeHolder == cubeFinder.maxX)
                                        {
                                            newValue = item2.replace(item2.find(start, item2.find(number)), number.size(), maxX);
                                            item2 = newValue;
                                        }
                                        break;
                                    case 1: // Y
                                        if (placeHolder == cubeFinder.minY)
                                        {
                                            newValue = item2.replace(item2.find(start, item2.find(number)), number.size(), minY);
                                            item2 = newValue;
                                        }
                                        else if (placeHolder == cubeFinder.maxY)
                                        {
                                            newValue = item2.replace(item2.find(start, item2.find(number)), number.size(), maxY);
                                            item2 = newValue;
                                        }
                                        break;
                                    case 2: // Z
                                        if (placeHolder == cubeFinder.minZ)
                                        {
                                            newValue = item2.replace(item2.find(start, item2.find(number)), number.size(), minZ);
                                            item2 = newValue;
                                        }
                                        else if (placeHolder == cubeFinder.maxZ)
                                        {
                                            newValue = item2.replace(item2.find(start, item2.find(number)), number.size(), maxZ);
                                            item2 = newValue;
                                        }
                                        break;
                                    }
                                    count++;
                                    number = "";
                                    numberFound = false;
                                }
                                else
                                {
                                    number += ch;
                                }

                            }
                            else if (ch == '-' || isdigit(ch))
                            {
                                numberFound = true;
                                number += ch;
                                start = ch;
                            }
                        }
                        count = 0;
                        std::cout << "REPLACED: " << item2 << "\n";
                    }
                    else {
                        continue;
                    }
                }
            }
        }
    }
    writeFile(cubeObj);
}

void FeatureFinder::featureFinderController(STEP stepDataObj)
{
    cout << "Welcome to the Feature Finder\n";
    STEP cubeObj;
    FeatureFinder cubeFinder;
    FeatureFinder mainObjFinder;
    cubeObj.stepController("Cube");  
    mainObjFinder.findMinMax(stepDataObj);
    cubeFinder.findMinMax(cubeObj);
    createCubeToFit(mainObjFinder, cubeFinder, cubeObj);
}
