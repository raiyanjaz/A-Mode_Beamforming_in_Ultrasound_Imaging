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
    int pixels;
    cout << "Enter scanline depth (>0): ";
    cin >> depth;
    cout << "Enter the number of pixels: ";
    cin >> pixels;

    float scanlineLocation[pixels];
    for (int i = 0; i < pixels; i++) 
        scanlineLocation[i] = i*(depth/(pixels - 1));
    return scanlineLocation;
}

// Create an array containing the element location (in x-direction) of the ultrasound transducer
float *genElementLocation(int numElement, float PITCH) {
    float eleLocation[numElement];
    for (int n = 0; n <= numElement-1; n++) 
        eleLocation[n] = (n - ((numElement-1)/2)) * PITCH;
}

// Allocate memory to store the beamformed scanline
float *createScanline(int numPixel) {
    float* scanline;
    scanline = new float[numPixel];
    return scanline;
}

// Beamform the A-mode scanline
void beamform(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, 
float *elementPosition, int numElement, int numSample, int numPixel, float FS, float SoS) {
    
    float tForward[numPixel];
    float tBackward[numPixel][numElement];
    float tTotal[numPixel][numElement];
    int sampleS[numPixel][numElement];

    for (int i = 0; i < numPixel; i++) {
        tForward[i] = (scanlinePosition[i] / SoS);
        for (int k = 0; k < numElement; k++) {
            tBackward[i][k] = (sqrt(pow(scanlinePosition[i], 2) + pow(elementPosition[i], 2)) / SoS);
            tTotal[i][k] = tForward[i] + tBackward[i][k];
            sampleS[i][k] = floor(tTotal[i][k] * FS);
        }
    }

    float pReal[numPixel];
    float pImag[numPixel];
    int sum = 0;

    for (int i = 0; i < numPixel; i++) {
        for (int k = 0; k < numElement; k++) {
            int* test = *(sampleS+i)+k;
            sum += realRFData[k][*test];
        }
        pReal[i] = sum;
        sum = 0;
    }

    for (int i = 0; i < numPixel; i++) {
        for (int k = 0; k < numElement; k++) {
            int* test = *(sampleS+i)+k;
            sum += imagRFData[k][*test];
        }
        pImag[i] = sum;
        sum = 0;
    }
    
}

// Write the scanline to a csv file
int outputScanline(const char *fileName, float *scanlinePosition, float *scanline, int numPixel) {
    
}

// Destroy all the allocated memory
void destroyAllArrays(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel) {
    
}