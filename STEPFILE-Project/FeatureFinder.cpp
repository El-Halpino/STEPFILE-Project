// This class will be used to identify high level features

// find min max of cartesian points
// create cube that fits those min / max
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
        cout << "\n" << "Key: " << key.first << " Results\n\n";
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
                            placeHolder = strtod(char_arr, &end);
                            switch (count) 
                            {
                            case 0: // X
                                if (count == 0 && placeHolder < minX)
                                {
                                    minX = placeHolder;
                                }
                                else if (count == 0 && placeHolder > maxX)
                                {
                                    maxX = placeHolder;
                                }
                                break;
                            case 1: // Y
                                if (count == 1 && placeHolder < minY) 
                                {
                                    minY = placeHolder;
                                }
                                else if (count == 1 && placeHolder > maxY) 
                                {
                                    maxY = placeHolder;
                                }
                                break;
                            case 2: // Z
                                if (count == 2 && placeHolder < minZ) 
                                {
                                    minZ = placeHolder;
                                }
                                else if (count == 2 && placeHolder > maxZ) 
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

void FeatureFinder::featureFinderController(STEP stepDataObj)
{
    cout << "Welcome to the Feature Finder\n";
	findMinMax(stepDataObj);
}
