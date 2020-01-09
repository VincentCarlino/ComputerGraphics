// compile with: g++ class.cpp -o class

class PPM{
public: 

    // Constructor
    PPM(unsigned int width, unsigned int height){
        // Allocate memory
        pixelData = new unsigned char[width*height*3];
    }
    // Destructor
    ~PPM(){
        // Reclaim memory when we are done.
        delete[] pixelData;
    }
    
private:    
    unsigned char* pixelData;

};


int main(){

    PPM myPPM(64,128);

    return 0;
}