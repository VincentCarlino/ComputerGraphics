// g++ vector.cpp -o vector
#include <iostream>
#include <vector>

// Prints all letters in the english alphabet
// A has a value of 65, B - 66, etc
int main(int argc, char** argv){

    std::vector<unsigned char> myData;
    
    for(unsigned int i = 65; i < 91; ++i){
        myData.push_back(i);
    }

    for(unsigned int i = 0; i < myData.size(); ++i){
        std::cout << myData[i] << std::endl;
    }

    return 0;
}