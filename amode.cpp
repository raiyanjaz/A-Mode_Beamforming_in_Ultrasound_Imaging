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
    float** RFData;
    RFData = new float*[numElement];
    for (int i = 0; i < numElement; i++)
        RFData[i] = new float[numSample];
    return RFData;
}

int loadRFData(float **RFData, const char *fileName, int numElement, int numSample) {
    // Open the text file fileName, read the data and store into RFData
    // You can use the getline() command to extract the data lines from the txt files
    ifstream infile(fileName); //Opens file

    if (infile.fail()) { //Checks if file is open
    	cerr << "Error: file not open.\n";
        return -1;
    }

    const int MAX = 100;
    char line[100];
    int elementCounter = 0;
    int sampleCounter = 0;

    while (infile.getline(line, MAX)) {
        RFData[elementCounter][sampleCounter] = atof(line);
        sampleCounter++;
        if (sampleCounter == numSample) { // Every 3338 samples, element counter is increased by 1.
            elementCounter++;
            sampleCounter = 0;
        }
        if (elementCounter == numElement) {
            break;
        }
    }
}

// Create an array containing the depth location (in z-direction) for each pixel on the scanline
float *genScanlineLocation(int &numPixel) {
    float depth;
    int numPixels;
    cout << "Enter scanline depth (>0): ";
    cin >> depth;
    cout << "Enter the number of pixels: ";
    cin >> numPixels;

    float scanlineLocation[numPixels];
    for (int i = 0; i < numPixels; i++) {
        scanlineLocation[i] = i*(depth/(numPixels - 1));
    }

    return scanlineLocation;
}

// Create an array containing the element location (in x-direction) of the ultrasound transducer
float *genElementLocation(int numElement, float PITCH) {
    /*float* eleLocation[128];
    int n = 0;
    n = (n - ((n-1)/2)) * 0.0003048;
    
    for (int i = 0)*/
}

// Allocate memory to store the beamformed scanline
float *createScanline(int numPixel)
{
    
}

// Beamform the A-mode scanline
void beamform(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel, float FS, float SoS)
{
    
}

// Write the scanline to a csv file
int outputScanline(const char *fileName, float *scanlinePosition, float *scanline, int numPixel)
{
    
}

// Destroy all the allocated memory
void destroyAllArrays(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel)
{
    
}