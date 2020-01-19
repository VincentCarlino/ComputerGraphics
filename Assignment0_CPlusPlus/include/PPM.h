/** @file PPM.h
 *  @brief Class for working with PPM images
 *  
 *  Class for working with P3 PPM images specifically.
 *
 *  @author Vincent Carlino
 *  @bug No known bugs.
 */
#ifndef PPM_H
#define PPM_H

#include <string>
#include <vector>

class ColorRGB{
public:
    // Constructor takes in RGB values
    ColorRGB(int R, int G, int B);
    // Destructor clears memory that has been allocated
    ~ColorRGB();
    // Sets the RGB values of this pixel
    void setValues(int R, int G, int B);
    // Subtracts 50 from each of the RGB components of this pixel.
    void darken();
    // Prints this RGB Pixel
    std::string toString();

    inline int getRed() { return red; }
    inline int getGreen() { return green; }
    inline int getBlue() { return blue; }

private:
    int red{0};
    int green{0};
    int blue{0};
};

class PPM{
public:
    // Constructor loads a filename with the .ppm extension
    PPM(std::string fileName);
    // Destructor clears any memory that has been allocated
    ~PPM();
    // Saves a PPM Image to a new file.
    void savePPM(std::string outputFileName);
    // Darken subtracts 50 from each of the red, green
    // and blue color components of all of the pixels
    // in the PPM. Note that no values may be less than
    // 0 in a ppm.
    void darken();
    // Sets a pixel to a specific R,G,B value 
    void setPixel(int x, int y, int R, int G, int B);
    // Returns the raw pixel data in an array.
    // You may research what 'inline' does.
    inline std::vector<ColorRGB> pixelData(){ return m_PixelData; }
    // Returns image width
    inline int getWidth(){ return m_width; }
    // Returns image height
    inline int getHeight(){ return m_height; }
// NOTE:    You may add any helper functions you like in the
//          private section.
private:    
    // Store the raw pixel data here
    // Data is R,G,B format
    std::vector<ColorRGB> m_PixelData;

    std::vector<int> m_PixValues;
    // Store width and height of image.
    int m_width{0};
    int m_height{0};

    void generatePPMFromLines(std::vector<std::string> lines);
};


#endif
