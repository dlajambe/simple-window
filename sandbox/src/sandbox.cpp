#include <iostream>
#include "simple_window.h"

void draw() {
	std::cout << "Welcome to Sandbox!" << std::endl;
	simple_window::create_window();
}

int main()
{
	try {
		draw();
	}
	catch (std::exception ex) {
		std::cerr << "Exception encountered: " << ex.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unhandled exception" << std::endl;
	}
	
}