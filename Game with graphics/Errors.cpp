#include "Errors.h"

#include <iostream>

void error(std::string message, bool fatal) {
	std::cout << message << std::endl;
	system("PAUSE");

	if (fatal) {
		exit(1);
	}
	else {
		return;
	}
}