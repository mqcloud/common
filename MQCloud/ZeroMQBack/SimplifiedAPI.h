#ifndef ZMQBackEndH
#define ZMQBackEndH
//#include <MQCloud/MQCloud.h>
#include <string>
#include <memory>
#include <functional>

//extern "C" {
//	extern const struct BackEnd ZMQBackEnd;
//}

class A {
	std::string name;
public:
	virtual void SetName(const std::string & n);

	virtual std::string SayHello();

	virtual ~A();
};


class B {
private:
	std::shared_ptr<A> p_ptr;
public:
	virtual void SetA(std::shared_ptr<A> p);
	virtual void SetSF(std::function<void(int)> f); // Ignored By SWIG!)

	virtual void CallA();

	virtual ~B();

};

#endif // ZMQBackEndH

