#include "PPM.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

ColorRGB::ColorRGB(int R, int G, int B){

}

ColorRGB::~ColorRGB(){}

void ColorRGB::setValues(int R, int G, int B){
    this->red = R;
    this->green = G;
    this->blue = B;
}

void ColorRGB::darken(){}

std::string ColorRGB::toString() {
    return std::to_string(this->red) + ", " + std::to_string(this->green) + ", " + std::to_string(this->blue);
}

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName){

    std::ifstream inFile;
    inFile.open(fileName);

    if(inFile.is_open()){
        std::string line;
        std::vector<std::string> lines;

        int index = 0;
        int lineNumber = 0;
        
        while(getline(inFile, line)){
            if(lineNumber == 2) {

                std::stringstream ss;

                ss.str(line);

                ss >> this->m_width;
                ss >> this->m_height;

                std::cout << this->m_width << std::endl;
                std::cout << this->m_height << std::endl;
                std::cout << this->m_width * this->m_height * 3 << std::endl;

            }
            else if(lineNumber > 2) {
                lines.push_back(line);
                ++index;
            }

            ++lineNumber;

        }

        this->generatePPMFromLines(lines);
        std::cout << lineNumber << std::endl;
    }

}

void PPM::generatePPMFromLines(std::vector<std::string> lines){
    int red = -1;
    int green = -1;
    int blue = -1;

    std::cout << "Generating RGB values from lines" << std::endl;

    ColorRGB myPixel(0, 0, 0);
    for(auto i : lines) {
        if(red < 0){
            red = stoi(i);
        }
        else if(green < 0){
            green = stoi(i);
        }
        else {
            blue = stoi(i);
            myPixel.setValues(red, green, blue);
            std::cout << myPixel.toString() << std::endl;
            red = -1;
            green = -1;
            blue = -1;

        }
    }
}

// Destructor clears any memory that has been allocated
PPM::~PPM(){
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName){
    // TODO: Save a PPM image to disk
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken(){
    // TODO: Output a 'filtered' PPM image.
}

// Sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, int R, int G, int B){
    // TODO: Implement
}