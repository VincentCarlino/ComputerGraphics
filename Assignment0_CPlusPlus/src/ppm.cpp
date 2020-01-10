#include "PPM.h"

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName){

    std::ifstream inFile;

    inFile.open(fileName);

    if(inFile.is_open()){

        std::string line;
        unsigned int lineNumber = 0;
        
        while(getline(inFile, line)){
            std::cout << line << std::endl;
            // if line number is 0 or 1, skip
            // if line number is 2, store x and y values (total length SHOULD be x * y * 3 for RGB values in each pixel)

            // Split lines by spaces (shouldn't be necessary but maybe include this)
            // store each value in PPM char array
        }
    }
    // TODO:    Load and parse a ppm to get its pixel
    //          data stored properly.
}

// Destructor clears any memory that has been allocated
PPM::~PPM(){
    // delete pixel data and stored height/width
    // delete[] m_PixelData;
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName){

    std::ofstream outFile;

    outFile.open(outFileName);

    // Write P3 and comment to out file
    /////////

    unsigned int i = 0;
    while(i < m_PixelData.length()) {
        //Append each pixel data value as a new line
        outFile << m_PixelData[i] << std::endl;
    }

    outFile.close()

}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken(){

    // TODO: Output a 'filtered' PPM image.
    unsigned int i = 0;
    while(i < m_PixelData.length()) {
        if(m_PixelData <= 50) {
            m_PixelData[i] = 0;
        }
        else {
            m_PixelData[i] = m_PixelData[i] - 50;
        }
    }
}

// Sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, int R, int G, int B){
    // TODO: Implement
}


/* 
3 * 8

0 0 0    0 0 0    0 0 0 
0 0 0    0 0 0    0 0 0 
0 0 0    0 0 0    0 0 0 
0 0 0    0 0 0    0 0 0 
0 0 0    0 0 0    0 0 0 
0 0 0    0 0 0    0 0 0 
0 0 0    0 0 0    0 0 0 
0 0 0    0 0 0    0 0 0 

R Values have 0 after i modulo 3
G Values have 1 after i modulo 3
B Values have 2 after i modulo 3

*/
