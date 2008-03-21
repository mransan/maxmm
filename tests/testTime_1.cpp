#include <iostream>

#include <maxutils/all.h>

using namespace maxutils;
int main()
{
	Time t;
	
	std::cout << "===========" << std::endl;
	std::cout << "testTime #1" << std::endl;
	std::cout << "===========" << std::endl;
	
	std::cout << std::endl;

	std::cout << "-> time addition:" << std::endl;
	t = Time::now();
	std::cout << "now:      " << t << std::endl;
	t += 1.5;
	std::cout << "now+=1.5: " << t << std::endl;
	
	std::cout << std::endl;
	
	std::cout << "-> time substraction" << std::endl;
	t = Time::now();
	std::cout << "now:      " << t << std::endl;
	t -= 1.5;
	std::cout << "now-=1.5: " << t << std::endl;

	return 0;	
}
