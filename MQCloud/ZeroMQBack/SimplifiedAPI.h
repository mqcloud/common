#ifndef ZMQBackEndH
#define ZMQBackEndH
//#include <MQCloud/MQCloud.h>
#include <string>
#include <memory>
#include <functional>
#include <iostream>

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
	virtual void SetSF(std::function<void(int)> f); // Ignored By SWIG yet crap is generated!(

	virtual void CallA();

	virtual ~B();

};

/*
#if _msc_ver >= 1700 || swig
struct _vs_emptytype { };

template<class rt = void, class t1 = _vs_emptytype, class t2 = _vs_emptytype, class t3 = _vs_emptytype>
struct genericfunc
#ifndef swig
		: std::function<rt (t1, t2, t3)>
#endif
{
	genericfunc() {}

#ifndef swig
	genericfunc(const std::function<rt (t1, t2, t3)> & action) : std::function<rt (t1, t2, t3)>(action) {}
#endif

	virtual rt onaction(t1 r1, t2 r2, t3 r3) {
		if(*this) {
			return (*this)(r1, r2, r3);
		}
	}

	virtual ~genericfunc() {}
};

template<typename rt, typename t1>
struct genericfunc<rt, t1, _vs_emptytype, _vs_emptytype>
#ifndef swig
		: std::function<rt (t1)>
#endif
{
	genericfunc() {}

#ifndef swig
	genericfunc(const std::function<rt (t1)> & action) : std::function<rt (t1)>(action) {}
#endif

	virtual rt onaction(t1 r1) {
		if(*this) {
			return (*this)(r1);
		}
	}

	virtual ~genericfunc() {}
};

template<typename rt, typename t1, typename t2>
struct genericfunc<rt, t1, t2, _vs_emptytype>
#ifndef swig
		: std::function<rt (t1, t2)>
#endif
{
	genericfunc() {}

#ifndef swig
	genericfunc(const std::function<rt (t1, t2)> & action) : std::function<rt (t1, t2)>(action) {}
#endif

	virtual rt onaction(t1 r1, t2 r2) {
		if(*this) {
			return (*this)(r1, r2);
		}
	}

	virtual ~genericfunc() {}
};


template<class t1 = _vs_emptytype, class t2 = _vs_emptytype, class t3 = _vs_emptytype>
struct genericaction
#ifndef swig
		: std::function<void (t1, t2, t3)>
#endif
{
	genericaction() {}

#ifndef swig
	genericaction(const std::function<void (t1, t2, t3)> & action) : std::function<void (t1, t2, t3)>(action) {}
#endif

	virtual void onaction(t1 r1, t2 r2, t3 r3) {
		if(*this) {
			(*this)(r1, r2, r3);
		}
	}

	virtual ~genericaction() {}
};

template<typename T1>
struct GenericAction<T1, _VS_EmptyType, _VS_EmptyType>
#ifndef SWIG
		: std::function<void (T1)>
#endif
{
	GenericAction() {}

#ifndef SWIG
	GenericAction(const std::function<void (T1)> & Action) : std::function<void (T1)>(Action) {}
#endif

	virtual void OnAction(T1 r1) {
		if(*this) {
			(*this)(r1);
		}
	}

	virtual ~GenericAction() {}
};

template<typename T1, typename T2>
struct GenericAction<T1, T2, _VS_EmptyType>
#ifndef SWIG
		: std::function<void (T1, T2)>
#endif
{
	GenericAction() {}

#ifndef SWIG
	GenericAction(const std::function<void (T1, T2)> & Action) : std::function<void (T1, T2)>(Action) {}
#endif

	virtual void OnAction(T1 r1, T2 r2) {
		if(*this) {
			(*this)(r1, r2);
		}
	}

	virtual ~GenericAction() {}
};

#else

template <class TR = void, class ... Types>
struct GenericFunc 
#ifndef SWIG
	: std::function<TR (Types...)>
#endif
{
	GenericFunc() {}

#ifndef SWIG
	GenericFunc(const std::function<TR (Types... t)> & Action) : std::function<TR (Types... t)>(Action) {}
#endif
	virtual TR OnAction(Types ... result) {
		if(*this) {
			return (*this)(result...);
		}
	}
	virtual ~GenericFunc(){}
};

template<class... Types> using GenericAction = GenericFunc<void, Types...>;
#endif

typedef GenericAction<B> BAction;
*/
#endif // ZMQBackEndH

