#include<iostream>
#include"JSONCheckKit.h"
int main(void)
{
	std::string jstr;
	std::getline(std::cin,jstr);
	JSONCheckKit c(jstr);
	c.JSONCheck();
	return 0;
}