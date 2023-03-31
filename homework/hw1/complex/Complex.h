#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#ifndef __COMPLEX_H__
#define __COMPLEX_H__
 
class Complex
{
public:

	void _modulus();
	void Print();

	double real;
	double image;
	double modulus;
};
 
#endif //__COMPLEX_H__