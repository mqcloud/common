#include "SimplifiedAPI.h"
#include <iostream>

void A::SetName(const std::string & n) {
	name = n;
}

std::string A::SayHello() {
	std::cout << "C++ Hello! from C++ to " << name << std::endl;
	return "C++";
}

A::~A() {
	std::cout << "C++ Del A with " << name << std::endl;
}

void B::SetA(std::shared_ptr<A> p) {
	std::cout << "C++ Set A" << std::endl;
	p_ptr = p;
}

void B::SetSF(std::function<void(int)> f) {}

void B::CallA() {
	std::cout << "C++ Call A" << std::endl;
	p_ptr->SayHello();
}

B::~B() {
	auto s = p_ptr->SayHello();
	std::cout << "C++ Del B owner of " << s << std::endl;
}
