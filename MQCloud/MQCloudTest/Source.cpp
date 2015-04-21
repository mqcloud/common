#include <vector>
#include <iostream>
#include <thread>

struct Demo {
	Demo() {
		std::cout << "hello" << std::endl;
		member = 1;
	}

	int member;

	~Demo() {
		std::cout << "but" << std::endl;
	}
};


void ThreadedExecutor(const int & data, void (*callback)() ) {
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "waited" << std::endl;
	callback();
}

void handler(Demo && a) {
	
}

int main()
{
	{
		Demo a;
		std::function<void()> f = std::bind( [] (Demo&& p)
		{
			ThreadedExecutor(p.member, f);

		}, std::move(a) );
		
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}