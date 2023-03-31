#define _CRT_SECURE_NO_WARNINGS 1
#include "../complex/Complex.h"
#include <cmath>
#include <iostream>

void Complex::_modulus()
{
	modulus= sqrt(real * real + image * image); 
}

void Complex::Print()
{
	if (image == 0.0)   //�鲿Ϊ0
	{
		std::cout << real << std::endl;
	}
	else
	{
		std::cout << real << "+" << image << "*i" << std::endl;
	}
}

