#include "PPM.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

// ColorRGB::ColorRGB(int R, int G, int B){
//     this->red = R;
//     this->green = G;
//     this->blue = B;
// }

// ColorRGB::~ColorRGB(){}

// void ColorRGB::setValues(int R, int G, int B){
//     this->red = R;
//     this->green = G;
//     this->blue = B;
// }

// void ColorRGB::darken(){}

// std::string ColorRGB::toString() {
//     return std::to_string(this->red) + ", " + std::to_string(this->green) + ", " + std::to_string(this->blue);
// }

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName){

    std::ifstream inFile;
    inFile.open(fileName);

    if(inFile.is_open()){
        int lineNumber = 0;
        std::string line;
        std::vector<std::string> lines;

        std::stringstream ss;
        std::string currentRGB;

        while(getline(inFile, line)){
            if(lineNumber == 2) {


                ss.str(line);

                ss >> this->m_width;
                ss >> this->m_height;


                std::cout << this->m_width << std::endl;
                std::cout << this->m_height << std::endl;
                std::cout << this->m_width * this->m_height * 3 << std::endl;
                
                ss.clear();
            }
            else if(lineNumber > 2) {

                // I was having difficulties piping a string stream into an unsigned character so as an alternative, 
                // I'm using integers to store the RGB values.
                ss.str(line);
                
                while(ss >> currentRGB){
                    this->m_PixValues.push_back(stoi(currentRGB));
                }
                ss.clear();
            }
            ++lineNumber;

        }
    }

}

void PPM::generatePPMFromLines(std::vector<std::string> lines){
    int red = -1;
    int green = -1;
    int blue = -1;

    std::cout << "Generating RGB values from lines ===============================>  " << std::endl;

    std::cout << "First value to read: " << lines[0] << std::endl;
    std::cout << "Last value to read: " << lines.back() << std::endl;

    std::cout << "Size of lines vector: " << lines.size() << std::endl;

    bool wrong = false;

    int pix = 0;

    for(auto i : lines) {

        if(red < 0){
            red = stoi(i);
        }
        else if(green < 0){
            green = stoi(i);
        }
        else {
            blue = stoi(i);
            ColorRGB myPixel(red, green, blue);

            if((red < 100 && !wrong) || pix == 55510) {
                std::cout << "Pixel " << pix << ": " << myPixel.toString() << std::endl;
                wrong = true;
            }

            this->m_PixelData.push_back(myPixel);
            red = -1;
            green = -1;
            blue = -1;

            ++pix;
        }
    }

    std::cout << "Pixel count: " << pix << std::endl;

    // for(auto i = this->m_PixelData.begin(); i != this->m_PixelData.end(); ++i) {
    //     std::cout << i->toString() << std::endl;
    // }
}

// Destructor clears any memory that has been allocated
PPM::~PPM(){
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName){

    std::ofstream outFile;
    
    outFile.open(outputFileName);
    outFile << "P3" << std::endl;
    outFile << "# CREATOR: Assignment0 VER 1" << std::endl;
    outFile << this->m_width << " " << this->m_height << std::endl;

    for(auto i : this->m_PixValues) {
        outFile << i << std::endl;
    }

    outFile.close();
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken(){

    for (int i = 0; i < this->m_PixValues.size(); ++i) {
        if(this->m_PixValues[i] < 50) {
            this->m_PixValues[i] = 0;
        }
        else {
            this->m_PixValues[i] = this->m_PixValues[i] - 50;
        }
    }
}

// Sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, int R, int G, int B){
    // TODO: Implement

    // Use x and y to find the correct pixel in the RGB vector
    // Use Pixel.setValues(R, G, B) to set the value of the pixel
}