#include "UltraISOValidator.h"
#include <locale.h>
#include <iostream>
#include <string>
//#pragma comment(lib, "libeay32.lib")
//#pragma comment(lib, "ssleay32.lib")

int main()
{
	_wsetlocale(LC_ALL,L"");

	std::string regcode;
	std::string name;
	std::cout << "Registration Name: " << std::endl;
	while (std::getline(std::cin, name, '\n'))
	{
		std::cout << "Registration Code: " << std::endl;
		std::getline(std::cin, regcode, '\n');
		if(validate(name, regcode))
		{
			std::cout << "correct registration!" << std::endl;
		}
		else
		{
			std::cout << "wrong registration!" << std::endl;
		}
		std::cout << "\nRegistration Name: " << std::endl;
	}

    return 0;
}