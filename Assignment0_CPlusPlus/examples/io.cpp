// Compile with:
// g++ io.cpp -o io
#include <iostream> // The standard input/output library

int main(){

	std::cout << "Hello" << std::endl;
	std::cout << "How old are you?" << std::endl;
	
	int age;
	
	std::cin >> age;
	
	std::cout << "That is great you are " << age << "!" << std::endl;

	return 0;
}