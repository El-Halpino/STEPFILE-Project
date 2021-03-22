// This class will be used to identify high level features

// create cube that fits those min / max // VERTEX_POINT,  squares have 4 vertexes which each point at a cart point (XYZ)
// make list of faces that touch

#include <iostream>
#include <iomanip>
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

    for (auto key : stepDataObj.stepFeatureList) // cycles through each feature
    {
        //cout << "\n" << "Key: " << key.first << " Results\n\n";
        for (auto it = key.second.begin(); it != key.second.end(); ++it) // Cycles through each sub feature
        {
            currentLine = *it;
            if (currentLine.find(" CARTESIAN_POINT ") != string::npos)
            {
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


void FeatureFinder::createCubeToFit(FeatureFinder mainObj)
{ // goal is to replace max values with new max values
    STEP cubeObj;
    FeatureFinder cubeFinder;

    cubeObj.stepController("Cube");
    cubeFinder.findMinMax(cubeObj);
    map<string, vector<string>> features = cubeObj.stepFeatureList;
    bool numberFound = false;
    string number = "";
    string concat = "";
    long double placeHolder;
    int count = 0;
    char start;
    char end;
    string maxX = to_string(mainObj.maxX);
    string maxY = to_string(mainObj.maxY);
    string maxZ = to_string(mainObj.maxZ);
    string minX = to_string(mainObj.minX);
    string minY = to_string(mainObj.minY);
    string minZ = to_string(mainObj.minZ);
    // #796 = CARTESIAN_POINT ( 'NONE',  ( 28.20906519726944239, 20.00000000000000000, 13.79214587795902425 ) ) ;

    for (auto item : cubeObj.vertexPoints)
    { 
        cout << item.first << "\n";
        for (auto item2 : item.second)
        {
            string step = item2.substr(0, item2.find("=") - 1);
            string subnLine = item2.substr(item2.find("="), item2.size());
            for (char& ch : subnLine) // Cycles through each character in item2
            {
                if (numberFound == true)
                {
                    if (ch == ',' || ch == ' ') // These charcters signal the end of the current number
                    {
                        end = ch;
                        int end1 = subnLine.find(ch);
                        char* char_arr;
                        char* end;
                        char_arr = &number[0];
                        placeHolder = strtod(char_arr, &end); // string to double
                      
                        switch (count)
                        {
                            case 0: // X
                                if (placeHolder == cubeFinder.minX)
                                {
                                    cout << "BEFORE: " << item2 << "\n";
                                    item2.replace(item2.find(start, 34), number.size(), minX);
                                    cout << "CHANGED: " << item2 << "\n";
                                }
                                else if (placeHolder == cubeFinder.maxX)
                                {
                                    
                                }
                                break;
                            case 1: // Y
                                if (placeHolder == cubeFinder.minY)
                                {
                                    
                                }
                                else if (placeHolder == cubeFinder.maxY)
                                {
                                    
                                }
                                break;
                            case 2: // Z
                                if (placeHolder == cubeFinder.minZ)
                                {
                                   
                                }
                                else if (placeHolder == cubeFinder.maxZ)
                                {
                                    
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
        }
    }
}

void FeatureFinder::featureFinderController(STEP stepDataObj)
{
    cout << "Welcome to the Feature Finder\n";
    FeatureFinder mainObj;
    mainObj.findMinMax(stepDataObj);
    mainObj.facesThatTouch(stepDataObj);
    createCubeToFit(mainObj);
}
