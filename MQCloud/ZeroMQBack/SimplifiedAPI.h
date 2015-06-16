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


#if _MSC_VER >= 1700 || SWIG
struct _VS_EmptyType { };

template<class RT = void, class T1 = _VS_EmptyType, class T2 = _VS_EmptyType, class T3 = _VS_EmptyType>
struct GenericFunc
#ifndef SWIG
		: std::function<RT (T1, T2, T3)>
#endif
{
	GenericFunc() {}

#ifndef SWIG
	GenericFunc(const std::function<RT (T1, T2, T3)> & Action) : std::function<RT (T1, T2, T3)>(Action) {}
#endif

	virtual RT OnAction(T1 r1, T2 r2, T3 r3) {
		if(*this) {
			return (*this)(r1, r2, r3);
		}
	}

	virtual ~GenericFunc() {}
};

template<typename RT, typename T1>
struct GenericFunc<RT, T1, _VS_EmptyType, _VS_EmptyType>
#ifndef SWIG
		: std::function<RT (T1)>
#endif
{
	GenericFunc() {}

#ifndef SWIG
	GenericFunc(const std::function<RT (T1)> & Action) : std::function<RT (T1)>(Action) {}
#endif

	virtual RT OnAction(T1 r1) {
		if(*this) {
			return (*this)(r1);
		}
	}

	virtual ~GenericFunc() {}
};

template<typename RT, typename T1, typename T2>
struct GenericFunc<RT, T1, T2, _VS_EmptyType>
#ifndef SWIG
		: std::function<RT (T1, T2)>
#endif
{
	GenericFunc() {}

#ifndef SWIG
	GenericFunc(const std::function<RT (T1, T2)> & Action) : std::function<RT (T1, T2)>(Action) {}
#endif

	virtual RT OnAction(T1 r1, T2 r2) {
		if(*this) {
			return (*this)(r1, r2);
		}
	}

	virtual ~GenericFunc() {}
};


template<class T1 = _VS_EmptyType, class T2 = _VS_EmptyType, class T3 = _VS_EmptyType>
struct GenericAction
#ifndef SWIG
		: std::function<void (T1, T2, T3)>
#endif
{
	GenericAction() {}

#ifndef SWIG
	GenericAction(const std::function<void (T1, T2, T3)> & Action) : std::function<void (T1, T2, T3)>(Action) {}
#endif

	virtual void OnAction(T1 r1, T2 r2, T3 r3) {
		if(*this) {
			(*this)(r1, r2, r3);
		}
	}

	virtual ~GenericAction() {}
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

#endif // ZMQBackEndH

