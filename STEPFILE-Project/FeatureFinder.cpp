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
    ofstream TestFile("WriteTests/cubextended.step");
    
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

bool isEqual(string pointOne , string pointTwo)
{
    string oneX, oneY, oneZ;
    string twoX, twoY, twoZ;
    string number;
    int count = 0;
    bool numberFound = false;
    for (auto ch : pointOne)
    {
        if (numberFound == true)
        {
            if (ch == ',' || ch == ' ')
            {
                switch (count)
                {
                case 0:
                    oneX = number;
                    break;
                case 1:
                    oneY = number;
                    break;
                case 2:
                    oneZ = number;
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
        }
    }
    count = 0;
    for (auto ch : pointTwo)
    {
        if (numberFound == true)
        {
            if (ch == ',' || ch == ' ')
            {
                switch (count)
                {
                case 0:
                    twoX = number;
                    break;
                case 1:
                    twoY = number;
                    break;
                case 2:
                    twoZ = number;
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
        }
    }
    if (oneX.substr(0, 15) == twoX.substr(0, 15))
    {
        if (oneY.substr(0, 15) == twoY.substr(0, 15))
        {
            if (oneZ.substr(0, 15) == twoZ.substr(0, 15))
            {
                return true;
            }
        }
    }
    return false;
}

void write(map<int, set<string>> highLevelFeatures, STEP stepDataObj)
{
    vector<string> header = stepDataObj.headerLines;
    set<string> compileLines = stepDataObj.diffLines;
    set<string> featureLines;
    // string FilePath = ("C:\\Users\\alanh\\source\\repos\\STEPFILE-Project\\WriteTests\\" + key.first + ".step");
    // ofstream AdvFace(FilePath.c_str());
    for (auto key : highLevelFeatures)
    {
        string name = to_string(key.first);
        string FilePath = ("WriteTests/Object" + name + ".step");
        ofstream TestFile(FilePath.c_str());
        for (auto item : key.second)
        {
            for (auto item2 : stepDataObj.stepFeatureList[item])
            {
                featureLines.insert(item2);
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
        featureLines.clear();
    }
}


void FeatureFinder::identifyHighLevelFeatures(STEP stepDataObj, STEP cubeObj) 
{
    // First, compare with the cube created to see what points are not shared.
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
                                    subLine1 = vPoint2.substr(vPoint2.find("("), vPoint2.size());
                                    if (isEqual(subLine, subLine1))
                                    {
                                        found = true;
                                    }
                                    else 
                                    {
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
   
    cout << "\n\n\nPoints not shared: " << linesNotShared.size() << "\n";
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
    int pointCount = 0;
    set <string> features;
    // check if all of faces vPoints can be found in Points not shared
    for (auto advFace : points)
    {
        for (auto vPoint : stepDataObj.vertexPoints[advFace.first])
        {
            subLine = vPoint.substr(0, vPoint.find(" "));
            for (auto vertex : linesNotShared)
            {
                subLine1 = vertex.substr(0, vertex.find(" "));
                if (subLine == subLine1)
                {
                    pointCount++;
                }
                if (pointCount == stepDataObj.vertexPoints[advFace.first].size())
                {
                    features.insert(advFace.first);
                    cout << advFace.first << "\n";
                    goto NextPoint;
                }
            }
        }
    NextPoint:
        pointCount = 0;
        continue;
    }
    // check if advFaces found have faces that touch in common
    set<string> features2 = features;
    set<string> toremove;
    map<int, set<string>> highLevelFeatures;
    int objectNo = 0;

    for (auto face : features)
    {
        if (find(toremove.begin(), toremove.end(), face) != toremove.end()) // ifthe current face has already been inserted
        {
            continue;
        } 
        else // if it hasn't, insert it now
        {
            highLevelFeatures[objectNo].insert(face);
            toremove.insert(face);
        }
        if (features2.empty()) // if there are no more features to search from
        {
            continue;
        }
        for (auto face2 : features2)
        {
            if (face != face2) // don't compare the same face
            {
                // check if face is touching face2
                if (find(stepDataObj.touchingFaces[face].begin(), stepDataObj.touchingFaces[face].end(), face2) != stepDataObj.touchingFaces[face].end())
                {
                    highLevelFeatures[objectNo].insert(face2);
                    toremove.insert(face2);
                }
                // check if face and face2 have a common face (that is listed in features)
                else
                {
                    for (auto common : stepDataObj.touchingFaces[face]) // check which faces touch face
                    {
                        for (auto common2 : stepDataObj.touchingFaces[face2])
                        {
                            if (common == common2 && find(features.begin(), features.end(), common) != features.end())
                            {
                                highLevelFeatures[objectNo].insert(face2);
                                toremove.insert(face2);
                            }
                        }
                    }
                }
            }
        }
        for (auto item : toremove)
        {
            features2.erase(item);
        }
        //toremove.clear();
        objectNo++;
    }
    for (auto item : highLevelFeatures) // print 
    {
        cout << "Object: " << item.first << "\n";
        for (auto item2 : item.second)
        {
            cout << item2 << "\n";
        }
    }
    if (highLevelFeatures.empty())
    {
        cout << "No Features found\n";
        return;
    }
    else {
        write(highLevelFeatures, stepDataObj);
    }
    
}

void FeatureFinder::featureFinderController(STEP stepDataObj)
{
    cout << "Welcome to the Feature Finder\n";
    findMinMax(stepDataObj);
    STEP cubeObj; // Create Cube Objects
    cubeObj.stepController("Cube"); // Read cube and fill variables
    createCubeToFit(cubeObj, stepDataObj);
}
