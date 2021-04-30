# Author
Alan Halpin
# Project Description
The STEP File Converter is a tool for CNC engineers, designed to automate a lengthy and costly process. Nothing like this application currently exists. The value is derived from the process being automated. This process involves performing vibration analysis on different permutations of an object until a satisfactory sequence of faces is found. Vibration analysis is performed to ensure the piece worked on remains stable throughout the cutting process. The proposed tool will extract geometrical data from a STEP file and catalogue the different faces of the object described. One by one these faces will be subtracted from a solid block shape. Vibration analysis is performed between each step, if the piece does not pass the requirements then a different face will be used. This will eventually create a list of faces to cut in order. Ex. A,B,F,G,C might be an order in which to carve first. Although vibration analysis already exists within various CAD applications, the process described only exists in a manual implementation. This tool will use the Solid Works API to outsource the calculations and identify the optimal cutting order within the application. An engineer will pass a STEP file to the program and the program will start working on the results. Depending on the parts complexity this process may take some time. Typically this process is done manually and can take up to three weeks to complete, this tool hopes to automate that process and reduce that time down to one day. 

# License
This work is licensed under the  Creative Commons Attribution-ShareAlike 4.0 International Licence. To view of this licence, visit http://creativecommons.org/licenses/by-sa/4.0/.

# Directions
 Ensure the STEPFILES folder and WriteTests folder are located in the same directory as STEPFILE-Project.exe.
<br>This application is only compatiable with STEP files of protocol AP203.
<br> Ensure object files are saved in the .STEP format.