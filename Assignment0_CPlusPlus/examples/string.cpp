// Compile with:
// g++ string.cpp -o string
#include <iostream>
#include <string>

int main(){

    std::string fullName = "Vincent Carlino";

    std::string firstName = fullName.substr(0,7);
    std::string lastName = fullName.substr(8,14);

    std::cout << "First: " << firstName << std::endl;
    std::cout << "Last: " << lastName << std::endl;

    unsigned int i = 0;
    while(i < fullName.length()){
        if(fullName.at(i)==' '){
            // Terminate from our loop
            break;
        }
        std::cout << fullName[i] << std::endl;
        ++i;
    }


    return 0;
}