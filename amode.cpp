#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;

float **createDataMatrix(int numElement, int numSample) {
    // Create a 2D data matrix of size numElement and numSample

    float** RFData; // Declare a double pointer variable for RFData
    RFData = new float*[numElement]; // Allocates RFData as an array (numElement length)
    for (int i = 0; i < numElement; i++)
        RFData[i] = new float[numSample]; // For every element, allocates a numElement number of blocks to each element
    return RFData;
}

int loadRFData(float **RFData, const char *fileName, int numElement, int numSample) {
    // Open the text file fileName, read the data and store into RFData
    // You can use the getline() command to extract the data lines from the txt files

    ifstream fin(fileName); //Opens file

    if (fin.fail()) // Returns -1 to main if file cannot be open
        return -1;

    const int MAX = 100;
    char line[100];
    int elementCounter = 0; // Counters to be used in the while loop
    int sampleCounter = 0;
   
    while (fin.getline(line, MAX)) { // Runs until end of file (EOF) returns false
        RFData[elementCounter][sampleCounter] = atof(line); // Converts char array to float and stores in RFData
        sampleCounter++;
        if (sampleCounter == numSample) { // Every 3338 samples, element counter is increased by 1.
            elementCounter++;
            sampleCounter = 0;
        }
    }

    fin.close(); // Closes file
}

float *genScanlineLocation(int &numPixel) {
    // Create an array containing the depth location (in z-direction) for each pixel on the scanline

    // Prompt user to input values for depth and numPixels
    float depth;
    cout << "Enter scanline depth (>0): ";
    cin >> depth;
    cout << "Enter the number of pixels: ";
    cin >> numPixel;

    float* scanlineLocation; 
    scanlineLocation = new float[numPixel]; // Allocate array of type float with size numElement
    for (int i = 0; i < numPixel; i++) 
        scanlineLocation[i] = i*(depth/(numPixel - 1)); // Desired values are stored in each element
    return scanlineLocation; // Return base address of array to main
}

float *genElementLocation(int numElement, float PITCH) {
    // Create an array containing the element location (in x-direction) of the ultrasound transducer

    float* eleLocation; 
    eleLocation = new float[numElement]; // Allocate array of type float with size numElement
    for (int n = 0; n <= numElement-1; n++) 
        eleLocation[n] = (n - ((numElement-1)/2)) * PITCH; // Desired values are stored in each element
    return eleLocation; // Return base address of array to main
}

float *createScanline(int numPixel) {
    // Allocate memory to store the beamformed scanline
    float* scanline;
    scanline = new float[numPixel]; // Allocates an block of elements with size numPixel of type float
    return scanline;
}

void beamform(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel, float FS, float SoS) {
    // Beamform the A-mode scanline
    
    // Declare variables to be used later
    float tForward;
    float tBackward;
    float tTotal[numPixel][numElement];
    int sampleS[numPixel][numElement];
    float pReal[numPixel];
    float pImag[numPixel];

    for (int i = 0; i < numPixel; i++) {
        tForward = scanlinePosition[i] / SoS; // Each element in scanlinePositon is divided by speed of sound and stored
        pReal[i] = 0;
        pImag[i] = 0;
        for (int k = 0; k < numElement; k++) {
            tBackward = sqrt(pow(scanlinePosition[i], 2) + pow(elementPosition[k], 2)) / SoS;
            tTotal[i][k] = tForward + tBackward;
            sampleS[i][k] = floor(tTotal[i][k] * FS); // Turns every element in tTotal into an integer to be stored in sampleS
            pReal[i] += realRFData[k][sampleS[i][k]]; // Values in both arrays are added and stored into pReal and pImag
            pImag[i] += imagRFData[k][sampleS[i][k]];
        }
        scanline[i] = sqrt(pow(pReal[i], 2) + pow(pImag[i] , 2)); // Stores the echo magnitude at ith scanline location
    } 
}

int outputScanline(const char *fileName, float *scanlinePosition, float *scanline, int numPixel) {
    // Write the scanline to a csv file

    ofstream fout; // Creates file that is named in main
    fout.open(fileName);

    if (!fout.good()) // Returns -1 to main if file was not created
        return -1;
    
    for (int i = 0; i < numPixel; i++)
        fout << scanlinePosition[i] << "," << scanline[i] << endl; // Outputs elements of scanlinePositon and scanline
    fout.close(); // Closes output file

    return 0;
}

void destroyAllArrays(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel) {
    // Destroy all the allocated memory
    
    for (int i = 0; i < numElement; i++) { // For loop to free the memory of all arrays within the 2D arrays
        delete realRFData[i];
        delete imagRFData[i];
    }
    // Frees the memory stored by the 1D arrays below
    delete scanline;
    delete scanlinePosition;
    delete elementPosition;
}