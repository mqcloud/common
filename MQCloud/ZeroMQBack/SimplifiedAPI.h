#ifndef ZMQBackEndH
#define ZMQBackEndH
#include <MQCloud/MQCloud.h>

extern "C" {
	extern const struct BackEnd ZMQBackEnd;
}

class A {
public:
	CoreThreadManagementInterface * ThreadingInterface;
};


/*
class B {
public:
	A * GetA_ptr() {return new A();}
	A GetA_copy() {return A();}
	void DestroyA(A* a) {
		delete a;
	}
	A * aaa_ptr;
	A aaa_raw;
};
*/
#endif // ZMQBackEndH

