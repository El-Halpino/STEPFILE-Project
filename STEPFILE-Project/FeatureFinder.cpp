// This class will be used to identify high level features
#include <iostream>
#include <iomanip>
#include <fstream>
#include "FeatureFinder.h"
#include "STEP.h"

using namespace std;

void FeatureFinder::findMinMax(STEP stepDataObj)
{                                       //          x                       y                    z
    // #796 = CARTESIAN_POINT ( 'NONE',  ( 28.20906519726944239, 20.00000000000000000, 13.79214587795902425 ) ) ;
    FeatureFinder::minX = 10000000;
    FeatureFinder::maxX = -10000000;
    FeatureFinder::minY = 10000000;
    FeatureFinder::maxY = -10000000;
    FeatureFinder::minZ = 10000000;
    FeatureFinder::maxZ = -10000000;
    FeatureFinder::maxXorg, minXorg, maxYorg, minYorg, maxZorg, minZorg;
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
                                    minXorg = number;
                                }
                                else if (placeHolder > maxX)
                                {
                                    maxX = placeHolder;
                                    maxXorg = number;
                                }
                                break;
                            case 1: // Y
                                if (placeHolder < minY) 
                                {
                                    minY = placeHolder;
                                    minYorg = number;
                                }
                                else if (placeHolder > maxY) 
                                {
                                    maxY = placeHolder;
                                    maxYorg = number;
                                }
                                break;
                            case 2: // Z
                                if (placeHolder < minZ) 
                                {
                                    minZ = placeHolder;
                                    minZorg = number;
                                }
                                else if (placeHolder > maxZ) 
                                {
                                    maxZ = placeHolder;
                                    maxZorg = number;
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
    /*
    cout << "\n\n";
    cout << setprecision(19);
    cout << "Max X Value: " << maxX << "\n";
    cout << "Min X Value: " << minX << "\n";
    cout << "Max Y Value: " << maxY << "\n";
    cout << "Min Y Value: " << minY << "\n";
    cout << "Max Z Value: " << maxZ << "\n";
    cout << "Min Z Value: " << minZ << "\n"; 
    */
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

void FeatureFinder::createCubeToFit(STEP cubeObj, STEP stepDataObj)
{
    FeatureFinder cubeFinder;
    cubeFinder.findMinMax(cubeObj); // Find min / max of cube

    map < string, vector<string>> features = cubeObj.stepFeatureList;
    string number = "";
    string maxX = to_string(FeatureFinder::maxX);
    string maxY = to_string(FeatureFinder::maxY);
    string maxZ = to_string(FeatureFinder::maxZ);
    string minX = to_string(FeatureFinder::minX);
    string minY = to_string(FeatureFinder::minY);
    string minZ = to_string(FeatureFinder::minZ);
    string stepNumber;
    string stepNumber1;
    string newValue;
    bool numberFound = false;
    long double placeHolder;
    int count = 0;
    char start;

    // #796 = CARTESIAN_POINT ( 'NONE',  ( 28.20906519726944239, 20.00000000000000000, 13.79214587795902425 ) ) ;

    for (auto &item : cubeObj.stepFeatureList) // cycles through each set of vectors
    {
        //std::cout << "\n\n" << item.first << "\n\n";
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
                                            newValue = item2.replace(item2.find(start, item2.find(number)), number.size(), FeatureFinder::minXorg);
                                            item2 = newValue;
                                        }
                                        else if (placeHolder == cubeFinder.maxX)
                                        {
                                            newValue = item2.replace(item2.find(start, item2.find(number)), number.size(), FeatureFinder::maxXorg);
                                            item2 = newValue;
                                        }
                                        break;
                                    case 1: // Y
                                        if (placeHolder == cubeFinder.minY)
                                        {
                                            newValue = item2.replace(item2.find(start, item2.find(number)), number.size(), FeatureFinder::minYorg);
                                            item2 = newValue;
                                        }
                                        else if (placeHolder == cubeFinder.maxY)
                                        {
                                            newValue = item2.replace(item2.find(start, item2.find(number)), number.size(), FeatureFinder::maxYorg);
                                            item2 = newValue;
                                        }
                                        break;
                                    case 2: // Z
                                        if (placeHolder == cubeFinder.minZ)
                                        {
                                            newValue = item2.replace(item2.find(start, item2.find(number)), number.size(), FeatureFinder::minZorg);
                                            item2 = newValue;
                                        }
                                        else if (placeHolder == cubeFinder.maxZ)
                                        {
                                            newValue = item2.replace(item2.find(start, item2.find(number)), number.size(), FeatureFinder::maxZorg);
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
                        //std::cout << "REPLACED: " << item2 << "\n";
                    }
                    else {
                        continue;
                    }
                }
            }
        }
    }

    writeFile(cubeObj);
    identifyHighLevelFeatures(stepDataObj, cubeObj);
}

void FeatureFinder::identifyHighLevelFeatures(STEP stepDataObj, STEP cubeObj) 
{
    /*
    map<string, vector<string>> touchingFaces;
	map<string, map<string, vector<string>>> edgeCurveGeometry;
    Adv face, Edge, Sub-features
    */
    // First, compare the cube created to see what points are not shared.
    set<string> linesNotShared;
    string subLine, subLine1, subSearch, subSearch1;
    bool found = false;

    for (auto advFace : stepDataObj.stepFeatureList)
    {
        for (auto vPoint : advFace.second)
        {
            for (auto searchItem : stepDataObj.vertexPoints[advFace.first])
            {
                subSearch = searchItem.substr(0, searchItem.find(" "));
                if (subSearch == vPoint.substr(0, vPoint.find(" ")))
                {
                    subLine = vPoint.substr(vPoint.find("("), vPoint.size());
                    //cout << "\n\n" << vPoint << "\n\n\n";
                    for (auto advFace1 : cubeObj.stepFeatureList)
                    {
                        for (auto vPoint2 : advFace1.second)
                        {
                            for (auto searchItem2 : cubeObj.vertexPoints[advFace1.first])
                            {
                                subSearch1 = searchItem2.substr(0, searchItem2.find(" "));
                                if (subSearch1 == vPoint2.substr(0, vPoint2.find(" ")))
                                {
                                    //cout << vPoint2 << "\n";
                                    subLine1 = vPoint2.substr(vPoint2.find("("), vPoint2.size());
                                    if (subLine == subLine1)
                                    {
                                        found = true;
                                    }
                                    else {
                                        continue;
                                    }
                                }
                            }
                                
                        }
                    }
                    if (found == false) // Current point has not been found, add to list
                    {
                        linesNotShared.insert(vPoint);
                    }
                    else {
                        found = false;
                    }  
                }
                else {
                    continue;
                }
            }
        }
    }
    
    cout << "Points not shared: " << linesNotShared.size() << "\n";
    for (auto item : linesNotShared)
    {
        cout << item << "\n";
    }
    // Which faces do these points belong to?
    map<string, set<string>> points;
    for (auto advFace : stepDataObj.stepFeatureList)
    {
        for (auto item : linesNotShared)
        {
            for (auto step : advFace.second)
            {
                if (item == step)
                {
                    points[advFace.first].insert(item);
                }
            }
        }
    }
    for (auto item : points)
    {
        cout << item.first << "\n";
        for (auto item2 : item.second)
        {
            cout << item2 << "\n";
        }
    }



    // Identify a slot 
    /*
    for (auto advFace : stepDataObj.edgeCurveGeometry) // iterate faces
    {
        for (auto advFace1 : stepDataObj.edgeCurveGeometry) // iterate other faces for comparison
        {
            if (advFace != advFace1)
            {
                for (auto edge : stepDataObj.edgeCurveGeometry[advFace.first]) // iterate edges
                {
                    for (auto edge1 : stepDataObj.edgeCurveGeometry[advFace1.first])
                    {
                        for (auto item : stepDataObj.edgeCurveGeometry[advFace.first][edge.first]) // iterate items
                        {
                            //cout << "ITEM1: " << item << "\n\n";
                            for (auto item1 : stepDataObj.edgeCurveGeometry[advFace1.first][edge1.first])
                            {
                                //cout << "ITEM2**********\n";
                            }
                        }
                    } // end edge1
                } // end edge
            }
        } // end advFace1
            
    } // end advFace
    */
    // identify a cyclindrical hole

    // Identify a square hole
    
}

void FeatureFinder::featureFinderController(STEP stepDataObj)
{
    cout << "Welcome to the Feature Finder\n";
    findMinMax(stepDataObj);
    STEP cubeObj; // Create Cube Objects
    cubeObj.stepController("Cube"); // Read cube and fill variables
    createCubeToFit(cubeObj, stepDataObj);
}
