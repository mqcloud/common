#include <vector>
#include <iostream>

int main()
{
	std::vector<int> a(100, 0);
	for (auto&& e : a)
	{
		e+=10;
	}

	for (auto&& e : a)
	{
		std::cout << e;
	}
	std::cin.get();

	return 0;
}