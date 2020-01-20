#include "PPM.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName){

    std::ifstream inFile;
    inFile.open(fileName);

    if(inFile.is_open()){
        int lineNumber = 0;
        std::string line;

        std::stringstream ss;
        std::string currentRGB;

        while(getline(inFile, line)){

            ss.str(line);
            
            while(ss >> currentRGB){
                
                if(lineNumber > 1) {
                    if(this->m_width == 0) {
                        this->m_width = stoi(currentRGB);
                    }
                    else if(this->m_height == 0) {
                        this->m_height = stoi(currentRGB);
                    }
                    else {
                        this->m_PixelData.push_back(stoi(currentRGB));
                    }
                }
            }

            ss.clear();
            
            ++lineNumber;
        }
    }
}

// Destructor clears any memory that has been allocated
PPM::~PPM(){
    // m_PixelData is deallocated automatically
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName){

    std::ofstream outFile;
    
    outFile.open(outputFileName);
    outFile << "P3" << std::endl;
    outFile << "# CREATOR: Assignment0 VER 1" << std::endl;
    outFile << this->m_width << " " << this->m_height << std::endl;

    for(auto i : this->m_PixelData) {
        outFile << i << std::endl;
    }

    outFile.close();
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken(){
    for (int i = 0; i < this->m_PixelData.size(); ++i) {
        if(this->m_PixelData[i] < 50) {
            this->m_PixelData[i] = 0;
        }
        else {
            this->m_PixelData[i] = this->m_PixelData[i] - 50;
        }
    }
}

// Sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, int R, int G, int B){
    int index = 3 * (y * this->m_width + x);
    int maxIndex = this->m_PixelData.size() - 3;

    if(index >  maxIndex) { 
        std::cerr << "Pixel index out of range" << std::endl;
    }
    else {
        this->m_PixelData[index] = R;
        this->m_PixelData[index + 1] = G;
        this->m_PixelData[index + 2] = B;

    }
}